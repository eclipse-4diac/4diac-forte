/*******************************************************************************
 * Copyright (c) 2011, 2023 ACIN, nxtControl, Profactor GmbH, fortiss GmbH
 *                          TU Wien/ACIN, Martin Erich Jobst,
 *                          Primetals Technologies Austria GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Martin Melik Merkumians, Ingo Hegny, Alois Zoitl, Stanislav Meduna,
 *   Matthias Plasch
 *                - initial API and implementation and/or initial documentation
 *   Martin Melik Merkumians - adds getToStringBufferSize tests
 *   Martin Jobst - add equals tests
 *                - add user-defined literal tests
 *   Alois Zoitl  - migrated data type toString to std::string
 *******************************************************************************/
#include <boost/test/tools/old/interface.hpp>
#include <boost/test/unit_test.hpp>
#include "forte_boost_output_support.h"

#include "forte/datatypes/forte_wstring.h"
#include "forte/datatypes/forte_wchar.h"

namespace forte::test {
  BOOST_AUTO_TEST_SUITE(CIEC_WSTRING_function_test)
  BOOST_AUTO_TEST_CASE(Type_test) {
    CIEC_WSTRING sTest;
    // check type information
    BOOST_CHECK_EQUAL(sTest.getDataTypeID(), CIEC_ANY::e_WSTRING);
    // check operator const char* data type size
    BOOST_CHECK_EQUAL(sizeof(sTest.getValue()), sizeof(char *));
  }

  BOOST_AUTO_TEST_CASE(Literal_test) {
    CIEC_WSTRING test1 = u""_WSTRING;
    BOOST_REQUIRE_EQUAL(test1.getValue(), "");

    CIEC_WSTRING test2 = u"test"_WSTRING;
    BOOST_REQUIRE_EQUAL(test2.getValue(), "test");

    CIEC_WSTRING test3 = u"test\x20ac"_WSTRING;
    BOOST_REQUIRE_EQUAL(test3.getValue(), "test\xe2\x82\xac");
  }

  BOOST_AUTO_TEST_CASE(String_manilulation_interface) {
    CIEC_WSTRING sTest;
    char cWStringTest[] = "This is a test string!"; // length 22 without trailing \0
    BOOST_CHECK_EQUAL(sTest.length(), 0);
    sTest = CIEC_WSTRING(cWStringTest);
    BOOST_CHECK_EQUAL(sTest.length(), 22);
    BOOST_CHECK_EQUAL(strcmp(sTest.getValue(), cWStringTest), 0);

    CIEC_WSTRING *psTest = new CIEC_WSTRING(cWStringTest);
    BOOST_CHECK_EQUAL(psTest->length(), 22);
    BOOST_CHECK_EQUAL(strcmp(psTest->getValue(), cWStringTest), 0);
    delete psTest;
  }

  BOOST_AUTO_TEST_CASE(String_assignment) {
    CIEC_WSTRING sTest1;
    CIEC_WSTRING sTest2;
    char cWStringTest1[] = "This is another test string!";
    char cWStringTest2[] = "Check string!";
    sTest1 = CIEC_WSTRING(cWStringTest1);
    sTest2 = sTest1;
    BOOST_CHECK_EQUAL(strcmp(sTest1.getValue(), sTest2.getValue()), 0);
    BOOST_CHECK_EQUAL(sTest1.length(), 28);
    BOOST_CHECK_EQUAL(sTest2.length(), 28);
    sTest2 = CIEC_WSTRING(cWStringTest2);
    BOOST_CHECK(0 != strcmp(sTest1.getValue(), sTest2.getValue()));
    BOOST_CHECK_EQUAL(sTest1 != sTest2, true);
    BOOST_CHECK_EQUAL(strcmp(cWStringTest2, sTest2.getValue()), 0);
  }

