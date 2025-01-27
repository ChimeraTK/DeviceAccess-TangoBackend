// SPDX-FileCopyrightText: Deutsches Elektronen-Synchrotron DESY, MSK, ChimeraTK Project <chimeratk-support@desy.de>
// SPDX-License-Identifier: LGPL-3.0-or-later

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE testUnifiedBackendTest

#include "TangoTestServer.h"
#include <tango/tango.h>

#include <ChimeraTK/UnifiedBackendTest.h>

#include <boost/filesystem.hpp>

#include <chrono>
#include <filesystem>
#include <random>

#define BOOST_NO_EXCEPTIONS
#include <boost/test/unit_test.hpp>
#undef BOOST_NO_EXCEPTIONS

using namespace boost::unit_test_framework;
using namespace ChimeraTK;

struct ThreadedTangoServer {
  explicit ThreadedTangoServer(std::string setTestName = "", bool setVerbose = false);
  ~ThreadedTangoServer();

  /*std::string t the Tango server
   *
   * The code makes sure that the server should be started enough that a proxy can connect to it
   * The fixture will then wait for the server to go into state "ON"
   */
  void start();

  /**
   * @brief Shut down the Tango server
   */
  void stop();

  /**
   * @brief Get the port used for CORBA commuication
   * @return The CORBA port of this server
   */
  static std::string port();

  /**
   * @brief An URL that can be used by the Tango::DeviceProxy to connect to this server
   * @return
   */
  [[nodiscard]] std::string getClientUrl() const;

  /**
   * @brief The Tango device name
   * @return The device name used for this server
   */
  [[nodiscard]] const std::string& device() const;

  /**
   * @brief Configure the Tango server to run against an offline database
   * @param basePath base name of the db file. A template database with the name basename + ".db"
   * has to exist in the working folder.
   */
  ThreadedTangoServer& setOfflineDatabase(const std::string& basePath);

  ThreadedTangoServer& setKeepOfflineDatabase(bool keep);

  ThreadedTangoServer& setCreateOfflineDatabase(bool create);

  ThreadedTangoServer& overrideNames(const std::string& newNames);

  std::string testName;
  std::string offlineDatabase;
  bool createOfflineDatabase{true};
  bool keepOfflineDatabase{false};
  bool verbose{false};
  std::vector<std::string> argv;
  std::thread tangoServerThread;
  std::string deviceString;
  Tango::Util* tg{nullptr};
  static std::atomic<bool> shutdownRequested;
  static ThreadedTangoServer* self;
  static ThreadedTangoServer& instance() { return *self; }
  TangoTestServer_ns::TangoTestServer* ourDevice{nullptr};
};

ThreadedTangoServer::ThreadedTangoServer(std::string setTestName, bool setVerbose)
: testName(std::move(setTestName)), verbose(setVerbose) {
  self = this;
  start();
}

/**********************************************************************************************************************/

ThreadedTangoServer::~ThreadedTangoServer() {
  if(tangoServerThread.joinable()) {
    stop();
  }

  if(!keepOfflineDatabase) {
    try {
      std::filesystem::remove(offlineDatabase);
    }
    catch(std::runtime_error&) {
      // ignore
    }
  }
}

/**********************************************************************************************************************/

void ThreadedTangoServer::start() {
  std::mutex in_mtx;
  std::unique_lock<std::mutex> in(in_mtx);
  std::condition_variable cv;
  bool threadRunning{false};

  if(threadRunning) {
    return;
  }

  tangoServerThread = std::thread([&]() {
    argv.emplace_back(testName + "_ds");
    argv.emplace_back("Test" + testName);
    if(verbose || std::getenv("TANGO_TESTS_VERBOSE") != nullptr) {
      argv.emplace_back("-v5");
    }
    deviceString = std::string("tango/test/") + testName;

    if(offlineDatabase.empty()) {
      argv.emplace_back("-nodb");
      argv.emplace_back("-dlist");
    }
    else {
      argv.emplace_back("-file=" + offlineDatabase);
    }
    argv.emplace_back(deviceString);
    argv.emplace_back("-ORBendPoint");
    argv.emplace_back("giop:tcp::" + port());

    std::vector<const char*> args;
    args.resize(argv.size());
    std::transform(argv.begin(), argv.end(), args.begin(), [&](auto& s) { return s.c_str(); });

    try {
      // Need to pass it down to something that usually takes argc, argv directly from main
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-const-cast)
      auto* tg = Tango::Util::init(int(args.size()), const_cast<char**>(args.data()));
      tg->server_init(false);
      auto devices = tg->get_device_list_by_class("TangoTestServer");
      assert(devices.size() == 1);

      ourDevice = dynamic_cast<TangoTestServer_ns::TangoTestServer*>(devices[0]);
      assert(ourDevice != nullptr);

      auto callback = []() -> bool {
        auto shutdown = ThreadedTangoServer::shutdownRequested.load();
        if(shutdown) {
          return true;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        return false;
      };

      tg->server_set_event_loop(callback);
      {
        std::lock_guard<std::mutex> lg(in_mtx);
        threadRunning = true;
        cv.notify_one();
      }

      tg->server_run();
    }
    catch(Tango::DevFailed& ex) {
      Tango::Except::print_exception(ex);
    }
  });
  cv.wait(in, [&] { return threadRunning; });
}

