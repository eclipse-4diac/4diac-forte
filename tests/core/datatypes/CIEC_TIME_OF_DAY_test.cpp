/*******************************************************************************
 * Copyright (c) 2011 ACIN, nxtControl
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Martin Melik Merkumians, Ingo Hegny, Alois Zoitl, Stanislav Meduna - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include <boost/test/unit_test.hpp>

#include "../../../src/core/datatypes/forte_time_of_day.h"

BOOST_AUTO_TEST_SUITE(CIEC_TIME_OF_DAY_function_test)
BOOST_AUTO_TEST_CASE(Type_test)
{
  CIEC_TIME_OF_DAY nTest;
  //check type information
  BOOST_CHECK_EQUAL(nTest.getDataTypeID(), CIEC_ANY::e_TIME_OF_DAY);
  //check operator bool data type size
  BOOST_CHECK_EQUAL(sizeof(nTest.operator TForteUInt64()), sizeof(TForteUInt64));

}

BOOST_AUTO_TEST_CASE(Operator_test)
{
  CIEC_TIME_OF_DAY nTest1;
  CIEC_TIME_OF_DAY nTest2;

  //check if data type is initialized with value of zero
  BOOST_CHECK_EQUAL(nTest1,0ULL);

  nTest1 = std::numeric_limits<TForteUInt64>::min();
  nTest2 = nTest1;
  BOOST_CHECK_EQUAL(nTest1, std::numeric_limits<TForteUInt64>::min());
  BOOST_CHECK_EQUAL(nTest2, std::numeric_limits<TForteUInt64>::min());

  nTest1 = 123894391ULL;
  nTest2 = nTest1;
  BOOST_CHECK_EQUAL(nTest1, 123894391ULL);
  BOOST_CHECK_EQUAL(nTest2, 123894391ULL);

  nTest1 = std::numeric_limits<TForteUInt64>::max();
  nTest2 = nTest1;
  BOOST_CHECK_EQUAL(nTest1, std::numeric_limits<TForteUInt64>::max());
  BOOST_CHECK_EQUAL(nTest2, std::numeric_limits<TForteUInt64>::max());

  nTest2 = 2145729524ULL;
  BOOST_CHECK_EQUAL(nTest1, std::numeric_limits<TForteUInt64>::max());
  BOOST_CHECK_EQUAL(nTest2, 2145729524ULL);

  ++nTest2;
  BOOST_CHECK_EQUAL(nTest2, 2145729525ULL);

}

BOOST_AUTO_TEST_CASE(Conversion_test)
{
  CIEC_TIME_OF_DAY nTest;

  char cBuffer[17];
  char cBufferFail[2];

  //check cast operator
  nTest = 0;

  BOOST_CHECK_EQUAL(nTest.operator TForteUInt64(), 0ULL);

  nTest = std::numeric_limits<TForteUInt64>::min();
  BOOST_CHECK_EQUAL(nTest.operator TForteUInt64(), std::numeric_limits<TForteUInt64>::min());

  nTest = -21654147L;
  BOOST_CHECK_EQUAL(nTest.operator TForteUInt64(), 18446744073687897469ULL);

  nTest = std::numeric_limits<TForteUInt64>::max();
  BOOST_CHECK_EQUAL(nTest.operator TForteUInt64(), std::numeric_limits<TForteUInt64>::max());

  //check toString and fromString
  strcpy(cBuffer, "");

  nTest = 0;
  BOOST_CHECK_EQUAL(nTest.fromString("15:36:55.36"), 11);
  BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 17), 12);
  BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
  BOOST_CHECK_EQUAL(strcmp(cBuffer, "15:36:55.360"), 0);
  strcpy(cBuffer, "");

  nTest = 0;
  BOOST_CHECK_EQUAL(nTest.fromString("15:36:55"), 8);
  BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 17), 12);
  BOOST_CHECK_EQUAL(strcmp(cBuffer, "15:36:55.000"), 0);
  strcpy(cBuffer, "");

  nTest = 0;
  BOOST_CHECK_EQUAL(nTest.fromString("15:36:55."), 9);
  BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 17), 12);
  BOOST_CHECK_EQUAL(strcmp(cBuffer, "15:36:55.000"), 0);
  strcpy(cBuffer, "");

  nTest = 0;
  BOOST_CHECK_EQUAL(nTest.fromString("15:36:55.0"), 10);
  BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 17), 12);
  BOOST_CHECK_EQUAL(strcmp(cBuffer, "15:36:55.000"), 0);
  strcpy(cBuffer, "");

  nTest = 0;
  BOOST_CHECK_EQUAL(nTest.fromString("15:36:55.1"), 10);
  BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 17), 12);
  BOOST_CHECK_EQUAL(strcmp(cBuffer, "15:36:55.100"), 0);
  strcpy(cBuffer, "");

  nTest = 0;
  BOOST_CHECK_EQUAL(nTest.fromString("15:36:55.10"), 11);
  BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 17), 12);
  BOOST_CHECK_EQUAL(strcmp(cBuffer, "15:36:55.100"), 0);

  nTest = 0;
  strcpy(cBuffer, "");
  BOOST_CHECK_EQUAL(nTest.fromString("15:36:55.100"), 12);
  BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 17), 12);
  BOOST_CHECK_EQUAL(strcmp(cBuffer, "15:36:55.100"), 0);
  strcpy(cBuffer, "");

  nTest = 0;
  BOOST_CHECK_EQUAL(nTest.fromString("15:36:55.12"), 11);
  BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 17), 12);
  BOOST_CHECK_EQUAL(strcmp(cBuffer, "15:36:55.120"), 0);
  strcpy(cBuffer, "");

  nTest = 0;
  BOOST_CHECK_EQUAL(nTest.fromString("15:36:55.120"), 12);
  BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 17), 12);
  BOOST_CHECK_EQUAL(strcmp(cBuffer, "15:36:55.120"), 0);
  strcpy(cBuffer, "");

  nTest = 0;
  BOOST_CHECK_EQUAL(nTest.fromString("15:36:55.123"), 12);
  BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 17), 12);
  BOOST_CHECK_EQUAL(strcmp(cBuffer, "15:36:55.123"), 0);
  strcpy(cBuffer, "");

  nTest = 0;
  BOOST_CHECK_EQUAL(nTest.fromString("15:36:55.1234"), 13);
  BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 17), 12);
  BOOST_CHECK_EQUAL(strcmp(cBuffer, "15:36:55.123"), 0);
  strcpy(cBuffer, "");

  
  nTest = 0;
  BOOST_CHECK_EQUAL(nTest.fromString("TIME_OF_DAY#15:36:55.36"), 23);
  BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 17), 12);
  BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
  BOOST_CHECK_EQUAL(strcmp(cBuffer, "15:36:55.360"), 0);
  strcpy(cBuffer, "");

  nTest = 0;
  BOOST_CHECK_EQUAL(nTest.fromString("time_of_day#15:36:55.36"), 23);
  BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 17), 12);
  BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
  BOOST_CHECK_EQUAL(strcmp(cBuffer, "15:36:55.360"), 0);
  strcpy(cBuffer, "");

  nTest = 0;
  BOOST_CHECK_EQUAL(nTest.fromString("TOD#15:36:55.36"), 15);
  BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 17), 12);
  BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
  BOOST_CHECK_EQUAL(strcmp(cBuffer, "15:36:55.360"), 0);
  strcpy(cBuffer, "");

  nTest = 0;
  BOOST_CHECK_EQUAL(nTest.fromString("tod#15:36:55.36"), 15);
  BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 17), 12);
  BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
  BOOST_CHECK_EQUAL(strcmp(cBuffer, "15:36:55.360"), 0);
  strcpy(cBuffer, "");

  nTest = 0;
  BOOST_CHECK_EQUAL(nTest.fromString("8:6:5.6"), 7);
  BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 17), 12);
  BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
  BOOST_CHECK_EQUAL(strcmp(cBuffer, "08:06:05.600"), 0);
  strcpy(cBuffer, "");

  nTest = 0;
  BOOST_CHECK_EQUAL(nTest.fromString("TIME_OF_DAY#8:6:5.6"), 19);
  BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 17), 12);
  BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
  BOOST_CHECK_EQUAL(strcmp(cBuffer, "08:06:05.600"), 0);
  strcpy(cBuffer, "");

  nTest = 0;
  BOOST_CHECK_EQUAL(nTest.fromString("time_of_day#8:6:5.6"), 19);
  BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 17), 12);
  BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
  BOOST_CHECK_EQUAL(strcmp(cBuffer, "08:06:05.600"), 0);
  strcpy(cBuffer, "");

  nTest = 0;
  BOOST_CHECK_EQUAL(nTest.fromString("TOD#8:6:5.6"), 11);
  BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 17), 12);
  BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
  BOOST_CHECK_EQUAL(strcmp(cBuffer, "08:06:05.600"), 0);
  strcpy(cBuffer, "");

  nTest = 0;
  BOOST_CHECK_EQUAL(nTest.fromString("tod#8:6:5.6"), 11);
  BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 17), 12);
  BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
  BOOST_CHECK_EQUAL(strcmp(cBuffer, "08:06:05.600"), 0);
  strcpy(cBuffer, "");

}
BOOST_AUTO_TEST_SUITE_END()
