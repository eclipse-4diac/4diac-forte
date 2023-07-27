/*******************************************************************************
 * Copyright (c) 2022, 2023 Primetals Technologies Austria GmbH
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
 *******************************************************************************/
#include <boost/test/unit_test.hpp>
#include "forte_boost_output_support.h"

#include "../../../src/core/datatypes/forte_wchar.h"

BOOST_AUTO_TEST_SUITE(CIEC_WCHAR_function_test)

BOOST_AUTO_TEST_CASE(Type_test)
{
  CIEC_WCHAR test;
  //check type information
  BOOST_CHECK_EQUAL(test.getDataTypeID(), CIEC_ANY::e_WCHAR);
  //check operator bool data type size
  BOOST_CHECK_EQUAL(sizeof(test.operator TForteWChar()), sizeof(TForteWChar));
}

BOOST_AUTO_TEST_CASE(Literal_test)
{
  CIEC_WCHAR test1 = u'a'_WCHAR;
  BOOST_TEST(static_cast<CIEC_WCHAR::TValueType>(test1) == u'a');

  CIEC_WCHAR test2 = u'\0'_WCHAR;
  BOOST_TEST(static_cast<CIEC_WCHAR::TValueType>(test2) == u'\0');

  CIEC_WCHAR test3 = 0x30_WCHAR;
  BOOST_TEST(static_cast<CIEC_WCHAR::TValueType>(test3) == 0x30);
}

BOOST_AUTO_TEST_CASE(Assignment_test)
{
  CIEC_WCHAR test1;
  CIEC_WCHAR test2;

  //initial value must be 0
  BOOST_CHECK_EQUAL(static_cast<CIEC_WCHAR::TValueType>(test1), 0);

  test1 = CIEC_WCHAR(0);
  test2 = test1;
  BOOST_CHECK_EQUAL(static_cast<CIEC_WCHAR::TValueType>(test1), 0);
  BOOST_CHECK_EQUAL(static_cast<CIEC_WCHAR::TValueType>(test2), 0);

  test1 = CIEC_WCHAR(12);
  test2 = test1;
  BOOST_CHECK_EQUAL(static_cast<CIEC_WCHAR::TValueType>(test1), 12);
  BOOST_CHECK_EQUAL(static_cast<CIEC_WCHAR::TValueType>(test2), 12);

  test1 = CIEC_WCHAR(255);
  test2 = test1;
  BOOST_CHECK_EQUAL(static_cast<CIEC_WCHAR::TValueType>(test1), 255);
  BOOST_CHECK_EQUAL(static_cast<CIEC_WCHAR::TValueType>(test2), 255);

  //check that assignment operator does not intertwine objects
  test2 = CIEC_WCHAR(128);
  BOOST_CHECK_EQUAL(static_cast<CIEC_WCHAR::TValueType>(test1), 255);
  BOOST_CHECK_EQUAL(static_cast<CIEC_WCHAR::TValueType>(test2), 128);

}

BOOST_AUTO_TEST_CASE(ToString_test){
  CIEC_WCHAR test('a');
  char toStringBuffer[14];
  const char expected[] = "WCHAR#\"a\"";
  BOOST_CHECK_EQUAL(test.toString(toStringBuffer, sizeof(toStringBuffer)), sizeof(expected) -1);
  BOOST_CHECK_EQUAL(toStringBuffer, expected);
}

BOOST_AUTO_TEST_CASE(ToStringEmpty_test){
  CIEC_WCHAR test;
  char toStringBuffer[14];
  const char expected[] = "WCHAR#\"\"";
  BOOST_CHECK_EQUAL(test.toString(toStringBuffer, sizeof(toStringBuffer)), sizeof(expected) -1);
  BOOST_CHECK_EQUAL(toStringBuffer, expected);
}

BOOST_AUTO_TEST_CASE(ToStringDollar_test){
  CIEC_WCHAR test('$');
  char toStringBuffer[14];
  const char expected[] = "WCHAR#\"$$\"";
  BOOST_CHECK_EQUAL(test.toString(toStringBuffer, sizeof(toStringBuffer)), sizeof(expected) -1);
  BOOST_CHECK_EQUAL(toStringBuffer, expected);
}

