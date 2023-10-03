/*******************************************************************************
 * Copyright (c) 2011, 2023 ACIN, fortiss GmbH
 *                          Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Martin Melik Merkumians, Ingo Hegny, Alois Zoitl - initial API and implementation and/or initial documentation
 *   Martin Jobst - add user-defined literal tests
 *   Hesam Rezaee - add literal tests based on new time elements
 *******************************************************************************/
#include <boost/test/unit_test.hpp>
#include <string>
#include "forte_boost_output_support.h"

#include "../../../src/core/datatypes/forte_time.h"

BOOST_AUTO_TEST_SUITE(CIEC_TIME_function_test)
BOOST_AUTO_TEST_CASE(Type_test)
{
  CIEC_TIME nTest;
  //check type information
  BOOST_CHECK_EQUAL(nTest.getDataTypeID(), CIEC_ANY::e_TIME);
  //check operator bool data type size
  BOOST_CHECK_EQUAL(sizeof(nTest.operator TForteInt64()), sizeof(TForteInt64));
}

BOOST_AUTO_TEST_CASE(Literal_test)
{
  CIEC_TIME test1 = 0_TIME;
  BOOST_TEST(static_cast<CIEC_TIME::TValueType>(test1) == 0);

  CIEC_TIME test2 = 9223372036854775807_TIME;
  BOOST_TEST(static_cast<CIEC_TIME::TValueType>(test2) == std::numeric_limits<TForteInt64>::max());

  CIEC_TIME test3 = -9223372036854775808_TIME;
  BOOST_TEST(static_cast<CIEC_TIME::TValueType>(test3) == std::numeric_limits<TForteInt64>::min());
}

BOOST_AUTO_TEST_CASE(Operator_test)
{
  CIEC_TIME nTest1;
  CIEC_TIME nTest2;

  //check if data type is initialized with value of zero
  BOOST_CHECK_EQUAL(static_cast<CIEC_TIME::TValueType>(nTest1), 0);

  nTest1 = CIEC_TIME(std::numeric_limits<TForteInt64>::min());
  nTest2 = nTest1;
  BOOST_CHECK_EQUAL(static_cast<CIEC_TIME::TValueType>(nTest1), std::numeric_limits<TForteInt64>::min());
  BOOST_CHECK_EQUAL(static_cast<CIEC_TIME::TValueType>(nTest2), std::numeric_limits<TForteInt64>::min());

  nTest1 = CIEC_TIME(std::numeric_limits<TForteInt64>::max());
  nTest2 = nTest1;
  BOOST_CHECK_EQUAL(static_cast<CIEC_TIME::TValueType>(nTest1), std::numeric_limits<TForteInt64>::max());
  BOOST_CHECK_EQUAL(static_cast<CIEC_TIME::TValueType>(nTest2), std::numeric_limits<TForteInt64>::max());

  nTest1 = CIEC_TIME(std::numeric_limits<TForteInt32>::min());
  nTest2 = nTest1;
  BOOST_CHECK_EQUAL(static_cast<CIEC_TIME::TValueType>(nTest1), std::numeric_limits<TForteInt32>::min());
  BOOST_CHECK_EQUAL(static_cast<CIEC_TIME::TValueType>(nTest2), std::numeric_limits<TForteInt32>::min());

  nTest1 = CIEC_TIME(static_cast<CIEC_TIME::TValueType>(9657545ULL));
  nTest2 = nTest1;
  BOOST_CHECK_EQUAL(static_cast<CIEC_TIME::TValueType>(nTest1), 9657545LL);
  BOOST_CHECK_EQUAL(static_cast<CIEC_TIME::TValueType>(nTest2), 9657545LL);

  nTest1 = CIEC_TIME(std::numeric_limits<TForteInt32>::max());
  nTest2 = nTest1;
  BOOST_CHECK_EQUAL(static_cast<CIEC_TIME::TValueType>(nTest1), std::numeric_limits<TForteInt32>::max());
  BOOST_CHECK_EQUAL(static_cast<CIEC_TIME::TValueType>(nTest2), std::numeric_limits<TForteInt32>::max());

  nTest2 = CIEC_TIME(static_cast<CIEC_TIME::TValueType>(564874642ULL));
  BOOST_CHECK_EQUAL(static_cast<CIEC_TIME::TValueType>(nTest1), std::numeric_limits<TForteInt32>::max());
  BOOST_CHECK_EQUAL(static_cast<CIEC_TIME::TValueType>(nTest2), 564874642LL);
}

