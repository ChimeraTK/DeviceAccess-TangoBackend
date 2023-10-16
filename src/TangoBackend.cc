// SPDX-FileCopyrightText: Deutsches Elektronen-Synchrotron DESY, MSK, ChimeraTK Project <chimeratk-support@desy.de>
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "TangoBackend.h"

#include <boost/shared_ptr.hpp>

#include <utility>

namespace util {
  static ChimeraTK::DataDescriptor buildDataDescriptor(int tangoDataType) {
    switch(tangoDataType) {
      case Tango::DEV_BOOLEAN:
        return ChimeraTK::DataDescriptor(ChimeraTK::DataType::Boolean);
      case Tango::DEV_DOUBLE:
        return ChimeraTK::DataDescriptor(ChimeraTK::DataType::float64);
      case Tango::DEV_FLOAT:
        return ChimeraTK::DataDescriptor(ChimeraTK::DataType::float32);
      case Tango::DEV_LONG64:
        return ChimeraTK::DataDescriptor(ChimeraTK::DataType::int64);
      case Tango::DEV_LONG:
      case Tango::DEV_INT:
      case Tango::DEV_STATE:
        return ChimeraTK::DataDescriptor(ChimeraTK::DataType::int32);
      case Tango::DEV_SHORT:
        return ChimeraTK::DataDescriptor(ChimeraTK::DataType::int16);
      case Tango::DEV_UCHAR:
        return ChimeraTK::DataDescriptor(ChimeraTK::DataType::uint8);
      case Tango::DEV_ULONG64:
        return ChimeraTK::DataDescriptor(ChimeraTK::DataType::uint64);
      case Tango::DEV_ULONG:
        return ChimeraTK::DataDescriptor(ChimeraTK::DataType::uint32);
      case Tango::DEV_USHORT:
        return ChimeraTK::DataDescriptor(ChimeraTK::DataType::uint16);
      case Tango::DEV_VOID:
        // Not sure if this is a proper type or just meaning that the command does not have an argument
        return ChimeraTK::DataDescriptor(ChimeraTK::DataType::Void);
      case Tango::DEV_STRING:
        return ChimeraTK::DataDescriptor(ChimeraTK::DataType::string);
      default:
        // FIXME Or logic_error? Probably too hard
        assert(false);
    }
    return {};
  }
}; // namespace util

namespace ChimeraTK {
  // FIXME READ_WITH_WRITE is a readback-channel that has to have a write channel defined somewhere.
  // How do we handle that one properly?
  TangoBackendRegisterInfo::TangoBackendRegisterInfo(const Tango::AttributeInfoEx& attribute)
  : BackendRegisterInfoBase(), _nElements(attribute.max_dim_x), _nChannels(attribute.max_dim_y), _name(attribute.name),
    _readable(attribute.writable == Tango::AttrWriteType::READ ||
        attribute.writable == Tango::AttrWriteType::READ_WRITE ||
        attribute.writable == Tango::AttrWriteType::READ_WITH_WRITE),
    _writeable(
        attribute.writable == Tango::AttrWriteType::WRITE || attribute.writable == Tango::AttrWriteType::READ_WRITE),
    _descriptor(util::buildDataDescriptor(attribute.data_type)), _tangoType(attribute.data_type) {}

  RegisterPath TangoBackendRegisterInfo::getRegisterName() const {
    return _name;
  }

  unsigned int TangoBackendRegisterInfo::getNumberOfElements() const {
    return _nElements;
  }
  unsigned int TangoBackendRegisterInfo::getNumberOfChannels() const {
    return _nChannels;
  }
  const DataDescriptor& TangoBackendRegisterInfo::getDataDescriptor() const {
    return _descriptor;
  }
  bool TangoBackendRegisterInfo::isReadable() const {
    return _readable;
  }

  bool TangoBackendRegisterInfo::isWriteable() const {
    return _writeable;
  }

  AccessModeFlags TangoBackendRegisterInfo::getSupportedAccessModes() const {
    return {};
  }

  boost::shared_ptr<DeviceBackend> TangoBackend::createInstance(
      std::string address, std::map<std::string, std::string> parameters) {
    return boost::shared_ptr<DeviceBackend>(new TangoBackend(std::move(address)));
  }

