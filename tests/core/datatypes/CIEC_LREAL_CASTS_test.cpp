/*******************************************************************************
 * Copyright (c) 2011 - 2012 ACIN, nxtControl, 2018 TU Vienna/ACIN
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Martin Melik Merkumians, Ingo Hegny, Alois Zoitl, Stanislav Meduna - initial API and implementation and/or initial documentation
 *   Martin Melik Merkumians - Adds cast template tests
 *******************************************************************************/
#ifdef FORTE_USE_64BIT_DATATYPES
#ifdef FORTE_USE_REAL_DATATYPE

#include <boost/test/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>

#include "../../../src/core/datatypes/forte_real.h"

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
//STRING-Datatypes
#include "../../../src/core/datatypes/forte_string.h"
#include "../../../src/core/datatypes/forte_wstring.h"
//TIME
#include "../../../src/core/datatypes/forte_time.h"

//64bit Datatypes
#include "../../../src/core/datatypes/forte_lword.h"
#include "../../../src/core/datatypes/forte_lint.h"
#include "../../../src/core/datatypes/forte_ulint.h"
#include "../../../src/core/datatypes/forte_lreal.h"

using namespace boost::unit_test;

BOOST_AUTO_TEST_SUITE(CIEC_LREAL_casts_function_test)


BOOST_AUTO_TEST_CASE(CASTS_LREAL_to_BOOL)
{
  CIEC_LREAL nTestLReal0;
  CIEC_LREAL nTestLReal1;
  CIEC_LREAL nTestLReal2_2874e6;
  CIEC_LREAL nTestLRealm6_2587em4;
  CIEC_LREAL nTestLReal1_0em37;
  CIEC_LREAL nTestLReal36;

  CIEC_BOOL bTestBool;

//check initial values
  BOOST_CHECK_EQUAL(nTestLReal0, 0);
  BOOST_CHECK_EQUAL(nTestLReal1, 0);
  BOOST_CHECK_EQUAL(nTestLReal2_2874e6, 0);
  BOOST_CHECK_EQUAL(nTestLRealm6_2587em4, 0);
  BOOST_CHECK_EQUAL(nTestLReal1_0em37, 0);
  BOOST_CHECK_EQUAL(nTestLReal36, 0);

  nTestLReal0=0.0;
  nTestLReal1=1.0;
  nTestLReal2_2874e6=2.2874e6;
  nTestLRealm6_2587em4=-6.2587e-4;
  nTestLReal1_0em37=1.0e-37;
  nTestLReal36=36.0;


  BOOST_CHECK_EQUAL(0.0, nTestLReal0);
  BOOST_CHECK_EQUAL(1.0, nTestLReal1);
  BOOST_CHECK_EQUAL(2.2874e6, nTestLReal2_2874e6);
  BOOST_CHECK_EQUAL(-6.2587e-4, nTestLRealm6_2587em4);
  BOOST_CHECK_EQUAL(1.0e-37, nTestLReal1_0em37);
  BOOST_CHECK_EQUAL(36.0, nTestLReal36);

//checks
  CIEC_ANY::specialCast(nTestLReal0, bTestBool);
  BOOST_CHECK_EQUAL(false, bTestBool);
  CIEC_ANY::specialCast(nTestLReal1, bTestBool);
  BOOST_CHECK_EQUAL(true, bTestBool);
  CIEC_ANY::specialCast(nTestLReal2_2874e6, bTestBool);
  BOOST_CHECK_EQUAL(true, bTestBool);
  CIEC_ANY::specialCast(nTestLReal36, bTestBool);
  BOOST_CHECK_EQUAL(true, bTestBool);
  CIEC_ANY::specialCast(nTestLRealm6_2587em4, bTestBool);
  BOOST_CHECK_EQUAL(false, bTestBool);
}

