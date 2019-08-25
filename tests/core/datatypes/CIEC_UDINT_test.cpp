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

#include "../../../src/core/datatypes/forte_udint.h"

BOOST_AUTO_TEST_SUITE(CIEC_UDINT_function_test)

BOOST_AUTO_TEST_CASE(Type_test)
{
  CIEC_UDINT nTest;
  //check type information
  BOOST_CHECK_EQUAL(nTest.getDataTypeID(), CIEC_ANY::e_UDINT);
  //check operator bool data type size
  BOOST_CHECK_EQUAL(sizeof(nTest.operator TForteUInt32()), sizeof(TForteUInt32));

}

BOOST_AUTO_TEST_CASE(Assignment_test)
{
  CIEC_UDINT nTest1;
  CIEC_UDINT nTest2;

  //initial value must be 0
  BOOST_CHECK_EQUAL(nTest1, 0UL);

  nTest1 = 0;
  nTest2 = nTest1;
  BOOST_CHECK_EQUAL(nTest1, 0UL);
  BOOST_CHECK_EQUAL(nTest2, 0UL);

  nTest1 = 13546842UL;
  nTest2 = nTest1;
  BOOST_CHECK_EQUAL(nTest1, 13546842UL);
  BOOST_CHECK_EQUAL(nTest2, 13546842UL);

  nTest1 = 4294967295UL;
  nTest2 = nTest1;
  BOOST_CHECK_EQUAL(nTest1, 4294967295UL);
  BOOST_CHECK_EQUAL(nTest2, 4294967295UL);

  //check that assignment operator does not intertwine objects
  nTest2 = 951753258UL;
  BOOST_CHECK_EQUAL(nTest1, 4294967295UL);
  BOOST_CHECK_EQUAL(nTest2, 951753258UL);

}

BOOST_AUTO_TEST_CASE(Conversion_test)
{
  CIEC_UDINT nTest;

  char cBuffer[11];
  char cBufferFail[2];

  //check cast operator
  nTest = 0;
  strcpy(cBuffer, "");

  BOOST_CHECK_EQUAL(nTest.operator TForteUInt32(), 0UL);

  nTest = 654753258UL;
  BOOST_CHECK_EQUAL(nTest.operator TForteUInt32(), 654753258UL);

  nTest = 4294967295UL;
  BOOST_CHECK_EQUAL(nTest.operator TForteUInt32(), 4294967295UL);

  //check toString and fromString
  BOOST_CHECK_EQUAL(nTest.fromString("0"), 1);
    BOOST_CHECK_EQUAL(nTest, 0UL);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 11), 1);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "0"), 0);

    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 0), -1);

    strcpy(cBuffer, "");

    BOOST_CHECK_EQUAL(nTest.fromString("2#0"), 3);
    BOOST_CHECK_EQUAL(nTest, 0UL);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 11), 1);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "0"), 0);

    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 0), -1);

    strcpy(cBuffer, "");

    BOOST_CHECK_EQUAL(nTest.fromString("8#0"), 3);
    BOOST_CHECK_EQUAL(nTest, 0UL);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 11), 1);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "0"), 0);

    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 0), -1);

    strcpy(cBuffer, "");

    BOOST_CHECK_EQUAL(nTest.fromString("16#0"), 4);
    BOOST_CHECK_EQUAL(nTest, 0UL);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 11), 1);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "0"), 0);

    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 0), -1);

    strcpy(cBuffer, "");

    BOOST_CHECK_EQUAL(nTest.fromString("147896321"), 9);
    BOOST_CHECK_EQUAL(nTest, 147896321UL);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 11), 9);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "147896321"), 0);

    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);

    strcpy(cBuffer, "");
    nTest = 0;

    BOOST_CHECK_EQUAL(nTest.fromString("2#1000110100001011100000000001"), 30);
    BOOST_CHECK_EQUAL(nTest, 147896321UL);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 11), 9);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "147896321"), 0);

    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);

    strcpy(cBuffer, "");
    nTest = 0;

    BOOST_CHECK_EQUAL(nTest.fromString("8#1064134001"), 12);
    BOOST_CHECK_EQUAL(nTest, 147896321UL);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 11), 9);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "147896321"), 0);

    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);

    strcpy(cBuffer, "");
    nTest = 0;

    BOOST_CHECK_EQUAL(nTest.fromString("16#8D0B801"), 10);
    BOOST_CHECK_EQUAL(nTest, 147896321UL);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 11), 9);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "147896321"), 0);

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
    BOOST_CHECK_EQUAL(nTest.fromString("42949672958"), -1);
    BOOST_CHECK_EQUAL(nTest.fromString("2#1111111111111111111111111111111100"), -1);
    BOOST_CHECK_EQUAL(nTest.fromString("8#377777777770"), -1);
    BOOST_CHECK_EQUAL(nTest.fromString("10#5294967295"), -1);
    BOOST_CHECK_EQUAL(nTest.fromString("16#FFFFFFFF0"), -1);
    BOOST_CHECK_EQUAL(nTest.fromString("-130"), -1);

  //check invalid fromString string
  BOOST_CHECK_EQUAL(nTest.fromString("NOT A VALID STRING"), -1);
}
BOOST_AUTO_TEST_SUITE_END()
