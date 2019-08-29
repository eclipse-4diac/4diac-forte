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

#include "../../../src/core/datatypes/forte_lint.h"

BOOST_AUTO_TEST_SUITE(CIEC_LINT_function_test)
BOOST_AUTO_TEST_CASE(Type_test)
{
  CIEC_LINT nTest;
  //check type information
  BOOST_CHECK_EQUAL(nTest.getDataTypeID(), CIEC_ANY::e_LINT);
  //check operator bool data type size
  BOOST_CHECK_EQUAL(sizeof(nTest.operator TForteInt64()), sizeof(TForteInt64));

}

BOOST_AUTO_TEST_CASE(Assignment_test)
{
  CIEC_LINT nTest1;
  CIEC_LINT nTest2;

  //initial value must be 0
  BOOST_CHECK_EQUAL(nTest1, 0);

  nTest1 = CIEC_LINT::scm_nMinVal;
  nTest2 = nTest1;
  BOOST_CHECK_EQUAL(nTest1, CIEC_LINT::scm_nMinVal);
  BOOST_CHECK_EQUAL(nTest2, CIEC_LINT::scm_nMinVal);

  nTest1 = -56921482365875466LL;
  nTest2 = nTest1;
  BOOST_CHECK_EQUAL(nTest1, -56921482365875466LL);
  BOOST_CHECK_EQUAL(nTest2, -56921482365875466LL);

  nTest1 = 4723658749321LL;
  nTest2 = nTest1;
  BOOST_CHECK_EQUAL(nTest1, 4723658749321LL);
  BOOST_CHECK_EQUAL(nTest2, 4723658749321LL);

  nTest1 = 9223372036854775807LL;
  nTest2 = nTest1;
  BOOST_CHECK_EQUAL(nTest1, 9223372036854775807LL);
  BOOST_CHECK_EQUAL(nTest2, 9223372036854775807LL);

  nTest2 = -6548432101LL;
  BOOST_CHECK_EQUAL(nTest1, 9223372036854775807LL);
  BOOST_CHECK_EQUAL(nTest2, -6548432101LL);
}

