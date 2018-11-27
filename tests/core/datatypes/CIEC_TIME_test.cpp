/*******************************************************************************
 * Copyright (c) 2011, 2103 ACIN, fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
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
  BOOST_CHECK_EQUAL(nTest, 16560000LL * (FORTE_TIME_BASE_UNITS_PER_SECOND / 1000));
  BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 17), 12);
  BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
  BOOST_CHECK_EQUAL(std::string(cBuffer), "T#16560000ms");
  strcpy(cBuffer, "");
  nTest = 0;

  BOOST_CHECK_EQUAL(nTest.fromString("time#4h36m"), 10);
  BOOST_CHECK_EQUAL(nTest, 16560000LL * (FORTE_TIME_BASE_UNITS_PER_SECOND / 1000));
  BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 17), 12);
  BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
  BOOST_CHECK_EQUAL(std::string(cBuffer), "T#16560000ms");
  strcpy(cBuffer, "");
  nTest = 0;

  BOOST_CHECK_EQUAL(nTest.fromString("T#4h36m"), 7);
  BOOST_CHECK_EQUAL(nTest, 16560000LL * (FORTE_TIME_BASE_UNITS_PER_SECOND / 1000));
  BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 17), 12);
  BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
  BOOST_CHECK_EQUAL(std::string(cBuffer), "T#16560000ms");
  strcpy(cBuffer, "");
  nTest = 0;

  BOOST_CHECK_EQUAL(nTest.fromString("t#4h36m"), 7);
  BOOST_CHECK_EQUAL(nTest, 16560000LL * (FORTE_TIME_BASE_UNITS_PER_SECOND / 1000));
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
  BOOST_CHECK_EQUAL(nTest, (CIEC_TIME::TValueType)(3022LL * (FORTE_TIME_BASE_UNITS_PER_SECOND / 1000LL)));
  BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 17), 8);
  BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
  BOOST_CHECK_EQUAL(std::string(cBuffer), "T#3022ms");
  strcpy(cBuffer, "");
  nTest = 0;

  BOOST_CHECK_EQUAL(nTest.fromString("time#3s_22ms"), 12);
  BOOST_CHECK_EQUAL(nTest, 3022LL * (FORTE_TIME_BASE_UNITS_PER_SECOND / 1000LL));
  BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 17), 8);
  BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
  BOOST_CHECK_EQUAL(std::string(cBuffer), "T#3022ms");
  strcpy(cBuffer, "");
  nTest = 0;

  BOOST_CHECK_EQUAL(nTest.fromString("T#3s_22ms"), 9);
  BOOST_CHECK_EQUAL(nTest, 3022LL * (FORTE_TIME_BASE_UNITS_PER_SECOND / 1000LL));
  BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 17), 8);
  BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
  BOOST_CHECK_EQUAL(std::string(cBuffer), "T#3022ms");
  strcpy(cBuffer, "");
  nTest = 0;

  BOOST_CHECK_EQUAL(nTest.fromString("t#3s_22ms"), 9);
  BOOST_CHECK_EQUAL(nTest, 3022LL * (FORTE_TIME_BASE_UNITS_PER_SECOND / 1000LL));
  BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 17), 8);
  BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
  BOOST_CHECK_EQUAL(std::string(cBuffer), "T#3022ms");
  strcpy(cBuffer, "");
  nTest = 0;
}

BOOST_AUTO_TEST_CASE(time_fromString_with_microseconds_test){
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

  time1.setFromMiliSeconds(200);
  time2.setFromMiliSeconds(200);

  BOOST_CHECK(time1 == time2);
  BOOST_CHECK_EQUAL(true, time1 >= time2);
  BOOST_CHECK_EQUAL(true, time1 <= time2);

  time2.setFromMiliSeconds(150);

  BOOST_CHECK_EQUAL(true, time1 > time2);
  BOOST_CHECK_EQUAL(true, time1 >= time2);
  BOOST_CHECK_EQUAL(true, time1 != time2);
  BOOST_CHECK_EQUAL(false, time1 < time2);
  BOOST_CHECK_EQUAL(false, time1 <= time2);

  time1.setFromMiliSeconds(120);
  BOOST_CHECK_EQUAL(false, time1 > time2);
  BOOST_CHECK_EQUAL(false, time1 >= time2);
  BOOST_CHECK_EQUAL(true, time1 != time2);
  BOOST_CHECK_EQUAL(true, time1 < time2);
  BOOST_CHECK_EQUAL(true, time1 <= time2);

}

BOOST_AUTO_TEST_SUITE_END()
