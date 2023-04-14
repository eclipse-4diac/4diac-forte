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
#include "../../../src/core/datatypes/forte_any_string_variant.h"

BOOST_AUTO_TEST_SUITE(CIEC_ANY_STRING_VARIANT_function_test)

    BOOST_AUTO_TEST_CASE(Type_test) {
      CIEC_ANY_STRING_VARIANT test;
      //check type information
      BOOST_CHECK_EQUAL(test.getDataTypeID(), CIEC_ANY::e_ANY);
    }

    BOOST_AUTO_TEST_CASE(Assignment_test) {
      CIEC_ANY_STRING_VARIANT test1;
      CIEC_ANY_STRING_VARIANT test2;

      // initial value must be 0
      BOOST_TEST('\0' == std::get<CIEC_STRING>(test1).getValue()[0]);
      BOOST_TEST('\0' == std::get<CIEC_STRING>(test2).getValue()[0]);

      // assign value
      test1 = CIEC_STRING("abc");
      BOOST_CHECK_EQUAL(std::strcmp("abc", std::get<CIEC_STRING>(test1).getValue()), 0);

      // assign each other
      test2.setValue(test1);
      BOOST_CHECK_EQUAL(std::strcmp("abc", std::get<CIEC_STRING>(test2).getValue()), 0);

      // assign dynamic value
      test2.setValue(CIEC_WSTRING("abc"));
      BOOST_CHECK_EQUAL(std::strcmp("abc", std::get<CIEC_WSTRING>(test2).getValue()), 0);
    }

    void checkStringConversion(CIEC_ANY_STRING_VARIANT &paValue, const char *paString, CIEC_ANY::EDataTypeID paDataTypeId) {
      char buf[50];
      BOOST_CHECK_EQUAL(paValue.fromString(paString), strlen(paString));
      BOOST_CHECK_EQUAL(paValue.getDataTypeID(), CIEC_ANY::e_ANY);
      BOOST_CHECK_EQUAL(paValue.unwrap().getDataTypeID(), paDataTypeId);
      BOOST_CHECK_EQUAL(paValue.toString(buf, sizeof(buf)), strlen(paString));
      BOOST_CHECK_EQUAL(std::strcmp(paString, buf), 0);
    }

    BOOST_AUTO_TEST_CASE(String_Conversion_test) {
      CIEC_ANY_STRING_VARIANT test;
      checkStringConversion(test, "STRING#'Hallo FORTE user!'", CIEC_ANY::e_STRING);
      checkStringConversion(test, "WSTRING#\"Hallo FORTE user!\"", CIEC_ANY::e_WSTRING);
    }

    BOOST_AUTO_TEST_CASE(Equality_test) {
      CIEC_ANY_STRING_VARIANT test1;
      CIEC_ANY_STRING_VARIANT test2;

      BOOST_TEST(test1.equals(test2));
      test1 = CIEC_STRING("abc");
      BOOST_TEST(!test1.equals(test2));
      test2.setValue(test1);
      BOOST_TEST(test1.equals(test2));
      test2.setValue(CIEC_WSTRING("abc"));
      BOOST_TEST(!test1.equals(test2));
    }

BOOST_AUTO_TEST_SUITE_END()
