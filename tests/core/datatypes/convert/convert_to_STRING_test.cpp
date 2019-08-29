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

#ifdef FORTE_USE_REAL_DATATYPE
  #include <boost/test/floating_point_comparison.hpp>

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

BOOST_AUTO_TEST_SUITE(Datatypes_to_STRING_conversion_test)

BOOST_AUTO_TEST_CASE(BOOLS_AND_BITS_to_STRING_test)
{
  CIEC_STRING sTest;
  CIEC_STRING sResult;
  //check BOOL
  CIEC_BOOL bTest = true;
  sResult = "TRUE";
  sTest = BOOL_TO_STRING(bTest);
  //check result value
  BOOST_CHECK(sTest == sResult);
  //check length value
  BOOST_CHECK_EQUAL(sTest.length(), 4);

  //check WORD
  CIEC_WORD nWTest = 0xffff;
  sResult = "65535";
  sTest = WORD_TO_STRING(nWTest);
  //check result value
  BOOST_CHECK(sTest == sResult);
  //check length value
  BOOST_CHECK_EQUAL(sTest.length(), 5);

  //check DWORD 4294967295
  CIEC_DWORD nDWTest = 0xffffffff;
  sResult = "4294967295";
  sTest = DWORD_TO_STRING(nDWTest);
  //check result value
  BOOST_CHECK(sTest == sResult);
  //check length value
  BOOST_CHECK_EQUAL(sTest.length(), 10);

  //check BYTE 255U
  CIEC_BYTE nBTest; 
  nBTest= 255U;
  sResult = "255";
  sTest = BYTE_TO_STRING(nBTest);
  //check result value
  BOOST_CHECK(sTest == sResult);
  //check length value
  BOOST_CHECK_EQUAL(sTest.length(), 3);

#ifdef FORTE_USE_64BIT_DATATYPES
  //check LWORD 255U
  CIEC_LWORD nLWTest; 
  nLWTest= 18446744073709551615ULL;
  sResult = "18446744073709551615";
  sTest = LWORD_TO_STRING(nLWTest);
  //check result value
  BOOST_CHECK(sTest == sResult);
  //check length value
  BOOST_CHECK_EQUAL(sTest.length(), 20);
#endif //FORTE_USE_64BIT_DATATYPES
}

BOOST_AUTO_TEST_CASE(INTEGERS_to_STRING_test)
{
  CIEC_STRING sTest;
  CIEC_STRING sResult;

  //check SINT
  CIEC_SINT nSTest = 127;
  sResult = "127";
  sTest = SINT_TO_STRING(nSTest);
  //check result value
  BOOST_CHECK(sTest == sResult);
  //check length value
  BOOST_CHECK_EQUAL(sTest.length(), 3);

  //check USINT
  CIEC_USINT nUSTest = 255;
  sResult = "255";
  sTest = USINT_TO_STRING(nUSTest);
  //check result value
  BOOST_CHECK(sTest == sResult);
  //check length value
  BOOST_CHECK_EQUAL(sTest.length(), 3);

  //check INT
  CIEC_INT nITest = 32767;
  sResult = "32767";
  sTest = INT_TO_STRING(nITest);
  //check result value
  BOOST_CHECK(sTest == sResult);
  //check length value
  BOOST_CHECK_EQUAL(sTest.length(), 5);

  //check UINT
  CIEC_UINT nUITest = 65535;
  sResult = "65535";
  sTest = UINT_TO_STRING(nUITest);
  //check result value
  BOOST_CHECK(sTest == sResult);
  //check length value
  BOOST_CHECK_EQUAL(sTest.length(), 5);

  //check DINT
  CIEC_DINT nDITest = 2147483647;
  sResult = "2147483647";
  sTest = DINT_TO_STRING(nDITest);
  //check result value
  BOOST_CHECK(sTest == sResult);
  //check length value
  BOOST_CHECK_EQUAL(sTest.length(), 10);

  //check UDINT
  CIEC_UDINT nUDITest = 4294967295ULL;
  sResult = "4294967295";
  sTest = UDINT_TO_STRING(nUDITest);
  //check result value
  BOOST_CHECK(sTest == sResult);
  //check length value
  BOOST_CHECK_EQUAL(sTest.length(), 10);

#ifdef FORTE_USE_64BIT_DATATYPES
  //check LINT
  CIEC_LINT nLITest; 
  nLITest= 9223372036854775807LL;
  sResult = "9223372036854775807";
  sTest = LINT_TO_STRING(nLITest);
  //check result value
  BOOST_CHECK(sTest == sResult);
  //check length value
  BOOST_CHECK_EQUAL(sTest.length(), 19);

  //check ULINT
  CIEC_ULINT nULITest; 
  nULITest= 18446744073709551615ULL;
  sResult = "18446744073709551615";
  sTest = ULINT_TO_STRING(nULITest);
  //check result value
  BOOST_CHECK(sTest == sResult);
  //check length value
  BOOST_CHECK_EQUAL(sTest.length(), 20);
#endif //FORTE_USE_64BIT_DATATYPES
}

BOOST_AUTO_TEST_CASE(TIME_to_STRING_test)
{
  CIEC_STRING sTest;
  CIEC_STRING sResult;
  
  //check TIME
  CIEC_TIME tTest = "T#3000ms";
  sResult = "T#3000ms";
  sTest = TIME_TO_STRING(tTest);
  //check result value
  BOOST_CHECK(sTest == sResult);
  //check length value
  BOOST_CHECK_EQUAL(sTest.length(), 8);
}

#ifdef FORTE_USE_REAL_DATATYPE
BOOST_AUTO_TEST_CASE(REAL_to_STRING_test)
{
  CIEC_STRING sTest;
  CIEC_STRING sResult;
  
  //check REAL
  CIEC_REAL nRTest = 1.46e-3f;
  sResult = "0.00146";
  sTest = REAL_TO_STRING(nRTest);
  //check result value
  BOOST_CHECK(sTest == sResult);
  //check length value
  BOOST_CHECK_EQUAL(sTest.length(), 7);

#ifdef FORTE_USE_64BIT_DATATYPES
  //check LREAL
  CIEC_LREAL nLRTest = -2.2874e6;
  sResult = "-2287400";
  sTest = LREAL_TO_STRING(nLRTest);
  //check result value
  BOOST_CHECK(sTest == sResult);
  //check length value
  BOOST_CHECK_EQUAL(sTest.length(), 8);
#endif //FORTE_USE_64BIT_DATATYPES
}
#endif //FORTE_USE_REAL_DATATYPE

BOOST_AUTO_TEST_SUITE_END()
