/*******************************************************************************
 * Copyright (c) 2011 - 2014 ACIN, fortiss GmbH
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

#include "../../../src/core/datatypes/forte_dint.h"

BOOST_AUTO_TEST_SUITE(CIEC_DINT_function_test)
BOOST_AUTO_TEST_CASE(Type_test)
{
  CIEC_DINT nTest;
  //check type information
  BOOST_CHECK_EQUAL(nTest.getDataTypeID(), CIEC_ANY::e_DINT);
  //check operator bool data type size
  BOOST_CHECK_EQUAL(sizeof(nTest.operator TForteInt32()), sizeof(TForteInt32));

}

BOOST_AUTO_TEST_CASE(Assignment_test)
{
  CIEC_DINT nTest1;
  CIEC_DINT nTest2;

  //initial value must be 0
  BOOST_CHECK_EQUAL(nTest1, 0);

  nTest1 = std::numeric_limits<TForteInt32>::min();
  nTest2 = nTest1;
  BOOST_CHECK_EQUAL(nTest1, std::numeric_limits<TForteInt32>::min());
  BOOST_CHECK_EQUAL(nTest2, std::numeric_limits<TForteInt32>::min());

  nTest1 = -896475124;
  nTest2 = nTest1;
  BOOST_CHECK_EQUAL(nTest1, -896475124);
  BOOST_CHECK_EQUAL(nTest2, -896475124);

  nTest1 = 36548;
  nTest2 = nTest1;
  BOOST_CHECK_EQUAL(nTest1, 36548);
  BOOST_CHECK_EQUAL(nTest2, 36548);

  nTest1 = std::numeric_limits<TForteInt32>::max();
  nTest2 = nTest1;
  BOOST_CHECK_EQUAL(nTest1, std::numeric_limits<TForteInt32>::max());
  BOOST_CHECK_EQUAL(nTest2, std::numeric_limits<TForteInt32>::max());

  //check that assignment operator does not intertwine objects
  nTest2 = 6874;
  BOOST_CHECK_EQUAL(nTest1, std::numeric_limits<TForteInt32>::max());
  BOOST_CHECK_EQUAL(nTest2, 6874);

}

BOOST_AUTO_TEST_CASE(Conversion_test)
{
  CIEC_DINT nTest;

  char cBuffer[12];
  char cBufferFail[2];

  //check cast operator
  nTest = 0;

  BOOST_CHECK_EQUAL(nTest.operator TForteInt32(), 0);

  nTest = std::numeric_limits<TForteInt32>::min();
  BOOST_CHECK_EQUAL(nTest.operator TForteInt32(), std::numeric_limits<TForteInt32>::min());

  nTest = 9853268;
  BOOST_CHECK_EQUAL(nTest.operator TForteInt32(), 9853268);

  nTest = std::numeric_limits<TForteInt32>::max();
  BOOST_CHECK_EQUAL(nTest.operator TForteInt32(), std::numeric_limits<TForteInt32>::max());

  //check toString and fromString
  strcpy(cBuffer, "");

  BOOST_CHECK_EQUAL(nTest.fromString("-2147483648"), 11);
  BOOST_CHECK_EQUAL(-2147483648L, nTest);
  BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 12), 11);
  BOOST_CHECK_EQUAL(strcmp(cBuffer, "-2147483648"), 0);
  BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
  strcpy(cBuffer, "");

  BOOST_CHECK_EQUAL(nTest.fromString("2#10000000000000000000000000000000"), 34);
  BOOST_CHECK_EQUAL(nTest, -2147483648L);
  BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 12), 11);
  BOOST_CHECK_EQUAL(strcmp(cBuffer, "-2147483648"), 0);
  BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
  strcpy(cBuffer, "");

  BOOST_CHECK_EQUAL(nTest.fromString("8#20000000000"), 13);
  BOOST_CHECK_EQUAL(nTest, -2147483648L);
  BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 12), 11);
  BOOST_CHECK_EQUAL(strcmp(cBuffer, "-2147483648"), 0);
  BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
  strcpy(cBuffer, "");

  BOOST_CHECK_EQUAL(nTest.fromString("16#80000000"), 11);
  BOOST_CHECK_EQUAL(nTest, -2147483648L);
  BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 12), 11);
  BOOST_CHECK_EQUAL(strcmp(cBuffer, "-2147483648"), 0);
  BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
  strcpy(cBuffer, "");


  BOOST_CHECK_EQUAL(nTest.fromString("0"), 1);
    BOOST_CHECK_EQUAL(nTest, 0);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 12), 1);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "0"), 0);
    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 0), -1);

    strcpy(cBuffer, "");

    BOOST_CHECK_EQUAL(nTest.fromString("2#0"), 3);
    BOOST_CHECK_EQUAL(nTest, 0);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 12), 1);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "0"), 0);
    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 0), -1);
    strcpy(cBuffer, "");

    BOOST_CHECK_EQUAL(nTest.fromString("8#0"), 3);
    BOOST_CHECK_EQUAL(nTest, 0);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 12), 1);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "0"), 0);
    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 0), -1);

    strcpy(cBuffer, "");

    BOOST_CHECK_EQUAL(nTest.fromString("16#0"), 4);
    BOOST_CHECK_EQUAL(nTest, 0);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 12), 1);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "0"), 0);
    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 0), -1);
    strcpy(cBuffer, "");


    BOOST_CHECK_EQUAL(nTest.fromString("368745"), 6);
    BOOST_CHECK_EQUAL(nTest, 368745);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 12), 6);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "368745"), 0);

    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);

    strcpy(cBuffer, "");
    nTest = 0;

    BOOST_CHECK_EQUAL(nTest.fromString("2#1011010000001101001"), 21);
    BOOST_CHECK_EQUAL(nTest, 368745);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 12), 6);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "368745"), 0);

    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);

    strcpy(cBuffer, "");
    nTest = 0;


    BOOST_CHECK_EQUAL(nTest.fromString("8#1320151"), 9);
    BOOST_CHECK_EQUAL(nTest, 368745);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 12), 6);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "368745"), 0);

    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);

    strcpy(cBuffer, "");
    nTest = 0;

    BOOST_CHECK_EQUAL(nTest.fromString("16#5a069"), 8);
    BOOST_CHECK_EQUAL(nTest, 368745);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 12), 6);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "368745"), 0);

    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);

    strcpy(cBuffer, "");
    nTest = 0;


    BOOST_CHECK_EQUAL(nTest.fromString("2147483647"), 10);
    BOOST_CHECK_EQUAL(nTest, 2147483647);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 12), 10);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "2147483647"), 0);
    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
    strcpy(cBuffer, "");
    nTest = 0;

    BOOST_CHECK_EQUAL(nTest.fromString("2#01111111111111111111111111111111"), 34);
    BOOST_CHECK_EQUAL(nTest, 2147483647);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 12), 10);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "2147483647"), 0);
    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
    strcpy(cBuffer, "");
    nTest = 0;

    BOOST_CHECK_EQUAL(nTest.fromString("8#17777777777"), 13);
    BOOST_CHECK_EQUAL(nTest, 2147483647);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 12), 10);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "2147483647"), 0);
    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
    strcpy(cBuffer, "");
    nTest = 0;

    BOOST_CHECK_EQUAL(nTest.fromString("16#7FFFFfFF"), 11);
    BOOST_CHECK_EQUAL(nTest, 2147483647);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 12), 10);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "2147483647"), 0);
    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
    strcpy(cBuffer, "");
    nTest = 0;

    BOOST_CHECK_EQUAL(nTest.fromString("10#300"), 6);
    BOOST_CHECK_EQUAL(nTest, 300);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 12), 3);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "300"), 0);

    //testing values outside of allowed range
    BOOST_CHECK_EQUAL(nTest.fromString("21474836473"), -1);
    BOOST_CHECK_EQUAL(nTest.fromString("2#1011111111111111111111111111111110"), -1);
    BOOST_CHECK_EQUAL(nTest.fromString("8#177777777770"), -1);
    BOOST_CHECK_EQUAL(nTest.fromString("16#7FFFFFFF0"), -1);
    BOOST_CHECK_EQUAL(nTest.fromString("-21474836473"), -1);

  //check invalid fromString string
  BOOST_CHECK_EQUAL(nTest.fromString("NOT A VALID STRING"), -1);
}
BOOST_AUTO_TEST_SUITE_END()
