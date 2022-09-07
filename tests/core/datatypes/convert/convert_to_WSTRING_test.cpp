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

#ifdef FORTE_USE_REAL_DATATYPE
  #include <boost/test/tools/floating_point_comparison.hpp>

  #include "../../../src/core/datatypes/forte_real.h"
#endif //FORTE_USE_REAL_DATATYPE

#ifdef FORTE_USE_64BIT_DATATYPES
  #include "../../../src/core/datatypes/forte_lword.h"
  #include "../../../src/core/datatypes/forte_lint.h"
  #include "../../../src/core/datatypes/forte_ulint.h"

#ifdef FORTE_USE_REAL_DATATYPE
  #include "../../../src/core/datatypes/forte_lreal.h"
#endif //FORTE_USE_REAL_DATATYPE
#endif //FORTE_USE_64BIT_DATATYPES

BOOST_AUTO_TEST_SUITE(Datatypes_to_WSTRING_conversion_test)

BOOST_AUTO_TEST_CASE(BOOLS_AND_BITS_to_WSTRING_test)
{
  CIEC_WSTRING sTest;
  CIEC_WSTRING sResult;
  //check BOOL
  CIEC_BOOL bTest(true);
  sResult = CIEC_WSTRING("TRUE");
  sTest = func_BOOL_TO_WSTRING(bTest);
  //check result value
  BOOST_CHECK(sTest == sResult);
  //check length value
  BOOST_CHECK_EQUAL(sTest.length(), 4);

  //check WORD
  CIEC_WORD nWTest(0xffff);
  sResult = CIEC_WSTRING("65535");
  sTest = func_WORD_TO_WSTRING(nWTest);
  //check result value
  BOOST_CHECK(sTest == sResult);
  //check length value
  BOOST_CHECK_EQUAL(sTest.length(), 5);

  //check DWORD 4294967295
  CIEC_DWORD nDWTest(0xffffffff);
  sResult = CIEC_WSTRING("4294967295");
  sTest = func_DWORD_TO_WSTRING(nDWTest);
  //check result value
  BOOST_CHECK(sTest == sResult);
  //check length value
  BOOST_CHECK_EQUAL(sTest.length(), 10);

  //check BYTE 255U
  CIEC_BYTE nBTest(255U);
  sResult = CIEC_WSTRING("255");
  sTest = func_BYTE_TO_WSTRING(nBTest);
  //check result value
  BOOST_CHECK(sTest == sResult);
  //check length value
  BOOST_CHECK_EQUAL(sTest.length(), 3);

#ifdef FORTE_USE_64BIT_DATATYPES
  //check LWORD 255U
  CIEC_LWORD nLWTest(18446744073709551615ULL);
  sResult = CIEC_WSTRING("18446744073709551615");
  sTest = func_LWORD_TO_WSTRING(nLWTest);
  //check result value
  BOOST_CHECK(sTest == sResult);
  //check length value
  BOOST_CHECK_EQUAL(sTest.length(), 20);
#endif //FORTE_USE_64BIT_DATATYPES
}