  BOOST_AUTO_TEST_CASE(String_compare) {
    CIEC_WSTRING sTest1;
    CIEC_WSTRING sTest2;
    CIEC_WSTRING sTest3;

    const char cWStringTest1[] = "Check string!";
    char cWStringTest2[] = "Check string!";
    char cWStringTest3[] = "checkstring!";

    sTest1 = CIEC_WSTRING(cWStringTest1);
    sTest2 = CIEC_WSTRING(cWStringTest2);
    sTest3 = CIEC_WSTRING(cWStringTest3);
    BOOST_CHECK(sTest1 == sTest2);
    BOOST_CHECK_EQUAL(strcmp(sTest1.getValue(), cWStringTest1), 0);
    BOOST_CHECK_EQUAL(strcmp(sTest2.getValue(), cWStringTest2), 0);

    BOOST_CHECK(!(sTest1 == sTest3));
    BOOST_CHECK_EQUAL(strcmp(sTest1.getValue(), cWStringTest1), 0);
    BOOST_CHECK_EQUAL(strcmp(sTest3.getValue(), cWStringTest3), 0);

    BOOST_CHECK(sTest2 != sTest3);
    BOOST_CHECK_EQUAL(strcmp(sTest2.getValue(), cWStringTest2), 0);
    BOOST_CHECK_EQUAL(strcmp(sTest3.getValue(), cWStringTest3), 0);

    BOOST_CHECK(!(sTest2 != sTest1));
    BOOST_CHECK_EQUAL(strcmp(sTest2.getValue(), cWStringTest2), 0);
    BOOST_CHECK_EQUAL(strcmp(sTest1.getValue(), cWStringTest1), 0);

    BOOST_CHECK(sTest1 == CIEC_WSTRING(cWStringTest2));
    BOOST_CHECK_EQUAL(strcmp(sTest1.getValue(), cWStringTest1), 0);

    BOOST_CHECK(CIEC_WSTRING(cWStringTest2) == sTest1);
    BOOST_CHECK_EQUAL(strcmp(sTest1.getValue(), cWStringTest1), 0);

    BOOST_CHECK(!(sTest1 == CIEC_WSTRING(cWStringTest3)));
    BOOST_CHECK_EQUAL(strcmp(sTest1.getValue(), cWStringTest1), 0);

    BOOST_CHECK(!(CIEC_WSTRING(cWStringTest3) == sTest1));
    BOOST_CHECK_EQUAL(strcmp(sTest1.getValue(), cWStringTest1), 0);

    BOOST_CHECK(sTest2 != CIEC_WSTRING(cWStringTest3));
    BOOST_CHECK_EQUAL(strcmp(sTest2.getValue(), cWStringTest2), 0);

    BOOST_CHECK(CIEC_WSTRING(cWStringTest3) != sTest2);
    BOOST_CHECK_EQUAL(strcmp(sTest2.getValue(), cWStringTest2), 0);

    BOOST_CHECK(!(sTest2 != CIEC_WSTRING(cWStringTest1)));
    BOOST_CHECK_EQUAL(strcmp(sTest2.getValue(), cWStringTest2), 0);

    BOOST_CHECK(!(CIEC_WSTRING(cWStringTest1) != sTest2));
    BOOST_CHECK_EQUAL(strcmp(sTest2.getValue(), cWStringTest2), 0);
  }

  BOOST_AUTO_TEST_CASE(String_equals) {
    CIEC_WSTRING sTest1;
    CIEC_WSTRING sTest2;
    CIEC_WSTRING sTest3;

    char cTest1[] = "Check string!";
    char cTest2[] = "Check string!";
    char cTest3[] = "checkstring!";

    sTest1 = CIEC_WSTRING(cTest1);
    sTest2 = CIEC_WSTRING(cTest2);
    sTest3 = CIEC_WSTRING(cTest3);
    BOOST_CHECK(sTest1.equals(sTest2));
    BOOST_CHECK(!sTest1.equals(CIEC_INT(0)));
    BOOST_CHECK(!sTest1.equals(sTest3));
    BOOST_CHECK(!sTest2.equals(sTest3));
  }

  BOOST_AUTO_TEST_CASE(Memory_Allocation) {
    CIEC_WSTRING sTest;
    sTest.reserve(10);
    BOOST_CHECK_EQUAL(sTest.length(), 0);
    sTest = CIEC_WSTRING("Test");
    BOOST_CHECK_EQUAL(sTest.length(), 4);
    sTest = CIEC_WSTRING("Test with more than ten characters");
    BOOST_CHECK_EQUAL(sTest.length(), 34);
  }

