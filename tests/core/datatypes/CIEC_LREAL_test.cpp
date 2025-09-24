/*******************************************************************************
 * Copyright (c) 2011, 2025 ACIN, nxtControl, fortiss GmbH, Martin Erich Jobst,
 *                          Primetals Technologies Austria GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Martin Melik Merkumians, Ingo Hegny, Alois Zoitl, Stanislav Meduna
 *                - initial API and implementation and/or initial documentation
 *   Martin Jobst - add equals tests
 *                - add user-defined literal tests
 *   Alois Zoitl  - migrated data type toString to std::string
 *******************************************************************************/
#include <boost/test/unit_test.hpp>
#include "forte_boost_output_support.h"
#include <boost/test/tools/floating_point_comparison.hpp>

#include "forte/datatypes/forte_lreal.h"

#include <cmath>

namespace forte::test {
  BOOST_AUTO_TEST_SUITE(CIEC_LREAL_function_test)
  BOOST_AUTO_TEST_CASE(Type_test) {
    CIEC_LREAL nTest;
    // check type information
    BOOST_REQUIRE_EQUAL(nTest.getDataTypeID(), CIEC_ANY::e_LREAL);
    // check operator bool data type size
    BOOST_REQUIRE_EQUAL(sizeof(nTest.operator TForteDFloat()), sizeof(TForteDFloat));
  }

  BOOST_AUTO_TEST_CASE(Literal_test) {
    CIEC_LREAL test1 = 0.0_LREAL;
    BOOST_TEST(static_cast<CIEC_LREAL::TValueType>(test1) == 0.0);

    CIEC_LREAL test2 = 3.1415_LREAL;
    BOOST_TEST(static_cast<CIEC_LREAL::TValueType>(test2) == 3.1415);

    CIEC_LREAL test3 = -3.1415_LREAL;
    BOOST_TEST(static_cast<CIEC_LREAL::TValueType>(test3) == -3.1415);

    CIEC_LREAL test4 = 3.1415e4_LREAL;
    BOOST_TEST(static_cast<CIEC_LREAL::TValueType>(test4) == 3.1415e4);

    CIEC_LREAL test5 = -3.1415e4_LREAL;
    BOOST_TEST(static_cast<CIEC_LREAL::TValueType>(test5) == -3.1415e4);

    CIEC_LREAL test6 = 31415_LREAL;
    BOOST_TEST(static_cast<CIEC_LREAL::TValueType>(test6) == 3.1415e4f);

    CIEC_LREAL test7 = -31415_LREAL;
    BOOST_TEST(static_cast<CIEC_LREAL::TValueType>(test7) == -3.1415e4f);
  }

  BOOST_AUTO_TEST_CASE(Assignment_test) {
    CIEC_LREAL nTest1;
    CIEC_LREAL nTest2;

    // initial value must be 0
    BOOST_TEST(static_cast<CIEC_LREAL::TValueType>(nTest1) == 0.0);

    nTest1 = CIEC_LREAL(std::numeric_limits<TForteDFloat>::min());
    nTest2 = nTest1;
    BOOST_TEST(static_cast<CIEC_LREAL::TValueType>(nTest1) == std::numeric_limits<TForteDFloat>::min());
    BOOST_TEST(static_cast<CIEC_LREAL::TValueType>(nTest2) == std::numeric_limits<TForteDFloat>::min());

    nTest1 = CIEC_LREAL(2.28743e6);
    nTest2 = nTest1;
    BOOST_TEST(static_cast<CIEC_LREAL::TValueType>(nTest1) == 2.28743e6);
    BOOST_TEST(static_cast<CIEC_LREAL::TValueType>(nTest2) == 2.28743e6);

    nTest1 = CIEC_LREAL(-6.2587e-4);
    nTest2 = nTest1;
    BOOST_TEST(static_cast<CIEC_LREAL::TValueType>(nTest1) == -6.2587e-4);
    BOOST_TEST(static_cast<CIEC_LREAL::TValueType>(nTest2) == -6.2587e-4);

    nTest1 = CIEC_LREAL(1.0E-37);
    nTest2 = nTest1;
    BOOST_TEST(static_cast<CIEC_LREAL::TValueType>(nTest1) == 1.0E-37);
    BOOST_TEST(static_cast<CIEC_LREAL::TValueType>(nTest2) == 1.0E-37);

    nTest1 = CIEC_LREAL(std::numeric_limits<TForteDFloat>::max());
    nTest2 = nTest1;
    BOOST_TEST(static_cast<CIEC_LREAL::TValueType>(nTest1) == std::numeric_limits<TForteDFloat>::max());
    BOOST_TEST(static_cast<CIEC_LREAL::TValueType>(nTest2) == std::numeric_limits<TForteDFloat>::max());

    // check that assignment operator does not intertwine objects
    nTest2 = CIEC_LREAL(-36.0);
    BOOST_TEST(static_cast<CIEC_LREAL::TValueType>(nTest1) == std::numeric_limits<TForteDFloat>::max());
    BOOST_TEST(static_cast<CIEC_LREAL::TValueType>(nTest2) == -36.0);
  }

