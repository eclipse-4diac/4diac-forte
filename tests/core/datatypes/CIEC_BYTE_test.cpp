/*******************************************************************************
 * Copyright (c) 2011, 2025 ACIN, Martin Erich Jobst,
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

#include "forte/datatypes/forte_byte.h"

namespace forte::test {
  BOOST_AUTO_TEST_SUITE(CIEC_BYTE_function_test)

  BOOST_AUTO_TEST_CASE(Type_test) {
    CIEC_BYTE nTest;
    // check type information
    BOOST_CHECK_EQUAL(nTest.getDataTypeID(), CIEC_ANY::e_BYTE);
    // check operator bool data type size
    BOOST_CHECK_EQUAL(sizeof(nTest.operator TForteByte()), sizeof(TForteByte));
  }

  BOOST_AUTO_TEST_CASE(Literal_test) {
    CIEC_BYTE test1 = 0x0_BYTE;
    BOOST_TEST(static_cast<CIEC_BYTE::TValueType>(test1) == 0);

    CIEC_BYTE test2 = 0xFF_BYTE;
    BOOST_TEST(static_cast<CIEC_BYTE::TValueType>(test2) == std::numeric_limits<CIEC_BYTE::TValueType>::max());
  }

  BOOST_AUTO_TEST_CASE(Assignment_test) {
    CIEC_BYTE nTest1;
    CIEC_BYTE nTest2;

    // initial value must be 0
    BOOST_CHECK_EQUAL(nTest1, 0);

    nTest1 = CIEC_BYTE(0);
    nTest2 = nTest1;
    BOOST_CHECK_EQUAL(nTest1, 0);
    BOOST_CHECK_EQUAL(nTest2, 0);

    nTest1 = CIEC_BYTE(12);
    nTest2 = nTest1;
    BOOST_CHECK_EQUAL(nTest1, 12);
    BOOST_CHECK_EQUAL(nTest2, 12);

    nTest1 = CIEC_BYTE(255);
    nTest2 = nTest1;
    BOOST_CHECK_EQUAL(nTest1, 255);
    BOOST_CHECK_EQUAL(nTest2, 255);

    // check that assignment operator does not intertwine objects
    nTest2 = CIEC_BYTE(128);
    BOOST_CHECK_EQUAL(nTest1, 255);
    BOOST_CHECK_EQUAL(nTest2, 128);
  }

  BOOST_AUTO_TEST_CASE(Conversion_test) {
    CIEC_BYTE testVal;

    std::string buffer;

    // check cast operator
    testVal = CIEC_BYTE(0);

    BOOST_CHECK_EQUAL(testVal.operator TForteByte(), 0);

    testVal = CIEC_BYTE(64);
    BOOST_CHECK_EQUAL(testVal.operator TForteByte(), 64);

    testVal = CIEC_BYTE(255);
    BOOST_CHECK_EQUAL(testVal.operator TForteByte(), 255);

    // check toString and fromString
    BOOST_CHECK_EQUAL(testVal.fromString("0"), 1);
    BOOST_CHECK_EQUAL(testVal, 0);
    testVal.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "0");

    buffer.clear();

    BOOST_CHECK_EQUAL(testVal.fromString("2#0"), 3);
    BOOST_CHECK_EQUAL(testVal, 0);
    testVal.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "0");

    buffer.clear();

    BOOST_CHECK_EQUAL(testVal.fromString("8#0"), 3);
    BOOST_CHECK_EQUAL(testVal, 0);
    testVal.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "0");

    buffer.clear();

    BOOST_CHECK_EQUAL(testVal.fromString("16#0"), 4);
    BOOST_CHECK_EQUAL(testVal, 0);
    testVal.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "0");

    buffer.clear();

    BOOST_CHECK_EQUAL(testVal.fromString("79"), 2);
    BOOST_CHECK_EQUAL(testVal, 79);
    testVal.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "79");

    buffer.clear();
    testVal = CIEC_BYTE(0);

    BOOST_CHECK_EQUAL(testVal.fromString("2#1001111"), 9);
    BOOST_CHECK_EQUAL(testVal, 79);
    testVal.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "79");

    buffer.clear();
    testVal = CIEC_BYTE(0);

    BOOST_CHECK_EQUAL(testVal.fromString("8#117"), 5);
    BOOST_CHECK_EQUAL(testVal, 79);
    testVal.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "79");

    buffer.clear();
    testVal = CIEC_BYTE(0);

    BOOST_CHECK_EQUAL(testVal.fromString("16#4F"), 5);
    BOOST_CHECK_EQUAL(testVal, 79);
    testVal.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "79");

    buffer.clear();
    testVal = CIEC_BYTE(0);

    BOOST_CHECK_EQUAL(testVal.fromString("255"), 3);
    BOOST_CHECK_EQUAL(testVal, 255);
    testVal.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "255");

    buffer.clear();
    testVal = CIEC_BYTE(0);

    BOOST_CHECK_EQUAL(testVal.fromString("2#11111111"), 10);
    BOOST_CHECK_EQUAL(testVal, 255);
    testVal.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "255");

    buffer.clear();
    testVal = CIEC_BYTE(0);

    BOOST_CHECK_EQUAL(testVal.fromString("8#377"), 5);
    BOOST_CHECK_EQUAL(testVal, 255);
    testVal.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "255");

    buffer.clear();
    testVal = CIEC_BYTE(0);

    BOOST_CHECK_EQUAL(testVal.fromString("16#FF"), 5);
    BOOST_CHECK_EQUAL(testVal, 255);
    testVal.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "255");

    buffer.clear();
    testVal = CIEC_BYTE(0);

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
