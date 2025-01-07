// SPDX-FileCopyrightText: Deutsches Elektronen-Synchrotron DESY, MSK, ChimeraTK Project <chimeratk-support@desy.de>
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "RegisterCatalogue.h"

namespace ChimeraTK {
  std::unique_ptr<ChimeraTK::BackendRegisterInfoBase> TangoRegisterInfo::clone() const {
    return std::unique_ptr<ChimeraTK::BackendRegisterInfoBase>(new TangoRegisterInfo(*this));
  }
} // namespace ChimeraTK
