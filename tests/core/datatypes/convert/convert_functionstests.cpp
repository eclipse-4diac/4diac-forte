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

  BOOST_AUTO_TEST_CASE(cast_lword_to_bool) {
    CIEC_LWORD trueResult(0xFF);
    CIEC_LWORD falseResult(0xFE);

    BOOST_TEST(func_LWORD_TO_BOOL(trueResult) == CIEC_BOOL(true));
    BOOST_TEST(func_LWORD_TO_BOOL(falseResult) == CIEC_BOOL(false));
  }

  BOOST_AUTO_TEST_CASE(cast_dword_to_bool) {
    CIEC_DWORD trueResult(0xFF);
    CIEC_DWORD falseResult(0xFE);

    BOOST_TEST(func_DWORD_TO_BOOL(trueResult) == CIEC_BOOL(true));
    BOOST_TEST(func_DWORD_TO_BOOL(falseResult) == CIEC_BOOL(false));
  }

  BOOST_AUTO_TEST_CASE(cast_word_to_bool) {
    CIEC_WORD trueResult(0xFF);
    CIEC_WORD falseResult(0xFE);

    BOOST_TEST(func_WORD_TO_BOOL(trueResult) == CIEC_BOOL(true));
    BOOST_TEST(func_WORD_TO_BOOL(falseResult) == CIEC_BOOL(false));
  }

  BOOST_AUTO_TEST_CASE(cast_byte_to_bool) {
    CIEC_BYTE trueResult(0xFF);
    CIEC_BYTE falseResult(0xFE);

    BOOST_TEST(func_BYTE_TO_BOOL(trueResult) == CIEC_BOOL(true));
    BOOST_TEST(func_BYTE_TO_BOOL(falseResult) == CIEC_BOOL(false));
  }

  BOOST_AUTO_TEST_CASE(cast_word_to_int_then_int_to_dint_sign_extension_test) {
    CIEC_WORD inWord(0xFFFF);
    CIEC_INT intermediateInt;
    CIEC_DINT result;
    intermediateInt = func_WORD_TO_INT(inWord);
    result = func_INT_TO_DINT(intermediateInt);
    BOOST_TEST(static_cast<CIEC_DINT::TValueType>(result) == -1);
  }

  BOOST_AUTO_TEST_CASE(cast_sint_to_byte_only_binary_transfer) {
    CIEC_SINT inSint(-1);
    CIEC_BYTE result;

    result = func_SINT_TO_BYTE(inSint);
    BOOST_TEST(static_cast<CIEC_BYTE::TValueType>(result) == 0xFF);
  }

  BOOST_AUTO_TEST_CASE(cast_sint_to_word_only_binary_transfer) {
    CIEC_SINT inSint(-1);
    CIEC_WORD result;

    result = func_SINT_TO_WORD(inSint);
    BOOST_TEST(static_cast<CIEC_WORD::TValueType>(result) == 0xFF);
  }

  BOOST_AUTO_TEST_CASE(cast_sint_to_dword_only_binary_transfer) {
    CIEC_SINT inSint(-1);
    CIEC_DWORD result;

    result = func_SINT_TO_DWORD(inSint);
    BOOST_TEST(static_cast<CIEC_DWORD::TValueType>(result) == 0xFF);
  }

  BOOST_AUTO_TEST_CASE(cast_sint_to_lword_only_binary_transfer) {
    CIEC_SINT inSint(-1);
    CIEC_LWORD result;

    result = func_SINT_TO_LWORD(inSint);
    BOOST_TEST(static_cast<CIEC_LWORD::TValueType>(result) == 0xFF);
  }

  BOOST_AUTO_TEST_CASE(cast_int_to_dword_only_binary_transfer) {
    CIEC_INT inInt(-1);
    CIEC_DWORD result;

    result = func_INT_TO_DWORD(inInt);
    BOOST_TEST(static_cast<CIEC_DWORD::TValueType>(result) == 0xFFFF);
  }

  BOOST_AUTO_TEST_CASE(cast_dint_to_dword_only_binary_transfer) {
    CIEC_DINT inDint(-1);
    CIEC_DWORD result;

    result = func_DINT_TO_DWORD(inDint);
    BOOST_TEST(static_cast<CIEC_DWORD::TValueType>(result) == 0xFFFFFFFF);
  }

  BOOST_AUTO_TEST_CASE(up_and_down_cast_from_lword_to_bool_and_back) {
    CIEC_LWORD inLword(0xFFFFFFFFFFFFFFFF);
    CIEC_LWORD result;

    result = func_BOOL_TO_LWORD(func_LWORD_TO_BOOL(inLword));
    BOOST_TEST(static_cast<CIEC_DWORD::TValueType>(result) == 1);
  }

  BOOST_AUTO_TEST_CASE(up_and_down_cast_from_lword_to_byte_and_back) {
    CIEC_LWORD inLword(0xFFFFFFFFFFFFFFFF);
    CIEC_LWORD result;

    result = func_BYTE_TO_LWORD(func_LWORD_TO_BYTE(inLword));
    BOOST_TEST(static_cast<CIEC_DWORD::TValueType>(result) == 0xFF);
  }

  BOOST_AUTO_TEST_CASE(up_and_down_cast_from_lword_to_word_and_back) {
    CIEC_LWORD inLword(0xFFFFFFFFFFFFFFFF);
    CIEC_LWORD result;

    result = func_WORD_TO_LWORD(func_LWORD_TO_WORD(inLword));
    BOOST_TEST(static_cast<CIEC_DWORD::TValueType>(result) == 0xFFFF);
  }

  BOOST_AUTO_TEST_CASE(up_and_down_cast_from_lword_to_dword_and_back) {
    CIEC_LWORD inLword(0xFFFFFFFFFFFFFFFF);
    CIEC_LWORD result;

    result = func_DWORD_TO_LWORD(func_LWORD_TO_DWORD(inLword));
    BOOST_TEST(static_cast<CIEC_DWORD::TValueType>(result) == 0xFFFFFFFF);
  }

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

  BOOST_AUTO_TEST_CASE(CONVERT_DT_TO_DATE_BEGIN_OF_DAY) {
    CIEC_DATE_AND_TIME date;
    date.fromString("DT#2018-07-10-00:00:00");

    CIEC_DATE converted(func_DT_TO_DATE(date));
    CIEC_DATE expected;
    expected.fromString("D#2018-07-10");
    BOOST_TEST(converted == expected);
  }

  BOOST_AUTO_TEST_CASE(CONVERT_DT_TO_DATE_END_OF_DAY) {
    CIEC_DATE_AND_TIME date;
    date.fromString("DT#2018-07-10-23:59:59");

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

  BOOST_AUTO_TEST_CASE(CONVERT_LDT_TO_LDATE_BEGIN_OF_DAY) {
    CIEC_LDATE_AND_TIME date;
    date.fromString("LDT#2018-07-10-00:00:00");

    CIEC_LDATE converted(func_LDT_TO_LDATE(date));
    CIEC_LDATE expected;
    expected.fromString("LD#2018-07-10");
    BOOST_TEST(converted == expected);
  }

  BOOST_AUTO_TEST_CASE(CONVERT_LDT_TO_DATE_BEGIN_OF_DAY) {
    CIEC_LDATE_AND_TIME date;
    date.fromString("LDT#2018-07-10-00:00:00");

    CIEC_DATE converted(func_LDT_TO_DATE(date));
    CIEC_DATE expected;
    expected.fromString("D#2018-07-10");
    BOOST_TEST(converted == expected);
  }

  BOOST_AUTO_TEST_CASE(CONVERT_LDT_TO_LDATE_END_OF_DAY) {
    CIEC_LDATE_AND_TIME date;
    date.fromString("LDT#2018-07-10-23:59:59");

    CIEC_LDATE converted(func_LDT_TO_LDATE(date));
    CIEC_LDATE expected;
    expected.fromString("LD#2018-07-10");
    BOOST_TEST(converted == expected);
  }

  BOOST_AUTO_TEST_CASE(CONVERT_LDT_TO_DATE_END_OF_DAY) {
    CIEC_LDATE_AND_TIME date;
    date.fromString("LDT#2018-07-10-23:59:59");

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

    BOOST_TEST(static_cast<CIEC_SINT::TValueType>(func_REAL_TO_SINT(roundUpReal)) == 51);
    BOOST_TEST(static_cast<CIEC_SINT::TValueType>(func_REAL_TO_SINT(roundDownReal)) == 50);
    BOOST_TEST(static_cast<CIEC_INT::TValueType>(func_REAL_TO_INT(roundUpReal)) == 51);
    BOOST_TEST(static_cast<CIEC_INT::TValueType>(func_REAL_TO_INT(roundDownReal)) == 50);
    BOOST_TEST(static_cast<CIEC_DINT::TValueType>(func_REAL_TO_DINT(roundUpReal)) == 51);
    BOOST_TEST(static_cast<CIEC_DINT::TValueType>(func_REAL_TO_DINT(roundDownReal)) == 50);
    BOOST_TEST(static_cast<CIEC_LINT::TValueType>(func_REAL_TO_LINT(roundUpReal)) == 51);
    BOOST_TEST(static_cast<CIEC_LINT::TValueType>(func_REAL_TO_LINT(roundDownReal)) == 50);

    BOOST_TEST(static_cast<CIEC_USINT::TValueType>(func_REAL_TO_USINT(roundUpReal)) == 51);
    BOOST_TEST(static_cast<CIEC_USINT::TValueType>(func_REAL_TO_USINT(roundDownReal)) == 50);
    BOOST_TEST(static_cast<CIEC_UINT::TValueType>(func_REAL_TO_UINT(roundUpReal)) == 51);
    BOOST_TEST(static_cast<CIEC_UINT::TValueType>(func_REAL_TO_UINT(roundDownReal)) == 50);
    BOOST_TEST(static_cast<CIEC_UDINT::TValueType>(func_REAL_TO_UDINT(roundUpReal)) == 51);
    BOOST_TEST(static_cast<CIEC_UDINT::TValueType>(func_REAL_TO_UDINT(roundDownReal)) == 50);
    BOOST_TEST(static_cast<CIEC_ULINT::TValueType>(func_REAL_TO_ULINT(roundUpReal)) == 51);
    BOOST_TEST(static_cast<CIEC_ULINT::TValueType>(func_REAL_TO_ULINT(roundDownReal)) == 50);
  }

  BOOST_AUTO_TEST_CASE(CONVERT_LREAL_TO_XINT) {
    CIEC_LREAL roundUpReal(50.5f);
    CIEC_LREAL roundDownReal(50.4f);

    BOOST_TEST(static_cast<CIEC_SINT::TValueType>(func_LREAL_TO_SINT(roundUpReal)) == 51);
    BOOST_TEST(static_cast<CIEC_SINT::TValueType>(func_LREAL_TO_SINT(roundDownReal)) == 50);
    BOOST_TEST(static_cast<CIEC_INT::TValueType>(func_LREAL_TO_INT(roundUpReal)) == 51);
    BOOST_TEST(static_cast<CIEC_INT::TValueType>(func_LREAL_TO_INT(roundDownReal)) == 50);
    BOOST_TEST(static_cast<CIEC_DINT::TValueType>(func_LREAL_TO_DINT(roundUpReal)) == 51);
    BOOST_TEST(static_cast<CIEC_DINT::TValueType>(func_LREAL_TO_DINT(roundDownReal)) == 50);
    BOOST_TEST(static_cast<CIEC_LINT::TValueType>(func_LREAL_TO_LINT(roundUpReal)) == 51);
    BOOST_TEST(static_cast<CIEC_LINT::TValueType>(func_LREAL_TO_LINT(roundDownReal)) == 50);

    BOOST_TEST(static_cast<CIEC_USINT::TValueType>(func_LREAL_TO_USINT(roundUpReal)) == 51);
    BOOST_TEST(static_cast<CIEC_USINT::TValueType>(func_LREAL_TO_USINT(roundDownReal)) == 50);
    BOOST_TEST(static_cast<CIEC_UINT::TValueType>(func_LREAL_TO_UINT(roundUpReal)) == 51);
    BOOST_TEST(static_cast<CIEC_UINT::TValueType>(func_LREAL_TO_UINT(roundDownReal)) == 50);
    BOOST_TEST(static_cast<CIEC_UDINT::TValueType>(func_LREAL_TO_UDINT(roundUpReal)) == 51);
    BOOST_TEST(static_cast<CIEC_UDINT::TValueType>(func_LREAL_TO_UDINT(roundDownReal)) == 50);
    BOOST_TEST(static_cast<CIEC_ULINT::TValueType>(func_LREAL_TO_ULINT(roundUpReal)) == 51);
    BOOST_TEST(static_cast<CIEC_ULINT::TValueType>(func_LREAL_TO_ULINT(roundDownReal)) == 50);
  }

  BOOST_AUTO_TEST_CASE(CONVERT_TIME_TO_S_LINT) {
    CIEC_TIME time;
    time.fromString("T#2d5h43m12s");
    CIEC_LINT timeInS;
    timeInS = func_TIME_IN_S_TO_LINT(time);
    BOOST_TEST(static_cast<CIEC_LINT::TValueType>(timeInS) == (INT64_C(172800) + INT64_C(18000) + INT64_C(2580) + INT64_C(12)));
  }

  BOOST_AUTO_TEST_CASE(CONVERT_TIME_TO_MS_LINT) {
    CIEC_TIME time;
    time.fromString("T#2d5h43m12s44ms");
    CIEC_LINT timeInMs;
    timeInMs = func_TIME_IN_MS_TO_LINT(time);
    BOOST_TEST(static_cast<CIEC_LINT::TValueType>(timeInMs) == (INT64_C(172800000) + INT64_C(18000000) + INT64_C(2580000) + INT64_C(12000) + INT64_C(44)));
  }

  BOOST_AUTO_TEST_CASE(CONVERT_TIME_TO_US_LINT) {
    CIEC_TIME time;
    time.fromString("T#2d5h43m12s44ms27us");
    CIEC_LINT timeInUs;
    timeInUs = func_TIME_IN_US_TO_LINT(time);
    BOOST_TEST(static_cast<CIEC_LINT::TValueType>(timeInUs) == (INT64_C(172800000000) + INT64_C(18000000000) + INT64_C(2580000000) + INT64_C(12000000) + INT64_C(44000) + INT64_C(27)));
  }

  BOOST_AUTO_TEST_CASE(CONVERT_TIME_TO_NS_LINT) {
    CIEC_TIME time;
    time.fromString("T#2d5h43m12s44ms27us31ns");
    CIEC_LINT timeInNs;
    timeInNs = func_TIME_IN_NS_TO_LINT(time);
    BOOST_TEST(static_cast<CIEC_LINT::TValueType>(timeInNs) == (INT64_C(172800000000000) + INT64_C(18000000000000) + INT64_C(2580000000000) + INT64_C(12000000000) + INT64_C(44000000) + INT64_C(27000) + INT64_C(31)));
  }

  BOOST_AUTO_TEST_CASE(CONVERT_TIME_TO_S_ULINT) {
    CIEC_TIME time;
    time.fromString("T#2d5h43m12s");
    CIEC_ULINT timeInS;
    timeInS = func_TIME_IN_S_TO_ULINT(time);
    BOOST_TEST(static_cast<CIEC_ULINT::TValueType>(timeInS) == (UINT64_C(172800) + UINT64_C(18000) + UINT64_C(2580) + UINT64_C(12)));
  }

  BOOST_AUTO_TEST_CASE(CONVERT_TIME_TO_MS_ULINT) {
    CIEC_TIME time;
    time.fromString("T#2d5h43m12s44ms");
    CIEC_ULINT timeInMs;
    timeInMs = func_TIME_IN_MS_TO_ULINT(time);
    BOOST_TEST(static_cast<CIEC_ULINT::TValueType>(timeInMs) == (UINT64_C(172800000) + UINT64_C(18000000) + UINT64_C(2580000) + UINT64_C(12000) + UINT64_C(44)));
  }

  BOOST_AUTO_TEST_CASE(CONVERT_TIME_TO_US_ULINT) {
    CIEC_TIME time;
    time.fromString("T#2d5h43m12s44ms27us");
    CIEC_ULINT timeInUs;
    timeInUs = func_TIME_IN_US_TO_ULINT(time);
    BOOST_TEST(static_cast<CIEC_ULINT::TValueType>(timeInUs) == (UINT64_C(172800000000) + UINT64_C(18000000000) + UINT64_C(2580000000) + UINT64_C(12000000) + UINT64_C(44000) + UINT64_C(27)));
  }

  BOOST_AUTO_TEST_CASE(CONVERT_TIME_TO_NS_ULINT) {
    CIEC_TIME time;
    time.fromString("T#2d5h43m12s44ms27us31ns");
    CIEC_ULINT timeInNs;
    timeInNs = func_TIME_IN_NS_TO_ULINT(time);
    BOOST_TEST(static_cast<CIEC_ULINT::TValueType>(timeInNs) == (UINT64_C(172800000000000) + UINT64_C(18000000000000) + UINT64_C(2580000000000) + UINT64_C(12000000000) + UINT64_C(44000000) + UINT64_C(27000) + UINT64_C(31)));
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
  
  /**** BYTE_BCD_TO_* ****/
  BOOST_AUTO_TEST_CASE(BYTE_BCD_TO_USINT) {
    using SourceType = CIEC_BYTE;
    using ResultType = CIEC_USINT;
    SourceType source1(0x11);
    SourceType source2(0x31);
    SourceType source3(0x36);
    ResultType result;
    result = func_BYTE_BCD_TO_USINT(source1);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 11);
    result = func_BYTE_BCD_TO_USINT(source2);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 31);
    result = func_BYTE_BCD_TO_USINT(source3);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 36);
  }

  BOOST_AUTO_TEST_CASE(BYTE_BCD_TO_UINT) {
    using SourceType = CIEC_BYTE;
    using ResultType = CIEC_UINT;
    SourceType source1(0x11);
    SourceType source2(0x31);
    SourceType source3(0x36);
    ResultType result;
    result = func_BYTE_BCD_TO_UINT(source1);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 11);
    result = func_BYTE_BCD_TO_UINT(source2);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 31);
    result = func_BYTE_BCD_TO_UINT(source3);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 36);
  }

  BOOST_AUTO_TEST_CASE(BYTE_BCD_TO_UDINT) {
    using SourceType = CIEC_BYTE;
    using ResultType = CIEC_UDINT;
    SourceType source1(0x11);
    SourceType source2(0x31);
    SourceType source3(0x36);
    ResultType result;
    result = func_BYTE_BCD_TO_UDINT(source1);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 11);
    result = func_BYTE_BCD_TO_UDINT(source2);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 31);
    result = func_BYTE_BCD_TO_UDINT(source3);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 36);
  }

  BOOST_AUTO_TEST_CASE(BYTE_BCD_TO_ULINT) {
    using SourceType = CIEC_BYTE;
    using ResultType = CIEC_ULINT;
    SourceType source1(0x11);
    SourceType source2(0x31);
    SourceType source3(0x36);
    ResultType result;
    result = func_BYTE_BCD_TO_ULINT(source1);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 11);
    result = func_BYTE_BCD_TO_ULINT(source2);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 31);
    result = func_BYTE_BCD_TO_ULINT(source3);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 36);
  }

  /**** WORD_BCD_TO_* ****/
  BOOST_AUTO_TEST_CASE(WORD_BCD_TO_USINT) {
    using SourceType = CIEC_WORD;
    using ResultType = CIEC_USINT;
    SourceType source1(0x11);
    SourceType source2(0x31);
    SourceType source3(0x36);
    SourceType source4(0x256);
    SourceType source5(0x257);
    ResultType result;
    result = func_WORD_BCD_TO_USINT(source1);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 11);
    result = func_WORD_BCD_TO_USINT(source2);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 31);
    result = func_WORD_BCD_TO_USINT(source3);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 36);
    result = func_WORD_BCD_TO_USINT(source4);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 0);
    result = func_WORD_BCD_TO_USINT(source5);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 1);
  }

  BOOST_AUTO_TEST_CASE(WORD_BCD_TO_UINT) {
    using SourceType = CIEC_WORD;
    using ResultType = CIEC_UINT;
    SourceType source1(0x11);
    SourceType source2(0x31);
    SourceType source3(0x36);
    SourceType source4(0x256);
    SourceType source5(0x257);
    ResultType result;
    result = func_WORD_BCD_TO_UINT(source1);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 11);
    result = func_WORD_BCD_TO_UINT(source2);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 31);
    result = func_WORD_BCD_TO_UINT(source3);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 36);
    result = func_WORD_BCD_TO_UINT(source4);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 256);
    result = func_WORD_BCD_TO_UINT(source5);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 257);
  }

  BOOST_AUTO_TEST_CASE(WORD_BCD_TO_UDINT) {
    using SourceType = CIEC_WORD;
    using ResultType = CIEC_UDINT;
    SourceType source1(0x11);
    SourceType source2(0x31);
    SourceType source3(0x36);
    SourceType source4(0x256);
    SourceType source5(0x257);
    ResultType result;
    result = func_WORD_BCD_TO_UDINT(source1);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 11);
    result = func_WORD_BCD_TO_UDINT(source2);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 31);
    result = func_WORD_BCD_TO_UDINT(source3);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 36);
    result = func_WORD_BCD_TO_UDINT(source4);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 256);
    result = func_WORD_BCD_TO_UDINT(source5);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 257);
  }

  BOOST_AUTO_TEST_CASE(WORD_BCD_TO_ULINT) {
    using SourceType = CIEC_WORD;
    using ResultType = CIEC_ULINT;
    SourceType source1(0x11);
    SourceType source2(0x31);
    SourceType source3(0x36);
    SourceType source4(0x256);
    SourceType source5(0x257);
    ResultType result;
    result = func_WORD_BCD_TO_ULINT(source1);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 11);
    result = func_WORD_BCD_TO_ULINT(source2);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 31);
    result = func_WORD_BCD_TO_ULINT(source3);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 36);
    result = func_WORD_BCD_TO_ULINT(source4);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 256);
    result = func_WORD_BCD_TO_ULINT(source5);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 257);
  }

  /**** DWORD_BCD_TO_* ****/
  BOOST_AUTO_TEST_CASE(DWORD_BCD_TO_USINT) {
    using SourceType = CIEC_DWORD;
    using ResultType = CIEC_USINT;
    SourceType source1(0x11);
    SourceType source2(0x31);
    SourceType source3(0x36);
    SourceType source4(0x256);
    SourceType source5(0x257);
    ResultType result;
    result = func_DWORD_BCD_TO_USINT(source1);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 11);
    result = func_DWORD_BCD_TO_USINT(source2);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 31);
    result = func_DWORD_BCD_TO_USINT(source3);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 36);
    result = func_DWORD_BCD_TO_USINT(source4);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 0);
    result = func_DWORD_BCD_TO_USINT(source5);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 1);
  }

  BOOST_AUTO_TEST_CASE(DWORD_BCD_TO_UINT) {
    using SourceType = CIEC_DWORD;
    using ResultType = CIEC_UINT;
    SourceType source1(0x11);
    SourceType source2(0x31);
    SourceType source3(0x36);
    SourceType source4(0x256);
    SourceType source5(0x257);
    ResultType result;
    result = func_DWORD_BCD_TO_UINT(source1);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 11);
    result = func_DWORD_BCD_TO_UINT(source2);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 31);
    result = func_DWORD_BCD_TO_UINT(source3);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 36);
    result = func_DWORD_BCD_TO_UINT(source4);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 256);
    result = func_DWORD_BCD_TO_UINT(source5);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 257);
  }

  BOOST_AUTO_TEST_CASE(DWORD_BCD_TO_UDINT) {
    using SourceType = CIEC_DWORD;
    using ResultType = CIEC_UDINT;
    SourceType source1(0x11);
    SourceType source2(0x31);
    SourceType source3(0x36);
    SourceType source4(0x256);
    SourceType source5(0x257);
    ResultType result;
    result = func_DWORD_BCD_TO_UDINT(source1);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 11);
    result = func_DWORD_BCD_TO_UDINT(source2);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 31);
    result = func_DWORD_BCD_TO_UDINT(source3);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 36);
    result = func_DWORD_BCD_TO_UDINT(source4);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 256);
    result = func_DWORD_BCD_TO_UDINT(source5);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 257);
  }

  BOOST_AUTO_TEST_CASE(DWORD_BCD_TO_ULINT) {
    using SourceType = CIEC_DWORD;
    using ResultType = CIEC_ULINT;
    SourceType source1(0x11);
    SourceType source2(0x31);
    SourceType source3(0x36);
    SourceType source4(0x256);
    SourceType source5(0x257);
    ResultType result;
    result = func_DWORD_BCD_TO_ULINT(source1);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 11);
    result = func_DWORD_BCD_TO_ULINT(source2);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 31);
    result = func_DWORD_BCD_TO_ULINT(source3);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 36);
    result = func_DWORD_BCD_TO_ULINT(source4);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 256);
    result = func_DWORD_BCD_TO_ULINT(source5);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 257);
  }

  /**** LWORD_BCD_TO_* ****/
  BOOST_AUTO_TEST_CASE(LWORD_BCD_TO_USINT) {
    using SourceType = CIEC_LWORD;
    using ResultType = CIEC_USINT;
    SourceType source1(0x11);
    SourceType source2(0x31);
    SourceType source3(0x36);
    SourceType source4(0x256);
    SourceType source5(0x257);
    ResultType result;
    result = func_LWORD_BCD_TO_USINT(source1);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 11);
    result = func_LWORD_BCD_TO_USINT(source2);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 31);
    result = func_LWORD_BCD_TO_USINT(source3);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 36);
    result = func_LWORD_BCD_TO_USINT(source4);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 0);
    result = func_LWORD_BCD_TO_USINT(source5);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 1);
  }

  BOOST_AUTO_TEST_CASE(LWORD_BCD_TO_UINT) {
    using SourceType = CIEC_LWORD;
    using ResultType = CIEC_UINT;
    SourceType source1(0x11);
    SourceType source2(0x31);
    SourceType source3(0x36);
    SourceType source4(0x256);
    SourceType source5(0x257);
    ResultType result;
    result = func_LWORD_BCD_TO_UINT(source1);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 11);
    result = func_LWORD_BCD_TO_UINT(source2);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 31);
    result = func_LWORD_BCD_TO_UINT(source3);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 36);
    result = func_LWORD_BCD_TO_UINT(source4);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 256);
    result = func_LWORD_BCD_TO_UINT(source5);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 257);
  }

  BOOST_AUTO_TEST_CASE(LWORD_BCD_TO_UDINT) {
    using SourceType = CIEC_LWORD;
    using ResultType = CIEC_UDINT;
    SourceType source1(0x11);
    SourceType source2(0x31);
    SourceType source3(0x36);
    SourceType source4(0x256);
    SourceType source5(0x257);
    ResultType result;
    result = func_LWORD_BCD_TO_UDINT(source1);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 11);
    result = func_LWORD_BCD_TO_UDINT(source2);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 31);
    result = func_LWORD_BCD_TO_UDINT(source3);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 36);
    result = func_LWORD_BCD_TO_UDINT(source4);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 256);
    result = func_LWORD_BCD_TO_UDINT(source5);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 257);
  }

  BOOST_AUTO_TEST_CASE(LWORD_BCD_TO_ULINT) {
    using SourceType = CIEC_LWORD;
    using ResultType = CIEC_ULINT;
    SourceType source1(0x11);
    SourceType source2(0x31);
    SourceType source3(0x36);
    SourceType source4(0x256);
    SourceType source5(0x257);
    ResultType result;
    result = func_LWORD_BCD_TO_ULINT(source1);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 11);
    result = func_LWORD_BCD_TO_ULINT(source2);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 31);
    result = func_LWORD_BCD_TO_ULINT(source3);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 36);
    result = func_LWORD_BCD_TO_ULINT(source4);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 256);
    result = func_LWORD_BCD_TO_ULINT(source5);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 257);
  }

    /**** *_BCD_TO_** ****/
  BOOST_AUTO_TEST_CASE(BCD_TO_USINT) {
    using SourceType = CIEC_LWORD;
    using ResultType = CIEC_USINT;
    SourceType source1(0x11);
    SourceType source2(0x31);
    SourceType source3(0x36);
    SourceType source4(0x256);
    SourceType source5(0x257);
    ResultType result;
    result = func_BCD_TO_USINT(source1);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 11);
    result = func_BCD_TO_USINT(source2);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 31);
    result = func_BCD_TO_USINT(source3);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 36);
    result = func_BCD_TO_USINT(source4);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 0);
    result = func_BCD_TO_USINT(source5);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 1);
  }

  BOOST_AUTO_TEST_CASE(BCD_TO_UINT) {
    using SourceType = CIEC_LWORD;
    using ResultType = CIEC_UINT;
    SourceType source1(0x11);
    SourceType source2(0x31);
    SourceType source3(0x36);
    SourceType source4(0x256);
    SourceType source5(0x257);
    ResultType result;
    result = func_BCD_TO_UINT(source1);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 11);
    result = func_BCD_TO_UINT(source2);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 31);
    result = func_BCD_TO_UINT(source3);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 36);
    result = func_BCD_TO_UINT(source4);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 256);
    result = func_BCD_TO_UINT(source5);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 257);
  }

  BOOST_AUTO_TEST_CASE(BCD_TO_UDINT) {
    using SourceType = CIEC_LWORD;
    using ResultType = CIEC_UDINT;
    SourceType source1(0x11);
    SourceType source2(0x31);
    SourceType source3(0x36);
    SourceType source4(0x256);
    SourceType source5(0x257);
    ResultType result;
    result = func_BCD_TO_UDINT(source1);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 11);
    result = func_BCD_TO_UDINT(source2);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 31);
    result = func_BCD_TO_UDINT(source3);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 36);
    result = func_BCD_TO_UDINT(source4);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 256);
    result = func_BCD_TO_UDINT(source5);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 257);
  }

  BOOST_AUTO_TEST_CASE(BCD_TO_ULINT) {
    using SourceType = CIEC_LWORD;
    using ResultType = CIEC_ULINT;
    SourceType source1(0x11);
    SourceType source2(0x31);
    SourceType source3(0x36);
    SourceType source4(0x256);
    SourceType source5(0x257);
    ResultType result;
    result = func_BCD_TO_ULINT(source1);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 11);
    result = func_BCD_TO_ULINT(source2);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 31);
    result = func_BCD_TO_ULINT(source3);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 36);
    result = func_BCD_TO_ULINT(source4);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 256);
    result = func_BCD_TO_ULINT(source5);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 257);
  }

  /**** USINT_TO_BCD_* ****/
  BOOST_AUTO_TEST_CASE(USINT_TO_BCD_BYTE) {
    using SourceType = CIEC_USINT;
    using ResultType = CIEC_BYTE;
    SourceType number1(11);
    SourceType number2(99);
    SourceType number3(101);
    CIEC_BYTE result;
    result = func_USINT_TO_BCD_BYTE(number1);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 0x11);
    result = func_USINT_TO_BCD_BYTE(number2);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 0x99);
    result = func_USINT_TO_BCD_BYTE(number3);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 0x01);
  }

  BOOST_AUTO_TEST_CASE(USINT_TO_BCD_WORD) {
    using SourceType = CIEC_USINT;
    using ResultType = CIEC_WORD;
    SourceType number1(11);
    SourceType number2(99);
    SourceType number3(101);
    ResultType result;
    result = func_USINT_TO_BCD_WORD(number1);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 0x11);
    result = func_USINT_TO_BCD_WORD(number2);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 0x99);
    result = func_USINT_TO_BCD_WORD(number3);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 0x101);
  }

  BOOST_AUTO_TEST_CASE(USINT_TO_BCD_DWORD) {
    using SourceType = CIEC_USINT;
    using ResultType = CIEC_DWORD;
    SourceType number1(11);
    SourceType number2(99);
    SourceType number3(101);
    ResultType result;
    result = func_USINT_TO_BCD_DWORD(number1);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 0x11);
    result = func_USINT_TO_BCD_DWORD(number2);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 0x99);
    result = func_USINT_TO_BCD_DWORD(number3);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 0x101);
  }

  BOOST_AUTO_TEST_CASE(USINT_TO_BCD_LWORD) {
    using SourceType = CIEC_USINT;
    using ResultType = CIEC_LWORD;
    SourceType number1(11);
    SourceType number2(99);
    SourceType number3(101);
    ResultType result;
    result = func_USINT_TO_BCD_LWORD(number1);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 0x11);
    result = func_USINT_TO_BCD_LWORD(number2);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 0x99);
    result = func_USINT_TO_BCD_LWORD(number3);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 0x101);
  }

  /**** UINT_TO_BCD_* ****/
  BOOST_AUTO_TEST_CASE(UINT_TO_BCD_BYTE) {
    using SourceType = CIEC_UINT;
    using ResultType = CIEC_BYTE;
    SourceType number1(11);
    SourceType number2(99);
    SourceType number3(101);
    CIEC_BYTE result;
    result = func_UINT_TO_BCD_BYTE(number1);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 0x11);
    result = func_UINT_TO_BCD_BYTE(number2);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 0x99);
    result = func_UINT_TO_BCD_BYTE(number3);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 0x01);
  }
  
  BOOST_AUTO_TEST_CASE(UINT_TO_BCD_WORD) {
    using SourceType = CIEC_UINT;
    using ResultType = CIEC_WORD;
    SourceType number1(11);
    SourceType number2(99);
    SourceType number3(101);
    ResultType result;
    result = func_UINT_TO_BCD_WORD(number1);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 0x11);
    result = func_UINT_TO_BCD_WORD(number2);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 0x99);
    result = func_UINT_TO_BCD_WORD(number3);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 0x101);
  }

  BOOST_AUTO_TEST_CASE(UINT_TO_BCD_DWORD) {
    using SourceType = CIEC_UINT;
    using ResultType = CIEC_DWORD;
    SourceType number1(11);
    SourceType number2(99);
    SourceType number3(101);
    ResultType result;
    result = func_UINT_TO_BCD_DWORD(number1);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 0x11);
    result = func_UINT_TO_BCD_DWORD(number2);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 0x99);
    result = func_UINT_TO_BCD_DWORD(number3);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 0x101);
  }

  BOOST_AUTO_TEST_CASE(UINT_TO_BCD_LWORD) {
    using SourceType = CIEC_UINT;
    using ResultType = CIEC_LWORD;
    SourceType number1(11);
    SourceType number2(99);
    SourceType number3(101);
    ResultType result;
    result = func_UINT_TO_BCD_LWORD(number1);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 0x11);
    result = func_UINT_TO_BCD_LWORD(number2);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 0x99);
    result = func_UINT_TO_BCD_LWORD(number3);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 0x101);
  }

  /**** UDINT_TO_BCD_* ****/
  BOOST_AUTO_TEST_CASE(UDINT_TO_BCD_BYTE) {
    using SourceType = CIEC_UDINT;
    using ResultType = CIEC_BYTE;
    SourceType number1(11);
    SourceType number2(99);
    SourceType number3(101);
    CIEC_BYTE result;
    result = func_UDINT_TO_BCD_BYTE(number1);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 0x11);
    result = func_UDINT_TO_BCD_BYTE(number2);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 0x99);
    result = func_UDINT_TO_BCD_BYTE(number3);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 0x01);
  }
  
  BOOST_AUTO_TEST_CASE(UDINT_TO_BCD_WORD) {
    using SourceType = CIEC_UDINT;
    using ResultType = CIEC_WORD;
    SourceType number1(11);
    SourceType number2(99);
    SourceType number3(101);
    ResultType result;
    result = func_UDINT_TO_BCD_WORD(number1);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 0x11);
    result = func_UDINT_TO_BCD_WORD(number2);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 0x99);
    result = func_UDINT_TO_BCD_WORD(number3);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 0x101);
  }

  BOOST_AUTO_TEST_CASE(UDINT_TO_BCD_DWORD) {
    using SourceType = CIEC_UDINT;
    using ResultType = CIEC_DWORD;
    SourceType number1(11);
    SourceType number2(99);
    SourceType number3(101);
    ResultType result;
    result = func_UDINT_TO_BCD_DWORD(number1);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 0x11);
    result = func_UDINT_TO_BCD_DWORD(number2);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 0x99);
    result = func_UDINT_TO_BCD_DWORD(number3);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 0x101);
  }

  BOOST_AUTO_TEST_CASE(UDINT_TO_BCD_LWORD) {
    using SourceType = CIEC_UDINT;
    using ResultType = CIEC_LWORD;
    SourceType number1(11);
    SourceType number2(99);
    SourceType number3(101);
    ResultType result;
    result = func_UDINT_TO_BCD_LWORD(number1);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 0x11);
    result = func_UDINT_TO_BCD_LWORD(number2);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 0x99);
    result = func_UDINT_TO_BCD_LWORD(number3);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 0x101);
  }

    /**** ULINT_TO_BCD_* ****/
  BOOST_AUTO_TEST_CASE(ULINT_TO_BCD_BYTE) {
    using SourceType = CIEC_ULINT;
    using ResultType = CIEC_BYTE;
    SourceType number1(11);
    SourceType number2(99);
    SourceType number3(101);
    CIEC_BYTE result;
    result = func_ULINT_TO_BCD_BYTE(number1);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 0x11);
    result = func_ULINT_TO_BCD_BYTE(number2);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 0x99);
    result = func_ULINT_TO_BCD_BYTE(number3);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 0x01);
  }
  
  BOOST_AUTO_TEST_CASE(ULINT_TO_BCD_WORD) {
    using SourceType = CIEC_ULINT;
    using ResultType = CIEC_WORD;
    SourceType number1(11);
    SourceType number2(99);
    SourceType number3(101);
    ResultType result;
    result = func_ULINT_TO_BCD_WORD(number1);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 0x11);
    result = func_ULINT_TO_BCD_WORD(number2);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 0x99);
    result = func_ULINT_TO_BCD_WORD(number3);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 0x101);
  }

  BOOST_AUTO_TEST_CASE(ULINT_TO_BCD_DWORD) {
    using SourceType = CIEC_ULINT;
    using ResultType = CIEC_DWORD;
    SourceType number1(11);
    SourceType number2(99);
    SourceType number3(101);
    ResultType result;
    result = func_ULINT_TO_BCD_DWORD(number1);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 0x11);
    result = func_ULINT_TO_BCD_DWORD(number2);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 0x99);
    result = func_ULINT_TO_BCD_DWORD(number3);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 0x101);
  }

  BOOST_AUTO_TEST_CASE(ULINT_TO_BCD_LWORD) {
    using SourceType = CIEC_ULINT;
    using ResultType = CIEC_LWORD;
    SourceType number1(11);
    SourceType number2(99);
    SourceType number3(101);
    ResultType result;
    result = func_ULINT_TO_BCD_LWORD(number1);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 0x11);
    result = func_ULINT_TO_BCD_LWORD(number2);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 0x99);
    result = func_ULINT_TO_BCD_LWORD(number3);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 0x101);
  }

  /**** *_TO_BCD_BYTE ****/
  BOOST_AUTO_TEST_CASE(TO_BCD_BYTE) {
    using SourceType = CIEC_ULINT;
    using ResultType = CIEC_BYTE;
    SourceType number1(11);
    SourceType number2(99);
    SourceType number3(101);
    CIEC_BYTE result;
    result = func_TO_BCD_BYTE(number1);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 0x11);
    result = func_TO_BCD_BYTE(number2);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 0x99);
    result = func_TO_BCD_BYTE(number3);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 0x01);
  }
  
  BOOST_AUTO_TEST_CASE(TO_BCD_WORD) {
    using SourceType = CIEC_ULINT;
    using ResultType = CIEC_WORD;
    SourceType number1(11);
    SourceType number2(99);
    SourceType number3(101);
    ResultType result;
    result = func_TO_BCD_WORD(number1);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 0x11);
    result = func_TO_BCD_WORD(number2);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 0x99);
    result = func_TO_BCD_WORD(number3);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 0x101);
  }

  BOOST_AUTO_TEST_CASE(TO_BCD_DWORD) {
    using SourceType = CIEC_ULINT;
    using ResultType = CIEC_DWORD;
    SourceType number1(11);
    SourceType number2(99);
    SourceType number3(101);
    ResultType result;
    result = func_TO_BCD_DWORD(number1);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 0x11);
    result = func_TO_BCD_DWORD(number2);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 0x99);
    result = func_TO_BCD_DWORD(number3);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 0x101);
  }

  BOOST_AUTO_TEST_CASE(TO_BCD_LWORD) {
    using SourceType = CIEC_ULINT;
    using ResultType = CIEC_LWORD;
    SourceType number1(11);
    SourceType number2(99);
    SourceType number3(101);
    ResultType result;
    result = func_TO_BCD_LWORD(number1);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 0x11);
    result = func_TO_BCD_LWORD(number2);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 0x99);
    result = func_TO_BCD_LWORD(number3);
    BOOST_TEST(static_cast<ResultType::TValueType>(result) == 0x101);
  }

  BOOST_AUTO_TEST_SUITE_END()