/**********************************************************************************************************************/

void ThreadedTangoServer::stop() {
  ThreadedTangoServer::shutdownRequested.store(true);
  tangoServerThread.join();
}

/**********************************************************************************************************************/

std::string ThreadedTangoServer::port() {
  static std::string corbaPort;
  if(corbaPort.empty()) {
    std::random_device rd;
    std::uniform_int_distribution<int> dist(10000, 50000);
    corbaPort = std::to_string(dist(rd));
  }

  return corbaPort;
}

/**********************************************************************************************************************/

std::string ThreadedTangoServer::getClientUrl() const {
  return "tango://localhost:" + port() + "/" + device() + "%23dbase=no";
}

/**********************************************************************************************************************/

const std::string& ThreadedTangoServer::device() const {
  return deviceString;
}

/**********************************************************************************************************************/

ThreadedTangoServer& ThreadedTangoServer::setCreateOfflineDatabase(bool create) {
  createOfflineDatabase = create;

  return *this;
}

/**********************************************************************************************************************/

ThreadedTangoServer& ThreadedTangoServer::setKeepOfflineDatabase(bool keep) {
  keepOfflineDatabase = keep;

  return *this;
}

/**********************************************************************************************************************/

ThreadedTangoServer& ThreadedTangoServer::setOfflineDatabase(const std::string& basePath) {
  offlineDatabase = basePath + ".db";
  if(createOfflineDatabase) {
    auto sourceTemplate = basePath + "_template.db";
    try {
      std::filesystem::copy_file(sourceTemplate, offlineDatabase, std::filesystem::copy_options::overwrite_existing);
    }
    catch(std::runtime_error& err) {
      std::cerr << err.what() << std::endl;
      throw;
    }
  }

  return *this;
}

/**********************************************************************************************************************/

ThreadedTangoServer& ThreadedTangoServer::overrideNames(const std::string& newNames) {
  testName = newNames;

  return *this;
}

/**********************************************************************************************************************/

std::atomic<bool> ThreadedTangoServer::shutdownRequested{false};

ThreadedTangoServer* ThreadedTangoServer::self{nullptr};
BOOST_GLOBAL_FIXTURE(ThreadedTangoServer);

/**********************************************************************************************************************/

template<typename DERIVED>
struct AllRegisterDefaults {
  DERIVED* derived{static_cast<DERIVED*>(this)};

  bool isWriteable() { return true; }
  bool isReadable() { return true; }
  ChimeraTK::AccessModeFlags supportedFlags() { return {}; }
  size_t nChannels() { return 1; }
  size_t writeQueueLength() { return std::numeric_limits<size_t>::max(); }
  size_t nRuntimeErrorCases() { return 0; }
  using rawUserType = std::nullptr_t;

  static constexpr auto capabilities = TestCapabilities<>()
                                           .enableTestCatalogue()
                                           .disableForceDataLossWrite()
                                           .disableAsyncReadInconsistency()
                                           .disableSwitchReadOnly()
                                           .disableSwitchWriteOnly();

  void setForceRuntimeError(bool enable, size_t) {
    if(enable) {
    }
  }
};

/**********************************************************************************************************************/

template<typename DERIVED>
struct ScalarDefaultsBase : AllRegisterDefaults<DERIVED> {
  using AllRegisterDefaults<DERIVED>::AllRegisterDefaults;
  using AllRegisterDefaults<DERIVED>::derived;
  size_t nElementsPerChannel() { return 1; }

  template<typename UserType>
  std::vector<std::vector<UserType>> generateValue() {
    return {{ChimeraTK::numericToUserType<UserType>(static_cast<typename DERIVED::minimumUserType>(
        derived->template getRemoteValue<typename DERIVED::minimumUserType>()[0][0] + derived->increment))}};
  }

  template<typename UserType>
  std::vector<std::vector<UserType>> getRemoteValue() {
    auto v = ChimeraTK::numericToUserType<UserType>(derived->getValue());
    return {{v}};
  }

  void setRemoteValue() {
    auto v = derived->template generateValue<typename DERIVED::minimumUserType>()[0][0];
    derived->setValue(v);
  }
};

template<typename DERIVED>
struct ScalarDefaults : ScalarDefaultsBase<DERIVED> {
  using AllRegisterDefaults<DERIVED>::derived;
  size_t nElementsPerChannel() { return 1; }

