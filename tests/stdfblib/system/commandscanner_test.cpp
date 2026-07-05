/*******************************************************************************
 * Copyright (c) 2026 Johannes Kepler University Linz
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include <boost/test/unit_test.hpp>
#include "../../stdfblib/system/src/CommandParser.h"

using forte::iec61499::system::detail::CommandScanner;

BOOST_AUTO_TEST_SUITE(commandscanner)

BOOST_AUTO_TEST_CASE(consumeMatchingPrefix) {
  CommandScanner s{"hello world"};
  BOOST_TEST(s.consume("hello"));
  BOOST_TEST(!s.empty());
}

BOOST_AUTO_TEST_CASE(consumeMatchingFullInputLeavesEmpty) {
  CommandScanner s{"hello"};
  BOOST_TEST(s.consume("hello"));
  BOOST_TEST(s.empty());
}

BOOST_AUTO_TEST_CASE(consumeNonMatching) {
  CommandScanner s{"hello"};
  BOOST_TEST(!s.consume("world"));
  BOOST_TEST(!s.empty());
  BOOST_TEST(s.consume("hello"));
}

BOOST_AUTO_TEST_CASE(consumePrefixLongerThanRemaining) {
  CommandScanner s{"hel"};
  BOOST_TEST(!s.consume("hello"));
  BOOST_TEST(!s.empty());
  BOOST_TEST(s.consume("hel"));
}

/*********************** takeUntil ***********************/
BOOST_AUTO_TEST_CASE(takeUntilReturnsToken) {
  CommandScanner s{"hello|rest"};
  auto token = s.takeUntil('|');
  BOOST_TEST(token == "hello");
  BOOST_TEST(!s.empty());
}

BOOST_AUTO_TEST_CASE(takeUntilLeavesCommandScannerEmpty) {
  CommandScanner s{"hello|"};
  auto token = s.takeUntil('|');
  BOOST_TEST(token == "hello");
  BOOST_TEST(s.empty());
}

BOOST_AUTO_TEST_CASE(takeUntilDelimiterAtStartReturnsEmptyToken) {
  CommandScanner s{"|rest"};
  auto token = s.takeUntil('|');
  BOOST_TEST(token.empty());
  BOOST_TEST(!s.empty());
  BOOST_TEST(s.consume("rest"));
}

BOOST_AUTO_TEST_CASE(takeUntilMissingDelimiterReturnsEmpty) {
  CommandScanner s{"hello"};
  BOOST_TEST(s.takeUntil('|').empty());
  BOOST_TEST(s.consume("hello"));
}

BOOST_AUTO_TEST_CASE(takeUntilEmptyInputReturnsEmpty) {
  CommandScanner s{""};
  BOOST_TEST(s.takeUntil('|').empty());
}

/********************* skipWhiteSpace *********************/
BOOST_AUTO_TEST_CASE(skipWhiteSpaceRemovesLeadingSpaces) {
  CommandScanner s{"   hello"};
  s.skipWhiteSpace();
  BOOST_TEST(s.consume("hello"));
}

BOOST_AUTO_TEST_CASE(skipWhiteSpaceHandlesMixedWhitespace) {
  CommandScanner s{" \t\r\nhello"};
  s.skipWhiteSpace();
  BOOST_TEST(s.consume("hello"));
}

BOOST_AUTO_TEST_CASE(skipWhiteSpaceNoLeadingWhitespaceLeavesUnchanged) {
  CommandScanner s{"hello"};
  s.skipWhiteSpace();
  BOOST_TEST(s.consume("hello"));
}

BOOST_AUTO_TEST_CASE(skipWhiteSpaceAllWhitespaceLeavesEmpty) {
  CommandScanner s{"   "};
  s.skipWhiteSpace();
  BOOST_TEST(s.empty());
}

/********************* peek *********************/
BOOST_AUTO_TEST_CASE(peekReturnsFirstCharacter) {
  CommandScanner s{"hello"};
  BOOST_TEST(s.peek() == 'h');
}

BOOST_AUTO_TEST_CASE(peekDoesNotAdvance) {
  CommandScanner s{"hello"};
  s.peek();
  BOOST_TEST(s.consume("hello"));
}

BOOST_AUTO_TEST_CASE(peekEmptyReturnsNullTerminator) {
  CommandScanner s{""};
  BOOST_TEST(s.peek() == '\0');
}
BOOST_AUTO_TEST_SUITE_END()
