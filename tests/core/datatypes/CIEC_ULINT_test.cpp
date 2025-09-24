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
#include "forte/datatypes/forte_ulint.h"

namespace forte::test {
  BOOST_AUTO_TEST_SUITE(CIEC_ULINT_function_test)

  BOOST_AUTO_TEST_CASE(Type_test) {
    CIEC_ULINT nTest;
    // check type information
    BOOST_CHECK_EQUAL(nTest.getDataTypeID(), CIEC_ANY::e_ULINT);
    // check operator bool data type size
    BOOST_CHECK_EQUAL(sizeof(nTest.operator TForteUInt64()), sizeof(TForteUInt64));
  }

  BOOST_AUTO_TEST_CASE(Literal_test) {
    CIEC_ULINT test1 = 0_ULINT;
    BOOST_TEST(static_cast<CIEC_ULINT::TValueType>(test1) == 0);

    CIEC_ULINT test2 = 18446744073709551615_ULINT;
    BOOST_TEST(static_cast<CIEC_ULINT::TValueType>(test2) == std::numeric_limits<CIEC_ULINT::TValueType>::max());
  }

  BOOST_AUTO_TEST_CASE(Assignment_test) {
    CIEC_ULINT nTest1;
    CIEC_ULINT nTest2;

    // initial value must be 0
    BOOST_CHECK_EQUAL(static_cast<CIEC_ULINT::TValueType>(nTest1), 0ULL);

    nTest1 = CIEC_ULINT(0ULL);
    nTest2 = nTest1;
    BOOST_CHECK_EQUAL(static_cast<CIEC_ULINT::TValueType>(nTest1), 0ULL);
    BOOST_CHECK_EQUAL(static_cast<CIEC_ULINT::TValueType>(nTest2), 0ULL);

    nTest1 = CIEC_ULINT(13546842ULL);
    nTest2 = nTest1;
    BOOST_CHECK_EQUAL(static_cast<CIEC_ULINT::TValueType>(nTest1), 13546842ULL);
    BOOST_CHECK_EQUAL(static_cast<CIEC_ULINT::TValueType>(nTest2), 13546842ULL);

    nTest1 = CIEC_ULINT(18446744073709551615ULL);
    nTest2 = nTest1;
    BOOST_CHECK_EQUAL(static_cast<CIEC_ULINT::TValueType>(nTest1), 18446744073709551615ULL);
    BOOST_CHECK_EQUAL(static_cast<CIEC_ULINT::TValueType>(nTest2), 18446744073709551615ULL);

    // check that assignment operator does not intertwine objects
    nTest2 = CIEC_ULINT(3652584218745ULL);
    BOOST_CHECK_EQUAL(static_cast<CIEC_ULINT::TValueType>(nTest1), 18446744073709551615ULL);
    BOOST_CHECK_EQUAL(static_cast<CIEC_ULINT::TValueType>(nTest2), 3652584218745ULL);
  }

  BOOST_AUTO_TEST_CASE(Conversion_test) {
    CIEC_ULINT testVal;

    std::string buffer;

    // check cast operator
    testVal = CIEC_ULINT(0);

    BOOST_CHECK_EQUAL(testVal.operator TForteUInt64(), 0ULL);

    testVal = CIEC_ULINT(5874125369851ULL);
    BOOST_CHECK_EQUAL(testVal.operator TForteUInt64(), 5874125369851ULL);

    testVal = CIEC_ULINT(18446744073709551615ULL);
    BOOST_CHECK_EQUAL(testVal.operator TForteUInt64(), 18446744073709551615ULL);

    // check toString and fromString
    BOOST_CHECK_EQUAL(testVal.fromString("0"), 1);
    BOOST_CHECK_EQUAL(static_cast<CIEC_ULINT::TValueType>(testVal), 0ULL);
    testVal.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "0");
    buffer.clear();

