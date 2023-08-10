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

#include "../../../src/core/datatypes/forte_string.h"
#include "../../../src/core/datatypes/forte_char.h"

using namespace boost::unit_test;

BOOST_AUTO_TEST_SUITE(CIEC_ANY_STRING_PARTIAL)

BOOST_AUTO_TEST_SUITE(CIEC_STRING_PARTIAL)

BOOST_AUTO_TEST_CASE(PARTIAL_ACCESS_STRING_READ) {
  CIEC_STRING stringValue("Primetals"_STRING);
  CIEC_CHAR testChar;
  testChar = stringValue[5];
  BOOST_TEST('e'_CHAR == testChar);
}

BOOST_AUTO_TEST_CASE(PARTIAL_ACCESS_STRING_WRITE) {
  CIEC_STRING testString("Primxtals"_STRING);
  testString[5] = 'e'_CHAR;
  BOOST_TEST("Primetals"_STRING == testString);
}

BOOST_AUTO_TEST_CASE(PARTIAL_ACCESS_STRING_READ_FROM_CONST) {
  const CIEC_STRING stringValue("Primetals"_STRING);
  CIEC_CHAR testChar;
  testChar = stringValue[5];
  BOOST_TEST('e'_CHAR == testChar);
}

BOOST_AUTO_TEST_CASE(PARTIAL_ACCESS_STRING_READ_WITH_CIEC_TYPE_INDEX) {
  CIEC_STRING stringValue("Primetals"_STRING);
  CIEC_CHAR testChar;
  testChar = stringValue[CIEC_UDINT(5)];
  BOOST_TEST('e'_CHAR == testChar);
}

BOOST_AUTO_TEST_CASE(PARTIAL_ACCESS_STRING_WRITE_WITH_CIEC_TYPE_INDEX) {
  CIEC_STRING testString("Primxtals"_STRING);
  testString[CIEC_UDINT(5)] = 'e'_CHAR;
  BOOST_TEST("Primetals"_STRING == testString);
}

BOOST_AUTO_TEST_CASE(PARTIAL_ACCESS_STRING_READ_FROM_CONST_WITH_CIEC_TYPE_INDEX) {
  const CIEC_STRING stringValue("Primetals"_STRING);
  CIEC_CHAR testChar;
  testChar = stringValue[CIEC_UDINT(5)];
  BOOST_TEST('e'_CHAR == testChar);
}

BOOST_AUTO_TEST_CASE(PARTIAL_ACCESS_STRING_READ_AT_INDEX_0) {
  // Vendor-specific behaviour for returning NUL and emitting an error
  CIEC_STRING stringValue("Primetals"_STRING);
  CIEC_CHAR testChar;
  testChar = stringValue[0];
  BOOST_TEST('\0'_CHAR == testChar);
}

BOOST_AUTO_TEST_CASE(PARTIAL_ACCESS_STRING_WRITE_AT_INDEX_0) {
  // Vendor-specific behaviour emitting an error and ignore the write
  CIEC_STRING testString("Primxtals"_STRING);
  testString[0] = 'e'_CHAR;
  BOOST_TEST("Primxtals"_STRING == testString);
}

/* These tests need also to be checked in the log, as both exhibit the 
 * same result, but the latter two emit an error at the devlog console */
BOOST_AUTO_TEST_CASE(PARTIAL_ACCESS_STRING_READ_AT_INDEX_END) {
  CIEC_STRING stringValue("Primetals"_STRING);
  CIEC_CHAR testChar;
  testChar = stringValue[9];
  BOOST_TEST('s'_CHAR == testChar);
}

BOOST_AUTO_TEST_CASE(PARTIAL_ACCESS_STRING_WRITE_AT_INDEX_END) {
  CIEC_STRING testString("Primxtals"_STRING);
  testString[9] = 'e'_CHAR;
  BOOST_TEST("Primxtale"_STRING == testString);
}
BOOST_AUTO_TEST_CASE(PARTIAL_ACCESS_STRING_READ_AT_INDEX_PAST_END) {
  // Vendor-specific behaviour for returning NUL and emitting an error
  CIEC_STRING stringValue("Primetals"_STRING);
  CIEC_CHAR testChar;
  BOOST_TEST(stringValue.length() == 9);
  testChar = stringValue[10];
  BOOST_TEST('\0'_CHAR == testChar);
  BOOST_TEST(stringValue.length() == 9); // length must remain constant
}

BOOST_AUTO_TEST_CASE(PARTIAL_ACCESS_STRING_WRITE_AT_INDEX_PAST_END) {
  /// Vendor-specific behaviour fill all symbols between requested index and former last symbol with \0
  CIEC_STRING testString("Primxtals"_STRING);
  testString[10] = 'e'_CHAR;
  BOOST_TEST("Primxtalse"_STRING == testString);
}

BOOST_AUTO_TEST_CASE(PARTIAL_ACCESS_STRING_WRITE_ON_EMPTY_STRING) {
  /// Vendor-specific behaviour fill all symbols between requested index and former last symbol with \0
  CIEC_STRING testString;
  testString[5] = 'e'_CHAR;
  BOOST_TEST("\0\0\0\0e"_STRING == testString);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()
