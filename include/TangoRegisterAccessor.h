// SPDX-FileCopyrightText: Deutsches Elektronen-Synchrotron DESY, MSK, ChimeraTK Project <chimeratk-support@desy.de>
// SPDX-License-Identifier: LGPL-3.0-or-later
#pragma once

#include "TangoBackend.h"
#include <tango/tango.h>

#include <ChimeraTK/NDRegisterAccessor.h>

#include <utility>

namespace ChimeraTK {

  template<typename UserType, typename TangoType>
  class TangoBackendRegisterAccessor : public NDRegisterAccessor<UserType> {
   public:
    TangoBackendRegisterAccessor(boost::shared_ptr<TangoBackend> backend_, TangoRegisterInfo info,
        const std::string& registerPathName, size_t numberOfWords, size_t wordOffsetInRegister, AccessModeFlags flags)
    : NDRegisterAccessor<UserType>(registerPathName, flags), backend(std::move(backend_)),
      registerInfo(std::move(info)) {
      auto actualLength = registerInfo.getNumberOfElements();
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

    void doPostRead(TransferType type, bool hasNewData) override;
    void doPreWrite(TransferType, VersionNumber) override;

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

  template<typename UserType, typename TangoType>
  void TangoBackendRegisterAccessor<UserType, TangoType>::doReadTransferSynchronously() {
    if(!backend->isFunctional()) {
      throw ChimeraTK::runtime_error(std::string("Exception reported by another accessor"));
    }

    try {
      attr = backend->getDeviceProxy()->read_attribute(registerInfo.attributeInfo.name);

      std::vector<TangoType> value;
      this->attr >> value;
    }
    catch(CORBA::Exception& ex) {
      throw ChimeraTK::runtime_error(
          "Failed to read from attribute " + registerInfo.attributeInfo.name + ": " + ex._name());
    }
  }

  template<typename UserType, typename TangoType>
  bool TangoBackendRegisterAccessor<UserType, TangoType>::doWriteTransfer(VersionNumber) {
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
  void TangoBackendRegisterAccessor<UserType, TangoType>::doPreWrite(TransferType type, VersionNumber version) {
    NDRegisterAccessor<UserType>::doPreWrite(type, version);
    if(!backend->isOpen()) {
      throw ChimeraTK::logic_error("Write operation not allowed while device is closed.");
    }
    if(!isWriteable()) {
      throw ChimeraTK::logic_error("Try to write read-only register \"" + registerInfo.getRegisterName() + "\".");
    }

    std::vector<TangoType> value(this->registerInfo.getNumberOfElements());

    std::transform(this->buffer_2D[0].begin(), this->buffer_2D[0].end(), value.begin(),
        ChimeraTK::userTypeToUserType<TangoType, UserType>);

    this->writeAttribute = Tango::DeviceAttribute(this->registerInfo.attributeInfo.name, value);
  }

  template<typename UserType, typename TangoType>
  void TangoBackendRegisterAccessor<UserType, TangoType>::doPostRead(TransferType type, bool hasNewData) {
    NDRegisterAccessor<UserType>::doPostRead(type, hasNewData);
    if(!hasNewData) {
      return;
    }

    std::vector<TangoType> value;
    try {
      this->attr >> value;
    }
    catch(Tango::DevFailed& ex) {
      throw ChimeraTK::runtime_error("Failed to read from attribute " + registerInfo.attributeInfo.name + ": ");
    }

    auto length = std::min(this->registerInfo.getNumberOfElements(), static_cast<unsigned int>(value.size()));
    std::transform(value.begin(), std::next(value.begin(), length), this->buffer_2D[0].begin(),
        ChimeraTK::userTypeToUserType<UserType, TangoType>);

    // FIXME: We currently have no means of correlating data from Tango, so everything gets a new version number
    TransferElement::_versionNumber = {};
  }

} // namespace ChimeraTK