BOOST_AUTO_TEST_CASE(Conversion_test)
{
  CIEC_TIME nTest;

  char cBuffer[14];
   char cBigBuffer[30];
  char cBufferFail[2];

  //check cast operator
  nTest = CIEC_TIME(static_cast<CIEC_TIME::TValueType>(0));

  BOOST_CHECK_EQUAL(static_cast<CIEC_TIME::TValueType>(nTest), 0);

  nTest = CIEC_TIME(std::numeric_limits<TForteInt32>::min());
  BOOST_CHECK_EQUAL(static_cast<CIEC_TIME::TValueType>(nTest), std::numeric_limits<TForteInt32>::min());

  nTest = CIEC_TIME(static_cast<CIEC_TIME::TValueType>(-21654147L));
  BOOST_CHECK_EQUAL(static_cast<CIEC_TIME::TValueType>(nTest), -21654147L);

  nTest = CIEC_TIME(std::numeric_limits<TForteInt32>::max());
  BOOST_CHECK_EQUAL(static_cast<CIEC_TIME::TValueType>(nTest), std::numeric_limits<TForteInt32>::max());

  //check toString and fromString
  strcpy(cBuffer, "");

  BOOST_CHECK_EQUAL(nTest.fromString("4h36m"), 1);
  /*BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 17), 12);
  BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
  BOOST_CHECK_EQUAL(strcmp(cBuffer, "T#16560000ms"), 0);
  strcpy(cBuffer, "");*/

  BOOST_CHECK_EQUAL(nTest.fromString("TIME#4h36m"), 10);
  BOOST_CHECK_EQUAL(static_cast<CIEC_TIME::TValueType>(nTest), 16560000LL * (CIEC_ANY_DURATION::csmForteTimeBaseUnitsPerSecond / 1000));
  BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 14), 7);
  BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
  BOOST_CHECK_EQUAL(std::string(cBuffer), "T#4h36m");
  strcpy(cBuffer, "");
  nTest = CIEC_TIME(static_cast<CIEC_TIME::TValueType>(0));

  BOOST_CHECK_EQUAL(nTest.fromString("time#4h36m"), 10);
  BOOST_CHECK_EQUAL(static_cast<CIEC_TIME::TValueType>(nTest), 16560000LL * (CIEC_ANY_DURATION::csmForteTimeBaseUnitsPerSecond / 1000));
  BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 14), 7);
  BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
  BOOST_CHECK_EQUAL(std::string(cBuffer), "T#4h36m");
  strcpy(cBuffer, "");
  nTest = CIEC_TIME(static_cast<CIEC_TIME::TValueType>(0));

  BOOST_CHECK_EQUAL(nTest.fromString("T#4h36m"), 7);
  BOOST_CHECK_EQUAL(static_cast<CIEC_TIME::TValueType>(nTest), 16560000LL * (CIEC_ANY_DURATION::csmForteTimeBaseUnitsPerSecond / 1000));
  BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 14), 7);
  BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
  BOOST_CHECK_EQUAL(std::string(cBuffer), "T#4h36m");
  strcpy(cBuffer, "");
  nTest = CIEC_TIME(static_cast<CIEC_TIME::TValueType>(0));

  BOOST_CHECK_EQUAL(nTest.fromString("t#4h36m"), 7);
  BOOST_CHECK_EQUAL(static_cast<CIEC_TIME::TValueType>(nTest), 16560000LL * (CIEC_ANY_DURATION::csmForteTimeBaseUnitsPerSecond / 1000));
  BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 14), 7);
  BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
  BOOST_CHECK_EQUAL(std::string(cBuffer), "T#4h36m");
  strcpy(cBuffer, "");
  nTest = CIEC_TIME(static_cast<CIEC_TIME::TValueType>(0));


  BOOST_CHECK_EQUAL(nTest.fromString("3s_22ms"), 1);
  /*BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 17), 17);
  BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
  BOOST_CHECK_EQUAL(strcmp(cBuffer, "T#3022ms"), 0);
  strcpy(cBuffer, "");*/

  BOOST_CHECK_EQUAL(nTest.fromString("TIME#3s_22ms"), 12);
  BOOST_CHECK_EQUAL(static_cast<CIEC_TIME::TValueType>(nTest), (CIEC_TIME::TValueType)(3022LL * (CIEC_ANY_DURATION::csmForteTimeBaseUnitsPerSecond / 1000LL)));
  BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 14), 8);
  BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
  BOOST_CHECK_EQUAL(std::string(cBuffer), "T#3s22ms");
  strcpy(cBuffer, "");
  nTest = CIEC_TIME(static_cast<CIEC_TIME::TValueType>(0));

  BOOST_CHECK_EQUAL(nTest.fromString("time#3s_22ms"), 12);
  BOOST_CHECK_EQUAL(static_cast<CIEC_TIME::TValueType>(nTest), 3022LL * (CIEC_ANY_DURATION::csmForteTimeBaseUnitsPerSecond / 1000LL));
  BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 14), 8);
  BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
  BOOST_CHECK_EQUAL(std::string(cBuffer), "T#3s22ms");
  strcpy(cBuffer, "");
  nTest = CIEC_TIME(static_cast<CIEC_TIME::TValueType>(0));

  BOOST_CHECK_EQUAL(nTest.fromString("T#3s_22ms"), 9);
  BOOST_CHECK_EQUAL(static_cast<CIEC_TIME::TValueType>(nTest), 3022LL * (CIEC_ANY_DURATION::csmForteTimeBaseUnitsPerSecond / 1000LL));
  BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 14), 8);
  BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
  BOOST_CHECK_EQUAL(std::string(cBuffer), "T#3s22ms");
  strcpy(cBuffer, "");
  nTest = CIEC_TIME(static_cast<CIEC_TIME::TValueType>(0));

  BOOST_CHECK_EQUAL(nTest.fromString("t#3s_22ms"), 9);
  BOOST_CHECK_EQUAL(static_cast<CIEC_TIME::TValueType>(nTest), 3022LL * (CIEC_ANY_DURATION::csmForteTimeBaseUnitsPerSecond / 1000LL));
  BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 14), 8);
  BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
  BOOST_CHECK_EQUAL(std::string(cBuffer), "T#3s22ms");
  strcpy(cBuffer, "");
  nTest = CIEC_TIME(static_cast<CIEC_TIME::TValueType>(0));


  BOOST_CHECK_EQUAL(nTest.fromString("T#76s"), 5);
  BOOST_CHECK_EQUAL(static_cast<CIEC_TIME::TValueType>(nTest), 76000LL * (CIEC_ANY_DURATION::csmForteTimeBaseUnitsPerSecond / 1000LL));
  BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 14), 7);
  BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
  BOOST_CHECK_EQUAL(std::string(cBuffer), "T#1m16s");
  strcpy(cBuffer, "");
  nTest = CIEC_TIME(static_cast<CIEC_TIME::TValueType>(0));

  BOOST_CHECK_EQUAL(nTest.fromString("T#76m76s"), 8);
  BOOST_CHECK_EQUAL(static_cast<CIEC_TIME::TValueType>(nTest), (CIEC_TIME::TValueType) (4636000LL * (CIEC_ANY_DURATION::csmForteTimeBaseUnitsPerSecond / 1000LL)));
  BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 14), 10);
  BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
  BOOST_CHECK_EQUAL(std::string(cBuffer), "T#1h17m16s");
  strcpy(cBuffer, "");
  nTest = CIEC_TIME(static_cast<CIEC_TIME::TValueType>(0));

  
  BOOST_CHECK_EQUAL(nTest.fromString("T#1d05h76m76s"), 13);
  BOOST_CHECK_EQUAL(static_cast<CIEC_TIME::TValueType>(nTest), (CIEC_TIME::TValueType) (109036000LL * (CIEC_ANY_DURATION::csmForteTimeBaseUnitsPerSecond / 1000LL)));
  BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 14), 12);
  BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
  BOOST_CHECK_EQUAL(std::string(cBuffer), "T#1d6h17m16s");
  strcpy(cBuffer, "");
  nTest = CIEC_TIME(static_cast<CIEC_TIME::TValueType>(0));

  BOOST_CHECK_EQUAL(nTest.fromString("T#1d76h76m76s"), 13);
  BOOST_CHECK_EQUAL(static_cast<CIEC_TIME::TValueType>(nTest), (CIEC_TIME::TValueType) (364636000LL * (CIEC_ANY_DURATION::csmForteTimeBaseUnitsPerSecond / 1000LL)));
  BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 14), 12);
  BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
  BOOST_CHECK_EQUAL(std::string(cBuffer), "T#4d5h17m16s");
  strcpy(cBuffer, "");
  nTest = CIEC_TIME(static_cast<CIEC_TIME::TValueType>(0));
   strcpy(cBigBuffer, "");

  BOOST_CHECK_EQUAL(nTest.fromString("T#1d76h76m76s200ms568us100ns"), 28);
  BOOST_CHECK_EQUAL(static_cast<CIEC_TIME::TValueType>(nTest), (CIEC_TIME::TValueType) (364636200568100LL));
  BOOST_CHECK_EQUAL(nTest.toString(cBigBuffer, 30), 27);
  BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
  BOOST_CHECK_EQUAL(std::string(cBigBuffer), "T#4d5h17m16s200ms568us100ns");
  strcpy(cBigBuffer, "");
  nTest = CIEC_TIME(static_cast<CIEC_TIME::TValueType>(0));

}

