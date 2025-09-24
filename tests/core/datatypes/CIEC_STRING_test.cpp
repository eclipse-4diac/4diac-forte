/*******************************************************************************
 * Copyright (c) 2011, 2025 ACIN, nxtControl, Profactor GmbH, fortiss GmbH,
 *                          TU Wien/ACIN, Primetals Technologies Austria GmbH,
 *                          Martin Erich Jobst
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Martin Melik Merkumians, Ingo Hegny, Alois Zoitl, Stanislav Meduna,
 *     Matthias Plasch - initial API and implementation and/or initial documentation
 *   Martin Melik Merkumians - adds getToStringBufferSize tests
 *   Martin Melik Merkumians - changes for removed implicit constructor for
 *      primitve types
 *   Martin Jobst - add equals tests
 *   Alois Zoitl  - migrated data type toString to std::string
 *******************************************************************************/
#include <atomic>
#include <boost/test/tools/old/interface.hpp>
#include <boost/test/unit_test.hpp>
#include "forte_boost_output_support.h"

#include "forte/datatypes/forte_string.h"

using namespace std::string_literals;

namespace forte::test {
  BOOST_AUTO_TEST_SUITE(CIEC_STRING_function_test)
  BOOST_AUTO_TEST_CASE(Type_test) {
    CIEC_STRING sTest;
    // check type information
    BOOST_REQUIRE(sTest.getDataTypeID() == CIEC_ANY::e_STRING);
  }

  BOOST_AUTO_TEST_CASE(String_literal) {
    CIEC_STRING test1 = ""_STRING;
    BOOST_REQUIRE(test1 == ""s);

    CIEC_STRING test2 = "test"_STRING;
    BOOST_REQUIRE(test2 == "test"s);

    CIEC_STRING test3 = "abc\0def"_STRING;
    BOOST_REQUIRE(test3 == "abc\0def"s);
  }

  BOOST_AUTO_TEST_CASE(String_length) {
    CIEC_STRING test1;
    BOOST_CHECK_EQUAL(test1.length(), 0);

    CIEC_STRING test2("\0"_STRING);
    BOOST_CHECK_EQUAL(test2.length(), 1);

    CIEC_STRING test3(""_STRING);
    BOOST_CHECK_EQUAL(test3.length(), 0);

    CIEC_STRING test4("1\03"_STRING); // {'1', '3', '\0'}
    BOOST_CHECK_EQUAL(test4.length(), 2);

    CIEC_STRING test5("123456789"_STRING);
    BOOST_CHECK_EQUAL(test5.length(), 9);
  }

  BOOST_AUTO_TEST_CASE(String_empty) {
    CIEC_STRING test1;
    BOOST_CHECK(test1.empty());

    CIEC_STRING test2("\0"_STRING);
    BOOST_CHECK(!test2.empty());

    CIEC_STRING test3(""_STRING);
    BOOST_CHECK(test3.empty());

    CIEC_STRING test4("1\03"_STRING); //{'1', '3', '\0'}
    BOOST_CHECK(!test4.empty());

    CIEC_STRING test5("12345789"_STRING);
    BOOST_CHECK(!test5.empty());
  }

  BOOST_AUTO_TEST_CASE(String_manipulation_interface) {
    CIEC_STRING sTest;
    BOOST_CHECK_EQUAL(sTest.length(), 0);
    BOOST_CHECK(sTest.empty());

    sTest = "This is a test string!"_STRING;
    BOOST_CHECK(!sTest.empty());
    BOOST_TEST(sTest.length() == 22);
    BOOST_TEST(sTest == "This is a test string!"_STRING);

    CIEC_STRING *psTest = new CIEC_STRING("This is a test string!"_STRING);
    BOOST_TEST(psTest->length() == 22);
    BOOST_TEST(*psTest == "This is a test string!"_STRING);
    delete psTest;
  }

  BOOST_AUTO_TEST_CASE(String_assignment) {
    CIEC_STRING sTest1;
    CIEC_STRING sTest2;

    sTest1 = "This is another test string!"_STRING;
    BOOST_TEST(sTest1 == "This is another test string!"_STRING);

    sTest2 = sTest1;
    BOOST_TEST(sTest1 == "This is another test string!"_STRING);

    BOOST_TEST(sTest1 == sTest2);
    BOOST_TEST(sTest1.length() == 28);
    BOOST_TEST(sTest2.length() == 28);
    sTest2 = "Check string!"_STRING;
    BOOST_TEST(sTest1 != sTest2);
  }

