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

BOOST_AUTO_TEST_CASE(Assignment_test)
{
  CIEC_REAL nTest1;
  CIEC_REAL nTest2;

  //initial value must be 0
  BOOST_CHECK_EQUAL(nTest1, 0.0f);

  nTest1 = std::numeric_limits<TForteFloat>::min();
  nTest2 = nTest1;
  BOOST_CHECK_EQUAL(nTest1, std::numeric_limits<TForteFloat>::min());
  BOOST_CHECK_EQUAL(nTest2, std::numeric_limits<TForteFloat>::min());

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

  nTest1 = std::numeric_limits<TForteFloat>::max();
  nTest2 = nTest1;
  BOOST_CHECK_EQUAL(nTest1, std::numeric_limits<TForteFloat>::max());
  BOOST_CHECK_EQUAL(nTest2, std::numeric_limits<TForteFloat>::max());

  //check that assignment operator does not intertwine objects
  nTest2 = -36.0;
  BOOST_CHECK_EQUAL(nTest1, std::numeric_limits<TForteFloat>::max());
  BOOST_CHECK_EQUAL(nTest2, -36.0);

}

BOOST_AUTO_TEST_CASE(Conversion_test)
{
  CIEC_REAL nTest;

  char cBuffer[12];
  char cBufferFail[2];

  //check cast operator
  nTest = 0.0f;
  BOOST_CHECK_EQUAL(nTest.operator TForteFloat(), 0.0f);

  nTest = std::numeric_limits<TForteFloat>::min();
  BOOST_CHECK_EQUAL(nTest.operator TForteFloat(), std::numeric_limits<TForteFloat>::min());

  nTest = -4.2345e4f;
  BOOST_CHECK_EQUAL(nTest.operator TForteFloat(), -4.2345e4f);

  nTest = 23.7813e2f;
  BOOST_CHECK_EQUAL(nTest.operator TForteFloat(), 23.7813e2f);

  nTest = std::numeric_limits<TForteFloat>::max();
  BOOST_CHECK_EQUAL(nTest.operator TForteFloat(), std::numeric_limits<TForteFloat>::max());

  //check toString and fromString
  strcpy(cBuffer, "");

  BOOST_CHECK_EQUAL(nTest.fromString("-1E-37"), 6);
  BOOST_CHECK_EQUAL(static_cast<TForteFloat>(nTest), -1.0E-37f);

    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 7), 6);
  BOOST_CHECK_EQUAL(strcmp(cBuffer, "-1e-37"), 0);

  BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
  strcpy(cBuffer, "");
  nTest = 0;

  BOOST_CHECK_EQUAL(nTest.fromString("0"), 1);
  BOOST_CHECK_EQUAL(static_cast<TForteFloat>(nTest), 0.0f);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 7), 1);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "0"), 0);
    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 0), -1);
    strcpy(cBuffer, "");
    nTest = 0;


    BOOST_CHECK_EQUAL(nTest.fromString("3.2523E15"), 9);
    BOOST_CHECK_EQUAL(static_cast<TForteFloat>(nTest), 3.2523e15f);

    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 11), 10);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "3.2523e+15"), 0);

    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
    strcpy(cBuffer, "");
    nTest = 0;

    BOOST_CHECK_EQUAL(nTest.fromString("1E37"), 4);
    BOOST_CHECK_EQUAL(static_cast<TForteFloat>(nTest), 1e37f);

    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 7), 5);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "1e+37"), 0);

    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
    strcpy(cBuffer, "");
    nTest = 0;

    //testing values outside of allowed range
    BOOST_CHECK_EQUAL(nTest.fromString("4e40"), -1);
    BOOST_CHECK_EQUAL(nTest.fromString("2#100101100"), 1);
    BOOST_CHECK_EQUAL(nTest.fromString("8#116100"), 1);
    BOOST_CHECK_EQUAL(nTest.fromString("10#300"), 2);
    BOOST_CHECK_EQUAL(nTest.fromString("16#FFFF0"), 2);
    BOOST_CHECK_EQUAL(nTest.fromString("-4e-40"), -1);

  //check invalid fromString string
  BOOST_CHECK_EQUAL(nTest.fromString("NOT A VALID STRING"), -1);
}

void realTypedFromString(const char *pa_acTestString, float pa_fResult){
  CIEC_REAL nTest;
  BOOST_CHECK_EQUAL(nTest.fromString(pa_acTestString), strlen(pa_acTestString));
  BOOST_CHECK_EQUAL(pa_fResult, nTest);
}

void faultingRealTypedFromString(const char *pa_acTestString){
  CIEC_REAL nTest;
  BOOST_CHECK_EQUAL(nTest.fromString(pa_acTestString), -1);
}


BOOST_AUTO_TEST_CASE(REAL_typed_fromString_tests){
  realTypedFromString("REAL#1E37", 1e37f);
  realTypedFromString("REAL#-4.2345e4",-4.2345e4f);
  realTypedFromString("REAL#0",0.0f);

  faultingRealTypedFromString("REAL#4e40");
  faultingRealTypedFromString("REAL#NOT A VALID STRING");
}


BOOST_AUTO_TEST_SUITE_END()