BOOST_AUTO_TEST_CASE(ToStringSingleQuote_test){
  CIEC_WCHAR test('"');
  char toStringBuffer[14];
  const char expected[] = "WCHAR#\"$\"\"";
  BOOST_CHECK_EQUAL(test.toString(toStringBuffer, sizeof(toStringBuffer)), sizeof(expected) -1);
  BOOST_CHECK_EQUAL(toStringBuffer, expected);
}

BOOST_AUTO_TEST_CASE(ToStringLineFeed_NewLine_test){
  CIEC_WCHAR test('\n');
  char toStringBuffer[14];
  const char expected[] = "WCHAR#\"$N\"";
  BOOST_CHECK_EQUAL(test.toString(toStringBuffer, sizeof(toStringBuffer)), sizeof(expected) -1);
  BOOST_CHECK_EQUAL(toStringBuffer, expected);
}

BOOST_AUTO_TEST_CASE(ToStringPageFeed_test){
  CIEC_WCHAR test('\f');
  char toStringBuffer[14];
  const char expected[] = "WCHAR#\"$P\"";
  BOOST_CHECK_EQUAL(test.toString(toStringBuffer, sizeof(toStringBuffer)), sizeof(expected) -1);
  BOOST_CHECK_EQUAL(toStringBuffer, expected);
}

BOOST_AUTO_TEST_CASE(ToStringCarriageReturn_test){
  CIEC_WCHAR test('\r');
  char toStringBuffer[14];
  const char expected[] = "WCHAR#\"$R\"";
  BOOST_CHECK_EQUAL(test.toString(toStringBuffer, sizeof(toStringBuffer)), sizeof(expected) -1);
  BOOST_CHECK_EQUAL(toStringBuffer, expected);
}

BOOST_AUTO_TEST_CASE(ToStringTab_test){
  CIEC_WCHAR test('\t');
  char toStringBuffer[14];
  const char expected[] = "WCHAR#\"$T\"";
  BOOST_CHECK_EQUAL(test.toString(toStringBuffer, sizeof(toStringBuffer)), sizeof(expected) -1);
  BOOST_CHECK_EQUAL(toStringBuffer, expected);
}


BOOST_AUTO_TEST_CASE(FromString_test){
  CIEC_WCHAR test;
  char toStringBuffer[14];
  const char expected[] = "WCHAR#\"A\"";
  BOOST_CHECK_EQUAL(test.fromString(expected), sizeof(expected) - 1);
  BOOST_CHECK_EQUAL(test.toString(toStringBuffer, sizeof(toStringBuffer)), sizeof(expected) -1);
  BOOST_CHECK_EQUAL(toStringBuffer, expected);
}

BOOST_AUTO_TEST_CASE(FromStringEmpty_test){
  CIEC_WCHAR test;
  char toStringBuffer[14];
  const char expected[] = "WCHAR#\"\"";
  BOOST_CHECK_EQUAL(test.fromString(expected), sizeof(expected) - 1);
  BOOST_CHECK_EQUAL(test.toString(toStringBuffer, sizeof(toStringBuffer)),sizeof(expected) - 1);
  BOOST_CHECK_EQUAL(toStringBuffer, expected);
}

BOOST_AUTO_TEST_CASE(FromStringDollar_test){
  CIEC_WCHAR test;
  char toStringBuffer[14];
  const char expected[] = "WCHAR#\"$$\"";
  BOOST_CHECK_EQUAL(test.fromString(expected), sizeof(expected) - 1);
  BOOST_CHECK_EQUAL(test.toString(toStringBuffer, sizeof(toStringBuffer)),10);
  BOOST_CHECK_EQUAL(toStringBuffer, expected);
}

