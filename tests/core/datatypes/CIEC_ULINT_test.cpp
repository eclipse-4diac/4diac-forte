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

#include "../../../src/core/datatypes/forte_ulint.h"

BOOST_AUTO_TEST_SUITE(CIEC_ULINT_function_test)

BOOST_AUTO_TEST_CASE(Type_test)
{
  CIEC_ULINT nTest;
  //check type information
  BOOST_CHECK_EQUAL(nTest.getDataTypeID(), CIEC_ANY::e_ULINT);
  //check operator bool data type size
  BOOST_CHECK_EQUAL(sizeof(nTest.operator TForteUInt64()), sizeof(TForteUInt64));

}

BOOST_AUTO_TEST_CASE(Assignment_test)
{
  CIEC_ULINT nTest1;
  CIEC_ULINT nTest2;

  //initial value must be 0
  BOOST_CHECK_EQUAL(nTest1, 0ULL);

  nTest1 = 0ULL;
  nTest2 = nTest1;
  BOOST_CHECK_EQUAL(nTest1, 0ULL);
  BOOST_CHECK_EQUAL(nTest2, 0ULL);

  nTest1 = 13546842ULL;
  nTest2 = nTest1;
  BOOST_CHECK_EQUAL(nTest1, 13546842ULL);
  BOOST_CHECK_EQUAL(nTest2, 13546842ULL);

  nTest1 = 18446744073709551615ULL;
  nTest2 = nTest1;
  BOOST_CHECK_EQUAL(nTest1, 18446744073709551615ULL);
  BOOST_CHECK_EQUAL(nTest2, 18446744073709551615ULL);

  //check that assignment operator does not intertwine objects
  nTest2 = 3652584218745ULL;
  BOOST_CHECK_EQUAL(nTest1, 18446744073709551615ULL);
  BOOST_CHECK_EQUAL(nTest2, 3652584218745ULL);

}

BOOST_AUTO_TEST_CASE(Conversion_test)
{
  CIEC_ULINT nTest;

  char cBuffer[21];
  char cBufferFail[2];

  //check cast operator
  nTest = 0;
  strcpy(cBuffer, "");

  BOOST_CHECK_EQUAL(nTest.operator TForteUInt64(), 0ULL);

  nTest = 5874125369851ULL;
  BOOST_CHECK_EQUAL(nTest.operator TForteUInt64(), 5874125369851ULL);

  nTest = 18446744073709551615ULL;
  BOOST_CHECK_EQUAL(nTest.operator TForteUInt64(), 18446744073709551615ULL);

  //check toString and fromString
  BOOST_CHECK_EQUAL(nTest.fromString("0"), 1);
    BOOST_CHECK_EQUAL(nTest, 0ULL);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 21), 1);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "0"), 0);
    strcpy(cBuffer, "");
    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 0), -1);

    BOOST_CHECK_EQUAL(nTest.fromString("2#0"), 3);
    BOOST_CHECK_EQUAL(nTest, 0ULL);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 21), 1);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "0"), 0);
    strcpy(cBuffer, "");
    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 0), -1);

    BOOST_CHECK_EQUAL(nTest.fromString("8#0"), 3);
    BOOST_CHECK_EQUAL(nTest, 0ULL);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 21), 1);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "0"), 0);
    strcpy(cBuffer, "");
    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 0), -1);

    BOOST_CHECK_EQUAL(nTest.fromString("16#0"), 4);
    BOOST_CHECK_EQUAL(nTest, 0ULL);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 21), 1);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "0"), 0);
    strcpy(cBuffer, "");
    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 0), -1);

    BOOST_CHECK_EQUAL(nTest.fromString("62584325147"), 11);
    BOOST_CHECK_EQUAL(nTest, 62584325147ULL);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 21), 11);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "62584325147"), 0);
    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
    strcpy(cBuffer, "");
    nTest = 0;

    BOOST_CHECK_EQUAL(nTest.fromString("2#111010010010010100010000010000011011"), 38);
    BOOST_CHECK_EQUAL(nTest, 62584325147ULL);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 21), 11);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "62584325147"), 0);
    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
    strcpy(cBuffer, "");
    nTest = 0;

    BOOST_CHECK_EQUAL(nTest.fromString("8#722224202033"), 14);
    BOOST_CHECK_EQUAL(nTest, 62584325147ULL);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 21), 11);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "62584325147"), 0);
    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
    strcpy(cBuffer, "");
    nTest = 0;

    BOOST_CHECK_EQUAL(nTest.fromString("16#E9251041B"), 12);
    BOOST_CHECK_EQUAL(nTest, 62584325147ULL);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 21), 11);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "62584325147"), 0);
    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
    strcpy(cBuffer, "");
    nTest = 0;


    BOOST_CHECK_EQUAL(nTest.fromString("18446744073709551615"), 20);
    BOOST_CHECK_EQUAL(nTest, 18446744073709551615ULL);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 21), 20);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "18446744073709551615"), 0);
    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
    strcpy(cBuffer, "");
    nTest = 0;

    BOOST_CHECK_EQUAL(nTest.fromString("2#1111111111111111111111111111111111111111111111111111111111111111"), 66);
    BOOST_CHECK_EQUAL(nTest, 18446744073709551615ULL);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 21), 20);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "18446744073709551615"), 0);
    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
    strcpy(cBuffer, "");
    nTest = 0;

    BOOST_CHECK_EQUAL(nTest.fromString("8#1777777777777777777777"), 24);
    BOOST_CHECK_EQUAL(nTest, 18446744073709551615ULL);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 21), 20);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "18446744073709551615"), 0);
    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
    strcpy(cBuffer, "");
    nTest = 0;

    BOOST_CHECK_EQUAL(nTest.fromString("16#FFFFFFFFFFFFFFFF"), 19);
    BOOST_CHECK_EQUAL(nTest, 18446744073709551615ULL);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 21), 20);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "18446744073709551615"), 0);
    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
    strcpy(cBuffer, "");
    nTest = 0;

    //testing values outside of allowed range
    BOOST_CHECK_EQUAL(nTest.fromString("184467440737095516159"), -1);
    BOOST_CHECK_EQUAL(nTest.fromString("2#11111111111111111111111111111111111111111111111111111111111111110"), -1);
    BOOST_CHECK_EQUAL(nTest.fromString("8#17777777777777777777770"), -1);
    BOOST_CHECK_EQUAL(nTest.fromString("10#92233720368547758076"), -1);
    BOOST_CHECK_EQUAL(nTest.fromString("16#FFFFFFFFFFFFFFFF0"), -1);
    BOOST_CHECK_EQUAL(nTest.fromString("-130"), -1);

  //check invalid fromString string
  BOOST_CHECK_EQUAL(nTest.fromString("NOT A VALID STRING"), -1);
}
BOOST_AUTO_TEST_SUITE_END()
