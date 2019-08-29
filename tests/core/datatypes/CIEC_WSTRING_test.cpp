/*******************************************************************************
 * Copyright (c) 2011 - 2014 ACIN, nxtControl, Profactor GmbH, fortiss GmbH
 *   2018 TU Wien/ACIN
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Martin Melik Merkumians, Ingo Hegny, Alois Zoitl,
 *   Stanislav Meduna, Matthias Plasch
 *    - initial API and implementation and/or initial documentation
 *   Martin Melik Merkumians - adds getToStringBufferSize tests
 *******************************************************************************/
#include <boost/test/unit_test.hpp>

#include "../../../src/core/datatypes/forte_wstring.h"

BOOST_AUTO_TEST_SUITE(CIEC_WSTRING_function_test)
BOOST_AUTO_TEST_CASE(Type_test)
{
  CIEC_WSTRING sTest;
  //check type information
  BOOST_CHECK_EQUAL(sTest.getDataTypeID(), CIEC_ANY::e_WSTRING);
  //check operator const char* data type size
  BOOST_CHECK_EQUAL(sizeof(sTest.getValue()), sizeof(char*));

}

BOOST_AUTO_TEST_CASE(String_manilulation_interface)
{
  CIEC_WSTRING sTest;
  char cWStringTest[] = "This is a test string!"; //length 22 without trailing \0
  BOOST_CHECK_EQUAL(sTest.length(), 0);
  sTest = cWStringTest;
  BOOST_CHECK_EQUAL(sTest.length(), 22);
  BOOST_CHECK_EQUAL(strcmp(sTest.getValue(), cWStringTest), 0);

  CIEC_WSTRING* psTest = new CIEC_WSTRING(cWStringTest);
  BOOST_CHECK_EQUAL(psTest->length(), 22);
  BOOST_CHECK_EQUAL(strcmp(psTest->getValue(), cWStringTest), 0);
  delete psTest;

}

BOOST_AUTO_TEST_CASE(String_assignment)
{
  CIEC_WSTRING sTest1;
  CIEC_WSTRING sTest2;
  char cWStringTest1[] = "This is another test string!";
  char cWStringTest2[] = "Check string!";
  sTest1 = cWStringTest1;
  sTest2 = sTest1;
  BOOST_CHECK_EQUAL(strcmp(sTest1.getValue(), sTest2.getValue()), 0);
  BOOST_CHECK_EQUAL(sTest1.length(), 28);
  BOOST_CHECK_EQUAL(sTest2.length(), 28);
  sTest2 = cWStringTest2;
  BOOST_CHECK(0 != strcmp(sTest1.getValue(), sTest2.getValue()));
  BOOST_CHECK_EQUAL(sTest1 != sTest2, true);
  BOOST_CHECK_EQUAL(strcmp(cWStringTest2, sTest2.getValue()), 0);
}

BOOST_AUTO_TEST_CASE(String_compare)
{
  CIEC_WSTRING sTest1;
  CIEC_WSTRING sTest2;
  CIEC_WSTRING sTest3;

  const char cWStringTest1[] = "Check string!";
  char cWStringTest2[] = "Check string!";
  char cWStringTest3[] = "checkstring!";

  sTest1 = cWStringTest1;
  sTest2 = cWStringTest2;
  sTest3 = cWStringTest3;
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


  BOOST_CHECK(sTest1 == cWStringTest2);
  BOOST_CHECK_EQUAL(strcmp(sTest1.getValue(), cWStringTest1), 0);

  BOOST_CHECK(cWStringTest2 == sTest1);
  BOOST_CHECK_EQUAL(strcmp(sTest1.getValue(), cWStringTest1), 0);

  BOOST_CHECK(!(sTest1 == cWStringTest3));
  BOOST_CHECK_EQUAL(strcmp(sTest1.getValue(), cWStringTest1), 0);

  BOOST_CHECK(!(cWStringTest3 == sTest1));
  BOOST_CHECK_EQUAL(strcmp(sTest1.getValue(), cWStringTest1), 0);

  BOOST_CHECK(sTest2 != cWStringTest3);
  BOOST_CHECK_EQUAL(strcmp(sTest2.getValue(), cWStringTest2), 0);

  BOOST_CHECK(cWStringTest3 != sTest2);
  BOOST_CHECK_EQUAL(strcmp(sTest2.getValue(), cWStringTest2), 0);

  BOOST_CHECK(!(sTest2 != cWStringTest1));
  BOOST_CHECK_EQUAL(strcmp(sTest2.getValue(), cWStringTest2), 0);

  BOOST_CHECK(!(cWStringTest1 != sTest2));
  BOOST_CHECK_EQUAL(strcmp(sTest2.getValue(), cWStringTest2), 0);
}

