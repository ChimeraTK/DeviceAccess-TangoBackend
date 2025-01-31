// SPDX-FileCopyrightText: Deutsches Elektronen-Synchrotron DESY, MSK, ChimeraTK Project <chimeratk-support@desy.de>
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "TangoServerLauncher.h"

#include <boost/test/unit_test_suite.hpp>
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE testUnifiedBackendTest

#define BOOST_NO_EXCEPTIONS
#include <boost/test/unit_test.hpp>
#undef BOOST_NO_EXCEPTIONS

using namespace boost::unit_test_framework;

BOOST_GLOBAL_FIXTURE(TangoServerLauncher);

BOOST_AUTO_TEST_SUITE(testValidity)

BOOST_AUTO_TEST_CASE(testValidityToServer) {}

BOOST_AUTO_TEST_SUITE_END()