BOOST_AUTO_TEST_CASE(Conversion_test)
{
  CIEC_LINT nTest;

  char cBuffer[21];
  char cBufferFail[2];

  //check cast operator
  nTest = 0;

  BOOST_CHECK_EQUAL(nTest.operator TForteInt64(), 0);

  nTest = -68734876324LL;
  BOOST_CHECK_EQUAL(nTest.operator TForteInt64(), -68734876324LL);

  nTest = CIEC_LINT::scm_nMinVal;
  BOOST_CHECK_EQUAL(nTest.operator TForteInt64(), CIEC_LINT::scm_nMinVal);

  nTest = 8462382567LL;
  BOOST_CHECK_EQUAL(nTest.operator TForteInt64(), 8462382567LL);

  nTest = 9223372036854775807LL;
  BOOST_CHECK_EQUAL(nTest.operator TForteInt64(), 9223372036854775807LL);

  //check toString and fromString
  strcpy(cBuffer, "");

  BOOST_CHECK_EQUAL(nTest.fromString("-9223372036854775808"), 20);
  BOOST_CHECK_EQUAL(nTest, CIEC_LINT::scm_nMinVal);
  BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 21), 20);
  BOOST_CHECK_EQUAL(strcmp(cBuffer, "-9223372036854775808"), 0);
  BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
  strcpy(cBuffer, "");
  nTest = 0;

  BOOST_CHECK_EQUAL(nTest.fromString("2#1000000000000000000000000000000000000000000000000000000000000000"), 66);
  BOOST_CHECK_EQUAL(nTest, CIEC_LINT::scm_nMinVal);
  BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 21), 20);
  BOOST_CHECK_EQUAL(strcmp(cBuffer, "-9223372036854775808"), 0);
  BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
  strcpy(cBuffer, "");
  nTest = 0;

  BOOST_CHECK_EQUAL(nTest.fromString("8#1000000000000000000000"), 24);
  BOOST_CHECK_EQUAL(nTest, CIEC_LINT::scm_nMinVal);
  BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 21), 20);
  BOOST_CHECK_EQUAL(strcmp(cBuffer, "-9223372036854775808"), 0);
  BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
  strcpy(cBuffer, "");
  nTest = 0;

  BOOST_CHECK_EQUAL(nTest.fromString("16#8000000000000000"), 19);
  BOOST_CHECK_EQUAL(nTest, CIEC_LINT::scm_nMinVal);
  BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 21), 20);
  BOOST_CHECK_EQUAL(strcmp(cBuffer, "-9223372036854775808"), 0);
  BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
  strcpy(cBuffer, "");
  nTest = 0;


  BOOST_CHECK_EQUAL(nTest.fromString("0"), 1);
    BOOST_CHECK_EQUAL(nTest, 0);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 21), 1);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "0"), 0);
    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 0), -1);

    strcpy(cBuffer, "");
    nTest = 0;

    BOOST_CHECK_EQUAL(nTest.fromString("2#0"), 3);
    BOOST_CHECK_EQUAL(nTest, 0);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 21), 1);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "0"), 0);
    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 0), -1);
    strcpy(cBuffer, "");
    nTest = 0;

    BOOST_CHECK_EQUAL(nTest.fromString("8#0"), 3);
    BOOST_CHECK_EQUAL(nTest, 0);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 21), 1);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "0"), 0);

    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 0), -1);

    strcpy(cBuffer, "");
    nTest = 0;

    BOOST_CHECK_EQUAL(nTest.fromString("16#0"), 4);
    BOOST_CHECK_EQUAL(nTest, 0);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 21), 1);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "0"), 0);
    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 0), -1);
    strcpy(cBuffer, "");
    nTest = 0;


    BOOST_CHECK_EQUAL(nTest.fromString("36891234593"), 11);
    BOOST_CHECK_EQUAL(nTest, 36891234593LL);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 21), 11);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "36891234593"), 0);

    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);

    strcpy(cBuffer, "");
    nTest = 0;

    BOOST_CHECK_EQUAL(nTest.fromString("2#100010010110111000111001000100100001"), 38);
    BOOST_CHECK_EQUAL(nTest, 36891234593LL);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 21), 11);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "36891234593"), 0);

    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);

    strcpy(cBuffer, "");
    nTest = 0;


    BOOST_CHECK_EQUAL(nTest.fromString("8#422670710441"), 14);
    BOOST_CHECK_EQUAL(nTest, 36891234593LL);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 21), 11);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "36891234593"), 0);

    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);

    strcpy(cBuffer, "");
    nTest = 0;

    BOOST_CHECK_EQUAL(nTest.fromString("16#896E39121"), 12);
    BOOST_CHECK_EQUAL(nTest, 36891234593LL);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 21), 11);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "36891234593"), 0);

    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);

    strcpy(cBuffer, "");
    nTest = 0;


    BOOST_CHECK_EQUAL(nTest.fromString("9223372036854775807"), 19);
    BOOST_CHECK_EQUAL(nTest, 9223372036854775807LL);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 21), 19);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "9223372036854775807"), 0);
    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
    strcpy(cBuffer, "");
    nTest = 0;

    BOOST_CHECK_EQUAL(nTest.fromString("2#0111111111111111111111111111111111111111111111111111111111111111"), 66);
    BOOST_CHECK_EQUAL(nTest, 9223372036854775807LL);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 21), 19);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "9223372036854775807"), 0);
    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
    strcpy(cBuffer, "");
    nTest = 0;

    BOOST_CHECK_EQUAL(nTest.fromString("8#777777777777777777777"), 23);
    BOOST_CHECK_EQUAL(nTest, 9223372036854775807LL);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 21), 19);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "9223372036854775807"), 0);
    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
    strcpy(cBuffer, "");
    nTest = 0;

    BOOST_CHECK_EQUAL(nTest.fromString("16#7FFFFFFFFFFFFFFF"), 19);
    BOOST_CHECK_EQUAL(nTest, 9223372036854775807LL);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 21), 19);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "9223372036854775807"), 0);
    BOOST_CHECK_EQUAL(nTest.toString(cBufferFail, 2), -1);
    strcpy(cBuffer, "");
    nTest = 0;

    BOOST_CHECK_EQUAL(nTest.fromString("10#300"), 6);
    BOOST_CHECK_EQUAL(nTest, 300);
    BOOST_CHECK_EQUAL(nTest.toString(cBuffer, 21), 3);
    BOOST_CHECK_EQUAL(strcmp(cBuffer, "300"), 0);

    //testing values outside of allowed range
    BOOST_CHECK_EQUAL(nTest.fromString("92233720368547758070"), -1);
    BOOST_CHECK_EQUAL(nTest.fromString("2#101111111111111111111111111111111111111111111111111111111111111110"), -1);
    BOOST_CHECK_EQUAL(nTest.fromString("8#7777777777777777777770"), -1);
    BOOST_CHECK_EQUAL(nTest.fromString("16#7FFFFFFFFFFFFFFF0"), -1);
    BOOST_CHECK_EQUAL(nTest.fromString("-9223372036854775810"), -1);
    BOOST_CHECK_EQUAL(nTest.fromString("-9223372036854775809"), -1);
    BOOST_CHECK_EQUAL(nTest.fromString("-9223372036854775808"), 20);
    BOOST_CHECK_EQUAL(nTest, -9223372036854775807LL - 1LL);
    BOOST_CHECK_EQUAL(nTest.fromString("-9223372036854775807"), 20);
    BOOST_CHECK_EQUAL(nTest, -9223372036854775807LL);
    BOOST_CHECK_EQUAL(nTest.fromString("-9223372036854775806"), 20);
    BOOST_CHECK_EQUAL(nTest, -9223372036854775806LL);

  //check invalid fromString string
  BOOST_CHECK_EQUAL(nTest.fromString("NOT A VALID STRING"), -1);
}

BOOST_AUTO_TEST_SUITE_END()
