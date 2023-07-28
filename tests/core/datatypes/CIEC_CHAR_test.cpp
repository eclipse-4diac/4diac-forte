/*******************************************************************************
 * Copyright (c) 2022 Primetals Technologies Austria GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Martin Melik Merkumians - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include <boost/test/unit_test.hpp>
#include "forte_boost_output_support.h"

#include "../../../src/core/datatypes/forte_char.h"

BOOST_AUTO_TEST_SUITE(CIEC_CHAR_function_test)

BOOST_AUTO_TEST_CASE(Type_test)
{
  CIEC_CHAR test;
  //check type information
  BOOST_CHECK_EQUAL(test.getDataTypeID(), CIEC_ANY::e_CHAR);
  //check operator bool data type size
  BOOST_CHECK_EQUAL(sizeof(test.operator TForteChar()), sizeof(TForteChar));
}

BOOST_AUTO_TEST_CASE(Char_literal)
{
  CIEC_CHAR test1 = 'a'_CHAR;
  BOOST_TEST(static_cast<CIEC_CHAR::TValueType>(test1) == 'a');

  CIEC_CHAR test2 = '\0'_CHAR;
  BOOST_TEST(static_cast<CIEC_CHAR::TValueType>(test2) == '\0');

  CIEC_CHAR test3 = 0x30_CHAR;
  BOOST_TEST(static_cast<CIEC_CHAR::TValueType>(test3) == 0x30);
}

BOOST_AUTO_TEST_CASE(Assignment_test)
{
  CIEC_CHAR test1;
  CIEC_CHAR test2;

  //initial value must be 0
  BOOST_CHECK_EQUAL(static_cast<CIEC_CHAR::TValueType>(test1), 0);

  test1 = CIEC_CHAR(0);
  test2 = test1;
  BOOST_CHECK_EQUAL(static_cast<CIEC_CHAR::TValueType>(test1), 0);
  BOOST_CHECK_EQUAL(static_cast<CIEC_CHAR::TValueType>(test2), 0);

  test1 = CIEC_CHAR(12);
  test2 = test1;
  BOOST_CHECK_EQUAL(static_cast<CIEC_CHAR::TValueType>(test1), 12);
  BOOST_CHECK_EQUAL(static_cast<CIEC_CHAR::TValueType>(test2), 12);

  test1 = CIEC_CHAR(255);
  test2 = test1;
  BOOST_CHECK_EQUAL(static_cast<CIEC_CHAR::TValueType>(test1), 255);
  BOOST_CHECK_EQUAL(static_cast<CIEC_CHAR::TValueType>(test2), 255);

  //check that assignment operator does not intertwine objects
  test2 = CIEC_CHAR(128);
  BOOST_CHECK_EQUAL(static_cast<CIEC_CHAR::TValueType>(test1), 255);
  BOOST_CHECK_EQUAL(static_cast<CIEC_CHAR::TValueType>(test2), 128);

}

BOOST_AUTO_TEST_CASE(ToString_test){
  CIEC_CHAR test('a');
  char toStringBuffer[11];
  const char expected[] = "CHAR#'a'";
  BOOST_CHECK_EQUAL(test.toString(toStringBuffer, sizeof(toStringBuffer)), sizeof(expected) -1);
  BOOST_CHECK_EQUAL(toStringBuffer, expected);
}

BOOST_AUTO_TEST_CASE(ToStringEmpty_test){
  CIEC_CHAR test;
  char toStringBuffer[11];
  const char expected[] = "CHAR#''";
  BOOST_CHECK_EQUAL(test.toString(toStringBuffer, sizeof(toStringBuffer)), sizeof(expected) -1);
  BOOST_CHECK_EQUAL(toStringBuffer, expected);
}

BOOST_AUTO_TEST_CASE(ToStringDollar_test){
  CIEC_CHAR test('$');
  char toStringBuffer[11];
  const char expected[] = "CHAR#'$$'";
  BOOST_CHECK_EQUAL(test.toString(toStringBuffer, sizeof(toStringBuffer)), sizeof(expected) -1);
  BOOST_CHECK_EQUAL(toStringBuffer, expected);
}

BOOST_AUTO_TEST_CASE(ToStringSingleQuote_test){
  CIEC_CHAR test('\'');
  char toStringBuffer[11];
  const char expected[] = "CHAR#'$''";
  BOOST_CHECK_EQUAL(test.toString(toStringBuffer, sizeof(toStringBuffer)), sizeof(expected) -1);
  BOOST_CHECK_EQUAL(toStringBuffer, expected);
}

BOOST_AUTO_TEST_CASE(ToStringLineFeed_NewLine_test){
  CIEC_CHAR test('\n');
  char toStringBuffer[11];
  const char expected[] = "CHAR#'$N'";
  BOOST_CHECK_EQUAL(test.toString(toStringBuffer, sizeof(toStringBuffer)), sizeof(expected) -1);
  BOOST_CHECK_EQUAL(toStringBuffer, expected);
}

BOOST_AUTO_TEST_CASE(ToStringPageFeed_test){
  CIEC_CHAR test('\f');
  char toStringBuffer[11];
  const char expected[] = "CHAR#'$P'";
  BOOST_CHECK_EQUAL(test.toString(toStringBuffer, sizeof(toStringBuffer)), sizeof(expected) -1);
  BOOST_CHECK_EQUAL(toStringBuffer, expected);
}

BOOST_AUTO_TEST_CASE(ToStringCarriageReturn_test){
  CIEC_CHAR test('\r');
  char toStringBuffer[11];
  const char expected[] = "CHAR#'$R'";
  BOOST_CHECK_EQUAL(test.toString(toStringBuffer, sizeof(toStringBuffer)), sizeof(expected) -1);
  BOOST_CHECK_EQUAL(toStringBuffer, expected);
}

BOOST_AUTO_TEST_CASE(ToStringTab_test){
  CIEC_CHAR test('\t');
  char toStringBuffer[11];
  const char expected[] = "CHAR#'$T'";
  BOOST_CHECK_EQUAL(test.toString(toStringBuffer, sizeof(toStringBuffer)), sizeof(expected) -1);
  BOOST_CHECK_EQUAL(toStringBuffer, expected);
}

BOOST_AUTO_TEST_CASE(ToStringFormFeed_test)
{
  CIEC_CHAR test('\f');
  char toStringBuffer[11];
  const char expected[] = "CHAR#'$P'";
  BOOST_CHECK_EQUAL(test.toString(toStringBuffer, sizeof(toStringBuffer)), sizeof(expected) - 1);
  BOOST_CHECK_EQUAL(toStringBuffer, expected);
}

BOOST_AUTO_TEST_CASE(FromString_test){
  CIEC_CHAR test;
  char toStringBuffer[11];
  const char expected[] = "CHAR#'A'";
  BOOST_CHECK_EQUAL(test.fromString(expected), sizeof(expected) - 1);
  BOOST_CHECK_EQUAL(test.toString(toStringBuffer, sizeof(toStringBuffer)), sizeof(expected) -1);
  BOOST_CHECK_EQUAL(toStringBuffer, expected);
}

BOOST_AUTO_TEST_CASE(FromStringEmpty_test){
  CIEC_CHAR test;
  char toStringBuffer[11];
  const char expected[] = "CHAR#''";
  BOOST_CHECK_EQUAL(test.fromString(expected), sizeof(expected) - 1);
  BOOST_CHECK_EQUAL(test.toString(toStringBuffer, sizeof(toStringBuffer)),sizeof(expected) - 1);
  BOOST_CHECK_EQUAL(toStringBuffer, expected);
}

BOOST_AUTO_TEST_CASE(FromStringDollar_test){
  CIEC_CHAR test;
  char toStringBuffer[11];
  const char expected[] = "CHAR#'$$'";
  BOOST_CHECK_EQUAL(test.fromString(expected), sizeof(expected) - 1);
  BOOST_CHECK_EQUAL(test.toString(toStringBuffer, sizeof(toStringBuffer)),9);
  BOOST_CHECK_EQUAL(toStringBuffer, expected);
}

BOOST_AUTO_TEST_CASE(FromStringQuote_test){
  CIEC_CHAR test;
  char toStringBuffer[11];
  const char expected[] = "CHAR#'$''";
  BOOST_CHECK_EQUAL(test.fromString(expected), sizeof(expected) - 1);
  BOOST_CHECK_EQUAL(test.toString(toStringBuffer, sizeof(toStringBuffer)),9);
  BOOST_CHECK_EQUAL(toStringBuffer, expected);
}

BOOST_AUTO_TEST_CASE(FromStringLineFeed_test){
  CIEC_CHAR test;
  char toStringBuffer[11];
  const char source[] = "CHAR#'$L'";
  const char expected[] = "CHAR#'$N'";
  BOOST_CHECK_EQUAL(test.fromString(source), sizeof(source) - 1);
  BOOST_CHECK_EQUAL(test.toString(toStringBuffer, sizeof(toStringBuffer)),9);
  BOOST_CHECK_EQUAL(toStringBuffer, expected);
}

BOOST_AUTO_TEST_CASE(FromStringNewLine_test){
  CIEC_CHAR test;
  char toStringBuffer[11];
  const char expected[] = "CHAR#'$N'";
  BOOST_CHECK_EQUAL(test.fromString(expected), sizeof(expected) - 1);
  BOOST_CHECK_EQUAL(test.toString(toStringBuffer, sizeof(toStringBuffer)),9);
  BOOST_CHECK_EQUAL(toStringBuffer, expected);
}

BOOST_AUTO_TEST_CASE(FromStringPageFeed_test){
  CIEC_CHAR test;
  char toStringBuffer[11];
  const char expected[] = "CHAR#'$P'";
  BOOST_CHECK_EQUAL(test.fromString(expected), sizeof(expected) - 1);
  BOOST_CHECK_EQUAL(test.toString(toStringBuffer, sizeof(toStringBuffer)),9);
  BOOST_CHECK_EQUAL(toStringBuffer, expected);
}

BOOST_AUTO_TEST_CASE(FromStringCarriageReturn_test){
  CIEC_CHAR test;
  char toStringBuffer[11];
  const char expected[] = "CHAR#'$R'";
  BOOST_CHECK_EQUAL(test.fromString(expected), sizeof(expected) - 1);
  BOOST_CHECK_EQUAL(test.toString(toStringBuffer, sizeof(toStringBuffer)),9);
  BOOST_CHECK_EQUAL(toStringBuffer, expected);
}

BOOST_AUTO_TEST_CASE(FromStringTab_test){
  CIEC_CHAR test;
  char toStringBuffer[11];
  const char expected[] = "CHAR#'$T'";
  BOOST_CHECK_EQUAL(test.fromString(expected), sizeof(expected) - 1);
  BOOST_CHECK_EQUAL(test.toString(toStringBuffer, sizeof(toStringBuffer)),9);
  BOOST_CHECK_EQUAL(toStringBuffer, expected);
}

BOOST_AUTO_TEST_CASE(FromStringCodePoint_test){
  CIEC_CHAR test;
  char toStringBuffer[11];
  const char source[] = "CHAR#'$0A'";
  const char expected[] = "CHAR#'$N'";
  BOOST_CHECK_EQUAL(test.fromString(source), sizeof(source) - 1);
  BOOST_CHECK_EQUAL(test.toString(toStringBuffer, sizeof(toStringBuffer)),9);
  BOOST_CHECK_EQUAL(toStringBuffer, expected);
}

BOOST_AUTO_TEST_SUITE_END()