BOOST_AUTO_TEST_CASE(FromStringQuote_test){
  CIEC_WCHAR test;
  char toStringBuffer[14];
  const char expected[] = "WCHAR#\"$\"\"";
  BOOST_CHECK_EQUAL(test.fromString(expected), sizeof(expected) - 1);
  BOOST_CHECK_EQUAL(test.toString(toStringBuffer, sizeof(toStringBuffer)),10);
  BOOST_CHECK_EQUAL(toStringBuffer, expected);
}

BOOST_AUTO_TEST_CASE(FromStringLineFeed_test){
  CIEC_WCHAR test;
  char toStringBuffer[14];
  const char source[] = "WCHAR#\"$L\"";
  const char expected[] = "WCHAR#\"$N\"";
  BOOST_CHECK_EQUAL(test.fromString(source), sizeof(source) - 1);
  BOOST_CHECK_EQUAL(test.toString(toStringBuffer, sizeof(toStringBuffer)),10);
  BOOST_CHECK_EQUAL(toStringBuffer, expected);
}

BOOST_AUTO_TEST_CASE(FromStringNewLine_test){
  CIEC_WCHAR test;
  char toStringBuffer[14];
  const char expected[] = "WCHAR#\"$N\"";
  BOOST_CHECK_EQUAL(test.fromString(expected), sizeof(expected) - 1);
  BOOST_CHECK_EQUAL(test.toString(toStringBuffer, sizeof(toStringBuffer)),10);
  BOOST_CHECK_EQUAL(toStringBuffer, expected);
}

BOOST_AUTO_TEST_CASE(FromStringPageFeed_test){
  CIEC_WCHAR test;
  char toStringBuffer[14];
  const char expected[] = "WCHAR#\"$P\"";
  BOOST_CHECK_EQUAL(test.fromString(expected), sizeof(expected) - 1);
  BOOST_CHECK_EQUAL(test.toString(toStringBuffer, sizeof(toStringBuffer)),10);
  BOOST_CHECK_EQUAL(toStringBuffer, expected);
}

BOOST_AUTO_TEST_CASE(FromStringCarriageReturn_test){
  CIEC_WCHAR test;
  char toStringBuffer[14];
  const char expected[] = "WCHAR#\"$R\"";
  BOOST_CHECK_EQUAL(test.fromString(expected), sizeof(expected) - 1);
  BOOST_CHECK_EQUAL(test.toString(toStringBuffer, sizeof(toStringBuffer)),10);
  BOOST_CHECK_EQUAL(toStringBuffer, expected);
}

BOOST_AUTO_TEST_CASE(FromStringTab_test){
  CIEC_WCHAR test;
  char toStringBuffer[14];
  const char expected[] = "WCHAR#\"$T\"";
  BOOST_CHECK_EQUAL(test.fromString(expected), sizeof(expected) - 1);
  BOOST_CHECK_EQUAL(test.toString(toStringBuffer, sizeof(toStringBuffer)),10);
  BOOST_CHECK_EQUAL(toStringBuffer, expected);
}

BOOST_AUTO_TEST_CASE(FromStringCodePoint_test){
  CIEC_WCHAR test;
  char toStringBuffer[14];
  const char source[] = "WCHAR#\"$000A\"";
  const char expected[] = "WCHAR#\"$N\"";
  BOOST_CHECK_EQUAL(test.fromString(source), sizeof(source) - 1);
  BOOST_CHECK_EQUAL(test.toString(toStringBuffer, sizeof(toStringBuffer)),10);
  BOOST_CHECK_EQUAL(toStringBuffer, expected);
}

BOOST_AUTO_TEST_CASE(FromStringHighCodePoint_test){
  CIEC_WCHAR test;
  char toStringBuffer[14];
  const char source[] = "WCHAR#\"$A00A\"";
  const char expected[] = "WCHAR#\"$A00A\"";
  BOOST_CHECK_EQUAL(test.fromString(source), sizeof(source) - 1);
  BOOST_CHECK_EQUAL(test.toString(toStringBuffer, sizeof(toStringBuffer)),sizeof(source) - 1);
  BOOST_CHECK_EQUAL(toStringBuffer, expected);
}

BOOST_AUTO_TEST_SUITE_END()