  BOOST_AUTO_TEST_CASE(WString_fromUTF16) {
    const TForteByte cBOMBE[] = {0xFE, 0xFF};
    const TForteByte cBOMLE[] = {0xFF, 0xFE};

    const TForteByte cWStringTest1Default[] = {0x00, 0x7a};
    const TForteByte cWStringTest1BE[] = {0xFE, 0xFF, 0x00, 0x7a};
    const TForteByte cWStringTest1LE[] = {0xFF, 0xFE, 0x7a, 0x00};

    // Japanese "nihongo", meaning "the Japanese language"
    const TForteByte cWStringTest2[] = {0xFE, 0xFF, 0x65, 0xe5, 0x67, 0x2c, 0x8a, 0x9e};
    const TForteByte cWStringTest2Result[] = {0xE6, 0x97, 0xA5, 0xE6, 0x9C, 0xAC, 0xE8, 0xAA, 0x9E, 0};

    // a Chinese character meaning 'stump of tree'
    const TForteByte cWStringTest3[] = {0xFE, 0xFF, 0xd8, 0x4c, 0xdf, 0xb4};
    const TForteByte cWStringTest3Result[] = {0xF0, 0xA3, 0x8E, 0xB4, 0};

    bool nRes;
    CIEC_WSTRING sTest;

    nRes = sTest.fromUTF16(cBOMBE, sizeof(cBOMBE));
    BOOST_CHECK(nRes);
    BOOST_CHECK_EQUAL(sTest.length(), 0);

    nRes = sTest.fromUTF16(cBOMLE, sizeof(cBOMLE));
    BOOST_CHECK(nRes);
    BOOST_CHECK_EQUAL(sTest.length(), 0);

    nRes = sTest.fromUTF16(cWStringTest1Default, sizeof(cWStringTest1Default));
    BOOST_CHECK(nRes);
    BOOST_CHECK_EQUAL(sTest.length(), 1);
    BOOST_CHECK(!strcmp(sTest.getValue(), "z"));
    nRes = sTest.fromUTF16(cWStringTest1BE, sizeof(cWStringTest1BE));
    BOOST_CHECK(nRes);
    BOOST_CHECK_EQUAL(sTest.length(), 1);
    BOOST_CHECK(!strcmp(sTest.getValue(), "z"));
    nRes = sTest.fromUTF16(cWStringTest1LE, sizeof(cWStringTest1LE));
    BOOST_CHECK(nRes);
    BOOST_CHECK_EQUAL(sTest.length(), 1);
    BOOST_CHECK(!strcmp(sTest.getValue(), "z"));

    nRes = sTest.fromUTF16(cWStringTest2, sizeof(cWStringTest2));
    BOOST_CHECK(nRes);
    BOOST_CHECK_EQUAL(sTest.length(), 9);
    BOOST_CHECK(!strcmp(sTest.getValue(), (const char *) cWStringTest2Result));

    nRes = sTest.fromUTF16(cWStringTest3, sizeof(cWStringTest3));
    BOOST_CHECK(nRes);
    BOOST_CHECK_EQUAL(sTest.length(), 4);
    BOOST_CHECK(!strcmp(sTest.getValue(), (const char *) cWStringTest3Result));

    nRes = sTest.fromUTF16(cWStringTest3, 0);
    BOOST_CHECK(nRes);
    BOOST_CHECK_EQUAL(sTest.length(), 0);

    nRes = sTest.fromUTF16(cWStringTest3, 3);
    BOOST_CHECK(!nRes);
  }

  BOOST_AUTO_TEST_CASE(WString_toUTF16) {
    const TForteByte cWStringTest1Result[] = {0x00, 0x7a};

    // Japanese "nihongo", meaning "the Japanese language"
    const TForteByte cWStringTest2[] = {0xE6, 0x97, 0xA5, 0xE6, 0x9C, 0xAC, 0xE8, 0xAA, 0x9E, 0};
    const TForteByte cWStringTest2Result[] = {0x65, 0xe5, 0x67, 0x2c, 0x8a, 0x9e};

    // a Chinese character meaning 'stump of tree'
    const TForteByte cWStringTest3[] = {0xF0, 0xA3, 0x8E, 0xB4, 0};
    const TForteByte cWStringTest3Result[] = {0xd8, 0x4c, 0xdf, 0xb4};

    TForteByte cBuffer[32];

    int nRes;
    CIEC_WSTRING sTest;

    nRes = sTest.toUTF16(cBuffer, sizeof(cWStringTest1Result));
    BOOST_CHECK_EQUAL(nRes, 0);

    sTest = CIEC_WSTRING("z");
    nRes = sTest.toUTF16(cBuffer, sizeof(cWStringTest1Result));
    BOOST_CHECK_EQUAL(nRes, 2);
    BOOST_CHECK(!memcmp(cBuffer, cWStringTest1Result, 2));

    sTest = CIEC_WSTRING((const char *) cWStringTest2);
    nRes = sTest.toUTF16(cBuffer, sizeof(cWStringTest2Result));
    BOOST_CHECK_EQUAL(nRes, 6);
    BOOST_CHECK(!memcmp(cBuffer, cWStringTest2Result, 6));

    sTest = CIEC_WSTRING((const char *) cWStringTest3);
    nRes = sTest.toUTF16(cBuffer, sizeof(cWStringTest3Result));
    BOOST_CHECK_EQUAL(nRes, 4);
    BOOST_CHECK(!memcmp(cBuffer, cWStringTest3Result, 4));

    nRes = sTest.toUTF16(cBuffer, static_cast<unsigned int>(sizeof(cWStringTest3Result) - 1));
    BOOST_CHECK_EQUAL(nRes, -1);
  }

