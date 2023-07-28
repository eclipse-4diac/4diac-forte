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
#include "../../../src/core/datatypes/forte_any_variant.h"

BOOST_AUTO_TEST_SUITE(CIEC_ANY_VARIANT_function_test)

    BOOST_AUTO_TEST_CASE(Type_test) {
      CIEC_ANY_VARIANT test;
      //check type information
      BOOST_CHECK_EQUAL(test.getDataTypeID(), CIEC_ANY::e_ANY);
    }

    BOOST_AUTO_TEST_CASE(Initialization_test) {
      // assign value
      CIEC_ANY_VARIANT test1 = CIEC_DINT(17);
      BOOST_TEST(17 == static_cast<CIEC_DINT::TValueType>(std::get<CIEC_DINT>(test1)));

      // assign generic value
      CIEC_DINT test3 = CIEC_DINT(4);
      CIEC_ANY &test4 = test3;
      CIEC_ANY_VARIANT test2 = test4;
      BOOST_TEST(4 == static_cast<CIEC_DINT::TValueType>(std::get<CIEC_DINT>(test2)));

      // assign each other
      CIEC_ANY_VARIANT test5 = test1;
      BOOST_TEST(17 == static_cast<CIEC_DINT::TValueType>(std::get<CIEC_DINT>(test5)));
    }

    BOOST_AUTO_TEST_CASE(Assignment_test) {
      CIEC_ANY_VARIANT test1;
      CIEC_ANY_VARIANT test2;

      // initial value must be 0
      BOOST_TEST(0 == static_cast<CIEC_SINT::TValueType>(std::get<CIEC_SINT>(test1)));
      BOOST_TEST(0 == static_cast<CIEC_SINT::TValueType>(std::get<CIEC_SINT>(test2)));

      // assign value
      test1 = CIEC_DINT(17);
      BOOST_TEST(17 == static_cast<CIEC_DINT::TValueType>(std::get<CIEC_DINT>(test1)));

      // assign generic value
      CIEC_DINT test3 = CIEC_DINT(4);
      CIEC_ANY &test4 = test3;
      test2 = test4;
      BOOST_TEST(4 == static_cast<CIEC_DINT::TValueType>(std::get<CIEC_DINT>(test2)));

      // assign each other
      test2.setValue(test1);
      BOOST_TEST(17 == static_cast<CIEC_DINT::TValueType>(std::get<CIEC_DINT>(test2)));

      // assign dynamic value
      test2.setValue(CIEC_INT(4));
      BOOST_TEST(4 == static_cast<CIEC_INT::TValueType>(std::get<CIEC_INT>(test2)));
    }

    void checkStringConversion(CIEC_ANY_VARIANT &paValue, const char *paString, CIEC_ANY::EDataTypeID paDataTypeId) {
      char buf[50];
      BOOST_CHECK_EQUAL(paValue.fromString(paString), strlen(paString));
      BOOST_CHECK_EQUAL(paValue.getDataTypeID(), CIEC_ANY::e_ANY);
      BOOST_CHECK_EQUAL(paValue.unwrap().getDataTypeID(), paDataTypeId);
      BOOST_CHECK_EQUAL(paValue.toString(buf, sizeof(buf)), strlen(paString));
      BOOST_TEST(paString == buf);
    }

    BOOST_AUTO_TEST_CASE(String_Conversion_test) {
      CIEC_ANY_VARIANT test;
      checkStringConversion(test, "BOOL#TRUE", CIEC_ANY::e_BOOL);
      checkStringConversion(test, "SINT#123", CIEC_ANY::e_SINT);
      checkStringConversion(test, "INT#5754", CIEC_ANY::e_INT);
      checkStringConversion(test, "DINT#112323", CIEC_ANY::e_DINT);
      checkStringConversion(test, "USINT#255", CIEC_ANY::e_USINT);
      checkStringConversion(test, "UINT#25754", CIEC_ANY::e_UINT);
      checkStringConversion(test, "UDINT#3112323", CIEC_ANY::e_UDINT);
      checkStringConversion(test, "BYTE#253", CIEC_ANY::e_BYTE);
      checkStringConversion(test, "WORD#25754", CIEC_ANY::e_WORD);
      checkStringConversion(test, "DWORD#3112323", CIEC_ANY::e_DWORD);
      checkStringConversion(test, "D#2008-04-03", CIEC_ANY::e_DATE);
      checkStringConversion(test, "TOD#15:36:55.360", CIEC_ANY::e_TIME_OF_DAY);
      checkStringConversion(test, "DT#1994-06-22-14:23:54.800", CIEC_ANY::e_DATE_AND_TIME);
      checkStringConversion(test, "T#311ms", CIEC_ANY::e_TIME);
      checkStringConversion(test, "LD#2008-04-03", CIEC_ANY::e_LDATE);
      checkStringConversion(test, "LTOD#15:36:55.360", CIEC_ANY::e_LTIME_OF_DAY);
      checkStringConversion(test, "LDT#1994-06-22-14:23:54.800", CIEC_ANY::e_LDATE_AND_TIME);
      checkStringConversion(test, "LT#311ms", CIEC_ANY::e_LTIME);
      checkStringConversion(test, "REAL#3.125", CIEC_ANY::e_REAL);
      checkStringConversion(test, "LREAL#3.1251234000000001", CIEC_ANY::e_LREAL);
      checkStringConversion(test, "CHAR#'a'", CIEC_ANY::e_CHAR);
      checkStringConversion(test, "WCHAR#\"a\"", CIEC_ANY::e_WCHAR);
      checkStringConversion(test, "STRING#'Hallo FORTE user!'", CIEC_ANY::e_STRING);
      checkStringConversion(test, "WSTRING#\"Hallo FORTE user!\"", CIEC_ANY::e_WSTRING);
      checkStringConversion(test, "LINT#123123123123", CIEC_ANY::e_LINT);
      checkStringConversion(test, "ULINT#123123123123123", CIEC_ANY::e_ULINT);
      checkStringConversion(test, "LWORD#123123123123123", CIEC_ANY::e_LWORD);
    }

    BOOST_AUTO_TEST_CASE(Equality_test) {
      CIEC_ANY_VARIANT test1;
      CIEC_ANY_VARIANT test2;

      BOOST_TEST(test1.equals(test2));
      test1 = CIEC_DINT(17);
      BOOST_TEST(!test1.equals(test2));
      test2.setValue(test1);
      BOOST_TEST(test1.equals(test2));
      test2.setValue(CIEC_INT(4));
      BOOST_TEST(!test1.equals(test2));
    }

BOOST_AUTO_TEST_SUITE_END()
