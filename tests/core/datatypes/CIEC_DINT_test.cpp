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
 *   Martin Jobst - add equals tests
 *                - add user-defined literal tests
 *******************************************************************************/
#include <boost/test/unit_test.hpp>
#include "forte_boost_output_support.h"

#include "../../../src/core/datatypes/forte_any_int.h"
#include "../../../src/core/datatypes/forte_dint.h"

BOOST_AUTO_TEST_SUITE(CIEC_DINT_function_test)
BOOST_AUTO_TEST_CASE(Type_test)
{
  CIEC_DINT nTest;
  //check type information
  BOOST_CHECK_EQUAL(nTest.getDataTypeID(), CIEC_ANY::e_DINT);
  //check operator bool data type size
  BOOST_CHECK_EQUAL(sizeof(nTest.operator TForteInt32()), sizeof(TForteInt32));

}

BOOST_AUTO_TEST_CASE(Literal_test)
{
  CIEC_DINT test1 = 0_DINT;
  BOOST_TEST(static_cast<CIEC_DINT::TValueType>(test1) == 0);

  CIEC_DINT test2 = 2147483647_DINT;
  BOOST_TEST(static_cast<CIEC_DINT::TValueType>(test2) == CIEC_DINT::scmMaxVal);

  CIEC_DINT test3 = -2147483648_DINT;
  BOOST_TEST(static_cast<CIEC_DINT::TValueType>(test3) == CIEC_DINT::scmMinVal);
}

BOOST_AUTO_TEST_CASE(Assignment_test)
{
  CIEC_DINT nTest1;
  CIEC_DINT nTest2;

  //initial value must be 0
  BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(nTest1), 0);

  nTest1 = CIEC_DINT(std::numeric_limits<TForteInt32>::min());
  nTest2 = nTest1;
  BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(nTest1), std::numeric_limits<TForteInt32>::min());
  BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(nTest2), std::numeric_limits<TForteInt32>::min());

  nTest1 = CIEC_DINT(-896475124);
  nTest2 = nTest1;
  BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(nTest1), -896475124);
  BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(nTest2), -896475124);

  nTest1 = CIEC_DINT(36548);
  nTest2 = nTest1;
  BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(nTest1), 36548);
  BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(nTest2), 36548);

  nTest1 = CIEC_DINT(std::numeric_limits<TForteInt32>::max());
  nTest2 = nTest1;
  BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(nTest1), std::numeric_limits<TForteInt32>::max());
  BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(nTest2), std::numeric_limits<TForteInt32>::max());

  //check that assignment operator does not intertwine objects
  nTest2 = CIEC_DINT(6874);
  BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(nTest1), std::numeric_limits<TForteInt32>::max());
  BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(nTest2), 6874);
}

BOOST_AUTO_TEST_CASE(Equality_test)
{
  CIEC_DINT nTest1;
  CIEC_DINT nTest2;

  BOOST_CHECK(nTest1.equals(nTest2));
  BOOST_CHECK(!nTest1.equals("abc"_STRING));

  nTest1 = CIEC_DINT(17);
  BOOST_CHECK(!nTest1.equals(nTest2));
  nTest2 = nTest1;
  BOOST_CHECK(nTest1.equals(nTest2));
}

BOOST_AUTO_TEST_CASE(ANY_INT_ASSIGNMENT_TEST) {
  CIEC_ANY_INT anyInt(50);
  CIEC_DINT test;

  test = anyInt;
  BOOST_TEST(50 == static_cast<CIEC_DINT::TValueType>(test));
}

