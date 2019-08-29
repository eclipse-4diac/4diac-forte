/*******************************************************************************
 * Copyright (c) 2011 - 2013 ACIN, nxtControl, Profactor GmbH, fortiss GmbH
 *   2018 TU Wien/ACIN
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
 *******************************************************************************/
#include <boost/test/unit_test.hpp>

#include "../../../src/core/datatypes/forte_string.h"

BOOST_AUTO_TEST_SUITE(CIEC_STRING_function_test)
BOOST_AUTO_TEST_CASE(Type_test)
{
  CIEC_STRING sTest;
  //check type information
  BOOST_CHECK_EQUAL(sTest.getDataTypeID(), CIEC_ANY::e_STRING);
  //check operator const char* data type size
  BOOST_CHECK_EQUAL(sizeof(sTest.getValue()), sizeof(char*));

}

BOOST_AUTO_TEST_CASE(String_length)
{
  CIEC_STRING test1;
  BOOST_CHECK_EQUAL(test1.length(), 0);

  CIEC_STRING test2 = "\0";
  BOOST_CHECK_EQUAL(test2.length(), 0);

  CIEC_STRING test3 = "";
  BOOST_CHECK_EQUAL(test3.length(), 0);

  CIEC_STRING test4 = "1\03"; // {'1', '3', '\0'}
  BOOST_CHECK_EQUAL(test4.length(), 2);

  CIEC_STRING test5 = "123456789";
  BOOST_CHECK_EQUAL(test5.length(), 9);

}

BOOST_AUTO_TEST_CASE(String_empty)
{
  CIEC_STRING test1;
  BOOST_CHECK(test1.empty());

  CIEC_STRING test2 = "\0";
  BOOST_CHECK(test2.empty());

  CIEC_STRING test3 = "";
  BOOST_CHECK(test3.empty());

  CIEC_STRING test4 = "1\03";  //{'1', '3', '\0'}
  BOOST_CHECK(!test4.empty());

  CIEC_STRING test5 = "12345789";
  BOOST_CHECK(!test5.empty());

}

BOOST_AUTO_TEST_CASE(String_manipulation_interface)
{
  CIEC_STRING sTest;
  char cTest[] = "This is a test string!"; //length 22 without trailing \0
  BOOST_CHECK_EQUAL(sTest.length(), 0);
  BOOST_CHECK(sTest.empty());

  sTest = cTest;
  BOOST_CHECK(! sTest.empty());
  BOOST_CHECK_EQUAL(sTest.length(), 22);
  BOOST_CHECK_EQUAL(strcmp(sTest.getValue(), cTest), 0);

  CIEC_STRING* psTest = new CIEC_STRING(cTest);
  BOOST_CHECK_EQUAL(psTest->length(), 22);
  BOOST_CHECK_EQUAL(strcmp(psTest->getValue(), cTest), 0);
  delete psTest;

}

BOOST_AUTO_TEST_CASE(String_assignment)
{
  CIEC_STRING sTest1;
  CIEC_STRING sTest2;
  char cTest1[] = "This is another test string!";
  char cTest2[] = "Check string!";

  sTest1 = cTest1;
  BOOST_CHECK_EQUAL(strcmp(sTest1.getValue(), cTest1), 0);

  sTest2 = sTest1;
  BOOST_CHECK_EQUAL(strcmp(sTest1.getValue(), cTest1), 0);

  BOOST_CHECK_EQUAL(strcmp(sTest1.getValue(), sTest2.getValue()), 0);
  BOOST_CHECK_EQUAL(sTest1.length(), 28);
  BOOST_CHECK_EQUAL(sTest2.length(), 28);
  sTest2 = cTest2;
  BOOST_CHECK((0 != strcmp(sTest1.getValue(), sTest2.getValue())));
}

