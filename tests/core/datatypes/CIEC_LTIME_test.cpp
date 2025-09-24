/*******************************************************************************
 * Copyright (c) 2011, 2025 ACIN, fortiss GmbH, Martin Erich Jobst,
 *                          Primetals Technologies Austria GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Martin Melik Merkumians, Ingo Hegny, Alois Zoitl
 *                - initial API and implementation and/or initial documentation
 *   Martin Jobst - add user-defined literal tests
 *   Hesam Rezaee - add literal tests based on new time elements
 *   Alois Zoitl  - migrated data type toString to std::string
 *******************************************************************************/
#include <boost/test/unit_test.hpp>
#include <string>
#include "forte_boost_output_support.h"

#include "forte/datatypes/forte_ltime.h"

namespace forte::test {
  BOOST_AUTO_TEST_SUITE(CIEC_LTIME_function_test)
  BOOST_AUTO_TEST_CASE(Type_test) {
    CIEC_LTIME nTest;
    // check type information
    BOOST_CHECK_EQUAL(nTest.getDataTypeID(), CIEC_ANY::e_LTIME);
    // check operator bool data type size
    BOOST_CHECK_EQUAL(sizeof(nTest.operator TForteInt64()), sizeof(TForteInt64));
  }

  BOOST_AUTO_TEST_CASE(Literal_test) {
    CIEC_LTIME test1 = 0_LTIME;
    BOOST_TEST(static_cast<CIEC_LTIME::TValueType>(test1) == 0);

    CIEC_LTIME test2 = 9223372036854775807_LTIME;
    BOOST_TEST(static_cast<CIEC_LTIME::TValueType>(test2) == std::numeric_limits<TForteInt64>::max());

    CIEC_LTIME test3 = -9223372036854775808_LTIME;
    BOOST_TEST(static_cast<CIEC_LTIME::TValueType>(test3) == std::numeric_limits<TForteInt64>::min());
  }

  BOOST_AUTO_TEST_CASE(Operator_test) {
    CIEC_LTIME nTest1;
    CIEC_LTIME nTest2;

    // check if data type is initialized with value of zero
    BOOST_CHECK_EQUAL(static_cast<CIEC_LTIME::TValueType>(nTest1), 0);

    nTest1 = CIEC_LTIME(std::numeric_limits<TForteInt64>::min());
    nTest2 = nTest1;
    BOOST_CHECK_EQUAL(static_cast<CIEC_LTIME::TValueType>(nTest1), std::numeric_limits<TForteInt64>::min());
    BOOST_CHECK_EQUAL(static_cast<CIEC_LTIME::TValueType>(nTest2), std::numeric_limits<TForteInt64>::min());

    nTest1 = CIEC_LTIME(std::numeric_limits<TForteInt64>::max());
    nTest2 = nTest1;
    BOOST_CHECK_EQUAL(static_cast<CIEC_LTIME::TValueType>(nTest1), std::numeric_limits<TForteInt64>::max());
    BOOST_CHECK_EQUAL(static_cast<CIEC_LTIME::TValueType>(nTest2), std::numeric_limits<TForteInt64>::max());

    nTest1 = CIEC_LTIME(std::numeric_limits<TForteInt32>::min());
    nTest2 = nTest1;
    BOOST_CHECK_EQUAL(static_cast<CIEC_LTIME::TValueType>(nTest1), std::numeric_limits<TForteInt32>::min());
    BOOST_CHECK_EQUAL(static_cast<CIEC_LTIME::TValueType>(nTest2), std::numeric_limits<TForteInt32>::min());

    nTest1 = CIEC_LTIME(static_cast<CIEC_LTIME::TValueType>(9657545ULL));
    nTest2 = nTest1;
    BOOST_CHECK_EQUAL(static_cast<CIEC_LTIME::TValueType>(nTest1), 9657545LL);
    BOOST_CHECK_EQUAL(static_cast<CIEC_LTIME::TValueType>(nTest2), 9657545LL);

    nTest1 = CIEC_LTIME(std::numeric_limits<TForteInt32>::max());
    nTest2 = nTest1;
    BOOST_CHECK_EQUAL(static_cast<CIEC_LTIME::TValueType>(nTest1), std::numeric_limits<TForteInt32>::max());
    BOOST_CHECK_EQUAL(static_cast<CIEC_LTIME::TValueType>(nTest2), std::numeric_limits<TForteInt32>::max());

    nTest2 = CIEC_LTIME(static_cast<CIEC_LTIME::TValueType>(564874642ULL));
    BOOST_CHECK_EQUAL(static_cast<CIEC_LTIME::TValueType>(nTest1), std::numeric_limits<TForteInt32>::max());
    BOOST_CHECK_EQUAL(static_cast<CIEC_LTIME::TValueType>(nTest2), 564874642LL);
  }

