// SPDX-FileCopyrightText: Deutsches Elektronen-Synchrotron DESY, MSK, ChimeraTK Project <chimeratk-support@desy.de>
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <tango/idl/tango.h>

#include <ChimeraTK/SupportedUserTypes.h>
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE testUnifiedBackendTest

#include "TangoServerLauncher.h"
#include <tango/tango.h>

#include <ChimeraTK/UnifiedBackendTest.h>

#include <boost/filesystem.hpp>
#include <boost/process.hpp>

#include <chrono>
#include <filesystem>
#include <random>

#define BOOST_NO_EXCEPTIONS
#include <boost/test/unit_test.hpp>
#undef BOOST_NO_EXCEPTIONS

using namespace boost::unit_test_framework;
using namespace ChimeraTK;

BOOST_GLOBAL_FIXTURE(TangoServerLauncher);

/**********************************************************************************************************************/

template<typename DERIVED>
struct AllRegisterDefaults {
  DERIVED* derived{static_cast<DERIVED*>(this)};

  bool isWriteable() { return true; }
  bool isReadable() { return true; }
  ChimeraTK::AccessModeFlags supportedFlags() { return {}; }
  size_t nChannels() { return 1; }
  size_t writeQueueLength() { return std::numeric_limits<size_t>::max(); }
  size_t nRuntimeErrorCases() { return 1; }
  using rawUserType = std::nullptr_t;

  static constexpr auto capabilities = TestCapabilities<>()
                                           .enableTestCatalogue()
                                           .disableForceDataLossWrite()
                                           .disableAsyncReadInconsistency()
                                           .disableSwitchReadOnly()
                                           .disableSwitchWriteOnly();