  BOOST_AUTO_TEST_CASE(WString_fromUTF8) {
    const TForteByte cASCII1[] = {0};
    const TForteByte cASCII2[] = {'A', 0};
    const TForteByte cASCII3[] = {0x7f, 0};
    const TForteByte cUpper1[] = {'A', 0xc2, 0xa2, 'A', 0};
    const TForteByte cUpper2[] = {'A', 0xe2, 0x82, 0xac, 'B', 0};
    const TForteByte cUpper2Capped[] = {'A', 0xe2, 0x82, 0xac, 0};
    const TForteByte cUpper3[] = {0xf0, 0xa4, 0xad, 0xa2, 0};
    const TForteByte cInvalid1[] = {0x80, 0};
    const TForteByte cInvalid2[] = {0xfe, 0x80, 0x80, 0x80, 0x80, 0x80, 0};

    int bRes;
    CIEC_WSTRING sTest;

    bRes = sTest.fromUTF8((const char *) cASCII1, -1, false);
    BOOST_CHECK_EQUAL(strlen((const char *) cASCII1), bRes);
    BOOST_CHECK_EQUAL(sTest.length(), 0);

    bRes = sTest.fromUTF8((const char *) cASCII2, -1, false);
    BOOST_CHECK_EQUAL(strlen((const char *) cASCII2), bRes);
    BOOST_CHECK(!strcmp("A", sTest.getValue()));

    bRes = sTest.fromUTF8((const char *) cASCII3, -1, false);
    BOOST_CHECK_EQUAL(strlen((const char *) cASCII3), bRes);
    BOOST_CHECK(!strcmp("\x7f", sTest.getValue()));

    bRes = sTest.fromUTF8((const char *) cUpper1, -1, false);
    BOOST_CHECK_EQUAL(strlen((const char *) cUpper1), bRes);
    BOOST_CHECK(!strcmp((const char *) cUpper1, sTest.getValue()));

    bRes = sTest.fromUTF8((const char *) cUpper2, -1, false);
    BOOST_CHECK_EQUAL(strlen((const char *) cUpper2), bRes);
    BOOST_CHECK(!strcmp((const char *) cUpper2, sTest.getValue()));

    bRes = sTest.fromUTF8((const char *) cUpper2, 4, false);
    BOOST_CHECK_EQUAL(4, bRes); // as we cap to 4 we expect the number of taken bytes to be 4
    BOOST_CHECK(!strcmp((const char *) cUpper2Capped, sTest.getValue()));

    bRes = sTest.fromUTF8((const char *) cUpper3, -1, false);
    BOOST_CHECK_EQUAL(strlen((const char *) cUpper3), bRes);
    BOOST_CHECK(!strcmp("?", sTest.getValue()));

    bRes = sTest.fromUTF8((const char *) cInvalid1, -1, false);
    BOOST_CHECK_EQUAL(-1, bRes);

    bRes = sTest.fromUTF8((const char *) cInvalid2, -1, false);
    BOOST_CHECK_EQUAL(-1, bRes);
  }

