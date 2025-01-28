// SPDX-FileCopyrightText: Deutsches Elektronen-Synchrotron DESY, MSK, ChimeraTK Project <chimeratk-support@desy.de>
// SPDX-License-Identifier: LGPL-3.0-or-later
#pragma once

#include "RegisterCatalogue.h"

#include <tango/tango.h>

#include <nlohmann/json.hpp>

#include <string>
namespace ChimeraTK {

  class OfflineCache {
   public:
    explicit OfflineCache(const std::string& cacheFilePath) : _cacheFilePath(cacheFilePath) {}

    TangoRegisterCatalogue&& read() {
      std::ifstream f(_cacheFilePath);
      nlohmann::json data;
      try {
        data = nlohmann::json::parse(f);
      }
      catch(nlohmann::json::exception& ex) {
        throw ChimeraTK::logic_error("Failed to parse " + _cacheFilePath + ": " + ex.what());
      }

      auto registers = data["catalogue"];

      if(!data["catalogue"].is_array()) {
        throw ChimeraTK::logic_error("malformed cache file, missing \"catalogue\" " + _cacheFilePath);
      }

      auto catalogue = std::make_unique<TangoRegisterCatalogue>();
      for(auto& reg : registers) {
        Tango::AttributeInfoEx info;
        info.name = reg["name"].get<std::string>();
        info.max_dim_x = reg["length"].get<int>();
        info.max_dim_y = reg["channels"].get<int>();
        info.data_type = reg["tangoTypeId"].get<int>();
        info.writable = static_cast<Tango::AttrWriteType>(reg["writable"].get<int>());
        if(info.max_dim_x == 1 && info.max_dim_y == 0) {
          info.data_format = Tango::AttrDataFormat::SCALAR;
        }
        else if(info.max_dim_x > 1 && info.max_dim_y == 0) {
          info.data_format = Tango::AttrDataFormat::SPECTRUM;
        }
        else if(info.max_dim_x > 1 && info.max_dim_y > 0) {
          info.data_format = Tango::AttrDataFormat::IMAGE;
        }
        catalogue->addRegister(TangoRegisterInfo(info));
      }

      return std::move(*catalogue.release());
    }

   private:
    std::string _cacheFilePath;
  };
} // namespace ChimeraTK
