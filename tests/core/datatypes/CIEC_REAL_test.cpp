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
#include <boost/test/tools/floating_point_comparison.hpp>

#include "forte_boost_output_support.h"

#include "forte/datatypes/forte_real.h"

#include <cmath>
#include <limits>

namespace forte::test {
  BOOST_AUTO_TEST_SUITE(CIEC_REAL_function_test)
  BOOST_AUTO_TEST_CASE(Type_test) {
    CIEC_REAL nTest;
    // check type information
    BOOST_REQUIRE_EQUAL(nTest.getDataTypeID(), CIEC_ANY::e_REAL);
    // check operator bool data type size
    BOOST_REQUIRE_EQUAL(sizeof(nTest.operator TForteFloat()), sizeof(TForteFloat));
  }

  BOOST_AUTO_TEST_CASE(Literal_test) {
    CIEC_REAL test1 = 0.0_REAL;
    BOOST_TEST(static_cast<CIEC_REAL::TValueType>(test1) == 0.0f);

    CIEC_REAL test2 = 3.1415_REAL;
    BOOST_TEST(static_cast<CIEC_REAL::TValueType>(test2) == 3.1415f);

    CIEC_REAL test3 = -3.1415_REAL;
    BOOST_TEST(static_cast<CIEC_REAL::TValueType>(test3) == -3.1415f);

    CIEC_REAL test4 = 3.1415e4_REAL;
    BOOST_TEST(static_cast<CIEC_REAL::TValueType>(test4) == 3.1415e4f);

    CIEC_REAL test5 = -3.1415e4_REAL;
    BOOST_TEST(static_cast<CIEC_REAL::TValueType>(test5) == -3.1415e4f);

    CIEC_REAL test6 = 31415_REAL;
    BOOST_TEST(static_cast<CIEC_REAL::TValueType>(test6) == 3.1415e4f);

    CIEC_REAL test7 = -31415_REAL;
    BOOST_TEST(static_cast<CIEC_REAL::TValueType>(test7) == -3.1415e4f);
  }

  BOOST_AUTO_TEST_CASE(Assignment_test) {
    CIEC_REAL nTest1;
    CIEC_REAL nTest2;

    // initial value must be 0
    BOOST_CHECK_EQUAL(nTest1, 0.0f);

    nTest1 = CIEC_REAL(std::numeric_limits<TForteFloat>::min());
    nTest2 = nTest1;
    BOOST_CHECK_EQUAL(nTest1, std::numeric_limits<TForteFloat>::min());
    BOOST_CHECK_EQUAL(nTest2, std::numeric_limits<TForteFloat>::min());

    nTest1 = CIEC_REAL(2.28743e6);
    nTest2 = nTest1;
    BOOST_CHECK_EQUAL(nTest1, 2.28743e6);
    BOOST_CHECK_EQUAL(nTest2, 2.28743e6);

    nTest1 = CIEC_REAL(-6.2587e-4f);
    nTest2 = nTest1;
    BOOST_CHECK_EQUAL(nTest1, -6.2587e-4f);
    BOOST_CHECK_EQUAL(nTest2, -6.2587e-4f);

    nTest1 = CIEC_REAL(1.0E-37f);
    nTest2 = nTest1;
    BOOST_CHECK_EQUAL(nTest1, 1.0E-37f);
    BOOST_CHECK_EQUAL(nTest2, 1.0E-37f);

    nTest1 = CIEC_REAL(std::numeric_limits<TForteFloat>::max());
    nTest2 = nTest1;
    BOOST_CHECK_EQUAL(nTest1, std::numeric_limits<TForteFloat>::max());
    BOOST_CHECK_EQUAL(nTest2, std::numeric_limits<TForteFloat>::max());

    // check that assignment operator does not intertwine objects
    nTest2 = CIEC_REAL(-36.0);
    BOOST_CHECK_EQUAL(nTest1, std::numeric_limits<TForteFloat>::max());
    BOOST_CHECK_EQUAL(nTest2, -36.0);
  }

  BOOST_AUTO_TEST_CASE(Equality_test) {
    CIEC_REAL nTest1;
    CIEC_REAL nTest2;
    CIEC_LREAL nTest3;

    BOOST_CHECK(nTest1.equals(nTest2));
    BOOST_CHECK(!nTest1.equals(nTest3)); // different type

    nTest1 = CIEC_REAL(std::numeric_limits<TForteFloat>::min());
    BOOST_CHECK(!nTest1.equals(nTest2));
    nTest2 = nTest1;
    BOOST_CHECK(nTest1.equals(nTest2));

    nTest1 = CIEC_REAL(2.28743e6);
    BOOST_CHECK(!nTest1.equals(nTest2));
    nTest2 = nTest1;
    BOOST_CHECK(nTest1.equals(nTest2));
  }