  BOOST_AUTO_TEST_CASE(String_clear) {
    CIEC_STRING test1;
    test1.clear();
    BOOST_CHECK_EQUAL(test1.length(), 0);

    CIEC_STRING test2(""_STRING);
    test2.clear();
    BOOST_CHECK_EQUAL(test2.length(), 0);

    CIEC_STRING test3("\0"_STRING);
    test3.clear();
    BOOST_CHECK_EQUAL(test3.length(), 0);

    CIEC_STRING test4("1\03"_STRING); //{'1', '3', '\0'}
    test4.clear();
    BOOST_CHECK_EQUAL(test4.length(), 0);

    CIEC_STRING test5("123456789"_STRING);
    test5.clear();
    BOOST_CHECK_EQUAL(test5.length(), 0);
  }

  BOOST_AUTO_TEST_CASE(String_re_assignment) {
    CIEC_STRING test1("123456789"_STRING);
    BOOST_TEST(test1 == "123456789"_STRING);
    BOOST_TEST(test1.length() == 9);

    test1.clear();
    BOOST_TEST(test1 == ""_STRING);
    BOOST_TEST(test1.length() == 0);

    test1.assign("123456789", 9);
    BOOST_TEST(test1 == "123456789"_STRING);
    BOOST_TEST(test1.length() == 9);

    test1.assign("123456", 6); // smaller string
    BOOST_TEST(test1 == "123456"_STRING);
    BOOST_TEST(test1.length() == 6);

    test1.assign("12345678", 8); // bigger string
    BOOST_TEST(test1 == "12345678"_STRING);
    BOOST_TEST(test1.length() == 8);

    test1.assign("1234567890", 9); // length to assign smaller than actual string coming from a smaller string
    BOOST_TEST(test1 == "123456789"_STRING);
    BOOST_TEST(test1.length() == 9);

    test1.assign("1234", 3); // length to assign smaller than actual string coming from a bigger string
    BOOST_TEST(test1 == "123"_STRING);
    BOOST_TEST(test1.length() == 3);

    test1.assign(nullptr, 0); // shouldn't do anything
    BOOST_TEST(test1 == ""_STRING);
    BOOST_TEST(test1.length() == 0);

    test1.assign("\x00", 0);
    BOOST_TEST(test1 == ""_STRING);
    BOOST_TEST(test1.length() == 0);

    test1.assign("0", 1);
    BOOST_TEST(test1 == "0"_STRING);
    BOOST_TEST(test1.length() == 1);

    test1.assign("", 0);
    BOOST_TEST(test1 == ""_STRING);
    BOOST_TEST(test1.length() == 0);
  }

  BOOST_AUTO_TEST_CASE(String_append) {
    CIEC_STRING test1("123456789"_STRING);
    BOOST_CHECK_EQUAL(test1.length(), 9);

    test1.append(""_STRING);
    BOOST_CHECK_EQUAL(test1.length(), 9);
    BOOST_TEST(test1 == "123456789"_STRING);

    test1.append("0"_STRING);
    BOOST_CHECK_EQUAL(test1.length(), 10);
    BOOST_TEST(test1 == "1234567890"_STRING);

    test1.append("\x00"_STRING);
    BOOST_CHECK_EQUAL(test1.length(), 11);
    BOOST_TEST(test1 == "1234567890\x00"_STRING);

    test1.append("123", 1);
    BOOST_CHECK_EQUAL(test1.length(), 12);
    BOOST_TEST(test1 == "1234567890\0"
                        "1"_STRING);

    test1.append(""_STRING);
    BOOST_CHECK_EQUAL(test1.length(), 12);
    BOOST_TEST(test1 == "1234567890\x00"
                        "1"_STRING);

    test1.append(nullptr, 0); // shouldn't do anything
    BOOST_CHECK_EQUAL(test1.length(), 12);
    BOOST_TEST(test1 == "1234567890\x00"
                        "1"_STRING);

    test1.append("1\x03"_STRING);
    BOOST_CHECK_EQUAL(test1.length(), 14);
    BOOST_TEST(test1 == "1234567890\x00"
                        "11\x03"_STRING);

    test1.append(nullptr, 0); // shouldn't do anything
    BOOST_CHECK_EQUAL(test1.length(), 14);
    BOOST_TEST(test1 == "1234567890\x00"
                        "11\x03"_STRING);

    test1.clear();
    BOOST_CHECK_EQUAL(test1.length(), 0);
    test1.append(""_STRING); // append to empty string
    BOOST_CHECK_EQUAL(test1.length(), 0);
    test1.append("\x00"_STRING); // append to empty string
    BOOST_CHECK_EQUAL(test1.length(), 1);
  }

