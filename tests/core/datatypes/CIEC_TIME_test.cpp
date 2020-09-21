/*******************************************************************************
 * Copyright (c) 2011, 2103 ACIN, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Martin Melik Merkumians, Ingo Hegny, Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include <boost/test/unit_test.hpp>
#include <string>

#include "../../../src/core/datatypes/forte_time.h"

BOOST_AUTO_TEST_SUITE(CIEC_TIME_function_test)
BOOST_AUTO_TEST_CASE(Type_test)
{
  CIEC_TIME nTest;
  //check type information
  BOOST_CHECK_EQUAL(nTest.getDataTypeID(), CIEC_ANY::e_TIME);
  //check operator bool data type size
#ifdef FORTE_USE_64BIT_DATATYPES
  BOOST_CHECK_EQUAL(sizeof(nTest.operator TForteInt64()), sizeof(TForteInt64));
#else
  BOOST_CHECK_EQUAL(sizeof(nTest.operator TForteInt32()), sizeof(TForteInt32));
#endif
}

BOOST_AUTO_TEST_CASE(Operator_test)
{
  CIEC_TIME nTest1;
  CIEC_TIME nTest2;

  //check if data type is initialized with value of zero
  BOOST_CHECK_EQUAL(nTest1,0);

#ifdef FORTE_USE_64BIT_DATATYPES
  nTest1 = std::numeric_limits<TForteInt64>::min();
  nTest2 = nTest1;
  BOOST_CHECK_EQUAL(nTest1, std::numeric_limits<TForteInt64>::min());
  BOOST_CHECK_EQUAL(nTest2, std::numeric_limits<TForteInt64>::min());

  nTest1 = std::numeric_limits<TForteInt64>::max();
  nTest2 = nTest1;
  BOOST_CHECK_EQUAL(nTest1, std::numeric_limits<TForteInt64>::max());
  BOOST_CHECK_EQUAL(nTest2, std::numeric_limits<TForteInt64>::max());
#endif

  nTest1 = std::numeric_limits<TForteInt32>::min();
  nTest2 = nTest1;
  BOOST_CHECK_EQUAL(nTest1, std::numeric_limits<TForteInt32>::min());
  BOOST_CHECK_EQUAL(nTest2, std::numeric_limits<TForteInt32>::min());

  nTest1 = 9657545ULL;
  nTest2 = nTest1;
  BOOST_CHECK_EQUAL(nTest1, 9657545LL);
  BOOST_CHECK_EQUAL(nTest2, 9657545LL);

  nTest1 = std::numeric_limits<TForteInt32>::max();
  nTest2 = nTest1;
  BOOST_CHECK_EQUAL(nTest1, std::numeric_limits<TForteInt32>::max());
  BOOST_CHECK_EQUAL(nTest2, std::numeric_limits<TForteInt32>::max());

  nTest2 = 564874642ULL;
  BOOST_CHECK_EQUAL(nTest1, std::numeric_limits<TForteInt32>::max());
  BOOST_CHECK_EQUAL(nTest2, 564874642LL);

  ++nTest2;
  BOOST_CHECK_EQUAL(nTest2, 564874643LL);

}

BOOST_AUTO_TEST_CASE(Conversion_test)
{
  CIEC_TIME nTest;

  char cBuffer[17];
  char cBufferFail[2];

  //check cast operator
  nTest = 0;

#ifdef FORTE_USE_64BIT_DATATYPES
  BOOST_CHECK_EQUAL(nTest.operator TForteInt64(), 0);
#else
  BOOST_CHECK_EQUAL(nTest.operator TForteInt32(), 0);
#endif

  nTest = std::numeric_limits<TForteInt32>::min();
#ifdef FORTE_USE_64BIT_DATATYPES
  BOOST_CHECK_EQUAL(nTest.operator TForteInt64(), std::numeric_limits<TForteInt32>::min());
#else
  BOOST_CHECK_EQUAL(nTest.operator TForteInt32(), std::numeric_limits<TForteInt32>::min());
#endif
  nTest = -21654147L;
#ifdef FORTE_USE_64BIT_DATATYPES
  BOOST_CHECK_EQUAL(nTest.operator TForteInt64(), -21654147L);
#else
  BOOST_CHECK_EQUAL(nTest.operator TForteInt32(), -21654147L);
#endif
  nTest = std::numeric_limits<TForteInt32>::max();
#ifdef FORTE_USE_64BIT_DATATYPES
  BOOST_CHECK_EQUAL(nTest.operator TForteInt64(), std::numeric_limits<TForteInt32>::max());
#else
  BOOST_CHECK_EQUAL(nTest.operator TForteInt32(), std::numeric_limits<TForteInt32>::max());
#endif
  //check toString and fromString
  strcpy(cBuffer, "");

  BOOST_CHECK_EQUAL(nTest.fromString("4h36m"), 1);
  /*BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 17), 12);
  BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
  BOOST_CHECK_EQUAL(strcmp(cBuffer, "T#16560000ms"), 0);
  strcpy(cBuffer, "");*/

  BOOST_CHECK_EQUAL(nTest.fromString("TIME#4h36m"), 10);
  BOOST_CHECK_EQUAL(nTest, 16560000LL * (cgForteTimeBaseUnitsPerSecond / 1000));
  BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 17), 12);
  BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
  BOOST_CHECK_EQUAL(std::string(cBuffer), "T#16560000ms");
  strcpy(cBuffer, "");
  nTest = 0;

  BOOST_CHECK_EQUAL(nTest.fromString("time#4h36m"), 10);
  BOOST_CHECK_EQUAL(nTest, 16560000LL * (cgForteTimeBaseUnitsPerSecond / 1000));
  BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 17), 12);
  BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
  BOOST_CHECK_EQUAL(std::string(cBuffer), "T#16560000ms");
  strcpy(cBuffer, "");
  nTest = 0;

  BOOST_CHECK_EQUAL(nTest.fromString("T#4h36m"), 7);
  BOOST_CHECK_EQUAL(nTest, 16560000LL * (cgForteTimeBaseUnitsPerSecond / 1000));
  BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 17), 12);
  BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
  BOOST_CHECK_EQUAL(std::string(cBuffer), "T#16560000ms");
  strcpy(cBuffer, "");
  nTest = 0;

  BOOST_CHECK_EQUAL(nTest.fromString("t#4h36m"), 7);
  BOOST_CHECK_EQUAL(nTest, 16560000LL * (cgForteTimeBaseUnitsPerSecond / 1000));
  BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 17), 12);
  BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
  BOOST_CHECK_EQUAL(std::string(cBuffer), "T#16560000ms");
  strcpy(cBuffer, "");
  nTest = 0;

  BOOST_CHECK_EQUAL(nTest.fromString("3s_22ms"), 1);
  /*BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 17), 17);
  BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
  BOOST_CHECK_EQUAL(strcmp(cBuffer, "T#3022ms"), 0);
  strcpy(cBuffer, "");*/

  BOOST_CHECK_EQUAL(nTest.fromString("TIME#3s_22ms"), 12);
  BOOST_CHECK_EQUAL(nTest, (CIEC_TIME::TValueType)(3022LL * (cgForteTimeBaseUnitsPerSecond / 1000LL)));
  BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 17), 8);
  BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
  BOOST_CHECK_EQUAL(std::string(cBuffer), "T#3022ms");
  strcpy(cBuffer, "");
  nTest = 0;

  BOOST_CHECK_EQUAL(nTest.fromString("time#3s_22ms"), 12);
  BOOST_CHECK_EQUAL(nTest, 3022LL * (cgForteTimeBaseUnitsPerSecond / 1000LL));
  BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 17), 8);
  BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
  BOOST_CHECK_EQUAL(std::string(cBuffer), "T#3022ms");
  strcpy(cBuffer, "");
  nTest = 0;

  BOOST_CHECK_EQUAL(nTest.fromString("T#3s_22ms"), 9);
  BOOST_CHECK_EQUAL(nTest, 3022LL * (cgForteTimeBaseUnitsPerSecond / 1000LL));
  BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 17), 8);
  BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
  BOOST_CHECK_EQUAL(std::string(cBuffer), "T#3022ms");
  strcpy(cBuffer, "");
  nTest = 0;

  BOOST_CHECK_EQUAL(nTest.fromString("t#3s_22ms"), 9);
  BOOST_CHECK_EQUAL(nTest, 3022LL * (cgForteTimeBaseUnitsPerSecond / 1000LL));
  BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 17), 8);
  BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
  BOOST_CHECK_EQUAL(std::string(cBuffer), "T#3022ms");
  strcpy(cBuffer, "");
  nTest = 0;
}

