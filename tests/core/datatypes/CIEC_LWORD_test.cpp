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

#include "../../../src/core/datatypes/forte_lword.h"

BOOST_AUTO_TEST_SUITE(CIEC_LWORD_function_test)
BOOST_AUTO_TEST_CASE(Type_test)
{
  CIEC_LWORD nTest;
  //check type information
  BOOST_CHECK_EQUAL(nTest.getDataTypeID(), CIEC_ANY::e_LWORD);
  //check operator bool data type size
  BOOST_CHECK_EQUAL(sizeof(nTest.operator TForteLWord()), sizeof(TForteLWord));

}

BOOST_AUTO_TEST_CASE(Assignment_test)
{
  CIEC_LWORD nTest1;
  CIEC_LWORD nTest2;

  //initial value must be 0
  BOOST_CHECK_EQUAL(nTest1, 0ULL);

  nTest1 = 0ULL;
  nTest2 = nTest1;
  BOOST_CHECK_EQUAL(nTest1, 0ULL);
  BOOST_CHECK_EQUAL(nTest2, 0ULL);

  nTest1 = 87456813ULL;
  nTest2 = nTest1;
  BOOST_CHECK_EQUAL(nTest1, 87456813ULL);
  BOOST_CHECK_EQUAL(nTest2, 87456813ULL);

  nTest1 = 18446744073709551615ULL;
  nTest2 = nTest1;
  BOOST_CHECK_EQUAL(nTest1, 18446744073709551615ULL);
  BOOST_CHECK_EQUAL(nTest2, 18446744073709551615ULL);

  //check that assignment operator does not intertwine objects
  nTest2 = 25432341ULL;
  BOOST_CHECK_EQUAL(nTest1, 18446744073709551615ULL);
  BOOST_CHECK_EQUAL(nTest2, 25432341ULL);

}

BOOST_AUTO_TEST_CASE(Conversion_test)
{
  CIEC_LWORD nTest;

  char cBuffer[21];
  char cBufferFail[2];

  //check cast operator
  nTest = 0;
  strcpy(cBuffer, "");

  BOOST_CHECK_EQUAL(nTest.operator TForteLWord(), 0ULL);

  nTest = 14524526231894545ULL;
  BOOST_CHECK_EQUAL(nTest.operator TForteLWord(), 14524526231894545ULL);

  nTest = 9223372036854775807ULL;
  BOOST_CHECK_EQUAL(nTest.operator TForteLWord(), 9223372036854775807ULL);

  //check toString and fromString
  BOOST_CHECK_EQUAL(nTest.fromString("0"), 1);
    BOOST_CHECK_EQUAL(nTest, 0ULL);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 21), 1);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "0"), 0);

    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 1), -1);

    strcpy(cBuffer, "");

    BOOST_CHECK_EQUAL(nTest.fromString("2#0"), 3);
    BOOST_CHECK_EQUAL(nTest, 0ULL);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 21), 1);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "0"), 0);

    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 1), -1);

    strcpy(cBuffer, "");

    BOOST_CHECK_EQUAL(nTest.fromString("8#0"), 3);
    BOOST_CHECK_EQUAL(nTest, 0ULL);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 21), 1);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "0"), 0);

    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 1), -1);

    strcpy(cBuffer, "");

    BOOST_CHECK_EQUAL(nTest.fromString("16#0"), 4);
    BOOST_CHECK_EQUAL(nTest, 0ULL);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 21), 1);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "0"), 0);

    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 1), -1);

    strcpy(cBuffer, "");

    BOOST_CHECK_EQUAL(nTest.fromString("45387346543765474"), 17);
    BOOST_CHECK_EQUAL(nTest, 45387346543765474ULL);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 21), 17);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "45387346543765474"), 0);

    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);

    strcpy(cBuffer, "");
    nTest = 0;

    BOOST_CHECK_EQUAL(nTest.fromString("2#10100001001111111000110100011100000010011110011111100010"), 58);
    BOOST_CHECK_EQUAL(nTest, 45387346543765474ULL);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 21), 17);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "45387346543765474"), 0);

    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);

    strcpy(cBuffer, "");
    nTest = 0;

    BOOST_CHECK_EQUAL(nTest.fromString("8#2411770643402363742"), 21);
    BOOST_CHECK_EQUAL(nTest, 45387346543765474ULL);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 21), 17);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "45387346543765474"), 0);

    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);

    strcpy(cBuffer, "");
    nTest = 0;

    BOOST_CHECK_EQUAL(nTest.fromString("16#A13F8D1C09E7E2"), 17);
    BOOST_CHECK_EQUAL(nTest, 45387346543765474ULL);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 21), 17);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "45387346543765474"), 0);

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
    BOOST_CHECK_EQUAL(nTest.fromString("184467440737095516150"), -1);
    BOOST_CHECK_EQUAL(nTest.fromString("2#11111111111111111111111111111111111111111111111111111111111111110"), -1);
    BOOST_CHECK_EQUAL(nTest.fromString("8#17777777777777777777770"), -1);
    BOOST_CHECK_EQUAL(nTest.fromString("10#922337203685477580754"), -1);
    BOOST_CHECK_EQUAL(nTest.fromString("16#FFFFFFFFFFFFFFFF0"), -1);
    BOOST_CHECK_EQUAL(nTest.fromString("-130"), -1);

  //check invalid fromString string
  BOOST_CHECK_EQUAL(nTest.fromString("NOT A VALID STRING"), -1);
}
BOOST_AUTO_TEST_SUITE_END()
