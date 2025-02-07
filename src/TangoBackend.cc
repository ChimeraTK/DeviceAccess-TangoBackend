// SPDX-FileCopyrightText: Deutsches Elektronen-Synchrotron DESY, MSK, ChimeraTK Project <chimeratk-support@desy.de>
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "TangoBackend.h"

#include "OfflineCache.h"
#include "RegisterCatalogue.h"
#include "TangoRegisterAccessor.h"
#include <tango/tango.h>

#include <ChimeraTK/BackendFactory.h>
#include <ChimeraTK/Exception.h>
#include <ChimeraTK/MetadataCatalogue.h>
#include <ChimeraTK/RegisterCatalogue.h>

#include <boost/shared_ptr.hpp>

#include <utility>

static struct BackendRegisterer {
  BackendRegisterer() {
    std::cout << "TangoBackend::BackendRegisterer: registering backend type tango" << std::endl;
    ChimeraTK::BackendFactory::getInstance().registerBackendType("tango", &ChimeraTK::TangoBackend::createInstance);
  }
} backendRegisterer;

/**********************************************************************************************************************/
/**********************************************************************************************************************/
/**********************************************************************************************************************/

namespace ChimeraTK {
  // This is from public API, cannot change currently
  // NOLINTNEXTLINE(performance-unnecessary-value-param)
  boost::shared_ptr<DeviceBackend> TangoBackend::createInstance(
      std::string address, [[maybe_unused]] std::map<std::string, std::string> parameters) {
    // This is from public API, cannot change currently
    // NOLINTNEXTLINE(performance-unnecessary-value-param)
    std::optional<std::string> cacheFile;
    auto it = parameters.find("cacheFile");
    if(it != parameters.end()) {
      cacheFile = it->second;
    }

    return boost::shared_ptr<DeviceBackend>(new TangoBackend(std::move(address), cacheFile));
  }

  /********************************************************************************************************************/

  TangoBackend::TangoBackend(std::string address, std::optional<std::string> cacheFile)
  : DeviceBackendImpl(), _address(std::move(address)) {
    auto it = _address.find("%23");
    if(it != std::string::npos) {
      _address.replace(it, 3, "#");
    }
    if(cacheFile) {
      _registerCatalogue = OfflineCache(cacheFile.value()).read();
    }
    FILL_VIRTUAL_FUNCTION_TEMPLATE_VTABLE(getRegisterAccessor_impl);
  }

  /********************************************************************************************************************/

  void TangoBackend::open() {
    // Apparently there is no way to reconnect the proxy via public api. All the relevant connection API
    // is protected.
    if(_deviceProxy) {
      _deviceProxy.reset();
    }

    try {
      _deviceProxy = std::make_shared<Tango::DeviceProxy>(_address);
      _deviceProxy->set_transparency_reconnection(false);
      auto catalogue = TangoRegisterCatalogue();

      // https://github.com/tango-controls/pytango/issues/375
      // Some interaction with the server is needed. DeviceProxy will not throw in its constructor
      std::unique_ptr<Tango::AttributeInfoListEx> attributes(_deviceProxy->attribute_list_query_ex());
      for(auto& attr : *attributes) {
        catalogue.addRegister(TangoRegisterInfo(attr));
      }
      _registerCatalogue = std::move(catalogue);
    }
    catch(Tango::WrongNameSyntax& ex) {
      throw ChimeraTK::logic_error(
          "Invalid Tango device address " + _address + ", " + static_cast<std::string>(ex.errors[0].desc));
    }
    catch(Tango::ConnectionFailed& ex) {
      std::string message = "Cannot connect to " + _address + ", " + static_cast<std::string>(ex.errors[0].desc);
      setException(message);
      throw ChimeraTK::runtime_error(message);
    }
    catch(CORBA::Exception& ex) {
      std::string message = "Cannot connect to " + _address + "for unknown reason";
      setException(message);
      throw ChimeraTK::runtime_error(message);
    }

    setOpenedAndClearException();
  }

  /********************************************************************************************************************/

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

