/*******************************************************************************
 * Copyright (c) 2011, 2023 ACIN, nxtControl, fortiss GmbH
 *                          Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Martin Melik Merkumians, Ingo Hegny, Alois Zoitl, Stanislav Meduna - initial API and implementation and/or initial documentation
 *   Martin Jobst - add equals tests
 *                - add user-defined literal tests
 *******************************************************************************/
#include <boost/test/unit_test.hpp>
#include <boost/test/tools/floating_point_comparison.hpp>

#include "forte_boost_output_support.h"

#include "../../../src/core/datatypes/forte_real.h"
#include <limits>


BOOST_AUTO_TEST_SUITE(CIEC_REAL_function_test)
BOOST_AUTO_TEST_CASE(Type_test)
{
  CIEC_REAL nTest;
  //check type information
  BOOST_REQUIRE_EQUAL(nTest.getDataTypeID(), CIEC_ANY::e_REAL);
  //check operator bool data type size
  BOOST_REQUIRE_EQUAL(sizeof(nTest.operator TForteFloat()), sizeof(TForteFloat));

}

BOOST_AUTO_TEST_CASE(Literal_test)
{
  CIEC_REAL test1 = 0.0_REAL;
  BOOST_TEST(static_cast<CIEC_REAL::TValueType>(test1) == 0.0f);

  CIEC_REAL test2 = 3.1415_REAL;
  BOOST_TEST(static_cast<CIEC_REAL::TValueType>(test2) == 3.1415f);

  CIEC_REAL test3 = -3.1415_REAL;
  BOOST_TEST(static_cast<CIEC_REAL::TValueType>(test3) == -3.1415f);

  CIEC_REAL test4 = 3.1415e4_REAL;
  BOOST_TEST(static_cast<CIEC_REAL::TValueType>(test4) == 3.1415e4f);

  CIEC_REAL test5 = -3.1415e4_REAL;
  BOOST_TEST(static_cast<CIEC_REAL::TValueType>(test5) == -3.1415e4f);

  CIEC_REAL test6 = 31415_REAL;
  BOOST_TEST(static_cast<CIEC_REAL::TValueType>(test6) == 3.1415e4f);

  CIEC_REAL test7 = -31415_REAL;
  BOOST_TEST(static_cast<CIEC_REAL::TValueType>(test7) == -3.1415e4f);
}

BOOST_AUTO_TEST_CASE(Assignment_test)
{
  CIEC_REAL nTest1;
  CIEC_REAL nTest2;

  //initial value must be 0
  BOOST_CHECK_EQUAL(nTest1, 0.0f);

  nTest1 = CIEC_REAL(std::numeric_limits<TForteFloat>::min());
  nTest2 = nTest1;
  BOOST_CHECK_EQUAL(nTest1, std::numeric_limits<TForteFloat>::min());
  BOOST_CHECK_EQUAL(nTest2, std::numeric_limits<TForteFloat>::min());

  nTest1 = CIEC_REAL(2.28743e6);
  nTest2 = nTest1;
  BOOST_CHECK_EQUAL(nTest1, 2.28743e6);
  BOOST_CHECK_EQUAL(nTest2, 2.28743e6);

  nTest1 = CIEC_REAL(-6.2587e-4f);
  nTest2 = nTest1;
  BOOST_CHECK_EQUAL(nTest1, -6.2587e-4f);
  BOOST_CHECK_EQUAL(nTest2, -6.2587e-4f);

  nTest1 = CIEC_REAL(1.0E-37f);
  nTest2 = nTest1;
  BOOST_CHECK_EQUAL(nTest1, 1.0E-37f);
  BOOST_CHECK_EQUAL(nTest2, 1.0E-37f);

  nTest1 = CIEC_REAL(std::numeric_limits<TForteFloat>::max());
  nTest2 = nTest1;
  BOOST_CHECK_EQUAL(nTest1, std::numeric_limits<TForteFloat>::max());
  BOOST_CHECK_EQUAL(nTest2, std::numeric_limits<TForteFloat>::max());

  //check that assignment operator does not intertwine objects
  nTest2 = CIEC_REAL(-36.0);
  BOOST_CHECK_EQUAL(nTest1, std::numeric_limits<TForteFloat>::max());
  BOOST_CHECK_EQUAL(nTest2, -36.0);

}

BOOST_AUTO_TEST_CASE(Equality_test)
{
  CIEC_REAL nTest1;
  CIEC_REAL nTest2;
  CIEC_LREAL nTest3;

  BOOST_CHECK(nTest1.equals(nTest2));
  BOOST_CHECK(!nTest1.equals(nTest3)); // different type

  nTest1 = CIEC_REAL(std::numeric_limits<TForteFloat>::min());
  BOOST_CHECK(!nTest1.equals(nTest2));
  nTest2 = nTest1;
  BOOST_CHECK(nTest1.equals(nTest2));

  nTest1 = CIEC_REAL(2.28743e6);
  BOOST_CHECK(!nTest1.equals(nTest2));
  nTest2 = nTest1;
  BOOST_CHECK(nTest1.equals(nTest2));
}