BOOST_AUTO_TEST_CASE(Memory_Allocation)
{
  CIEC_WSTRING sTest;
  sTest.reserve(10);
  BOOST_CHECK_EQUAL(sTest.length(), 0);
  sTest = "Test";
  BOOST_CHECK_EQUAL(sTest.length(), 4);
  sTest = "Test with more than ten characters";
  BOOST_CHECK_EQUAL(sTest.length(), 34);
}

BOOST_AUTO_TEST_CASE(WString_fromUTF16)
{
  const TForteByte cBOMBE[] = { 0xFE, 0xFF };
  const TForteByte cBOMLE[] = { 0xFF, 0xFE };

  const TForteByte cWStringTest1Default[] = { 0x00, 0x7a };
  const TForteByte cWStringTest1BE[] = { 0xFE, 0xFF, 0x00, 0x7a };
  const TForteByte cWStringTest1LE[] = { 0xFF, 0xFE, 0x7a, 0x00 };

  // Japanese "nihongo", meaning "the Japanese language"
  const TForteByte cWStringTest2[] = { 0xFE, 0xFF, 0x65, 0xe5, 0x67, 0x2c, 0x8a, 0x9e };
  const TForteByte cWStringTest2Result[] = { 0xE6, 0x97, 0xA5, 0xE6, 0x9C, 0xAC, 0xE8, 0xAA, 0x9E, 0 };

  // a Chinese character meaning 'stump of tree'
  const TForteByte cWStringTest3[] = { 0xFE, 0xFF, 0xd8, 0x4c, 0xdf, 0xb4 };
  const TForteByte cWStringTest3Result[] = { 0xF0, 0xA3, 0x8E, 0xB4, 0 };

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
  BOOST_CHECK(! nRes);
}

BOOST_AUTO_TEST_CASE(WString_toUTF16)
{
  const TForteByte cWStringTest1Result[] = { 0x00, 0x7a };

  // Japanese "nihongo", meaning "the Japanese language"
  const TForteByte cWStringTest2[] = { 0xE6, 0x97, 0xA5, 0xE6, 0x9C, 0xAC, 0xE8, 0xAA, 0x9E, 0 };
  const TForteByte cWStringTest2Result[] = { 0x65, 0xe5, 0x67, 0x2c, 0x8a, 0x9e };

  // a Chinese character meaning 'stump of tree'
  const TForteByte cWStringTest3[] = { 0xF0, 0xA3, 0x8E, 0xB4, 0 };
  const TForteByte cWStringTest3Result[] = { 0xd8, 0x4c, 0xdf, 0xb4 };

  TForteByte cBuffer[32];

  int nRes;
  CIEC_WSTRING sTest;

  nRes = sTest.toUTF16(cBuffer, sizeof(cWStringTest1Result));
  BOOST_CHECK_EQUAL(nRes, 0);

  sTest = "z";
  nRes = sTest.toUTF16(cBuffer, sizeof(cWStringTest1Result));
  BOOST_CHECK_EQUAL(nRes, 2);
  BOOST_CHECK(! memcmp(cBuffer, cWStringTest1Result, 2));

  sTest = (const char *) cWStringTest2;
  nRes = sTest.toUTF16(cBuffer, sizeof(cWStringTest2Result));
  BOOST_CHECK_EQUAL(nRes, 6);
  BOOST_CHECK(! memcmp(cBuffer, cWStringTest2Result, 6));

  sTest = (const char *) cWStringTest3;
  nRes = sTest.toUTF16(cBuffer, sizeof(cWStringTest3Result));
  BOOST_CHECK_EQUAL(nRes, 4);
  BOOST_CHECK(! memcmp(cBuffer, cWStringTest3Result, 4));

  nRes = sTest.toUTF16(cBuffer, static_cast<unsigned int>(sizeof(cWStringTest3Result)-1));
  BOOST_CHECK_EQUAL(nRes, -1);
}