  void setForceRuntimeError(bool enable, size_t) {
    if(enable) {
      TangoServerLauncher::self->stop();
    }
    else {
      TangoServerLauncher::self->start();
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
  std::string writePath() { return "IntScalar"; }
  std::string readPath() { return "IntScalar"; }
  typedef int32_t minimumUserType;
  int32_t increment{3};

  void setValue(minimumUserType v) {
    auto attr = Tango::DeviceAttribute(writePath(), v);
    TangoServerLauncher::self->remoteProxy->write_attribute(attr);
  }

  minimumUserType getValue() {
    Tango::DevLong value;
    auto attr = TangoServerLauncher::self->remoteProxy->read_attribute(readPath());
    attr >> value;

    return value;
  }
};

struct RegSomeRoInt : ScalarDefaults<RegSomeRoInt> {
  std::string path() { return "IntRoScalar"; }
  std::string writePath() { return "IntScalar"; }
  std::string readPath() { return "IntScalar"; }
  typedef int32_t minimumUserType;
  int32_t increment{3};

  static constexpr auto capabilities = ScalarDefaults<RegSomeInt>::capabilities.enableTestReadOnly();

  void setValue(minimumUserType v) {
    auto attr = Tango::DeviceAttribute(writePath(), v);
    TangoServerLauncher::self->remoteProxy->write_attribute(attr);
  }

  minimumUserType getValue() {
    Tango::DevLong value;
    auto attr = TangoServerLauncher::self->remoteProxy->read_attribute(readPath());
    attr >> value;

    return value;
  }
};

struct RegSomeWoInt : ScalarDefaults<RegSomeWoInt> {
  std::string path() { return "IntWoScalar"; }
  std::string writePath() { return "IntScalar"; }
  std::string readPath() { return "IntScalar"; }
  typedef int32_t minimumUserType;
  int32_t increment{3};

  static constexpr auto capabilities = ScalarDefaults<RegSomeInt>::capabilities.enableTestWriteOnly();

  void setValue(minimumUserType v) {
    auto attr = Tango::DeviceAttribute(writePath(), v);
    TangoServerLauncher::self->remoteProxy->write_attribute(attr);
  }

  minimumUserType getValue() {
    Tango::DevLong value;
    auto attr = TangoServerLauncher::self->remoteProxy->read_attribute(readPath());
    attr >> value;

    return value;
  }
};

struct RegSomeBool : ScalarDefaults<RegSomeBool> {
  std::string path() { return "BooleanScalar"; }
  std::string writePath() { return "BooleanScalar"; }
  std::string readPath() { return "BooleanScalar"; }

  typedef ChimeraTK::Boolean minimumUserType;

  bool increment{false};

  void setValue(minimumUserType v) {
    try {
      auto attr = Tango::DeviceAttribute(writePath(), bool(v));
      TangoServerLauncher::self->remoteProxy->write_attribute(attr);
    }
    catch(CORBA::Exception& ex) {
      Tango::Except::print_exception(ex);
      assert(false);
    }
  }

  minimumUserType getValue() {
    Tango::DevBoolean value;
    try {
      auto attr = TangoServerLauncher::self->remoteProxy->read_attribute(readPath());
      attr >> value;
    }
    catch(CORBA::Exception& ex) {
      Tango::Except::print_exception(ex);
      assert(false);
    }

    return value;
  }
};

struct RegSomeRoBool : ScalarDefaults<RegSomeRoBool> {
  std::string path() { return "BooleanRoScalar"; }
  std::string writePath() { return "BooleanScalar"; }
  std::string readPath() { return "BooleanScalar"; }

  typedef ChimeraTK::Boolean minimumUserType;

  bool increment{false};

  static constexpr auto capabilities = ScalarDefaults<RegSomeInt>::capabilities.enableTestReadOnly();

  void setValue(minimumUserType v) {
    auto attr = Tango::DeviceAttribute(readPath(), bool(v));
    TangoServerLauncher::self->remoteProxy->write_attribute(attr);
  }

  minimumUserType getValue() {
    Tango::DevBoolean value;
    auto attr = TangoServerLauncher::self->remoteProxy->read_attribute(writePath());
    attr >> value;

    return value;
  }
};

struct RegSomeWoBool : ScalarDefaults<RegSomeWoBool> {
  std::string path() { return "BooleanWoScalar"; }
  std::string writePath() { return "BooleanScalar"; }
  std::string readPath() { return "BooleanScalar"; }

  typedef ChimeraTK::Boolean minimumUserType;

  bool increment{false};

  static constexpr auto capabilities = ScalarDefaults<RegSomeInt>::capabilities.enableTestWriteOnly();

  void setValue(minimumUserType v) {
    auto attr = Tango::DeviceAttribute(writePath(), bool(v));
    TangoServerLauncher::self->remoteProxy->write_attribute(attr);
  }

  minimumUserType getValue() {
    Tango::DevBoolean value;
    auto attr = TangoServerLauncher::self->remoteProxy->read_attribute(readPath());
    attr >> value;

    return value;
  }
};

struct RegSomeString : ScalarDefaults<RegSomeString> {
  std::string path() { return "StringScalar"; }
  std::string writePath() { return "StringScalar"; }
  std::string readPath() { return "StringScalar"; }

  typedef std::string minimumUserType;

  std::string increment;

  void setValue(minimumUserType v) {
    auto attr = Tango::DeviceAttribute(writePath(), v);
    TangoServerLauncher::self->remoteProxy->write_attribute(attr);
  }

  template<typename UserType>
  std::vector<std::vector<UserType>> generateValue() {
    assert(false);
  }

  minimumUserType getValue() {
    std::string value;
    auto attr = TangoServerLauncher::self->remoteProxy->read_attribute(readPath());
    attr >> value;

    return value;
  }

  size_t someValue{42};
};

template<>
std::vector<std::vector<std::string>> RegSomeString::generateValue<std::string>() {
  ++someValue;
  return {{"This is a string: " + std::to_string(someValue)}};
}

struct RegSomeIntArray : ArrayDefaults<RegSomeIntArray> {
  std::string path() { return "IntSpectrum"; }
  std::string writePath() { return "IntSpectrum"; }
  std::string readPath() { return "IntSpectrum"; }
  size_t nElementsPerChannel() { return 10; }
  typedef int32_t minimumUserType;
  int32_t increment{12};

  void setValue(int i, Tango::DevLong v) {
    try {
      auto attrRead = TangoServerLauncher::self->remoteProxy->read_attribute(readPath());

      std::vector<Tango::DevLong> values(10, {});
      attrRead >> values;

      values[i] = v;
      // for some reason, the vector returned by Tango is one too large
      values.resize(10);

      auto attr = Tango::DeviceAttribute(writePath(), values);
      TangoServerLauncher::self->remoteProxy->write_attribute(attr);
    }
    catch(CORBA::Exception& ex) {
      Tango::Except::print_exception(ex);
      assert(false);
    }
  }

  void setRemoteValue() { setRemoteValueImpl<Tango::DevLong>(); }

  Tango::DevLong getValue(int i) {
    auto attrRead = TangoServerLauncher::self->remoteProxy->read_attribute(readPath());

    std::vector<Tango::DevLong> values;
    attrRead >> values;

    return values[i];
  }
};

struct RegSomeStringArray : ArrayDefaults<RegSomeStringArray> {
  using ArrayDefaults<RegSomeStringArray>::generateValue;

  std::string path() { return "StringSpectrum"; }
  std::string writePath() { return "StringSpectrum"; }
  std::string readPath() { return "StringSpectrum"; }

  size_t nElementsPerChannel() { return 10; }
  using minimumUserType = std::string;
  int32_t increment{12};

  void setValue(int i, std::string v) {
    auto attrRead = TangoServerLauncher::self->remoteProxy->read_attribute(readPath());

    std::vector<std::string> values;
    attrRead >> values;

    values[i] = v;

    // for some reason, the vector returned by Tango is one too large
    values.resize(10);

    auto attr = Tango::DeviceAttribute(writePath(), values);
    TangoServerLauncher::self->remoteProxy->write_attribute(attr);
  }

  void setRemoteValue() { setRemoteValueImpl<std::string>(); }

  std::string getValue(int i) {
    auto attrRead = TangoServerLauncher::self->remoteProxy->read_attribute(readPath());

    std::vector<std::string> values;
    attrRead >> values;

    return values[i];
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

struct RegSomeBooleanArray : ArrayDefaults<RegSomeBooleanArray> {
  using ArrayDefaults<RegSomeBooleanArray>::generateValue;

  std::string path() { return "BooleanSpectrum"; }
  std::string writePath() { return "BooleanSpectrum"; }
  std::string readPath() { return "BooleanSpectrum"; }

  size_t nElementsPerChannel() { return 10; }
  using minimumUserType = ChimeraTK::Boolean;
  int32_t increment{12};

  void setValue(int i, bool v) {
    auto attrRead = TangoServerLauncher::self->remoteProxy->read_attribute(readPath());

    std::vector<Tango::DevBoolean> values;
    try {
      attrRead >> values;
    }
    catch(Tango::DevFailed& ex) {
      Tango::Except::print_exception(ex);
    }

    values[i] = v;

    // for some reason, the vector returned by Tango is one too large
    values.resize(10);

    auto attr = Tango::DeviceAttribute(writePath(), values);
    TangoServerLauncher::self->remoteProxy->write_attribute(attr);
  }

  void setRemoteValue() { setRemoteValueImpl<ChimeraTK::Boolean>(); }

  ChimeraTK::Boolean getValue(int i) {
    auto attrRead = TangoServerLauncher::self->remoteProxy->read_attribute(readPath());

    std::vector<Tango::DevBoolean> values;
    attrRead >> values;

    return {values[i]};
  }

  template<typename UserType>
  std::vector<std::vector<UserType>> generateValue() {
    assert(false);
  }

  size_t someValue{23};
};

template<>
std::vector<std::vector<ChimeraTK::Boolean>> RegSomeBooleanArray::generateValue<ChimeraTK::Boolean>() {
  std::vector<ChimeraTK::Boolean> value;
  ++someValue;

  std::vector<ChimeraTK::Boolean> val;
  val.reserve(derived->nElementsPerChannel());
  for(size_t i = 0; i < derived->nElementsPerChannel(); ++i) {
    val.emplace_back((i % 2) == 1);
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
                 .addRegister<RegSomeBooleanArray>();

  ubt.runTests("(tango:" + TangoServerLauncher::self->getClientUrl() + "?cacheFile=unifiedBackendTestCache.json)");
}

/**********************************************************************************************************************/
