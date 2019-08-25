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

#include "../../../src/core/datatypes/forte_word.h"

BOOST_AUTO_TEST_SUITE(CIEC_WORD_function_test)

BOOST_AUTO_TEST_CASE(Type_test)
{
  CIEC_WORD nTest;
  //check type information
  BOOST_CHECK_EQUAL(nTest.getDataTypeID(), CIEC_ANY::e_WORD);
  //check operator bool data type size
  BOOST_CHECK_EQUAL(sizeof(nTest.operator TForteWord()), sizeof(TForteWord));

}

BOOST_AUTO_TEST_CASE(Assignment_test)
{
  CIEC_WORD nTest1;
  CIEC_WORD nTest2;

  //initial value must be 0
  BOOST_CHECK_EQUAL(nTest1, 0);

  nTest1 = 0;
  nTest2 = nTest1;
  BOOST_CHECK_EQUAL(nTest1, 0);
  BOOST_CHECK_EQUAL(nTest2, 0);

  nTest1 = 523;
  nTest2 = nTest1;
  BOOST_CHECK_EQUAL(nTest1, 523);
  BOOST_CHECK_EQUAL(nTest2, 523);

  nTest1 = 65535;
  nTest2 = nTest1;
  BOOST_CHECK_EQUAL(nTest1, 65535);
  BOOST_CHECK_EQUAL(nTest2, 65535);

  //check that assignment operator does not intertwine objects
  nTest2 = 40396;
  BOOST_CHECK_EQUAL(nTest1, 65535);
  BOOST_CHECK_EQUAL(nTest2, 40396);

}

BOOST_AUTO_TEST_CASE(Conversion_test)
{
  CIEC_WORD nTest;

  char cBuffer[6];
  char cBufferFail[2];

  //check cast operator
  nTest = 0;
  strcpy(cBuffer, "");

  BOOST_CHECK_EQUAL(nTest.operator TForteWord(), 0);

  nTest = 8756;
  BOOST_CHECK_EQUAL(nTest.operator TForteWord(), 8756);

  nTest = 65535;
  BOOST_CHECK_EQUAL(nTest.operator TForteWord(), 65535);

  //check toString and fromString
  BOOST_CHECK_EQUAL(nTest.fromString("0"), 1);
    BOOST_CHECK_EQUAL(nTest, 0);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 6), 1);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "0"), 0);

    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 1), -1);

    strcpy(cBuffer, "");

    BOOST_CHECK_EQUAL(nTest.fromString("2#0"), 3);
    BOOST_CHECK_EQUAL(nTest, 0);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 6), 1);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "0"), 0);

    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 1), -1);

    strcpy(cBuffer, "");

    BOOST_CHECK_EQUAL(nTest.fromString("8#0"), 3);
    BOOST_CHECK_EQUAL(nTest, 0);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 6), 1);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "0"), 0);

    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 1), -1);

    strcpy(cBuffer, "");


    BOOST_CHECK_EQUAL(nTest.fromString("16#0"), 4);
    BOOST_CHECK_EQUAL(nTest, 0);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 6), 1);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "0"), 0);

    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 1), -1);

    strcpy(cBuffer, "");

    BOOST_CHECK_EQUAL(nTest.fromString("79"), 2);
    BOOST_CHECK_EQUAL(nTest, 79);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 6), 2);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "79"), 0);

    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);

    strcpy(cBuffer, "");
    nTest = 0;

    BOOST_CHECK_EQUAL(nTest.fromString("2#1001111"), 9);
    BOOST_CHECK_EQUAL(nTest, 79);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 6), 2);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "79"), 0);

    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);

    strcpy(cBuffer, "");
    nTest = 0;

    BOOST_CHECK_EQUAL(nTest.fromString("8#117"), 5);
    BOOST_CHECK_EQUAL(nTest, 79);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 6), 2);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "79"), 0);

    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);

    strcpy(cBuffer, "");
    nTest = 0;

    BOOST_CHECK_EQUAL(nTest.fromString("16#4F"), 5);
    BOOST_CHECK_EQUAL(nTest, 79);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 6), 2);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "79"), 0);

    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);

    strcpy(cBuffer, "");
    nTest = 0;

    BOOST_CHECK_EQUAL(nTest.fromString("65535"), 5);
    BOOST_CHECK_EQUAL(nTest, 65535);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 6), 5);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "65535"), 0);

    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);

    strcpy(cBuffer, "");
    nTest = 0;

    BOOST_CHECK_EQUAL(nTest.fromString("2#1111111111111111"), 18);
    BOOST_CHECK_EQUAL(nTest, 65535);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 6), 5);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "65535"), 0);

    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);

    strcpy(cBuffer, "");
    nTest = 0;

    BOOST_CHECK_EQUAL(nTest.fromString("8#177777"), 8);
    BOOST_CHECK_EQUAL(nTest, 65535);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 6), 5);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "65535"), 0);

    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);

    strcpy(cBuffer, "");
    nTest = 0;

    BOOST_CHECK_EQUAL(nTest.fromString("16#FFFF"), 7);
    BOOST_CHECK_EQUAL(nTest, 65535);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 6), 5);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "65535"), 0);

    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);

    strcpy(cBuffer, "");
    nTest = 0;

    //testing values outside of allowed range
    BOOST_CHECK_EQUAL(nTest.fromString("70000"), -1);
    BOOST_CHECK_EQUAL(nTest.fromString("2#10001000101110000"), -1);
    BOOST_CHECK_EQUAL(nTest.fromString("8#210560"), -1);
    BOOST_CHECK_EQUAL(nTest.fromString("10#70000"), -1);
    BOOST_CHECK_EQUAL(nTest.fromString("16#11170"), -1);
    BOOST_CHECK_EQUAL(nTest.fromString("-130"), -1);

  //check invalid fromString string
  BOOST_CHECK_EQUAL(nTest.fromString("NOT A VALID STRING"), -1);
}
BOOST_AUTO_TEST_SUITE_END()