BOOST_AUTO_TEST_CASE(CASTS_LREAL_to_BITDTs)
{
  CIEC_BYTE nTestByte;
  CIEC_WORD nTestWord;
  CIEC_DWORD nTestDWord;
  CIEC_LWORD nTestLWord;

  CIEC_LREAL nTestLReal0;
  CIEC_LREAL nTestLReal1;
  CIEC_LREAL nTestLReal2_2874e6;
  CIEC_LREAL nTestLRealm6_2587em4;
  CIEC_LREAL nTestLReal1_0em37;
  CIEC_LREAL nTestLReal36;

//check initial values
  BOOST_CHECK_EQUAL(nTestLReal0, 0.0);
  BOOST_CHECK_EQUAL(nTestLReal1, 0.0);
  BOOST_CHECK_EQUAL(nTestLReal2_2874e6, 0.0);
  BOOST_CHECK_EQUAL(nTestLRealm6_2587em4, 0.0);
  BOOST_CHECK_EQUAL(nTestLReal1_0em37, 0.0);
  BOOST_CHECK_EQUAL(nTestLReal36, 0);

  nTestLReal0=0.0;
  nTestLReal1=1.0;
  nTestLReal2_2874e6=2.2874e6;
  nTestLRealm6_2587em4=-6.2587e-4;
  nTestLReal1_0em37=1.0e-37;
  nTestLReal36=36.0;

  //the 4byte hex value for 36.0:        0x4042000000000000
  //the 4byte hex value for -6.2587e-4f: 0xbf44822d973cf2fb
  //the 4byte hex value for 1:           0x3ff0000000000000

  BOOST_CHECK_EQUAL(0.0, nTestLReal0);
  BOOST_CHECK_EQUAL(1.0, nTestLReal1);
  BOOST_CHECK_EQUAL(2.2874e6, nTestLReal2_2874e6);
  BOOST_CHECK_EQUAL(-6.2587e-4, nTestLRealm6_2587em4);
  BOOST_CHECK_EQUAL(1.0e-37, nTestLReal1_0em37);
  BOOST_CHECK_EQUAL(36.0, nTestLReal36);

//toByte
  nTestByte.setValue(nTestLReal0);
  CIEC_ANY::specialCast(nTestLReal0, nTestByte);
  BOOST_CHECK_EQUAL(nTestByte, 0U);
  CIEC_ANY::specialCast(nTestLReal1, nTestByte);
  BOOST_CHECK_EQUAL(nTestByte, 0);
  CIEC_ANY::specialCast(nTestLReal36, nTestByte);
  BOOST_CHECK_EQUAL(nTestByte, 0);
  CIEC_ANY::specialCast(nTestLRealm6_2587em4, nTestByte);
  BOOST_CHECK_EQUAL(nTestByte, 0xfb);

//toWord
  CIEC_ANY::specialCast(nTestLReal0, nTestWord);
  BOOST_CHECK_EQUAL(nTestWord, 0U);
  CIEC_ANY::specialCast(nTestLReal1, nTestWord);
  BOOST_CHECK_EQUAL(nTestWord, 0x0);
  CIEC_ANY::specialCast(nTestLReal36, nTestWord);
  BOOST_CHECK_EQUAL(nTestWord, 0x0);
  CIEC_ANY::specialCast(nTestLRealm6_2587em4, nTestWord);
  BOOST_CHECK_EQUAL(nTestWord, 0xf2fb);

//toDWord
  CIEC_ANY::specialCast(nTestLReal0, nTestDWord);
  BOOST_CHECK_EQUAL(nTestDWord, 0UL);
  CIEC_ANY::specialCast(nTestLReal1, nTestDWord);
  BOOST_CHECK_EQUAL(nTestDWord, 0x0);
  CIEC_ANY::specialCast(nTestLReal36, nTestDWord);
  BOOST_CHECK_EQUAL(nTestDWord, 0x0);
  CIEC_ANY::specialCast(nTestLRealm6_2587em4, nTestDWord);
  BOOST_CHECK_EQUAL(nTestDWord, 0x973cf2fb);

//toLWord
  CIEC_ANY::specialCast(nTestLReal0, nTestLWord);
  BOOST_CHECK_EQUAL(nTestLWord, 0ULL);
  CIEC_ANY::specialCast(nTestLReal1, nTestLWord);
  BOOST_CHECK_EQUAL(0x3ff0000000000000ULL, nTestLWord);
  CIEC_ANY::specialCast(nTestLReal36, nTestLWord);
  BOOST_CHECK_EQUAL(0x4042000000000000ULL, nTestLWord);
  CIEC_ANY::specialCast(nTestLRealm6_2587em4, nTestLWord);
  BOOST_CHECK_EQUAL(nTestLWord, 0xbf44822d973cf2fbULL);
}