  BOOST_AUTO_TEST_CASE(Equality_test) {
    CIEC_LREAL nTest1;
    CIEC_LREAL nTest2;
    CIEC_REAL nTest3;

    BOOST_CHECK(nTest1.equals(nTest2));
    BOOST_CHECK(!nTest1.equals(nTest3)); // different type

    nTest1 = CIEC_LREAL(std::numeric_limits<TForteFloat>::min());
    BOOST_CHECK(!nTest1.equals(nTest2));
    nTest2 = nTest1;
    BOOST_CHECK(nTest1.equals(nTest2));

    nTest1 = CIEC_LREAL(2.28743e6);
    BOOST_CHECK(!nTest1.equals(nTest2));
    nTest2 = nTest1;
    BOOST_CHECK(nTest1.equals(nTest2));
  }

  BOOST_AUTO_TEST_CASE(Conversion_test) {
    CIEC_LREAL testVal;

    std::string buffer;

    // check cast operator
    testVal = CIEC_LREAL(0.0);
    BOOST_CHECK_EQUAL(testVal.operator TForteDFloat(), 0.0);

    testVal = CIEC_LREAL(std::numeric_limits<TForteDFloat>::min());
    BOOST_CHECK_EQUAL(testVal.operator TForteDFloat(), std::numeric_limits<TForteDFloat>::min());

    testVal = CIEC_LREAL(-4.2345e4);
    BOOST_CHECK_EQUAL(testVal.operator TForteDFloat(), -4.2345e4);

    testVal = CIEC_LREAL(23.7813e2);
    BOOST_CHECK_EQUAL(testVal.operator TForteDFloat(), 23.7813e2);

    testVal = CIEC_LREAL(std::numeric_limits<TForteDFloat>::max());
    BOOST_CHECK_EQUAL(testVal.operator TForteDFloat(), std::numeric_limits<TForteDFloat>::max());

    // check toString and fromString
    buffer.clear();

    BOOST_CHECK_EQUAL(testVal.fromString("-1E-37"), 6);
    BOOST_CHECK_EQUAL(static_cast<TForteDFloat>(testVal), -1.0E-37);

    testVal.toString(buffer);
    BOOST_TEST(buffer == "-1.0000000000000001e-37");

    buffer.clear();
    testVal = CIEC_LREAL(0);

    BOOST_CHECK_EQUAL(testVal.fromString("0"), 1);
    BOOST_CHECK_EQUAL(static_cast<TForteDFloat>(testVal), 0.0);
    testVal.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "0.0");
    buffer.clear();
    testVal = CIEC_LREAL(0);

    BOOST_CHECK_EQUAL(testVal.fromString("3.2523E15"), 9);
    BOOST_CHECK_EQUAL(static_cast<TForteDFloat>(testVal), 3.2523e15);

