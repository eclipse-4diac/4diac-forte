/*******************************************************************************
 * Copyright (c) 2011, 2025 ACIN, nxtControl, Martin Erich Jobst,
 *                          Primetals Technologies Austria GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Martin Melik Merkumians, Ingo Hegny, Alois Zoitl, Stanislav Meduna
 *                - initial API and implementation and/or initial documentation
 *   Martin Jobst - add user-defined literal tests
 *   Alois Zoitl  - migrated data type toString to std::string
 *******************************************************************************/
#include <boost/test/unit_test.hpp>
#include "forte_boost_output_support.h"

#include "forte/datatypes/forte_any_int.h"
#include "forte/datatypes/forte_int.h"

namespace forte::test {
  BOOST_AUTO_TEST_SUITE(CIEC_INT_function_test)
  BOOST_AUTO_TEST_CASE(Type_test) {
    CIEC_INT nTest;
    // check type information
    BOOST_CHECK_EQUAL(nTest.getDataTypeID(), CIEC_ANY::e_INT);
    // check operator bool data type size
    BOOST_CHECK_EQUAL(sizeof(nTest.operator TForteInt16()), sizeof(TForteInt16));
  }

  BOOST_AUTO_TEST_CASE(Literal_test) {
    CIEC_INT test1 = 0_INT;
    BOOST_TEST(static_cast<CIEC_INT::TValueType>(test1) == 0);

    CIEC_INT test2 = 32767_INT;
    BOOST_TEST(static_cast<CIEC_INT::TValueType>(test2) == std::numeric_limits<CIEC_INT::TValueType>::max());

    CIEC_INT test3 = -32768_INT;
    BOOST_TEST(static_cast<CIEC_INT::TValueType>(test3) == std::numeric_limits<CIEC_INT::TValueType>::min());
  }

  BOOST_AUTO_TEST_CASE(Assignment_test) {
    CIEC_INT nTest1;
    CIEC_INT nTest2;

    // initial value must be 0
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(nTest1), 0);

