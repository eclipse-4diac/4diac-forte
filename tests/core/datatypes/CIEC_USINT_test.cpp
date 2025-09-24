/*******************************************************************************
 * Copyright (c) 2011, 2025 ACIN, Martin Erich Jobst,
 *                          Primetals Technologies Austria GmbH
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

#include "forte/datatypes/forte_any_int.h"
#include "forte/datatypes/forte_usint.h"

namespace forte::test {
  BOOST_AUTO_TEST_SUITE(CIEC_USINT_function_test)

  BOOST_AUTO_TEST_CASE(Type_test) {
    CIEC_USINT nTest;
    // check type information
    BOOST_CHECK_EQUAL(nTest.getDataTypeID(), CIEC_ANY::e_USINT);
    // check operator bool data type size
    BOOST_CHECK_EQUAL(sizeof(nTest.operator TForteUInt8()), sizeof(TForteUInt8));
  }

  BOOST_AUTO_TEST_CASE(Literal_test) {
    CIEC_USINT test1 = 0_USINT;
    BOOST_TEST(static_cast<CIEC_USINT::TValueType>(test1) == 0);

    CIEC_USINT test2 = 255_USINT;
    BOOST_TEST(static_cast<CIEC_USINT::TValueType>(test2) == std::numeric_limits<CIEC_USINT::TValueType>::max());
  }

  BOOST_AUTO_TEST_CASE(Assignment_test) {
    CIEC_USINT nTest1;
    CIEC_USINT nTest2;

    // initial value must be 0
    BOOST_CHECK_EQUAL(static_cast<CIEC_USINT::TValueType>(nTest1), 0U);

    nTest1 = CIEC_USINT(0U);
    nTest2 = nTest1;
    BOOST_CHECK_EQUAL(static_cast<CIEC_USINT::TValueType>(nTest1), 0U);
    BOOST_CHECK_EQUAL(static_cast<CIEC_USINT::TValueType>(nTest2), 0U);

    nTest1 = CIEC_USINT(12U);
    nTest2 = nTest1;
    BOOST_CHECK_EQUAL(static_cast<CIEC_USINT::TValueType>(nTest1), 12U);
    BOOST_CHECK_EQUAL(static_cast<CIEC_USINT::TValueType>(nTest2), 12U);

    nTest1 = CIEC_USINT(255U);
    nTest2 = nTest1;
    BOOST_CHECK_EQUAL(static_cast<CIEC_USINT::TValueType>(nTest1), 255U);
    BOOST_CHECK_EQUAL(static_cast<CIEC_USINT::TValueType>(nTest2), 255U);

    // check that assignment operator does not intertwine objects
    nTest2 = CIEC_USINT(128U);
    BOOST_CHECK_EQUAL(static_cast<CIEC_USINT::TValueType>(nTest1), 255U);
    BOOST_CHECK_EQUAL(static_cast<CIEC_USINT::TValueType>(nTest2), 128U);
  }

  BOOST_AUTO_TEST_CASE(Conversion_test) {
    CIEC_USINT testVal;

    std::string buffer;

    // check cast operator
    testVal = CIEC_USINT(0);

    BOOST_CHECK_EQUAL(testVal.operator TForteUInt8(), 0U);

    testVal = CIEC_USINT(64U);
    BOOST_CHECK_EQUAL(testVal.operator TForteUInt8(), 64U);

    testVal = CIEC_USINT(255U);
    BOOST_CHECK_EQUAL(testVal.operator TForteUInt8(), 255U);

    // check toString and fromString
    BOOST_CHECK_EQUAL(testVal.fromString("0"), 1);
    BOOST_CHECK_EQUAL(static_cast<CIEC_USINT::TValueType>(testVal), 0U);
    testVal.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "0");
    buffer.clear();

    BOOST_CHECK_EQUAL(testVal.fromString("2#0"), 3);
    BOOST_CHECK_EQUAL(static_cast<CIEC_USINT::TValueType>(testVal), 0U);
    testVal.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "0");
    buffer.clear();

    BOOST_CHECK_EQUAL(testVal.fromString("8#0"), 3);
    BOOST_CHECK_EQUAL(static_cast<CIEC_USINT::TValueType>(testVal), 0U);
    testVal.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "0");
    buffer.clear();

    BOOST_CHECK_EQUAL(testVal.fromString("16#0"), 4);
    BOOST_CHECK_EQUAL(static_cast<CIEC_USINT::TValueType>(testVal), 0U);
    testVal.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "0");

    buffer.clear();

    BOOST_CHECK_EQUAL(testVal.fromString("79"), 2);
    BOOST_CHECK_EQUAL(static_cast<CIEC_USINT::TValueType>(testVal), 79U);
    testVal.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "79");

    buffer.clear();
    testVal = CIEC_USINT(0);

    BOOST_CHECK_EQUAL(testVal.fromString("2#1001111"), 9);
    BOOST_CHECK_EQUAL(static_cast<CIEC_USINT::TValueType>(testVal), 79U);
    testVal.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "79");

    buffer.clear();
    testVal = CIEC_USINT(0);

    BOOST_CHECK_EQUAL(testVal.fromString("8#117"), 5);
    BOOST_CHECK_EQUAL(static_cast<CIEC_USINT::TValueType>(testVal), 79U);
    testVal.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "79");

    buffer.clear();
    testVal = CIEC_USINT(0);

    BOOST_CHECK_EQUAL(testVal.fromString("16#4F"), 5);
    BOOST_CHECK_EQUAL(static_cast<CIEC_USINT::TValueType>(testVal), 79U);
    testVal.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "79");

    buffer.clear();
    testVal = CIEC_USINT(0);

    BOOST_CHECK_EQUAL(testVal.fromString("255"), 3);
    BOOST_CHECK_EQUAL(static_cast<CIEC_USINT::TValueType>(testVal), 255U);
    testVal.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "255");

    buffer.clear();
    testVal = CIEC_USINT(0);

    BOOST_CHECK_EQUAL(testVal.fromString("2#11111111"), 10);
    BOOST_CHECK_EQUAL(static_cast<CIEC_USINT::TValueType>(testVal), 255U);
    testVal.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "255");

    buffer.clear();
    testVal = CIEC_USINT(0);

    BOOST_CHECK_EQUAL(testVal.fromString("8#377"), 5);
    BOOST_CHECK_EQUAL(static_cast<CIEC_USINT::TValueType>(testVal), 255U);
    testVal.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "255");

    buffer.clear();
    testVal = CIEC_USINT(0);

    BOOST_CHECK_EQUAL(testVal.fromString("16#FF"), 5);
    BOOST_CHECK_EQUAL(static_cast<CIEC_USINT::TValueType>(testVal), 255U);
    testVal.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "255");

    buffer.clear();
    testVal = CIEC_USINT(0);

    // testing values outside of allowed range
    BOOST_CHECK_EQUAL(testVal.fromString("300"), -1);
    BOOST_CHECK_EQUAL(testVal.fromString("2#100101100"), -1);
    BOOST_CHECK_EQUAL(testVal.fromString("8#454"), -1);
    BOOST_CHECK_EQUAL(testVal.fromString("10#300"), -1);
    BOOST_CHECK_EQUAL(testVal.fromString("16#12C"), -1);
    BOOST_CHECK_EQUAL(testVal.fromString("-130"), -1);

    // check invalid fromString string
    BOOST_CHECK_EQUAL(testVal.fromString("NOT A VALID STRING"), -1);
  }
  BOOST_AUTO_TEST_SUITE_END()
} // namespace forte::test