  BOOST_AUTO_TEST_CASE(WString_toUTF8) {
    const char cASCII1[] = "";
    const char cASCII2[] = {'A', 0};
    const char cASCII3[] = {0x7f, 0};
    const char cUpper1[] = {'A', static_cast<char>(0xc2), static_cast<char>(0xa2), 'A', 0};
    const char cUpper2[] = {'A', static_cast<char>(0xe2), static_cast<char>(0x82), static_cast<char>(0xac), 'B', 0};
    const char cUpper3[] = {'A', static_cast<char>(0xe2), static_cast<char>(0x82), static_cast<char>(0xac), '$', 'B',
                            0};
    const char cUpper3Escaped[] = {
        '"', 'A', static_cast<char>(0xe2), static_cast<char>(0x82), static_cast<char>(0xac), '$', '$', 'B', '"', 0};

    CIEC_WSTRING sTest;
    std::string sResult;

    sTest = CIEC_WSTRING("");
    sTest.toUTF8(sResult, false);
    BOOST_CHECK_EQUAL(sResult, cASCII1);

    sTest = CIEC_WSTRING("A");
    sResult.clear();
    sTest.toUTF8(sResult, false);
    BOOST_CHECK_EQUAL(sResult, cASCII2);

    sTest = CIEC_WSTRING("\x7f");
    sResult.clear();
    sTest.toUTF8(sResult, false);
    BOOST_CHECK_EQUAL(sResult, cASCII3);

    sTest = CIEC_WSTRING((const char *) cUpper1);
    sResult.clear();
    sTest.toUTF8(sResult, false);
    BOOST_CHECK_EQUAL(sResult, cUpper1);

    sTest = CIEC_WSTRING((const char *) cUpper2);
    sResult.clear();
    sTest.toUTF8(sResult, false);
    BOOST_CHECK_EQUAL(sResult, cUpper2);

    sTest = CIEC_WSTRING((const char *) cUpper3);
    sResult.clear();
    sTest.toUTF8(sResult, true);
    BOOST_CHECK_EQUAL(sResult, cUpper3Escaped);
  }

  const char cWStringTestLiteral1[] = "Test String";
  const char cWStringTestResult1[] = "Test String";
  const char cWStringTestToStringResult1[] = "\"Test String\"";
  const char cWStringTestLiteral2[] = "\"This is another test string!\"";
  const char cWStringTestResult2[] = "This is another test string!";
  const char cWStringTestDollarLiteral[] = "\"$$\"";
  const char cWStringTestDollarResult[] = "$";
  const char cWStringTestDollarToStringResult[] = "\"$$\"";
  const char cWStringTestLineFeedLiteral[] = "\"$L$l\"";
  const char cWStringTestLineFeedResult[] = "\n\n";
  const char cWStringTestLineFeedToStringResult[] = "\"$l$l\"";
  const char cWStringTestNewLineLiteral[] = "\"$N$n\"";
  const char cWStringTestNewLineResult[] = "\n\n";
  const char cWStringTestNewLineToStringResult[] = "\"$l$l\"";
  const char cWStringTestFormFeedLiteral[] = "\"$P$p\"";
  const char cWStringTestFormFeedResult[] = "\f\f";
  const char cWStringTestFormFeedToStringResult[] = "\"$p$p\"";
  const char cWStringTestCarriageReturnLiteral[] = "\"$R$r\"";
  const char cWStringTestCarriageReturnResult[] = "\r\r";
  const char cWStringTestCarriageReturnToStringResult[] = "\"$r$r\"";
  const char cWStringTestTabLiteral[] = "\"$T$t\"";
  const char cWStringTestTabResult[] = "\t\t";
  const char cWStringTestTabToStringResult[] = "\"$t$t\"";
  const char cWStringTestSingleQuoteLiteral[] = "\"$\"\"";
  const char cWStringTestSingleQuoteResult[] = "\"";
  const char cWStringTestSingleQuoteToStringResult[] = "\"$\"\"";
  const char cWStringTestDoubleQuoteLiteral[] = "\"$\"\"";
  const char cWStringTestDoubleQuoteResult[] = "\"";
  const char cWStringTestDoubleQuoteToStringResult[] = "\"$\"\"";
  const char cWStringTestEscapedCharacterLiteral[] = "\"$0030\"";
  const char cWStringTestEscapedCharacterResult[] = "0";
  const char cWStringTestEscapedCharacterToStringResult[] = "\"0\"";
  const char cWStringTestEscapedWCharacterLiteral[] = "\"$01a0\"";
  const char cWStringTestEscapedWCharacterResult[] = "\xc6\xa0";
  const char cWStringTestEscapedWCharacterToStringResult[] = "\"xc6\xa0\"";