    nTest1 = CIEC_INT(-32768);
    nTest2 = nTest1;
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(nTest1), -32768);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(nTest2), -32768);

    nTest1 = CIEC_INT(-25789);
    nTest2 = nTest1;
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(nTest1), -25789);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(nTest2), -25789);

    nTest1 = CIEC_INT(15748);
    nTest2 = nTest1;
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(nTest1), 15748);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(nTest2), 15748);

    nTest1 = CIEC_INT(32767);
    nTest2 = nTest1;
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(nTest1), 32767);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(nTest2), 32767);

    // check that assignment operator does not intertwine objects
    nTest2 = CIEC_INT(-36);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(nTest1), 32767);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(nTest2), -36);
  }

  BOOST_AUTO_TEST_CASE(Conversion_test) {
    CIEC_INT testVal;

    std::string buffer;

    // check cast operator
    testVal = CIEC_INT(0);

    BOOST_CHECK_EQUAL(testVal.operator TForteInt16(), 0);

    testVal = CIEC_INT(-32768);
    BOOST_CHECK_EQUAL(testVal.operator TForteInt16(), -32768);

    testVal = CIEC_INT(6984);
    BOOST_CHECK_EQUAL(testVal.operator TForteInt16(), 6984);

    testVal = CIEC_INT(32767);
    BOOST_CHECK_EQUAL(testVal.operator TForteInt16(), 32767);

    // check toString and fromString
    buffer.clear();

    BOOST_CHECK_EQUAL(testVal.fromString("-32768"), 6);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(testVal), -32768);
    BOOST_CHECK_EQUAL(testVal.fromString("INT#-32768"), 10);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(testVal), -32768);
    testVal.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "-32768");
    buffer.clear();
    testVal = CIEC_INT(0);

    BOOST_CHECK_EQUAL(testVal.fromString("2#1000000000000000"), 18);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(testVal), -32768);
    BOOST_CHECK_EQUAL(testVal.fromString("INT#2#1000000000000000"), 22);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(testVal), -32768);
    testVal.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "-32768");

    buffer.clear();
    testVal = CIEC_INT(0);

    BOOST_CHECK_EQUAL(testVal.fromString("8#100000"), 8);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(testVal), -32768);
    BOOST_CHECK_EQUAL(testVal.fromString("INT#8#100000"), 12);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(testVal), -32768);
    testVal.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "-32768");

    buffer.clear();
    testVal = CIEC_INT(0);

    BOOST_CHECK_EQUAL(testVal.fromString("16#8000"), 7);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(testVal), -32768);
    BOOST_CHECK_EQUAL(testVal.fromString("INT#16#8000"), 11);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(testVal), -32768);
    testVal.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "-32768");

    BOOST_CHECK_EQUAL(testVal.fromString("16#ffff"), 7);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(testVal), -1);

    buffer.clear();

    BOOST_CHECK_EQUAL(testVal.fromString("0"), 1);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(testVal), 0);
    testVal.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "0");

    buffer.clear();

    BOOST_CHECK_EQUAL(testVal.fromString("2#0"), 3);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(testVal), 0);
    BOOST_CHECK_EQUAL(testVal.fromString("INT#2#0"), 7);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(testVal), 0);
    testVal.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "0");

    buffer.clear();

    BOOST_CHECK_EQUAL(testVal.fromString("8#0"), 3);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(testVal), 0);
    BOOST_CHECK_EQUAL(testVal.fromString("INT#8#0"), 7);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(testVal), 0);
    testVal.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "0");

    buffer.clear();

    BOOST_CHECK_EQUAL(testVal.fromString("16#0"), 4);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(testVal), 0);
    testVal.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "0");

    buffer.clear();

    BOOST_CHECK_EQUAL(testVal.fromString("99"), 2);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(testVal), 99);
    BOOST_CHECK_EQUAL(testVal.fromString("INT#99"), 6);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(testVal), 99);
    testVal.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "99");

    buffer.clear();
    testVal = CIEC_INT(0);

    BOOST_CHECK_EQUAL(testVal.fromString("2#1100011"), 9);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(testVal), 99);
    testVal.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "99");

    buffer.clear();
    testVal = CIEC_INT(0);

    BOOST_CHECK_EQUAL(testVal.fromString("8#143"), 5);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(testVal), 99);
    testVal.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "99");

    buffer.clear();
    testVal = CIEC_INT(0);

    BOOST_CHECK_EQUAL(testVal.fromString("16#63"), 5);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(testVal), 99);
    testVal.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "99");

    buffer.clear();
    testVal = CIEC_INT(0);

    BOOST_CHECK_EQUAL(testVal.fromString("32767"), 5);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(testVal), 32767);
    testVal.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "32767");

    buffer.clear();
    testVal = CIEC_INT(0);

    BOOST_CHECK_EQUAL(testVal.fromString("2#0111111111111111"), 18);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(testVal), 32767);
    testVal.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "32767");

    buffer.clear();
    testVal = CIEC_INT(0);

    BOOST_CHECK_EQUAL(testVal.fromString("8#77777"), 7);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(testVal), 32767);
    testVal.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "32767");

    buffer.clear();
    testVal = CIEC_INT(0);

    BOOST_CHECK_EQUAL(testVal.fromString("16#7FFF"), 7);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(testVal), 32767);
    testVal.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "32767");

    buffer.clear();
    testVal = CIEC_INT(0);

    BOOST_CHECK_EQUAL(testVal.fromString("10#300"), 6);
    BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(testVal), 300);
    testVal.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "300");

    // testing values outside of allowed range
    BOOST_CHECK_EQUAL(testVal.fromString("40000"), -1);
    BOOST_CHECK_EQUAL(testVal.fromString("2#100101100100101101"), -1);
    BOOST_CHECK_EQUAL(testVal.fromString("8#1161007"), -1);
    BOOST_CHECK_EQUAL(testVal.fromString("16#FFFF0"), -1);
    BOOST_CHECK_EQUAL(testVal.fromString("-40000"), -1);

    // check invalid fromString string
    BOOST_CHECK_EQUAL(testVal.fromString("NOT A VALID STRING"), -1);
  }
  BOOST_AUTO_TEST_SUITE_END()
} // namespace forte::test