    testVal.toString(buffer);
    BOOST_TEST(buffer == "3252300000000000.0");

    buffer.clear();
    testVal = CIEC_LREAL(0);

    BOOST_CHECK_EQUAL(testVal.fromString("1E37"), 4);
    BOOST_CHECK_EQUAL(static_cast<TForteDFloat>(testVal), 1e37);

    testVal.toString(buffer);
    BOOST_TEST(buffer == "9.9999999999999995e+36");

    buffer.clear();
    testVal = CIEC_LREAL(0);

    // testing values outside of allowed range
    BOOST_CHECK_EQUAL(testVal.fromString("4e401"), -1);
    BOOST_CHECK_EQUAL(testVal.fromString("2#100101100"), 1);
    BOOST_CHECK_EQUAL(testVal.fromString("8#116100"), 1);
    BOOST_CHECK_EQUAL(testVal.fromString("10#300"), 2);
    BOOST_CHECK_EQUAL(testVal.fromString("16#FFFF0"), 2);
    BOOST_CHECK_EQUAL(testVal.fromString("-4e401"), -1);

    // check invalid fromString string
    BOOST_CHECK_EQUAL(testVal.fromString("NOT A VALID STRING"), -1);
  }

  BOOST_AUTO_TEST_CASE(ToString_Tests) {
    CIEC_LREAL testVal(3.0);
    std::string buffer;

    testVal.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "3.0");

    buffer.clear();
    testVal = CIEC_LREAL(10.0);
    testVal.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "10.0");

    buffer.clear();
    testVal = CIEC_LREAL(2.0e20);
    testVal.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "2.0e+20");

    buffer.clear();
    testVal = CIEC_LREAL(INFINITY);
    testVal.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "inf");

    buffer.clear();
    testVal = CIEC_LREAL(-INFINITY);
    testVal.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "-inf");

    buffer.clear();
    testVal = CIEC_LREAL(NAN);
    testVal.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "nan");

    buffer.clear();
    testVal = CIEC_LREAL(-NAN);
    testVal.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "-nan");
  }

  void lRealTypedFromString(const char *paTestString, double paResult) {
    CIEC_LREAL nTest;
    BOOST_TEST(nTest.fromString(paTestString) == strlen(paTestString));
    if (std::isnormal(paResult) || std::isinf(paResult)) {
      BOOST_TEST(paResult == static_cast<CIEC_LREAL::TValueType>(nTest));
    } else if (std::isnan(paResult)) {
      BOOST_TEST(std::isnan(static_cast<CIEC_LREAL::TValueType>(nTest)));
    }
  }

  void faultingLRealTypedFromString(const char *paTestString) {
    CIEC_LREAL nTest;
    BOOST_CHECK_EQUAL(nTest.fromString(paTestString), -1);
  }

  BOOST_AUTO_TEST_CASE(REAL_typed_fromString_tests) {
    lRealTypedFromString("LREAL#1E37", 1e37);
    lRealTypedFromString("LREAL#-4.2345e4", -4.2345e4);
    lRealTypedFromString("LREAL#0", 0.0);
    lRealTypedFromString("LREAL#3.2523E15", 3.2523E15);

    lRealTypedFromString("LREAL#inf", INFINITY);
    lRealTypedFromString("LREAL#-inf", -INFINITY);
    lRealTypedFromString("LREAL#nan", NAN);
    lRealTypedFromString("LREAL#-nan", NAN);
    lRealTypedFromString("LREAL#Infinity", INFINITY);
    lRealTypedFromString("LREAL#-Infinity", -INFINITY);
    lRealTypedFromString("LREAL#NaN", NAN);
    lRealTypedFromString("LREAL#-NaN", NAN);

    faultingLRealTypedFromString("LREAL#4e401");
    faultingLRealTypedFromString("LREAL#-4e401");
    faultingLRealTypedFromString("LREAL#NOT A VALID STRING");
  }

  BOOST_AUTO_TEST_SUITE_END()
} // namespace forte::test