  BOOST_AUTO_TEST_CASE(String_compare) {
    CIEC_STRING sTest1;
    CIEC_STRING sTest2;
    CIEC_STRING sTest3;

    sTest1 = "Check string!"_STRING;
    sTest2 = "Check string!"_STRING;
    sTest3 = "checkstring!"_STRING;

    BOOST_CHECK(sTest1 == sTest2);
    BOOST_TEST(sTest1 == "Check string!"_STRING);
    BOOST_TEST(sTest2 == "Check string!"_STRING);

    BOOST_CHECK(!(sTest1 == sTest3));
    BOOST_TEST(sTest1 == "Check string!"_STRING);
    BOOST_TEST(sTest3 == "checkstring!"_STRING);

    BOOST_CHECK(sTest2 != sTest3);
    BOOST_TEST(sTest2 == "Check string!"_STRING);
    BOOST_TEST(sTest3 == "checkstring!"_STRING);

    BOOST_CHECK(!(sTest2 != sTest1));
    BOOST_TEST(sTest2 == "Check string!"_STRING);
    BOOST_TEST(sTest1 == "Check string!"_STRING);

    BOOST_CHECK(sTest1 == "Check string!"_STRING);
    BOOST_TEST(sTest1 == "Check string!"_STRING);

    BOOST_CHECK("Check string!"_STRING == sTest1);
    BOOST_TEST(sTest1 == "Check string!"_STRING);

    BOOST_CHECK(!(sTest1 == "checkstring!"_STRING));
    BOOST_TEST(sTest1 == "Check string!"_STRING);

    BOOST_CHECK(!("checkstring!"_STRING == sTest1));
    BOOST_TEST(sTest1 == "Check string!"_STRING);

    BOOST_CHECK(sTest2 != "checkstring!"_STRING);
    BOOST_TEST(sTest2 == "Check string!"_STRING);

    BOOST_CHECK("checkstring!"_STRING != sTest2);
    BOOST_TEST(sTest2 == "Check string!"_STRING);

    BOOST_CHECK(!(sTest2 != "Check string!"_STRING));
    BOOST_TEST(sTest2 == "Check string!"_STRING);

    BOOST_CHECK(!("Check string!"_STRING != sTest2));
    BOOST_TEST(sTest2 == "Check string!"_STRING);
  }

  BOOST_AUTO_TEST_CASE(String_equals) {
    CIEC_STRING sTest1;
    CIEC_STRING sTest2;
    CIEC_STRING sTest3;

    sTest1 = "Check string!"_STRING;
    sTest2 = "Check string!"_STRING;
    sTest3 = "checkstring!"_STRING;
    BOOST_CHECK(sTest1.equals(sTest2));
    BOOST_CHECK(!sTest1.equals(CIEC_INT(0)));
    BOOST_CHECK(!sTest1.equals(sTest3));
    BOOST_CHECK(!sTest2.equals(sTest3));
  }

  BOOST_AUTO_TEST_CASE(String_binary_interface) {
    CIEC_STRING sTest1;
    CIEC_STRING sTest2;
    char cTest[] = "This is a test\x00string!"; // embedded \0, length 22 without trailing \0
    BOOST_CHECK_EQUAL(sTest1.length(), 0);
    sTest1.assign(cTest, static_cast<TForteUInt16>(sizeof(cTest) - 1));
    BOOST_CHECK_EQUAL(sTest1.length(), 22);
    BOOST_TEST(sTest1 == std::string(cTest, sizeof(cTest) - 1));

    sTest2 = sTest1;
    BOOST_CHECK_EQUAL(sTest2.length(), 22);
    BOOST_TEST(sTest2 == std::string(cTest, sizeof(cTest) - 1));

    CIEC_STRING *psTest = new CIEC_STRING(sTest1);
    BOOST_CHECK_EQUAL(psTest->length(), 22);
    BOOST_TEST(*psTest == std::string(cTest, sizeof(cTest) - 1));
    delete psTest;
  }

  BOOST_AUTO_TEST_CASE(Memory_Allocation) {
    CIEC_STRING sTest;

    BOOST_TEST(sTest.length() == 0);
    sTest.reserve(10);
    BOOST_TEST(sTest.length() == 0);
    sTest = "Test"_STRING;
    BOOST_TEST(sTest.length() == 4);
    sTest = "Test with more than ten characters"_STRING;
    BOOST_TEST(sTest.length() == 34);

    sTest = "Test Test"_STRING;
    BOOST_TEST(sTest.length() == 9);

    sTest = ""_STRING;
    BOOST_TEST(sTest.length() == 0);
  }

