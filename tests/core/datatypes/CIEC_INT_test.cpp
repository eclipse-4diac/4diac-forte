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

#include "../../../src/core/datatypes/forte_int.h"

BOOST_AUTO_TEST_SUITE(CIEC_INT_function_test)
BOOST_AUTO_TEST_CASE(Type_test)
{
  CIEC_INT nTest;
  //check type information
  BOOST_CHECK_EQUAL(nTest.getDataTypeID(), CIEC_ANY::e_INT);
  //check operator bool data type size
  BOOST_CHECK_EQUAL(sizeof(nTest.operator TForteInt16()), sizeof(TForteInt16));

}

BOOST_AUTO_TEST_CASE(Assignment_test)
{
  CIEC_INT nTest1;
  CIEC_INT nTest2;

  //initial value must be 0
  BOOST_CHECK_EQUAL(nTest1, 0);

  nTest1 = -32768;
  nTest2 = nTest1;
  BOOST_CHECK_EQUAL(nTest1, -32768);
  BOOST_CHECK_EQUAL(nTest2, -32768);

  nTest1 = -25789;
  nTest2 = nTest1;
  BOOST_CHECK_EQUAL(nTest1, -25789);
  BOOST_CHECK_EQUAL(nTest2, -25789);

  nTest1 = 15748;
  nTest2 = nTest1;
  BOOST_CHECK_EQUAL(nTest1, 15748);
  BOOST_CHECK_EQUAL(nTest2, 15748);

  nTest1 = 32767;
  nTest2 = nTest1;
  BOOST_CHECK_EQUAL(nTest1, 32767);
  BOOST_CHECK_EQUAL(nTest2, 32767);

  //check that assignment operator does not intertwine objects
  nTest2 = -36;
  BOOST_CHECK_EQUAL(nTest1, 32767);
  BOOST_CHECK_EQUAL(nTest2, -36);

}