BOOST_AUTO_TEST_CASE(WString_fromUTF8)
{
  const TForteByte cASCII1[] = { 0 };
  const TForteByte cASCII2[] = { 'A', 0 };
  const TForteByte cASCII3[] = { 0x7f, 0 };
  const TForteByte cUpper1[] = { 'A', 0xc2, 0xa2, 'A', 0 };
  const TForteByte cUpper2[] = { 'A', 0xe2, 0x82, 0xac, 'B', 0 };
  const TForteByte cUpper2Capped[] = { 'A', 0xe2, 0x82, 0xac, 0 };
  const TForteByte cUpper3[] = { 0xf0, 0xa4, 0xad, 0xa2, 0 };
  const TForteByte cInvalid1[] = { 0x80, 0 };
  const TForteByte cInvalid2[] = { 0xfe, 0x80, 0x80, 0x80, 0x80, 0x80, 0 };

  int bRes;
  CIEC_WSTRING sTest;

  bRes = sTest.fromUTF8((const char *) cASCII1, -1, false);
  BOOST_CHECK_EQUAL(strlen((const char *)cASCII1), bRes);
  BOOST_CHECK_EQUAL(sTest.length(), 0);

  bRes = sTest.fromUTF8((const char *) cASCII2, -1, false);
  BOOST_CHECK_EQUAL(strlen((const char *)cASCII2), bRes);
  BOOST_CHECK(! strcmp("A", sTest.getValue()));

  bRes = sTest.fromUTF8((const char *) cASCII3, -1, false);
  BOOST_CHECK_EQUAL(strlen((const char *)cASCII3), bRes);
  BOOST_CHECK(! strcmp("\x7f", sTest.getValue()));

  bRes = sTest.fromUTF8((const char *) cUpper1, -1, false);
  BOOST_CHECK_EQUAL(strlen((const char *)cUpper1), bRes);
  BOOST_CHECK(! strcmp((const char *) cUpper1, sTest.getValue()));

  bRes = sTest.fromUTF8((const char *) cUpper2, -1, false);
  BOOST_CHECK_EQUAL(strlen((const char *)cUpper2), bRes);
  BOOST_CHECK(! strcmp((const char *) cUpper2, sTest.getValue()));

  bRes = sTest.fromUTF8((const char *) cUpper2, 4, false);
  BOOST_CHECK_EQUAL(4, bRes);  //as we cap to 4 we expect the number of taken bytes to be 4
  BOOST_CHECK(! strcmp((const char *) cUpper2Capped, sTest.getValue()));

  bRes = sTest.fromUTF8((const char *) cUpper3, -1, false);
  BOOST_CHECK_EQUAL(strlen((const char *) cUpper3), bRes);
  BOOST_CHECK(! strcmp("?", sTest.getValue()));

  bRes = sTest.fromUTF8((const char *) cInvalid1, -1, false);
  BOOST_CHECK_EQUAL(-1, bRes);

  bRes = sTest.fromUTF8((const char *) cInvalid2, -1, false);
  BOOST_CHECK_EQUAL(-1, bRes);
}