  BOOST_AUTO_TEST_CASE(String_toUTF8) {
    const char cASCII1[] = {0};
    const char cASCII2[] = {'A', 0};
    const char cASCII3[] = {0x7f, 0};
    const char cUpper1[] = {'A', static_cast<char>(0xc2), static_cast<char>(0xa2), 'A', 0};

    CIEC_STRING sTest;
    std::string sResult;

    sTest = ""_STRING;
    sResult.clear();
    sTest.toUTF8(sResult, false);
    BOOST_CHECK_EQUAL(sResult, cASCII1);

    sTest = "A"_STRING;
    sResult.clear();
    sTest.toUTF8(sResult, false);
    BOOST_CHECK_EQUAL(sResult, cASCII2);

    sTest = "\x7f"_STRING;
    sResult.clear();
    sTest.toUTF8(sResult, false);
    BOOST_CHECK_EQUAL(sResult, cASCII3);

    sTest = "A\xa2"
            "A"_STRING;
    sResult.clear();
    sTest.toUTF8(sResult, false);
    BOOST_CHECK_EQUAL(sResult, cUpper1);
  }

  const std::string cTestLiteral1 = "Test String"s;

  const std::string cTestLiteral2 = "\'This is another test string!\'"s;
  const CIEC_STRING cTestResult2 = "This is another test string!"_STRING;

  const std::string cTestDollarLiteral = "\'$$\'"s;
  const CIEC_STRING cTestDollarResult = "$"_STRING;
  const std::string cTestDollarToStringResult = "\'$$\'"s;

  const std::string cTestLineFeedLiteral = "\'$L$l\'"s;
  const CIEC_STRING cTestLineFeedResult = "\n\n"_STRING;
  const CIEC_STRING cTestLineFeedToStringResult = "\'$l$l\'"_STRING;
  const std::string cTestNewLineLiteral = "\'$N$n\'"s;
  const CIEC_STRING cTestNewLineResult = "\n\n"_STRING;
  const CIEC_STRING cTestNewLineToStringResult = "\'$l$l\'"_STRING;
  const std::string cTestFormFeedLiteral = "\'$P$p\'"s;
  const CIEC_STRING cTestFormFeedResult = "\f\f"_STRING;
  const CIEC_STRING cTestFormFeedToStringResult = "\'$p$p\'"_STRING;
  const std::string cTestCarriageReturnLiteral = "\'$R$r\'"s;
  const CIEC_STRING cTestCarriageReturnResult = "\r\r"_STRING;
  const CIEC_STRING cTestCarriageReturnToStringResult = "\'$r$r\'"_STRING;
  const std::string cTestTabLiteral = "\'$T$t\'"s;
  const CIEC_STRING cTestTabResult = "\t\t"_STRING;
  const CIEC_STRING cTestTabToStringResult = "\'$t$t\'"_STRING;
  const std::string cTestSingleQuoteLiteral = "\'$\'\'"s;
  const CIEC_STRING cTestSingleQuoteResult = "\'"_STRING;
  const CIEC_STRING cTestSingleQuoteToStringResult = "\'$\'\'"_STRING;
  const std::string cTestDoubleQuoteLiteral = "\'\"\'"s;
  const CIEC_STRING cTestDoubleQuoteResult = "\""_STRING;
  const CIEC_STRING cTestDoubleQuoteToStringResult = "\'\"\'"_STRING;
  const std::string cTestEscapedCharacterLiteral = "\'$30\'"s;
  const CIEC_STRING cTestEscapedCharacterResult = "0"_STRING;
  const CIEC_STRING cTestEscapedCharacterToStringResult = "\'0\'"_STRING;