BOOST_AUTO_TEST_CASE(CASTS_LREAL_to_INTS)
{
  CIEC_USINT nTestUSInt;
  CIEC_INT nTestInt;
  CIEC_UINT nTestUInt;
  CIEC_DINT nTestDInt;
  CIEC_UDINT nTestUDInt;
  CIEC_LINT nTestLInt;
  CIEC_ULINT nTestULInt;

  CIEC_LREAL nTestLReal0;
  CIEC_LREAL nTestLReal1;
  CIEC_LREAL nTestLReal2_2874e6;
  CIEC_LREAL nTestLRealm6_2587em4;
  CIEC_LREAL nTestLReal1_0em37;
  CIEC_LREAL nTestLReal36;

//check initial values
  BOOST_CHECK_EQUAL(nTestLReal0, 0);
  BOOST_CHECK_EQUAL(nTestLReal1, 0);
  BOOST_CHECK_EQUAL(nTestLReal2_2874e6, 0);
  BOOST_CHECK_EQUAL(nTestLRealm6_2587em4, 0);
  BOOST_CHECK_EQUAL(nTestLReal1_0em37, 0);
  BOOST_CHECK_EQUAL(nTestLReal36, 0);

  nTestLReal0=0.0;
  nTestLReal1=1.0;
  nTestLReal2_2874e6=2.2874e6;
  nTestLRealm6_2587em4=-6.2587e-4;
  nTestLReal1_0em37=1.0e-37;
  nTestLReal36=36.0;

  BOOST_CHECK_EQUAL(0.0, nTestLReal0);
  BOOST_CHECK_EQUAL(1.0, nTestLReal1);
  BOOST_CHECK_EQUAL(2.2874e6, nTestLReal2_2874e6);
  BOOST_CHECK_EQUAL(-6.2587e-4, nTestLRealm6_2587em4);
  BOOST_CHECK_EQUAL(1.0e-37, nTestLReal1_0em37);
  BOOST_CHECK_EQUAL(36.0, nTestLReal36);

//toUSINT
  CIEC_ANY::specialCast(nTestLReal0, nTestUSInt);
  BOOST_CHECK_EQUAL(nTestUSInt, 0U);
  CIEC_ANY::specialCast(nTestLReal1, nTestUSInt);
  BOOST_CHECK_EQUAL(nTestUSInt, 1U);
  CIEC_ANY::specialCast(nTestLReal36, nTestUSInt);
  BOOST_CHECK_EQUAL(nTestUSInt, 36U);

//toINT
  CIEC_ANY::specialCast(nTestLReal0, nTestInt);
  BOOST_CHECK_EQUAL(nTestInt, 0);
  CIEC_ANY::specialCast(nTestLReal1, nTestInt);
  BOOST_CHECK_EQUAL(nTestInt, 1);
  CIEC_ANY::specialCast(nTestLReal36, nTestInt);
  BOOST_CHECK_EQUAL(nTestInt, 36);

//toUINT
    CIEC_ANY::specialCast(nTestLReal0, nTestUInt);
    BOOST_CHECK_EQUAL(nTestUInt, 0U);
    CIEC_ANY::specialCast(nTestLReal1, nTestUInt);
    BOOST_CHECK_EQUAL(nTestUInt, 1U);
    CIEC_ANY::specialCast(nTestLReal36, nTestUInt);
    BOOST_CHECK_EQUAL(nTestUInt, 36U);

//toDINT
  CIEC_ANY::specialCast(nTestLReal0, nTestDInt);
  BOOST_CHECK_EQUAL(nTestDInt, 0L);
  CIEC_ANY::specialCast(nTestLReal1, nTestDInt);
  BOOST_CHECK_EQUAL(nTestDInt, 1L);
  CIEC_ANY::specialCast(nTestLReal36, nTestDInt);
  BOOST_CHECK_EQUAL(nTestDInt, 36L);

//toUDINT
  CIEC_ANY::specialCast(nTestLReal0, nTestUDInt);
  BOOST_CHECK_EQUAL(nTestUDInt, 0UL);
  CIEC_ANY::specialCast(nTestLReal1, nTestUDInt);
  BOOST_CHECK_EQUAL(nTestUDInt, 1UL);
  CIEC_ANY::specialCast(nTestLReal36, nTestUDInt);
  BOOST_CHECK_EQUAL(nTestUDInt, 36UL);

//toLINT
  CIEC_ANY::specialCast(nTestLReal0, nTestLInt);
  BOOST_CHECK_EQUAL(nTestLInt, 0LL);
  CIEC_ANY::specialCast(nTestLReal1, nTestLInt);
  BOOST_CHECK_EQUAL(nTestLInt, 1LL);
  CIEC_ANY::specialCast(nTestLReal36, nTestLInt);
  BOOST_CHECK_EQUAL(nTestLInt, 36LL);

//toULINT
  CIEC_ANY::specialCast(nTestLReal0, nTestULInt);
  BOOST_CHECK_EQUAL(nTestULInt, 0ULL);
  CIEC_ANY::specialCast(nTestLReal1, nTestULInt);
  BOOST_CHECK_EQUAL(nTestULInt, 1ULL);
  CIEC_ANY::specialCast(nTestLReal36, nTestULInt);
  BOOST_CHECK_EQUAL(nTestULInt, 36ULL);
}