BOOST_AUTO_TEST_CASE(test_upperbound_lowerbound_test){
  CIEC_TIME nTest;
  char cBuffer[40];
  char cBufferFail[2];

  strcpy(cBuffer, "");

  CIEC_TIME test2 = 9223372036854775807_TIME;
  BOOST_CHECK_EQUAL(test2.toString(cBuffer, 40), 33);
  BOOST_CHECK_EQUAL(std::string(cBuffer) , "T#106751d23h47m16s854ms775us807ns");

  strcpy(cBuffer, "");

  CIEC_TIME test3 = 9223372036854775806_TIME;
  BOOST_CHECK_EQUAL(test3.toString(cBuffer, 40), 33);
  BOOST_CHECK_EQUAL(std::string(cBuffer) , "T#106751d23h47m16s854ms775us806ns");

  strcpy(cBuffer, "");

  CIEC_TIME test5 = -9223372036854775807_TIME;
  BOOST_CHECK_EQUAL(test5.toString(cBuffer, 40), 34);
  BOOST_CHECK_EQUAL(std::string(cBuffer) , "T#-106751d23h47m16s854ms775us807ns");

  strcpy(cBuffer, "");

  CIEC_TIME test6 = -9223372036854775806_TIME;
  BOOST_CHECK_EQUAL(test6.toString(cBuffer, 40), 34);
  BOOST_CHECK_EQUAL(std::string(cBuffer) , "T#-106751d23h47m16s854ms775us806ns");

  strcpy(cBuffer, "");

  BOOST_CHECK_EQUAL(nTest.fromString("T#-106751d23h47m16s854ms775us808ns"), 34);
  BOOST_CHECK_EQUAL(static_cast<CIEC_TIME::TValueType>(nTest), std::numeric_limits<CIEC_TIME::TValueType>::min());
  BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 40), 34);
  BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
  BOOST_CHECK_EQUAL(std::string(cBuffer), "T#-106751d23h47m16s854ms775us808ns");

  strcpy(cBuffer, "");
  nTest = CIEC_TIME(static_cast<CIEC_TIME::TValueType>(0));

}

