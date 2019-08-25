/*******************************************************************************
 * Copyright (c) 2018 TU Vienna/ACIN
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Martin Melik Merkumians - initial API and implementation and/or initial documentation
 *   Martin Melik-Merkumians - adds tests for REAL and LREAL casts
 *******************************************************************************/

#include <boost/test/unit_test.hpp>

#include "convert_functions.h"

#include "forte_sint.h"
#include "forte_lint.h"
#include "forte_real.h"
#include "forte_date_and_time.h"
#include "forte_time_of_day.h"

BOOST_AUTO_TEST_SUITE(convert_functions)

  BOOST_AUTO_TEST_CASE(CONVERT_DT_TO_TOD) {
    CIEC_DATE_AND_TIME date;
    date.fromString("DT#2018-07-10-12:38:50.123");

    CIEC_TIME_OF_DAY tod(DT_TO_TOD(date));
    BOOST_TEST((TForteUInt64)tod == (12*3600 + 38*60 + 50)*1000ULL + 123);
  }

  BOOST_AUTO_TEST_CASE(CONVERT_REAL_TO_XINT) {
    CIEC_REAL roundUpReal(50.5f);
    CIEC_REAL roundDownReal(50.4f);

    BOOST_TEST(REAL_TO_SINT(roundUpReal) == 51);
    BOOST_TEST(REAL_TO_SINT(roundDownReal) == 50);
    BOOST_TEST(REAL_TO_INT(roundUpReal) == 51);
    BOOST_TEST(REAL_TO_INT(roundDownReal) == 50);
    BOOST_TEST(REAL_TO_DINT(roundUpReal) == 51);
    BOOST_TEST(REAL_TO_DINT(roundDownReal) == 50);
    BOOST_TEST(REAL_TO_LINT(roundUpReal) == 51);
    BOOST_TEST(REAL_TO_LINT(roundDownReal) == 50);

    BOOST_TEST(REAL_TO_USINT(roundUpReal) == 51);
    BOOST_TEST(REAL_TO_USINT(roundDownReal) == 50);
    BOOST_TEST(REAL_TO_UINT(roundUpReal) == 51);
    BOOST_TEST(REAL_TO_UINT(roundDownReal) == 50);
    BOOST_TEST(REAL_TO_UDINT(roundUpReal) == 51);
    BOOST_TEST(REAL_TO_UDINT(roundDownReal) == 50);
    BOOST_TEST(REAL_TO_ULINT(roundUpReal) == 51);
    BOOST_TEST(REAL_TO_ULINT(roundDownReal) == 50);
  }

  BOOST_AUTO_TEST_CASE(CONVERT_LREAL_TO_XINT) {
    CIEC_LREAL roundUpReal(50.5f);
    CIEC_LREAL roundDownReal(50.4f);

    BOOST_TEST(LREAL_TO_SINT(roundUpReal) == 51);
    BOOST_TEST(LREAL_TO_SINT(roundDownReal) == 50);
    BOOST_TEST(LREAL_TO_INT(roundUpReal) == 51);
    BOOST_TEST(LREAL_TO_INT(roundDownReal) == 50);
    BOOST_TEST(LREAL_TO_DINT(roundUpReal) == 51);
    BOOST_TEST(LREAL_TO_DINT(roundDownReal) == 50);
    BOOST_TEST(LREAL_TO_LINT(roundUpReal) == 51);
    BOOST_TEST(LREAL_TO_LINT(roundDownReal) == 50);

    BOOST_TEST(LREAL_TO_USINT(roundUpReal) == 51);
    BOOST_TEST(LREAL_TO_USINT(roundDownReal) == 50);
    BOOST_TEST(LREAL_TO_UINT(roundUpReal) == 51);
    BOOST_TEST(LREAL_TO_UINT(roundDownReal) == 50);
    BOOST_TEST(LREAL_TO_UDINT(roundUpReal) == 51);
    BOOST_TEST(LREAL_TO_UDINT(roundDownReal) == 50);
    BOOST_TEST(LREAL_TO_ULINT(roundUpReal) == 51);
    BOOST_TEST(LREAL_TO_ULINT(roundDownReal) == 50);
  }

  BOOST_AUTO_TEST_CASE(CONVERT_TIME_TO_S_LINT) {
    CIEC_TIME time("T#2d5h43m12s");
    CIEC_LINT timeInS;
    timeInS = TIME_IN_S_TO_LINT(time);
    BOOST_TEST(timeInS == (INT64_C(172800) + INT64_C(18000) + INT64_C(2580) + INT64_C(12)));
  }

  BOOST_AUTO_TEST_CASE(CONVERT_TIME_TO_MS_LINT) {
    CIEC_TIME time("T#2d5h43m12s44ms");
    CIEC_LINT timeInMs;
    timeInMs = TIME_IN_MS_TO_LINT(time);
    BOOST_TEST(timeInMs == (INT64_C(172800000) + INT64_C(18000000) + INT64_C(2580000) + INT64_C(12000) + INT64_C(44)));
  }

  BOOST_AUTO_TEST_CASE(CONVERT_TIME_TO_US_LINT) {
    CIEC_TIME time("T#2d5h43m12s44ms27us");
    CIEC_LINT timeInUs;
    timeInUs = TIME_IN_US_TO_LINT(time);
    BOOST_TEST(timeInUs == (INT64_C(172800000000) + INT64_C(18000000000) + INT64_C(2580000000) + INT64_C(12000000) + INT64_C(44000) + INT64_C(27)));
  }

  BOOST_AUTO_TEST_CASE(CONVERT_TIME_TO_NS_LINT) {
    CIEC_TIME time("T#2d5h43m12s44ms27us31ns");
    CIEC_LINT timeInNs;
    timeInNs = TIME_IN_NS_TO_LINT(time);
    BOOST_TEST(timeInNs == (INT64_C(172800000000000) + INT64_C(18000000000000) + INT64_C(2580000000000) + INT64_C(12000000000) + INT64_C(44000000) + INT64_C(27000) + INT64_C(31)));
  }

  BOOST_AUTO_TEST_CASE(CONVERT_TIME_TO_S_ULINT) {
    CIEC_TIME time("T#2d5h43m12s");
    CIEC_ULINT timeInS;
    timeInS = TIME_IN_S_TO_ULINT(time);
    BOOST_TEST(timeInS == (UINT64_C(172800) + UINT64_C(18000) + UINT64_C(2580) + UINT64_C(12)));
  }

  BOOST_AUTO_TEST_CASE(CONVERT_TIME_TO_MS_ULINT) {
    CIEC_TIME time("T#2d5h43m12s44ms");
    CIEC_ULINT timeInMs;
    timeInMs = TIME_IN_MS_TO_ULINT(time);
    BOOST_TEST(timeInMs == (UINT64_C(172800000) + UINT64_C(18000000) + UINT64_C(2580000) + UINT64_C(12000) + UINT64_C(44)));
  }

  BOOST_AUTO_TEST_CASE(CONVERT_TIME_TO_US_ULINT) {
    CIEC_TIME time("T#2d5h43m12s44ms27us");
    CIEC_ULINT timeInUs;
    timeInUs = TIME_IN_US_TO_ULINT(time);
    BOOST_TEST(timeInUs == (UINT64_C(172800000000) + UINT64_C(18000000000) + UINT64_C(2580000000) + UINT64_C(12000000) + UINT64_C(44000) + UINT64_C(27)));
  }

  BOOST_AUTO_TEST_CASE(CONVERT_TIME_TO_NS_ULINT) {
    CIEC_TIME time("T#2d5h43m12s44ms27us31ns");
    CIEC_ULINT timeInNs;
    timeInNs = TIME_IN_NS_TO_ULINT(time);
    BOOST_TEST(timeInNs == (UINT64_C(172800000000000) + UINT64_C(18000000000000) + UINT64_C(2580000000000) + UINT64_C(12000000000) + UINT64_C(44000000) + UINT64_C(27000) + UINT64_C(31)));
  }

  BOOST_AUTO_TEST_CASE(CONVERT_TIME_TO_S_LREAL) {
    CIEC_TIME time("T#2d5h43m12s44ms27us31ns");
    CIEC_LREAL timeInS;
    timeInS = TIME_IN_S_TO_LREAL(time);
    BOOST_TEST(timeInS == static_cast<CIEC_LREAL>(INT64_C(172800000000000) + INT64_C(18000000000000) + INT64_C(2580000000000) + INT64_C(12000000000) + INT64_C(44000000) + INT64_C(27000) + INT64_C(31)) / static_cast<CIEC_LREAL>(1E9));
  }

  BOOST_AUTO_TEST_CASE(CONVERT_TIME_TO_MS_LREAL) {
    CIEC_TIME time("T#2d5h43m12s44ms27us31ns");
    CIEC_LREAL timeInMs;
    timeInMs = TIME_IN_MS_TO_LREAL(time);
    BOOST_TEST(
      timeInMs == static_cast<CIEC_LREAL>(INT64_C(172800000000000) + INT64_C(18000000000000) + INT64_C(2580000000000) + INT64_C(12000000000) + INT64_C(44000000) + INT64_C(27000) + INT64_C(31)) / static_cast<CIEC_LREAL>(1E6));
  }

  BOOST_AUTO_TEST_CASE(CONVERT_TIME_TO_US_LREAL) {
    CIEC_TIME time("T#2d5h43m12s44ms27us31ns");
    CIEC_LREAL timeInUs;
    timeInUs = TIME_IN_US_TO_LREAL(time);
    BOOST_TEST(
      timeInUs == static_cast<CIEC_LREAL>(INT64_C(172800000000000) + INT64_C(18000000000000) + INT64_C(2580000000000) + INT64_C(12000000000) + INT64_C(44000000) + INT64_C(27000) + INT64_C(31)) / static_cast<CIEC_LREAL>(1E3));
  }

  BOOST_AUTO_TEST_CASE(CONVERT_TIME_TO_NS_LREAL) {
    CIEC_TIME time("T#2d5h43m12s44ms27us31ns");
    CIEC_LREAL timeInNs;
    timeInNs = TIME_IN_NS_TO_LREAL(time);
    BOOST_TEST(
      timeInNs == static_cast<CIEC_LREAL>(INT64_C(172800000000000) + INT64_C(18000000000000) + INT64_C(2580000000000) + INT64_C(12000000000) + INT64_C(44000000) + INT64_C(27000) + INT64_C(31)));
  }
  BOOST_AUTO_TEST_SUITE_END()