BOOST_AUTO_TEST_CASE(WString_toUTF8)
{
  const TForteByte cASCII1[] = { 0 };
  const TForteByte cASCII2[] = { 'A', 0 };
  const TForteByte cASCII3[] = { 0x7f, 0 };
  const TForteByte cUpper1[] = { 'A', 0xc2, 0xa2, 'A', 0 };
  const TForteByte cUpper2[] = { 'A', 0xe2, 0x82, 0xac, 'B', 0 };
  const TForteByte cUpper3[] = { 'A', 0xe2, 0x82, 0xac, '$', 'B', 0 };
  const TForteByte cUpper3Escaped[] = { '"', 'A', 0xe2, 0x82, 0xac, '$', '$', 'B', '"', 0 };

  int nRes;
  CIEC_WSTRING sTest;
  char sResult[32];

  sTest = "";
  sResult[0] = '\0';
  nRes = sTest.toUTF8(sResult, sizeof(sResult), false);
  BOOST_CHECK_EQUAL(nRes, 0);
  BOOST_CHECK(! memcmp(sResult, cASCII1, sizeof(cASCII1)));

  sTest = "A";
  sResult[0] = '\0';
  nRes = sTest.toUTF8(sResult, sizeof(sResult), false);
  BOOST_CHECK_EQUAL(nRes, 1);
  BOOST_CHECK(! memcmp(sResult, cASCII2, sizeof(cASCII2)));

  sTest = "\x7f";
  sResult[0] = '\0';
  nRes = sTest.toUTF8(sResult, sizeof(sResult), false);
  BOOST_CHECK_EQUAL(nRes, 1);
  BOOST_CHECK(! memcmp(sResult, cASCII3, sizeof(cASCII3)));
  nRes = sTest.toUTF8((char *) sResult, 1, false);
  BOOST_CHECK_EQUAL(nRes, -1);
  nRes = sTest.toUTF8((char *) sResult, 2, false);
  BOOST_CHECK_EQUAL(nRes, 1);

  sTest = (const char *) cUpper1;
  sResult[0] = '\0';
  nRes = sTest.toUTF8(sResult, sizeof(sResult), false);
  BOOST_CHECK_EQUAL(nRes, 4);
  BOOST_CHECK(! memcmp(sResult, cUpper1, sizeof(cUpper1)));
  nRes = sTest.toUTF8(sResult, 4, false);
  BOOST_CHECK_EQUAL(nRes, -1);
  nRes = sTest.toUTF8(sResult, 5, false);
  BOOST_CHECK_EQUAL(nRes, 4);

  sTest = (const char *) cUpper2;
  sResult[0] = '\0';
  nRes = sTest.toUTF8(sResult, sizeof(sResult), false);
  BOOST_CHECK_EQUAL(nRes, 5);
  BOOST_CHECK(! memcmp(sResult, cUpper2, sizeof(cUpper2)));
  
  sTest = (const char *) cUpper3;
  sResult[0] = '\0';
  nRes = sTest.toUTF8(sResult, sizeof(sResult), true);
  BOOST_CHECK_EQUAL(nRes, 9);
  BOOST_CHECK(! memcmp(sResult, cUpper3Escaped, sizeof(cUpper3Escaped)));
}

const char cWStringTestLiteral1[] = "Test String";
const char cWStringTestResult1[] = "Test String";
const char cWStringTestToStringResult1[] = "\"Test String\"";
const char cWStringTestLiteral2[] = "\"This is another test string!\"";
const char cWStringTestResult2[] = "This is another test string!";
const char cWStringTestDollarLiteral[] ="\"$$\"";
const char cWStringTestDollarResult[] = "$";
const char cWStringTestDollarToStringResult[] = "\"$$\"";
const char cWStringTestLineFeedLiteral[] = "\"$L$l\"";
const char cWStringTestLineFeedResult[] = {0x10, 0x10, '\0'};
const char cWStringTestLineFeedToStringResult[] = "\"$l$l\"";
const char cWStringTestNewLineLiteral[] = "\"$N$n\"";
const char cWStringTestNewLineResult[] = "\n\n";
const char cWStringTestNewLineToStringResult[] = "\"$n$n\"";
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


BOOST_AUTO_TEST_CASE(WString_fromString)
{
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

  BOOST_CHECK_EQUAL(strlen(cWStringTestEscapedCharacterLiteral), sTestee.fromString(cWStringTestEscapedCharacterLiteral));
  BOOST_CHECK_EQUAL(sTestee.length(), strlen(cWStringTestEscapedCharacterResult));
  BOOST_CHECK_EQUAL(0, strcmp(sTestee.getValue(), cWStringTestEscapedCharacterResult));

  BOOST_CHECK_EQUAL(strlen(cWStringTestEscapedWCharacterLiteral), sTestee.fromString(cWStringTestEscapedWCharacterLiteral));
  BOOST_CHECK_EQUAL(sTestee.length(), strlen(cWStringTestEscapedWCharacterResult));
  BOOST_CHECK_EQUAL(0, strcmp(sTestee.getValue(), cWStringTestEscapedWCharacterResult));
}

void wStringTypedFromString(const std::string &pa_sSrc, const char* pa_acResult){
  CIEC_WSTRING sTestee;
  BOOST_CHECK_EQUAL(pa_sSrc.length(), sTestee.fromString(pa_sSrc.c_str()));
  BOOST_CHECK_EQUAL(sTestee.length(), strlen(pa_acResult));
  BOOST_CHECK_EQUAL(0, strcmp(sTestee.getValue(), pa_acResult));
}

