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
#include "forte/datatypes/forte_sint.h"

namespace forte::test {
  BOOST_AUTO_TEST_SUITE(CIEC_SINT_function_test)
  BOOST_AUTO_TEST_CASE(Type_test) {
    CIEC_SINT nTest;
    // check type information
    BOOST_CHECK_EQUAL(nTest.getDataTypeID(), CIEC_ANY::e_SINT);
    // check operator bool data type size
    BOOST_CHECK_EQUAL(sizeof(nTest.operator TForteInt8()), sizeof(TForteInt8));
  }

  BOOST_AUTO_TEST_CASE(Literal_test) {
    CIEC_SINT test1 = 0_SINT;
    BOOST_TEST(static_cast<CIEC_SINT::TValueType>(test1) == 0);

    CIEC_SINT test2 = 127_SINT;
    BOOST_TEST(static_cast<CIEC_SINT::TValueType>(test2) == std::numeric_limits<CIEC_SINT::TValueType>::max());

    CIEC_SINT test3 = -128_SINT;
    BOOST_TEST(static_cast<CIEC_SINT::TValueType>(test3) == std::numeric_limits<CIEC_SINT::TValueType>::min());
  }

  BOOST_AUTO_TEST_CASE(Assignment_test) {
    CIEC_SINT nTest1;
    CIEC_SINT nTest2;

    // initial value must be 0
    BOOST_TEST(0 == static_cast<CIEC_SINT::TValueType>(nTest1));

    nTest1 = CIEC_SINT(-128);
    nTest2 = nTest1;
    BOOST_TEST(-128 == static_cast<CIEC_SINT::TValueType>(nTest1));
    BOOST_TEST(-128 == static_cast<CIEC_SINT::TValueType>(nTest2));

    nTest1 = CIEC_SINT(-65);
    nTest2 = nTest1;
    BOOST_TEST(-65 == static_cast<CIEC_SINT::TValueType>(nTest1));
    BOOST_TEST(-65 == static_cast<CIEC_SINT::TValueType>(nTest2));

    nTest1 = CIEC_SINT(98);
    nTest2 = nTest1;
    BOOST_TEST(98 == static_cast<CIEC_SINT::TValueType>(nTest1));
    BOOST_TEST(98 == static_cast<CIEC_SINT::TValueType>(nTest2));

    nTest1 = CIEC_SINT(127);
    nTest2 = nTest1;
    BOOST_TEST(127 == static_cast<CIEC_SINT::TValueType>(nTest1));
    BOOST_TEST(127 == static_cast<CIEC_SINT::TValueType>(nTest2));

    // check that assignment operator does not intertwine objects
    nTest2 = CIEC_SINT(-74);
    BOOST_TEST(127 == static_cast<CIEC_SINT::TValueType>(nTest1));
    BOOST_TEST(-74 == static_cast<CIEC_SINT::TValueType>(nTest2));
  }

  BOOST_AUTO_TEST_CASE(Conversion_test) {
    CIEC_SINT testVal;

    std::string buffer;

    // check cast operator
    testVal = CIEC_SINT(0);

    BOOST_CHECK_EQUAL(testVal.operator TForteInt8(), 0);

    testVal = CIEC_SINT(-128);
    BOOST_CHECK_EQUAL(testVal.operator TForteInt8(), -128);

    testVal = CIEC_SINT(53);
    BOOST_CHECK_EQUAL(testVal.operator TForteInt8(), 53);

    testVal = CIEC_SINT(127);
    BOOST_CHECK_EQUAL(testVal.operator TForteInt8(), 127);

    // check toString and fromString
    buffer.clear();

    BOOST_CHECK_EQUAL(testVal.fromString("-128"), 4);
    BOOST_CHECK_EQUAL(static_cast<CIEC_SINT::TValueType>(testVal), -128);
    testVal.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "-128");

    buffer.clear();
    testVal = CIEC_SINT(0);