  template<typename UserType>
  std::vector<std::vector<UserType>> getRemoteValue() {
    auto v = ChimeraTK::numericToUserType<UserType>(derived->getValue());
    return {{v}};
  }

  void setRemoteValue() {
    auto v = derived->template generateValue<typename DERIVED::minimumUserType>()[0][0];
    derived->setValue(v);
  }
};

/**********************************************************************************************************************/

template<typename DERIVED>
struct ArrayDefaults : AllRegisterDefaults<DERIVED> {
  using AllRegisterDefaults<DERIVED>::AllRegisterDefaults;
  using AllRegisterDefaults<DERIVED>::derived;

  template<typename UserType>
  std::vector<std::vector<UserType>> generateValue() {
    auto curval = derived->template getRemoteValue<typename DERIVED::minimumUserType>()[0];
    std::vector<UserType> val;
    for(size_t i = 0; i < derived->nElementsPerChannel(); ++i) {
      val.push_back(ChimeraTK::numericToUserType<UserType>(curval[i] + (i + 1) * derived->increment));
    }
    return {val};
  }

  template<typename UserType>
  std::vector<std::vector<UserType>> getRemoteValue() {
    std::vector<UserType> val;
    for(size_t i = 0; i < derived->nElementsPerChannel(); ++i) {
      val.push_back(ChimeraTK::numericToUserType<UserType>(derived->getValue(i)));
    }
    return {val};
  }

  template<typename REMOTE_TYPE>
  void setRemoteValueImpl() {
    auto val = derived->template generateValue<typename DERIVED::minimumUserType>()[0];
    for(size_t i = 0; i < derived->nElementsPerChannel(); ++i) {
      derived->setValue(i, ChimeraTK::userTypeToUserType<REMOTE_TYPE, typename DERIVED::minimumUserType>(val[i]));
    }
  }
};

/**********************************************************************************************************************/

struct RegSomeInt : ScalarDefaults<RegSomeInt> {
  std::string path() { return "IntScalar"; }
  typedef int32_t minimumUserType;
  int32_t increment{3};

  void setValue(minimumUserType v) { ThreadedTangoServer::self->ourDevice->attr_IntScalar_read[0] = v; }

  minimumUserType getValue() { return ThreadedTangoServer::self->ourDevice->attr_IntScalar_read[0]; }
};

struct RegSomeRoInt : ScalarDefaults<RegSomeRoInt> {
  std::string path() { return "IntRoScalar"; }
  typedef int32_t minimumUserType;
  int32_t increment{3};

  static constexpr auto capabilities = ScalarDefaults<RegSomeInt>::capabilities.enableTestReadOnly();

  void setValue(minimumUserType v) { ThreadedTangoServer::self->ourDevice->attr_IntRoScalar_read[0] = v; }

  minimumUserType getValue() { return ThreadedTangoServer::self->ourDevice->attr_IntRoScalar_read[0]; }
};

struct RegSomeWoInt : ScalarDefaults<RegSomeWoInt> {
  std::string path() { return "IntWoScalar"; }
  typedef int32_t minimumUserType;
  int32_t increment{3};

  static constexpr auto capabilities = ScalarDefaults<RegSomeInt>::capabilities.enableTestWriteOnly();

  void setValue(minimumUserType v) { ThreadedTangoServer::self->ourDevice->attr_IntWoScalar_read[0] = v; }

  minimumUserType getValue() { return ThreadedTangoServer::self->ourDevice->attr_IntWoScalar_read[0]; }
};

struct RegSomeBool : ScalarDefaults<RegSomeBool> {
  std::string path() { return "BooleanScalar"; }

  typedef ChimeraTK::Boolean minimumUserType;

  bool increment{false};

  void setValue(minimumUserType v) { ThreadedTangoServer::self->ourDevice->attr_BooleanScalar_read[0] = v; }

  minimumUserType getValue() { return ThreadedTangoServer::self->ourDevice->attr_BooleanScalar_read[0]; }
};

struct RegSomeRoBool : ScalarDefaults<RegSomeRoBool> {
  std::string path() { return "BooleanRoScalar"; }

  typedef ChimeraTK::Boolean minimumUserType;

  bool increment{false};

  static constexpr auto capabilities = ScalarDefaults<RegSomeInt>::capabilities.enableTestReadOnly();

  void setValue(minimumUserType v) { ThreadedTangoServer::self->ourDevice->attr_BooleanRoScalar_read[0] = v; }

  minimumUserType getValue() { return ThreadedTangoServer::self->ourDevice->attr_BooleanRoScalar_read[0]; }
};

struct RegSomeWoBool : ScalarDefaults<RegSomeWoBool> {
  std::string path() { return "BooleanWoScalar"; }

  typedef ChimeraTK::Boolean minimumUserType;

