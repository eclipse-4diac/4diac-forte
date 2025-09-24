/*******************************************************************************
 * Copyright (c) 2022, 2025 Primetals Technologies Austria GmbH
 *                          Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Martin Melik Merkumians - initial API and implementation and/or initial documentation
 *   Martin Jobst - add user-defined literal tests
 *   Alois Zoitl  - migrated data type toString to std::string
 *******************************************************************************/
#include <boost/test/unit_test.hpp>
#include "forte_boost_output_support.h"

#include "forte/datatypes/forte_wchar.h"

namespace forte::test {
  BOOST_AUTO_TEST_SUITE(CIEC_WCHAR_function_test)

  BOOST_AUTO_TEST_CASE(Type_test) {
    CIEC_WCHAR test;
    // check type information
    BOOST_CHECK_EQUAL(test.getDataTypeID(), CIEC_ANY::e_WCHAR);
    // check operator bool data type size
    BOOST_CHECK_EQUAL(sizeof(test.operator TForteWChar()), sizeof(TForteWChar));
  }

  BOOST_AUTO_TEST_CASE(Literal_test) {
    CIEC_WCHAR test1 = u'a'_WCHAR;
    BOOST_TEST(static_cast<uint16_t>(static_cast<CIEC_WCHAR::TValueType>(test1)) == 'a');

    CIEC_WCHAR test2 = u'\0'_WCHAR;
    BOOST_TEST(static_cast<uint16_t>(static_cast<CIEC_WCHAR::TValueType>(test2)) == '\0');

    CIEC_WCHAR test3 = 0x30_WCHAR;
    BOOST_TEST(static_cast<uint16_t>(static_cast<CIEC_WCHAR::TValueType>(test3)) == 0x30);
  }

  BOOST_AUTO_TEST_CASE(Assignment_test) {
    CIEC_WCHAR test1;
    CIEC_WCHAR test2;

    // initial value must be 0
    BOOST_CHECK_EQUAL(static_cast<uint16_t>(static_cast<CIEC_WCHAR::TValueType>(test1)), 0);

    test1 = CIEC_WCHAR(0);
    test2 = test1;
    BOOST_CHECK_EQUAL(static_cast<uint16_t>(static_cast<CIEC_WCHAR::TValueType>(test1)), 0);
    BOOST_CHECK_EQUAL(static_cast<uint16_t>(static_cast<CIEC_WCHAR::TValueType>(test2)), 0);

    test1 = CIEC_WCHAR(12);
    test2 = test1;
    BOOST_CHECK_EQUAL(static_cast<uint16_t>(static_cast<CIEC_WCHAR::TValueType>(test1)), 12);
    BOOST_CHECK_EQUAL(static_cast<uint16_t>(static_cast<CIEC_WCHAR::TValueType>(test2)), 12);

    test1 = CIEC_WCHAR(255);
    test2 = test1;
    BOOST_CHECK_EQUAL(static_cast<uint16_t>(static_cast<CIEC_WCHAR::TValueType>(test1)), 255);
    BOOST_CHECK_EQUAL(static_cast<uint16_t>(static_cast<CIEC_WCHAR::TValueType>(test2)), 255);

    // check that assignment operator does not intertwine objects
    test2 = CIEC_WCHAR(128);
    BOOST_CHECK_EQUAL(static_cast<uint16_t>(static_cast<CIEC_WCHAR::TValueType>(test1)), 255);
    BOOST_CHECK_EQUAL(static_cast<uint16_t>(static_cast<CIEC_WCHAR::TValueType>(test2)), 128);
  }

  BOOST_AUTO_TEST_CASE(ToString_test) {
    CIEC_WCHAR test('a');
    std::string toStringBuffer;
    const char expected[] = "WCHAR#\"a\"";
    test.toString(toStringBuffer);
    BOOST_CHECK_EQUAL(toStringBuffer, expected);
  }

  BOOST_AUTO_TEST_CASE(ToStringEmpty_test) {
    CIEC_WCHAR test;
    std::string toStringBuffer;
    const char expected[] = "WCHAR#\"$0000\"";
    test.toString(toStringBuffer);
    BOOST_CHECK_EQUAL(toStringBuffer, expected);
  }

  BOOST_AUTO_TEST_CASE(ToStringDollar_test) {
    CIEC_WCHAR test('$');
    std::string toStringBuffer;
    const char expected[] = "WCHAR#\"$$\"";
    test.toString(toStringBuffer);
    BOOST_CHECK_EQUAL(toStringBuffer, expected);
  }

  BOOST_AUTO_TEST_CASE(ToStringSingleQuote_test) {
    CIEC_WCHAR test('"');
    std::string toStringBuffer;
    const char expected[] = "WCHAR#\"$\"\"";
    test.toString(toStringBuffer);
    BOOST_CHECK_EQUAL(toStringBuffer, expected);
  }

  BOOST_AUTO_TEST_CASE(ToStringLineFeed_NewLine_test) {
    CIEC_WCHAR test('\n');
    std::string toStringBuffer;
    const char expected[] = "WCHAR#\"$l\"";
    test.toString(toStringBuffer);
    BOOST_CHECK_EQUAL(toStringBuffer, expected);
  }

  BOOST_AUTO_TEST_CASE(ToStringPageFeed_test) {
    CIEC_WCHAR test('\f');
    std::string toStringBuffer;
    const char expected[] = "WCHAR#\"$p\"";
    test.toString(toStringBuffer);
    BOOST_CHECK_EQUAL(toStringBuffer, expected);
  }