BOOST_AUTO_TEST_CASE(CASTS_LREAL_to_REAL)
{
  CIEC_LREAL nTestLReal0;
  CIEC_LREAL nTestLReal1;
  CIEC_LREAL nTestLReal2_2874e6;
  CIEC_LREAL nTestLRealm6_2587em4;
  CIEC_LREAL nTestLReal1_0em37;
  CIEC_LREAL nTestLReal36;

//check initial values
  BOOST_CHECK_EQUAL(nTestLReal0, 0);
  BOOST_CHECK_EQUAL(nTestLReal1, 0);
  BOOST_CHECK_EQUAL(nTestLReal2_2874e6, 0);
  BOOST_CHECK_EQUAL(nTestLRealm6_2587em4, 0);
  BOOST_CHECK_EQUAL(nTestLReal1_0em37, 0);
  BOOST_CHECK_EQUAL(nTestLReal36, 0);

  nTestLReal0=0.0;
  nTestLReal1=1.0;
  nTestLReal2_2874e6=2.2874e6;
  nTestLRealm6_2587em4=-6.2587e-4;
  nTestLReal1_0em37=1.0e-37;
  nTestLReal36=36.0;

  BOOST_CHECK_EQUAL(0.0, nTestLReal0);
  BOOST_CHECK_EQUAL(1.0, nTestLReal1);
  BOOST_CHECK_EQUAL(2.2874e6, nTestLReal2_2874e6);
  BOOST_CHECK_EQUAL(-6.2587e-4, nTestLRealm6_2587em4);
  BOOST_CHECK_EQUAL(1.0e-37, nTestLReal1_0em37);
  BOOST_CHECK_EQUAL(36.0, nTestLReal36);



  CIEC_REAL nTestReal;

//toLREAL
  BOOST_CHECK_EQUAL(1.0, nTestLReal1);
  BOOST_CHECK_EQUAL(2.2874e6, nTestLReal2_2874e6);
  BOOST_CHECK_EQUAL(-6.2587e-4, nTestLRealm6_2587em4);
  BOOST_CHECK_EQUAL(1.0e-37, nTestLReal1_0em37);
  BOOST_CHECK_EQUAL(36.0, nTestLReal36);

  CIEC_ANY::specialCast(nTestLReal0, nTestReal);
  BOOST_CHECK_EQUAL(0.0f, nTestReal);
  CIEC_ANY::specialCast(nTestLReal1, nTestReal);
  BOOST_CHECK_EQUAL(1.0f, nTestReal);
  CIEC_ANY::specialCast(nTestLReal2_2874e6, nTestReal);
  BOOST_CHECK_EQUAL(2.2874e6f, nTestReal);
  CIEC_ANY::specialCast(nTestLRealm6_2587em4, nTestReal);
  BOOST_CHECK_EQUAL(-6.2587e-4f, nTestReal);
  CIEC_ANY::specialCast(nTestLReal1_0em37, nTestReal);
  BOOST_CHECK_EQUAL(1.0e-37f, nTestReal);
  CIEC_ANY::specialCast(nTestLReal36, nTestReal);
  BOOST_CHECK_EQUAL(36.0f, nTestReal);
}

