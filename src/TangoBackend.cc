// SPDX-FileCopyrightText: Deutsches Elektronen-Synchrotron DESY, MSK, ChimeraTK Project <chimeratk-support@desy.de>
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "TangoBackend.h"

#include "OfflineCache.h"
#include "RegisterCatalogue.h"
#include "TangoRegisterAccessor.h"
#include <tango/tango.h>

#include <ChimeraTK/BackendFactory.h>
#include <ChimeraTK/MetadataCatalogue.h>
#include <ChimeraTK/RegisterCatalogue.h>

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
  // This is from public API, cannot change currently
  // NOLINTNEXTLINE(performance-unnecessary-value-param)
  boost::shared_ptr<DeviceBackend> TangoBackend::createInstance(
      std::string address, [[maybe_unused]] std::map<std::string, std::string> parameters) {
    // This is from public API, cannot change currently
    // NOLINTNEXTLINE(performance-unnecessary-value-param)
    std::string cacheFile;
    try {
      cacheFile = parameters.at("cacheFile");
    }
    catch(std::out_of_range&) {
      throw ChimeraTK::logic_error("Missing mandatory parameter: cacheFile");
    }

    return boost::shared_ptr<DeviceBackend>(new TangoBackend(address, cacheFile));
  }

  TangoBackend::TangoBackend(std::string address, const std::string& cacheFile) : DeviceBackendImpl(), _address(std::move(address)) {
    _address.replace(_address.find("%23"), 3, "#");
    _registerCatalogue = OfflineCache(cacheFile).read();
    FILL_VIRTUAL_FUNCTION_TEMPLATE_VTABLE(getRegisterAccessor_impl);
  }

  void TangoBackend::open() {
    try {
      if(!_deviceProxy) {
        _deviceProxy = std::make_shared<Tango::DeviceProxy>(_address);
        _deviceProxy->set_transparency_reconnection(false);
        auto catalogue = TangoRegisterCatalogue();

        std::unique_ptr<Tango::AttributeInfoListEx> attributes(_deviceProxy->attribute_list_query_ex());
        for(auto& attr : *attributes) {
          catalogue.addRegister(TangoRegisterInfo(attr));
        }
        _registerCatalogue = std::move(catalogue);
      }
      else {
        _deviceProxy->connect(_address);
      }
    }
    catch(Tango::WrongNameSyntax& ex) {
      throw ChimeraTK::logic_error(
          "Invalid Tango device address " + _address + ", " + static_cast<std::string>(ex.errors[0].desc));
    }
    catch(Tango::ConnectionFailed& ex) {
      throw ChimeraTK::runtime_error(
          "Cannot connect to " + _address + ", " + static_cast<std::string>(ex.errors[0].desc));
    }

    setOpenedAndClearException();
  }

  void TangoBackend::close() {
    _opened = false;
    _deviceProxy.reset();
  }

  /********************************************************************************************************************/

  template<typename UserType>
  boost::shared_ptr<NDRegisterAccessor<UserType>> TangoBackend::getRegisterAccessor_impl(
      const RegisterPath& registerPathName, size_t numberOfWords, size_t wordOffsetInRegister, AccessModeFlags flags) {
    boost::shared_ptr<NDRegisterAccessor<UserType>> p;

    auto info = _registerCatalogue.getBackendRegister(registerPathName);

    auto sharedThis = boost::static_pointer_cast<TangoBackend>(shared_from_this());
    p.reset(new TangoBackendRegisterAccessor<UserType>(
        sharedThis, info, registerPathName, numberOfWords, wordOffsetInRegister, flags));
    p->setExceptionBackend(shared_from_this());
    return p;
  }

  RegisterCatalogue TangoBackend::getRegisterCatalogue() const {
    return RegisterCatalogue(_registerCatalogue.clone());
  }

  std::string TangoBackend::readDeviceInfo() {
    return std::string("Tango Device: ") + _address;
  }

} // namespace ChimeraTK