BOOST_AUTO_TEST_CASE(WString_fromString_typed)
{
  wStringTypedFromString(std::string("WSTRING#") + cWStringTestLiteral1, cWStringTestResult1);
  wStringTypedFromString(std::string("WSTRING#") + cWStringTestLiteral2, cWStringTestResult2);
  wStringTypedFromString(std::string("WSTRING#") + cWStringTestDollarLiteral, cWStringTestDollarResult);
}

BOOST_AUTO_TEST_CASE(WString_toString)
{
  CIEC_WSTRING sTestee;
  char acBuffer[200];

  sTestee = cWStringTestResult1;
  BOOST_CHECK_EQUAL(sTestee.toString(acBuffer, 200), strlen(cWStringTestToStringResult1));
  BOOST_CHECK_EQUAL(0, strncmp(cWStringTestToStringResult1, acBuffer, strlen(cWStringTestToStringResult1)));

  sTestee = cWStringTestResult2;
  BOOST_CHECK_EQUAL(sTestee.toString(acBuffer, 200), strlen(cWStringTestLiteral2));
  BOOST_CHECK_EQUAL(0, strncmp(cWStringTestLiteral2, acBuffer, strlen(cWStringTestLiteral2)));

  sTestee = cWStringTestDollarResult;
  BOOST_CHECK_EQUAL(sTestee.toString(acBuffer, 200), strlen(cWStringTestDollarToStringResult));
  BOOST_CHECK_EQUAL(0, strncmp(cWStringTestDollarToStringResult, acBuffer, strlen(cWStringTestDollarToStringResult)));

  sTestee = cWStringTestLineFeedResult;
  BOOST_CHECK_EQUAL(sTestee.toString(acBuffer, 200), strlen(cWStringTestLineFeedToStringResult));
  BOOST_CHECK_EQUAL(0, strncmp(cWStringTestLineFeedToStringResult, acBuffer, strlen(cWStringTestLineFeedToStringResult)));

  sTestee = cWStringTestNewLineResult;
  BOOST_CHECK_EQUAL(sTestee.toString(acBuffer, 200), strlen(cWStringTestNewLineToStringResult));
  BOOST_CHECK_EQUAL(0, strncmp(cWStringTestNewLineToStringResult, acBuffer, strlen(cWStringTestNewLineToStringResult)));

  sTestee = cWStringTestFormFeedResult;
  BOOST_CHECK_EQUAL(sTestee.toString(acBuffer, 200), strlen(cWStringTestFormFeedToStringResult));
  BOOST_CHECK_EQUAL(0, strncmp(cWStringTestFormFeedToStringResult, acBuffer, strlen(cWStringTestFormFeedToStringResult)));

  sTestee = cWStringTestCarriageReturnResult;
  BOOST_CHECK_EQUAL(sTestee.toString(acBuffer, 200), strlen(cWStringTestCarriageReturnToStringResult));
  BOOST_CHECK_EQUAL(0, strncmp(cWStringTestCarriageReturnToStringResult, acBuffer, strlen(cWStringTestCarriageReturnToStringResult)));

  sTestee = cWStringTestTabResult;
  BOOST_CHECK_EQUAL(sTestee.toString(acBuffer, 200), strlen(cWStringTestTabToStringResult));
  BOOST_CHECK_EQUAL(0, strncmp(cWStringTestTabToStringResult, acBuffer, strlen(cWStringTestTabToStringResult)));

  sTestee = cWStringTestSingleQuoteResult;
  BOOST_CHECK_EQUAL(sTestee.toString(acBuffer, 200), strlen(cWStringTestSingleQuoteToStringResult));
  BOOST_CHECK_EQUAL(0, strncmp(cWStringTestSingleQuoteToStringResult, acBuffer, strlen(cWStringTestSingleQuoteToStringResult)));

  sTestee = cWStringTestDoubleQuoteResult;
  BOOST_CHECK_EQUAL(sTestee.toString(acBuffer, 200), strlen(cWStringTestDoubleQuoteToStringResult));
  BOOST_CHECK_EQUAL(0, strncmp(cWStringTestDoubleQuoteToStringResult, acBuffer, strlen(cWStringTestDoubleQuoteToStringResult)));

  sTestee = cWStringTestEscapedCharacterResult;
  BOOST_CHECK_EQUAL(sTestee.toString(acBuffer, 200), strlen(cWStringTestEscapedCharacterToStringResult));
  BOOST_CHECK_EQUAL(0, strncmp(cWStringTestEscapedCharacterToStringResult, acBuffer, strlen(cWStringTestEscapedCharacterToStringResult)));
}