  BOOST_AUTO_TEST_CASE(String_fromString) {
    CIEC_STRING sTestee;

    BOOST_TEST(-1 == sTestee.fromString(cTestLiteral1.c_str()));
    BOOST_TEST(sTestee == ""_STRING);
    sTestee.clear();

    BOOST_TEST(cTestLiteral2.length() == sTestee.fromString(cTestLiteral2.c_str()));
    BOOST_TEST(sTestee == cTestResult2);
    sTestee.clear();

    BOOST_TEST(cTestDollarLiteral.length() == sTestee.fromString(cTestDollarLiteral.c_str()));
    BOOST_TEST(sTestee == cTestDollarResult);
    sTestee.clear();

    BOOST_TEST(cTestLineFeedLiteral.length() == sTestee.fromString(cTestLineFeedLiteral.c_str()));
    BOOST_TEST(sTestee == cTestLineFeedResult);
    sTestee.clear();

    BOOST_TEST(cTestDollarLiteral.length() == sTestee.fromString(cTestDollarLiteral.c_str()));
    BOOST_TEST(sTestee == cTestDollarResult);
    sTestee.clear();

    BOOST_TEST(cTestFormFeedLiteral.length() == sTestee.fromString(cTestFormFeedLiteral.c_str()));
    BOOST_TEST(sTestee == cTestFormFeedResult);
    sTestee.clear();

    BOOST_TEST(cTestCarriageReturnLiteral.length() == sTestee.fromString(cTestCarriageReturnLiteral.c_str()));
    BOOST_TEST(sTestee == cTestCarriageReturnResult);
    sTestee.clear();

    BOOST_TEST(cTestTabLiteral.length() == sTestee.fromString(cTestTabLiteral.c_str()));
    BOOST_TEST(sTestee == cTestTabResult);
    sTestee.clear();

    BOOST_TEST(cTestSingleQuoteLiteral.length() == sTestee.fromString(cTestSingleQuoteLiteral.c_str()));
    BOOST_TEST(sTestee == cTestSingleQuoteResult);
    sTestee.clear();

    BOOST_TEST(cTestDoubleQuoteLiteral.length() == sTestee.fromString(cTestDoubleQuoteLiteral.c_str()));
    BOOST_TEST(sTestee == cTestDoubleQuoteResult);
    sTestee.clear();

    BOOST_TEST(cTestEscapedCharacterLiteral.length() == sTestee.fromString(cTestEscapedCharacterLiteral.c_str()));
    BOOST_TEST(sTestee == cTestEscapedCharacterResult);
  }

  void stringTypedFromString(const std::string &paSrc, const CIEC_STRING &paResult) {
    CIEC_STRING sTestee;
    BOOST_TEST(paSrc.length() == sTestee.fromString(paSrc.c_str()));
    BOOST_TEST(sTestee == paResult);
  }

  BOOST_AUTO_TEST_CASE(String_fromString_typed) {
    const std::string typePrefix("STRING#"s);
    stringTypedFromString(typePrefix + cTestLiteral2, cTestResult2);
    stringTypedFromString(typePrefix + cTestDollarLiteral, cTestDollarResult);
  }

  BOOST_AUTO_TEST_CASE(String_toString) {
    CIEC_STRING sTestee;
    std::string acBuffer;

    sTestee = CIEC_STRING(cTestResult2);
    sTestee.toString(acBuffer);
    BOOST_CHECK_EQUAL(cTestLiteral2, acBuffer);

    acBuffer.clear();
    sTestee = CIEC_STRING(cTestDollarResult);
    sTestee.toString(acBuffer);
    BOOST_CHECK_EQUAL(cTestDollarToStringResult, acBuffer);

    acBuffer.clear();
    sTestee = CIEC_STRING(cTestLineFeedResult);
    sTestee.toString(acBuffer);
    BOOST_CHECK_EQUAL(cTestLineFeedToStringResult, acBuffer);

    acBuffer.clear();
    sTestee = CIEC_STRING(cTestNewLineResult);
    sTestee.toString(acBuffer);
    BOOST_CHECK_EQUAL(cTestNewLineToStringResult, acBuffer);

    acBuffer.clear();
    sTestee = CIEC_STRING(cTestFormFeedResult);
    sTestee.toString(acBuffer);
    BOOST_CHECK_EQUAL(cTestFormFeedToStringResult, acBuffer);

    acBuffer.clear();
    sTestee = CIEC_STRING(cTestCarriageReturnResult);
    sTestee.toString(acBuffer);
    BOOST_CHECK_EQUAL(cTestCarriageReturnToStringResult, acBuffer);

    acBuffer.clear();
    sTestee = CIEC_STRING(cTestTabResult);
    sTestee.toString(acBuffer);
    BOOST_CHECK_EQUAL(cTestTabToStringResult, acBuffer);

    acBuffer.clear();
    sTestee = CIEC_STRING(cTestSingleQuoteResult);
    sTestee.toString(acBuffer);
    BOOST_CHECK_EQUAL(cTestSingleQuoteToStringResult, acBuffer);

    acBuffer.clear();
    sTestee = CIEC_STRING(cTestDoubleQuoteResult);
    sTestee.toString(acBuffer);
    BOOST_CHECK_EQUAL(cTestDoubleQuoteToStringResult, acBuffer);

    acBuffer.clear();
    sTestee = CIEC_STRING(cTestEscapedCharacterResult);
    sTestee.toString(acBuffer);
    BOOST_CHECK_EQUAL(cTestEscapedCharacterToStringResult, acBuffer);
  }

