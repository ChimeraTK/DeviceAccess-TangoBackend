// SPDX-FileCopyrightText: Deutsches Elektronen-Synchrotron DESY, MSK, ChimeraTK Project <chimeratk-support@desy.de>
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "TangoServerLauncher.h"

#include <chrono>
#include <filesystem>
#include <memory>
#include <random>
#include <thread>

TangoServerLauncher::TangoServerLauncher(std::string setTestName, bool setVerbose)
: testName(std::move(setTestName)), verbose(setVerbose) {
  self = this;
  start();
}

/**********************************************************************************************************************/

TangoServerLauncher::~TangoServerLauncher() {
  if(tangoServerProcess.joinable()) {
    stop();
  }

  remoteProxy.reset();

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

void TangoServerLauncher::start() {
  if(tangoServerProcess.running()) {
    return;
  }

  argv.clear();
  // argv.emplace_back(testName + "_ds");
  argv.emplace_back("Test" + testName);
  if(verbose || std::getenv("TANGO_TESTS_VERBOSE") != nullptr) {
    argv.emplace_back("-v5");
  }
  deviceString = std::string("tango/test/tg_test01") + testName;

  if(offlineDatabase.empty()) {
    argv.emplace_back("-nodb");
    argv.emplace_back("-dlist");
  }
  else {
    argv.emplace_back("-file=" + offlineDatabase);
  }
  argv.emplace_back(deviceString);
  argv.emplace_back("-ORBendPoint");
  argv.emplace_back("giop:tcp:127.0.0.1:" + port());

  try {
    tangoServerProcess = boost::process::child("./tangoTestServer", boost::process::args(argv));
  }
  catch(...) {
    abort();
  }

  auto url = getClientUrl();
  url.replace(url.find("%23"), 3, "#");
  while(true) {
    try {
      std::this_thread::sleep_for(std::chrono::milliseconds(500));
      remoteProxy = std::make_shared<Tango::DeviceProxy>(url);
      break;
    }
    catch(CORBA::Exception& ex) {
      Tango::Except::print_exception(ex);
      assert(false);
    }
  }
}

/**********************************************************************************************************************/

void TangoServerLauncher::stop() {
  remoteProxy.reset();

  auto remotePid = tangoServerProcess.id();
  kill(remotePid, SIGINT);
  tangoServerProcess.wait();
}

/**********************************************************************************************************************/

std::string TangoServerLauncher::port() {
  static std::string corbaPort;
  if(corbaPort.empty()) {
    std::random_device rd;
    std::uniform_int_distribution<int> dist(10000, 50000);
    corbaPort = std::to_string(dist(rd));
  }

  return corbaPort;
}

/**********************************************************************************************************************/

std::string TangoServerLauncher::getClientUrl() const {
  return "tango://localhost:" + port() + "/" + device() + "%23dbase=no";
}

/**********************************************************************************************************************/

const std::string& TangoServerLauncher::device() const {
  return deviceString;
}

/**********************************************************************************************************************/

TangoServerLauncher& TangoServerLauncher::setCreateOfflineDatabase(bool create) {
  createOfflineDatabase = create;

  return *this;
}

/**********************************************************************************************************************/

TangoServerLauncher& TangoServerLauncher::setKeepOfflineDatabase(bool keep) {
  keepOfflineDatabase = keep;

  return *this;
}

/**********************************************************************************************************************/

TangoServerLauncher& TangoServerLauncher::setOfflineDatabase(const std::string& basePath) {
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

TangoServerLauncher& TangoServerLauncher::overrideNames(const std::string& newNames) {
  testName = newNames;

  return *this;
}

/**********************************************************************************************************************/

TangoServerLauncher* TangoServerLauncher::self{nullptr};