    BOOST_CHECK_EQUAL(testVal.fromString("2#0"), 3);
    BOOST_CHECK_EQUAL(static_cast<CIEC_ULINT::TValueType>(testVal), 0ULL);
    testVal.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "0");
    buffer.clear();

    BOOST_CHECK_EQUAL(testVal.fromString("8#0"), 3);
    BOOST_CHECK_EQUAL(static_cast<CIEC_ULINT::TValueType>(testVal), 0ULL);
    testVal.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "0");
    buffer.clear();

    BOOST_CHECK_EQUAL(testVal.fromString("16#0"), 4);
    BOOST_CHECK_EQUAL(static_cast<CIEC_ULINT::TValueType>(testVal), 0ULL);
    testVal.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "0");
    buffer.clear();

    BOOST_CHECK_EQUAL(testVal.fromString("62584325147"), 11);
    BOOST_CHECK_EQUAL(static_cast<CIEC_ULINT::TValueType>(testVal), 62584325147ULL);
    testVal.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "62584325147");
    buffer.clear();
    testVal = CIEC_ULINT(0);

    BOOST_CHECK_EQUAL(testVal.fromString("2#111010010010010100010000010000011011"), 38);
    BOOST_CHECK_EQUAL(static_cast<CIEC_ULINT::TValueType>(testVal), 62584325147ULL);
    testVal.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "62584325147");
    buffer.clear();
    testVal = CIEC_ULINT(0);

    BOOST_CHECK_EQUAL(testVal.fromString("8#722224202033"), 14);
    BOOST_CHECK_EQUAL(static_cast<CIEC_ULINT::TValueType>(testVal), 62584325147ULL);
    testVal.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "62584325147");
    buffer.clear();
    testVal = CIEC_ULINT(0);

    BOOST_CHECK_EQUAL(testVal.fromString("16#E9251041B"), 12);
    BOOST_CHECK_EQUAL(static_cast<CIEC_ULINT::TValueType>(testVal), 62584325147ULL);
    testVal.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "62584325147");
    buffer.clear();
    testVal = CIEC_ULINT(0);

    BOOST_CHECK_EQUAL(testVal.fromString("18446744073709551615"), 20);
    BOOST_CHECK_EQUAL(static_cast<CIEC_ULINT::TValueType>(testVal), 18446744073709551615ULL);
    testVal.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "18446744073709551615");
    buffer.clear();
    testVal = CIEC_ULINT(0);

    BOOST_CHECK_EQUAL(testVal.fromString("2#1111111111111111111111111111111111111111111111111111111111111111"), 66);
    BOOST_CHECK_EQUAL(static_cast<CIEC_ULINT::TValueType>(testVal), 18446744073709551615ULL);
    testVal.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "18446744073709551615");
    buffer.clear();
    testVal = CIEC_ULINT(0);

    BOOST_CHECK_EQUAL(testVal.fromString("8#1777777777777777777777"), 24);
    BOOST_CHECK_EQUAL(static_cast<CIEC_ULINT::TValueType>(testVal), 18446744073709551615ULL);
    testVal.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "18446744073709551615");
    buffer.clear();
    testVal = CIEC_ULINT(0);

    BOOST_CHECK_EQUAL(testVal.fromString("16#FFFFFFFFFFFFFFFF"), 19);
    BOOST_CHECK_EQUAL(static_cast<CIEC_ULINT::TValueType>(testVal), 18446744073709551615ULL);
    testVal.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "18446744073709551615");
    buffer.clear();
    testVal = CIEC_ULINT(0);

    // testing values outside of allowed range
    BOOST_CHECK_EQUAL(testVal.fromString("184467440737095516159"), -1);
    BOOST_CHECK_EQUAL(testVal.fromString("2#11111111111111111111111111111111111111111111111111111111111111110"), -1);
    BOOST_CHECK_EQUAL(testVal.fromString("8#17777777777777777777770"), -1);
    BOOST_CHECK_EQUAL(testVal.fromString("10#92233720368547758076"), -1);
    BOOST_CHECK_EQUAL(testVal.fromString("16#FFFFFFFFFFFFFFFF0"), -1);
    BOOST_CHECK_EQUAL(testVal.fromString("-130"), -1);

    // check invalid fromString string
    BOOST_CHECK_EQUAL(testVal.fromString("NOT A VALID STRING"), -1);
  }
  BOOST_AUTO_TEST_SUITE_END()
} // namespace forte::test
