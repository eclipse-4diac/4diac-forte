/*******************************************************************************
 * Copyright (c) 2011 - 2013 ACIN, nxtControl, fortiss GmbH
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
#include <boost/test/floating_point_comparison.hpp>

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

BOOST_AUTO_TEST_CASE(Assignment_test)
{
  CIEC_LREAL nTest1;
  CIEC_LREAL nTest2;

  //initial value must be 0
  BOOST_CHECK_EQUAL(nTest1, 0.0f);

  nTest1 = std::numeric_limits<TForteDFloat>::min();
  nTest2 = nTest1;
  BOOST_CHECK_EQUAL(nTest1, std::numeric_limits<TForteDFloat>::min());
  BOOST_CHECK_EQUAL(nTest2, std::numeric_limits<TForteDFloat>::min());

  nTest1 = 2.28743e6;
  nTest2 = nTest1;
  BOOST_CHECK_EQUAL(nTest1, 2.28743e6);
  BOOST_CHECK_EQUAL(nTest2, 2.28743e6);

  nTest1 = -6.2587e-4f;
  nTest2 = nTest1;
  BOOST_CHECK_EQUAL(nTest1, -6.2587e-4f);
  BOOST_CHECK_EQUAL(nTest2, -6.2587e-4f);

  nTest1 = 1.0E-37f;
  nTest2 = nTest1;
  BOOST_CHECK_EQUAL(nTest1, 1.0E-37f);
  BOOST_CHECK_EQUAL(nTest2, 1.0E-37f);

  nTest1 = std::numeric_limits<TForteDFloat>::max();
  nTest2 = nTest1;
  BOOST_CHECK_EQUAL(nTest1, std::numeric_limits<TForteDFloat>::max());
  BOOST_CHECK_EQUAL(nTest2, std::numeric_limits<TForteDFloat>::max());

  //check that assignment operator does not intertwine objects
  nTest2 = -36.0;
  BOOST_CHECK_EQUAL(nTest1, std::numeric_limits<TForteDFloat>::max());
  BOOST_CHECK_EQUAL(nTest2, -36.0);

}

BOOST_AUTO_TEST_CASE(Conversion_test)
{
  CIEC_LREAL nTest;

  char cBuffer[12];
  char cBufferFail[2];

  //check cast operator
  nTest = 0.0;
  BOOST_CHECK_EQUAL(nTest.operator TForteDFloat(), 0.0);

  nTest = std::numeric_limits<TForteDFloat>::min();
  BOOST_CHECK_EQUAL(nTest.operator TForteDFloat(), std::numeric_limits<TForteDFloat>::min());

  nTest = -4.2345e4;
  BOOST_CHECK_EQUAL(nTest.operator TForteDFloat(), -4.2345e4);

  nTest = 23.7813e2;
  BOOST_CHECK_EQUAL(nTest.operator TForteDFloat(), 23.7813e2);

  nTest = std::numeric_limits<TForteDFloat>::max();
  BOOST_CHECK_EQUAL(nTest.operator TForteDFloat(), std::numeric_limits<TForteDFloat>::max());

  //check toString and fromString
  strcpy(cBuffer, "");

  BOOST_CHECK_EQUAL(nTest.fromString("-1E-37"), 6);
  BOOST_CHECK_EQUAL(static_cast<TForteDFloat>(nTest), -1.0E-37);

  BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 10), 6);
  BOOST_CHECK_EQUAL(strcmp(cBuffer, "-1e-37"), 0);

  BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
  strcpy(cBuffer, "");
  nTest = 0;

  BOOST_CHECK_EQUAL(nTest.fromString("0"), 1);
  BOOST_CHECK_EQUAL(static_cast<TForteDFloat>(nTest), 0.0);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 11), 1);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "0"), 0);
    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 0), -1);
    strcpy(cBuffer, "");
    nTest = 0;


    BOOST_CHECK_EQUAL(nTest.fromString("3.2523E15"), 9);
    BOOST_CHECK_EQUAL(static_cast<TForteDFloat>(nTest), 3.2523e15);

    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 11), 10);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "3.2523e+15"), 0);

    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
    strcpy(cBuffer, "");
    nTest = 0;

    BOOST_CHECK_EQUAL(nTest.fromString("1E37"), 4);
    BOOST_CHECK_EQUAL(static_cast<TForteDFloat>(nTest), 1e37);

    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 11), 5);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "1e+37"), 0);

    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
    strcpy(cBuffer, "");
    nTest = 0;

    //testing values outside of allowed range
    BOOST_CHECK_EQUAL(nTest.fromString("4e401"), -1);
    BOOST_CHECK_EQUAL(nTest.fromString("2#100101100"), 1);
    BOOST_CHECK_EQUAL(nTest.fromString("8#116100"), 1);
    BOOST_CHECK_EQUAL(nTest.fromString("10#300"), 2);
    BOOST_CHECK_EQUAL(nTest.fromString("16#FFFF0"), 2);
    BOOST_CHECK_EQUAL(nTest.fromString("-4e-401"), -1);

  //check invalid fromString string
  BOOST_CHECK_EQUAL(nTest.fromString("NOT A VALID STRING"), -1);
}

void lRealTypedFromString(const char *pa_acTestString, double pa_fResult){
  CIEC_LREAL nTest;
  BOOST_CHECK_EQUAL(nTest.fromString(pa_acTestString), strlen(pa_acTestString));
  BOOST_CHECK_EQUAL(pa_fResult, nTest);
}

void faultingLRealTypedFromString(const char *pa_acTestString){
  CIEC_LREAL nTest;
  BOOST_CHECK_EQUAL(nTest.fromString(pa_acTestString), -1);
}


BOOST_AUTO_TEST_CASE(REAL_typed_fromString_tests){
  lRealTypedFromString("LREAL#1E37", 1e37);
  lRealTypedFromString("LREAL#-4.2345e4",-4.2345e4);
  lRealTypedFromString("LREAL#0",0.0);
  lRealTypedFromString("LREAL#3.2523E15",3.2523E15);

  faultingLRealTypedFromString("LREAL#4e401");
  faultingLRealTypedFromString("LREAL#-4e-401");
  faultingLRealTypedFromString("LREAL#NOT A VALID STRING");
}

BOOST_AUTO_TEST_SUITE_END()
