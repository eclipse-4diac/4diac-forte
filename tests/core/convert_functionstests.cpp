/*******************************************************************************
 * Copyright (c) 2018 TU Vienna/ACIN
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
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

  BOOST_AUTO_TEST_SUITE_END()
