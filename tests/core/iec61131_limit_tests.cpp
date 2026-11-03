/*******************************************************************************
 * Copyright (c) 2026 Martin Melik Merkumians
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *******************************************************************************/

#include "forte/iec61131_functions/func_LIMIT.h"

#include <boost/test/unit_test.hpp>

#include "forte/datatypes/forte_int.h"

using namespace forte::literals;

namespace forte::test {
  BOOST_AUTO_TEST_SUITE(IEC61131_LIMIT)

  BOOST_AUTO_TEST_CASE(limit_int) {
    BOOST_TEST(static_cast<CIEC_INT::TValueType>(func_LIMIT(0_INT, -5_INT, 10_INT)) == 0);
    BOOST_TEST(static_cast<CIEC_INT::TValueType>(func_LIMIT(0_INT, 5_INT, 10_INT)) == 5);
    BOOST_TEST(static_cast<CIEC_INT::TValueType>(func_LIMIT(0_INT, 15_INT, 10_INT)) == 10);
    BOOST_TEST(static_cast<CIEC_INT::TValueType>(func_LIMIT(0_INT, 0_INT, 10_INT)) == 0);
    BOOST_TEST(static_cast<CIEC_INT::TValueType>(func_LIMIT(0_INT, 10_INT, 10_INT)) == 10);
  }

  BOOST_AUTO_TEST_CASE(limit_int_inverted_bounds) {
    BOOST_TEST(static_cast<CIEC_INT::TValueType>(func_LIMIT(10_INT, -5_INT, 0_INT)) == 0);
    BOOST_TEST(static_cast<CIEC_INT::TValueType>(func_LIMIT(10_INT, 5_INT, 0_INT)) == 0);
    BOOST_TEST(static_cast<CIEC_INT::TValueType>(func_LIMIT(10_INT, 15_INT, 0_INT)) == 0);
  }

  BOOST_AUTO_TEST_SUITE_END()
} // namespace forte::test