BOOST_AUTO_TEST_CASE(String_clear)
{
  CIEC_STRING test1;
  test1.clear();
  BOOST_CHECK_EQUAL(test1.length(), 0);

  CIEC_STRING test2 = "";
  test2.clear();
  BOOST_CHECK_EQUAL(test2.length(), 0);

  CIEC_STRING test3 = "\0";
  test3.clear();
  BOOST_CHECK_EQUAL(test3.length(), 0);

  CIEC_STRING test4 = "1\03";  //{'1', '3', '\0'}
  test4.clear();
  BOOST_CHECK_EQUAL(test4.length(), 0);

  CIEC_STRING test5 = "123456789";
  test5.clear();
  BOOST_CHECK_EQUAL(test5.length(), 0);

}

BOOST_AUTO_TEST_CASE(String_re_assignment)
{
  CIEC_STRING test1 = "123456789";
  BOOST_CHECK_EQUAL(test1.length(), 9);

  test1.clear();
  BOOST_CHECK_EQUAL(test1.length(), 0);

  test1.assign("123456789", 9);
  BOOST_CHECK_EQUAL(test1.length(), 9);

  test1.assign("123456", 6); //smaller string
  BOOST_CHECK_EQUAL(test1.length(), 6);

  test1.assign("12345678", 8); //bigger string
  BOOST_CHECK_EQUAL(test1.length(), 8);

  test1.assign("1234567890", 9); //length to assign smaller than actual string coming from a smaller string
  BOOST_CHECK_EQUAL(test1.length(), 9);

  test1.assign("1234", 3);//length to assign smaller than actual string coming from a bigger string
  BOOST_CHECK_EQUAL(test1.length(), 3);

  test1.assign(0, 0);//shouldn't do anything
  BOOST_CHECK_EQUAL(test1.length(), 3);

  test1.assign("\0", 0);
  BOOST_CHECK_EQUAL(test1.length(), 0);

  test1.assign("0", 1);
  BOOST_CHECK_EQUAL(test1.length(), 1);

  test1.assign("", 0);
  BOOST_CHECK_EQUAL(test1.length(), 0);
}

BOOST_AUTO_TEST_CASE(String_append)
{
  CIEC_STRING test1 = "123456789";
  BOOST_CHECK_EQUAL(test1.length(), 9);

  test1.append("");
  BOOST_CHECK_EQUAL(test1.length(), 9);
  BOOST_CHECK_EQUAL(strcmp(test1.getValue(), "123456789"), 0);

  test1.append("0");
  BOOST_CHECK_EQUAL(test1.length(), 10);
  BOOST_CHECK_EQUAL(strcmp(test1.getValue(), "1234567890"), 0);

  test1.append("\0");
  BOOST_CHECK_EQUAL(test1.length(), 10);
  BOOST_CHECK_EQUAL(strcmp(test1.getValue(), "1234567890"), 0);

  test1.append("123", 1);
  BOOST_CHECK_EQUAL(test1.length(), 11);
  BOOST_CHECK_EQUAL(strcmp(test1.getValue(), "12345678901"), 0);

  test1.append("");
  BOOST_CHECK_EQUAL(test1.length(), 11);
  BOOST_CHECK_EQUAL(strcmp(test1.getValue(), "12345678901"), 0);

  test1.append(0, 0); //shouldn't do anything
  BOOST_CHECK_EQUAL(test1.length(), 11);
  BOOST_CHECK_EQUAL(strcmp(test1.getValue(), "12345678901"), 0);

  test1.append("1\03"); // {'1', '3', '\0'}
  BOOST_CHECK_EQUAL(test1.length(), 13);
  BOOST_CHECK_EQUAL(strcmp(test1.getValue(), "123456789011\3"), 0);

  test1.append(0, 0);//shouldn't do anything
  BOOST_CHECK_EQUAL(test1.length(), 13);
  BOOST_CHECK_EQUAL(strcmp(test1.getValue(), "123456789011\3"), 0);

  test1.clear();
  test1.append("\0"); //append to empty string
  BOOST_CHECK_EQUAL(test1.length(), 0);

  test1.append(""); //append to empty string
  BOOST_CHECK_EQUAL(test1.length(), 0);

}