  BOOST_AUTO_TEST_CASE(WString_fromString) {
    CIEC_WSTRING sTestee;

    BOOST_CHECK_EQUAL(strlen(cWStringTestLiteral1), sTestee.fromString(cWStringTestLiteral1));
    BOOST_CHECK_EQUAL(sTestee.length(), strlen(cWStringTestResult1));
    BOOST_CHECK_EQUAL(0, strcmp(sTestee.getValue(), cWStringTestResult1));

    BOOST_CHECK_EQUAL(strlen(cWStringTestLiteral2), sTestee.fromString(cWStringTestLiteral2));
    BOOST_CHECK_EQUAL(sTestee.length(), strlen(cWStringTestResult2));
    BOOST_CHECK_EQUAL(0, strcmp(sTestee.getValue(), cWStringTestResult2));

    BOOST_CHECK_EQUAL(strlen(cWStringTestDollarLiteral), sTestee.fromString(cWStringTestDollarLiteral));
    BOOST_CHECK_EQUAL(sTestee.length(), strlen(cWStringTestDollarResult));
    BOOST_CHECK_EQUAL(0, strcmp(sTestee.getValue(), cWStringTestDollarResult));

    BOOST_CHECK_EQUAL(strlen(cWStringTestLineFeedLiteral), sTestee.fromString(cWStringTestLineFeedLiteral));
    BOOST_CHECK_EQUAL(sTestee.length(), strlen(cWStringTestLineFeedResult));
    BOOST_CHECK_EQUAL(0, strcmp(sTestee.getValue(), cWStringTestLineFeedResult));

    BOOST_CHECK_EQUAL(strlen(cWStringTestDollarLiteral), sTestee.fromString(cWStringTestDollarLiteral));
    BOOST_CHECK_EQUAL(sTestee.length(), strlen(cWStringTestDollarResult));
    BOOST_CHECK_EQUAL(0, strcmp(sTestee.getValue(), cWStringTestDollarResult));

    BOOST_CHECK_EQUAL(strlen(cWStringTestFormFeedLiteral), sTestee.fromString(cWStringTestFormFeedLiteral));
    BOOST_CHECK_EQUAL(sTestee.length(), strlen(cWStringTestFormFeedResult));
    BOOST_CHECK_EQUAL(0, strcmp(sTestee.getValue(), cWStringTestFormFeedResult));

    BOOST_CHECK_EQUAL(strlen(cWStringTestCarriageReturnLiteral), sTestee.fromString(cWStringTestCarriageReturnLiteral));
    BOOST_CHECK_EQUAL(sTestee.length(), strlen(cWStringTestCarriageReturnResult));
    BOOST_CHECK_EQUAL(0, strcmp(sTestee.getValue(), cWStringTestCarriageReturnResult));

    BOOST_CHECK_EQUAL(strlen(cWStringTestTabLiteral), sTestee.fromString(cWStringTestTabLiteral));
    BOOST_CHECK_EQUAL(sTestee.length(), strlen(cWStringTestTabResult));
    BOOST_CHECK_EQUAL(0, strcmp(sTestee.getValue(), cWStringTestTabResult));

    BOOST_CHECK_EQUAL(strlen(cWStringTestSingleQuoteLiteral), sTestee.fromString(cWStringTestSingleQuoteLiteral));
    BOOST_CHECK_EQUAL(sTestee.length(), strlen(cWStringTestSingleQuoteResult));
    BOOST_CHECK_EQUAL(0, strcmp(sTestee.getValue(), cWStringTestSingleQuoteResult));

    BOOST_CHECK_EQUAL(strlen(cWStringTestDoubleQuoteLiteral), sTestee.fromString(cWStringTestDoubleQuoteLiteral));
    BOOST_CHECK_EQUAL(sTestee.length(), strlen(cWStringTestDoubleQuoteResult));
    BOOST_CHECK_EQUAL(0, strcmp(sTestee.getValue(), cWStringTestDoubleQuoteResult));

    BOOST_CHECK_EQUAL(strlen(cWStringTestEscapedCharacterLiteral),
                      sTestee.fromString(cWStringTestEscapedCharacterLiteral));
    BOOST_CHECK_EQUAL(sTestee.length(), strlen(cWStringTestEscapedCharacterResult));
    BOOST_CHECK_EQUAL(0, strcmp(sTestee.getValue(), cWStringTestEscapedCharacterResult));

    BOOST_CHECK_EQUAL(strlen(cWStringTestEscapedWCharacterLiteral),
                      sTestee.fromString(cWStringTestEscapedWCharacterLiteral));
    BOOST_CHECK_EQUAL(sTestee.length(), strlen(cWStringTestEscapedWCharacterResult));
    BOOST_CHECK_EQUAL(0, strcmp(sTestee.getValue(), cWStringTestEscapedWCharacterResult));
  }