BOOST_AUTO_TEST_CASE(INTEGERS_to_WSTRING_test)
{
  CIEC_WSTRING sTest;
  CIEC_WSTRING sResult;

  //check SINT
  CIEC_SINT nSTest(127);
  sResult = CIEC_WSTRING("127");
  sTest = func_SINT_TO_WSTRING(nSTest);
  //check result value
  BOOST_CHECK(sTest == sResult);
  //check length value
  BOOST_CHECK_EQUAL(sTest.length(), 3);

  //check USINT
  CIEC_USINT nUSTest(255);
  sResult = CIEC_WSTRING("255");
  sTest = func_USINT_TO_WSTRING(nUSTest);
  //check result value
  BOOST_CHECK(sTest == sResult);
  //check length value
  BOOST_CHECK_EQUAL(sTest.length(), 3);

  //check INT
  CIEC_INT nITest(32767);
  sResult = CIEC_WSTRING("32767");
  sTest = func_INT_TO_WSTRING(nITest);
  //check result value
  BOOST_CHECK(sTest == sResult);
  //check length value
  BOOST_CHECK_EQUAL(sTest.length(), 5);

  //check UINT
  CIEC_UINT nUITest(65535);
  sResult = CIEC_WSTRING("65535");
  sTest = func_UINT_TO_WSTRING(nUITest);
  //check result value
  BOOST_CHECK(sTest == sResult);
  //check length value
  BOOST_CHECK_EQUAL(sTest.length(), 5);

  //check DINT
  CIEC_DINT nDITest(2147483647);
  sResult = CIEC_WSTRING("2147483647");
  sTest = func_DINT_TO_WSTRING(nDITest);
  //check result value
  BOOST_CHECK(sTest == sResult);
  //check length value
  BOOST_CHECK_EQUAL(sTest.length(), 10);

  //check UDINT
  CIEC_UDINT nUDITest(4294967295ULL);
  sResult = CIEC_WSTRING("4294967295");
  sTest = func_UDINT_TO_WSTRING(nUDITest);
  //check result value
  BOOST_CHECK(sTest == sResult);
  //check length value
  BOOST_CHECK_EQUAL(sTest.length(), 10);

#ifdef FORTE_USE_64BIT_DATATYPES
  //check LINT
  CIEC_LINT nLITest(9223372036854775807LL);
  sResult = CIEC_WSTRING("9223372036854775807");
  sTest = func_LINT_TO_WSTRING(nLITest);
  //check result value
  BOOST_CHECK(sTest == sResult);
  //check length value
  BOOST_CHECK_EQUAL(sTest.length(), 19);

  //check ULINT
  CIEC_ULINT nULITest(18446744073709551615ULL);
  sResult = CIEC_WSTRING("18446744073709551615");
  sTest = func_ULINT_TO_WSTRING(nULITest);
  //check result value
  BOOST_CHECK(sTest == sResult);
  //check length value
  BOOST_CHECK_EQUAL(sTest.length(), 20);
#endif //FORTE_USE_64BIT_DATATYPES
}

BOOST_AUTO_TEST_CASE(TIME_to_WSTRING_test)
{
  CIEC_WSTRING sTest;
  CIEC_WSTRING sResult;
  
  //check TIME
  CIEC_TIME tTest;
  tTest.fromString("T#3000ms");
  sResult = CIEC_WSTRING("T#3000ms");
  sTest = func_TIME_TO_WSTRING(tTest);
  //check result value
  BOOST_CHECK(sTest == sResult);
  //check length value
  BOOST_CHECK_EQUAL(sTest.length(), 8);
}

#ifdef FORTE_USE_REAL_DATATYPE
BOOST_AUTO_TEST_CASE(REAL_to_WSTRING_test)
{
  CIEC_WSTRING sTest;
  CIEC_WSTRING sResult;
  
  //check REAL
  CIEC_REAL nRTest(1.46e-3f);
  sResult = CIEC_WSTRING("0.00146");
  sTest = func_REAL_TO_WSTRING(nRTest);
  //check result value
  BOOST_CHECK(sTest == sResult);
  //check length value
  BOOST_CHECK_EQUAL(sTest.length(), 7);

#ifdef FORTE_USE_64BIT_DATATYPES
  //check LREAL
  CIEC_LREAL nLRTest(-2.2874e6);
  sResult = CIEC_WSTRING("-2287400.0");
  sTest = func_LREAL_TO_WSTRING(nLRTest);
  //check result value
  BOOST_CHECK(sTest == sResult);
  //check length value
  BOOST_CHECK_EQUAL(sTest.length(), 10);
#endif //FORTE_USE_64BIT_DATATYPES
}
#endif //FORTE_USE_REAL_DATATYPE

BOOST_AUTO_TEST_SUITE_END()