BOOST_AUTO_TEST_CASE(String_compare)
{
  CIEC_STRING sTest1;
  CIEC_STRING sTest2;
  CIEC_STRING sTest3;

  char cTest1[] = "Check string!";
  char cTest2[] = "Check string!";
  char cTest3[] = "checkstring!";

  sTest1 = cTest1;
  sTest2 = cTest2;
  sTest3 = cTest3;
  BOOST_CHECK(sTest1 == sTest2);
  BOOST_CHECK_EQUAL(strcmp(sTest1.getValue(), cTest1), 0);
  BOOST_CHECK_EQUAL(strcmp(sTest2.getValue(), cTest2), 0);

  BOOST_CHECK(!(sTest1 == sTest3));
  BOOST_CHECK_EQUAL(strcmp(sTest1.getValue(), cTest1), 0);
  BOOST_CHECK_EQUAL(strcmp(sTest3.getValue(), cTest3), 0);

  BOOST_CHECK(sTest2 != sTest3);
  BOOST_CHECK_EQUAL(strcmp(sTest2.getValue(), cTest2), 0);
  BOOST_CHECK_EQUAL(strcmp(sTest3.getValue(), cTest3), 0);

  BOOST_CHECK(!(sTest2 != sTest1));
  BOOST_CHECK_EQUAL(strcmp(sTest2.getValue(), cTest2), 0);
  BOOST_CHECK_EQUAL(strcmp(sTest1.getValue(), cTest1), 0);
  

  BOOST_CHECK(sTest1 == cTest2);
  BOOST_CHECK_EQUAL(strcmp(sTest1.getValue(), cTest1), 0);

  BOOST_CHECK(cTest2 == sTest1);
  BOOST_CHECK_EQUAL(strcmp(sTest1.getValue(), cTest1), 0);

  BOOST_CHECK(!(sTest1 == cTest3));
  BOOST_CHECK_EQUAL(strcmp(sTest1.getValue(), cTest1), 0);

  BOOST_CHECK(!(cTest3 == sTest1));
  BOOST_CHECK_EQUAL(strcmp(sTest1.getValue(), cTest1), 0);

  BOOST_CHECK(sTest2 != cTest3);
  BOOST_CHECK_EQUAL(strcmp(sTest2.getValue(), cTest2), 0);

  BOOST_CHECK(cTest3 != sTest2);
  BOOST_CHECK_EQUAL(strcmp(sTest2.getValue(), cTest2), 0);

  BOOST_CHECK(!(sTest2 != cTest1));
  BOOST_CHECK_EQUAL(strcmp(sTest2.getValue(), cTest2), 0);

  BOOST_CHECK(!(cTest1 != sTest2));
  BOOST_CHECK_EQUAL(strcmp(sTest2.getValue(), cTest2), 0);
}

BOOST_AUTO_TEST_CASE(String_binary_interface)
{
  CIEC_STRING sTest1, sTest2;
  char cTest[] = "This is a test\0string!"; //embedded \0, length 22 without trailing \0
  BOOST_CHECK_EQUAL(sTest1.length(), 0);
  sTest1.assign(cTest, static_cast<TForteUInt16>(sizeof(cTest)-1));
  BOOST_CHECK_EQUAL(sTest1.length(), 22);
  BOOST_CHECK_EQUAL(memcmp(sTest1.getValue(), cTest, 22), 0);

  sTest2 = sTest1;
  BOOST_CHECK_EQUAL(sTest2.length(), 22);
  BOOST_CHECK_EQUAL(memcmp(sTest2.getValue(), cTest, 22), 0);

  CIEC_STRING* psTest = new CIEC_STRING(sTest1);
  BOOST_CHECK_EQUAL(psTest->length(), 22);
  BOOST_CHECK_EQUAL(memcmp(psTest->getValue(), cTest, 22), 0);
  delete psTest;
}