BOOST_AUTO_TEST_CASE(CASTS_LREAL_to_TIME)
{
  CIEC_TIME nTestTime;

  CIEC_LREAL nTestLReal0;
  CIEC_LREAL nTestLReal1;
  CIEC_LREAL nTestLReal2_2874e6;
  CIEC_LREAL nTestLRealm6_2587em4;
  CIEC_LREAL nTestLReal1_0em37;
  CIEC_LREAL nTestLReal36;

//check initial values
  BOOST_CHECK_EQUAL(nTestLReal0, 0);
  BOOST_CHECK_EQUAL(nTestLReal1, 0);
  BOOST_CHECK_EQUAL(nTestLReal2_2874e6, 0);
  BOOST_CHECK_EQUAL(nTestLRealm6_2587em4, 0);
  BOOST_CHECK_EQUAL(nTestLReal1_0em37, 0);
  BOOST_CHECK_EQUAL(nTestLReal36, 0);

  nTestLReal0=0.0;
  nTestLReal1=1.0;
  nTestLReal2_2874e6=2.2874e6;
  nTestLRealm6_2587em4=-6.2587e-4;
  nTestLReal1_0em37=1.0e-37;
  nTestLReal36=36.0;

  BOOST_CHECK_EQUAL(0.0, nTestLReal0);
  BOOST_CHECK_EQUAL(1.0, nTestLReal1);
  BOOST_CHECK_EQUAL(2.2874e6, nTestLReal2_2874e6);
  BOOST_CHECK_EQUAL(-6.2587e-4, nTestLRealm6_2587em4);
  BOOST_CHECK_EQUAL(1.0e-37, nTestLReal1_0em37);
  BOOST_CHECK_EQUAL(36.0, nTestLReal36);

//toTime
  CIEC_ANY::specialCast(nTestLReal0, nTestTime);
  BOOST_CHECK_EQUAL(nTestTime, 0);
  CIEC_ANY::specialCast(nTestLReal1, nTestTime);
  BOOST_CHECK_EQUAL(nTestTime, 1);
  CIEC_ANY::specialCast(nTestLReal36, nTestTime);
  BOOST_CHECK_EQUAL(nTestTime, 36);

}

