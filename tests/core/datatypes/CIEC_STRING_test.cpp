/*******************************************************************************
 * Copyright (c) 2011 - 2013 ACIN, nxtControl, Profactor GmbH, fortiss GmbH
 *              2018 TU Wien/ACIN
 *               2022 Primetals Technologies Austria GmbH
 *               2023 Martin Erich Jobst
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
 *    Martin Melik Merkumians - changes for removed implicit constructor for
 *      primitve types
 *   Martin Jobst - add equals tests
 *******************************************************************************/
#include <boost/test/unit_test.hpp>
#include "forte_boost_output_support.h"

#include "../../../src/core/datatypes/forte_string.h"

using namespace std::string_literals;

BOOST_AUTO_TEST_SUITE(CIEC_STRING_function_test)
BOOST_AUTO_TEST_CASE(Type_test)
{
  CIEC_STRING sTest;
  //check type information
  BOOST_CHECK_EQUAL(sTest.getDataTypeID(), CIEC_ANY::e_STRING);
}

BOOST_AUTO_TEST_CASE(String_literal)
{
  CIEC_STRING test1 = ""_STRING;
  BOOST_TEST(test1.getStorage() == ""s);

  CIEC_STRING test2 = "test"_STRING;
  BOOST_TEST(test2.getStorage() == "test"s);

  CIEC_STRING test3 = "abc\0def"_STRING;
  BOOST_TEST(test3.getStorage() == "abc\0def"s);
}