  BOOST_AUTO_TEST_CASE(Conversion_test) {
    CIEC_REAL testVal;

    std::string buffer;

    // check cast operator
    testVal = CIEC_REAL(0.0f);
    BOOST_CHECK_EQUAL(testVal.operator TForteFloat(), 0.0f);

    testVal = CIEC_REAL(std::numeric_limits<TForteFloat>::min());
    BOOST_CHECK_EQUAL(testVal.operator TForteFloat(), std::numeric_limits<TForteFloat>::min());

    testVal = CIEC_REAL(-4.2345e4f);
    BOOST_CHECK_EQUAL(testVal.operator TForteFloat(), -4.2345e4f);

    testVal = CIEC_REAL(23.7813e2f);
    BOOST_CHECK_EQUAL(testVal.operator TForteFloat(), 23.7813e2f);

    testVal = CIEC_REAL(std::numeric_limits<TForteFloat>::max());
    BOOST_CHECK_EQUAL(testVal.operator TForteFloat(), std::numeric_limits<TForteFloat>::max());

    // check toString and fromString
    BOOST_CHECK_EQUAL(testVal.fromString("-1E-37"), 6);
    BOOST_CHECK_EQUAL(static_cast<TForteFloat>(testVal), -1.0E-37f);

    testVal.toString(buffer);
    BOOST_TEST(buffer == "-9.99999991e-38");

    buffer.clear();
    testVal = CIEC_REAL(0);

    BOOST_CHECK_EQUAL(testVal.fromString("0"), 1);
    BOOST_CHECK_EQUAL(static_cast<TForteFloat>(testVal), 0.0f);
    testVal.toString(buffer);
    BOOST_TEST(buffer == "0.0");
    buffer.clear();
    testVal = CIEC_REAL(0);

    BOOST_CHECK_EQUAL(testVal.fromString("3.2523E15"), 9);
    BOOST_CHECK_EQUAL(static_cast<TForteFloat>(testVal), 3.2523e15f);

    testVal.toString(buffer);
    BOOST_TEST(buffer == "3.2523001e+15");

    buffer.clear();
    testVal = CIEC_REAL(0);

    BOOST_CHECK_EQUAL(testVal.fromString("1E37"), 4);
    BOOST_CHECK_EQUAL(static_cast<TForteFloat>(testVal), 1e37f);

    testVal.toString(buffer);
    BOOST_TEST(buffer == "9.99999993e+36");

    buffer.clear();
    testVal = CIEC_REAL(0);

    // testing values outside of allowed range
    BOOST_CHECK_EQUAL(testVal.fromString("4e40"), -1);
    BOOST_CHECK_EQUAL(testVal.fromString("2#100101100"), 1);
    BOOST_CHECK_EQUAL(testVal.fromString("8#116100"), 1);
    BOOST_CHECK_EQUAL(testVal.fromString("10#300"), 2);
    BOOST_CHECK_EQUAL(testVal.fromString("16#FFFF0"), 2);
    BOOST_CHECK_EQUAL(testVal.fromString("-4e40"), -1);

    // check invalid fromString string
    BOOST_CHECK_EQUAL(testVal.fromString("NOT A VALID STRING"), -1);
  }

  BOOST_AUTO_TEST_CASE(ToString_Tests) {
    CIEC_REAL testVal(3.0f);
    std::string buffer;
    testVal.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "3.0");

    buffer.clear();
    testVal = CIEC_REAL(10.0f);
    testVal.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "10.0");

    buffer.clear();
    testVal = CIEC_REAL(2.0e10f);
    testVal.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "2.0e+10");

    buffer.clear();
    testVal = CIEC_REAL(INFINITY);
    testVal.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "inf");

    buffer.clear();
    testVal = CIEC_REAL(-INFINITY);
    testVal.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "-inf");

    buffer.clear();
    testVal = CIEC_REAL(NAN);
    testVal.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "nan");

    buffer.clear();
    testVal = CIEC_REAL(-NAN);
    testVal.toString(buffer);
    BOOST_CHECK_EQUAL(buffer, "-nan");
  }

  void realTypedFromString(const char *paTestString, float paResult) {
    CIEC_REAL nTest;
    BOOST_TEST(nTest.fromString(paTestString) == strlen(paTestString));
    if (std::isnormal(paResult) || std::isinf(paResult)) {
      BOOST_TEST(paResult == static_cast<CIEC_REAL::TValueType>(nTest));
    } else if (std::isnan(paResult)) {
      BOOST_TEST(std::isnan(static_cast<CIEC_REAL::TValueType>(nTest)));
    }
  }

  void faultingRealTypedFromString(const char *paTestString) {
    CIEC_REAL nTest;
    BOOST_CHECK_EQUAL(nTest.fromString(paTestString), -1);
  }

  BOOST_AUTO_TEST_CASE(REAL_typed_fromString_tests) {
    realTypedFromString("REAL#1E37", 1e37f);
    realTypedFromString("REAL#-4.2345e4", -4.2345e4f);
    realTypedFromString("REAL#0", 0.0f);
    realTypedFromString("REAL#0.0", 0.0f);

    realTypedFromString("REAL#inf", INFINITY);
    realTypedFromString("REAL#-inf", -INFINITY);
    realTypedFromString("REAL#nan", NAN);
    realTypedFromString("REAL#-nan", -NAN);
    realTypedFromString("REAL#Infinity", INFINITY);
    realTypedFromString("REAL#-Infinity", -INFINITY);
    realTypedFromString("REAL#NaN", NAN);
    realTypedFromString("REAL#-NaN", -NAN);

    faultingRealTypedFromString("REAL#4e40");
    faultingRealTypedFromString("REAL#NOT A VALID STRING");
  }

  BOOST_AUTO_TEST_SUITE_END()
} // namespace forte::test
