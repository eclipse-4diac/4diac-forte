/*******************************************************************************
 * Copyright (c) 2011, 2025 ACIN, fortiss GmbH, Martin Erich Jobst,
 *                          Primetals Technologies Ausria GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Martin Melik Merkumians, Ingo Hegny, Alois Zoitl
 *                - initial API and implementation and/or initial documentation
 *   Martin Jobst - add user-defined literal tests
 *   Alois Zoitl  - migrated data type toString to std::string
 *******************************************************************************/
#include <boost/test/unit_test.hpp>
#include "forte_boost_output_support.h"

#include "forte/datatypes/forte_bool.h"

using namespace boost::unit_test;

namespace forte::test {
  BOOST_AUTO_TEST_SUITE(CIEC_BOOL_function_test)

  BOOST_AUTO_TEST_CASE(Type_test) {
    CIEC_BOOL bTest;
    // check type information
    BOOST_CHECK_EQUAL(bTest.getDataTypeID(), CIEC_ANY::e_BOOL);
    // check operator bool data type size
    BOOST_CHECK_EQUAL(sizeof(bTest.operator bool()), sizeof(bool));
  }

  BOOST_AUTO_TEST_CASE(Literal_test) {
    CIEC_BOOL test1 = 0_BOOL;
    BOOST_TEST(static_cast<CIEC_BOOL::TValueType>(test1) == false);

    CIEC_BOOL test2 = 1_BOOL;
    BOOST_TEST(static_cast<CIEC_BOOL::TValueType>(test2) == true);

    CIEC_BOOL test3 = false_BOOL;
    BOOST_TEST(static_cast<CIEC_BOOL::TValueType>(test3) == false);

    CIEC_BOOL test4 = true_BOOL;
    BOOST_TEST(static_cast<CIEC_BOOL::TValueType>(test4) == true);
  }

  BOOST_AUTO_TEST_CASE(Assignment_test) {
    CIEC_BOOL bTest1;
    CIEC_BOOL bTest2;

    // initial value must be 0 = false
    BOOST_CHECK_EQUAL(bTest1, false);

    bTest1 = true_BOOL;
    bTest2 = bTest1;
    BOOST_CHECK_EQUAL(bTest1, true);
    BOOST_CHECK_EQUAL(bTest2, true);

    bTest1 = false_BOOL;
    bTest2 = bTest1;
    BOOST_CHECK_EQUAL(bTest1, false);
    BOOST_CHECK_EQUAL(bTest2, false);

    // check that assignment operator does not intertwine objects
    bTest2 = true_BOOL;
    BOOST_CHECK_EQUAL(bTest1, false);
    BOOST_CHECK_EQUAL(bTest2, true);
  }

  BOOST_AUTO_TEST_CASE(Conversion_test) {
    CIEC_BOOL bTest;
    std::string cBuffer1;
    std::string cBuffer2;

    // check cast operator
    bTest = true_BOOL;
    BOOST_CHECK_EQUAL(bTest.operator bool(), true);

    bTest = false_BOOL;
    BOOST_CHECK_EQUAL(bTest.operator bool(), false);

    // check toString and fromString
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

    bTest.toString(cBuffer1);
    bTest.toString(cBuffer2);

    BOOST_CHECK_EQUAL(cBuffer1, "TRUE");
    BOOST_CHECK_EQUAL(cBuffer2, "TRUE");

    cBuffer1.clear();
    cBuffer2.clear();

    BOOST_CHECK_EQUAL(bTest.fromString("FALSE"), 5);
    BOOST_CHECK_EQUAL(bTest, false);

    BOOST_CHECK_EQUAL(bTest.fromString("0"), 1);
    BOOST_CHECK_EQUAL(bTest, false);

    BOOST_CHECK_EQUAL(bTest.fromString("false"), 5);
    BOOST_CHECK_EQUAL(bTest, false);

    bTest.toString(cBuffer1);
    bTest.toString(cBuffer2);

    BOOST_CHECK_EQUAL(cBuffer1, "FALSE");
    BOOST_CHECK_EQUAL(cBuffer2, "FALSE");

    // check invalid fromString string

    BOOST_CHECK_EQUAL(bTest.fromString("NOT A VALID STRING"), -1);
  }
  BOOST_AUTO_TEST_SUITE_END()
} // namespace forte::test