BOOST_AUTO_TEST_CASE(Memory_Allocation)
{
  CIEC_STRING sTest;

  BOOST_CHECK_EQUAL(sTest.length(), 0);
  BOOST_CHECK_EQUAL(strlen(sTest.getValue()),0);
  BOOST_CHECK_EQUAL(sTest.getValue()[0],'\0');
  sTest.reserve(10);
  BOOST_CHECK_EQUAL(sTest.length(), 0);
  BOOST_CHECK_EQUAL(strlen(sTest.getValue()),0);
  BOOST_CHECK_EQUAL(sTest.getValue()[0],'\0');
  sTest = "Test";
  BOOST_CHECK_EQUAL(sTest.length(), 4);
  sTest = "Test with more than ten characters";
  BOOST_CHECK_EQUAL(sTest.length(), 34);

  sTest = "Test Test";
  BOOST_CHECK_EQUAL(sTest.length(), 9);

  sTest = "";
  BOOST_CHECK_EQUAL(sTest.length(), 0);
}

BOOST_AUTO_TEST_CASE(String_fromUTF8)
{
  const TForteByte cASCII1[] = { 0 };
  const TForteByte cASCII2[] = { 'A', 0 };
  const TForteByte cASCII3[] = { 0x7f, 0 };
  const TForteByte cUpper1[] = { 'A', 0xc2, 0xa2, 'A', 0 };
  const TForteByte cUpper2[] = { 'A', 0xe2, 0x82, 0xac, 'B', 0 };
  const TForteByte cUpper3[] = { 0xf0, 0xa4, 0xad, 0xa2, 0 };
  const TForteByte cInvalid1[] = { 0x80, 0 };
  const TForteByte cInvalid2[] = { 0xfe, 0x80, 0x80, 0x80, 0x80, 0x80, 0 };

  int nRes;
  CIEC_STRING sTest;

  nRes = sTest.fromUTF8((const char *) cASCII1, -1, false);
  BOOST_CHECK_EQUAL(strlen((const char *)cASCII1), nRes);
  BOOST_CHECK_EQUAL(sTest.length(), 0);

  nRes = sTest.fromUTF8((const char *) cASCII2, -1, false);
  BOOST_CHECK_EQUAL(strlen((const char *)cASCII2), nRes);
  BOOST_CHECK(! strcmp("A", sTest.getValue()));

  nRes = sTest.fromUTF8((const char *) cASCII3, -1, false);
  BOOST_CHECK_EQUAL(strlen((const char *)cASCII3), nRes);
  BOOST_CHECK(! strcmp("\x7f", sTest.getValue()));

  nRes = sTest.fromUTF8((const char *) cUpper1, -1, false);
  BOOST_CHECK_EQUAL(strlen((const char *)cUpper1), nRes);
  BOOST_CHECK(! strcmp("A\xa2""A", sTest.getValue()));

  nRes = sTest.fromUTF8((const char *) cUpper2, -1, false);
  BOOST_CHECK_EQUAL(strlen((const char *)cUpper2), nRes);
  BOOST_CHECK(! strcmp("A?B", sTest.getValue()));

  nRes = sTest.fromUTF8((const char *) cUpper2, 4, false);
  BOOST_CHECK_EQUAL(4, nRes);
  BOOST_CHECK(! strcmp("A?", sTest.getValue()));

  nRes = sTest.fromUTF8((const char *) cUpper3, -1, false);
  BOOST_CHECK_EQUAL(strlen((const char *)cUpper3), nRes);
  BOOST_CHECK(! strcmp("?", sTest.getValue()));

  nRes = sTest.fromUTF8((const char *) cInvalid1, -1, false);
  BOOST_CHECK_EQUAL(-1, nRes);

  nRes = sTest.fromUTF8((const char *) cInvalid2, -1, false);
  BOOST_CHECK_EQUAL(-1, nRes);
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
  nRes = sTest.toUTF8(sResult, 1, false);
  BOOST_CHECK_EQUAL(nRes, -1);

  sTest = "\x7f";
  sResult[0] = '\0';
  nRes = sTest.toUTF8(sResult, sizeof(sResult), false);
  BOOST_CHECK_EQUAL(nRes, 1);
  BOOST_CHECK(! memcmp(sResult, cASCII3, sizeof(cASCII3)));
  nRes = sTest.toUTF8(sResult, 1, false);
  BOOST_CHECK_EQUAL(nRes, -1);
  nRes = sTest.toUTF8(sResult, 2, false);
  BOOST_CHECK_EQUAL(nRes, 1);

  sTest = "A\xa2""A";
  sResult[0] = '\0';
  nRes = sTest.toUTF8(sResult, sizeof(sResult), false);
  BOOST_CHECK_EQUAL(nRes, 4);
  BOOST_CHECK(! memcmp(sResult, cUpper1, sizeof(cUpper1)));
  nRes = sTest.toUTF8(sResult, 4, false);
  BOOST_CHECK_EQUAL(nRes, -1);
  nRes = sTest.toUTF8(sResult, 5, false);
  BOOST_CHECK_EQUAL(nRes, 4);
}