BOOST_AUTO_TEST_CASE(toString_with_microseconds_test){
  CIEC_TIME time;
  char cBuffer[17];

  time.setFromMicroSeconds(0);
  BOOST_CHECK_EQUAL(time.toString(cBuffer, 17), 5);
  BOOST_CHECK_EQUAL(std::string(cBuffer), "T#0ns");

  time.setFromMicroSeconds(1);
  BOOST_CHECK_EQUAL(time.toString(cBuffer, 17), 5);
  BOOST_CHECK_EQUAL(std::string(cBuffer), "T#1us");

  time.setFromMicroSeconds(10);
  BOOST_CHECK_EQUAL(time.toString(cBuffer, 17), 6);
  BOOST_CHECK_EQUAL(std::string(cBuffer), "T#10us");

  time.setFromMicroSeconds(100);
  BOOST_CHECK_EQUAL(time.toString(cBuffer, 17), 7);
  BOOST_CHECK_EQUAL(std::string(cBuffer), "T#100us");

  time.setFromMicroSeconds(3450001);
  BOOST_CHECK_EQUAL(time.toString(cBuffer, 17), 12);
  BOOST_CHECK_EQUAL(std::string(cBuffer), "T#3s450ms1us");
}

BOOST_AUTO_TEST_CASE(toString_with_negative_times_test)
{
  CIEC_TIME time;
  char cBuffer[17];

  time.setFromMicroSeconds(-1);
  BOOST_CHECK_EQUAL(time.toString(cBuffer, 17), 6);
  BOOST_CHECK_EQUAL(std::string(cBuffer), "T#-1us");

  time.setFromMicroSeconds(-10);
  BOOST_CHECK_EQUAL(time.toString(cBuffer, 17), 7);
  BOOST_CHECK_EQUAL(std::string(cBuffer), "T#-10us");

  time.setFromMicroSeconds(-100);
  BOOST_CHECK_EQUAL(time.toString(cBuffer, 17), 8);
  BOOST_CHECK_EQUAL(std::string(cBuffer), "T#-100us");

  time.setFromMicroSeconds(-3450001);
  BOOST_CHECK_EQUAL(time.toString(cBuffer, 17), 13);
  BOOST_CHECK_EQUAL(std::string(cBuffer), "T#-3s450ms1us");
}

