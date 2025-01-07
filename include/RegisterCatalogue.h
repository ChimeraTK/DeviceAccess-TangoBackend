// SPDX-FileCopyrightText: Deutsches Elektronen-Synchrotron DESY, MSK, ChimeraTK Project <chimeratk-support@desy.de>
// SPDX-License-Identifier: LGPL-3.0-or-later
#pragma once

#include <tango/tango.h>

#include <ChimeraTK/BackendRegisterCatalogue.h>

namespace ChimeraTK {

  /**********************************************************************************************************************/

  struct TangoRegisterInfo;

  /**********************************************************************************************************************/

  class TangoRegisterCatalogue : public ChimeraTK::BackendRegisterCatalogue<TangoRegisterInfo> {};

  /**********************************************************************************************************************/

  struct TangoRegisterInfo : public ChimeraTK::BackendRegisterInfoBase {
    [[nodiscard]] ChimeraTK::RegisterPath getRegisterName() const override { return ""; }

    [[nodiscard]] unsigned int getNumberOfElements() const override { return 1; }

    [[nodiscard]] unsigned int getNumberOfChannels() const override { return 1; }

    [[nodiscard]] bool isReadable() const override { return true; }

    [[nodiscard]] bool isWriteable() const override { return true; }

    [[nodiscard]] ChimeraTK::AccessModeFlags getSupportedAccessModes() const override { return {}; }

    [[nodiscard]] const ChimeraTK::DataDescriptor& getDataDescriptor() const override { return descriptor; }

    [[nodiscard]] std::unique_ptr<ChimeraTK::BackendRegisterInfoBase> clone() const override;

    DataDescriptor descriptor;
  };
} // namespace ChimeraTK