BOOST_AUTO_TEST_CASE(LREAL_Castable_test)
{
  bool bUp, bDown;

  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_LREAL, CIEC_ANY::e_BOOL, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(bDown);

  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_LREAL, CIEC_ANY::e_SINT, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_LREAL, CIEC_ANY::e_INT, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_LREAL, CIEC_ANY::e_DINT, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_LREAL, CIEC_ANY::e_LINT, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(bDown);

  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_LREAL, CIEC_ANY::e_USINT, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_LREAL, CIEC_ANY::e_UINT, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_LREAL, CIEC_ANY::e_UDINT, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_LREAL, CIEC_ANY::e_ULINT, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(bDown);

  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_LREAL, CIEC_ANY::e_BYTE, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_LREAL, CIEC_ANY::e_WORD, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_LREAL, CIEC_ANY::e_DWORD, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_LREAL, CIEC_ANY::e_LWORD, bUp, bDown)); BOOST_CHECK(bUp); BOOST_CHECK(! bDown);

  BOOST_CHECK(!CIEC_ANY::isCastable(CIEC_ANY::e_LREAL, CIEC_ANY::e_DATE, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(!CIEC_ANY::isCastable(CIEC_ANY::e_LREAL, CIEC_ANY::e_TIME_OF_DAY, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(!CIEC_ANY::isCastable(CIEC_ANY::e_LREAL, CIEC_ANY::e_DATE_AND_TIME, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(!CIEC_ANY::isCastable(CIEC_ANY::e_LREAL, CIEC_ANY::e_TIME, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(! bDown);

  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_LREAL, CIEC_ANY::e_REAL, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_LREAL, CIEC_ANY::e_LREAL, bUp, bDown)); BOOST_CHECK(bUp); BOOST_CHECK(! bDown);

  BOOST_CHECK(!CIEC_ANY::isCastable(CIEC_ANY::e_LREAL, CIEC_ANY::e_STRING, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(!CIEC_ANY::isCastable(CIEC_ANY::e_LREAL, CIEC_ANY::e_WSTRING, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(! bDown);
}

BOOST_AUTO_TEST_CASE(Explict_cast_operator_to_LREAL)
{

  CIEC_USINT nUsint(2);
  CIEC_UINT nUint(3);
  CIEC_UDINT nUdint(4);
  CIEC_ULINT nUlint(5);
  CIEC_SINT nSint(6);
  CIEC_INT nInt(7);
  CIEC_DINT nDint(8);
  CIEC_LINT nLint(9);
  CIEC_REAL nReal(10);
  CIEC_LREAL nLreal(11);
  CIEC_LWORD nLword(0x4026000000000000);

  CIEC_LREAL nResult(0);

  nResult = CIEC_ANY::cast<CIEC_LREAL>(nUsint);
  BOOST_TEST(nResult == 2);
  nResult = CIEC_ANY::cast<CIEC_LREAL>(nUint);
  BOOST_TEST(nResult == 3);
  nResult = CIEC_ANY::cast<CIEC_LREAL>(nUdint);
  BOOST_TEST(nResult == 4);
  nResult = CIEC_ANY::cast<CIEC_LREAL>(nUlint);
  BOOST_TEST(nResult == 5);
  nResult = CIEC_ANY::cast<CIEC_LREAL>(nSint);
  BOOST_TEST(nResult == 6);
  nResult = CIEC_ANY::cast<CIEC_LREAL>(nInt);
  BOOST_TEST(nResult == 7);
  nResult = CIEC_ANY::cast<CIEC_LREAL>(nDint);
  BOOST_TEST(nResult == 8);
  nResult = CIEC_ANY::cast<CIEC_LREAL>(nLint);
  BOOST_TEST(nResult == 9);
  nResult = CIEC_ANY::cast<CIEC_LREAL>(nReal);
  BOOST_TEST(nResult == 10);
  nResult = CIEC_ANY::cast<CIEC_LREAL>(nLreal);
  BOOST_TEST(nResult == 11);
  nResult = CIEC_ANY::cast<CIEC_LREAL>(nLword);
  BOOST_TEST(nResult == 11);
}

BOOST_AUTO_TEST_SUITE_END()
#endif //FORTE_USE_REAL_DATATYPE
#endif //FORTE_USE_64BIT_DATATYPES