char cTestLiteral1[] = "Test String";
char cTestResult1[] = "Test String";
char cTestToStringResult1[] = "\'Test String\'";
char cTestLiteral2[] = "\'This is another test string!\'";
char cTestResult2[] = "This is another test string!";
char cTestDollarLiteral[] ="\'$$\'";
char cTestDollarResult[] = "$";
char cTestDollarToStringResult[] = "\'$$\'";
char cTestLineFeedLiteral[] = "\'$L$l\'";
char cTestLineFeedResult[] = {0x10, 0x10, '\0'};
char cTestLineFeedToStringResult[] = "\'$l$l\'";
char cTestNewLineLiteral[] = "\'$N$n\'";
char cTestNewLineResult[] = "\n\n";
char cTestNewLineToStringResult[] = "\'$n$n\'";
char cTestFormFeedLiteral[] = "\'$P$p\'";
char cTestFormFeedResult[] = "\f\f";
char cTestFormFeedToStringResult[] = "\'$p$p\'";
char cTestCarriageReturnLiteral[] = "\'$R$r\'";
char cTestCarriageReturnResult[] = "\r\r";
char cTestCarriageReturnToStringResult[] = "\'$r$r\'";
char cTestTabLiteral[] = "\'$T$t\'";
char cTestTabResult[] = "\t\t";
char cTestTabToStringResult[] = "\'$t$t\'";
char cTestSingleQuoteLiteral[] = "\'$\'\'";
char cTestSingleQuoteResult[] = "\'";
char cTestSingleQuoteToStringResult[] = "\'$\'\'";
char cTestDoubleQuoteLiteral[] = "\'$\"\'";
char cTestDoubleQuoteResult[] = "\"";
char cTestDoubleQuoteToStringResult[] = "\'$\"\'";
char cTestEscapedCharacterLiteral[] = "\'$30\'";
char cTestEscapedCharacterResult[] = "0";
char cTestEscapedCharacterToStringResult[] = "\'0\'";

