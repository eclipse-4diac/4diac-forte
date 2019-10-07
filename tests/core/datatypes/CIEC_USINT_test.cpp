/*******************************************************************************
 * Copyright (c) 2011 ACIN
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

#include "../../../src/core/datatypes/forte_usint.h"

BOOST_AUTO_TEST_SUITE(CIEC_USINT_function_test)

BOOST_AUTO_TEST_CASE(Type_test)
{
  CIEC_USINT nTest;
  //check type information
  BOOST_CHECK_EQUAL(nTest.getDataTypeID(), CIEC_ANY::e_USINT);
  //check operator bool data type size
  BOOST_CHECK_EQUAL(sizeof(nTest.operator TForteUInt8()), sizeof(TForteUInt8));

}

BOOST_AUTO_TEST_CASE(Assignment_test)
{
  CIEC_USINT nTest1;
  CIEC_USINT nTest2;

  //initial value must be 0
  BOOST_CHECK_EQUAL(nTest1, 0U);

  nTest1 = 0U;
  nTest2 = nTest1;
  BOOST_CHECK_EQUAL(nTest1, 0U);
  BOOST_CHECK_EQUAL(nTest2, 0U);

  nTest1 = 12U;
  nTest2 = nTest1;
  BOOST_CHECK_EQUAL(nTest1, 12U);
  BOOST_CHECK_EQUAL(nTest2, 12U);

  nTest1 = 255U;
  nTest2 = nTest1;
  BOOST_CHECK_EQUAL(nTest1, 255U);
  BOOST_CHECK_EQUAL(nTest2, 255U);

  //check that assignment operator does not intertwine objects
  nTest2 = 128U;
  BOOST_CHECK_EQUAL(nTest1, 255U);
  BOOST_CHECK_EQUAL(nTest2, 128U);

}

BOOST_AUTO_TEST_CASE(Conversion_test)
{
  CIEC_USINT nTest;

  char cBuffer[4];
  char cBufferFail[2];

  //check cast operator
  nTest = 0;

  BOOST_CHECK_EQUAL(nTest.operator TForteUInt8(), 0U);

  nTest = 64U;
  BOOST_CHECK_EQUAL(nTest.operator TForteUInt8(), 64U);

  nTest = 255U;
  BOOST_CHECK_EQUAL(nTest.operator TForteUInt8(), 255U);

  //check toString and fromString
  BOOST_CHECK_EQUAL(nTest.fromString("0"), 1);
    BOOST_CHECK_EQUAL(nTest, 0U);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 4), 1);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "0"), 0);
    strcpy(cBuffer, "");

    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 0), -1);

    BOOST_CHECK_EQUAL(nTest.fromString("2#0"), 3);
    BOOST_CHECK_EQUAL(nTest, 0U);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 4), 1);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "0"), 0);
    strcpy(cBuffer, "");

    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 0), -1);

    BOOST_CHECK_EQUAL(nTest.fromString("8#0"), 3);
    BOOST_CHECK_EQUAL(nTest, 0U);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 4), 1);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "0"), 0);
    strcpy(cBuffer, "");

    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 0), -1);

    BOOST_CHECK_EQUAL(nTest.fromString("16#0"), 4);
    BOOST_CHECK_EQUAL(nTest, 0U);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 4), 1);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "0"), 0);

    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 0), -1);

    strcpy(cBuffer, "");

    BOOST_CHECK_EQUAL(nTest.fromString("79"), 2);
    BOOST_CHECK_EQUAL(nTest, 79U);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 4), 2);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "79"), 0);

    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);

    strcpy(cBuffer, "");
    nTest = 0;

    BOOST_CHECK_EQUAL(nTest.fromString("2#1001111"), 9);
    BOOST_CHECK_EQUAL(nTest, 79U);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 4), 2);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "79"), 0);

    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);

    strcpy(cBuffer, "");
    nTest = 0;

    BOOST_CHECK_EQUAL(nTest.fromString("8#117"), 5);
    BOOST_CHECK_EQUAL(nTest, 79U);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 4), 2);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "79"), 0);

    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);

    strcpy(cBuffer, "");
    nTest = 0;

    BOOST_CHECK_EQUAL(nTest.fromString("16#4F"), 5);
    BOOST_CHECK_EQUAL(nTest, 79U);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 4), 2);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "79"), 0);

    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);

    strcpy(cBuffer, "");
    nTest = 0;

    BOOST_CHECK_EQUAL(nTest.fromString("255"), 3);
    BOOST_CHECK_EQUAL(nTest, 255U);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 4), 3);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "255"), 0);

    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);

    strcpy(cBuffer, "");
    nTest = 0;

    BOOST_CHECK_EQUAL(nTest.fromString("2#11111111"), 10);
    BOOST_CHECK_EQUAL(nTest, 255U);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 4), 3);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "255"), 0);

    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);

    strcpy(cBuffer, "");
    nTest = 0;

    BOOST_CHECK_EQUAL(nTest.fromString("8#377"), 5);
    BOOST_CHECK_EQUAL(nTest, 255U);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 4), 3);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "255"), 0);

    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);

    strcpy(cBuffer, "");
    nTest = 0;

    BOOST_CHECK_EQUAL(nTest.fromString("16#FF"), 5);
    BOOST_CHECK_EQUAL(nTest, 255U);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 4), 3);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "255"), 0);

    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);

    strcpy(cBuffer, "");
    nTest = 0;

    //testing values outside of allowed range
    BOOST_CHECK_EQUAL(nTest.fromString("300"), -1);
    BOOST_CHECK_EQUAL(nTest.fromString("2#100101100"), -1);
    BOOST_CHECK_EQUAL(nTest.fromString("8#454"), -1);
    BOOST_CHECK_EQUAL(nTest.fromString("10#300"), -1);
    BOOST_CHECK_EQUAL(nTest.fromString("16#12C"), -1);
    BOOST_CHECK_EQUAL(nTest.fromString("-130"), -1);

  //check invalid fromString string
  BOOST_CHECK_EQUAL(nTest.fromString("NOT A VALID STRING"), -1);
}
BOOST_AUTO_TEST_SUITE_END()