  BOOST_AUTO_TEST_CASE(Conversion_test) {
    CIEC_LTIME nTest;

    std::string buffer;

    // check cast operator
    nTest = CIEC_LTIME(static_cast<CIEC_LTIME::TValueType>(0));

    BOOST_CHECK_EQUAL(static_cast<CIEC_LTIME::TValueType>(nTest), 0);

    nTest = CIEC_LTIME(std::numeric_limits<TForteInt32>::min());
    BOOST_CHECK_EQUAL(static_cast<CIEC_LTIME::TValueType>(nTest), std::numeric_limits<TForteInt32>::min());

    nTest = CIEC_LTIME(static_cast<CIEC_LTIME::TValueType>(-21654147L));
    BOOST_CHECK_EQUAL(static_cast<CIEC_LTIME::TValueType>(nTest), -21654147L);

    nTest = CIEC_LTIME(std::numeric_limits<TForteInt32>::max());
    BOOST_CHECK_EQUAL(static_cast<CIEC_LTIME::TValueType>(nTest), std::numeric_limits<TForteInt32>::max());

    // check toString and fromString
    BOOST_CHECK_EQUAL(nTest.fromString("4h36m"), 1);
    /*nTest.toString(cBuffer, 17), 12);
    nTest.toString(cBufferFail, 2), -1);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "LT#16560000ms"), 0);
    strcpy(cBuffer, "");*/

    BOOST_CHECK_EQUAL(nTest.fromString("LTIME#4h36m"), 11);
    BOOST_CHECK_EQUAL(static_cast<CIEC_LTIME::TValueType>(nTest),
                      16560000LL * (CIEC_ANY_DURATION::csmForteTimeBaseUnitsPerSecond / 1000LL));
    nTest.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "LT#4h36m");
    buffer.clear();
    nTest = CIEC_LTIME(static_cast<CIEC_LTIME::TValueType>(0));