BOOST_AUTO_TEST_CASE(String_fromString)
{
  CIEC_STRING sTestee;

  BOOST_CHECK_EQUAL(strlen(cTestLiteral1), sTestee.fromString(cTestLiteral1));
  BOOST_CHECK_EQUAL(sTestee.length(), strlen(cTestResult1));
  BOOST_CHECK_EQUAL(0, strcmp(sTestee.getValue(), cTestResult1));

  BOOST_CHECK_EQUAL(strlen(cTestLiteral2), sTestee.fromString(cTestLiteral2));
  BOOST_CHECK_EQUAL(sTestee.length(), strlen(cTestResult2));
  BOOST_CHECK_EQUAL(0, strcmp(sTestee.getValue(), cTestResult2));

  BOOST_CHECK_EQUAL(strlen(cTestDollarLiteral), sTestee.fromString(cTestDollarLiteral));
  BOOST_CHECK_EQUAL(sTestee.length(), strlen(cTestDollarResult));
  BOOST_CHECK_EQUAL(0, strcmp(sTestee.getValue(), cTestDollarResult));

  BOOST_CHECK_EQUAL(strlen(cTestLineFeedLiteral), sTestee.fromString(cTestLineFeedLiteral));
  BOOST_CHECK_EQUAL(sTestee.length(), strlen(cTestLineFeedResult));
  BOOST_CHECK_EQUAL(0, strcmp(sTestee.getValue(), cTestLineFeedResult));

  BOOST_CHECK_EQUAL(strlen(cTestDollarLiteral), sTestee.fromString(cTestDollarLiteral));
  BOOST_CHECK_EQUAL(sTestee.length(), strlen(cTestDollarResult));
  BOOST_CHECK_EQUAL(0, strcmp(sTestee.getValue(), cTestDollarResult));

  BOOST_CHECK_EQUAL(strlen(cTestFormFeedLiteral), sTestee.fromString(cTestFormFeedLiteral));
  BOOST_CHECK_EQUAL(sTestee.length(), strlen(cTestFormFeedResult));
  BOOST_CHECK_EQUAL(0, strcmp(sTestee.getValue(), cTestFormFeedResult));

  BOOST_CHECK_EQUAL(strlen(cTestCarriageReturnLiteral), sTestee.fromString(cTestCarriageReturnLiteral));
  BOOST_CHECK_EQUAL(sTestee.length(), strlen(cTestCarriageReturnResult));
  BOOST_CHECK_EQUAL(0, strcmp(sTestee.getValue(), cTestCarriageReturnResult));

  BOOST_CHECK_EQUAL(strlen(cTestTabLiteral), sTestee.fromString(cTestTabLiteral));
  BOOST_CHECK_EQUAL(sTestee.length(), strlen(cTestTabResult));
  BOOST_CHECK_EQUAL(0, strcmp(sTestee.getValue(), cTestTabResult));

  BOOST_CHECK_EQUAL(strlen(cTestSingleQuoteLiteral), sTestee.fromString(cTestSingleQuoteLiteral));
  BOOST_CHECK_EQUAL(sTestee.length(), strlen(cTestSingleQuoteResult));
  BOOST_CHECK_EQUAL(0, strcmp(sTestee.getValue(), cTestSingleQuoteResult));

  BOOST_CHECK_EQUAL(strlen(cTestDoubleQuoteLiteral), sTestee.fromString(cTestDoubleQuoteLiteral));
  BOOST_CHECK_EQUAL(sTestee.length(), strlen(cTestDoubleQuoteResult));
  BOOST_CHECK_EQUAL(0, strcmp(sTestee.getValue(), cTestDoubleQuoteResult));

  BOOST_CHECK_EQUAL(strlen(cTestEscapedCharacterLiteral), sTestee.fromString(cTestEscapedCharacterLiteral));
  BOOST_CHECK_EQUAL(sTestee.length(), strlen(cTestEscapedCharacterResult));
  BOOST_CHECK_EQUAL(0, strcmp(sTestee.getValue(), cTestEscapedCharacterResult));
}

void stringTypedFromString(const std::string &pa_sSrc, const char* pa_acResult){
  CIEC_STRING sTestee;
  BOOST_CHECK_EQUAL(pa_sSrc.length(), sTestee.fromString(pa_sSrc.c_str()));
  BOOST_CHECK_EQUAL(sTestee.length(), strlen(pa_acResult));
  BOOST_CHECK_EQUAL(0, strcmp(sTestee.getValue(), pa_acResult));
}

BOOST_AUTO_TEST_CASE(String_fromString_typed)
{
  stringTypedFromString(std::string("STRING#") + cTestLiteral1, cTestResult1);
  stringTypedFromString(std::string("STRING#") + cTestLiteral2, cTestResult2);
  stringTypedFromString(std::string("STRING#") + cTestDollarLiteral, cTestDollarResult);
}

