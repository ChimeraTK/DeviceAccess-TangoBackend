// SPDX-FileCopyrightText: Deutsches Elektronen-Synchrotron DESY, MSK, ChimeraTK Project <chimeratk-support@desy.de>
// SPDX-License-Identifier: LGPL-3.0-or-later
#pragma once

#include "RegisterCatalogue.h"
#include <nlohmann/json.hpp>
#include <tango/tango.h>

#include <string>
namespace ChimeraTK {

  class OfflineCache {
   public:
    explicit OfflineCache(std::string cacheFilePath) : _cacheFilePath(std::move(cacheFilePath)) {}

    TangoRegisterCatalogue read();

   private:
    std::string _cacheFilePath;
  };
} // namespace ChimeraTK
