// SPDX-FileCopyrightText: Deutsches Elektronen-Synchrotron DESY, MSK, ChimeraTK Project <chimeratk-support@desy.de>
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "OfflineCache.h"

namespace ChimeraTK {

  TangoRegisterCatalogue OfflineCache::read() {
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

    auto catalogue = TangoRegisterCatalogue();
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
      catalogue.addRegister(TangoRegisterInfo(info));
    }

    return catalogue;
  }
} // namespace ChimeraTK
