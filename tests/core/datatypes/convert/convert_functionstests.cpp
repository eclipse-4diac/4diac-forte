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
#include "forte_boost_output_support.h"

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

    CIEC_TIME_OF_DAY tod(func_DT_TO_TOD(date));
    BOOST_TEST(tod == CIEC_TIME_OF_DAY(((12 * 3600 + 38 * 60 + 50) * 1000ULL + 123) * 1000000ULL));
  }

  BOOST_AUTO_TEST_CASE(CONVERT_DT_TO_LDT) {
    CIEC_DATE_AND_TIME date;
    date.fromString("DT#2018-07-10-12:38:50.123");

    CIEC_LDATE_AND_TIME ldt(func_DT_TO_LDT(date));
    CIEC_LDATE_AND_TIME expected;
    expected.fromString("LDT#2018-07-10-12:38:50.123");
    BOOST_TEST(ldt == expected);
  }

  BOOST_AUTO_TEST_CASE(CONVERT_DT_TO_DATE) {
    CIEC_DATE_AND_TIME date;
    date.fromString("DT#2018-07-10-12:38:50.123");

    CIEC_DATE converted(func_DT_TO_DATE(date));
    CIEC_DATE expected;
    expected.fromString("D#2018-07-10");
    BOOST_TEST(converted == expected);
  }

  BOOST_AUTO_TEST_CASE(CONVERT_DT_TO_LTOD) {
    CIEC_DATE_AND_TIME date;
    date.fromString("DT#2018-07-10-12:38:50.123");

    CIEC_LTIME_OF_DAY converted(func_DT_TO_LTOD(date));
    CIEC_LTIME_OF_DAY expected;
    expected.fromString("LTOD#12:38:50.123");
    BOOST_TEST(converted == expected);
  }

  BOOST_AUTO_TEST_CASE(CONVERT_LDT_TO_DT) {
    CIEC_LDATE_AND_TIME ldt;
    ldt.fromString("LDT#2018-07-10-12:38:50.123");

    CIEC_DATE_AND_TIME dt(func_LDT_TO_DT(ldt));
    CIEC_DATE_AND_TIME expected;
    expected.fromString("DT#2018-07-10-12:38:50.123");
    BOOST_TEST(ldt == expected);
  }

  BOOST_AUTO_TEST_CASE(CONVERT_LDT_TO_LDATE) {
    CIEC_LDATE_AND_TIME date;
    date.fromString("LDT#2018-07-10-12:38:50.123");

    CIEC_LDATE converted(func_LDT_TO_LDATE(date));
    CIEC_LDATE expected;
    expected.fromString("LD#2018-07-10");
    BOOST_TEST(converted == expected);
  }

  BOOST_AUTO_TEST_CASE(CONVERT_LDT_TO_DATE) {
    CIEC_LDATE_AND_TIME date;
    date.fromString("LDT#2018-07-10-12:38:50.123");

    CIEC_DATE converted(func_LDT_TO_DATE(date));
    CIEC_DATE expected;
    expected.fromString("D#2018-07-10");
    BOOST_TEST(converted == expected);
  }

  BOOST_AUTO_TEST_CASE(CONVERT_LDT_TO_LTOD) {
    CIEC_LDATE_AND_TIME date;
    date.fromString("LDT#2018-07-10-12:38:50.123");

    CIEC_LTIME_OF_DAY converted(func_LDT_TO_LTOD(date));
    CIEC_LTIME_OF_DAY expected;
    expected.fromString("LTOD#12:38:50.123");
    BOOST_TEST(converted == expected);
  }

  BOOST_AUTO_TEST_CASE(CONVERT_LDT_TO_TOD) {
    CIEC_LDATE_AND_TIME date;
    date.fromString("LDT#2018-07-10-12:38:50.123");

    CIEC_TIME_OF_DAY converted(func_LDT_TO_TOD(date));
    CIEC_TIME_OF_DAY expected;
    expected.fromString("TOD#12:38:50.123");
    BOOST_TEST(converted == expected);
  }

  BOOST_AUTO_TEST_CASE(CONVERT_TIME_TO_LTIME) {
    CIEC_TIME value;
    value.fromString("TIME#12h34m56s789ms");

    CIEC_LTIME converted(func_TIME_TO_LTIME(value));
    CIEC_LTIME expected;
    expected.fromString("LTIME#12h34m56s789ms");
    BOOST_TEST(converted == expected);
  }

  BOOST_AUTO_TEST_CASE(CONVERT_LTIME_TO_TIME) {
    CIEC_LTIME value;
    value.fromString("LTIME#12h34m56s789ms");

    CIEC_TIME converted(func_LTIME_TO_TIME(value));
    CIEC_TIME expected;
    expected.fromString("TIME#12h34m56s789ms");
    BOOST_TEST(converted == expected);
  }

  BOOST_AUTO_TEST_CASE(CONVERT_REAL_TO_XINT) {
    CIEC_REAL roundUpReal(50.5f);
    CIEC_REAL roundDownReal(50.4f);

    BOOST_TEST(func_REAL_TO_SINT(roundUpReal) == 51);
    BOOST_TEST(func_REAL_TO_SINT(roundDownReal) == 50);
    BOOST_TEST(func_REAL_TO_INT(roundUpReal) == 51);
    BOOST_TEST(func_REAL_TO_INT(roundDownReal) == 50);
    BOOST_TEST(func_REAL_TO_DINT(roundUpReal) == 51);
    BOOST_TEST(func_REAL_TO_DINT(roundDownReal) == 50);
    BOOST_TEST(func_REAL_TO_LINT(roundUpReal) == 51);
    BOOST_TEST(func_REAL_TO_LINT(roundDownReal) == 50);

    BOOST_TEST(func_REAL_TO_USINT(roundUpReal) == 51);
    BOOST_TEST(func_REAL_TO_USINT(roundDownReal) == 50);
    BOOST_TEST(func_REAL_TO_UINT(roundUpReal) == 51);
    BOOST_TEST(func_REAL_TO_UINT(roundDownReal) == 50);
    BOOST_TEST(func_REAL_TO_UDINT(roundUpReal) == 51);
    BOOST_TEST(func_REAL_TO_UDINT(roundDownReal) == 50);
    BOOST_TEST(func_REAL_TO_ULINT(roundUpReal) == 51);
    BOOST_TEST(func_REAL_TO_ULINT(roundDownReal) == 50);
  }

  BOOST_AUTO_TEST_CASE(CONVERT_LREAL_TO_XINT) {
    CIEC_LREAL roundUpReal(50.5f);
    CIEC_LREAL roundDownReal(50.4f);

    BOOST_TEST(func_LREAL_TO_SINT(roundUpReal) == 51);
    BOOST_TEST(func_LREAL_TO_SINT(roundDownReal) == 50);
    BOOST_TEST(func_LREAL_TO_INT(roundUpReal) == 51);
    BOOST_TEST(func_LREAL_TO_INT(roundDownReal) == 50);
    BOOST_TEST(func_LREAL_TO_DINT(roundUpReal) == 51);
    BOOST_TEST(func_LREAL_TO_DINT(roundDownReal) == 50);
    BOOST_TEST(func_LREAL_TO_LINT(roundUpReal) == 51);
    BOOST_TEST(func_LREAL_TO_LINT(roundDownReal) == 50);

    BOOST_TEST(func_LREAL_TO_USINT(roundUpReal) == 51);
    BOOST_TEST(func_LREAL_TO_USINT(roundDownReal) == 50);
    BOOST_TEST(func_LREAL_TO_UINT(roundUpReal) == 51);
    BOOST_TEST(func_LREAL_TO_UINT(roundDownReal) == 50);
    BOOST_TEST(func_LREAL_TO_UDINT(roundUpReal) == 51);
    BOOST_TEST(func_LREAL_TO_UDINT(roundDownReal) == 50);
    BOOST_TEST(func_LREAL_TO_ULINT(roundUpReal) == 51);
    BOOST_TEST(func_LREAL_TO_ULINT(roundDownReal) == 50);
  }

  BOOST_AUTO_TEST_CASE(CONVERT_TIME_TO_S_LINT) {
    CIEC_TIME time;
    time.fromString("T#2d5h43m12s");
    CIEC_LINT timeInS;
    timeInS = func_TIME_IN_S_TO_LINT(time);
    BOOST_TEST(timeInS == (INT64_C(172800) + INT64_C(18000) + INT64_C(2580) + INT64_C(12)));
  }

  BOOST_AUTO_TEST_CASE(CONVERT_TIME_TO_MS_LINT) {
    CIEC_TIME time;
    time.fromString("T#2d5h43m12s44ms");
    CIEC_LINT timeInMs;
    timeInMs = func_TIME_IN_MS_TO_LINT(time);
    BOOST_TEST(timeInMs == (INT64_C(172800000) + INT64_C(18000000) + INT64_C(2580000) + INT64_C(12000) + INT64_C(44)));
  }

  BOOST_AUTO_TEST_CASE(CONVERT_TIME_TO_US_LINT) {
    CIEC_TIME time;
    time.fromString("T#2d5h43m12s44ms27us");
    CIEC_LINT timeInUs;
    timeInUs = func_TIME_IN_US_TO_LINT(time);
    BOOST_TEST(timeInUs == (INT64_C(172800000000) + INT64_C(18000000000) + INT64_C(2580000000) + INT64_C(12000000) + INT64_C(44000) + INT64_C(27)));
  }

  BOOST_AUTO_TEST_CASE(CONVERT_TIME_TO_NS_LINT) {
    CIEC_TIME time;
    time.fromString("T#2d5h43m12s44ms27us31ns");
    CIEC_LINT timeInNs;
    timeInNs = func_TIME_IN_NS_TO_LINT(time);
    BOOST_TEST(timeInNs == (INT64_C(172800000000000) + INT64_C(18000000000000) + INT64_C(2580000000000) + INT64_C(12000000000) + INT64_C(44000000) + INT64_C(27000) + INT64_C(31)));
  }

  BOOST_AUTO_TEST_CASE(CONVERT_TIME_TO_S_ULINT) {
    CIEC_TIME time;
    time.fromString("T#2d5h43m12s");
    CIEC_ULINT timeInS;
    timeInS = func_TIME_IN_S_TO_ULINT(time);
    BOOST_TEST(timeInS == (UINT64_C(172800) + UINT64_C(18000) + UINT64_C(2580) + UINT64_C(12)));
  }

  BOOST_AUTO_TEST_CASE(CONVERT_TIME_TO_MS_ULINT) {
    CIEC_TIME time;
    time.fromString("T#2d5h43m12s44ms");
    CIEC_ULINT timeInMs;
    timeInMs = func_TIME_IN_MS_TO_ULINT(time);
    BOOST_TEST(timeInMs == (UINT64_C(172800000) + UINT64_C(18000000) + UINT64_C(2580000) + UINT64_C(12000) + UINT64_C(44)));
  }

  BOOST_AUTO_TEST_CASE(CONVERT_TIME_TO_US_ULINT) {
    CIEC_TIME time;
    time.fromString("T#2d5h43m12s44ms27us");
    CIEC_ULINT timeInUs;
    timeInUs = func_TIME_IN_US_TO_ULINT(time);
    BOOST_TEST(timeInUs == (UINT64_C(172800000000) + UINT64_C(18000000000) + UINT64_C(2580000000) + UINT64_C(12000000) + UINT64_C(44000) + UINT64_C(27)));
  }

  BOOST_AUTO_TEST_CASE(CONVERT_TIME_TO_NS_ULINT) {
    CIEC_TIME time;
    time.fromString("T#2d5h43m12s44ms27us31ns");
    CIEC_ULINT timeInNs;
    timeInNs = func_TIME_IN_NS_TO_ULINT(time);
    BOOST_TEST(timeInNs == (UINT64_C(172800000000000) + UINT64_C(18000000000000) + UINT64_C(2580000000000) + UINT64_C(12000000000) + UINT64_C(44000000) + UINT64_C(27000) + UINT64_C(31)));
  }

  BOOST_AUTO_TEST_CASE(CONVERT_TIME_TO_S_LREAL) {
    CIEC_TIME time;
    time.fromString("T#2d5h43m12s44ms27us31ns");
    CIEC_LREAL timeInS;
    timeInS = func_TIME_IN_S_TO_LREAL(time);
    BOOST_TEST(timeInS == static_cast<CIEC_LREAL>(INT64_C(172800000000000) + INT64_C(18000000000000) + INT64_C(2580000000000) + INT64_C(12000000000) + INT64_C(44000000) + INT64_C(27000) + INT64_C(31)) / static_cast<CIEC_LREAL>(1E9));
  }

  BOOST_AUTO_TEST_CASE(CONVERT_TIME_TO_MS_LREAL) {
    CIEC_TIME time;
    time.fromString("T#2d5h43m12s44ms27us31ns");
    CIEC_LREAL timeInMs;
    timeInMs = func_TIME_IN_MS_TO_LREAL(time);
    BOOST_TEST(
      timeInMs == static_cast<CIEC_LREAL>(INT64_C(172800000000000) + INT64_C(18000000000000) + INT64_C(2580000000000) + INT64_C(12000000000) + INT64_C(44000000) + INT64_C(27000) + INT64_C(31)) / static_cast<CIEC_LREAL>(1E6));
  }

  BOOST_AUTO_TEST_CASE(CONVERT_TIME_TO_US_LREAL) {
    CIEC_TIME time;
    time.fromString("T#2d5h43m12s44ms27us31ns");
    CIEC_LREAL timeInUs;
    timeInUs = func_TIME_IN_US_TO_LREAL(time);
    BOOST_TEST(
      timeInUs == static_cast<CIEC_LREAL>(INT64_C(172800000000000) + INT64_C(18000000000000) + INT64_C(2580000000000) + INT64_C(12000000000) + INT64_C(44000000) + INT64_C(27000) + INT64_C(31)) / static_cast<CIEC_LREAL>(1E3));
  }

  BOOST_AUTO_TEST_CASE(CONVERT_TIME_TO_NS_LREAL) {
    CIEC_TIME time;
    time.fromString("T#2d5h43m12s44ms27us31ns");
    CIEC_LREAL timeInNs;
    timeInNs = func_TIME_IN_NS_TO_LREAL(time);
    BOOST_TEST(
      timeInNs == static_cast<CIEC_LREAL>(INT64_C(172800000000000) + INT64_C(18000000000000) + INT64_C(2580000000000) + INT64_C(12000000000) + INT64_C(44000000) + INT64_C(27000) + INT64_C(31)));
  }
  BOOST_AUTO_TEST_SUITE_END()
