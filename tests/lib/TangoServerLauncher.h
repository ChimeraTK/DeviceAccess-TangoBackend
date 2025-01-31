// SPDX-FileCopyrightText: Deutsches Elektronen-Synchrotron DESY, MSK, ChimeraTK Project <chimeratk-support@desy.de>
// SPDX-License-Identifier: LGPL-3.0-or-later
#pragma once

#include <tango/tango.h>

#include <boost/process.hpp>

#include <string>
#include <vector>

struct TangoServerLauncher {
  explicit TangoServerLauncher(std::string setTestName = "Unified", bool setVerbose = false);
  ~TangoServerLauncher();

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
  TangoServerLauncher& setOfflineDatabase(const std::string& basePath);

  TangoServerLauncher& setKeepOfflineDatabase(bool keep);

  TangoServerLauncher& setCreateOfflineDatabase(bool create);

  TangoServerLauncher& overrideNames(const std::string& newNames);

  std::string testName;
  std::string offlineDatabase;
  bool createOfflineDatabase{true};
  bool keepOfflineDatabase{false};
  bool verbose{false};
  std::vector<std::string> argv;

  boost::process::child tangoServerProcess;

  std::string deviceString;
  Tango::Util* tg{nullptr};
  static TangoServerLauncher* self;
  static TangoServerLauncher& instance() { return *self; }
  std::shared_ptr<Tango::DeviceProxy> remoteProxy;
};