BOOST_AUTO_TEST_CASE(time_comparision_tests){
  CIEC_TIME time1, time2;

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

BOOST_AUTO_TEST_CASE(get_time_in_nanoseconds)
{
  CIEC_TIME time;

  time.setFromNanoSeconds(1);
  BOOST_CHECK(time.getInNanoSeconds() == static_cast<CIEC_TIME::TValueType>(CIEC_TIME(static_cast<CIEC_TIME::TValueType>(1))));

  time.setFromNanoSeconds(10);
  BOOST_CHECK(time.getInNanoSeconds() == static_cast<CIEC_TIME::TValueType>(CIEC_TIME(static_cast<CIEC_TIME::TValueType>(10))));

  time.setFromNanoSeconds(999);
  BOOST_CHECK(time.getInNanoSeconds() == static_cast<CIEC_TIME::TValueType>(CIEC_TIME(static_cast<CIEC_TIME::TValueType>(999))));

  time.setFromNanoSeconds(10325643);
  BOOST_CHECK(time.getInNanoSeconds() == static_cast<CIEC_TIME::TValueType>(CIEC_TIME(static_cast<CIEC_TIME::TValueType>(10325643))));
}

BOOST_AUTO_TEST_CASE(parse_time_literal_in_days)
{
  CIEC_TIME time;

  time.fromString("T#1d");
  BOOST_CHECK_EQUAL(time.getInDays(), 1);

  time.fromString("T#10d");
  BOOST_CHECK_EQUAL(time.getInDays(), 10);

  time.fromString("T#99d");
  BOOST_CHECK_EQUAL(time.getInDays(), 99);

  time.fromString("T#56123d");
  BOOST_CHECK_EQUAL(time.getInDays(), 56123);
}

BOOST_AUTO_TEST_CASE(parse_negative_signed_time_literal_in_days)
{
  CIEC_TIME time;

  time.fromString("T#-1d");
  BOOST_CHECK_EQUAL(time.getInDays(), -1);

  time.fromString("T#-10d");
  BOOST_CHECK_EQUAL(time.getInDays(), -10);

  time.fromString("T#-99d");
  BOOST_CHECK_EQUAL(time.getInDays(), -99);

  time.fromString("T#-56123d");
  BOOST_CHECK_EQUAL(time.getInDays(), -56123);
  
}

BOOST_AUTO_TEST_CASE(parse_time_literal_in_hours)
{
  CIEC_TIME time;

  time.fromString("T#1h");
  BOOST_CHECK_EQUAL(time.getInHours(), 1);

  time.fromString("T#10h");
  BOOST_CHECK_EQUAL(time.getInHours(), 10);

  time.fromString("T#99h");
  BOOST_CHECK_EQUAL(time.getInHours(), 99);

  time.fromString("T#12345h");
  BOOST_CHECK_EQUAL(time.getInHours(), 12345);
}

BOOST_AUTO_TEST_CASE(parse_negative_signed_time_literal_in_hours)
{
  CIEC_TIME time;

  time.fromString("T#-1h");
  BOOST_CHECK_EQUAL(time.getInHours(), -1);

  time.fromString("T#-10h");
  BOOST_CHECK_EQUAL(time.getInHours(), -10);

  time.fromString("T#-99h");
  BOOST_CHECK_EQUAL(time.getInHours(), -99);

  time.fromString("T#-12345h");
  BOOST_CHECK_EQUAL(time.getInHours(), -12345);
  
}

BOOST_AUTO_TEST_CASE(parse_time_literal_in_minutes)
{
  CIEC_TIME time;

  time.fromString("T#1m");
  BOOST_CHECK_EQUAL(time.getInMinutes(), 1);

  time.fromString("T#10m");
  BOOST_CHECK_EQUAL(time.getInMinutes(), 10);

  time.fromString("T#99m");
  BOOST_CHECK_EQUAL(time.getInMinutes(), 99);

  time.fromString("T#10325643m");
  BOOST_CHECK_EQUAL(time.getInMinutes(), 10325643);
}

BOOST_AUTO_TEST_CASE(parse_negative_signed_time_literal_in_minutes)
{
  CIEC_TIME time;

  time.fromString("T#-1m");
  BOOST_CHECK_EQUAL(time.getInMinutes(), -1);

  time.fromString("T#-10m");
  BOOST_CHECK_EQUAL(time.getInMinutes(), -10);

  time.fromString("T#-99m");
  BOOST_CHECK_EQUAL(time.getInMinutes(), -99);

  time.fromString("T#-10325643m");
  BOOST_CHECK_EQUAL(time.getInMinutes(), -10325643);
  
}

BOOST_AUTO_TEST_CASE(parse_time_literal_in_seconds)
{
  CIEC_TIME time;

  time.fromString("T#1s");
  BOOST_CHECK_EQUAL(time.getInSeconds(), 1);

  time.fromString("T#10s");
  BOOST_CHECK_EQUAL(time.getInSeconds(), 10);

  time.fromString("T#99s");
  BOOST_CHECK_EQUAL(time.getInSeconds(), 99);

  time.fromString("T#10325643s");
  BOOST_CHECK_EQUAL(time.getInSeconds(), 10325643);
}

BOOST_AUTO_TEST_CASE(parse_negative_signed_time_literal_in_seconds)
{
  CIEC_TIME time;

  time.fromString("T#-1s");
  BOOST_CHECK_EQUAL(time.getInSeconds(), -1);

  time.fromString("T#-10s");
  BOOST_CHECK_EQUAL(time.getInSeconds(), -10);

  time.fromString("T#-99s");
  BOOST_CHECK_EQUAL(time.getInSeconds(), -99);

  time.fromString("T#-10325643s");
  BOOST_CHECK_EQUAL(time.getInSeconds(), -10325643);
  
}

BOOST_AUTO_TEST_CASE(parse_time_literal_in_milliseconds)
{
  CIEC_TIME time;

  time.fromString("T#1ms");
  BOOST_CHECK_EQUAL(time.getInMilliSeconds(), 1);

  time.fromString("T#10ms");
  BOOST_CHECK_EQUAL(time.getInMilliSeconds(), 10);

  time.fromString("T#999ms");
  BOOST_CHECK_EQUAL(time.getInMilliSeconds(), 999);

  time.fromString("T#10325643ms");
  BOOST_CHECK_EQUAL(time.getInMilliSeconds(), 10325643);
}

BOOST_AUTO_TEST_CASE(parse_positive_signed_time_literal_in_milliseconds)
{
  CIEC_TIME time;

  time.fromString("T#+1ms");
  BOOST_CHECK_EQUAL(time.getInMilliSeconds(), 1);

  time.fromString("T#+10ms");
  BOOST_CHECK_EQUAL(time.getInMilliSeconds(), 10);

  time.fromString("T#+999ms");
  BOOST_CHECK_EQUAL(time.getInMilliSeconds(), 999);

  time.fromString("T#+10325643ms");
  BOOST_CHECK_EQUAL(time.getInMilliSeconds(), 10325643);
}

BOOST_AUTO_TEST_CASE(parse_negative_signed_time_literal_in_milliseconds)
{
  CIEC_TIME time;

  time.fromString("T#-1ms");
  BOOST_CHECK_EQUAL(time.getInMilliSeconds(), -1);

  time.fromString("T#-10ms");
  BOOST_CHECK_EQUAL(time.getInMilliSeconds(), -10);

  time.fromString("T#-999ms");
  BOOST_CHECK_EQUAL(time.getInMilliSeconds(), -999);

  time.fromString("T#-10325643ms");
  BOOST_CHECK_EQUAL(time.getInMilliSeconds(), -10325643);
}

BOOST_AUTO_TEST_CASE(parse_time_literal_in_microseconds)
{
  CIEC_TIME time;

  time.fromString("T#1us");
  BOOST_CHECK_EQUAL(time.getInMicroSeconds(), 1);

  time.fromString("T#10us");
  BOOST_CHECK_EQUAL(time.getInMicroSeconds(), 10);

  time.fromString("T#999us");
  BOOST_CHECK_EQUAL(time.getInMicroSeconds(), 999);

  time.fromString("T#10325643us");
  BOOST_CHECK_EQUAL(time.getInMicroSeconds(), 10325643);
}

BOOST_AUTO_TEST_CASE(parse_negative_signed_time_literal_in_microseconds)
{
  CIEC_TIME time;

  time.fromString("T#-1us");
  BOOST_CHECK_EQUAL(time.getInMicroSeconds(), -1);

  time.fromString("T#-10us");
  BOOST_CHECK_EQUAL(time.getInMicroSeconds(), -10);

  time.fromString("T#-999us");
  BOOST_CHECK_EQUAL(time.getInMicroSeconds(), -999);

  time.fromString("T#-10325643us");
  BOOST_CHECK_EQUAL(time.getInMicroSeconds(), -10325643);
}

BOOST_AUTO_TEST_CASE(parse_missing_s_in_microseconds_literal)
{
  CIEC_TIME time;

  BOOST_CHECK_EQUAL(time.fromString("T#-1u"), -1);
  BOOST_CHECK_EQUAL(time.getInMicroSeconds(), 0);

  BOOST_CHECK_EQUAL(time.fromString("T#-1ut"), -1);
  BOOST_CHECK_EQUAL(time.getInMicroSeconds(), 0);
}

BOOST_AUTO_TEST_CASE(parse_time_literal_in_nanoseconds)
{
  CIEC_TIME time;

  time.fromString("T#1ns");
  BOOST_CHECK_EQUAL(time.getInNanoSeconds(), 1);

  time.fromString("T#10ns");
  BOOST_CHECK_EQUAL(time.getInNanoSeconds(), 10);

  time.fromString("T#999ns");
  BOOST_CHECK_EQUAL(time.getInNanoSeconds(), 999);

  time.fromString("T#10325643ns");
  BOOST_CHECK_EQUAL(time.getInNanoSeconds(), 10325643);
}

BOOST_AUTO_TEST_CASE(parse_missing_s_in_nanoseconds_literal)
{
  CIEC_TIME time;

  BOOST_CHECK_EQUAL(time.fromString("T#1234nx"), -1);
  BOOST_CHECK_EQUAL(time.getInMicroSeconds(), 0);
}

BOOST_AUTO_TEST_CASE(parse_negative_signed_time_literal_in_nanoseconds)
{
  CIEC_TIME time;

  time.fromString("T#-1ns");
  BOOST_CHECK_EQUAL(time.getInNanoSeconds(), -1);

  time.fromString("T#-10ns");
  BOOST_CHECK_EQUAL(time.getInNanoSeconds(), -10);

  time.fromString("T#-999ns");
  BOOST_CHECK_EQUAL(time.getInNanoSeconds(), -999);

  time.fromString("T#-10325643ns");
  BOOST_CHECK_EQUAL(time.getInNanoSeconds(), -10325643);
}

BOOST_AUTO_TEST_CASE(parse_time_literals_with_missing_end_unit)
{
  CIEC_TIME time;

  BOOST_CHECK_EQUAL(-1, time.fromString("T#68231"));
  BOOST_CHECK_EQUAL(time.getInNanoSeconds(), 0);
  BOOST_CHECK_EQUAL(-1, time.fromString("T#1234r"));
  BOOST_CHECK_EQUAL(time.getInNanoSeconds(), 0);
  BOOST_CHECK_EQUAL(-1, time.fromString("T#1h23"));
  BOOST_CHECK_EQUAL(time.getInNanoSeconds(), 0);
  BOOST_CHECK_EQUAL(-1, time.fromString("T#23m89"));
  BOOST_CHECK_EQUAL(time.getInNanoSeconds(), 0);
  BOOST_CHECK_EQUAL(-1, time.fromString("T#55s514"));
  BOOST_CHECK_EQUAL(time.getInNanoSeconds(), 0);
  BOOST_CHECK_EQUAL(-1, time.fromString("T#344ms1"));
  BOOST_CHECK_EQUAL(time.getInNanoSeconds(), 0);
  BOOST_CHECK_EQUAL(-1, time.fromString("T#344ms12399"));
  BOOST_CHECK_EQUAL(time.getInNanoSeconds(), 0);
  BOOST_CHECK_EQUAL(-1, time.fromString("T#344ms12399"));
  BOOST_CHECK_EQUAL(time.getInNanoSeconds(), 0);
}

BOOST_AUTO_TEST_CASE(parse_wrong_time_prefix)
{
  CIEC_TIME time;

  BOOST_CHECK_EQUAL(-1, time.fromString("T23m"));
  BOOST_CHECK_EQUAL(time.getInNanoSeconds(), 0);
  BOOST_CHECK_EQUAL(-1, time.fromString("TIME23m"));
  BOOST_CHECK_EQUAL(time.getInNanoSeconds(), 0);
  BOOST_CHECK_EQUAL(-1, time.fromString("Tome#23m"));
  BOOST_CHECK_EQUAL(time.getInNanoSeconds(), 0);
  BOOST_CHECK_EQUAL(-1, time.fromString("Tine#23m"));
  BOOST_CHECK_EQUAL(time.getInNanoSeconds(), 0);
  BOOST_CHECK_EQUAL(-1, time.fromString("Tima#23m"));
  BOOST_CHECK_EQUAL(time.getInNanoSeconds(), 0);
}

BOOST_AUTO_TEST_CASE(parse_time_in_struct_or_array_literal)
{
  CIEC_TIME time;

  BOOST_CHECK_EQUAL(7, time.fromString("T#999ms, "));
  BOOST_CHECK_EQUAL(time.getInMilliSeconds(), 999);

  BOOST_CHECK_EQUAL(6, time.fromString("T#23ms , "));
  BOOST_CHECK_EQUAL(time.getInMilliSeconds(), 23);

  BOOST_CHECK_EQUAL(12, time.fromString("T#10325643us,"));
  BOOST_CHECK_EQUAL(time.getInMicroSeconds(), 10325643);
}

BOOST_AUTO_TEST_CASE(InternalRepresentationTest) {
  CIEC_TIME directRep(3600000000000);
  CIEC_TIME fromStringRep;
  fromStringRep.fromString("T#1h");

  BOOST_TEST(directRep == fromStringRep);
}

BOOST_AUTO_TEST_SUITE_END()
