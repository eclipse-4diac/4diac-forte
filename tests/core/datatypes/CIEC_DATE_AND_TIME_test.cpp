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

#include "../../../src/core/datatypes/forte_date_and_time.h"

BOOST_AUTO_TEST_SUITE(CIEC_DATE_AND_TIME_function_test)

BOOST_AUTO_TEST_CASE(Type_test)
{
  CIEC_DATE_AND_TIME nTest;
  //check type information
  BOOST_CHECK_EQUAL(nTest.getDataTypeID(), CIEC_ANY::e_DATE_AND_TIME);
  //check operator bool data type size
  BOOST_CHECK_EQUAL(sizeof(nTest.operator TForteUInt64()), sizeof(TForteUInt64));

}

BOOST_AUTO_TEST_CASE(Operator_test)
{
  CIEC_DATE_AND_TIME nTest1;
  CIEC_DATE_AND_TIME nTest2;

  //check if data type is initialized with value of zero
  BOOST_CHECK_EQUAL(nTest1,0ULL);

  nTest1 = std::numeric_limits<TForteUInt64>::min();
  nTest2 = nTest1;
  BOOST_CHECK_EQUAL(nTest1, std::numeric_limits<TForteUInt64>::min());
  BOOST_CHECK_EQUAL(nTest2, std::numeric_limits<TForteUInt64>::min());

  nTest1 = 9657545ULL;
  nTest2 = nTest1;
  BOOST_CHECK_EQUAL(nTest1, 9657545ULL);
  BOOST_CHECK_EQUAL(nTest2, 9657545ULL);

  nTest1 = std::numeric_limits<TForteUInt64>::max();
  nTest2 = nTest1;
  BOOST_CHECK_EQUAL(nTest1, std::numeric_limits<TForteUInt64>::max());
  BOOST_CHECK_EQUAL(nTest2, std::numeric_limits<TForteUInt64>::max());

  nTest2 = 564874642ULL;
  BOOST_CHECK_EQUAL(nTest1, std::numeric_limits<TForteUInt64>::max());
  BOOST_CHECK_EQUAL(nTest2, 564874642ULL);

  ++nTest2;
  BOOST_CHECK_EQUAL(nTest2, 564874643ULL);

}