    BOOST_CHECK_EQUAL(testVal.fromString("2#10000000"), 10);
    BOOST_CHECK_EQUAL(static_cast<CIEC_SINT::TValueType>(testVal), -128);
    testVal.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "-128");

    buffer.clear();
    testVal = CIEC_SINT(0);

    BOOST_CHECK_EQUAL(testVal.fromString("8#200"), 5);
    BOOST_CHECK_EQUAL(static_cast<CIEC_SINT::TValueType>(testVal), -128);
    testVal.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "-128");

    buffer.clear();
    testVal = CIEC_SINT(0);

    BOOST_CHECK_EQUAL(testVal.fromString("16#80"), 5);
    BOOST_CHECK_EQUAL(static_cast<CIEC_SINT::TValueType>(testVal), -128);
    testVal.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "-128");

    buffer.clear();
    testVal = CIEC_SINT(0);

    BOOST_CHECK_EQUAL(testVal.fromString("0"), 1);
    BOOST_CHECK_EQUAL(static_cast<CIEC_SINT::TValueType>(testVal), 0);
    testVal.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "0");

    buffer.clear();

    BOOST_CHECK_EQUAL(testVal.fromString("2#0"), 3);
    BOOST_CHECK_EQUAL(static_cast<CIEC_SINT::TValueType>(testVal), 0);
    testVal.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "0");

    buffer.clear();

    BOOST_CHECK_EQUAL(testVal.fromString("8#0"), 3);
    BOOST_CHECK_EQUAL(static_cast<CIEC_SINT::TValueType>(testVal), 0);
    testVal.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "0");

    buffer.clear();

    BOOST_CHECK_EQUAL(testVal.fromString("16#0"), 4);
    BOOST_CHECK_EQUAL(static_cast<CIEC_SINT::TValueType>(testVal), 0);
    testVal.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "0");

    buffer.clear();

    BOOST_CHECK_EQUAL(testVal.fromString("99"), 2);
    BOOST_CHECK_EQUAL(static_cast<CIEC_SINT::TValueType>(testVal), 99);
    testVal.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "99");

    buffer.clear();
    testVal = CIEC_SINT(0);

    BOOST_CHECK_EQUAL(testVal.fromString("2#1100011"), 9);
    BOOST_CHECK_EQUAL(static_cast<CIEC_SINT::TValueType>(testVal), 99);
    testVal.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "99");

    buffer.clear();
    testVal = CIEC_SINT(0);

    BOOST_CHECK_EQUAL(testVal.fromString("8#143"), 5);
    BOOST_CHECK_EQUAL(static_cast<CIEC_SINT::TValueType>(testVal), 99);
    testVal.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "99");

    buffer.clear();
    testVal = CIEC_SINT(0);

    BOOST_CHECK_EQUAL(testVal.fromString("16#63"), 5);
    BOOST_CHECK_EQUAL(static_cast<CIEC_SINT::TValueType>(testVal), 99);
    testVal.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "99");

    buffer.clear();
    testVal = CIEC_SINT(0);

    BOOST_CHECK_EQUAL(testVal.fromString("127"), 3);
    BOOST_CHECK_EQUAL(static_cast<CIEC_SINT::TValueType>(testVal), 127);
    testVal.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "127");

    buffer.clear();
    testVal = CIEC_SINT(0);

    BOOST_CHECK_EQUAL(testVal.fromString("2#01111111"), 10);
    BOOST_CHECK_EQUAL(static_cast<CIEC_SINT::TValueType>(testVal), 127);
    testVal.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "127");

    buffer.clear();
    testVal = CIEC_SINT(0);

    BOOST_CHECK_EQUAL(testVal.fromString("8#177"), 5);
    BOOST_CHECK_EQUAL(static_cast<CIEC_SINT::TValueType>(testVal), 127);
    testVal.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "127");

    buffer.clear();
    testVal = CIEC_SINT(0);

    BOOST_CHECK_EQUAL(testVal.fromString("16#7F"), 5);
    BOOST_CHECK_EQUAL(static_cast<CIEC_SINT::TValueType>(testVal), 127);
    testVal.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "127");

    buffer.clear();
    testVal = CIEC_SINT(0);

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
