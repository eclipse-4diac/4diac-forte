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

#include "../../../src/core/datatypes/forte_dword.h"

BOOST_AUTO_TEST_SUITE(CIEC_DWORD_function_test)

BOOST_AUTO_TEST_CASE(Type_test)
{
  CIEC_DWORD nTest;
  //check type information
  BOOST_CHECK_EQUAL(nTest.getDataTypeID(), CIEC_ANY::e_DWORD);
  //check operator bool data type size
  BOOST_CHECK_EQUAL(sizeof(nTest.operator TForteDWord()), sizeof(TForteDWord));

}

BOOST_AUTO_TEST_CASE(Assignment_test)
{
  CIEC_DWORD nTest1;
  CIEC_DWORD nTest2;

  //initial value must be 0
  BOOST_CHECK_EQUAL(nTest1, 0UL);

  nTest1 = 0;
  nTest2 = nTest1;
  BOOST_CHECK_EQUAL(nTest1, 0UL);
  BOOST_CHECK_EQUAL(nTest2, 0UL);

  nTest1 = 87456813UL;
  nTest2 = nTest1;
  BOOST_CHECK_EQUAL(nTest1, 87456813UL);
  BOOST_CHECK_EQUAL(nTest2, 87456813UL);

  nTest1 = 4294967295UL;
  nTest2 = nTest1;
  BOOST_CHECK_EQUAL(nTest1, 4294967295UL);
  BOOST_CHECK_EQUAL(nTest2, 4294967295UL);

  //check that assignment operator does not intertwine objects
  nTest2 = 25432341UL;
  BOOST_CHECK_EQUAL(nTest1, 4294967295UL);
  BOOST_CHECK_EQUAL(nTest2, 25432341UL);

}

BOOST_AUTO_TEST_CASE(Conversion_test)
{
  CIEC_DWORD nTest;

  char cBuffer[11];
  char cBufferFail[2];

  //check cast operator
  nTest = 0;
  strcpy(cBuffer, "");

  BOOST_CHECK_EQUAL(nTest.operator TForteDWord(), 0UL);

  nTest = 294967295UL;
  BOOST_CHECK_EQUAL(nTest.operator TForteDWord(), 294967295UL);

  nTest = 4294967295UL;
  BOOST_CHECK_EQUAL(nTest.operator TForteDWord(), 4294967295UL);

  //check toString and fromString
  BOOST_CHECK_EQUAL(nTest.fromString("0"), 1);
    BOOST_CHECK_EQUAL(nTest, 0UL);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 11), 1);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "0"), 0);

    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 1), -1);

    strcpy(cBuffer, "");

    BOOST_CHECK_EQUAL(nTest.fromString("2#0"), 3);
    BOOST_CHECK_EQUAL(nTest, 0UL);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 11), 1);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "0"), 0);

    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 1), -1);

    strcpy(cBuffer, "");

    BOOST_CHECK_EQUAL(nTest.fromString("8#0"), 3);
    BOOST_CHECK_EQUAL(nTest, 0UL);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 11), 1);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "0"), 0);

    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 1), -1);

    strcpy(cBuffer, "");

    BOOST_CHECK_EQUAL(nTest.fromString("16#0"), 4);
    BOOST_CHECK_EQUAL(nTest, 0UL);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 11), 1);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "0"), 0);

    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 1), -1);

    strcpy(cBuffer, "");

    BOOST_CHECK_EQUAL(nTest.fromString("56397517"), 8);
    BOOST_CHECK_EQUAL(nTest, 56397517UL);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 11), 8);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "56397517"), 0);

    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);

    strcpy(cBuffer, "");
    nTest = 0;

    BOOST_CHECK_EQUAL(nTest.fromString("2#11010111001000111011001101"), 28);
    BOOST_CHECK_EQUAL(nTest, 56397517UL);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 11), 8);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "56397517"), 0);

    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);

    strcpy(cBuffer, "");
    nTest = 0;

    BOOST_CHECK_EQUAL(nTest.fromString("8#327107315"), 11);
    BOOST_CHECK_EQUAL(nTest, 56397517UL);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 11), 8);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "56397517"), 0);

    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);

    strcpy(cBuffer, "");
    nTest = 0;

    BOOST_CHECK_EQUAL(nTest.fromString("16#35C8ECD"), 10);
    BOOST_CHECK_EQUAL(nTest, 56397517UL);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 11), 8);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "56397517"), 0);

    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);

    strcpy(cBuffer, "");
    nTest = 0;

    BOOST_CHECK_EQUAL(nTest.fromString("4294967295"), 10);
    BOOST_CHECK_EQUAL(nTest, 4294967295UL);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 11), 10);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "4294967295"), 0);

    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);

    strcpy(cBuffer, "");
    nTest = 0;

    BOOST_CHECK_EQUAL(nTest.fromString("2#11111111111111111111111111111111"), 34);
    BOOST_CHECK_EQUAL(nTest, 4294967295UL);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 11), 10);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "4294967295"), 0);

    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);

    strcpy(cBuffer, "");
    nTest = 0;

    BOOST_CHECK_EQUAL(nTest.fromString("8#37777777777"), 13);
    BOOST_CHECK_EQUAL(nTest, 4294967295UL);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 11), 10);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "4294967295"), 0);

    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);

    strcpy(cBuffer, "");
    nTest = 0;

    BOOST_CHECK_EQUAL(nTest.fromString("16#FFFFFFFF"), 11);
    BOOST_CHECK_EQUAL(nTest, 4294967295UL);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 11), 10);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "4294967295"), 0);

    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);

    strcpy(cBuffer, "");
    nTest = 0;

    //testing values outside of allowed range
    BOOST_CHECK_EQUAL(nTest.fromString("5000000000"), -1);
    BOOST_CHECK_EQUAL(nTest.fromString("2#100101010000001011111001000000000"), -1);
    BOOST_CHECK_EQUAL(nTest.fromString("8#45201371000"), -1);
    BOOST_CHECK_EQUAL(nTest.fromString("10#5000000000"), -1);
    BOOST_CHECK_EQUAL(nTest.fromString("16#12A05F200"), -1);
    BOOST_CHECK_EQUAL(nTest.fromString("-130"), -1);

  //check invalid fromString string
  BOOST_CHECK_EQUAL(nTest.fromString("NOT A VALID STRING"), -1);
}
BOOST_AUTO_TEST_SUITE_END()