  BOOST_AUTO_TEST_CASE(ToStringCarriageReturn_test) {
    CIEC_WCHAR test('\r');
    std::string toStringBuffer;
    const char expected[] = "WCHAR#\"$r\"";
    test.toString(toStringBuffer);
    BOOST_CHECK_EQUAL(toStringBuffer, expected);
  }

  BOOST_AUTO_TEST_CASE(ToStringTab_test) {
    CIEC_WCHAR test('\t');
    std::string toStringBuffer;
    const char expected[] = "WCHAR#\"$t\"";
    test.toString(toStringBuffer);
    BOOST_CHECK_EQUAL(toStringBuffer, expected);
  }

  BOOST_AUTO_TEST_CASE(FromString_test) {
    CIEC_WCHAR test;
    std::string toStringBuffer;
    const char expected[] = "WCHAR#\"A\"";
    BOOST_CHECK_EQUAL(test.fromString(expected), sizeof(expected) - 1);
    test.toString(toStringBuffer);
    BOOST_CHECK_EQUAL(toStringBuffer, expected);
  }

  BOOST_AUTO_TEST_CASE(FromStringEmpty_test) {
    CIEC_WCHAR test;
    std::string toStringBuffer;
    const char input[] = "WCHAR#\"\"";
    BOOST_CHECK_EQUAL(test.fromString(input), sizeof(input) - 1);
    test.toString(toStringBuffer);
    BOOST_CHECK_EQUAL(toStringBuffer, "WCHAR#\"$0000\"");
  }

  BOOST_AUTO_TEST_CASE(FromStringDollar_test) {
    CIEC_WCHAR test;
    std::string toStringBuffer;
    const char expected[] = "WCHAR#\"$$\"";
    BOOST_CHECK_EQUAL(test.fromString(expected), sizeof(expected) - 1);
    test.toString(toStringBuffer);
    BOOST_CHECK_EQUAL(toStringBuffer, expected);
  }

  BOOST_AUTO_TEST_CASE(FromStringQuote_test) {
    CIEC_WCHAR test;
    std::string toStringBuffer;
    const char expected[] = "WCHAR#\"$\"\"";
    BOOST_CHECK_EQUAL(test.fromString(expected), sizeof(expected) - 1);
    test.toString(toStringBuffer);
    BOOST_CHECK_EQUAL(toStringBuffer, expected);
  }

  BOOST_AUTO_TEST_CASE(FromStringLineFeed_test) {
    CIEC_WCHAR test;
    std::string toStringBuffer;
    const char source[] = "WCHAR#\"$L\"";
    const char expected[] = "WCHAR#\"$l\"";
    BOOST_CHECK_EQUAL(test.fromString(source), sizeof(source) - 1);
    test.toString(toStringBuffer);
    BOOST_CHECK_EQUAL(toStringBuffer, expected);
  }

  BOOST_AUTO_TEST_CASE(FromStringNewLine_test) {
    CIEC_WCHAR test;
    std::string toStringBuffer;
    const char source[] = "WCHAR#\"$N\"";
    const char expected[] = "WCHAR#\"$l\"";
    BOOST_CHECK_EQUAL(test.fromString(source), sizeof(source) - 1);
    test.toString(toStringBuffer);
    BOOST_CHECK_EQUAL(toStringBuffer, expected);
  }

  BOOST_AUTO_TEST_CASE(FromStringPageFeed_test) {
    CIEC_WCHAR test;
    std::string toStringBuffer;
    const char expected[] = "WCHAR#\"$p\"";
    BOOST_CHECK_EQUAL(test.fromString(expected), sizeof(expected) - 1);
    test.toString(toStringBuffer);
    BOOST_CHECK_EQUAL(toStringBuffer, expected);
  }

  BOOST_AUTO_TEST_CASE(FromStringCarriageReturn_test) {
    CIEC_WCHAR test;
    std::string toStringBuffer;
    const char expected[] = "WCHAR#\"$r\"";
    BOOST_CHECK_EQUAL(test.fromString(expected), sizeof(expected) - 1);
    test.toString(toStringBuffer);
    BOOST_CHECK_EQUAL(toStringBuffer, expected);
  }

  BOOST_AUTO_TEST_CASE(FromStringTab_test) {
    CIEC_WCHAR test;
    std::string toStringBuffer;
    const char expected[] = "WCHAR#\"$t\"";
    BOOST_CHECK_EQUAL(test.fromString(expected), sizeof(expected) - 1);
    test.toString(toStringBuffer);
    BOOST_CHECK_EQUAL(toStringBuffer, expected);
  }

  BOOST_AUTO_TEST_CASE(FromStringCodePoint_test) {
    CIEC_WCHAR test;
    std::string toStringBuffer;
    const char source[] = "WCHAR#\"$000A\"";
    const char expected[] = "WCHAR#\"$l\"";
    BOOST_CHECK_EQUAL(test.fromString(source), sizeof(source) - 1);
    test.toString(toStringBuffer);
    BOOST_CHECK_EQUAL(toStringBuffer, expected);
  }

  BOOST_AUTO_TEST_CASE(FromStringHighCodePoint_test) {
    CIEC_WCHAR test;
    std::string toStringBuffer;
    const char source[] = "WCHAR#\"$A00A\"";
    const char expected[] = "WCHAR#\"$A00A\"";
    BOOST_CHECK_EQUAL(test.fromString(source), sizeof(source) - 1);
    test.toString(toStringBuffer);
    BOOST_CHECK_EQUAL(toStringBuffer, expected);
  }

  BOOST_AUTO_TEST_SUITE_END()
} // namespace forte::test