  BOOST_AUTO_TEST_CASE(String_to_string_nonprintable_sybmol_nul) {
    CIEC_STRING testString("\0"_STRING);
    std::string cStringBuffer;
    testString.toString(cStringBuffer);
    BOOST_TEST("'$00'" == cStringBuffer);
  }

  BOOST_AUTO_TEST_CASE(String_to_string_nonprintable_sybmol_esc) {
    CIEC_STRING testString("\x1b"_STRING);
    std::string cStringBuffer;
    testString.toString(cStringBuffer);
    BOOST_TEST("'$1B'" == cStringBuffer);
  }

  BOOST_AUTO_TEST_CASE(String_getToStringBufferSize_NoSpecialSymbols) {
    CIEC_STRING testString("4diac 4 ever!"_STRING);

    std::string cStringBuffer;
    testString.toString(cStringBuffer);
    BOOST_CHECK_EQUAL("'4diac 4 ever!'", cStringBuffer);
  }

  BOOST_AUTO_TEST_CASE(String_getToStringBufferSize_Dollar) {
    CIEC_STRING testString("$"_STRING);

    std::string cStringBuffer;
    testString.toString(cStringBuffer);
    BOOST_CHECK_EQUAL("'$$'", cStringBuffer);
  }

  BOOST_AUTO_TEST_CASE(String_getToStringBufferSize_SingleQuote) {
    CIEC_STRING testString("\'"_STRING);

    std::string cStringBuffer;
    testString.toString(cStringBuffer);
    BOOST_CHECK_EQUAL("'$''", cStringBuffer);
  }

  BOOST_AUTO_TEST_CASE(String_getToStringBufferSize_DoubleQuote) {
    CIEC_STRING testString("\""_STRING);

    std::string cStringBuffer;
    testString.toString(cStringBuffer);
    BOOST_CHECK_EQUAL("'\"'", cStringBuffer);
  }

  BOOST_AUTO_TEST_CASE(String_getToStringBufferSize_LineFeed) {
    CIEC_STRING testString("\n"_STRING);

    std::string cStringBuffer;
    testString.toString(cStringBuffer);
    BOOST_CHECK_EQUAL("'$l'", cStringBuffer);
  }

  BOOST_AUTO_TEST_CASE(String_getToStringBufferSize_FormFeed) {
    CIEC_STRING testString("\f"_STRING);

    std::string cStringBuffer;
    testString.toString(cStringBuffer);
    BOOST_CHECK_EQUAL("'$p'", cStringBuffer);
  }

  BOOST_AUTO_TEST_CASE(String_getToStringBufferSize_CarriageReturn) {
    CIEC_STRING testString("\r"_STRING);

    std::string cStringBuffer;
    testString.toString(cStringBuffer);
    BOOST_CHECK_EQUAL("'$r'", cStringBuffer);
  }

  BOOST_AUTO_TEST_CASE(String_getToStringBufferSize_Tab) {
    CIEC_STRING testString("\t"_STRING);

    std::string cStringBuffer;
    testString.toString(cStringBuffer);
    BOOST_CHECK_EQUAL("'$t'", cStringBuffer);
  }

  BOOST_AUTO_TEST_CASE(String_getToStringBufferSize_NonCommonSymbol) {
    CIEC_STRING testString("\x8A"_STRING);

    std::string cStringBuffer;
    testString.toString(cStringBuffer);
    BOOST_CHECK_EQUAL("'$8A'", cStringBuffer);
  }

  BOOST_AUTO_TEST_CASE(Implicit_cast_from_CHAR) {
    CIEC_CHAR testChar('4');
    CIEC_STRING resultString(testChar);

    BOOST_TEST("4"_STRING == resultString);
  }

  BOOST_AUTO_TEST_CASE(Assignment_from_CHAR) {
    CIEC_CHAR testChar('4');
    CIEC_STRING resultString;

    resultString = testChar;

    BOOST_TEST("4"_STRING == resultString);
  }

  BOOST_AUTO_TEST_SUITE_END()

} // namespace forte::test
