/*******************************************************************************
 * Copyright (c) 2014 Profactor GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Matthias Plasch - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include <boost/test/unit_test.hpp>
#include "forte_boost_output_support.h"

#include "convert_functions.h"
#include "../../../src/core/datatypes/forte_wstring.h"

//BOOLEAN
#include "../../../src/core/datatypes/forte_bool.h"
//BIT-Datatypes
#include "../../../src/core/datatypes/forte_byte.h"
#include "../../../src/core/datatypes/forte_word.h"
#include "../../../src/core/datatypes/forte_dword.h"
//INT-Datatypes
#include "../../../src/core/datatypes/forte_sint.h"
#include "../../../src/core/datatypes/forte_usint.h"
#include "../../../src/core/datatypes/forte_int.h"
#include "../../../src/core/datatypes/forte_uint.h"
#include "../../../src/core/datatypes/forte_dint.h"
#include "../../../src/core/datatypes/forte_udint.h"
//Time
#include "../../../src/core/datatypes/forte_time.h"

#include "../../../src/core/datatypes/forte_lword.h"
#include "../../../src/core/datatypes/forte_lint.h"
#include "../../../src/core/datatypes/forte_ulint.h"

#include <boost/test/tools/floating_point_comparison.hpp>
#include "../../../src/core/datatypes/forte_real.h"
#include "../../../src/core/datatypes/forte_lreal.h"

BOOST_AUTO_TEST_SUITE(Datatypes_to_WSTRING_conversion_test)

BOOST_AUTO_TEST_CASE(BOOLS_AND_BITS_to_WSTRING_test)
{
  CIEC_WSTRING sTest;
  CIEC_WSTRING sResult;
  //check BOOL
  CIEC_BOOL bTest(true);
  sResult = CIEC_WSTRING("TRUE");
  sTest = func_BOOL_AS_WSTRING(bTest);
  //check result value
  BOOST_CHECK(sTest == sResult);
  //check length value
  BOOST_CHECK_EQUAL(sTest.length(), 4);

  //check WORD
  CIEC_WORD nWTest(0xffff);
  sResult = CIEC_WSTRING("65535");
  sTest = func_WORD_AS_WSTRING(nWTest);
  //check result value
  BOOST_CHECK(sTest == sResult);
  //check length value
  BOOST_CHECK_EQUAL(sTest.length(), 5);

  //check DWORD 4294967295
  CIEC_DWORD nDWTest(0xffffffff);
  sResult = CIEC_WSTRING("4294967295");
  sTest = func_DWORD_AS_WSTRING(nDWTest);
  //check result value
  BOOST_CHECK(sTest == sResult);
  //check length value
  BOOST_CHECK_EQUAL(sTest.length(), 10);

  //check BYTE 255U
  CIEC_BYTE nBTest(255U);
  sResult = CIEC_WSTRING("255");
  sTest = func_BYTE_AS_WSTRING(nBTest);
  //check result value
  BOOST_CHECK(sTest == sResult);
  //check length value
  BOOST_CHECK_EQUAL(sTest.length(), 3);

  //check LWORD 255U
  CIEC_LWORD nLWTest(18446744073709551615ULL);
  sResult = CIEC_WSTRING("18446744073709551615");
  sTest = func_LWORD_AS_WSTRING(nLWTest);
  //check result value
  BOOST_TEST(sTest == sResult);
  //check length value
  BOOST_CHECK_EQUAL(sTest.length(), 20);
}

BOOST_AUTO_TEST_CASE(INTEGERS_to_WSTRING_test)
{
  CIEC_WSTRING sTest;
  CIEC_WSTRING sResult;

  //check SINT
  CIEC_SINT nSTest(127);
  sResult = CIEC_WSTRING("127");
  sTest = func_SINT_AS_WSTRING(nSTest);
  //check result value
  BOOST_CHECK(sTest == sResult);
  //check length value
  BOOST_CHECK_EQUAL(sTest.length(), 3);

  //check USINT
  CIEC_USINT nUSTest(255);
  sResult = CIEC_WSTRING("255");
  sTest = func_USINT_AS_WSTRING(nUSTest);
  //check result value
  BOOST_CHECK(sTest == sResult);
  //check length value
  BOOST_CHECK_EQUAL(sTest.length(), 3);

  //check INT
  CIEC_INT nITest(32767);
  sResult = CIEC_WSTRING("32767");
  sTest = func_INT_AS_WSTRING(nITest);
  //check result value
  BOOST_CHECK(sTest == sResult);
  //check length value
  BOOST_CHECK_EQUAL(sTest.length(), 5);

  //check UINT
  CIEC_UINT nUITest(65535);
  sResult = CIEC_WSTRING("65535");
  sTest = func_UINT_AS_WSTRING(nUITest);
  //check result value
  BOOST_CHECK(sTest == sResult);
  //check length value
  BOOST_CHECK_EQUAL(sTest.length(), 5);

  //check DINT
  CIEC_DINT nDITest(2147483647);
  sResult = CIEC_WSTRING("2147483647");
  sTest = func_DINT_AS_WSTRING(nDITest);
  //check result value
  BOOST_CHECK(sTest == sResult);
  //check length value
  BOOST_CHECK_EQUAL(sTest.length(), 10);

  //check UDINT
  CIEC_UDINT nUDITest(4294967295ULL);
  sResult = CIEC_WSTRING("4294967295");
  sTest = func_UDINT_AS_WSTRING(nUDITest);
  //check result value
  BOOST_CHECK(sTest == sResult);
  //check length value
  BOOST_CHECK_EQUAL(sTest.length(), 10);

  //check LINT
  CIEC_LINT nLITest(9223372036854775807LL);
  sResult = CIEC_WSTRING("9223372036854775807");
  sTest = func_LINT_AS_WSTRING(nLITest);
  //check result value
  BOOST_CHECK(sTest == sResult);
  //check length value
  BOOST_CHECK_EQUAL(sTest.length(), 19);

  //check ULINT
  CIEC_ULINT nULITest(18446744073709551615ULL);
  sResult = CIEC_WSTRING("18446744073709551615");
  sTest = func_ULINT_AS_WSTRING(nULITest);
  //check result value
  BOOST_CHECK(sTest == sResult);
  //check length value
  BOOST_CHECK_EQUAL(sTest.length(), 20);
}

BOOST_AUTO_TEST_CASE(TIME_to_WSTRING_test)
{
  CIEC_WSTRING sTest;
  CIEC_WSTRING sResult;
  
  //check TIME
  CIEC_TIME tTest;
  tTest.fromString("T#3000ms");
  sResult = CIEC_WSTRING("T#3s");
  sTest = func_TIME_AS_WSTRING(tTest);
  //check result value
  BOOST_CHECK(sTest == sResult);
  //check length value
  BOOST_CHECK_EQUAL(sTest.length(), 4);
}

BOOST_AUTO_TEST_CASE(REAL_to_WSTRING_test)
{
  CIEC_WSTRING sTest;
  CIEC_WSTRING sResult;
  
  //check REAL
  CIEC_REAL nRTest(1.46e-3f);
  sResult = CIEC_WSTRING("0.00146000006");
  sTest = func_REAL_AS_WSTRING(nRTest);
  //check result value
  BOOST_TEST(sTest == sResult);
  //check length value
  BOOST_TEST(sTest.length() == 13);

  //check LREAL
  CIEC_LREAL nLRTest(-2.2874e6);
  sResult = CIEC_WSTRING("-2287400.0");
  sTest = func_LREAL_AS_WSTRING(nLRTest);
  //check result value
  BOOST_TEST(sTest == sResult);
  //check length value
  BOOST_TEST(sTest.length() == 10);
}

BOOST_AUTO_TEST_SUITE_END()
