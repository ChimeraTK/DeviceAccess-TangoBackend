// SPDX-FileCopyrightText: Deutsches Elektronen-Synchrotron DESY, MSK, ChimeraTK Project <chimeratk-support@desy.de>
// SPDX-License-Identifier: LGPL-3.0-or-later
#pragma once

#include "TangoBackend.h"
#include <omniORB4/CORBA.h>
#include <tango/idl/tango.h>
#include <tango/tango.h>

#include <ChimeraTK/Exception.h>
#include <ChimeraTK/NDRegisterAccessor.h>
#include <ChimeraTK/TransferElement.h>

#include <iterator>
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
      elementOffset = wordOffsetInRegister;
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
    TangoRegisterInfo registerInfo;

    // This will be filled by the readTransfer to be processed in doPostRead
    Tango::DeviceAttribute readAttribute;

    // This will be filled by preWrite to be sent to the server in doWriteTransfer
    Tango::DeviceAttribute writeAttribute;
    bool isPartial{false};

    size_t elementOffset;
  };

  /********************************************************************************************************************/

  template<typename UserType, typename TangoType>
  void TangoBackendRegisterAccessor<UserType, TangoType>::doReadTransferSynchronously() {
    if(!backend->isFunctional()) {
      throw ChimeraTK::runtime_error(std::string("Exception reported by another accessor"));
    }

    try {
      readAttribute = backend->getDeviceProxy()->read_attribute(registerInfo.attributeInfo.name);
    }
    catch(CORBA::Exception& ex) {
      throw ChimeraTK::runtime_error(
          "Failed to read from attribute " + registerInfo.attributeInfo.name + ": " + ex._name());
    }
  }

  /********************************************************************************************************************/

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

  /********************************************************************************************************************/

  template<typename UserType, typename TangoType>
  void TangoBackendRegisterAccessor<UserType, TangoType>::doPreWrite(TransferType type, VersionNumber version) {
    NDRegisterAccessor<UserType>::doPreWrite(type, version);
    if(!backend->isOpen()) {
      throw ChimeraTK::logic_error("Write operation not allowed while device is closed.");
    }
    if(!isWriteable()) {
      throw ChimeraTK::logic_error("Try to write read-only register \"" + registerInfo.getRegisterName() + "\".");
    }

    std::vector<TangoType> value;

    // Perform read-modify-write
    if(isPartial) {
      try {
        auto attr = backend->getDeviceProxy()->read_attribute(registerInfo.attributeInfo.name);
        attr >> value;
      }
      catch(CORBA::Exception& ex) {
        throw ChimeraTK::runtime_error(
            "Failure to read in read-modfiy-write of partial accessor " + registerInfo.getRegisterName());
      }
    }

    // Some versions of Tango return one element too much, so we do this unconditionally, even for read-modify-write
    value.resize(this->registerInfo.getNumberOfElements());

    auto destinationStart = value.begin() + elementOffset;

    if constexpr(std::is_same_v<TangoType, Tango::DevEnum>) {
      std::transform(this->buffer_2D[0].begin(), this->buffer_2D[0].end(), destinationStart,
          ChimeraTK::userTypeToNumeric<Tango::DevEnum, UserType>);
    }
    else if constexpr(std::is_same_v<TangoType, Tango::DevState>) {
      // Technically we should never end up here, unless someone added an additional attribute
      // That uses DevState and isn't the built-in "State" which is r/o
      std::transform(this->buffer_2D[0].begin(), this->buffer_2D[0].end(), destinationStart,
          [](UserType& v) { return static_cast<Tango::DevState>(ChimeraTK::userTypeToNumeric<int, UserType>(v)); });
    }
    else {
      std::transform(this->buffer_2D[0].begin(), this->buffer_2D[0].end(), destinationStart,
          ChimeraTK::userTypeToUserType<TangoType, UserType>);
    }
    this->writeAttribute = Tango::DeviceAttribute(this->registerInfo.attributeInfo.name, value);
  }

  /********************************************************************************************************************/

  template<typename UserType, typename TangoType>
  void TangoBackendRegisterAccessor<UserType, TangoType>::doPostRead(TransferType type, bool hasNewData) {
    NDRegisterAccessor<UserType>::doPostRead(type, hasNewData);
    if(!hasNewData) {
      return;
    }

    // If the Attribute Quality is invalid, data extraction will not work. There is no data to read.
    if(this->readAttribute.get_quality() != Tango::AttrQuality::ATTR_INVALID) {
      std::vector<TangoType> value;
      try {
        this->readAttribute >> value;
      }
      catch(Tango::DevFailed& ex) {
        throw ChimeraTK::runtime_error("Failed to read from attribute " + registerInfo.attributeInfo.name);
      }

      auto length = std::min(this->buffer_2D[0].size(), value.size());
      auto sourceStart = value.begin() + elementOffset;
      auto sourceEnd = sourceStart + length;
      if constexpr(std::is_same_v<TangoType, Tango::DevEnum>) {
        std::transform(
            sourceStart, sourceEnd, this->buffer_2D[0].begin(), ChimeraTK::numericToUserType<UserType, Tango::DevEnum>);
      }
      if constexpr(std::is_same_v<TangoType, Tango::DevState>) {
        std::transform(sourceStart, sourceEnd, this->buffer_2D[0].begin(),
            [](TangoType& v) { return ChimeraTK::numericToUserType<UserType, int>(static_cast<int>(v)); });
      }
      else {
        std::transform(
            sourceStart, sourceEnd, this->buffer_2D[0].begin(), ChimeraTK::userTypeToUserType<UserType, TangoType>);
      }
    }

    // FIXME: We currently have no means of correlating data from Tango, so everything gets a new version number
    TransferElement::_versionNumber = {};

    // FIXME: Not really sure if CHANGING is also valid, but it is definitely more valid than all the others.
    TransferElement::_dataValidity = (this->readAttribute.quality == Tango::AttrQuality::ATTR_VALID ||
                                         this->readAttribute.quality == Tango::AttrQuality::ATTR_CHANGING) ?
        DataValidity::ok :
        DataValidity::faulty;
  }

} // namespace ChimeraTK
