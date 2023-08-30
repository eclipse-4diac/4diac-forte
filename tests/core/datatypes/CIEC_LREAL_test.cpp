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
#include "forte_boost_output_support.h"
#include <boost/test/tools/floating_point_comparison.hpp>

#include "../../../src/core/datatypes/forte_lreal.h"


BOOST_AUTO_TEST_SUITE(CIEC_LREAL_function_test)
BOOST_AUTO_TEST_CASE(Type_test)
{
  CIEC_LREAL nTest;
  //check type information
  BOOST_REQUIRE_EQUAL(nTest.getDataTypeID(), CIEC_ANY::e_LREAL);
  //check operator bool data type size
  BOOST_REQUIRE_EQUAL(sizeof(nTest.operator TForteDFloat()), sizeof(TForteDFloat));

}

BOOST_AUTO_TEST_CASE(Literal_test)
{
  CIEC_LREAL test1 = 0.0_LREAL;
  BOOST_TEST(static_cast<CIEC_LREAL::TValueType>(test1) == 0.0);

  CIEC_LREAL test2 = 3.1415_LREAL;
  BOOST_TEST(static_cast<CIEC_LREAL::TValueType>(test2) == 3.1415);

  CIEC_LREAL test3 = -3.1415_LREAL;
  BOOST_TEST(static_cast<CIEC_LREAL::TValueType>(test3) == -3.1415);

  CIEC_LREAL test4 = 3.1415e4_LREAL;
  BOOST_TEST(static_cast<CIEC_LREAL::TValueType>(test4) == 3.1415e4);

  CIEC_LREAL test5 = -3.1415e4_LREAL;
  BOOST_TEST(static_cast<CIEC_LREAL::TValueType>(test5) == -3.1415e4);

  CIEC_LREAL test6 = 31415_LREAL;
  BOOST_TEST(static_cast<CIEC_LREAL::TValueType>(test6) == 3.1415e4f);

  CIEC_LREAL test7 = -31415_LREAL;
  BOOST_TEST(static_cast<CIEC_LREAL::TValueType>(test7) == -3.1415e4f);
}

BOOST_AUTO_TEST_CASE(Assignment_test)
{
  CIEC_LREAL nTest1;
  CIEC_LREAL nTest2;

  //initial value must be 0
  BOOST_CHECK_EQUAL(nTest1, 0.0f);

  nTest1 = CIEC_LREAL(std::numeric_limits<TForteDFloat>::min());
  nTest2 = nTest1;
  BOOST_CHECK_EQUAL(nTest1, std::numeric_limits<TForteDFloat>::min());
  BOOST_CHECK_EQUAL(nTest2, std::numeric_limits<TForteDFloat>::min());

  nTest1 = CIEC_LREAL(2.28743e6);
  nTest2 = nTest1;
  BOOST_CHECK_EQUAL(nTest1, 2.28743e6);
  BOOST_CHECK_EQUAL(nTest2, 2.28743e6);

  nTest1 = CIEC_LREAL(-6.2587e-4f);
  nTest2 = nTest1;
  BOOST_CHECK_EQUAL(nTest1, -6.2587e-4f);
  BOOST_CHECK_EQUAL(nTest2, -6.2587e-4f);

  nTest1 = CIEC_LREAL(1.0E-37f);
  nTest2 = nTest1;
  BOOST_CHECK_EQUAL(nTest1, 1.0E-37f);
  BOOST_CHECK_EQUAL(nTest2, 1.0E-37f);

  nTest1 = CIEC_LREAL(std::numeric_limits<TForteDFloat>::max());
  nTest2 = nTest1;
  BOOST_CHECK_EQUAL(nTest1, std::numeric_limits<TForteDFloat>::max());
  BOOST_CHECK_EQUAL(nTest2, std::numeric_limits<TForteDFloat>::max());

  //check that assignment operator does not intertwine objects
  nTest2 = CIEC_LREAL(-36.0);
  BOOST_CHECK_EQUAL(nTest1, std::numeric_limits<TForteDFloat>::max());
  BOOST_CHECK_EQUAL(nTest2, -36.0);

}

BOOST_AUTO_TEST_CASE(Equality_test)
{
  CIEC_LREAL nTest1;
  CIEC_LREAL nTest2;
  CIEC_REAL nTest3;

  BOOST_CHECK(nTest1.equals(nTest2));
  BOOST_CHECK(!nTest1.equals(nTest3)); // different type

  nTest1 = CIEC_LREAL(std::numeric_limits<TForteFloat>::min());
  BOOST_CHECK(!nTest1.equals(nTest2));
  nTest2 = nTest1;
  BOOST_CHECK(nTest1.equals(nTest2));

  nTest1 = CIEC_LREAL(2.28743e6);
  BOOST_CHECK(!nTest1.equals(nTest2));
  nTest2 = nTest1;
  BOOST_CHECK(nTest1.equals(nTest2));
}

