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

  BOOST_AUTO_TEST_CASE(Assignment_test){
    CIEC_BOOL bTest1;
    CIEC_BOOL bTest2;

    //initial value must be 0 = false
    BOOST_CHECK_EQUAL(bTest1, false);

    bTest1 = true;
    bTest2 = bTest1;
    BOOST_CHECK_EQUAL(bTest1, true);
    BOOST_CHECK_EQUAL(bTest2, true);

    bTest1 = false;
    bTest2 = bTest1;
    BOOST_CHECK_EQUAL(bTest1, false);
    BOOST_CHECK_EQUAL(bTest2, false);

    //check that assignment operator does not intertwine objects
    bTest2 = true;
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
    bTest = true;
    BOOST_CHECK_EQUAL(bTest.operator bool(), true);

    bTest = false;
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