BOOST_AUTO_TEST_CASE(Conversion_test)
{
  CIEC_INT nTest;

  char cBuffer[7];
  char cBufferFail[2];

  //check cast operator
  nTest = 0;

  BOOST_CHECK_EQUAL(nTest.operator TForteInt16(), 0);

  nTest = -32768;
  BOOST_CHECK_EQUAL(nTest.operator TForteInt16(), -32768);

  nTest = 6984;
  BOOST_CHECK_EQUAL(nTest.operator TForteInt16(), 6984);

  nTest = 32767;
  BOOST_CHECK_EQUAL(nTest.operator TForteInt16(), 32767);

  //check toString and fromString
  strcpy(cBuffer, "");

  BOOST_CHECK_EQUAL(nTest.fromString("-32768"), 6);
  BOOST_CHECK_EQUAL(nTest, -32768);
  BOOST_CHECK_EQUAL(nTest.fromString("INT#-32768"), 10);
    BOOST_CHECK_EQUAL(nTest, -32768);
  BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 7), 6);
  BOOST_CHECK_EQUAL(strcmp(cBuffer, "-32768"), 0);
  BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
  strcpy(cBuffer, "");
  nTest = 0;

    BOOST_CHECK_EQUAL(nTest.fromString("2#1000000000000000"), 18);
    BOOST_CHECK_EQUAL(nTest, -32768);
    BOOST_CHECK_EQUAL(nTest.fromString("INT#2#1000000000000000"), 22);
        BOOST_CHECK_EQUAL(nTest, -32768);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 7), 6);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "-32768"), 0);

    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);

    strcpy(cBuffer, "");
    nTest = 0;

    BOOST_CHECK_EQUAL(nTest.fromString("8#100000"), 8);
    BOOST_CHECK_EQUAL(nTest, -32768);
    BOOST_CHECK_EQUAL(nTest.fromString("INT#8#100000"), 12);
    BOOST_CHECK_EQUAL(nTest, -32768);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 7), 6);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "-32768"), 0);

    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);

    strcpy(cBuffer, "");
    nTest = 0;

    BOOST_CHECK_EQUAL(nTest.fromString("16#8000"), 7);
    BOOST_CHECK_EQUAL(nTest, -32768);
    BOOST_CHECK_EQUAL(nTest.fromString("INT#16#8000"), 11);
        BOOST_CHECK_EQUAL(nTest, -32768);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 7), 6);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "-32768"), 0);

    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);

    BOOST_CHECK_EQUAL(nTest.fromString("16#ffff"), 7);
    BOOST_CHECK_EQUAL(nTest, -1);

    strcpy(cBuffer, "");

  BOOST_CHECK_EQUAL(nTest.fromString("0"), 1);
    BOOST_CHECK_EQUAL(nTest, 0);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 7), 1);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "0"), 0);

    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 0), -1);

    strcpy(cBuffer, "");

    BOOST_CHECK_EQUAL(nTest.fromString("2#0"), 3);
    BOOST_CHECK_EQUAL(nTest, 0);
    BOOST_CHECK_EQUAL(nTest.fromString("INT#2#0"), 7);
        BOOST_CHECK_EQUAL(nTest, 0);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 7), 1);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "0"), 0);

    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 0), -1);

    strcpy(cBuffer, "");

    BOOST_CHECK_EQUAL(nTest.fromString("8#0"), 3);
    BOOST_CHECK_EQUAL(nTest, 0);
    BOOST_CHECK_EQUAL(nTest.fromString("INT#8#0"), 7);
    BOOST_CHECK_EQUAL(nTest, 0);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 7), 1);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "0"), 0);

    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 0), -1);

    strcpy(cBuffer, "");

    BOOST_CHECK_EQUAL(nTest.fromString("16#0"), 4);
    BOOST_CHECK_EQUAL(nTest, 0);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 7), 1);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "0"), 0);

    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 0), -1);

    strcpy(cBuffer, "");


    BOOST_CHECK_EQUAL(nTest.fromString("99"), 2);
    BOOST_CHECK_EQUAL(nTest, 99);
    BOOST_CHECK_EQUAL(nTest.fromString("INT#99"), 6);
    BOOST_CHECK_EQUAL(nTest, 99);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 7), 2);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "99"), 0);

    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);

    strcpy(cBuffer, "");
    nTest = 0;

    BOOST_CHECK_EQUAL(nTest.fromString("2#1100011"), 9);
    BOOST_CHECK_EQUAL(nTest, 99);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 7), 2);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "99"), 0);

    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);

    strcpy(cBuffer, "");
    nTest = 0;


    BOOST_CHECK_EQUAL(nTest.fromString("8#143"), 5);
    BOOST_CHECK_EQUAL(nTest, 99);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 7), 2);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "99"), 0);

    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);

    strcpy(cBuffer, "");
    nTest = 0;

    BOOST_CHECK_EQUAL(nTest.fromString("16#63"), 5);
    BOOST_CHECK_EQUAL(nTest, 99);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 7), 2);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "99"), 0);

    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);

    strcpy(cBuffer, "");
    nTest = 0;


    BOOST_CHECK_EQUAL(nTest.fromString("32767"), 5);
    BOOST_CHECK_EQUAL(nTest, 32767);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 7), 5);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "32767"), 0);

    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);

    strcpy(cBuffer, "");
    nTest = 0;

    BOOST_CHECK_EQUAL(nTest.fromString("2#0111111111111111"), 18);
    BOOST_CHECK_EQUAL(nTest, 32767);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 7), 5);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "32767"), 0);

    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);

    strcpy(cBuffer, "");
    nTest = 0;

    BOOST_CHECK_EQUAL(nTest.fromString("8#77777"), 7);
    BOOST_CHECK_EQUAL(nTest, 32767);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 7), 5);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "32767"), 0);

    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);

    strcpy(cBuffer, "");
    nTest = 0;

    BOOST_CHECK_EQUAL(nTest.fromString("16#7FFF"), 7);
    BOOST_CHECK_EQUAL(nTest, 32767);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 7), 5);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "32767"), 0);

    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);

    strcpy(cBuffer, "");
    nTest = 0;

    BOOST_CHECK_EQUAL(nTest.fromString("10#300"), 6);
    BOOST_CHECK_EQUAL(nTest, 300);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 7), 3);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "300"), 0);

    //testing values outside of allowed range
    BOOST_CHECK_EQUAL(nTest.fromString("40000"), -1);
    BOOST_CHECK_EQUAL(nTest.fromString("2#100101100100101101"), -1);
    BOOST_CHECK_EQUAL(nTest.fromString("8#1161007"), -1);
    BOOST_CHECK_EQUAL(nTest.fromString("16#FFFF0"), -1);
    BOOST_CHECK_EQUAL(nTest.fromString("-40000"), -1);

  //check invalid fromString string
  BOOST_CHECK_EQUAL(nTest.fromString("NOT A VALID STRING"), -1);
}
BOOST_AUTO_TEST_SUITE_END()