    switch(info.attributeInfo.data_format) {
      case Tango::AttrDataFormat::SPECTRUM:
      case Tango::AttrDataFormat::SCALAR: {
        switch(info.attributeInfo.data_type) {
          case Tango::DEV_BOOLEAN:
            p.reset(new TangoBackendRegisterAccessor<UserType, Tango::DevBoolean>(
                sharedThis, info, registerPathName, numberOfWords, wordOffsetInRegister, flags));
            break;
          case Tango::DEV_SHORT:
            p.reset(new TangoBackendRegisterAccessor<UserType, Tango::DevShort>(
                sharedThis, info, registerPathName, numberOfWords, wordOffsetInRegister, flags));
            break;
          case Tango::DEV_LONG:
            p.reset(new TangoBackendRegisterAccessor<UserType, Tango::DevLong>(
                sharedThis, info, registerPathName, numberOfWords, wordOffsetInRegister, flags));
            break;
          case Tango::DEV_FLOAT:
            p.reset(new TangoBackendRegisterAccessor<UserType, Tango::DevFloat>(
                sharedThis, info, registerPathName, numberOfWords, wordOffsetInRegister, flags));
            break;
          case Tango::DEV_DOUBLE:
            p.reset(new TangoBackendRegisterAccessor<UserType, Tango::DevDouble>(
                sharedThis, info, registerPathName, numberOfWords, wordOffsetInRegister, flags));
            break;
          case Tango::DEV_USHORT:
            p.reset(new TangoBackendRegisterAccessor<UserType, Tango::DevUShort>(
                sharedThis, info, registerPathName, numberOfWords, wordOffsetInRegister, flags));
            break;
          case Tango::DEV_ULONG:
            p.reset(new TangoBackendRegisterAccessor<UserType, Tango::DevULong>(
                sharedThis, info, registerPathName, numberOfWords, wordOffsetInRegister, flags));
            break;
          case Tango::DEV_STRING:
            p.reset(new TangoBackendRegisterAccessor<UserType, std::string>(
                sharedThis, info, registerPathName, numberOfWords, wordOffsetInRegister, flags));
            break;
          case Tango::DEV_STATE:
            p.reset(new TangoBackendRegisterAccessor<UserType, Tango::DevState>(
                sharedThis, info, registerPathName, numberOfWords, wordOffsetInRegister, flags));
            break;
          case Tango::DEV_UCHAR:
            p.reset(new TangoBackendRegisterAccessor<UserType, Tango::DevUChar>(
                sharedThis, info, registerPathName, numberOfWords, wordOffsetInRegister, flags));
            break;
          case Tango::DEV_LONG64:
            p.reset(new TangoBackendRegisterAccessor<UserType, Tango::DevLong64>(
                sharedThis, info, registerPathName, numberOfWords, wordOffsetInRegister, flags));
            break;
          case Tango::DEV_ULONG64:
            p.reset(new TangoBackendRegisterAccessor<UserType, Tango::DevULong64>(
                sharedThis, info, registerPathName, numberOfWords, wordOffsetInRegister, flags));
            break;
          // Compatibility for old servers that have DEV_INT
          case 27:
            p.reset(new TangoBackendRegisterAccessor<UserType, Tango::DevLong>(
                sharedThis, info, registerPathName, numberOfWords, wordOffsetInRegister, flags));
            break;
          case Tango::DEV_ENUM:
            p.reset(new TangoBackendRegisterAccessor<UserType, Tango::DevEnum>(
                sharedThis, info, registerPathName, numberOfWords, wordOffsetInRegister, flags));
            break;
          default:
            throw ChimeraTK::logic_error("Unsupported Tango type " + std::to_string(info.attributeInfo.data_type));
            break;
        }
        break;
      }
      case Tango::AttrDataFormat::IMAGE:
        break;
      case Tango::AttrDataFormat::FMT_UNKNOWN:
      default:
        throw ChimeraTK::logic_error(
            "Broken Tango attribute database information. " + registerPathName + " does not come with a data format");
    }

    p->setExceptionBackend(sharedThis);
    return p;
  }

  /********************************************************************************************************************/

  RegisterCatalogue TangoBackend::getRegisterCatalogue() const {
    return RegisterCatalogue(_registerCatalogue.clone());
  }

  /********************************************************************************************************************/

  std::string TangoBackend::readDeviceInfo() {
    return std::string("Tango Device: ") + _address;
  }

  /********************************************************************************************************************/

} // namespace ChimeraTK
