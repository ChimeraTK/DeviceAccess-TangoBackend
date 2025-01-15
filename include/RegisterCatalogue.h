// SPDX-FileCopyrightText: Deutsches Elektronen-Synchrotron DESY, MSK, ChimeraTK Project <chimeratk-support@desy.de>
// SPDX-License-Identifier: LGPL-3.0-or-later
#pragma once

#include <tango/tango.h>

#include <ChimeraTK/BackendRegisterCatalogue.h>

namespace util {
  using Dt = ChimeraTK::DataType::TheType;

  static ChimeraTK::DataType mapTangoToUserType(int tangoType) {
    static const std::map<Tango::CmdArgType, ChimeraTK::DataType::TheType> typeMap{
        {Tango::DEV_BOOLEAN, Dt::Boolean},
        {Tango::DEV_SHORT, Dt::int16},
        {Tango::DEV_LONG, Dt::int32},
        {Tango::DEV_FLOAT, Dt::float32},
        {Tango::DEV_DOUBLE, Dt::float64},
        {Tango::DEV_USHORT, Dt::uint16},
        {Tango::DEV_ULONG, Dt::uint32},
        {Tango::DEV_STRING, Dt::string},
        {Tango::DEV_STATE, Dt::uint8},
        {Tango::DEV_UCHAR, Dt::uint8},
        {Tango::DEV_LONG64, Dt::int64},
        {Tango::DEV_ULONG64, Dt::uint64},
        {Tango::DEV_ENUM, Dt::int64},
    };

    try {
      // Compatiblitiy with older tango servers (There used to be DEV_INT which no longer exists)
      // FIXME: Do we really need this or is this handled by Tango
      if(tangoType == 27) {
        tangoType = static_cast<int>(Tango::DEV_LONG);
      }

      return typeMap.at(static_cast<Tango::CmdArgType>(tangoType));
    } catch (std::out_of_range&) {
      throw ChimeraTK::logic_error("Unmappable Tango type " + std::to_string(tangoType));
    }
  }
} // namespace util

namespace ChimeraTK {

  /**********************************************************************************************************************/

  struct TangoRegisterInfo;

  /**********************************************************************************************************************/

  class TangoRegisterCatalogue : public ChimeraTK::BackendRegisterCatalogue<TangoRegisterInfo> {};

  /**********************************************************************************************************************/

  struct TangoRegisterInfo : public ChimeraTK::BackendRegisterInfoBase {
    TangoRegisterInfo() = default;

    explicit TangoRegisterInfo(Tango::AttributeInfoEx& info)
    : descriptor(util::mapTangoToUserType(info.data_type)), attributeInfo(info) {}

    [[nodiscard]] ChimeraTK::RegisterPath getRegisterName() const override { return "/" + attributeInfo.name; }

    [[nodiscard]] unsigned int getNumberOfElements() const override { return attributeInfo.max_dim_x; }

    [[nodiscard]] unsigned int getNumberOfChannels() const override { return attributeInfo.max_dim_y; }

    [[nodiscard]] bool isReadable() const override {
      return attributeInfo.writable == Tango::AttrWriteType::READ_WITH_WRITE ||
          attributeInfo.writable == Tango::AttrWriteType::READ_WRITE ||
          attributeInfo.writable == Tango::AttrWriteType::READ;
    }

    [[nodiscard]] bool isWriteable() const override {
      return attributeInfo.writable == Tango::AttrWriteType::READ_WRITE ||attributeInfo.writable ==
          Tango::AttrWriteType::WRITE;
    }

    [[nodiscard]] ChimeraTK::AccessModeFlags getSupportedAccessModes() const override { return {}; }

    [[nodiscard]] const ChimeraTK::DataDescriptor& getDataDescriptor() const override { return descriptor; }

    [[nodiscard]] std::unique_ptr<ChimeraTK::BackendRegisterInfoBase> clone() const override;

    DataDescriptor descriptor;
    Tango::AttributeInfoEx attributeInfo;
  };
} // namespace ChimeraTK