BOOST_AUTO_TEST_CASE(Conversion_test)
{
  CIEC_DINT nTest;

  char cBuffer[12];
  char cBufferFail[2];

  //check cast operator
  nTest = CIEC_DINT(0);

  BOOST_CHECK_EQUAL(nTest.operator TForteInt32(), 0);

  nTest = CIEC_DINT(std::numeric_limits<TForteInt32>::min());
  BOOST_CHECK_EQUAL(nTest.operator TForteInt32(), std::numeric_limits<TForteInt32>::min());

  nTest = CIEC_DINT(9853268);
  BOOST_CHECK_EQUAL(nTest.operator TForteInt32(), 9853268);

  nTest = CIEC_DINT(std::numeric_limits<TForteInt32>::max());
  BOOST_CHECK_EQUAL(nTest.operator TForteInt32(), std::numeric_limits<TForteInt32>::max());

  //check toString and fromString
  strcpy(cBuffer, "");

  BOOST_CHECK_EQUAL(nTest.fromString("-2147483648"), 11);
  BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(nTest), -2147483648L);
  BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 12), 11);
  BOOST_CHECK_EQUAL(strcmp(cBuffer, "-2147483648"), 0);
  BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
  strcpy(cBuffer, "");

  BOOST_CHECK_EQUAL(nTest.fromString("2#10000000000000000000000000000000"), 34);
  BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(nTest), -2147483648L);
  BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 12), 11);
  BOOST_CHECK_EQUAL(strcmp(cBuffer, "-2147483648"), 0);
  BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
  strcpy(cBuffer, "");

  BOOST_CHECK_EQUAL(nTest.fromString("8#20000000000"), 13);
  BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(nTest), -2147483648L);
  BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 12), 11);
  BOOST_CHECK_EQUAL(strcmp(cBuffer, "-2147483648"), 0);
  BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
  strcpy(cBuffer, "");

  BOOST_CHECK_EQUAL(nTest.fromString("16#80000000"), 11);
  BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(nTest), -2147483648L);
  BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 12), 11);
  BOOST_CHECK_EQUAL(strcmp(cBuffer, "-2147483648"), 0);
  BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
  strcpy(cBuffer, "");

  BOOST_CHECK_EQUAL(nTest.fromString("0"), 1);
  BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(nTest), 0);
  BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 12), 1);
  BOOST_CHECK_EQUAL(strcmp(cBuffer, "0"), 0);
  BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 0), -1);
  strcpy(cBuffer, "");

  BOOST_CHECK_EQUAL(nTest.fromString("2#0"), 3);
  BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(nTest), 0);
  BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 12), 1);
  BOOST_CHECK_EQUAL(strcmp(cBuffer, "0"), 0);
  BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 0), -1);
  strcpy(cBuffer, "");

  BOOST_CHECK_EQUAL(nTest.fromString("8#0"), 3);
  BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(nTest), 0);
  BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 12), 1);
  BOOST_CHECK_EQUAL(strcmp(cBuffer, "0"), 0);
  BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 0), -1);
  strcpy(cBuffer, "");

  BOOST_CHECK_EQUAL(nTest.fromString("16#0"), 4);
  BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(nTest), 0);
  BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 12), 1);
  BOOST_CHECK_EQUAL(strcmp(cBuffer, "0"), 0);
  BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 0), -1);
  strcpy(cBuffer, "");

  BOOST_CHECK_EQUAL(nTest.fromString("368745"), 6);
  BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(nTest), 368745);
  BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 12), 6);
  BOOST_CHECK_EQUAL(strcmp(cBuffer, "368745"), 0);
  BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
  strcpy(cBuffer, "");

  nTest = CIEC_DINT(0);
  BOOST_CHECK_EQUAL(nTest.fromString("2#1011010000001101001"), 21);
  BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(nTest), 368745);
  BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 12), 6);
  BOOST_CHECK_EQUAL(strcmp(cBuffer, "368745"), 0);

  BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
  strcpy(cBuffer, "");

  nTest = CIEC_DINT(0);
  BOOST_CHECK_EQUAL(nTest.fromString("8#1320151"), 9);
  BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(nTest), 368745);
  BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 12), 6);
  BOOST_CHECK_EQUAL(strcmp(cBuffer, "368745"), 0);
  BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
  strcpy(cBuffer, "");
  
  nTest = CIEC_DINT(0);
  BOOST_CHECK_EQUAL(nTest.fromString("16#5a069"), 8);
  BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(nTest), 368745);
  BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 12), 6);
  BOOST_CHECK_EQUAL(strcmp(cBuffer, "368745"), 0);
  BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
  strcpy(cBuffer, "");
  nTest = CIEC_DINT(0);

  BOOST_CHECK_EQUAL(nTest.fromString("2147483647"), 10);
  BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(nTest), 2147483647);
  BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 12), 10);
  BOOST_CHECK_EQUAL(strcmp(cBuffer, "2147483647"), 0);
  BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
  strcpy(cBuffer, "");
  nTest = CIEC_DINT(0);

  BOOST_CHECK_EQUAL(nTest.fromString("2#01111111111111111111111111111111"), 34);
  BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(nTest), 2147483647);
  BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 12), 10);
  BOOST_CHECK_EQUAL(strcmp(cBuffer, "2147483647"), 0);
  BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
  strcpy(cBuffer, "");
  nTest = CIEC_DINT(0);

  BOOST_CHECK_EQUAL(nTest.fromString("8#17777777777"), 13);
  BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(nTest), 2147483647);
  BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 12), 10);
  BOOST_CHECK_EQUAL(strcmp(cBuffer, "2147483647"), 0);
  BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
  strcpy(cBuffer, "");
  nTest = CIEC_DINT(0);

  BOOST_CHECK_EQUAL(nTest.fromString("16#7FFFFfFF"), 11);
  BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(nTest), 2147483647);
  BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 12), 10);
  BOOST_CHECK_EQUAL(strcmp(cBuffer, "2147483647"), 0);
  BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
  strcpy(cBuffer, "");
  nTest = CIEC_DINT(0);

  BOOST_CHECK_EQUAL(nTest.fromString("10#300"), 6);
  BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(nTest), 300);
  BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 12), 3);
  BOOST_CHECK_EQUAL(strcmp(cBuffer, "300"), 0);

  //testing values outside of allowed range
  BOOST_CHECK_EQUAL(nTest.fromString("21474836473"), -1);
  BOOST_CHECK_EQUAL(nTest.fromString("2#1011111111111111111111111111111110"), -1);
  BOOST_CHECK_EQUAL(nTest.fromString("8#177777777770"), -1);
  BOOST_CHECK_EQUAL(nTest.fromString("16#7FFFFFFF0"), -1);
  BOOST_CHECK_EQUAL(nTest.fromString("-21474836473"), -1);

  //check invalid fromString string
  BOOST_CHECK_EQUAL(nTest.fromString("NOT A VALID STRING"), -1);
}
BOOST_AUTO_TEST_SUITE_END()