  bool increment{false};

  static constexpr auto capabilities = ScalarDefaults<RegSomeInt>::capabilities.enableTestWriteOnly();

  void setValue(minimumUserType v) { ThreadedTangoServer::self->ourDevice->attr_BooleanRoScalar_read[0] = v; }

  minimumUserType getValue() { return ThreadedTangoServer::self->ourDevice->attr_BooleanRoScalar_read[0]; }
};

struct RegSomeString : ScalarDefaults<RegSomeString> {
  std::string path() { return "StringScalar"; }

  typedef std::string minimumUserType;

  std::string increment;

  void setValue(minimumUserType v) {
    if(ThreadedTangoServer::self->ourDevice->attr_StringScalar_read[0] != nullptr) {
      Tango::string_free(ThreadedTangoServer::self->ourDevice->attr_StringScalar_read[0]);
    }

    ThreadedTangoServer::self->ourDevice->attr_StringScalar_read[0] = Tango::string_dup(v.c_str());
  }

  template<typename UserType>
  std::vector<std::vector<UserType>> generateValue() {
    assert(false);
  }

  minimumUserType getValue() { return std::string(ThreadedTangoServer::self->ourDevice->attr_StringScalar_read[0]); }

  size_t someValue{42};
};

template<>
std::vector<std::vector<std::string>> RegSomeString::generateValue<std::string>() {
  ++someValue;
  return {{"This is a string: " + std::to_string(someValue)}};
}

struct RegSomeIntArray : ArrayDefaults<RegSomeIntArray> {
  std::string path() { return "IntSpectrum"; }
  size_t nElementsPerChannel() { return 10; }
  typedef int32_t minimumUserType;
  int32_t increment{12};

  void setValue(int i, Tango::DevLong v) { ThreadedTangoServer::self->ourDevice->attr_IntSpectrum_read[i] = v; }

  void setRemoteValue() { setRemoteValueImpl<Tango::DevLong>(); }

  Tango::DevLong getValue(int i) {
    auto val = ThreadedTangoServer::self->ourDevice->attr_IntSpectrum_read[i];
    return val;
  }
};

struct RegSomeStringArray : ArrayDefaults<RegSomeStringArray> {
  using ArrayDefaults<RegSomeStringArray>::generateValue;

  std::string path() { return "StringSpectrum"; }
  size_t nElementsPerChannel() { return 10; }
  using minimumUserType = std::string;
  int32_t increment{12};

  void setValue(int i, std::string v) {
    if(ThreadedTangoServer::self->ourDevice->attr_StringSpectrum_read[i] != nullptr) {
      Tango::string_free(ThreadedTangoServer::self->ourDevice->attr_StringSpectrum_read[i]);
    }

    ThreadedTangoServer::self->ourDevice->attr_StringSpectrum_read[i] = Tango::string_dup(v.c_str());
  }

  void setRemoteValue() { setRemoteValueImpl<std::string>(); }

  std::string getValue(int i) {
    auto val = ThreadedTangoServer::self->ourDevice->attr_StringSpectrum_read[i];
    return val;
  }

  template<typename UserType>
  std::vector<std::vector<UserType>> generateValue() {
    assert(false);
  }

  size_t someValue{23};
};

template<>
std::vector<std::vector<std::string>> RegSomeStringArray::generateValue<std::string>() {
  std::vector<std::string> value;
  ++someValue;

  std::vector<std::string> val;
  for(size_t i = 0; i < derived->nElementsPerChannel(); ++i) {
    val.push_back("This is a string " + std::to_string(someValue + (i + 1)));
    ++someValue;
  }
  return {val};
}

/**********************************************************************************************************************/

BOOST_AUTO_TEST_CASE(unifiedBackendTest) {
  auto ubt = ChimeraTK::UnifiedBackendTest<>()
                 .addRegister<RegSomeInt>()
                 .addRegister<RegSomeRoInt>()
                 .addRegister<RegSomeWoInt>()
                 .addRegister<RegSomeBool>()
                 .addRegister<RegSomeRoBool>()
                 .addRegister<RegSomeWoBool>()
                 .addRegister<RegSomeString>()
                 .addRegister<RegSomeIntArray>()
                 .addRegister<RegSomeStringArray>()
#if 0
                 .addRegister<RegSomeFloat>()
                 .addRegister<RegSomeDouble>()
                 .addRegister<RegSomeString>()
                 .addRegister<RegSomeShortArray>()
                 .addRegister<RegSomeLongArray>()
                 .addRegister<RegSomeFloatArray>()
                 .addRegister<RegSomeDoubleArray>();
#endif
      ;

  ubt.runTests("(tango:" + ThreadedTangoServer::self->getClientUrl() + "?cacheFile=unifiedBackendTestCache.json)");
}

/**********************************************************************************************************************/
