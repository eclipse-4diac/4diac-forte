/*******************************************************************************
 * Copyright (c) 2011, 2023 ACIN, fortiss GmbH
 *                          Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Martin Melik Merkumians, Ingo Hegny, Alois Zoitl - initial API and implementation and/or initial documentation
 *   Martin Jobst - add user-defined literal tests
 *******************************************************************************/
#include <boost/test/unit_test.hpp>
#include "forte_boost_output_support.h"

#include "../../../src/core/datatypes/forte_bool.h"

using namespace boost::unit_test;

BOOST_AUTO_TEST_SUITE(CIEC_BOOL_function_test)

  BOOST_AUTO_TEST_CASE(Type_test){
    CIEC_BOOL bTest;
    //check type information
    BOOST_CHECK_EQUAL(bTest.getDataTypeID(), CIEC_ANY::e_BOOL);
    //check operator bool data type size
    BOOST_CHECK_EQUAL(sizeof(bTest.operator bool()), sizeof(bool));

  }

  BOOST_AUTO_TEST_CASE(Literal_test)
  {
    CIEC_BOOL test1 = 0_BOOL;
    BOOST_TEST(static_cast<CIEC_BOOL::TValueType>(test1) == false);

    CIEC_BOOL test2 = 1_BOOL;
    BOOST_TEST(static_cast<CIEC_BOOL::TValueType>(test2) == true);

    CIEC_BOOL test3 = false_BOOL;
    BOOST_TEST(static_cast<CIEC_BOOL::TValueType>(test3) == false);

    CIEC_BOOL test4 = true_BOOL;
    BOOST_TEST(static_cast<CIEC_BOOL::TValueType>(test4) == true);
  }

  BOOST_AUTO_TEST_CASE(Assignment_test){
    CIEC_BOOL bTest1;
    CIEC_BOOL bTest2;

    //initial value must be 0 = false
    BOOST_CHECK_EQUAL(bTest1, false);

    bTest1 = CIEC_BOOL(true);
    bTest2 = bTest1;
    BOOST_CHECK_EQUAL(bTest1, true);
    BOOST_CHECK_EQUAL(bTest2, true);

    bTest1 = CIEC_BOOL(false);
    bTest2 = bTest1;
    BOOST_CHECK_EQUAL(bTest1, false);
    BOOST_CHECK_EQUAL(bTest2, false);

    //check that assignment operator does not intertwine objects
    bTest2 = CIEC_BOOL(true);
    BOOST_CHECK_EQUAL(bTest1, false);
    BOOST_CHECK_EQUAL(bTest2, true);

  }

  BOOST_AUTO_TEST_CASE(Conversion_test){
    CIEC_BOOL bTest;
    char cBuffer1[10];
    char cBuffer2[6];
    char cBuffer3[4];
    char cBuffer4[3];

    strcpy(cBuffer1, "");
    strcpy(cBuffer2, "");
    strcpy(cBuffer3, "");
    strcpy(cBuffer4, "");

    //check cast operator
    bTest = CIEC_BOOL(true);
    BOOST_CHECK_EQUAL(bTest.operator bool(), true);

    bTest = CIEC_BOOL(false);
    BOOST_CHECK_EQUAL(bTest.operator bool(), false);

    //check toString and fromString
    BOOST_CHECK_EQUAL(bTest.fromString("true"), 4);
    BOOST_CHECK_EQUAL(bTest, true);

    BOOST_CHECK_EQUAL(bTest.fromString("1"), 1);
    BOOST_CHECK_EQUAL(bTest, true);

    BOOST_CHECK_EQUAL(bTest.fromString("TRUE"), 4);
    BOOST_CHECK_EQUAL(bTest, true);

    BOOST_CHECK_EQUAL(bTest.fromString("BOOL#FALSE"), strlen("BOOL#FALSE"));
    BOOST_CHECK_EQUAL(bTest, false);

    BOOST_CHECK_EQUAL(bTest.fromString("BOOL#TRUE"), strlen("BOOL#TRUE"));
    BOOST_CHECK_EQUAL(bTest, true);

    BOOST_CHECK_EQUAL(bTest.toString(cBuffer1,10), 4);
    BOOST_CHECK_EQUAL(bTest.toString(cBuffer2,6), 4);
    BOOST_CHECK_EQUAL(bTest.toString(cBuffer3,4), -1);
    BOOST_CHECK_EQUAL(bTest.toString(cBuffer4,3), -1);

    BOOST_CHECK_EQUAL(strcmp(cBuffer1, "TRUE"), 0);
    BOOST_CHECK_EQUAL(strcmp(cBuffer2, "TRUE"), 0);

    strcpy(cBuffer1, "");
    strcpy(cBuffer2, "");
    strcpy(cBuffer3, "");
    strcpy(cBuffer4, "");

    BOOST_CHECK_EQUAL(bTest.fromString("FALSE"), 5);
    BOOST_CHECK_EQUAL(bTest, false);

    BOOST_CHECK_EQUAL(bTest.fromString("0"), 1);
    BOOST_CHECK_EQUAL(bTest, false);

    BOOST_CHECK_EQUAL(bTest.fromString("false"), 5);
    BOOST_CHECK_EQUAL(bTest, false);

    BOOST_CHECK_EQUAL(bTest.toString(cBuffer1,10), 5);
    BOOST_CHECK_EQUAL(bTest.toString(cBuffer2,6), 5);
    BOOST_CHECK_EQUAL(bTest.toString(cBuffer3,4), -1);
    BOOST_CHECK_EQUAL(bTest.toString(cBuffer4,3), -1);

    BOOST_CHECK_EQUAL(strcmp(cBuffer1, "FALSE"), 0);
    BOOST_CHECK_EQUAL(strcmp(cBuffer2, "FALSE"), 0);

    //check invalid fromString string

    BOOST_CHECK_EQUAL(bTest.fromString("NOT A VALID STRING"), -1);

  }
  BOOST_AUTO_TEST_SUITE_END()