BOOST_AUTO_TEST_CASE(Conversion_test)
{
  CIEC_LREAL nTest;

  char cBuffer[50];
  char cBufferFail[2];

  //check cast operator
  nTest = CIEC_LREAL(0.0);
  BOOST_CHECK_EQUAL(nTest.operator TForteDFloat(), 0.0);

  nTest = CIEC_LREAL(std::numeric_limits<TForteDFloat>::min());
  BOOST_CHECK_EQUAL(nTest.operator TForteDFloat(), std::numeric_limits<TForteDFloat>::min());

  nTest = CIEC_LREAL(-4.2345e4);
  BOOST_CHECK_EQUAL(nTest.operator TForteDFloat(), -4.2345e4);

  nTest = CIEC_LREAL(23.7813e2);
  BOOST_CHECK_EQUAL(nTest.operator TForteDFloat(), 23.7813e2);

  nTest = CIEC_LREAL(std::numeric_limits<TForteDFloat>::max());
  BOOST_CHECK_EQUAL(nTest.operator TForteDFloat(), std::numeric_limits<TForteDFloat>::max());

  //check toString and fromString
  strcpy(cBuffer, "");

  BOOST_CHECK_EQUAL(nTest.fromString("-1E-37"), 6);
  BOOST_CHECK_EQUAL(static_cast<TForteDFloat>(nTest), -1.0E-37);

  BOOST_CHECK_EQUAL(nTest.toString(cBuffer, sizeof(cBuffer)), 23);
  BOOST_TEST(cBuffer == "-1.0000000000000001e-37");

  BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
  strcpy(cBuffer, "");
  nTest = CIEC_LREAL(0);

  BOOST_CHECK_EQUAL(nTest.fromString("0"), 1);
  BOOST_CHECK_EQUAL(static_cast<TForteDFloat>(nTest), 0.0);
  BOOST_CHECK_EQUAL(nTest.toString(cBuffer, sizeof(cBuffer)), 3);
  BOOST_CHECK_EQUAL(strcmp(cBuffer, "0.0"), 0);
  BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 0), -1);
  strcpy(cBuffer, "");
  nTest = CIEC_LREAL(0);

  BOOST_CHECK_EQUAL(nTest.fromString("3.2523E15"), 9);
  BOOST_CHECK_EQUAL(static_cast<TForteDFloat>(nTest), 3.2523e15);

  BOOST_CHECK_EQUAL(nTest.toString(cBuffer, sizeof(cBuffer)), 18);
  BOOST_TEST(cBuffer == "3252300000000000.0");

  BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
  strcpy(cBuffer, "");
  nTest = CIEC_LREAL(0);

  BOOST_CHECK_EQUAL(nTest.fromString("1E37"), 4);
  BOOST_CHECK_EQUAL(static_cast<TForteDFloat>(nTest), 1e37);

  BOOST_CHECK_EQUAL(nTest.toString(cBuffer, sizeof(cBuffer)), 22);
  BOOST_TEST(cBuffer == "9.9999999999999995e+36");

  BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
  strcpy(cBuffer, "");
  nTest = CIEC_LREAL(0);

  //testing values outside of allowed range
  BOOST_CHECK_EQUAL(nTest.fromString("4e401"), -1);
  BOOST_CHECK_EQUAL(nTest.fromString("2#100101100"), 1);
  BOOST_CHECK_EQUAL(nTest.fromString("8#116100"), 1);
  BOOST_CHECK_EQUAL(nTest.fromString("10#300"), 2);
  BOOST_CHECK_EQUAL(nTest.fromString("16#FFFF0"), 2);
  BOOST_CHECK_EQUAL(nTest.fromString("-4e401"), -1);

  //check invalid fromString string
  BOOST_CHECK_EQUAL(nTest.fromString("NOT A VALID STRING"), -1);
}

BOOST_AUTO_TEST_CASE(ToString_Tests)
{
  CIEC_LREAL nTest(3.0);
  char cBuffer[10];
  BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 7), 3);
  BOOST_CHECK_EQUAL(strcmp(cBuffer, "3.0"), 0);

  nTest = CIEC_REAL(10.0);
  BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 7), 4);
  BOOST_CHECK_EQUAL(strcmp(cBuffer, "10.0"), 0);
}

void lRealTypedFromString(const char *paTestString, double paResult){
  CIEC_LREAL nTest;
  BOOST_CHECK_EQUAL(nTest.fromString(paTestString), strlen(paTestString));
  BOOST_CHECK_EQUAL(paResult, nTest);
}

void faultingLRealTypedFromString(const char *paTestString){
  CIEC_LREAL nTest;
  BOOST_CHECK_EQUAL(nTest.fromString(paTestString), -1);
}


BOOST_AUTO_TEST_CASE(REAL_typed_fromString_tests){
  lRealTypedFromString("LREAL#1E37", 1e37);
  lRealTypedFromString("LREAL#-4.2345e4",-4.2345e4);
  lRealTypedFromString("LREAL#0",0.0);
  lRealTypedFromString("LREAL#3.2523E15",3.2523E15);

  faultingLRealTypedFromString("LREAL#4e401");
  faultingLRealTypedFromString("LREAL#-4e401");
  faultingLRealTypedFromString("LREAL#NOT A VALID STRING");
}

BOOST_AUTO_TEST_SUITE_END()