  void wStringTypedFromString(const std::string &paSrc, const char *paResult) {
    CIEC_WSTRING sTestee;
    BOOST_CHECK_EQUAL(paSrc.length(), sTestee.fromString(paSrc.c_str()));
    BOOST_CHECK_EQUAL(sTestee.length(), strlen(paResult));
    BOOST_CHECK_EQUAL(0, strcmp(sTestee.getValue(), paResult));
  }

  BOOST_AUTO_TEST_CASE(WString_fromString_typed) {
    wStringTypedFromString(std::string("WSTRING#") + cWStringTestLiteral1, cWStringTestResult1);
    wStringTypedFromString(std::string("WSTRING#") + cWStringTestLiteral2, cWStringTestResult2);
    wStringTypedFromString(std::string("WSTRING#") + cWStringTestDollarLiteral, cWStringTestDollarResult);
  }

  BOOST_AUTO_TEST_CASE(WString_toString) {
    CIEC_WSTRING sTestee;
    std::string acBuffer;

    sTestee = CIEC_WSTRING(cWStringTestResult1);
    sTestee.toString(acBuffer);
    BOOST_CHECK_EQUAL(cWStringTestToStringResult1, acBuffer);
    acBuffer.clear();

    sTestee = CIEC_WSTRING(cWStringTestResult2);
    sTestee.toString(acBuffer);
    BOOST_CHECK_EQUAL(cWStringTestLiteral2, acBuffer);
    acBuffer.clear();

    sTestee = CIEC_WSTRING(cWStringTestDollarResult);
    sTestee.toString(acBuffer);
    BOOST_CHECK_EQUAL(cWStringTestDollarToStringResult, acBuffer);
    acBuffer.clear();

    sTestee = CIEC_WSTRING(cWStringTestLineFeedResult);
    sTestee.toString(acBuffer);
    BOOST_CHECK_EQUAL(cWStringTestLineFeedToStringResult, acBuffer);
    acBuffer.clear();

    sTestee = CIEC_WSTRING(cWStringTestNewLineResult);
    sTestee.toString(acBuffer);
    BOOST_CHECK_EQUAL(cWStringTestNewLineToStringResult, acBuffer);
    acBuffer.clear();

    sTestee = CIEC_WSTRING(cWStringTestFormFeedResult);
    sTestee.toString(acBuffer);
    BOOST_CHECK_EQUAL(cWStringTestFormFeedToStringResult, acBuffer);
    acBuffer.clear();

    sTestee = CIEC_WSTRING(cWStringTestCarriageReturnResult);
    sTestee.toString(acBuffer);
    BOOST_CHECK_EQUAL(cWStringTestCarriageReturnToStringResult, acBuffer);
    acBuffer.clear();

    sTestee = CIEC_WSTRING(cWStringTestTabResult);
    sTestee.toString(acBuffer);
    BOOST_CHECK_EQUAL(cWStringTestTabToStringResult, acBuffer);
    acBuffer.clear();

    sTestee = CIEC_WSTRING(cWStringTestSingleQuoteResult);
    sTestee.toString(acBuffer);
    BOOST_CHECK_EQUAL(cWStringTestSingleQuoteToStringResult, acBuffer);
    acBuffer.clear();

    sTestee = CIEC_WSTRING(cWStringTestDoubleQuoteResult);
    sTestee.toString(acBuffer);
    BOOST_CHECK_EQUAL(cWStringTestDoubleQuoteToStringResult, acBuffer);
    acBuffer.clear();

    sTestee = CIEC_WSTRING(cWStringTestEscapedCharacterResult);
    sTestee.toString(acBuffer);
    BOOST_CHECK_EQUAL(cWStringTestEscapedCharacterToStringResult, acBuffer);
  }

  BOOST_AUTO_TEST_SUITE_END()
} // namespace forte::test