BOOST_AUTO_TEST_CASE(WString_getToStringBufferSize_NoSpecialSymbols)
{
  CIEC_WSTRING testString("4diac 4 ever!");

  unsigned int bufferSize = testString.getToStringBufferSize();
  BOOST_CHECK_EQUAL(13 + 2 + 1, bufferSize);
}

BOOST_AUTO_TEST_CASE(WString_getToStringBufferSize_Dollar)
{
  CIEC_WSTRING testString("$");

  unsigned int bufferSize = testString.getToStringBufferSize();
  BOOST_CHECK_EQUAL(2 + 2 + 1, bufferSize); // "$$"\0
}

BOOST_AUTO_TEST_CASE(WString_getToStringBufferSize_SingleQuote)
{
  CIEC_WSTRING testString("\'");

  unsigned int bufferSize = testString.getToStringBufferSize();
  BOOST_CHECK_EQUAL(1 + 2 + 1, bufferSize); // "'"\0
}

BOOST_AUTO_TEST_CASE(WString_getToStringBufferSize_DoubleQuote)
{
  CIEC_WSTRING testString("\"");

  unsigned int bufferSize = testString.getToStringBufferSize();
  BOOST_CHECK_EQUAL(2 + 2 + 1, bufferSize); // "$""\0
}

BOOST_AUTO_TEST_CASE(WString_getToStringBufferSize_LineFeed)
{
  CIEC_WSTRING testString("\x10");

  unsigned int bufferSize = testString.getToStringBufferSize();
  BOOST_CHECK_EQUAL(2 + 2 + 1, bufferSize); // "$L"\0
}

BOOST_AUTO_TEST_CASE(WString_getToStringBufferSize_NewLine)
{
  CIEC_WSTRING testString("\n");

  unsigned int bufferSize = testString.getToStringBufferSize();
  BOOST_CHECK_EQUAL(2 + 2 + 1, bufferSize); // "$N"\0
}

BOOST_AUTO_TEST_CASE(WString_getToStringBufferSize_FormFeed)
{
  CIEC_WSTRING testString("\f");

  unsigned int bufferSize = testString.getToStringBufferSize();
  BOOST_CHECK_EQUAL(2 + 2 + 1, bufferSize); // "$P"\0
}

BOOST_AUTO_TEST_CASE(WString_getToStringBufferSize_CarriageReturn)
{
  CIEC_WSTRING testString("\r");

  unsigned int bufferSize = testString.getToStringBufferSize();
  BOOST_CHECK_EQUAL(2 + 2 + 1, bufferSize); // "$R"\0
}

BOOST_AUTO_TEST_CASE(WString_getToStringBufferSize_Tab)
{
  CIEC_WSTRING testString("\t");

  unsigned int bufferSize = testString.getToStringBufferSize();
  BOOST_CHECK_EQUAL(2 + 2 + 1, bufferSize); // "$T"\0
}

BOOST_AUTO_TEST_CASE(WString_getToStringBufferSize_NonCommonSymbol_1ByteUsed)
{
  CIEC_WSTRING testString("\x8A");

  unsigned int bufferSize = testString.getToStringBufferSize();
  BOOST_CHECK_EQUAL(5 + 2 + 1, bufferSize); // "$008A"\0
}

BOOST_AUTO_TEST_CASE(WString_getToStringBufferSize_NonCommonSymbol_2BytesUsed)
{
  CIEC_WSTRING testString("\xDA\x8A");

  unsigned int bufferSize = testString.getToStringBufferSize();
  BOOST_CHECK_EQUAL(5 + 2 + 1, bufferSize); // "$008A"\0
}

BOOST_AUTO_TEST_CASE(WString_getToStringBufferSize_NonCommonSymbol_Two_OneBytesUsed)
{
  CIEC_WSTRING testString("\x8B\x8A");

  unsigned int bufferSize = testString.getToStringBufferSize();
  BOOST_CHECK_EQUAL(5 + 5 + 2 + 1, bufferSize); // "$008A"\0
}

BOOST_AUTO_TEST_SUITE_END()