  TangoBackend::TangoBackend(std::string address) : DeviceBackendImpl(), _address(std::move(address)) {}

  void TangoBackend::open() {
    try {
      _deviceProxy = std::make_shared<Tango::DeviceProxy>(_address);
      // FIXME: I think we want to do this manually?
      _deviceProxy->set_transparency_reconnection(false);
    }
    catch(Tango::WrongNameSyntax& ex) {
      throw ChimeraTK::logic_error(
          std::string("Invalid tango device address ") + _address + ", " + static_cast<std::string>(ex.errors[0].desc));
    }
    catch(Tango::ConnectionFailed& ex) {
      throw ChimeraTK::runtime_error(
          "Cannot connect to " + _address + ": " + static_cast<std::string>(ex.errors[0].desc));
    }
  }

  void TangoBackend::close() {
    if(_deviceProxy) {
      // Not sure if we have to do this
      _deviceProxy->cancel_all_polling_asynch_request();
    }
    _deviceProxy.reset();
  }

  bool TangoBackend::isFunctional() const {
    if(!_deviceProxy) return false;

    try {
      _deviceProxy->ping();
      return true;
    }
    catch(Tango::CommunicationFailed& ex) {
    }
    catch(Tango::ConnectionFailed& ex) {
    }
  }

  RegisterCatalogue TangoBackend::getRegisterCatalogue() const {
    if(_catalogueCompleted) return RegisterCatalogue(_mutableCatalogue.clone());

    if(!_deviceProxy) {
      return RegisterCatalogue(_mutableCatalogue.clone());
    }

    // FIXME Do in thread?
    try {
      std::unique_ptr<Tango::AttributeInfoListEx> attributes(_deviceProxy->attribute_list_query_ex());
      for(auto& attributeInfo : *attributes) {
        TangoBackendRegisterInfo registerInfo(attributeInfo);
        _mutableCatalogue.addRegister(registerInfo);
      }
    }
    catch(Tango::ConnectionFailed& ex) {
    }
    catch(Tango::CommunicationFailed& ex) {
    }
    catch(Tango::DevError& ex) {
    }

    _catalogueCompleted = true;
    return RegisterCatalogue(_mutableCatalogue.clone());
  }

  MetadataCatalogue TangoBackend::getMetadataCatalogue() const {
    return {};
  }

  std::string TangoBackend::readDeviceInfo() {
    return "Tango Device@" + _address;
  }

  template<typename UserType>
  boost::shared_ptr<NDRegisterAccessor<UserType>> TangoBackend::getRegisterAccessor_impl(
      const RegisterPath& registerPathName, size_t numberOfWords, size_t wordOffsetInRegister, AccessModeFlags flags) {
    if(_mutableCatalogue.getNumberOfRegisters() == 0) {
      throw ChimeraTK::logic_error("No registers found in the catalog.");
    }

    TangoBackendRegisterInfo* info = nullptr;
    for(auto it = _mutableCatalogue.begin(), ite = _mutableCatalogue.end(); it != ite; it++) {
      if(it->getRegisterName() == registerPathName) {
        info = dynamic_cast<TangoBackendRegisterInfo*>(&(*it));
        break;
      }
    }
    if(info == nullptr) {
      throw ChimeraTK::logic_error(
          std::string("Requested register (") + registerPathName + ") was not found in the catalog.");
    }
    if(numberOfWords + wordOffsetInRegister > info->getNumberOfElements() ||
        (numberOfWords == 0 && wordOffsetInRegister > 0)) {
      std::stringstream ss;
      ss << "Requested number of words/elements ( " << numberOfWords << ") with offset " << wordOffsetInRegister
         << " exceeds the number of available words/elements: " << info->getNumberOfElements();
      throw ChimeraTK::logic_error(ss.str());
    }

    if(numberOfWords == 0) numberOfWords = info->getNumberOfElements();

    switch(info->getTangoType()) {
      case Tango::DEV_LONG: {
      }
      default:
        assert(false);
    }
  }

  void TangoBackend::setException() {}

  void TangoBackend::activateAsyncRead() noexcept {};
} // namespace ChimeraTK