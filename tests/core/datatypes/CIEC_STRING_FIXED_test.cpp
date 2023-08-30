/*******************************************************************************
 * Copyright (c) 2023 Primetals Technologies Austria GmbH
 *               
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

#include "../../../src/core/datatypes/forte_string_fixed.h"

using namespace std::string_literals;

BOOST_AUTO_TEST_SUITE(CIEC_STRING_FIXED_function_test)
BOOST_AUTO_TEST_CASE(Type_test) {
  CIEC_STRING_FIXED<5> sTest;
  //check type information
  BOOST_REQUIRE(sTest.getDataTypeID() == CIEC_ANY::e_STRING);
}

BOOST_AUTO_TEST_CASE(Initialize_with_string_literal_same_size) {
  CIEC_STRING_FIXED<5> sTest = "12345"_STRING;
  BOOST_TEST(sTest == "12345"_STRING);
}

BOOST_AUTO_TEST_CASE(Initialize_with_CHAR) {
  CIEC_STRING_FIXED<5> fixed5 = 'a'_CHAR;
  BOOST_TEST(fixed5 == "a"_STRING);
}

BOOST_AUTO_TEST_CASE(Initialize_with_string_literal_shorter_size) {
  CIEC_STRING_FIXED<5> sTest = "123"_STRING;
  BOOST_TEST(sTest == "123"_STRING);
}

BOOST_AUTO_TEST_CASE(Initialize_with_string_literal_longer_size) {
  CIEC_STRING_FIXED<5> sTest = "123456789"_STRING;
  BOOST_TEST(sTest == "12345"_STRING);
}

BOOST_AUTO_TEST_CASE(Assign_CHAR) {
  CIEC_STRING_FIXED<5> fixed5;
  fixed5 = 'a'_CHAR;
  BOOST_TEST(fixed5 == "a"_STRING);
}

BOOST_AUTO_TEST_CASE(Assignment_to_max_length_string) {
  CIEC_STRING_FIXED<5> fixed = "123456789"_STRING;
  CIEC_STRING sTest = fixed;
  BOOST_TEST(sTest == "12345"_STRING);
}

BOOST_AUTO_TEST_CASE(Assignment_from_one_fixed_to_a_different_fixed_length_string) {
  CIEC_STRING_FIXED<9> fixed9 = "123456789"_STRING;
  CIEC_STRING_FIXED<5> fixed5 = fixed9;
  BOOST_TEST(fixed5 == "12345"_STRING);
}

BOOST_AUTO_TEST_CASE(Assign_method_exact) {
  CIEC_STRING_FIXED<5> fixed5;
  fixed5.assign("12345", 5);
  BOOST_TEST(fixed5 == "12345"_STRING);
}

BOOST_AUTO_TEST_CASE(Assign_method_less) {
  CIEC_STRING_FIXED<5> fixed5;
  fixed5.assign("123", 3);
  BOOST_TEST(fixed5 == "123"_STRING);
}

BOOST_AUTO_TEST_CASE(Assign_method_more) {
  CIEC_STRING_FIXED<5> fixed5;
  fixed5.assign("123456789", 9);
  BOOST_TEST(fixed5 == "12345"_STRING);
}

BOOST_AUTO_TEST_CASE(append_from_cstring_less_than_max) {
  CIEC_STRING_FIXED<5> fixed5;
  fixed5.append("123");
  BOOST_TEST(fixed5 == "123"_STRING);
}

BOOST_AUTO_TEST_CASE(append_from_cstring_more_than_max) {
  CIEC_STRING_FIXED<5> fixed5;
  fixed5.append("123456789");
  BOOST_TEST(fixed5 == "12345"_STRING);
}

BOOST_AUTO_TEST_CASE(append_from_cstring) {
  CIEC_STRING_FIXED<5> fixed5;
  fixed5.append("12345");
  BOOST_TEST(fixed5 == "12345"_STRING);
}

BOOST_AUTO_TEST_CASE(append_from_cstring_exceeding_length) {
  CIEC_STRING_FIXED<5> fixed5("123"_STRING);
  fixed5.append("456");
  BOOST_TEST(fixed5 == "12345"_STRING);
}

BOOST_AUTO_TEST_CASE(append_from_cstring_with_length) {
  CIEC_STRING_FIXED<5> fixed5;
  fixed5.append("12345", 5);
  BOOST_TEST(fixed5 == "12345"_STRING);
}

BOOST_AUTO_TEST_CASE(append_from_cstring_with_length_exceeding_length) {
  CIEC_STRING_FIXED<5> fixed5("123"_STRING);
  fixed5.append("456", 3);
  BOOST_TEST(fixed5 == "12345"_STRING);
}

BOOST_AUTO_TEST_CASE(append_from_CIEC_STRING) {
  CIEC_STRING_FIXED<5> fixed5;
  CIEC_STRING appendString("12345"_STRING);
  fixed5.append(appendString);
  BOOST_TEST(fixed5 == "12345"_STRING);
}

BOOST_AUTO_TEST_CASE(append_from_CIEC_STRING_exceeding_max_length) {
  CIEC_STRING_FIXED<5> fixed5("123"_STRING);
  CIEC_STRING appendString("456"_STRING);
  fixed5.append(appendString);
  BOOST_TEST(fixed5 == "12345"_STRING);
}

BOOST_AUTO_TEST_CASE(append_fromString) {
  CIEC_STRING_FIXED<5> fixed5;
  std::string appendString("12345"s);
  fixed5.append(appendString);
  BOOST_TEST(fixed5 == "12345"_STRING);
}

BOOST_AUTO_TEST_CASE(append_fromString_exceeding_max_length) {
  CIEC_STRING_FIXED<5> fixed5("123"_STRING);
  std::string appendString("456"s);
  fixed5.append(appendString);
  BOOST_TEST(fixed5 == "12345"_STRING);
}

BOOST_AUTO_TEST_CASE(read_partial) {
  CIEC_STRING_FIXED<5> fixed5("12345"_STRING);
  CIEC_CHAR symbol = fixed5[3];
  BOOST_TEST(symbol =='3'_CHAR);
}

BOOST_AUTO_TEST_CASE(write_partial) {
  CIEC_STRING_FIXED<5> fixed5("12345"_STRING);
  CIEC_CHAR symbol('9'_CHAR);
  fixed5[3] = symbol;
  BOOST_TEST(fixed5 == "12945"_STRING);
}

BOOST_AUTO_TEST_CASE(assign_operator_string_to_fixed) {
  CIEC_STRING_FIXED<5> fixed5;
  fixed5 = "123456789"_STRING;
  BOOST_TEST(fixed5 == "12345"_STRING);
}

BOOST_AUTO_TEST_CASE(assign_operator_fixed_to_string) {
  CIEC_STRING nonFixed;
  CIEC_STRING_FIXED<5> fixed5("12345"_STRING);
  nonFixed = fixed5;
  BOOST_TEST(nonFixed == "12345"_STRING);
}

BOOST_AUTO_TEST_CASE(assign_operator_fixed5_to_fixed9) {
  CIEC_STRING_FIXED<9> fixed9;
  CIEC_STRING_FIXED<5> fixed5("12345"_STRING);
  fixed9 = fixed5;
  BOOST_TEST(fixed9 == "12345"_STRING);
}

BOOST_AUTO_TEST_CASE(assign_operator_fixed9_to_fixed5) {
  CIEC_STRING_FIXED<9> fixed9 = "123456789"_STRING;
  CIEC_STRING_FIXED<5> fixed5;
  fixed5 = fixed9;
  BOOST_TEST(fixed5 == "12345"_STRING);
}


BOOST_AUTO_TEST_SUITE_END()