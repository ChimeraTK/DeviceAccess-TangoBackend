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
    TangoBackendRegisterAccessor(boost::shared_ptr<TangoBackend> backend_, const std::string& registerPathName,
        size_t numberOfWords, size_t wordOffsetInRegister, AccessModeFlags flags)
    : NDRegisterAccessor<UserType>(registerPathName, flags),
      backend(std::move(backend_)) {}
    void doReadTransferSynchronously() override;
    bool doWriteTransfer(VersionNumber) override;

    const std::type_info& getValueType() { return typeid(UserType); }

    std::vector<boost::shared_ptr<TransferElement>> getHardwareAccessingElements() override {
      return {boost::enable_shared_from_this<TransferElement>::shared_from_this()};
    }

    std::list<boost::shared_ptr<TransferElement>> getInternalElements() override { return {}; }

    [[nodiscard]] bool isReadOnly() const override { return isReadable() && !isWriteable(); }

    [[nodiscard]] bool isWriteable() const override { return true; }

    [[nodiscard]] bool isReadable() const override { return true; }


    /// Pointer to the backend
    boost::shared_ptr<TangoBackend> backend;
    std::shared_ptr<Tango::DeviceProxy> proxy;
  };

  template<typename UserType>
  void TangoBackendRegisterAccessor<UserType>::doReadTransferSynchronously() {
    if(!backend->isFunctional()) {
      throw ChimeraTK::runtime_error(std::string("Exception reported by another accessor"));
    }

    throw ChimeraTK::logic_error("Not implemented");
  }

  template<typename UserType>
  bool TangoBackendRegisterAccessor<UserType>::doWriteTransfer(VersionNumber) {
    throw ChimeraTK::logic_error("Not implemented");
  }


} // namespace ChimeraTK