    BOOST_CHECK_EQUAL(nTest.fromString("ltime#4h36m"), 11);
    BOOST_CHECK_EQUAL(static_cast<CIEC_LTIME::TValueType>(nTest),
                      16560000LL * (CIEC_ANY_DURATION::csmForteTimeBaseUnitsPerSecond / 1000LL));
    nTest.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "LT#4h36m");
    buffer.clear();
    nTest = CIEC_LTIME(static_cast<CIEC_LTIME::TValueType>(0));

    BOOST_CHECK_EQUAL(nTest.fromString("LT#4h36m"), 8);
    BOOST_CHECK_EQUAL(static_cast<CIEC_LTIME::TValueType>(nTest),
                      16560000LL * (CIEC_ANY_DURATION::csmForteTimeBaseUnitsPerSecond / 1000LL));
    nTest.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "LT#4h36m");
    buffer.clear();
    nTest = CIEC_LTIME(static_cast<CIEC_LTIME::TValueType>(0));

    BOOST_CHECK_EQUAL(nTest.fromString("lt#4h36m"), 8);
    BOOST_CHECK_EQUAL(static_cast<CIEC_LTIME::TValueType>(nTest),
                      16560000LL * (CIEC_ANY_DURATION::csmForteTimeBaseUnitsPerSecond / 1000LL));
    nTest.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "LT#4h36m");
    buffer.clear();
    nTest = CIEC_LTIME(static_cast<CIEC_LTIME::TValueType>(0));

    BOOST_CHECK_EQUAL(nTest.fromString("3s_22ms"), 1);
    /*nTest.toString(cBuffer, 17), 17);
    nTest.toString(cBufferFail, 2), -1);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "LT#3022ms"), 0);
    strcpy(cBuffer, "");*/

    BOOST_CHECK_EQUAL(nTest.fromString("LTIME#3s_22ms"), 13);
    BOOST_CHECK_EQUAL(static_cast<CIEC_LTIME::TValueType>(nTest),
                      (CIEC_LTIME::TValueType) (3022LL * (CIEC_ANY_DURATION::csmForteTimeBaseUnitsPerSecond / 1000LL)));
    nTest.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "LT#3s22ms");
    buffer.clear();
    nTest = CIEC_LTIME(static_cast<CIEC_LTIME::TValueType>(0));

    BOOST_CHECK_EQUAL(nTest.fromString("LTIME#3s_22ms"), 13);
    BOOST_CHECK_EQUAL(static_cast<CIEC_LTIME::TValueType>(nTest),
                      3022LL * (CIEC_ANY_DURATION::csmForteTimeBaseUnitsPerSecond / 1000LL));
    nTest.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "LT#3s22ms");
    buffer.clear();
    nTest = CIEC_LTIME(static_cast<CIEC_LTIME::TValueType>(0));

    BOOST_CHECK_EQUAL(nTest.fromString("LT#3s_22ms"), 10);
    BOOST_CHECK_EQUAL(static_cast<CIEC_LTIME::TValueType>(nTest),
                      3022LL * (CIEC_ANY_DURATION::csmForteTimeBaseUnitsPerSecond / 1000LL));
    nTest.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "LT#3s22ms");
    buffer.clear();
    nTest = CIEC_LTIME(static_cast<CIEC_LTIME::TValueType>(0));

    BOOST_CHECK_EQUAL(nTest.fromString("LT#3s_22ms"), 10);
    BOOST_CHECK_EQUAL(static_cast<CIEC_LTIME::TValueType>(nTest),
                      3022LL * (CIEC_ANY_DURATION::csmForteTimeBaseUnitsPerSecond / 1000LL));
    nTest.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "LT#3s22ms");
    buffer.clear();
    nTest = CIEC_LTIME(static_cast<CIEC_LTIME::TValueType>(0));

    BOOST_CHECK_EQUAL(nTest.fromString("LT#76s"), 6);
    BOOST_CHECK_EQUAL(
        static_cast<CIEC_LTIME::TValueType>(nTest),
        (CIEC_LTIME::TValueType) (76000LL * (CIEC_ANY_DURATION::csmForteTimeBaseUnitsPerSecond / 1000LL)));
    nTest.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "LT#1m16s");
    buffer.clear();
    nTest = CIEC_LTIME(static_cast<CIEC_LTIME::TValueType>(0));

    BOOST_CHECK_EQUAL(nTest.fromString("LT#76m76s"), 9);
    BOOST_CHECK_EQUAL(
        static_cast<CIEC_LTIME::TValueType>(nTest),
        (CIEC_LTIME::TValueType) (4636000LL * (CIEC_ANY_DURATION::csmForteTimeBaseUnitsPerSecond / 1000LL)));
    nTest.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "LT#1h17m16s");
    buffer.clear();
    nTest = CIEC_LTIME(static_cast<CIEC_LTIME::TValueType>(0));

    BOOST_CHECK_EQUAL(nTest.fromString("LT#1d05h76m76s"), 14);
    BOOST_CHECK_EQUAL(
        static_cast<CIEC_LTIME::TValueType>(nTest),
        (CIEC_LTIME::TValueType) (109036000LL * (CIEC_ANY_DURATION::csmForteTimeBaseUnitsPerSecond / 1000LL)));
    nTest.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "LT#1d6h17m16s");
    buffer.clear();
    nTest = CIEC_LTIME(static_cast<CIEC_LTIME::TValueType>(0));

    BOOST_CHECK_EQUAL(nTest.fromString("LT#1d76h76m76s"), 14);
    BOOST_CHECK_EQUAL(
        static_cast<CIEC_LTIME::TValueType>(nTest),
        (CIEC_LTIME::TValueType) (364636000LL * (CIEC_ANY_DURATION::csmForteTimeBaseUnitsPerSecond / 1000LL)));
    nTest.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "LT#4d5h17m16s");
    buffer.clear();
    nTest = CIEC_LTIME(static_cast<CIEC_LTIME::TValueType>(0));

    BOOST_CHECK_EQUAL(nTest.fromString("LT#1d76h76m76s200ms568us100ns"), 29);
    BOOST_CHECK_EQUAL(static_cast<CIEC_LTIME::TValueType>(nTest), (CIEC_LTIME::TValueType) (364636200568100LL));
    nTest.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "LT#4d5h17m16s200ms568us100ns");
    nTest = CIEC_LTIME(static_cast<CIEC_LTIME::TValueType>(0));
  }

  BOOST_AUTO_TEST_CASE(test_upperbound_lowerbound_test) {
    CIEC_LTIME nTest;
    std::string buffer;

    CIEC_LTIME test2 = 9223372036854775807_TIME;
    test2.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "LT#106751d23h47m16s854ms775us807ns");

    buffer.clear();

    CIEC_LTIME test3 = 9223372036854775806_TIME;
    test3.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "LT#106751d23h47m16s854ms775us806ns");

    buffer.clear();

    CIEC_LTIME test5 = -9223372036854775807_TIME;
    test5.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "LT#-106751d23h47m16s854ms775us807ns");

    buffer.clear();

    CIEC_LTIME test6 = -9223372036854775806_TIME;
    test6.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "LT#-106751d23h47m16s854ms775us806ns");

    buffer.clear();

    BOOST_CHECK_EQUAL(nTest.fromString("LT#-106751d23h47m16s854ms775us808ns"), 35);
    BOOST_CHECK_EQUAL(static_cast<CIEC_LTIME::TValueType>(nTest), std::numeric_limits<CIEC_LTIME::TValueType>::min());
    nTest.toString(buffer);
    BOOST_TEST(buffer == "LT#-106751d23h47m16s854ms775us808ns");

    buffer.clear();
    nTest = CIEC_LTIME(static_cast<CIEC_LTIME::TValueType>(0));
  }

  BOOST_AUTO_TEST_CASE(toString_with_microseconds_test) {
    CIEC_LTIME time;
    std::string buffer;

    time.setFromMicroSeconds(0);
    time.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "LT#0ns");
    buffer.clear();

    time.setFromMicroSeconds(1);
    time.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "LT#1us");
    buffer.clear();

    time.setFromMicroSeconds(10);
    time.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "LT#10us");
    buffer.clear();

    time.setFromMicroSeconds(100);
    time.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "LT#100us");
    buffer.clear();

    time.setFromMicroSeconds(3450001);
    time.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "LT#3s450ms1us");
    buffer.clear();
  }

  BOOST_AUTO_TEST_CASE(toString_with_negative_times_test) {
    CIEC_LTIME time;
    std::string buffer;

    time.setFromMicroSeconds(-1);
    time.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "LT#-1us");
    buffer.clear();

    time.setFromMicroSeconds(-10);
    time.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "LT#-10us");
    buffer.clear();

    time.setFromMicroSeconds(-100);
    time.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "LT#-100us");
    buffer.clear();

    time.setFromMicroSeconds(-3450001);
    time.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "LT#-3s450ms1us");
    buffer.clear();
  }

  BOOST_AUTO_TEST_CASE(ltime_comparision_tests) {
    CIEC_LTIME time1, time2;

    time1.setFromMilliSeconds(200);
    time2.setFromMilliSeconds(200);

    BOOST_CHECK(time1 == time2);
    BOOST_CHECK_EQUAL(true, time1 >= time2);
    BOOST_CHECK_EQUAL(true, time1 <= time2);

    time2.setFromMilliSeconds(150);

    BOOST_CHECK_EQUAL(true, time1 > time2);
    BOOST_CHECK_EQUAL(true, time1 >= time2);
    BOOST_CHECK_EQUAL(true, time1 != time2);
    BOOST_CHECK_EQUAL(false, time1 < time2);
    BOOST_CHECK_EQUAL(false, time1 <= time2);

    time1.setFromMilliSeconds(120);
    BOOST_CHECK_EQUAL(false, time1 > time2);
    BOOST_CHECK_EQUAL(false, time1 >= time2);
    BOOST_CHECK_EQUAL(true, time1 != time2);
    BOOST_CHECK_EQUAL(true, time1 < time2);
    BOOST_CHECK_EQUAL(true, time1 <= time2);
  }

  BOOST_AUTO_TEST_CASE(get_ltime_in_nanoseconds) {
    CIEC_LTIME time;

    time.setFromNanoSeconds(1);
    BOOST_CHECK(time.getInNanoSeconds() ==
                static_cast<CIEC_LTIME::TValueType>(CIEC_LTIME(static_cast<CIEC_LTIME::TValueType>(1))));

    time.setFromNanoSeconds(10);
    BOOST_CHECK(time.getInNanoSeconds() ==
                static_cast<CIEC_LTIME::TValueType>(CIEC_LTIME(static_cast<CIEC_LTIME::TValueType>(10))));

    time.setFromNanoSeconds(999);
    BOOST_CHECK(time.getInNanoSeconds() ==
                static_cast<CIEC_LTIME::TValueType>(CIEC_LTIME(static_cast<CIEC_LTIME::TValueType>(999))));

    time.setFromNanoSeconds(10325643);
    BOOST_CHECK(time.getInNanoSeconds() ==
                static_cast<CIEC_LTIME::TValueType>(CIEC_LTIME(static_cast<CIEC_LTIME::TValueType>(10325643))));
  }

  BOOST_AUTO_TEST_CASE(parse_time_literal_in_days) {
    CIEC_LTIME time;

    time.fromString("LT#1d");
    BOOST_CHECK_EQUAL(time.getInDays(), 1);

    time.fromString("LT#10d");
    BOOST_CHECK_EQUAL(time.getInDays(), 10);

    time.fromString("LT#99d");
    BOOST_CHECK_EQUAL(time.getInDays(), 99);

    time.fromString("LT#56123d");
    BOOST_CHECK_EQUAL(time.getInDays(), 56123);
  }

  BOOST_AUTO_TEST_CASE(parse_negative_signed_time_literal_in_days) {
    CIEC_LTIME time;

    time.fromString("LT#-1d");
    BOOST_CHECK_EQUAL(time.getInDays(), -1);

    time.fromString("LT#-10d");
    BOOST_CHECK_EQUAL(time.getInDays(), -10);

    time.fromString("LT#-99d");
    BOOST_CHECK_EQUAL(time.getInDays(), -99);

    time.fromString("LT#-56123d");
    BOOST_CHECK_EQUAL(time.getInDays(), -56123);
  }

  BOOST_AUTO_TEST_CASE(parse_time_literal_in_hours) {
    CIEC_LTIME time;

    time.fromString("LT#1h");
    BOOST_CHECK_EQUAL(time.getInHours(), 1);

    time.fromString("LT#10h");
    BOOST_CHECK_EQUAL(time.getInHours(), 10);

    time.fromString("LT#99h");
    BOOST_CHECK_EQUAL(time.getInHours(), 99);

    time.fromString("LT#12345h");
    BOOST_CHECK_EQUAL(time.getInHours(), 12345);
  }

  BOOST_AUTO_TEST_CASE(parse_negative_signed_time_literal_in_hours) {
    CIEC_LTIME time;

    time.fromString("LT#-1h");
    BOOST_CHECK_EQUAL(time.getInHours(), -1);

    time.fromString("LT#-10h");
    BOOST_CHECK_EQUAL(time.getInHours(), -10);

    time.fromString("LT#-99h");
    BOOST_CHECK_EQUAL(time.getInHours(), -99);

    time.fromString("LT#-12345h");
    BOOST_CHECK_EQUAL(time.getInHours(), -12345);
  }

  BOOST_AUTO_TEST_CASE(parse_time_literal_in_minutes) {
    CIEC_LTIME time;

    time.fromString("LT#1m");
    BOOST_CHECK_EQUAL(time.getInMinutes(), 1);

    time.fromString("LT#10m");
    BOOST_CHECK_EQUAL(time.getInMinutes(), 10);

    time.fromString("LT#99m");
    BOOST_CHECK_EQUAL(time.getInMinutes(), 99);

    time.fromString("LT#10325643m");
    BOOST_CHECK_EQUAL(time.getInMinutes(), 10325643);
  }

  BOOST_AUTO_TEST_CASE(parse_negative_signed_time_literal_in_minutes) {
    CIEC_LTIME time;

    time.fromString("LT#-1m");
    BOOST_CHECK_EQUAL(time.getInMinutes(), -1);

    time.fromString("LT#-10m");
    BOOST_CHECK_EQUAL(time.getInMinutes(), -10);

    time.fromString("LT#-99m");
    BOOST_CHECK_EQUAL(time.getInMinutes(), -99);

    time.fromString("LT#-10325643m");
    BOOST_CHECK_EQUAL(time.getInMinutes(), -10325643);
  }

  BOOST_AUTO_TEST_CASE(parse_time_literal_in_seconds) {
    CIEC_LTIME time;

    time.fromString("LT#1s");
    BOOST_CHECK_EQUAL(time.getInSeconds(), 1);

    time.fromString("LT#10s");
    BOOST_CHECK_EQUAL(time.getInSeconds(), 10);

    time.fromString("LT#99s");
    BOOST_CHECK_EQUAL(time.getInSeconds(), 99);

    time.fromString("LT#10325643s");
    BOOST_CHECK_EQUAL(time.getInSeconds(), 10325643);
  }

  BOOST_AUTO_TEST_CASE(parse_negative_signed_time_literal_in_seconds) {
    CIEC_LTIME time;

    time.fromString("LT#-1s");
    BOOST_CHECK_EQUAL(time.getInSeconds(), -1);

    time.fromString("LT#-10s");
    BOOST_CHECK_EQUAL(time.getInSeconds(), -10);

    time.fromString("LT#-99s");
    BOOST_CHECK_EQUAL(time.getInSeconds(), -99);

    time.fromString("LT#-10325643s");
    BOOST_CHECK_EQUAL(time.getInSeconds(), -10325643);
  }

  BOOST_AUTO_TEST_CASE(parse_ltime_literal_in_milliseconds) {
    CIEC_LTIME time;

    time.fromString("LT#1ms");
    BOOST_CHECK_EQUAL(time.getInMilliSeconds(), 1);

    time.fromString("LT#10ms");
    BOOST_CHECK_EQUAL(time.getInMilliSeconds(), 10);

    time.fromString("LT#999ms");
    BOOST_CHECK_EQUAL(time.getInMilliSeconds(), 999);

    time.fromString("LT#10325643ms");
    BOOST_CHECK_EQUAL(time.getInMilliSeconds(), 10325643);
  }

  BOOST_AUTO_TEST_CASE(parse_positive_signed_ltime_literal_in_milliseconds) {
    CIEC_LTIME time;

    time.fromString("LT#+1ms");
    BOOST_CHECK_EQUAL(time.getInMilliSeconds(), 1);

    time.fromString("LT#+10ms");
    BOOST_CHECK_EQUAL(time.getInMilliSeconds(), 10);

    time.fromString("LT#+999ms");
    BOOST_CHECK_EQUAL(time.getInMilliSeconds(), 999);

    time.fromString("LT#+10325643ms");
    BOOST_CHECK_EQUAL(time.getInMilliSeconds(), 10325643);
  }

  BOOST_AUTO_TEST_CASE(parse_negative_signed_ltime_literal_in_milliseconds) {
    CIEC_LTIME time;

    time.fromString("LT#-1ms");
    BOOST_CHECK_EQUAL(time.getInMilliSeconds(), -1);

    time.fromString("LT#-10ms");
    BOOST_CHECK_EQUAL(time.getInMilliSeconds(), -10);

    time.fromString("LT#-999ms");
    BOOST_CHECK_EQUAL(time.getInMilliSeconds(), -999);

    time.fromString("LT#-10325643ms");
    BOOST_CHECK_EQUAL(time.getInMilliSeconds(), -10325643);
  }

  BOOST_AUTO_TEST_CASE(parse_ltime_literal_in_microseconds) {
    CIEC_LTIME time;

    time.fromString("LT#1us");
    BOOST_CHECK_EQUAL(time.getInMicroSeconds(), 1);

    time.fromString("LT#10us");
    BOOST_CHECK_EQUAL(time.getInMicroSeconds(), 10);

    time.fromString("LT#999us");
    BOOST_CHECK_EQUAL(time.getInMicroSeconds(), 999);

    time.fromString("LT#10325643us");
    BOOST_CHECK_EQUAL(time.getInMicroSeconds(), 10325643);
  }

  BOOST_AUTO_TEST_CASE(parse_negative_signed_ltime_literal_in_microseconds) {
    CIEC_LTIME time;

    time.fromString("LT#-1us");
    BOOST_CHECK_EQUAL(time.getInMicroSeconds(), -1);

    time.fromString("LT#-10us");
    BOOST_CHECK_EQUAL(time.getInMicroSeconds(), -10);

    time.fromString("LT#-999us");
    BOOST_CHECK_EQUAL(time.getInMicroSeconds(), -999);

    time.fromString("LT#-10325643us");
    BOOST_CHECK_EQUAL(time.getInMicroSeconds(), -10325643);
  }

  BOOST_AUTO_TEST_CASE(parse_missing_s_in_microseconds_literal) {
    CIEC_LTIME time;

    BOOST_CHECK_EQUAL(time.fromString("LT#-1u"), -1);
    BOOST_CHECK_EQUAL(time.getInMicroSeconds(), 0);

    BOOST_CHECK_EQUAL(time.fromString("LT#-1ut"), -1);
    BOOST_CHECK_EQUAL(time.getInMicroSeconds(), 0);
  }

  BOOST_AUTO_TEST_CASE(parse_ltime_literal_in_nanoseconds) {
    CIEC_LTIME time;

    time.fromString("LT#1ns");
    BOOST_CHECK_EQUAL(time.getInNanoSeconds(), 1);

    time.fromString("LT#10ns");
    BOOST_CHECK_EQUAL(time.getInNanoSeconds(), 10);

    time.fromString("LT#999ns");
    BOOST_CHECK_EQUAL(time.getInNanoSeconds(), 999);

    time.fromString("LT#10325643ns");
    BOOST_CHECK_EQUAL(time.getInNanoSeconds(), 10325643);
  }

  BOOST_AUTO_TEST_CASE(parse_missing_s_in_nanoseconds_literal) {
    CIEC_LTIME time;

    BOOST_CHECK_EQUAL(time.fromString("LT#1234nx"), -1);
    BOOST_CHECK_EQUAL(time.getInMicroSeconds(), 0);
  }

  BOOST_AUTO_TEST_CASE(parse_negative_signed_time_literal_in_nanoseconds) {
    CIEC_LTIME time;

    time.fromString("LT#-1ns");
    BOOST_CHECK_EQUAL(time.getInNanoSeconds(), -1);

    time.fromString("LT#-10ns");
    BOOST_CHECK_EQUAL(time.getInNanoSeconds(), -10);

    time.fromString("LT#-999ns");
    BOOST_CHECK_EQUAL(time.getInNanoSeconds(), -999);

    time.fromString("LT#-10325643ns");
    BOOST_CHECK_EQUAL(time.getInNanoSeconds(), -10325643);
  }

  BOOST_AUTO_TEST_CASE(parse_ltime_literals_with_missing_end_unit) {
    CIEC_LTIME time;

    BOOST_CHECK_EQUAL(-1, time.fromString("LT#68231"));
    BOOST_CHECK_EQUAL(time.getInNanoSeconds(), 0);
    BOOST_CHECK_EQUAL(-1, time.fromString("LT#1234r"));
    BOOST_CHECK_EQUAL(time.getInNanoSeconds(), 0);
    BOOST_CHECK_EQUAL(-1, time.fromString("LT#1h23"));
    BOOST_CHECK_EQUAL(time.getInNanoSeconds(), 0);
    BOOST_CHECK_EQUAL(-1, time.fromString("LT#23m89"));
    BOOST_CHECK_EQUAL(time.getInNanoSeconds(), 0);
    BOOST_CHECK_EQUAL(-1, time.fromString("LT#55s514"));
    BOOST_CHECK_EQUAL(time.getInNanoSeconds(), 0);
    BOOST_CHECK_EQUAL(-1, time.fromString("LT#344ms1"));
    BOOST_CHECK_EQUAL(time.getInNanoSeconds(), 0);
    BOOST_CHECK_EQUAL(-1, time.fromString("LT#344ms12399"));
    BOOST_CHECK_EQUAL(time.getInNanoSeconds(), 0);
    BOOST_CHECK_EQUAL(-1, time.fromString("LT#344ms12399"));
    BOOST_CHECK_EQUAL(time.getInNanoSeconds(), 0);
  }

  BOOST_AUTO_TEST_CASE(parse_wrong_ltime_prefix) {
    CIEC_LTIME time;

    BOOST_CHECK_EQUAL(-1, time.fromString("LT23m"));
    BOOST_CHECK_EQUAL(time.getInNanoSeconds(), 0);
    BOOST_CHECK_EQUAL(-1, time.fromString("LTIME23m"));
    BOOST_CHECK_EQUAL(time.getInNanoSeconds(), 0);
    BOOST_CHECK_EQUAL(-1, time.fromString("LTome#23m"));
    BOOST_CHECK_EQUAL(time.getInNanoSeconds(), 0);
    BOOST_CHECK_EQUAL(-1, time.fromString("LTine#23m"));
    BOOST_CHECK_EQUAL(time.getInNanoSeconds(), 0);
    BOOST_CHECK_EQUAL(-1, time.fromString("LTima#23m"));
    BOOST_CHECK_EQUAL(time.getInNanoSeconds(), 0);
  }

  BOOST_AUTO_TEST_CASE(parse_ltime_in_struct_or_array_literal) {
    CIEC_LTIME time;

    BOOST_CHECK_EQUAL(8, time.fromString("LT#999ms, "));
    BOOST_CHECK_EQUAL(time.getInMilliSeconds(), 999);

    BOOST_CHECK_EQUAL(7, time.fromString("LT#23ms , "));
    BOOST_CHECK_EQUAL(time.getInMilliSeconds(), 23);

    BOOST_CHECK_EQUAL(13, time.fromString("LT#10325643us,"));
    BOOST_CHECK_EQUAL(time.getInMicroSeconds(), 10325643);
  }

  BOOST_AUTO_TEST_CASE(InternalRepresentationTest) {
    CIEC_LTIME directRep(3600000000000);
    CIEC_LTIME fromStringRep;
    fromStringRep.fromString("LT#1h");

    BOOST_TEST(directRep == fromStringRep);
  }

  BOOST_AUTO_TEST_SUITE_END()
} // namespace forte::test
