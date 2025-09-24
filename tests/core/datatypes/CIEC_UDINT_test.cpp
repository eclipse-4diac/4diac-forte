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

#include "forte/datatypes/forte_udint.h"

namespace forte::test {
  BOOST_AUTO_TEST_SUITE(CIEC_UDINT_function_test)

  BOOST_AUTO_TEST_CASE(Type_test) {
    CIEC_UDINT nTest;
    // check type information
    BOOST_CHECK_EQUAL(nTest.getDataTypeID(), CIEC_ANY::e_UDINT);
    // check operator bool data type size
    BOOST_CHECK_EQUAL(sizeof(nTest.operator TForteUInt32()), sizeof(TForteUInt32));
  }

  BOOST_AUTO_TEST_CASE(Literal_test) {
    CIEC_UDINT test1 = 0_UDINT;
    BOOST_TEST(static_cast<CIEC_UDINT::TValueType>(test1) == 0);

    CIEC_UDINT test2 = 4294967295_UDINT;
    BOOST_TEST(static_cast<CIEC_UDINT::TValueType>(test2) == std::numeric_limits<CIEC_UDINT::TValueType>::max());
  }

  BOOST_AUTO_TEST_CASE(Assignment_test) {
    CIEC_UDINT nTest1;
    CIEC_UDINT nTest2;

    // initial value must be 0
    BOOST_CHECK_EQUAL(static_cast<CIEC_UDINT::TValueType>(nTest1), 0UL);

    nTest1 = CIEC_UDINT(0);
    nTest2 = nTest1;
    BOOST_CHECK_EQUAL(static_cast<CIEC_UDINT::TValueType>(nTest1), 0UL);
    BOOST_CHECK_EQUAL(static_cast<CIEC_UDINT::TValueType>(nTest2), 0UL);

    nTest1 = CIEC_UDINT(13546842UL);
    nTest2 = nTest1;
    BOOST_CHECK_EQUAL(static_cast<CIEC_UDINT::TValueType>(nTest1), 13546842UL);
    BOOST_CHECK_EQUAL(static_cast<CIEC_UDINT::TValueType>(nTest2), 13546842UL);

    nTest1 = CIEC_UDINT(4294967295UL);
    nTest2 = nTest1;
    BOOST_CHECK_EQUAL(static_cast<CIEC_UDINT::TValueType>(nTest1), 4294967295UL);
    BOOST_CHECK_EQUAL(static_cast<CIEC_UDINT::TValueType>(nTest2), 4294967295UL);

    // check that assignment operator does not intertwine objects
    nTest2 = CIEC_UDINT(951753258UL);
    BOOST_CHECK_EQUAL(static_cast<CIEC_UDINT::TValueType>(nTest1), 4294967295UL);
    BOOST_CHECK_EQUAL(static_cast<CIEC_UDINT::TValueType>(nTest2), 951753258UL);
  }

  BOOST_AUTO_TEST_CASE(Conversion_test) {
    CIEC_UDINT testVal;

    std::string buffer;

    // check cast operator
    testVal = CIEC_UDINT(0);

    BOOST_CHECK_EQUAL(testVal.operator TForteUInt32(), 0UL);

    testVal = CIEC_UDINT(654753258UL);
    BOOST_CHECK_EQUAL(testVal.operator TForteUInt32(), 654753258UL);

    testVal = CIEC_UDINT(4294967295UL);
    BOOST_CHECK_EQUAL(testVal.operator TForteUInt32(), 4294967295UL);

    // check toString and fromString
    BOOST_CHECK_EQUAL(testVal.fromString("0"), 1);
    BOOST_CHECK_EQUAL(static_cast<CIEC_UDINT::TValueType>(testVal), 0UL);
    testVal.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "0");

    buffer.clear();