BOOST_AUTO_TEST_CASE(String_length)
{
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

BOOST_AUTO_TEST_CASE(String_empty)
{
  CIEC_STRING test1;
  BOOST_CHECK(test1.empty());

  CIEC_STRING test2("\0"_STRING);
  BOOST_CHECK(!test2.empty());

  CIEC_STRING test3(""_STRING);
  BOOST_CHECK(test3.empty());

  CIEC_STRING test4("1\03"_STRING);  //{'1', '3', '\0'}
  BOOST_CHECK(!test4.empty());

  CIEC_STRING test5("12345789"_STRING);
  BOOST_CHECK(!test5.empty());

}

BOOST_AUTO_TEST_CASE(String_manipulation_interface)
{
  CIEC_STRING sTest;
  std::string cTest = "This is a test string!"s; //length 22 without trailing \0
  BOOST_CHECK_EQUAL(sTest.length(), 0);
  BOOST_CHECK(sTest.empty());

  sTest = CIEC_STRING(cTest);
  BOOST_CHECK(! sTest.empty());
  BOOST_TEST(sTest.length() == 22);
  BOOST_TEST(sTest.getStorage() == cTest);

  CIEC_STRING* psTest = new CIEC_STRING(cTest);
  BOOST_TEST(psTest->length() == 22);
  BOOST_TEST(psTest->getStorage() == cTest);
  delete psTest;

}

BOOST_AUTO_TEST_CASE(String_assignment)
{
  CIEC_STRING sTest1;
  CIEC_STRING sTest2;
  std::string cTest1 = "This is another test string!"s;
  std::string cTest2 = "Check string!"s;

  sTest1 = CIEC_STRING(cTest1);
  BOOST_TEST(sTest1.getStorage() == cTest1);

  sTest2 = sTest1;
  BOOST_TEST(sTest1.getStorage() == cTest1);

  BOOST_TEST(sTest1 == sTest2);
  BOOST_TEST(sTest1.length() == 28);
  BOOST_TEST(sTest2.length() == 28);
  sTest2 = CIEC_STRING(cTest2);
  BOOST_TEST(sTest1 != sTest2);
}

BOOST_AUTO_TEST_CASE(String_clear)
{
  CIEC_STRING test1;
  test1.clear();
  BOOST_CHECK_EQUAL(test1.length(), 0);

  CIEC_STRING test2(""_STRING);
  test2.clear();
  BOOST_CHECK_EQUAL(test2.length(), 0);

  CIEC_STRING test3("\0"_STRING);
  test3.clear();
  BOOST_CHECK_EQUAL(test3.length(), 0);

  CIEC_STRING test4("1\03"_STRING);  //{'1', '3', '\0'}
  test4.clear();
  BOOST_CHECK_EQUAL(test4.length(), 0);

  CIEC_STRING test5("123456789"_STRING);
  test5.clear();
  BOOST_CHECK_EQUAL(test5.length(), 0);

}

BOOST_AUTO_TEST_CASE(String_re_assignment)
{
  CIEC_STRING test1("123456789"_STRING);
  BOOST_TEST(test1.getStorage() == "123456789"s);
  BOOST_TEST(test1.length() == 9);

  test1.clear();
  BOOST_TEST(test1.getStorage() == ""s);
  BOOST_TEST(test1.length() == 0);

  test1.assign("123456789", 9);
  BOOST_TEST(test1.getStorage() == "123456789"s);
  BOOST_TEST(test1.length() == 9);

  test1.assign("123456", 6); //smaller string
  BOOST_TEST(test1.getStorage() == "123456"s);
  BOOST_TEST(test1.length() == 6);

  test1.assign("12345678", 8); //bigger string
  BOOST_TEST(test1.getStorage() == "12345678"s);
  BOOST_TEST(test1.length() == 8);

  test1.assign("1234567890", 9); //length to assign smaller than actual string coming from a smaller string
  BOOST_TEST(test1.getStorage() == "123456789"s);
  BOOST_TEST(test1.length() == 9);

  test1.assign("1234", 3);//length to assign smaller than actual string coming from a bigger string
  BOOST_TEST(test1.getStorage() == "123"s);
  BOOST_TEST(test1.length() == 3);

  test1.assign(nullptr, 0);//shouldn't do anything
  BOOST_TEST(test1.getStorage() == ""s);
  BOOST_TEST(test1.length() == 0);

  test1.assign("\0", 0);
  BOOST_TEST(test1.getStorage() == ""s);
  BOOST_TEST(test1.length() == 0);

  test1.assign("0", 1);
  BOOST_TEST(test1.getStorage() == "0"s);
  BOOST_TEST(test1.length() == 1);

  test1.assign("", 0);
  BOOST_TEST(test1.getStorage() == ""s);
  BOOST_TEST(test1.length() == 0);
}

BOOST_AUTO_TEST_CASE(String_append) {
  CIEC_STRING test1("123456789"_STRING);
  BOOST_CHECK_EQUAL(test1.length(), 9);

  test1.append("");
  BOOST_CHECK_EQUAL(test1.length(), 9);
  BOOST_TEST(test1.getStorage() == "123456789"s);

  test1.append("0");
  BOOST_CHECK_EQUAL(test1.length(), 10);
  BOOST_TEST(test1.getStorage() == "1234567890"s);

  test1.append("\0");
  BOOST_CHECK_EQUAL(test1.length(), 10);
  BOOST_TEST(test1.getStorage() == "1234567890"s);

  test1.append("123", 1);
  BOOST_CHECK_EQUAL(test1.length(), 11);
  BOOST_TEST(test1.getStorage() == "12345678901"s);

  test1.append("");
  BOOST_CHECK_EQUAL(test1.length(), 11);
  BOOST_TEST(test1.getStorage() == "12345678901"s);

  test1.append(nullptr, 0); //shouldn't do anything
  BOOST_CHECK_EQUAL(test1.length(), 11);
  BOOST_TEST(test1.getStorage() == "12345678901"s);

  test1.append("1\03"); // {'1', '3', '\0'}
  BOOST_CHECK_EQUAL(test1.length(), 13);
  BOOST_TEST(test1.getStorage() == "123456789011\3"s);

  test1.append(nullptr, 0);//shouldn't do anything
  BOOST_CHECK_EQUAL(test1.length(), 13);
  BOOST_TEST(test1.getStorage() == "123456789011\3"s);

  test1.clear();
  test1.append("\0"); //append to empty string
  BOOST_CHECK_EQUAL(test1.length(), 0);

  test1.append(""); //append to empty string
  BOOST_CHECK_EQUAL(test1.length(), 0);

}

BOOST_AUTO_TEST_CASE(String_compare) {
  CIEC_STRING sTest1;
  CIEC_STRING sTest2;
  CIEC_STRING sTest3;

  std::string cTest1 = "Check string!"s;
  std::string cTest2 = "Check string!"s;
  std::string cTest3 = "checkstring!"s;

  sTest1 = CIEC_STRING(cTest1);
  sTest2 = CIEC_STRING(cTest2);
  sTest3 = CIEC_STRING(cTest3);
  BOOST_CHECK(sTest1 == sTest2);
  BOOST_TEST(sTest1.getStorage() == cTest1);
  BOOST_TEST(sTest2.getStorage() == cTest2);

  BOOST_CHECK(!(sTest1 == sTest3));
  BOOST_TEST(sTest1.getStorage() == cTest1);
  BOOST_TEST(sTest3.getStorage() == cTest3);

  BOOST_CHECK(sTest2 != sTest3);
  BOOST_TEST(sTest2.getStorage() == cTest2);
  BOOST_TEST(sTest3.getStorage() == cTest3);

  BOOST_CHECK(!(sTest2 != sTest1));
  BOOST_TEST(sTest2.getStorage() == cTest2);
  BOOST_TEST(sTest1.getStorage() == cTest1);
  

  BOOST_CHECK(sTest1 == CIEC_STRING(cTest2));
  BOOST_TEST(sTest1.getStorage() == cTest1);

  BOOST_CHECK(CIEC_STRING(cTest2) == sTest1);
  BOOST_TEST(sTest1.getStorage() == cTest1);

  BOOST_CHECK(!(sTest1 == CIEC_STRING(cTest3)));
  BOOST_TEST(sTest1.getStorage() == cTest1);

  BOOST_CHECK(!(CIEC_STRING(cTest3) == sTest1));
  BOOST_TEST(sTest1.getStorage() == cTest1);

  BOOST_CHECK(sTest2 != CIEC_STRING(cTest3));
  BOOST_TEST(sTest2.getStorage() == cTest2);

  BOOST_CHECK(CIEC_STRING(cTest3) != sTest2);
  BOOST_TEST(sTest2.getStorage() == cTest2);

  BOOST_CHECK(!(sTest2 != CIEC_STRING(cTest1)));
  BOOST_TEST(sTest2.getStorage() == cTest2);

  BOOST_CHECK(!(CIEC_STRING(cTest1) != sTest2));
  BOOST_TEST(sTest2.getStorage() == cTest2);
}

BOOST_AUTO_TEST_CASE(String_equals) {
  CIEC_STRING sTest1;
  CIEC_STRING sTest2;
  CIEC_STRING sTest3;

  std::string cTest1 = "Check string!";
  std::string cTest2 = "Check string!";
  std::string cTest3 = "checkstring!";

  sTest1 = CIEC_STRING(cTest1);
  sTest2 = CIEC_STRING(cTest2);
  sTest3 = CIEC_STRING(cTest3);
  BOOST_CHECK(sTest1.equals(sTest2));
  BOOST_CHECK(!sTest1.equals(CIEC_INT(0)));
  BOOST_CHECK(!sTest1.equals(sTest3));
  BOOST_CHECK(!sTest2.equals(sTest3));
}

BOOST_AUTO_TEST_CASE(String_binary_interface)
{
  CIEC_STRING sTest1;
  CIEC_STRING sTest2;
  char cTest[] = "This is a test\0string!"; //embedded \0, length 22 without trailing \0
  BOOST_CHECK_EQUAL(sTest1.length(), 0);
  sTest1.assign(cTest, static_cast<TForteUInt16>(sizeof(cTest)-1));
  BOOST_CHECK_EQUAL(sTest1.length(), 22);
  BOOST_TEST(sTest1.getStorage() == std::string(cTest, sizeof(cTest) - 1));

  sTest2 = sTest1;
  BOOST_CHECK_EQUAL(sTest2.length(), 22);
  BOOST_TEST(sTest2.getStorage() == std::string(cTest, sizeof(cTest) - 1));

  CIEC_STRING* psTest = new CIEC_STRING(sTest1);
  BOOST_CHECK_EQUAL(psTest->length(), 22);
  BOOST_TEST(psTest->getStorage() == std::string(cTest, sizeof(cTest) - 1));
  delete psTest;
}

BOOST_AUTO_TEST_CASE(Memory_Allocation)
{
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

BOOST_AUTO_TEST_CASE(String_toUTF8)
{
  const TForteByte cASCII1[] = { 0 };
  const TForteByte cASCII2[] = { 'A', 0 };
  const TForteByte cASCII3[] = { 0x7f, 0 };
  const TForteByte cUpper1[] = { 'A', 0xc2, 0xa2, 'A', 0 };

  int nRes;
  CIEC_STRING sTest;
  char sResult[32];

  sTest = ""_STRING;
  sResult[0] = '\0';
  nRes = sTest.toUTF8(sResult, sizeof(sResult), false);
  BOOST_CHECK_EQUAL(nRes, 0);
  BOOST_CHECK(! memcmp(sResult, cASCII1, sizeof(cASCII1)));

  sTest = "A"_STRING;
  sResult[0] = '\0';
  nRes = sTest.toUTF8(sResult, sizeof(sResult), false);
  BOOST_CHECK_EQUAL(nRes, 1);
  BOOST_CHECK(! memcmp(sResult, cASCII2, sizeof(cASCII2)));
  nRes = sTest.toUTF8(sResult, 1, false);
  BOOST_CHECK_EQUAL(nRes, -1);

  sTest = "\x7f"_STRING;
  sResult[0] = '\0';
  nRes = sTest.toUTF8(sResult, sizeof(sResult), false);
  BOOST_CHECK_EQUAL(nRes, 1);
  BOOST_CHECK(! memcmp(sResult, cASCII3, sizeof(cASCII3)));
  nRes = sTest.toUTF8(sResult, 1, false);
  BOOST_CHECK_EQUAL(nRes, -1);
  nRes = sTest.toUTF8(sResult, 2, false);
  BOOST_CHECK_EQUAL(nRes, 1);

  sTest = "A\xa2""A"_STRING;
  sResult[0] = '\0';
  nRes = sTest.toUTF8(sResult, sizeof(sResult), false);
  BOOST_CHECK_EQUAL(nRes, 4);
  BOOST_CHECK(! memcmp(sResult, cUpper1, sizeof(cUpper1)));
  nRes = sTest.toUTF8(sResult, 4, false);
  BOOST_CHECK_EQUAL(nRes, -1);
  nRes = sTest.toUTF8(sResult, 5, false);
  BOOST_CHECK_EQUAL(nRes, 4);
}

const std::string cTestLiteral1 = "Test String"s;

const std::string cTestLiteral2 = "\'This is another test string!\'"s;
const std::string cTestResult2 = "This is another test string!"s;

const std::string cTestDollarLiteral = "\'$$\'"s;
const std::string cTestDollarResult = "$"s;
const std::string cTestDollarToStringResult = "\'$$\'"s;

const std::string cTestLineFeedLiteral = "\'$L$l\'"s;
const std::string cTestLineFeedResult = "\x10\x10"s;
const std::string cTestLineFeedToStringResult = "\'$l$l\'"s;
const std::string cTestNewLineLiteral = "\'$N$n\'"s;
const std::string cTestNewLineResult = "\n\n"s;
const std::string cTestNewLineToStringResult = "\'$n$n\'"s;
const std::string cTestFormFeedLiteral = "\'$P$p\'"s;
const std::string cTestFormFeedResult = "\f\f"s;
const std::string cTestFormFeedToStringResult = "\'$p$p\'"s;
const std::string cTestCarriageReturnLiteral = "\'$R$r\'"s;
const std::string cTestCarriageReturnResult = "\r\r"s;
const std::string cTestCarriageReturnToStringResult = "\'$r$r\'"s;
const std::string cTestTabLiteral = "\'$T$t\'"s;
const std::string cTestTabResult = "\t\t"s;
const std::string cTestTabToStringResult = "\'$t$t\'"s;
const std::string cTestSingleQuoteLiteral = "\'$\'\'"s;
const std::string cTestSingleQuoteResult = "\'"s;
const std::string cTestSingleQuoteToStringResult = "\'$\'\'"s;
const std::string cTestDoubleQuoteLiteral = "\'\"\'"s;
const std::string cTestDoubleQuoteResult = "\""s;
const std::string cTestDoubleQuoteToStringResult = "\'\"\'"s;
const std::string cTestEscapedCharacterLiteral = "\'$30\'"s;
const std::string cTestEscapedCharacterResult = "0"s;
const std::string cTestEscapedCharacterToStringResult = "\'0\'"s;

BOOST_AUTO_TEST_CASE(String_fromString)
{
  CIEC_STRING sTestee;

  BOOST_TEST(-1 == sTestee.fromString(cTestLiteral1.c_str()));
  BOOST_TEST(sTestee.length() == 0);
  sTestee.clear();

  BOOST_TEST(cTestLiteral2.length() == sTestee.fromString(cTestLiteral2.c_str()));
  BOOST_TEST(sTestee.getStorage() == cTestResult2);
  sTestee.clear();

  BOOST_TEST(cTestDollarLiteral.length() == sTestee.fromString(cTestDollarLiteral.c_str()));
  BOOST_TEST(sTestee.getStorage() == cTestDollarResult);
  sTestee.clear();

  BOOST_TEST(cTestLineFeedLiteral.length() == sTestee.fromString(cTestLineFeedLiteral.c_str()));
  BOOST_TEST(sTestee.getStorage() == std::string(cTestLineFeedResult));
  sTestee.clear();

  BOOST_TEST(cTestDollarLiteral.length() == sTestee.fromString(cTestDollarLiteral.c_str()));
  BOOST_TEST(sTestee.getStorage() == cTestDollarResult);
  sTestee.clear();

  BOOST_TEST(cTestFormFeedLiteral.length() == sTestee.fromString(cTestFormFeedLiteral.c_str()));
  BOOST_TEST(sTestee.getStorage() == cTestFormFeedResult);
  sTestee.clear();

  BOOST_TEST(cTestCarriageReturnLiteral.length() == sTestee.fromString(cTestCarriageReturnLiteral.c_str()));
  BOOST_TEST(sTestee.getStorage() == cTestCarriageReturnResult);
  sTestee.clear();

  BOOST_TEST(cTestTabLiteral.length() == sTestee.fromString(cTestTabLiteral.c_str()));
  BOOST_TEST(sTestee.getStorage() == cTestTabResult);
  sTestee.clear();

  BOOST_TEST(cTestSingleQuoteLiteral.length() == sTestee.fromString(cTestSingleQuoteLiteral.c_str()));
  BOOST_TEST(sTestee.getStorage() == cTestSingleQuoteResult);
  sTestee.clear();

  BOOST_TEST(cTestDoubleQuoteLiteral.length() == sTestee.fromString(cTestDoubleQuoteLiteral.c_str()));
  BOOST_TEST(sTestee.getStorage() == cTestDoubleQuoteResult);
  sTestee.clear();

  BOOST_TEST(cTestEscapedCharacterLiteral.length() == sTestee.fromString(cTestEscapedCharacterLiteral.c_str()));
  BOOST_TEST(sTestee.getStorage() == cTestEscapedCharacterResult);
}

void stringTypedFromString(const std::string &pa_sSrc, const std::string pa_acResult){
  CIEC_STRING sTestee;
  BOOST_TEST(pa_sSrc.length() == sTestee.fromString(pa_sSrc.c_str()));
  BOOST_TEST(sTestee.getStorage() == pa_acResult);
}

BOOST_AUTO_TEST_CASE(String_fromString_typed) {
  const std::string typePrefix("STRING#");
  stringTypedFromString(typePrefix + cTestLiteral2, cTestResult2);
  stringTypedFromString(typePrefix + cTestDollarLiteral, cTestDollarResult);
}

BOOST_AUTO_TEST_CASE(String_toString)
{
  CIEC_STRING sTestee;
  char acBuffer[200];

  sTestee = CIEC_STRING(cTestResult2);
  BOOST_CHECK_EQUAL(sTestee.toString(acBuffer, 200), cTestLiteral2.length());
  BOOST_TEST(cTestLiteral2 == acBuffer);

  sTestee = CIEC_STRING(cTestDollarResult);
  BOOST_CHECK_EQUAL(sTestee.toString(acBuffer, 200), cTestDollarToStringResult.length());
  BOOST_TEST(cTestDollarToStringResult == acBuffer);

  sTestee = CIEC_STRING(cTestLineFeedResult);
  BOOST_CHECK_EQUAL(sTestee.toString(acBuffer, 200), cTestLineFeedToStringResult.length());
  BOOST_TEST(cTestLineFeedToStringResult == acBuffer);

  sTestee = CIEC_STRING(cTestNewLineResult);
  BOOST_CHECK_EQUAL(sTestee.toString(acBuffer, 200), cTestNewLineToStringResult.length());
  BOOST_TEST(cTestNewLineToStringResult == acBuffer);

  sTestee = CIEC_STRING(cTestFormFeedResult);
  BOOST_CHECK_EQUAL(sTestee.toString(acBuffer, 200), cTestFormFeedToStringResult.length());
  BOOST_TEST(cTestFormFeedToStringResult == acBuffer);

  sTestee = CIEC_STRING(cTestCarriageReturnResult);
  BOOST_CHECK_EQUAL(sTestee.toString(acBuffer, 200), cTestCarriageReturnToStringResult.length());
  BOOST_TEST(cTestCarriageReturnToStringResult == acBuffer);

  sTestee = CIEC_STRING(cTestTabResult);
  BOOST_CHECK_EQUAL(sTestee.toString(acBuffer, 200), cTestTabToStringResult.length());
  BOOST_TEST(cTestTabToStringResult == acBuffer);

  sTestee = CIEC_STRING(cTestSingleQuoteResult);
  BOOST_CHECK_EQUAL(sTestee.toString(acBuffer, 200), cTestSingleQuoteToStringResult.length());
  BOOST_TEST(cTestSingleQuoteToStringResult == acBuffer);

  sTestee = CIEC_STRING(cTestDoubleQuoteResult);
  BOOST_CHECK_EQUAL(sTestee.toString(acBuffer, 200), cTestDoubleQuoteToStringResult.length());
  BOOST_TEST(cTestDoubleQuoteToStringResult == acBuffer);

  sTestee = CIEC_STRING(cTestEscapedCharacterResult);
  BOOST_CHECK_EQUAL(sTestee.toString(acBuffer, 200), cTestEscapedCharacterToStringResult.length());
  BOOST_TEST(cTestEscapedCharacterToStringResult == acBuffer);
}

BOOST_AUTO_TEST_CASE(String_toString_faultcase_buffer_size_zero)
{
  CIEC_STRING testString("4diac 4 ever!"_STRING);
  const size_t bufferSize = 50;
  char cStringBuffer[bufferSize];

  BOOST_CHECK_EQUAL(-1, testString.toString(cStringBuffer, 0));
}

BOOST_AUTO_TEST_CASE(String_toString_faultcase_buffer_pointer_nullptr)
{
  CIEC_STRING testString;
  const size_t bufferSize = 50;

  BOOST_CHECK_EQUAL(-1, testString.toString(nullptr, bufferSize));
}

BOOST_AUTO_TEST_CASE(String_toString_faultcase_buffer_not_enough_buffer_size)
{
  const std::string sample_string = "4diac 4 ever!"s;
  CIEC_STRING testString(sample_string);
  const size_t bufferSize = sample_string.length() + 3; // For enclosing single quotes and '\0'
  char cStringBuffer[bufferSize];

  for(size_t i = 0; i < bufferSize; ++i) {
    BOOST_CHECK_EQUAL(-1, testString.toString(cStringBuffer, i));
  }
  BOOST_CHECK_EQUAL(bufferSize - 1, testString.toString(cStringBuffer, bufferSize)); // \0 is not counted
}

BOOST_AUTO_TEST_CASE(String_getToStringBufferSize_NoSpecialSymbols)
{
  CIEC_STRING testString("4diac 4 ever!"_STRING);

  size_t bufferSize = testString.getToStringBufferSize();
  BOOST_CHECK_EQUAL(13 + 2 + 1, bufferSize);
}

BOOST_AUTO_TEST_CASE(String_getToStringBufferSize_Dollar)
{
  CIEC_STRING testString("$"_STRING);

  size_t bufferSize = testString.getToStringBufferSize();
  BOOST_CHECK_EQUAL(2 + 2 + 1, bufferSize); // '$$'\0
}

BOOST_AUTO_TEST_CASE(String_getToStringBufferSize_SingleQuote)
{
  CIEC_STRING testString("\'"_STRING);

  size_t bufferSize = testString.getToStringBufferSize();
  BOOST_CHECK_EQUAL(2 + 2 + 1, bufferSize); // '$''\0
}

BOOST_AUTO_TEST_CASE(String_getToStringBufferSize_DoubleQuote)
{
  CIEC_STRING testString("\""_STRING);

  size_t bufferSize = testString.getToStringBufferSize();
  BOOST_CHECK_EQUAL(1 + 2 + 1, bufferSize); // '"'\0
}

BOOST_AUTO_TEST_CASE(String_getToStringBufferSize_LineFeed)
{
  CIEC_STRING testString("\x10"_STRING);

  size_t bufferSize = testString.getToStringBufferSize();
  BOOST_CHECK_EQUAL(2 + 2 + 1, bufferSize); // '$L'\0
}

BOOST_AUTO_TEST_CASE(String_getToStringBufferSize_NewLine)
{
  CIEC_STRING testString("\n"_STRING);

  size_t bufferSize = testString.getToStringBufferSize();
  BOOST_CHECK_EQUAL(2 + 2 + 1, bufferSize); // '$N'\0
}

BOOST_AUTO_TEST_CASE(String_getToStringBufferSize_FormFeed)
{
  CIEC_STRING testString("\f"_STRING);

  size_t bufferSize = testString.getToStringBufferSize();
  BOOST_CHECK_EQUAL(2 + 2 + 1, bufferSize); // '$P'\0
}

BOOST_AUTO_TEST_CASE(String_getToStringBufferSize_CarriageReturn)
{
  CIEC_STRING testString("\r"_STRING);

  size_t bufferSize = testString.getToStringBufferSize();
  BOOST_CHECK_EQUAL(2 + 2 + 1, bufferSize); // '$R'\0
}

BOOST_AUTO_TEST_CASE(String_getToStringBufferSize_Tab)
{
  CIEC_STRING testString("\t"_STRING);

  size_t bufferSize = testString.getToStringBufferSize();
  BOOST_CHECK_EQUAL(2 + 2 + 1, bufferSize); // '$T'\0
}

BOOST_AUTO_TEST_CASE(String_getToStringBufferSize_NonCommonSymbol)
{
  CIEC_STRING testString("\x8A"_STRING);

  size_t bufferSize = testString.getToStringBufferSize();
  BOOST_CHECK_EQUAL(3 + 2 + 1, bufferSize); // '$8A'\0
}

BOOST_AUTO_TEST_CASE(Implicit_cast_from_CHAR)
{
  CIEC_CHAR testChar('4');
  CIEC_STRING resultString(testChar);

  size_t bufferSize = resultString.getToStringBufferSize();

  BOOST_TEST(4 == bufferSize); //'<symbol>'\0 = 4
  BOOST_TEST("4"_STRING == resultString);
}

BOOST_AUTO_TEST_CASE(Assignment_from_CHAR)
{
  CIEC_CHAR testChar('4');
  CIEC_STRING resultString;

  resultString = testChar;

  size_t bufferSize = resultString.getToStringBufferSize();

  BOOST_CHECK_EQUAL(4, bufferSize); //'<symbol>'\0 = 4
  BOOST_CHECK_EQUAL("4"_STRING, resultString);
}

BOOST_AUTO_TEST_SUITE_END()
