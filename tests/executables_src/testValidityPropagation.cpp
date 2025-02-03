// SPDX-FileCopyrightText: Deutsches Elektronen-Synchrotron DESY, MSK, ChimeraTK Project <chimeratk-support@desy.de>
// SPDX-License-Identifier: LGPL-3.0-or-later
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE testValidity

#include "TangoServerLauncher.h"

#define BOOST_NO_EXCEPTIONS
#include <boost/test/unit_test.hpp>
#undef BOOST_NO_EXCEPTIONS

using namespace boost::unit_test_framework;

BOOST_GLOBAL_FIXTURE(TangoServerLauncher);

#include <ChimeraTK/Device.h>

BOOST_AUTO_TEST_CASE(testValidityFromServer) {
    auto cdd = "(tango: " + TangoServerLauncher::self->getClientUrl() + ")";
    auto device = ChimeraTK::Device(cdd);

    BOOST_CHECK_NO_THROW(device.open());

    
 {
    auto accessor = device.getScalarRegisterAccessor<uint64_t>("InvalidValue");
    BOOST_CHECK_NO_THROW(accessor.read());
    BOOST_TEST(accessor.dataValidity() == ChimeraTK::DataValidity::faulty);
}

{
    auto accessor = device.getScalarRegisterAccessor<uint64_t>("AlarmValue");
    BOOST_CHECK_NO_THROW(accessor.read());
    BOOST_TEST(accessor.dataValidity() == ChimeraTK::DataValidity::faulty);
}

{
    auto accessor = device.getScalarRegisterAccessor<uint64_t>("WarningValue");
    BOOST_CHECK_NO_THROW(accessor.read());
    BOOST_TEST(accessor.dataValidity() == ChimeraTK::DataValidity::faulty);
}
}