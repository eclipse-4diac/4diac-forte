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

#include "../../../src/core/datatypes/forte_sint.h"

BOOST_AUTO_TEST_SUITE(CIEC_SINT_function_test)
BOOST_AUTO_TEST_CASE(Type_test)
{
  CIEC_SINT nTest;
  //check type information
  BOOST_CHECK_EQUAL(nTest.getDataTypeID(), CIEC_ANY::e_SINT);
  //check operator bool data type size
  BOOST_CHECK_EQUAL(sizeof(nTest.operator TForteInt8()), sizeof(TForteInt8));

}

BOOST_AUTO_TEST_CASE(Assignment_test)
{
  CIEC_SINT nTest1;
  CIEC_SINT nTest2;

  //initial value must be 0
  BOOST_CHECK_EQUAL(nTest1, 0);

  nTest1 = -128;
  nTest2 = nTest1;
  BOOST_CHECK_EQUAL(nTest1, -128);
  BOOST_CHECK_EQUAL(nTest2, -128);

  nTest1 = -65;
  nTest2 = nTest1;
  BOOST_CHECK_EQUAL(nTest1, -65);
  BOOST_CHECK_EQUAL(nTest2, -65);

  nTest1 = 98;
  nTest2 = nTest1;
  BOOST_CHECK_EQUAL(nTest1, 98);
  BOOST_CHECK_EQUAL(nTest2, 98);

  nTest1 = 127;
  nTest2 = nTest1;
  BOOST_CHECK_EQUAL(nTest1, 127);
  BOOST_CHECK_EQUAL(nTest2, 127);

  //check that assignment operator does not intertwine objects
  nTest2 = -74;
  BOOST_CHECK_EQUAL(nTest1, 127);
  BOOST_CHECK_EQUAL(nTest2, -74);

}

BOOST_AUTO_TEST_CASE(Conversion_test)
{
  CIEC_SINT nTest;

  char cBuffer[5];
  char cBufferFail[2];

  //check cast operator
  nTest = 0;

  BOOST_CHECK_EQUAL(nTest.operator TForteInt8(), 0);

  nTest = -128;
  BOOST_CHECK_EQUAL(nTest.operator TForteInt8(), -128);

  nTest = 53;
  BOOST_CHECK_EQUAL(nTest.operator TForteInt8(), 53);

  nTest = 127;
  BOOST_CHECK_EQUAL(nTest.operator TForteInt8(), 127);

  //check toString and fromString
  strcpy(cBuffer, "");

  BOOST_CHECK_EQUAL(nTest.fromString("-128"), 4);
  BOOST_CHECK_EQUAL(nTest, -128);
  BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 5), 4);
  BOOST_CHECK_EQUAL(strcmp(cBuffer, "-128"), 0);
  BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
  strcpy(cBuffer, "");
  nTest = 0;

  BOOST_CHECK_EQUAL(nTest.fromString("2#10000000"), 10);
  BOOST_CHECK_EQUAL(nTest, -128);
  BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 5), 4);
  BOOST_CHECK_EQUAL(strcmp(cBuffer, "-128"), 0);
  BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
  strcpy(cBuffer, "");
  nTest = 0;

  BOOST_CHECK_EQUAL(nTest.fromString("8#200"), 5);
  BOOST_CHECK_EQUAL(nTest, -128);
  BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 5), 4);
  BOOST_CHECK_EQUAL(strcmp(cBuffer, "-128"), 0);
  BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
  strcpy(cBuffer, "");
  nTest = 0;

  BOOST_CHECK_EQUAL(nTest.fromString("16#80"), 5);
  BOOST_CHECK_EQUAL(nTest, -128);
  BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 5), 4);
  BOOST_CHECK_EQUAL(strcmp(cBuffer, "-128"), 0);
  BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
  strcpy(cBuffer, "");
  nTest = 0;


  BOOST_CHECK_EQUAL(nTest.fromString("0"), 1);
    BOOST_CHECK_EQUAL(nTest, 0);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 5), 1);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "0"), 0);
    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 1), -1);
    strcpy(cBuffer, "");

    BOOST_CHECK_EQUAL(nTest.fromString("2#0"), 3);
    BOOST_CHECK_EQUAL(nTest, 0);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 5), 1);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "0"), 0);
    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 1), -1);
    strcpy(cBuffer, "");

    BOOST_CHECK_EQUAL(nTest.fromString("8#0"), 3);
    BOOST_CHECK_EQUAL(nTest, 0);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 5), 1);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "0"), 0);
    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 1), -1);
    strcpy(cBuffer, "");

    BOOST_CHECK_EQUAL(nTest.fromString("16#0"), 4);
    BOOST_CHECK_EQUAL(nTest, 0);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 5), 1);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "0"), 0);
    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 1), -1);
    strcpy(cBuffer, "");


    BOOST_CHECK_EQUAL(nTest.fromString("99"), 2);
    BOOST_CHECK_EQUAL(nTest, 99);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 5), 2);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "99"), 0);
    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
    strcpy(cBuffer, "");
    nTest = 0;

    BOOST_CHECK_EQUAL(nTest.fromString("2#1100011"), 9);
    BOOST_CHECK_EQUAL(nTest, 99);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 5), 2);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "99"), 0);
    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
    strcpy(cBuffer, "");
    nTest = 0;

    BOOST_CHECK_EQUAL(nTest.fromString("8#143"), 5);
    BOOST_CHECK_EQUAL(nTest, 99);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 5), 2);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "99"), 0);
    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
    strcpy(cBuffer, "");
    nTest = 0;

    BOOST_CHECK_EQUAL(nTest.fromString("16#63"), 5);
    BOOST_CHECK_EQUAL(nTest, 99);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 5), 2);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "99"), 0);
    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
    strcpy(cBuffer, "");
    nTest = 0;


    BOOST_CHECK_EQUAL(nTest.fromString("127"), 3);
    BOOST_CHECK_EQUAL(nTest, 127);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 5), 3);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "127"), 0);
    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
    strcpy(cBuffer, "");
    nTest = 0;

    BOOST_CHECK_EQUAL(nTest.fromString("2#01111111"), 10);
    BOOST_CHECK_EQUAL(nTest, 127);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 5), 3);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "127"), 0);
    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
    strcpy(cBuffer, "");
    nTest = 0;

    BOOST_CHECK_EQUAL(nTest.fromString("8#177"), 5);
    BOOST_CHECK_EQUAL(nTest, 127);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 5), 3);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "127"), 0);
    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
    strcpy(cBuffer, "");
    nTest = 0;

    BOOST_CHECK_EQUAL(nTest.fromString("16#7F"), 5);
    BOOST_CHECK_EQUAL(nTest, 127);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 5), 3);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "127"), 0);
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
