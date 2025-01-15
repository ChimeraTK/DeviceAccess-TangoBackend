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

      if(nElements + elementOffset > actualLength) {
        throw ChimeraTK::logic_error(
            "Requested number of words exceeds the length of the Tango attribute " + registerPathName);
      }

      //auto isPartial = nElements != actualLength || elementOffset != 0;

      NDRegisterAccessor<UserType>::buffer_2D.resize(1);
      NDRegisterAccessor<UserType>::buffer_2D[0].resize(nElements);
    }
    void doReadTransferSynchronously() override;
    bool doWriteTransfer(VersionNumber) override;

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
  };

  template<typename UserType>
  void TangoBackendRegisterAccessor<UserType>::doReadTransferSynchronously() {
    if(!backend->isFunctional()) {
      throw ChimeraTK::runtime_error(std::string("Exception reported by another accessor"));
    }

    try {
      attr = backend->getDeviceProxy()->read_attribute(registerInfo.attributeInfo.name);
    }
    catch(CORBA::Exception& ex) {
      throw ChimeraTK::runtime_error(
          "Failed to read from attribute " + registerInfo.attributeInfo.name + ": " + ex._name());
    }
  }

  template<typename UserType>
  bool TangoBackendRegisterAccessor<UserType>::doWriteTransfer(VersionNumber) {
    throw ChimeraTK::logic_error("Not implemented");
  }


} // namespace ChimeraTK
