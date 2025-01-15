// SPDX-FileCopyrightText: Deutsches Elektronen-Synchrotron DESY, MSK, ChimeraTK Project <chimeratk-support@desy.de>
// SPDX-License-Identifier: LGPL-3.0-or-later
#pragma once

#include "RegisterCatalogue.h"

#include <ChimeraTK/DeviceBackend.h>
#include <ChimeraTK/DeviceBackendImpl.h>

namespace Tango {
  class DeviceProxy;
} // namespace Tango

namespace ChimeraTK {
  class TangoBackend : public DeviceBackendImpl {
   public:
    static boost::shared_ptr<DeviceBackend> createInstance(
        std::string address, std::map<std::string, std::string> parameters);

    template<typename UserType>
    boost::shared_ptr<NDRegisterAccessor<UserType>> getRegisterAccessor_impl(
        const RegisterPath& registerPathName, size_t numberOfWords, size_t wordOffsetInRegister, AccessModeFlags flags);
    DEFINE_VIRTUAL_FUNCTION_TEMPLATE_VTABLE_FILLER(DoocsBackend, getRegisterAccessor_impl, 4);

    explicit TangoBackend(std::string address);

    void open() override;
    void close() override;

    RegisterCatalogue getRegisterCatalogue() const override;

    std::string readDeviceInfo() override;

    std::shared_ptr<Tango::DeviceProxy> getDeviceProxy() { return _deviceProxy; }

   private:
    std::string _address;
    std::shared_ptr<Tango::DeviceProxy> _deviceProxy;
    mutable TangoRegisterCatalogue _registerCatalogue{};
    DataDescriptor mapTangoType();
  };
} // namespace ChimeraTK