BOOST_AUTO_TEST_CASE(String_toString)
{
  CIEC_STRING sTestee;
  char acBuffer[200];

  sTestee = cTestResult1;
  BOOST_CHECK_EQUAL(sTestee.toString(acBuffer, 200), strlen(cTestToStringResult1));
  BOOST_CHECK_EQUAL(0, strncmp(cTestToStringResult1, acBuffer, strlen(cTestToStringResult1)));

  sTestee = cTestResult2;
  BOOST_CHECK_EQUAL(sTestee.toString(acBuffer, 200), strlen(cTestLiteral2));
  BOOST_CHECK_EQUAL(0, strncmp(cTestLiteral2, acBuffer, strlen(cTestLiteral2)));

  sTestee = cTestDollarResult;
  BOOST_CHECK_EQUAL(sTestee.toString(acBuffer, 200), strlen(cTestDollarToStringResult));
  BOOST_CHECK_EQUAL(0, strncmp(cTestDollarToStringResult, acBuffer, strlen(cTestDollarToStringResult)));

  sTestee = cTestLineFeedResult;
  BOOST_CHECK_EQUAL(sTestee.toString(acBuffer, 200), strlen(cTestLineFeedToStringResult));
  BOOST_CHECK_EQUAL(0, strncmp(cTestLineFeedToStringResult, acBuffer, strlen(cTestLineFeedToStringResult)));

  sTestee = cTestNewLineResult;
  BOOST_CHECK_EQUAL(sTestee.toString(acBuffer, 200), strlen(cTestNewLineToStringResult));
  BOOST_CHECK_EQUAL(0, strncmp(cTestNewLineToStringResult, acBuffer, strlen(cTestNewLineToStringResult)));

  sTestee = cTestFormFeedResult;
  BOOST_CHECK_EQUAL(sTestee.toString(acBuffer, 200), strlen(cTestFormFeedToStringResult));
  BOOST_CHECK_EQUAL(0, strncmp(cTestFormFeedToStringResult, acBuffer, strlen(cTestFormFeedToStringResult)));

  sTestee = cTestCarriageReturnResult;
  BOOST_CHECK_EQUAL(sTestee.toString(acBuffer, 200), strlen(cTestCarriageReturnToStringResult));
  BOOST_CHECK_EQUAL(0, strncmp(cTestCarriageReturnToStringResult, acBuffer, strlen(cTestCarriageReturnToStringResult)));

  sTestee = cTestTabResult;
  BOOST_CHECK_EQUAL(sTestee.toString(acBuffer, 200), strlen(cTestTabToStringResult));
  BOOST_CHECK_EQUAL(0, strncmp(cTestTabToStringResult, acBuffer, strlen(cTestTabToStringResult)));

  sTestee = cTestSingleQuoteResult;
  BOOST_CHECK_EQUAL(sTestee.toString(acBuffer, 200), strlen(cTestSingleQuoteToStringResult));
  BOOST_CHECK_EQUAL(0, strncmp(cTestSingleQuoteToStringResult, acBuffer, strlen(cTestSingleQuoteToStringResult)));

  sTestee = cTestDoubleQuoteResult;
  BOOST_CHECK_EQUAL(sTestee.toString(acBuffer, 200), strlen(cTestDoubleQuoteToStringResult));
  BOOST_CHECK_EQUAL(0, strncmp(cTestDoubleQuoteToStringResult, acBuffer, strlen(cTestDoubleQuoteToStringResult)));

  sTestee = cTestEscapedCharacterResult;
  BOOST_CHECK_EQUAL(sTestee.toString(acBuffer, 200), strlen(cTestEscapedCharacterToStringResult));
  BOOST_CHECK_EQUAL(0, strncmp(cTestEscapedCharacterToStringResult, acBuffer, strlen(cTestEscapedCharacterToStringResult)));
}

BOOST_AUTO_TEST_CASE(String_toString_faultcase_buffer_size_zero)
{
  CIEC_STRING testString = "4diac 4 ever!";
  const size_t bufferSize = 50;
  char cStringBuffer[bufferSize];

  BOOST_CHECK_EQUAL(-1, testString.toString(cStringBuffer, 0));
}

