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
  CIEC_STRING stringValue("Primetals");
  CIEC_CHAR testChar;
  testChar = stringValue[5];
  BOOST_TEST(CIEC_CHAR('e') == testChar);
}

BOOST_AUTO_TEST_CASE(PARTIAL_ACCESS_STRING_WRITE) {
  CIEC_STRING testString("Primxtals");
  CIEC_CHAR charValue('e');
  testString[5] = charValue;
  BOOST_TEST(CIEC_STRING("Primetals") == testString);
}

BOOST_AUTO_TEST_CASE(PARTIAL_ACCESS_STRING_READ_FROM_CONST) {
  const CIEC_STRING stringValue("Primetals");
  CIEC_CHAR testChar;
  testChar = stringValue[5];
  BOOST_TEST(CIEC_CHAR('e') == testChar);
}

BOOST_AUTO_TEST_CASE(PARTIAL_ACCESS_STRING_READ_AT_INDEX_0) {
  // Vendor-specific behaviour for returning the first element and emitting an error
  CIEC_STRING stringValue("Primetals");
  CIEC_CHAR testChar;
  testChar = stringValue[0];
  BOOST_TEST(CIEC_CHAR('P') == testChar);
}

BOOST_AUTO_TEST_CASE(PARTIAL_ACCESS_STRING_WRITE_AT_INDEX_0) {
  // Vendor-specific behaviour for returning the first element and emitting an error
  CIEC_STRING testString("Primxtals");
  CIEC_CHAR charValue('e');
  testString[0] = charValue;
  BOOST_TEST(CIEC_STRING("erimxtals") == testString);
}

/* These tests need also to be checked in the log, as both exhibit the 
 * same result, but the latter two emit an error at the devlog console */
BOOST_AUTO_TEST_CASE(PARTIAL_ACCESS_STRING_READ_AT_INDEX_END) {
  CIEC_STRING stringValue("Primetals");
  CIEC_CHAR testChar;
  testChar = stringValue[9];
  BOOST_TEST(CIEC_CHAR('s') == testChar);
}

BOOST_AUTO_TEST_CASE(PARTIAL_ACCESS_STRING_WRITE_AT_INDEX_END) {
  CIEC_STRING testString("Primxtals");
  CIEC_CHAR charValue('e');
  testString[9] = charValue;
  BOOST_TEST(CIEC_STRING("Primxtale") == testString);
}
BOOST_AUTO_TEST_CASE(PARTIAL_ACCESS_STRING_READ_AT_INDEX_PAST_END) {
  // Vendor-specific behaviour for returning the last element and emitting an error
  CIEC_STRING stringValue("Primetals");
  CIEC_CHAR testChar;
  testChar = stringValue[10];
  BOOST_TEST(CIEC_CHAR('s') == testChar);
}

BOOST_AUTO_TEST_CASE(PARTIAL_ACCESS_STRING_WRITE_AT_INDEX_PAST_END) {
  // Vendor-specific behaviour for returning the last element and emitting an error
  CIEC_STRING testString("Primxtals");
  CIEC_CHAR charValue('e');
  testString[10] = charValue;
  BOOST_TEST(CIEC_STRING("Primxtale") == testString);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()
