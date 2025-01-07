// SPDX-FileCopyrightText: Deutsches Elektronen-Synchrotron DESY, MSK, ChimeraTK Project <chimeratk-support@desy.de>
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "TangoBackend.h"

#include "RegisterCatalogue.h"

#include <ChimeraTK/BackendFactory.h>
#include <ChimeraTK/RegisterCatalogue.h>
#include <ChimeraTK/MetadataCatalogue.h>

#include <tango/tango.h>

#include <boost/shared_ptr.hpp>

#include <utility>

static struct BackendRegisterer {
  BackendRegisterer() {
    std::cout << "TangoBackend::BackendRegisterer: registering backend type tango" << std::endl;
    ChimeraTK::BackendFactory::getInstance().registerBackendType(
        "tango", &ChimeraTK::TangoBackend::createInstance);
  }
} backendRegisterer;

namespace ChimeraTK {
  boost::shared_ptr<DeviceBackend> TangoBackend::createInstance(
      std::string address, [[maybe_unused]] std::map<std::string, std::string> parameters) {
    // This is from public API, cannot change currently
    // NOLINTNEXTLINE(performance-unnecessary-value-param)
    return boost::shared_ptr<DeviceBackend>(new TangoBackend(address));
  }

  TangoBackend::TangoBackend(std::string address) : DeviceBackendImpl(), _address(std::move(address)) {}

  void TangoBackend::open() {}

  void TangoBackend::close() {}


  RegisterCatalogue TangoBackend::getRegisterCatalogue() const {
    return RegisterCatalogue(_registerCatalogue.clone());
  }

  std::string TangoBackend::readDeviceInfo() {
    return std::string("Tango Device: ") + _address;
  }

} // namespace ChimeraTK