BOOST_AUTO_TEST_CASE(Conversion_test)
{
  CIEC_REAL nTest;

  char cBuffer[50];
  char cBufferFail[2];

  //check cast operator
  nTest = CIEC_REAL(0.0f);
  BOOST_CHECK_EQUAL(nTest.operator TForteFloat(), 0.0f);

  nTest = CIEC_REAL(std::numeric_limits<TForteFloat>::min());
  BOOST_CHECK_EQUAL(nTest.operator TForteFloat(), std::numeric_limits<TForteFloat>::min());

  nTest = CIEC_REAL(-4.2345e4f);
  BOOST_CHECK_EQUAL(nTest.operator TForteFloat(), -4.2345e4f);

  nTest = CIEC_REAL(23.7813e2f);
  BOOST_CHECK_EQUAL(nTest.operator TForteFloat(), 23.7813e2f);

  nTest = CIEC_REAL(std::numeric_limits<TForteFloat>::max());
  BOOST_CHECK_EQUAL(nTest.operator TForteFloat(), std::numeric_limits<TForteFloat>::max());

  //check toString and fromString
  strcpy(cBuffer, "");

  BOOST_CHECK_EQUAL(nTest.fromString("-1E-37"), 6);
  BOOST_CHECK_EQUAL(static_cast<TForteFloat>(nTest), -1.0E-37f);

  BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 50), 15);
  BOOST_TEST(cBuffer == "-9.99999991e-38");

  BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
  strcpy(cBuffer, "");
  nTest = CIEC_REAL(0);

  BOOST_CHECK_EQUAL(nTest.fromString("0"), 1);
  BOOST_CHECK_EQUAL(static_cast<TForteFloat>(nTest), 0.0f);
  BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 7), 3);
  BOOST_TEST(cBuffer ==  "0.0");
  BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 0), -1);
  strcpy(cBuffer, "");
  nTest = CIEC_REAL(0);


  BOOST_CHECK_EQUAL(nTest.fromString("3.2523E15"), 9);
  BOOST_CHECK_EQUAL(static_cast<TForteFloat>(nTest), 3.2523e15f);

  BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 50), 13);
  BOOST_TEST(cBuffer == "3.2523001e+15");

  BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
  strcpy(cBuffer, "");
  nTest = CIEC_REAL(0);

  BOOST_CHECK_EQUAL(nTest.fromString("1E37"), 4);
  BOOST_CHECK_EQUAL(static_cast<TForteFloat>(nTest), 1e37f);

  BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 52), 14);
  BOOST_TEST(cBuffer == "9.99999993e+36");

  BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
  strcpy(cBuffer, "");
  nTest = CIEC_REAL(0);

  //testing values outside of allowed range
  BOOST_CHECK_EQUAL(nTest.fromString("4e40"), -1);
  BOOST_CHECK_EQUAL(nTest.fromString("2#100101100"), 1);
  BOOST_CHECK_EQUAL(nTest.fromString("8#116100"), 1);
  BOOST_CHECK_EQUAL(nTest.fromString("10#300"), 2);
  BOOST_CHECK_EQUAL(nTest.fromString("16#FFFF0"), 2);
  BOOST_CHECK_EQUAL(nTest.fromString("-4e40"), -1);

  //check invalid fromString string
  BOOST_CHECK_EQUAL(nTest.fromString("NOT A VALID STRING"), -1);
}

BOOST_AUTO_TEST_CASE(ToString_Tests) {
  CIEC_REAL nTest(3.0f);
  char cBuffer[10];
  BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 7), 3);
  BOOST_CHECK_EQUAL(strcmp(cBuffer, "3.0"), 0);

  nTest = CIEC_REAL(10.0f);
  BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 7), 4);
  BOOST_CHECK_EQUAL(strcmp(cBuffer, "10.0"), 0);
}

void realTypedFromString(const char *paTestString, float paResult){
  CIEC_REAL nTest;
  BOOST_CHECK_EQUAL(nTest.fromString(paTestString), strlen(paTestString));
  BOOST_CHECK_EQUAL(paResult, nTest);
}

void faultingRealTypedFromString(const char *paTestString){
  CIEC_REAL nTest;
  BOOST_CHECK_EQUAL(nTest.fromString(paTestString), -1);
}


BOOST_AUTO_TEST_CASE(REAL_typed_fromString_tests){
  realTypedFromString("REAL#1E37", 1e37f);
  realTypedFromString("REAL#-4.2345e4",-4.2345e4f);
  realTypedFromString("REAL#0",0.0f);
  realTypedFromString("REAL#0.0",0.0f);

  faultingRealTypedFromString("REAL#4e40");
  faultingRealTypedFromString("REAL#NOT A VALID STRING");
}


BOOST_AUTO_TEST_SUITE_END()
