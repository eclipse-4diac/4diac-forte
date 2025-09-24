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
#include "forte/datatypes/forte_uint.h"

namespace forte::test {
  BOOST_AUTO_TEST_SUITE(CIEC_UINT_function_test)

  BOOST_AUTO_TEST_CASE(Type_test) {
    CIEC_UINT testVal;
    // check type information
    BOOST_CHECK_EQUAL(testVal.getDataTypeID(), CIEC_ANY::e_UINT);
    // check operator bool data type size
    BOOST_CHECK_EQUAL(sizeof(testVal.operator TForteUInt16()), sizeof(TForteUInt16));
  }

  BOOST_AUTO_TEST_CASE(Literal_test) {
    CIEC_UINT test1 = 0_UINT;
    BOOST_TEST(static_cast<CIEC_UINT::TValueType>(test1) == 0);

    CIEC_UINT test2 = 65535_UINT;
    BOOST_TEST(static_cast<CIEC_UINT::TValueType>(test2) == std::numeric_limits<CIEC_UINT::TValueType>::max());
  }

  BOOST_AUTO_TEST_CASE(Assignment_test) {
    CIEC_UINT testVal1;
    CIEC_UINT testVal2;

    // initial value must be 0
    BOOST_CHECK_EQUAL(static_cast<CIEC_UINT::TValueType>(testVal1), 0);

    testVal1 = CIEC_UINT(0);
    testVal2 = testVal1;
    BOOST_CHECK_EQUAL(static_cast<CIEC_UINT::TValueType>(testVal1), 0);
    BOOST_CHECK_EQUAL(static_cast<CIEC_UINT::TValueType>(testVal2), 0);

    testVal1 = CIEC_UINT(23584);
    testVal2 = testVal1;
    BOOST_CHECK_EQUAL(static_cast<CIEC_UINT::TValueType>(testVal1), 23584);
    BOOST_CHECK_EQUAL(static_cast<CIEC_UINT::TValueType>(testVal2), 23584);

    testVal1 = CIEC_UINT(65535);
    testVal2 = testVal1;
    BOOST_CHECK_EQUAL(static_cast<CIEC_UINT::TValueType>(testVal1), 65535);
    BOOST_CHECK_EQUAL(static_cast<CIEC_UINT::TValueType>(testVal2), 65535);

    // check that assignment operator does not intertwine objects
    testVal2 = CIEC_UINT(12800);
    BOOST_CHECK_EQUAL(static_cast<CIEC_UINT::TValueType>(testVal1), 65535);
    BOOST_CHECK_EQUAL(static_cast<CIEC_UINT::TValueType>(testVal2), 12800);
  }

  BOOST_AUTO_TEST_CASE(Conversion_test) {
    CIEC_UINT testVal;

    std::string buffer;

    // check cast operator
    testVal = CIEC_UINT(0);

    BOOST_CHECK_EQUAL(testVal.operator TForteUInt16(), 0);

    testVal = CIEC_UINT(2453);
    BOOST_CHECK_EQUAL(testVal.operator TForteUInt16(), 2453);

    testVal = CIEC_UINT(65535);
    BOOST_CHECK_EQUAL(testVal.operator TForteUInt16(), 65535);

    // check toString and fromString
    BOOST_CHECK_EQUAL(testVal.fromString("0"), 1);
    BOOST_CHECK_EQUAL(static_cast<CIEC_UINT::TValueType>(testVal), 0);
    testVal.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "0");

    buffer.clear();

    BOOST_CHECK_EQUAL(testVal.fromString("2#0"), 3);
    BOOST_CHECK_EQUAL(static_cast<CIEC_UINT::TValueType>(testVal), 0);
    testVal.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "0");

    buffer.clear();

    BOOST_CHECK_EQUAL(testVal.fromString("8#0"), 3);
    BOOST_CHECK_EQUAL(static_cast<CIEC_UINT::TValueType>(testVal), 0);
    testVal.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "0");

    buffer.clear();

    BOOST_CHECK_EQUAL(testVal.fromString("16#0"), 4);
    BOOST_CHECK_EQUAL(static_cast<CIEC_UINT::TValueType>(testVal), 0);
    testVal.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "0");

    buffer.clear();

    BOOST_CHECK_EQUAL(testVal.fromString("38549"), 5);
    BOOST_CHECK_EQUAL(static_cast<CIEC_UINT::TValueType>(testVal), 38549);
    testVal.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "38549");

    buffer.clear();
    testVal = CIEC_UINT(0);

    BOOST_CHECK_EQUAL(testVal.fromString("2#1001011010010101"), 18);
    BOOST_CHECK_EQUAL(static_cast<CIEC_UINT::TValueType>(testVal), 38549);
    testVal.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "38549");

    buffer.clear();
    testVal = CIEC_UINT(0);

    BOOST_CHECK_EQUAL(testVal.fromString("8#113225"), 8);
    BOOST_CHECK_EQUAL(static_cast<CIEC_UINT::TValueType>(testVal), 38549);
    testVal.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "38549");

    buffer.clear();
    testVal = CIEC_UINT(0);

    BOOST_CHECK_EQUAL(testVal.fromString("16#9695"), 7);
    BOOST_CHECK_EQUAL(static_cast<CIEC_UINT::TValueType>(testVal), 38549);
    testVal.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "38549");

    buffer.clear();
    testVal = CIEC_UINT(0);

    BOOST_CHECK_EQUAL(testVal.fromString("65535"), 5);
    BOOST_CHECK_EQUAL(static_cast<CIEC_UINT::TValueType>(testVal), 65535);
    testVal.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "65535");

    buffer.clear();
    testVal = CIEC_UINT(0);

    BOOST_CHECK_EQUAL(testVal.fromString("2#1111111111111111"), 18);
    BOOST_CHECK_EQUAL(static_cast<CIEC_UINT::TValueType>(testVal), 65535);
    testVal.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "65535");

    buffer.clear();
    testVal = CIEC_UINT(0);

    BOOST_CHECK_EQUAL(testVal.fromString("8#177777"), 8);
    BOOST_CHECK_EQUAL(static_cast<CIEC_UINT::TValueType>(testVal), 65535);
    testVal.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "65535");

    buffer.clear();
    testVal = CIEC_UINT(0);

    BOOST_CHECK_EQUAL(testVal.fromString("16#FFFF"), 7);
    BOOST_CHECK_EQUAL(static_cast<CIEC_UINT::TValueType>(testVal), 65535);
    testVal.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "65535");

    buffer.clear();
    testVal = CIEC_UINT(0);

    // testing values outside of allowed range
    BOOST_CHECK_EQUAL(testVal.fromString("70000"), -1);
    BOOST_CHECK_EQUAL(testVal.fromString("2#111111111111111100"), -1);
    BOOST_CHECK_EQUAL(testVal.fromString("8#200000"), -1);
    BOOST_CHECK_EQUAL(testVal.fromString("10#70000"), -1);
    BOOST_CHECK_EQUAL(testVal.fromString("16#FFFF0"), -1);
    BOOST_CHECK_EQUAL(testVal.fromString("-130"), -1);

    // check invalid fromString string
    BOOST_CHECK_EQUAL(testVal.fromString("NOT A VALID STRING"), -1);
  }
  BOOST_AUTO_TEST_SUITE_END()
} // namespace forte::test
