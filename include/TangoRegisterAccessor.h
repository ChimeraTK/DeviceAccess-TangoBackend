// SPDX-FileCopyrightText: Deutsches Elektronen-Synchrotron DESY, MSK, ChimeraTK Project <chimeratk-support@desy.de>
// SPDX-License-Identifier: LGPL-3.0-or-later
#pragma once

#include "TangoBackend.h"
#include <tango/tango.h>

#include <ChimeraTK/NDRegisterAccessor.h>

#include <utility>

namespace ChimeraTK {

  template<typename UserType>
  class TangoBackendRegisterAccessor : public NDRegisterAccessor<UserType> {
   public:
    TangoBackendRegisterAccessor(boost::shared_ptr<TangoBackend> backend_, TangoRegisterInfo info,
        const std::string& registerPathName, size_t numberOfWords, size_t wordOffsetInRegister, AccessModeFlags flags)
    : NDRegisterAccessor<UserType>(registerPathName, flags), backend(std::move(backend_)),
      registerInfo(std::move(info)) {
      auto actualLength = info.getNumberOfElements();
      auto elementOffset = wordOffsetInRegister;
      auto nElements = numberOfWords;

      if(nElements == 0) {
        nElements = actualLength;
      }

      if(flags.has(AccessMode::raw) || flags.has(AccessMode::wait_for_new_data)) {
        throw ChimeraTK::logic_error("invalid access mode for this register");
      }

      if(nElements + elementOffset > actualLength) {
        throw ChimeraTK::logic_error(
            "Requested number of words exceeds the length of the Tango attribute " + registerPathName);
      }

      isPartial = nElements != actualLength || elementOffset != 0;

      NDRegisterAccessor<UserType>::buffer_2D.resize(1);
      NDRegisterAccessor<UserType>::buffer_2D[0].resize(nElements);
    }

    void doReadTransferSynchronously() override;
    bool doWriteTransfer(VersionNumber) override;

    void doPreRead(TransferType) override {
      if(!backend->isOpen()) {
        throw ChimeraTK::logic_error("Read operation not allowed while device is closed.");
      }
      if(!isReadable()) {
        throw ChimeraTK::logic_error(
            "Try to read from write-only register \"" + registerInfo.getRegisterName() + "\".");
      }
    }

    void doPreWrite(TransferType, VersionNumber) override {
      if(!backend->isOpen()) {
        throw ChimeraTK::logic_error("Write operation not allowed while device is closed.");
      }
      if(!isWriteable()) {
        throw ChimeraTK::logic_error("Try to write read-only register \"" + registerInfo.getRegisterName() + "\".");
      }
    }

    const std::type_info& getValueType() { return typeid(UserType); }

    std::vector<boost::shared_ptr<TransferElement>> getHardwareAccessingElements() override {
      return {boost::enable_shared_from_this<TransferElement>::shared_from_this()};
    }

    std::list<boost::shared_ptr<TransferElement>> getInternalElements() override { return {}; }

    [[nodiscard]] bool isReadOnly() const override { return isReadable() && !isWriteable(); }

    [[nodiscard]] bool isWriteable() const override { return registerInfo.isWriteable(); }

    [[nodiscard]] bool isReadable() const override { return registerInfo.isReadable(); }

    /// Pointer to the backend
    boost::shared_ptr<TangoBackend> backend;
    std::shared_ptr<Tango::DeviceProxy> proxy;
    TangoRegisterInfo registerInfo;
    Tango::DeviceAttribute attr;
    Tango::DeviceAttribute writeAttribute;
    bool isPartial{false};
  };

  template<typename UserType>
  void TangoBackendRegisterAccessor<UserType>::doReadTransferSynchronously() {
    if(!backend->isFunctional()) {
      throw ChimeraTK::runtime_error(std::string("Exception reported by another accessor"));
    }

    try {
      attr = backend->getDeviceProxy()->read_attribute(registerInfo.attributeInfo.name);
    }
    catch(Tango::ConnectionFailed& ex) {
      throw ChimeraTK::runtime_error(
          "Failed to read from attribute " + registerInfo.attributeInfo.name + ": " + ex._name());
    }
  }

  template<typename UserType>
  bool TangoBackendRegisterAccessor<UserType>::doWriteTransfer(VersionNumber) {
    if(!backend->isFunctional()) {
      throw ChimeraTK::runtime_error(std::string("Exception reported by another accessor"));
    }

    try {
      backend->getDeviceProxy()->write_attribute(writeAttribute);
    }
    catch(CORBA::Exception& ex) {
      throw ChimeraTK::runtime_error(
          "Failed to write to attribute " + registerInfo.attributeInfo.name + ": " + ex._name());
    }

    return false;
  }

  template<typename UserType, typename TangoType>
  class TangoBackendScalarRegisterAccessor : public TangoBackendRegisterAccessor<UserType> {
    using TangoBackendRegisterAccessor<UserType>::TangoBackendRegisterAccessor;

    void doPreWrite(TransferType type, VersionNumber version) {
      TangoBackendRegisterAccessor<UserType>::doPreWrite(type, version);
      auto value = ChimeraTK::userTypeToNumeric<TangoType, UserType>(this->buffer_2D[0][0]);
      this->writeAttribute = Tango::DeviceAttribute(this->registerInfo.attributeInfo.name, value);
    }

    void doPostRead(TransferType type, bool hasNewData) {
      TangoBackendRegisterAccessor<UserType>::doPostRead(type, hasNewData);
      if(!hasNewData) {
        return;
      }

      TangoType value;
      this->attr >> value;
      this->buffer_2D[0][0] = ChimeraTK::numericToUserType<UserType>(value);

      // FIXME: We currently have no means of correlating data from Tango, so everything gets a new version number
      TransferElement::_versionNumber = {};
    }
  };

  template<typename UserType, typename TangoType>
  class TangoBackendSpectrumRegisterAccessor : public TangoBackendRegisterAccessor<UserType> {
    using TangoBackendRegisterAccessor<UserType>::TangoBackendRegisterAccessor;

    void doPreWrite(TransferType type, VersionNumber version) {
      TangoBackendRegisterAccessor<UserType>::doPreWrite(type, version);
      std::vector<TangoType> t(10, 0);
      for (unsigned int i = 0; i < this->registerInfo.getNumberOfElements(); i++) {
        t[i] = ChimeraTK::userTypeToNumeric<TangoType, UserType>(this->buffer_2D[0][i]);
      }
      this->writeAttribute = Tango::DeviceAttribute(this->registerInfo.attributeInfo.name, t);
    }

    void doPostRead(TransferType type, bool hasNewData) {
      TangoBackendRegisterAccessor<UserType>::doPostRead(type, hasNewData);
      if(!hasNewData) {
        return;
      }



      std::vector<TangoType> value;
      this->attr >> value;
      auto length = std::min(this->registerInfo.getNumberOfElements(), static_cast<unsigned int>(value.size()));

      for (unsigned int i = 0; i < length; i++) {
        this->buffer_2D[0][i] = ChimeraTK::numericToUserType<UserType>(value[i]);
      }

      // FIXME: We currently have no means of correlating data from Tango, so everything gets a new version number
      TransferElement::_versionNumber = {};
    }
  };
} // namespace ChimeraTK