    BOOST_CHECK_EQUAL(testVal.fromString("2#0"), 3);
    BOOST_CHECK_EQUAL(static_cast<CIEC_UDINT::TValueType>(testVal), 0UL);
    testVal.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "0");

    buffer.clear();

    BOOST_CHECK_EQUAL(testVal.fromString("8#0"), 3);
    BOOST_CHECK_EQUAL(static_cast<CIEC_UDINT::TValueType>(testVal), 0UL);
    testVal.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "0");

    buffer.clear();

    BOOST_CHECK_EQUAL(testVal.fromString("16#0"), 4);
    BOOST_CHECK_EQUAL(static_cast<CIEC_UDINT::TValueType>(testVal), 0UL);
    testVal.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "0");

    buffer.clear();

    BOOST_CHECK_EQUAL(testVal.fromString("147896321"), 9);
    BOOST_CHECK_EQUAL(static_cast<CIEC_UDINT::TValueType>(testVal), 147896321UL);
    testVal.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "147896321");

    buffer.clear();
    testVal = CIEC_UDINT(0);

    BOOST_CHECK_EQUAL(testVal.fromString("2#1000110100001011100000000001"), 30);
    BOOST_CHECK_EQUAL(static_cast<CIEC_UDINT::TValueType>(testVal), 147896321UL);
    testVal.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "147896321");

    buffer.clear();
    testVal = CIEC_UDINT(0);

    BOOST_CHECK_EQUAL(testVal.fromString("8#1064134001"), 12);
    BOOST_CHECK_EQUAL(static_cast<CIEC_UDINT::TValueType>(testVal), 147896321UL);
    testVal.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "147896321");

    buffer.clear();
    testVal = CIEC_UDINT(0);

    BOOST_CHECK_EQUAL(testVal.fromString("16#8D0B801"), 10);
    BOOST_CHECK_EQUAL(static_cast<CIEC_UDINT::TValueType>(testVal), 147896321UL);
    testVal.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "147896321");

    buffer.clear();
    testVal = CIEC_UDINT(0);

    BOOST_CHECK_EQUAL(testVal.fromString("4294967295"), 10);
    BOOST_CHECK_EQUAL(static_cast<CIEC_UDINT::TValueType>(testVal), 4294967295UL);
    testVal.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "4294967295");

    buffer.clear();
    testVal = CIEC_UDINT(0);

    BOOST_CHECK_EQUAL(testVal.fromString("2#11111111111111111111111111111111"), 34);
    BOOST_CHECK_EQUAL(static_cast<CIEC_UDINT::TValueType>(testVal), 4294967295UL);
    testVal.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "4294967295");

    buffer.clear();
    testVal = CIEC_UDINT(0);

    BOOST_CHECK_EQUAL(testVal.fromString("8#37777777777"), 13);
    BOOST_CHECK_EQUAL(static_cast<CIEC_UDINT::TValueType>(testVal), 4294967295UL);
    testVal.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "4294967295");

    buffer.clear();
    testVal = CIEC_UDINT(0);

    BOOST_CHECK_EQUAL(testVal.fromString("16#FFFFFFFF"), 11);
    BOOST_CHECK_EQUAL(static_cast<CIEC_UDINT::TValueType>(testVal), 4294967295UL);
    testVal.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "4294967295");

    buffer.clear();
    testVal = CIEC_UDINT(0);

    // testing values outside of allowed range
    BOOST_CHECK_EQUAL(testVal.fromString("42949672958"), -1);
    BOOST_CHECK_EQUAL(testVal.fromString("2#1111111111111111111111111111111100"), -1);
    BOOST_CHECK_EQUAL(testVal.fromString("8#377777777770"), -1);
    BOOST_CHECK_EQUAL(testVal.fromString("10#5294967295"), -1);
    BOOST_CHECK_EQUAL(testVal.fromString("16#FFFFFFFF0"), -1);
    BOOST_CHECK_EQUAL(testVal.fromString("-130"), -1);

    // check invalid fromString string
    BOOST_CHECK_EQUAL(testVal.fromString("NOT A VALID STRING"), -1);
  }
  BOOST_AUTO_TEST_SUITE_END()
} // namespace forte::test
