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

#include "../../../src/core/datatypes/forte_byte.h"

BOOST_AUTO_TEST_SUITE(CIEC_BYTE_function_test)

BOOST_AUTO_TEST_CASE(Type_test)
{
  CIEC_BYTE nTest;
  //check type information
  BOOST_CHECK_EQUAL(nTest.getDataTypeID(), CIEC_ANY::e_BYTE);
  //check operator bool data type size
  BOOST_CHECK_EQUAL(sizeof(nTest.operator TForteByte()), sizeof(TForteByte));

}

BOOST_AUTO_TEST_CASE(Assignment_test)
{
  CIEC_BYTE nTest1;
  CIEC_BYTE nTest2;

  //initial value must be 0
  BOOST_CHECK_EQUAL(nTest1, 0);

  nTest1 = 0;
  nTest2 = nTest1;
  BOOST_CHECK_EQUAL(nTest1, 0);
  BOOST_CHECK_EQUAL(nTest2, 0);

  nTest1 = 12;
  nTest2 = nTest1;
  BOOST_CHECK_EQUAL(nTest1, 12);
  BOOST_CHECK_EQUAL(nTest2, 12);

  nTest1 = 255;
  nTest2 = nTest1;
  BOOST_CHECK_EQUAL(nTest1, 255);
  BOOST_CHECK_EQUAL(nTest2, 255);

  //check that assignment operator does not intertwine objects
  nTest2 = 128;
  BOOST_CHECK_EQUAL(nTest1, 255);
  BOOST_CHECK_EQUAL(nTest2, 128);

}

BOOST_AUTO_TEST_CASE(Conversion_test)
{
  CIEC_BYTE nTest;

  char cBuffer[4];
  char cBufferFail[2];

  //check cast operator
  nTest = 0;
  strcpy(cBuffer, "");

  BOOST_CHECK_EQUAL(nTest.operator TForteByte(), 0);

  nTest = 64;
  BOOST_CHECK_EQUAL(nTest.operator TForteByte(), 64);

  nTest = 255;
  BOOST_CHECK_EQUAL(nTest.operator TForteByte(), 255);

  //check toString and fromString
  BOOST_CHECK_EQUAL(nTest.fromString("0"), 1);
    BOOST_CHECK_EQUAL(nTest, 0);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 4), 1);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "0"), 0);

    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 1), -1);

    strcpy(cBuffer, "");

    BOOST_CHECK_EQUAL(nTest.fromString("2#0"), 3);
    BOOST_CHECK_EQUAL(nTest, 0);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 4), 1);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "0"), 0);

    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 1), -1);

    strcpy(cBuffer, "");

    BOOST_CHECK_EQUAL(nTest.fromString("8#0"), 3);
    BOOST_CHECK_EQUAL(nTest, 0);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 4), 1);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "0"), 0);

    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 1), -1);

    strcpy(cBuffer, "");

    BOOST_CHECK_EQUAL(nTest.fromString("16#0"), 4);
    BOOST_CHECK_EQUAL(nTest, 0);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 4), 1);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "0"), 0);

    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 1), -1);

    strcpy(cBuffer, "");

    BOOST_CHECK_EQUAL(nTest.fromString("79"), 2);
    BOOST_CHECK_EQUAL(nTest, 79);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 4), 2);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "79"), 0);

    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);

    strcpy(cBuffer, "");
    nTest = 0;

    BOOST_CHECK_EQUAL(nTest.fromString("2#1001111"), 9);
    BOOST_CHECK_EQUAL(nTest, 79);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 4), 2);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "79"), 0);

    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);

    strcpy(cBuffer, "");
    nTest = 0;

    BOOST_CHECK_EQUAL(nTest.fromString("8#117"), 5);
    BOOST_CHECK_EQUAL(nTest, 79);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 4), 2);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "79"), 0);

    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);

    strcpy(cBuffer, "");
    nTest = 0;

    BOOST_CHECK_EQUAL(nTest.fromString("16#4F"), 5);
    BOOST_CHECK_EQUAL(nTest, 79);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 4), 2);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "79"), 0);

    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);

    strcpy(cBuffer, "");
    nTest = 0;

    BOOST_CHECK_EQUAL(nTest.fromString("255"), 3);
    BOOST_CHECK_EQUAL(nTest, 255);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 4), 3);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "255"), 0);

    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);

    strcpy(cBuffer, "");
    nTest = 0;

    BOOST_CHECK_EQUAL(nTest.fromString("2#11111111"), 10);
    BOOST_CHECK_EQUAL(nTest, 255);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 4), 3);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "255"), 0);

    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);

    strcpy(cBuffer, "");
    nTest = 0;

    BOOST_CHECK_EQUAL(nTest.fromString("8#377"), 5);
    BOOST_CHECK_EQUAL(nTest, 255);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 4), 3);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "255"), 0);

    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);

    strcpy(cBuffer, "");
    nTest = 0;

    BOOST_CHECK_EQUAL(nTest.fromString("16#FF"), 5);
    BOOST_CHECK_EQUAL(nTest, 255);
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