BOOST_AUTO_TEST_CASE(String_toString_faultcase_buffer_pointer_nullptr)
{
  CIEC_STRING testString;
  const size_t bufferSize = 50;

  BOOST_CHECK_EQUAL(-1, testString.toString(0, bufferSize));
}

BOOST_AUTO_TEST_CASE(String_toString_faultcase_buffer_not_enough_buffer_size)
{
  const char sample_string[] = "4diac 4 ever!";
  CIEC_STRING testString(sample_string);
  const size_t bufferSize = sizeof(sample_string) + 2; // For enclosing single quotes
  char cStringBuffer[bufferSize];

  for(size_t i = 0; i < bufferSize; ++i) {
    BOOST_CHECK_EQUAL(-1, testString.toString(cStringBuffer, i));
  }
  BOOST_CHECK_EQUAL(bufferSize - 1, testString.toString(cStringBuffer, bufferSize)); // \0 is not counted
}

BOOST_AUTO_TEST_CASE(String_getToStringBufferSize_NoSpecialSymbols)
{
  CIEC_STRING testString("4diac 4 ever!");

  unsigned int bufferSize = testString.getToStringBufferSize();
  BOOST_CHECK_EQUAL(13 + 2 + 1, bufferSize);
}

BOOST_AUTO_TEST_CASE(String_getToStringBufferSize_Dollar)
{
  CIEC_STRING testString("$");

  unsigned int bufferSize = testString.getToStringBufferSize();
  BOOST_CHECK_EQUAL(2 + 2 + 1, bufferSize); // '$$'\0
}

BOOST_AUTO_TEST_CASE(String_getToStringBufferSize_SingleQuote)
{
  CIEC_STRING testString("\'");

  unsigned int bufferSize = testString.getToStringBufferSize();
  BOOST_CHECK_EQUAL(2 + 2 + 1, bufferSize); // '$''\0
}

BOOST_AUTO_TEST_CASE(String_getToStringBufferSize_DoubleQuote)
{
  CIEC_STRING testString("\"");

  unsigned int bufferSize = testString.getToStringBufferSize();
  BOOST_CHECK_EQUAL(1 + 2 + 1, bufferSize); // '"'\0
}

BOOST_AUTO_TEST_CASE(String_getToStringBufferSize_LineFeed)
{
  CIEC_STRING testString("\x10");

  unsigned int bufferSize = testString.getToStringBufferSize();
  BOOST_CHECK_EQUAL(2 + 2 + 1, bufferSize); // '$L'\0
}

BOOST_AUTO_TEST_CASE(String_getToStringBufferSize_NewLine)
{
  CIEC_STRING testString("\n");

  unsigned int bufferSize = testString.getToStringBufferSize();
  BOOST_CHECK_EQUAL(2 + 2 + 1, bufferSize); // '$N'\0
}

BOOST_AUTO_TEST_CASE(String_getToStringBufferSize_FormFeed)
{
  CIEC_STRING testString("\f");

  unsigned int bufferSize = testString.getToStringBufferSize();
  BOOST_CHECK_EQUAL(2 + 2 + 1, bufferSize); // '$P'\0
}

BOOST_AUTO_TEST_CASE(String_getToStringBufferSize_CarriageReturn)
{
  CIEC_STRING testString("\r");

  unsigned int bufferSize = testString.getToStringBufferSize();
  BOOST_CHECK_EQUAL(2 + 2 + 1, bufferSize); // '$R'\0
}

BOOST_AUTO_TEST_CASE(String_getToStringBufferSize_Tab)
{
  CIEC_STRING testString("\t");

  unsigned int bufferSize = testString.getToStringBufferSize();
  BOOST_CHECK_EQUAL(2 + 2 + 1, bufferSize); // '$T'\0
}

BOOST_AUTO_TEST_CASE(String_getToStringBufferSize_NonCommonSymbol)
{
  CIEC_STRING testString("\x8A");

  unsigned int bufferSize = testString.getToStringBufferSize();
  BOOST_CHECK_EQUAL(3 + 2 + 1, bufferSize); // '$8A'\0
}

BOOST_AUTO_TEST_SUITE_END()
