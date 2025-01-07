// SPDX-FileCopyrightText: Deutsches Elektronen-Synchrotron DESY, MSK, ChimeraTK Project <chimeratk-support@desy.de>
// SPDX-License-Identifier: LGPL-3.0-or-later
#pragma once

#include <tango/tango.h>

#include <ChimeraTK/BackendRegisterCatalogue.h>

namespace ChimeraTK {

  /********************************************************************************************************************/

  struct TangoRegisterInfo;

  /********************************************************************************************************************/

  class TangoRegisterCatalogue : public ChimeraTK::BackendRegisterCatalogue<TangoRegisterInfo> {};

  /********************************************************************************************************************/

  struct TangoRegisterInfo : public ChimeraTK::BackendRegisterInfoBase {
    TangoRegisterInfo() = default;

    explicit TangoRegisterInfo(Tango::AttributeInfoEx& info);

    [[nodiscard]] ChimeraTK::RegisterPath getRegisterName() const override { return "/" + attributeInfo.name; }

    [[nodiscard]] unsigned int getNumberOfElements() const override { return attributeInfo.max_dim_x; }

    [[nodiscard]] unsigned int getNumberOfChannels() const override { return attributeInfo.max_dim_y; }

    [[nodiscard]] bool isReadable() const override {
      return attributeInfo.writable == Tango::AttrWriteType::READ_WITH_WRITE ||
          attributeInfo.writable == Tango::AttrWriteType::READ_WRITE ||
          attributeInfo.writable == Tango::AttrWriteType::READ;
    }

    [[nodiscard]] bool isWriteable() const override {
      return attributeInfo.writable == Tango::AttrWriteType::READ_WRITE ||
          attributeInfo.writable == Tango::AttrWriteType::WRITE;
    }

    [[nodiscard]] ChimeraTK::AccessModeFlags getSupportedAccessModes() const override { return {}; }

    [[nodiscard]] const ChimeraTK::DataDescriptor& getDataDescriptor() const override { return descriptor; }

    [[nodiscard]] std::unique_ptr<ChimeraTK::BackendRegisterInfoBase> clone() const override;

    DataDescriptor descriptor;
    Tango::AttributeInfoEx attributeInfo;
  };
} // namespace ChimeraTK
