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
#include "../../../src/core/datatypes/forte_any_unsigned_variant.h"

BOOST_AUTO_TEST_SUITE(CIEC_ANY_UNSIGNED_VARIANT_function_test)

    BOOST_AUTO_TEST_CASE(Type_test) {
      CIEC_ANY_UNSIGNED_VARIANT test;
      //check type information
      BOOST_CHECK_EQUAL(test.getDataTypeID(), CIEC_ANY::e_ANY);
    }

    BOOST_AUTO_TEST_CASE(Initialization_test) {
      // assign value
      CIEC_ANY_UNSIGNED_VARIANT test1 = CIEC_UDINT(17);
      BOOST_TEST(17 == static_cast<CIEC_UDINT::TValueType>(std::get<CIEC_UDINT>(test1)));
  
      // assign generic value
      CIEC_UDINT test3 = CIEC_UDINT(4);
      CIEC_ANY_UNSIGNED &test4 = test3;
      CIEC_ANY_UNSIGNED_VARIANT test2 = test4;
      BOOST_TEST(4 == static_cast<CIEC_UDINT::TValueType>(std::get<CIEC_UDINT>(test2)));
  
      // assign each other
      CIEC_ANY_UNSIGNED_VARIANT test5 = test1;
      BOOST_TEST(17 == static_cast<CIEC_UDINT::TValueType>(std::get<CIEC_UDINT>(test5)));
    }
  
    BOOST_AUTO_TEST_CASE(Assignment_test) {
      CIEC_ANY_UNSIGNED_VARIANT test1;
      CIEC_ANY_UNSIGNED_VARIANT test2;

      // initial value must be 0
      BOOST_TEST(0 == static_cast<CIEC_USINT::TValueType>(std::get<CIEC_USINT>(test1)));
      BOOST_TEST(0 == static_cast<CIEC_USINT::TValueType>(std::get<CIEC_USINT>(test2)));

      // assign value
      test1 = CIEC_UDINT(17);
      BOOST_TEST(17 == static_cast<CIEC_UDINT::TValueType>(std::get<CIEC_UDINT>(test1)));

      // assign generic value
      CIEC_UDINT test3 = CIEC_UDINT(4);
      CIEC_ANY_UNSIGNED &test4 = test3;
      test2 = test4;
      BOOST_TEST(4 == static_cast<CIEC_UDINT::TValueType>(std::get<CIEC_UDINT>(test2)));

      // assign each other
      test2.setValue(test1);
      BOOST_TEST(17 == static_cast<CIEC_UDINT::TValueType>(std::get<CIEC_UDINT>(test2)));

      // assign dynamic value
      test2.setValue(CIEC_UINT(4));
      BOOST_TEST(4 == static_cast<CIEC_UINT::TValueType>(std::get<CIEC_UINT>(test2)));
    }

    void checkStringConversion(CIEC_ANY_UNSIGNED_VARIANT &paValue, const char *paString, CIEC_ANY::EDataTypeID paDataTypeId) {
      char buf[50];
      BOOST_CHECK_EQUAL(paValue.fromString(paString), strlen(paString));
      BOOST_CHECK_EQUAL(paValue.getDataTypeID(), CIEC_ANY::e_ANY);
      BOOST_CHECK_EQUAL(paValue.unwrap().getDataTypeID(), paDataTypeId);
      BOOST_CHECK_EQUAL(paValue.toString(buf, sizeof(buf)), strlen(paString));
      BOOST_CHECK_EQUAL(std::strcmp(paString, buf), 0);
    }

    BOOST_AUTO_TEST_CASE(String_Conversion_test) {
      CIEC_ANY_UNSIGNED_VARIANT test;
      checkStringConversion(test, "USINT#255", CIEC_ANY::e_USINT);
      checkStringConversion(test, "UINT#25754", CIEC_ANY::e_UINT);
      checkStringConversion(test, "UDINT#3112323", CIEC_ANY::e_UDINT);
      checkStringConversion(test, "ULINT#123123123123123", CIEC_ANY::e_ULINT);
    }

    BOOST_AUTO_TEST_CASE(Equality_test) {
      CIEC_ANY_UNSIGNED_VARIANT test1;
      CIEC_ANY_UNSIGNED_VARIANT test2;

      BOOST_TEST(test1.equals(test2));
      test1 = CIEC_UDINT(17);
      BOOST_TEST(!test1.equals(test2));
      test2.setValue(test1);
      BOOST_TEST(test1.equals(test2));
      test2.setValue(CIEC_UINT(4));
      BOOST_TEST(!test1.equals(test2));
    }

BOOST_AUTO_TEST_SUITE_END()