BOOST_AUTO_TEST_CASE(Conversion_test)
{
  CIEC_DATE_AND_TIME nTest;

  char cBuffer[40];
  char cBufferFail[2];

  //check cast operator
  nTest = 0;

  BOOST_CHECK_EQUAL(nTest.operator TForteUInt64(), 0ULL);

  nTest = std::numeric_limits<TForteUInt64>::min();
  BOOST_CHECK_EQUAL(nTest.operator TForteUInt64(), std::numeric_limits<TForteUInt64>::min());

  nTest = 4268743213844LL;
  BOOST_CHECK_EQUAL(nTest.operator TForteUInt64(), 4268743213844ULL);

  nTest = std::numeric_limits<TForteUInt64>::max();
  BOOST_CHECK_EQUAL(nTest.operator TForteUInt64(), std::numeric_limits<TForteUInt64>::max());

  //check toString and fromString
  strcpy(cBuffer, "");

  nTest = 0;
  BOOST_CHECK_EQUAL(nTest.fromString("1994-06-22-14:23:54.8"), 21);
  BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 40), 23);
  BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
  BOOST_CHECK_EQUAL(strcmp(cBuffer, "1994-06-22-14:23:54.800"), 0);
  strcpy(cBuffer, "");

  nTest = 0;
  BOOST_CHECK_EQUAL(nTest.fromString("1994-06-22-14:23:54"), 19);
  BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 40), 23);
  BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
  BOOST_CHECK_EQUAL(strcmp(cBuffer, "1994-06-22-14:23:54.000"), 0);
  strcpy(cBuffer, "");

  nTest = 0;
  BOOST_CHECK_EQUAL(nTest.fromString("1994-06-22-14:23:54.1"), 21);
  BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 40), 23);
  BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
  BOOST_CHECK_EQUAL(strcmp(cBuffer, "1994-06-22-14:23:54.100"), 0);
  strcpy(cBuffer, "");

  nTest = 0;
  BOOST_CHECK_EQUAL(nTest.fromString("1994-06-22-14:23:54.10"), 22);
  BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 40), 23);
  BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
  BOOST_CHECK_EQUAL(strcmp(cBuffer, "1994-06-22-14:23:54.100"), 0);
  strcpy(cBuffer, "");

  nTest = 0;
  BOOST_CHECK_EQUAL(nTest.fromString("1994-06-22-14:23:54.100"), 23);
  BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 40), 23);
  BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
  BOOST_CHECK_EQUAL(strcmp(cBuffer, "1994-06-22-14:23:54.100"), 0);
  strcpy(cBuffer, "");
  
  nTest = 0;
  BOOST_CHECK_EQUAL(nTest.fromString("1994-06-22-14:23:54.12"), 22);
  BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 40), 23);
  BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
  BOOST_CHECK_EQUAL(strcmp(cBuffer, "1994-06-22-14:23:54.120"), 0);
  strcpy(cBuffer, "");

  nTest = 0;
  BOOST_CHECK_EQUAL(nTest.fromString("1994-06-22-14:23:54.120"), 23);
  BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 40), 23);
  BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
  BOOST_CHECK_EQUAL(strcmp(cBuffer, "1994-06-22-14:23:54.120"), 0);
  strcpy(cBuffer, "");

  nTest = 0;
  BOOST_CHECK_EQUAL(nTest.fromString("1994-06-22-14:23:54.123"), 23);
  BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 40), 23);
  BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
  BOOST_CHECK_EQUAL(strcmp(cBuffer, "1994-06-22-14:23:54.123"), 0);
  strcpy(cBuffer, "");
  
  nTest = 0;
  BOOST_CHECK_EQUAL(nTest.fromString("DATE_AND_TIME#1994-06-22-14:23:54.8"), 35);
  BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 40), 23);
  BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
  BOOST_CHECK_EQUAL(strcmp(cBuffer, "1994-06-22-14:23:54.800"), 0);
  strcpy(cBuffer, "");

  nTest = 0;
  BOOST_CHECK_EQUAL(nTest.fromString("date_and_time#1994-06-22-14:23:54.8"), 35);
  BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 40), 23);
  BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
  BOOST_CHECK_EQUAL(strcmp(cBuffer, "1994-06-22-14:23:54.800"), 0);
  strcpy(cBuffer, "");

  nTest = 0;
  BOOST_CHECK_EQUAL(nTest.fromString("DT#1994-06-22-14:23:54.8"), 24);
  BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 40), 23);
  BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
  BOOST_CHECK_EQUAL(strcmp(cBuffer, "1994-06-22-14:23:54.800"), 0);
  strcpy(cBuffer, "");

  nTest = 0;
  BOOST_CHECK_EQUAL(nTest.fromString("dt#1994-06-22-14:23:54.8"), 24);
  BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 40), 23);
  BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
  BOOST_CHECK_EQUAL(strcmp(cBuffer, "1994-06-22-14:23:54.800"), 0);
  strcpy(cBuffer, "");

  nTest = 0;
  BOOST_CHECK_EQUAL(nTest.fromString("2008-04-03-09:05:23.4"), 21);
  BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 40), 23);
  BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
  BOOST_CHECK_EQUAL(strcmp(cBuffer, "2008-04-03-09:05:23.400"), 0);
  strcpy(cBuffer, "");

  nTest = 0;
  BOOST_CHECK_EQUAL(nTest.fromString("DATE_AND_TIME#2008-04-03-09:05:23.4"), 35);
  BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 40), 23);
  BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
  BOOST_CHECK_EQUAL(strcmp(cBuffer, "2008-04-03-09:05:23.400"), 0);
  strcpy(cBuffer, "");

  nTest = 0;
  BOOST_CHECK_EQUAL(nTest.fromString("date_and_time#2008-04-03-09:05:23.4"), 35);
  BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 40), 23);
  BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
  BOOST_CHECK_EQUAL(strcmp(cBuffer, "2008-04-03-09:05:23.400"), 0);
  strcpy(cBuffer, "");
  nTest = 0;

  BOOST_CHECK_EQUAL(nTest.fromString("DT#2008-04-03-09:05:23.4"), 24);
  BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 40), 23);
  BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
  BOOST_CHECK_EQUAL(strcmp(cBuffer, "2008-04-03-09:05:23.400"), 0);
  strcpy(cBuffer, "");
  nTest = 0;

  BOOST_CHECK_EQUAL(nTest.fromString("dt#2008-04-03-09:05:23.4"), 24);
  BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 40), 23);
  BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
  BOOST_CHECK_EQUAL(strcmp(cBuffer, "2008-04-03-09:05:23.400"), 0);
  strcpy(cBuffer, "");
  nTest = 0;

}

BOOST_AUTO_TEST_SUITE_END()
