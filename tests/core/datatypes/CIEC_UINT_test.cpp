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

#include "../../../src/core/datatypes/forte_uint.h"

BOOST_AUTO_TEST_SUITE(CIEC_UINT_function_test)

BOOST_AUTO_TEST_CASE(Type_test)
{
  CIEC_UINT nTest;
  //check type information
  BOOST_CHECK_EQUAL(nTest.getDataTypeID(), CIEC_ANY::e_UINT);
  //check operator bool data type size
  BOOST_CHECK_EQUAL(sizeof(nTest.operator TForteUInt16()), sizeof(TForteUInt16));

}

BOOST_AUTO_TEST_CASE(Assignment_test)
{
  CIEC_UINT nTest1;
  CIEC_UINT nTest2;

  //initial value must be 0
  BOOST_CHECK_EQUAL(nTest1, 0);

  nTest1 = 0;
  nTest2 = nTest1;
  BOOST_CHECK_EQUAL(nTest1, 0);
  BOOST_CHECK_EQUAL(nTest2, 0);

  nTest1 = 23584;
  nTest2 = nTest1;
  BOOST_CHECK_EQUAL(nTest1, 23584);
  BOOST_CHECK_EQUAL(nTest2, 23584);

  nTest1 = 65535;
  nTest2 = nTest1;
  BOOST_CHECK_EQUAL(nTest1, 65535);
  BOOST_CHECK_EQUAL(nTest2, 65535);

  //check that assignment operator does not intertwine objects
  nTest2 = 12800;
  BOOST_CHECK_EQUAL(nTest1, 65535);
  BOOST_CHECK_EQUAL(nTest2, 12800);

}

BOOST_AUTO_TEST_CASE(Conversion_test)
{
  CIEC_UINT nTest;

  char cBuffer[6];
  char cBufferFail[2];

  //check cast operator
  nTest = 0;
  strcpy(cBuffer, "");

  BOOST_CHECK_EQUAL(nTest.operator TForteUInt16(), 0);

  nTest = 2453;
  BOOST_CHECK_EQUAL(nTest.operator TForteUInt16(), 2453);

  nTest = 65535;
  BOOST_CHECK_EQUAL(nTest.operator TForteUInt16(), 65535);

  //check toString and fromString
  BOOST_CHECK_EQUAL(nTest.fromString("0"), 1);
    BOOST_CHECK_EQUAL(nTest, 0);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 6), 1);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "0"), 0);

    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 0), -1);

    strcpy(cBuffer, "");

    BOOST_CHECK_EQUAL(nTest.fromString("2#0"), 3);
    BOOST_CHECK_EQUAL(nTest, 0);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 6), 1);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "0"), 0);

    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 0), -1);

    strcpy(cBuffer, "");

    BOOST_CHECK_EQUAL(nTest.fromString("8#0"), 3);
    BOOST_CHECK_EQUAL(nTest, 0);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 6), 1);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "0"), 0);

    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 0), -1);

    strcpy(cBuffer, "");

    BOOST_CHECK_EQUAL(nTest.fromString("16#0"), 4);
    BOOST_CHECK_EQUAL(nTest, 0);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 6), 1);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "0"), 0);

    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 0), -1);

    strcpy(cBuffer, "");


    BOOST_CHECK_EQUAL(nTest.fromString("38549"), 5);
    BOOST_CHECK_EQUAL(nTest, 38549);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 6), 5);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "38549"), 0);

    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);

    strcpy(cBuffer, "");
    nTest = 0;

    BOOST_CHECK_EQUAL(nTest.fromString("2#1001011010010101"), 18);
    BOOST_CHECK_EQUAL(nTest, 38549);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 6), 5);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "38549"), 0);

    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);

    strcpy(cBuffer, "");
    nTest = 0;

    BOOST_CHECK_EQUAL(nTest.fromString("8#113225"), 8);
    BOOST_CHECK_EQUAL(nTest, 38549);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 6), 5);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "38549"), 0);

    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);

    strcpy(cBuffer, "");
    nTest = 0;

    BOOST_CHECK_EQUAL(nTest.fromString("16#9695"), 7);
    BOOST_CHECK_EQUAL(nTest, 38549);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 6), 5);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "38549"), 0);

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
    BOOST_CHECK_EQUAL(nTest.fromString("2#111111111111111100"), -1);
    BOOST_CHECK_EQUAL(nTest.fromString("8#200000"), -1);
    BOOST_CHECK_EQUAL(nTest.fromString("10#70000"), -1);
    BOOST_CHECK_EQUAL(nTest.fromString("16#FFFF0"), -1);
    BOOST_CHECK_EQUAL(nTest.fromString("-130"), -1);

  //check invalid fromString string
  BOOST_CHECK_EQUAL(nTest.fromString("NOT A VALID STRING"), -1);
}
BOOST_AUTO_TEST_SUITE_END()
