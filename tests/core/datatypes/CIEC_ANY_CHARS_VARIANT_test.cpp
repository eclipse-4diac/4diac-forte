/*******************************************************************************
 * Copyright (c) 2023 Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Martin Erich Jobst - initial implementation
 *******************************************************************************/
#include <boost/test/unit_test.hpp>
#include "forte_boost_output_support.h"
#include "../../../src/core/datatypes/forte_any_chars_variant.h"

BOOST_AUTO_TEST_SUITE(CIEC_ANY_CHARS_VARIANT_function_test)

    BOOST_AUTO_TEST_CASE(Type_test) {
      CIEC_ANY_CHARS_VARIANT test;
      //check type information
      BOOST_CHECK_EQUAL(test.getDataTypeID(), CIEC_ANY::e_ANY);
    }

    BOOST_AUTO_TEST_CASE(Initialization_test) {
      // assign value
      CIEC_ANY_CHARS_VARIANT test1 = 'a'_CHAR;
      BOOST_TEST('a' == static_cast<CIEC_CHAR::TValueType>(std::get<CIEC_CHAR>(test1)));
  
      // assign generic value
      CIEC_CHAR test3 = 'b'_CHAR;
      CIEC_ANY_CHAR &test4 = test3;
      CIEC_ANY_CHARS_VARIANT test2 = test4;
      BOOST_TEST('b' == static_cast<CIEC_CHAR::TValueType>(std::get<CIEC_CHAR>(test2)));
  
      // assign each other
      CIEC_ANY_CHARS_VARIANT test5 = test1;
      BOOST_TEST('a' == static_cast<CIEC_CHAR::TValueType>(std::get<CIEC_CHAR>(test5)));
    }
  
    BOOST_AUTO_TEST_CASE(Assignment_test) {
      CIEC_ANY_CHARS_VARIANT test1;
      CIEC_ANY_CHARS_VARIANT test2;

      // initial value must be 0
      BOOST_TEST(0 == static_cast<CIEC_CHAR::TValueType>(std::get<CIEC_CHAR>(test1)));
      BOOST_TEST(0 == static_cast<CIEC_CHAR::TValueType>(std::get<CIEC_CHAR>(test2)));

      // assign value
      test1 = CIEC_CHAR('a');
      BOOST_TEST('a' == static_cast<CIEC_CHAR::TValueType>(std::get<CIEC_CHAR>(test1)));

      // assign generic value
      CIEC_CHAR test3 = 'b'_CHAR;
      CIEC_ANY_CHAR &test4 = test3;
      test2 = test4;
      BOOST_TEST('b' == static_cast<CIEC_CHAR::TValueType>(std::get<CIEC_CHAR>(test2)));

      // assign each other
      test2.setValue(test1);
      BOOST_TEST('a' == static_cast<CIEC_CHAR::TValueType>(std::get<CIEC_CHAR>(test2)));

      // assign dynamic value
      test2.setValue(CIEC_WCHAR('a'));
      BOOST_TEST('a' == static_cast<CIEC_WCHAR::TValueType>(std::get<CIEC_WCHAR>(test2)));
    }

    void checkStringConversion(CIEC_ANY_CHARS_VARIANT &paValue, const char *paString, CIEC_ANY::EDataTypeID paDataTypeId) {
      char buf[50];
      BOOST_CHECK_EQUAL(paValue.fromString(paString), strlen(paString));
      BOOST_CHECK_EQUAL(paValue.getDataTypeID(), CIEC_ANY::e_ANY);
      BOOST_CHECK_EQUAL(paValue.unwrap().getDataTypeID(), paDataTypeId);
      BOOST_CHECK_EQUAL(paValue.toString(buf, sizeof(buf)), strlen(paString));
      BOOST_CHECK_EQUAL(std::strcmp(paString, buf), 0);
    }

    BOOST_AUTO_TEST_CASE(String_Conversion_test) {
      CIEC_ANY_CHARS_VARIANT test;
      checkStringConversion(test, "CHAR#'a'", CIEC_ANY::e_CHAR);
      checkStringConversion(test, "WCHAR#\"a\"", CIEC_ANY::e_WCHAR);
      checkStringConversion(test, "STRING#'Hallo FORTE user!'", CIEC_ANY::e_STRING);
      checkStringConversion(test, "WSTRING#\"Hallo FORTE user!\"", CIEC_ANY::e_WSTRING);
    }

    BOOST_AUTO_TEST_CASE(Equality_test) {
      CIEC_ANY_CHARS_VARIANT test1;
      CIEC_ANY_CHARS_VARIANT test2;

      BOOST_TEST(test1.equals(test2));
      test1 = "abc"_STRING;
      BOOST_TEST(!test1.equals(test2));
      test2.setValue(test1);
      BOOST_TEST(test1.equals(test2));
      test2.setValue(CIEC_WSTRING("abc"));
      BOOST_TEST(!test1.equals(test2));
    }

BOOST_AUTO_TEST_SUITE_END()