BOOST_AUTO_TEST_CASE(toString_with_microseconds_test){
  CIEC_TIME time;
  char cBuffer[17];

  time.setFromMicroSeconds(0);
  BOOST_CHECK_EQUAL(time.toString(cBuffer, 17), 5);
  BOOST_CHECK_EQUAL(std::string(cBuffer), "T#0ms");

  time.setFromMicroSeconds(1);
  BOOST_CHECK_EQUAL(time.toString(cBuffer, 17), 9);
  BOOST_CHECK_EQUAL(std::string(cBuffer), "T#0.001ms");

  time.setFromMicroSeconds(10);
  BOOST_CHECK_EQUAL(time.toString(cBuffer, 17), 9);
  BOOST_CHECK_EQUAL(std::string(cBuffer), "T#0.010ms");

  time.setFromMicroSeconds(100);
  BOOST_CHECK_EQUAL(time.toString(cBuffer, 17), 9);
  BOOST_CHECK_EQUAL(std::string(cBuffer), "T#0.100ms");

  time.setFromMicroSeconds(3450001);
  BOOST_CHECK_EQUAL(time.toString(cBuffer, 17), 12);
  BOOST_CHECK_EQUAL(std::string(cBuffer), "T#3450.001ms");
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
  BOOST_CHECK(time.getInNanoSeconds() == 1);

  time.setFromNanoSeconds(10);
  BOOST_CHECK(time.getInNanoSeconds() == 10);

  time.setFromNanoSeconds(999);
  BOOST_CHECK(time.getInNanoSeconds() == 999);

  time.setFromNanoSeconds(10325643);
  BOOST_CHECK(time.getInNanoSeconds() == 10325643);
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

BOOST_AUTO_TEST_SUITE_END()
