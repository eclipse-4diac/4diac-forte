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
#include "../../../src/core/datatypes/forte_string.h"

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


BOOST_AUTO_TEST_SUITE(Datatypes_to_STRING_conversion_test)

BOOST_AUTO_TEST_CASE(BOOLS_AND_BITS_to_STRING_test)
{
  CIEC_STRING sTest;
  CIEC_STRING sResult;
  //check BOOL
  CIEC_BOOL bTest(true);
  sResult = "TRUE"_STRING;
  sTest = func_BOOL_AS_STRING(bTest);
  //check result value
  BOOST_TEST(sTest == sResult);
  //check length value
  BOOST_TEST(sTest.length() == 4);

  //check WORD
  CIEC_WORD nWTest(0xffff);
  sResult = "65535"_STRING;
  sTest = func_WORD_AS_STRING(nWTest);
  //check result value
  BOOST_TEST(sTest == sResult);
  //check length value
  BOOST_TEST(sTest.length() == 5);

  //check DWORD 4294967295
  CIEC_DWORD nDWTest(0xffffffff);
  sResult = "4294967295"_STRING;
  sTest = func_DWORD_AS_STRING(nDWTest);
  //check result value
  BOOST_TEST(sTest == sResult);
  //check length value
  BOOST_TEST(sTest.length() == 10);

  //check BYTE 255U
  CIEC_BYTE nBTest(255U);
  sResult = "255"_STRING;
  sTest = func_BYTE_AS_STRING(nBTest);
  //check result value
  BOOST_TEST(sTest == sResult);
  //check length value
  BOOST_TEST(sTest.length() == 3);

  //check LWORD 255U
  CIEC_LWORD nLWTest(18446744073709551615ULL);
  sResult = "18446744073709551615"_STRING;
  sTest = func_LWORD_AS_STRING(nLWTest);
  //check result value
  BOOST_TEST(sTest == sResult);
  //check length value
  BOOST_TEST(sTest.length() == 20);
}

BOOST_AUTO_TEST_CASE(INTEGERS_to_STRING_test)
{
  CIEC_STRING sTest;
  CIEC_STRING sResult;

  //check SINT
  CIEC_SINT nSTest(127);
  sResult = "127"_STRING;
  sTest = func_SINT_AS_STRING(nSTest);
  //check result value
  BOOST_TEST(sTest == sResult);
  //check length value
  BOOST_TEST(sTest.length() == 3);

  //check USINT
  CIEC_USINT nUSTest(255);
  sResult = "255"_STRING;
  sTest = func_USINT_AS_STRING(nUSTest);
  //check result value
  BOOST_TEST(sTest == sResult);
  //check length value
  BOOST_TEST(sTest.length() == 3);

  //check INT
  CIEC_INT nITest(32767);
  sResult = "32767"_STRING;
  sTest = func_INT_AS_STRING(nITest);
  //check result value
  BOOST_TEST(sTest == sResult);
  //check length value
  BOOST_TEST(sTest.length() == 5);

  //check UINT
  CIEC_UINT nUITest(65535);
  sResult = "65535"_STRING;
  sTest = func_UINT_AS_STRING(nUITest);
  //check result value
  BOOST_TEST(sTest == sResult);
  //check length value
  BOOST_TEST(sTest.length() == 5);

  //check DINT
  CIEC_DINT nDITest(2147483647);
  sResult = "2147483647"_STRING;
  sTest = func_DINT_AS_STRING(nDITest);
  //check result value
  BOOST_TEST(sTest == sResult);
  //check length value
  BOOST_TEST(sTest.length() == 10);

  //check UDINT
  CIEC_UDINT nUDITest(4294967295ULL);
  sResult = "4294967295"_STRING;
  sTest = func_UDINT_AS_STRING(nUDITest);
  //check result value
  BOOST_TEST(sTest == sResult);
  //check length value
  BOOST_TEST(sTest.length() == 10);

  //check LINT
  CIEC_LINT nLITest(9223372036854775807LL);
  sResult = "9223372036854775807"_STRING;
  sTest = func_LINT_AS_STRING(nLITest);
  //check result value
  BOOST_TEST(sTest == sResult);
  //check length value
  BOOST_TEST(sTest.length() == 19);

  //check ULINT
  CIEC_ULINT nULITest(18446744073709551615ULL);
  sResult = "18446744073709551615"_STRING;
  sTest = func_ULINT_AS_STRING(nULITest);
  //check result value
  BOOST_TEST(sTest == sResult);
  //check length value
  BOOST_TEST(sTest.length() == 20);
}

BOOST_AUTO_TEST_CASE(TIME_to_STRING_test)
{
  CIEC_STRING sTest;
  CIEC_STRING sResult;
  
  //check TIME
  CIEC_TIME tTest;
  tTest.fromString("T#3000ms");
  sResult = "T#3s"_STRING;
  sTest = func_TIME_AS_STRING(tTest);
  //check result value
  BOOST_TEST(sTest == sResult);
  //check length value
  BOOST_TEST(sTest.length() == 4);
}

BOOST_AUTO_TEST_CASE(REAL_to_STRING_test) {
  CIEC_STRING sTest;
  CIEC_STRING sResult;
  
  //check REAL
  CIEC_REAL nRTest(1.46e-3f);
  sResult = "0.00146000006"_STRING;
  sTest = func_REAL_AS_STRING(nRTest);
  //check result value
  BOOST_TEST(sTest == sResult);
  //check length value
  BOOST_TEST(sTest.length() == 13);

  //check LREAL
  CIEC_LREAL nLRTest(-2.2874e6);
  sResult = "-2287400.0"_STRING;
  sTest = func_LREAL_AS_STRING(nLRTest);
  //check result value
  BOOST_TEST(sTest == sResult);
  //check length value
  BOOST_TEST(sTest.length() == 10);
}

BOOST_AUTO_TEST_SUITE_END()
