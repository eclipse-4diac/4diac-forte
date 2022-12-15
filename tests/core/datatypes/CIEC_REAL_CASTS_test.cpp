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
#include <boost/test/unit_test.hpp>
#include <boost/test/tools/floating_point_comparison.hpp>

#include "forte_boost_output_support.h"

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


#include "../../../src/core/datatypes/forte_lword.h"
#include "../../../src/core/datatypes/forte_lint.h"
#include "../../../src/core/datatypes/forte_ulint.h"
#include "../../../src/core/datatypes/forte_lreal.h"

using namespace boost::unit_test;

BOOST_AUTO_TEST_SUITE(CIEC_REAL_casts_function_test)


BOOST_AUTO_TEST_CASE(CASTS_REAL_to_BOOL)
{
  CIEC_REAL nTestReal0;
  CIEC_REAL nTestReal1;
  CIEC_REAL nTestReal2_2874e6;
  CIEC_REAL nTestRealm6_2587em4;
  CIEC_REAL nTestReal1_0em37;
  CIEC_REAL nTestReal36;

  CIEC_BOOL bTestBool;

//check initial values
  BOOST_CHECK_EQUAL(nTestReal0, 0.0f);
  BOOST_CHECK_EQUAL(nTestReal1, 0.0f);
  BOOST_CHECK_EQUAL(nTestReal2_2874e6, 0.0f);
  BOOST_CHECK_EQUAL(nTestRealm6_2587em4, 0.0f);
  BOOST_CHECK_EQUAL(nTestReal1_0em37, 0.0f);
  BOOST_CHECK_EQUAL(nTestReal36, 0.0f);

  nTestReal0 = CIEC_REAL(0.0f);
  nTestReal1 = CIEC_REAL(1.0f);
  nTestReal2_2874e6 = CIEC_REAL(2.2874e6f);
  nTestRealm6_2587em4 = CIEC_REAL(-6.2587e-4f);
  nTestReal1_0em37 = CIEC_REAL(1.0e-37f);
  nTestReal36 = CIEC_REAL(36.0f);


  BOOST_CHECK_EQUAL(0.0f, nTestReal0);
  BOOST_CHECK_EQUAL(1.0f, nTestReal1);
  BOOST_CHECK_EQUAL(2.2874e6f, nTestReal2_2874e6);
  BOOST_CHECK_EQUAL(-6.2587e-4f, nTestRealm6_2587em4);
  BOOST_CHECK_EQUAL(1.0e-37f, nTestReal1_0em37);
  BOOST_CHECK_EQUAL(36.0f, nTestReal36);


//checks
  CIEC_ANY::specialCast(nTestReal0, bTestBool);
  BOOST_CHECK_EQUAL(false, bTestBool);
  CIEC_ANY::specialCast(nTestReal1, bTestBool);
  BOOST_CHECK_EQUAL(true, bTestBool);
  CIEC_ANY::specialCast(nTestReal2_2874e6, bTestBool);
  BOOST_CHECK_EQUAL(true, bTestBool);
  CIEC_ANY::specialCast(nTestRealm6_2587em4, bTestBool);
  BOOST_CHECK_EQUAL(false, bTestBool);

}

BOOST_AUTO_TEST_CASE(CASTS_REAL_to_BITDTs)
{
  CIEC_BYTE nTestByte;
  CIEC_WORD nTestWord;
  CIEC_DWORD nTestDWord;
  CIEC_LWORD nTestLWord;

  CIEC_REAL nTestReal0;
  CIEC_REAL nTestReal1;
  CIEC_REAL nTestReal2_2874e6;
  CIEC_REAL nTestRealm6_2587em4;
  CIEC_REAL nTestReal1_0em37;
  CIEC_REAL nTestReal36;

//check initial values
  BOOST_CHECK_EQUAL(nTestReal0, 0.0f);
  BOOST_CHECK_EQUAL(nTestReal1, 0.0f);
  BOOST_CHECK_EQUAL(nTestReal2_2874e6, 0.0f);
  BOOST_CHECK_EQUAL(nTestRealm6_2587em4, 0.0f);
  BOOST_CHECK_EQUAL(nTestReal1_0em37, 0.0f);
  BOOST_CHECK_EQUAL(nTestReal36, 0.0f);

  nTestReal0 = CIEC_REAL(0.0f);
  nTestReal1 = CIEC_REAL(1.0f);
  nTestReal2_2874e6 = CIEC_REAL(2.2874e6f);
  nTestRealm6_2587em4 = CIEC_REAL(-6.2587e-4f);
  nTestReal1_0em37 = CIEC_REAL(1.0e-37f);
  nTestReal36 = CIEC_REAL(36.0f);

  BOOST_CHECK_EQUAL(0.0f, nTestReal0);
  BOOST_CHECK_EQUAL(1.0f, nTestReal1);
  BOOST_CHECK_EQUAL(2.2874e6f, nTestReal2_2874e6);
  BOOST_CHECK_EQUAL(-6.2587e-4f, nTestRealm6_2587em4);
  BOOST_CHECK_EQUAL(1.0e-37f, nTestReal1_0em37);
  BOOST_CHECK_EQUAL(36.0f, nTestReal36);

  //the 4byte hex value for 36.0:        0x42100000
  //the 4byte hex value for -6.2587e-4f: 0xba24116d
  //the 4byte hex value for 1:           0x3f800000

//toByte
  CIEC_ANY::specialCast(nTestReal0, nTestByte);
  BOOST_CHECK_EQUAL(nTestByte, 0U);
  CIEC_ANY::specialCast(nTestReal1, nTestByte);
  BOOST_CHECK_EQUAL(nTestByte, 0x00);
  CIEC_ANY::specialCast(nTestReal36, nTestByte);
  BOOST_CHECK_EQUAL(nTestByte, 0x00);
  CIEC_ANY::specialCast(nTestRealm6_2587em4, nTestByte);
  BOOST_CHECK_EQUAL(nTestByte, 0x6d);

//toWord
  CIEC_ANY::specialCast(nTestReal0, nTestWord);
  BOOST_CHECK_EQUAL(nTestWord, 0U);
  CIEC_ANY::specialCast(nTestReal1, nTestWord);
  BOOST_CHECK_EQUAL(nTestWord, 0x00);
  CIEC_ANY::specialCast(nTestReal36, nTestWord);
  BOOST_CHECK_EQUAL(nTestWord, 0x0000);
  CIEC_ANY::specialCast(nTestRealm6_2587em4, nTestWord);
  BOOST_CHECK_EQUAL(nTestWord, 0x116d);

//toDWord
  CIEC_ANY::specialCast(nTestReal0, nTestDWord);
  BOOST_CHECK_EQUAL(nTestDWord, 0UL);
  CIEC_ANY::specialCast(nTestReal1, nTestDWord);
  BOOST_CHECK_EQUAL(nTestDWord, 0x3f800000);
  CIEC_ANY::specialCast(nTestReal36, nTestDWord);
  BOOST_CHECK_EQUAL(nTestDWord, 0x42100000);
  CIEC_ANY::specialCast(nTestRealm6_2587em4, nTestDWord);
  BOOST_CHECK_EQUAL(nTestDWord, 0xba24116d);

//toLWord
  CIEC_ANY::specialCast(nTestReal0, nTestLWord);
  BOOST_CHECK_EQUAL(nTestLWord, 0ULL);
  CIEC_ANY::specialCast(nTestReal1, nTestLWord);
  BOOST_CHECK_EQUAL(nTestLWord, 0x3f800000);
  CIEC_ANY::specialCast(nTestReal36, nTestLWord);
  BOOST_CHECK_EQUAL(nTestLWord, 0x42100000);
  CIEC_ANY::specialCast(nTestRealm6_2587em4, nTestLWord);
  BOOST_CHECK_EQUAL(nTestLWord, 0xba24116d);
}

BOOST_AUTO_TEST_CASE(CASTS_REAL_to_INTS)
{
  CIEC_USINT nTestUSInt;
  CIEC_INT nTestInt;
  CIEC_UINT nTestUInt;
  CIEC_DINT nTestDInt;
  CIEC_UDINT nTestUDInt;
  CIEC_LINT nTestLInt;
  CIEC_ULINT nTestULInt;

  CIEC_REAL nTestReal0;
  CIEC_REAL nTestReal1;
  CIEC_REAL nTestReal2_2874e6;
  CIEC_REAL nTestRealm6_2587em4;
  CIEC_REAL nTestReal1_0em37;
  CIEC_REAL nTestReal36;

//check initial values
  BOOST_CHECK_EQUAL(nTestReal0, 0.0f);
  BOOST_CHECK_EQUAL(nTestReal1, 0.0f);
  BOOST_CHECK_EQUAL(nTestReal2_2874e6, 0.0f);
  BOOST_CHECK_EQUAL(nTestRealm6_2587em4, 0.0f);
  BOOST_CHECK_EQUAL(nTestReal1_0em37, 0.0f);
  BOOST_CHECK_EQUAL(nTestReal36, 0.0f);

  nTestReal0 = CIEC_REAL(0.0f);
  nTestReal1 = CIEC_REAL(1.0f);
  nTestReal2_2874e6 = CIEC_REAL(2.2874e6f);
  nTestRealm6_2587em4 = CIEC_REAL(-6.2587e-4f);
  nTestReal1_0em37 = CIEC_REAL(1.0e-37f);
  nTestReal36 = CIEC_REAL(36.0f);

  BOOST_CHECK_EQUAL(0.0f, nTestReal0);
  BOOST_CHECK_EQUAL(1.0f, nTestReal1);
  BOOST_CHECK_EQUAL(2.2874e6f, nTestReal2_2874e6);
  BOOST_CHECK_EQUAL(-6.2587e-4f, nTestRealm6_2587em4);
  BOOST_CHECK_EQUAL(1.0e-37f, nTestReal1_0em37);
  BOOST_CHECK_EQUAL(36.0f, nTestReal36);

//toUSINT
  CIEC_ANY::specialCast(nTestReal0, nTestUSInt);
  BOOST_CHECK_EQUAL(static_cast<CIEC_USINT::TValueType>(nTestUSInt), 0U);
  CIEC_ANY::specialCast(nTestReal1, nTestUSInt);
  BOOST_CHECK_EQUAL(static_cast<CIEC_USINT::TValueType>(nTestUSInt), 1U);
  CIEC_ANY::specialCast(nTestReal36, nTestUSInt);
  BOOST_CHECK_EQUAL(static_cast<CIEC_USINT::TValueType>(nTestUSInt), 36U);

//toINT
  CIEC_ANY::specialCast(nTestReal0, nTestInt);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(nTestInt), 0);
  CIEC_ANY::specialCast(nTestReal1, nTestInt);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(nTestInt), 1);
  CIEC_ANY::specialCast(nTestReal36, nTestInt);
  BOOST_CHECK_EQUAL(static_cast<CIEC_INT::TValueType>(nTestInt), 36);

// toUINT
  CIEC_ANY::specialCast(nTestReal0, nTestUInt);
  BOOST_CHECK_EQUAL(static_cast<CIEC_UINT::TValueType>(nTestUInt), 0U);
  CIEC_ANY::specialCast(nTestReal1, nTestUInt);
  BOOST_CHECK_EQUAL(static_cast<CIEC_UINT::TValueType>(nTestUInt), 1U);
  CIEC_ANY::specialCast(nTestReal36, nTestUInt);
  BOOST_CHECK_EQUAL(static_cast<CIEC_UINT::TValueType>(nTestUInt), 36U);

// toDINT
  CIEC_ANY::specialCast(nTestReal0, nTestDInt);
  BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(nTestDInt), 0L);
  CIEC_ANY::specialCast(nTestReal1, nTestDInt);
  BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(nTestDInt), 1L);
  CIEC_ANY::specialCast(nTestReal36, nTestDInt);
  BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(nTestDInt), 36L);

//toUDINT
  CIEC_ANY::specialCast(nTestReal0, nTestUDInt);
  BOOST_CHECK_EQUAL(static_cast<CIEC_UDINT::TValueType>(nTestUDInt), 0UL);
  CIEC_ANY::specialCast(nTestReal1, nTestUDInt);
  BOOST_CHECK_EQUAL(static_cast<CIEC_UDINT::TValueType>(nTestUDInt), 1UL);
  CIEC_ANY::specialCast(nTestReal36, nTestUDInt);
  BOOST_CHECK_EQUAL(static_cast<CIEC_UDINT::TValueType>(nTestUDInt), 36UL);

//toLINT
  CIEC_ANY::specialCast(nTestReal0, nTestLInt);
  BOOST_CHECK_EQUAL(static_cast<CIEC_LINT::TValueType>(nTestLInt), 0LL);
  CIEC_ANY::specialCast(nTestReal1, nTestLInt);
  BOOST_CHECK_EQUAL(static_cast<CIEC_LINT::TValueType>(nTestLInt), 1LL);
  CIEC_ANY::specialCast(nTestReal36, nTestLInt);
  BOOST_CHECK_EQUAL(static_cast<CIEC_LINT::TValueType>(nTestLInt), 36LL);

// toULINT
  CIEC_ANY::specialCast(nTestReal0, nTestULInt);
  BOOST_CHECK_EQUAL(static_cast<CIEC_ULINT::TValueType>(nTestULInt), 0ULL);
  CIEC_ANY::specialCast(nTestReal1, nTestULInt);
  BOOST_CHECK_EQUAL(static_cast<CIEC_ULINT::TValueType>(nTestULInt), 1ULL);
  CIEC_ANY::specialCast(nTestReal36, nTestULInt);
  BOOST_CHECK_EQUAL(static_cast<CIEC_ULINT::TValueType>(nTestULInt), 36ULL);
}

BOOST_AUTO_TEST_CASE(CASTS_REAL_to_LREAL)
{
  CIEC_REAL nTestReal0;
  CIEC_REAL nTestReal1;
  CIEC_REAL nTestReal2_2874e6;
  CIEC_REAL nTestRealm6_2587em4;
  CIEC_REAL nTestReal1_0em37;
  CIEC_REAL nTestReal36;

//check initial values
  BOOST_CHECK_EQUAL(nTestReal0, 0.0f);
  BOOST_CHECK_EQUAL(nTestReal1, 0.0f);
  BOOST_CHECK_EQUAL(nTestReal2_2874e6, 0.0f);
  BOOST_CHECK_EQUAL(nTestRealm6_2587em4, 0.0f);
  BOOST_CHECK_EQUAL(nTestReal1_0em37, 0.0f);
  BOOST_CHECK_EQUAL(nTestReal36, 0.0f);

  nTestReal0 = CIEC_REAL(0.0f);
  nTestReal1 = CIEC_REAL(1.0f);
  nTestReal2_2874e6 = CIEC_REAL(2.2874e6f);
  nTestRealm6_2587em4 = CIEC_REAL(-6.2587e-4f);
  nTestReal1_0em37 = CIEC_REAL(1.0e-37f);
  nTestReal36 = CIEC_REAL(36.0f);

  BOOST_CHECK_EQUAL(0.0f, nTestReal0);
  BOOST_CHECK_EQUAL(1.0f, nTestReal1);
  BOOST_CHECK_EQUAL(2.2874e6f, nTestReal2_2874e6);
  BOOST_CHECK_EQUAL(-6.2587e-4f, nTestRealm6_2587em4);
  BOOST_CHECK_EQUAL(1.0e-37f, nTestReal1_0em37);
  BOOST_CHECK_EQUAL(36.0f, nTestReal36);

  CIEC_LREAL nTestLReal;

//toLREAL
  BOOST_CHECK_EQUAL(1.0f, nTestReal1);
  BOOST_CHECK_EQUAL(2.2874e6f, nTestReal2_2874e6);
  BOOST_CHECK_EQUAL(-6.2587e-4f, nTestRealm6_2587em4);
  BOOST_CHECK_EQUAL(1.0e-37f, nTestReal1_0em37);
  BOOST_CHECK_EQUAL(36.0f, nTestReal36);

  CIEC_ANY::specialCast(nTestReal0, nTestLReal);
  BOOST_CHECK_EQUAL((double)0.0f, nTestLReal);
  CIEC_ANY::specialCast(nTestReal1, nTestLReal);
  BOOST_CHECK_EQUAL((double)1.0f, nTestLReal);
  nTestLReal.setValue(nTestReal2_2874e6);
  CIEC_ANY::specialCast(nTestReal2_2874e6, nTestLReal);
  BOOST_CHECK_EQUAL(((double)2.2874e6f), nTestLReal);
  CIEC_ANY::specialCast(nTestRealm6_2587em4, nTestLReal);
  BOOST_CHECK_EQUAL(((double)-6.2587e-4f), nTestLReal);
  nTestLReal.setValue(nTestReal1_0em37);
  CIEC_ANY::specialCast(nTestReal1_0em37, nTestLReal);
  BOOST_CHECK_EQUAL(((double)1.0e-37f), nTestLReal);
  CIEC_ANY::specialCast(nTestReal36, nTestLReal);
  BOOST_CHECK_EQUAL(((double)36.0f), nTestLReal);
}

BOOST_AUTO_TEST_CASE(REAL_Castable_test)
{
  bool bUp, bDown;

  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_REAL, CIEC_ANY::e_BOOL, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(bDown);

  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_REAL, CIEC_ANY::e_SINT, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_REAL, CIEC_ANY::e_INT, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_REAL, CIEC_ANY::e_DINT, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_REAL, CIEC_ANY::e_LINT, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(bDown);

  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_REAL, CIEC_ANY::e_USINT, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_REAL, CIEC_ANY::e_UINT, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_REAL, CIEC_ANY::e_UDINT, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_REAL, CIEC_ANY::e_ULINT, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(bDown);

  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_REAL, CIEC_ANY::e_BYTE, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_REAL, CIEC_ANY::e_WORD, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_REAL, CIEC_ANY::e_DWORD, bUp, bDown)); BOOST_CHECK(bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_REAL, CIEC_ANY::e_LWORD, bUp, bDown)); BOOST_CHECK(bUp); BOOST_CHECK(! bDown);

  BOOST_CHECK(!CIEC_ANY::isCastable(CIEC_ANY::e_REAL, CIEC_ANY::e_DATE, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(!CIEC_ANY::isCastable(CIEC_ANY::e_REAL, CIEC_ANY::e_TIME_OF_DAY, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(!CIEC_ANY::isCastable(CIEC_ANY::e_REAL, CIEC_ANY::e_DATE_AND_TIME, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(!CIEC_ANY::isCastable(CIEC_ANY::e_REAL, CIEC_ANY::e_TIME, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(! bDown);

  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_REAL, CIEC_ANY::e_REAL, bUp, bDown)); BOOST_CHECK(bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_REAL, CIEC_ANY::e_LREAL, bUp, bDown)); BOOST_CHECK(bUp); BOOST_CHECK(! bDown);

  BOOST_CHECK(!CIEC_ANY::isCastable(CIEC_ANY::e_REAL, CIEC_ANY::e_STRING, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(!CIEC_ANY::isCastable(CIEC_ANY::e_REAL, CIEC_ANY::e_WSTRING, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(! bDown);
}

BOOST_AUTO_TEST_CASE(Explict_cast_operator_to_REAL)
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
  CIEC_DWORD nDword(0x41200000);

  CIEC_REAL nResult(0);

  nResult = CIEC_ANY::cast<CIEC_REAL>(nUsint);
  BOOST_TEST(nResult == 2);
  nResult = CIEC_ANY::cast<CIEC_REAL>(nUint);
  BOOST_TEST(nResult == 3);
  nResult = CIEC_ANY::cast<CIEC_REAL>(nUdint);
  BOOST_TEST(nResult == 4);
  nResult = CIEC_ANY::cast<CIEC_REAL>(nUlint);
  BOOST_TEST(nResult == 5);
  nResult = CIEC_ANY::cast<CIEC_REAL>(nSint);
  BOOST_TEST(nResult == 6);
  nResult = CIEC_ANY::cast<CIEC_REAL>(nInt);
  BOOST_TEST(nResult == 7);
  nResult = CIEC_ANY::cast<CIEC_REAL>(nDint);
  BOOST_TEST(nResult == 8);
  nResult = CIEC_ANY::cast<CIEC_REAL>(nLint);
  BOOST_TEST(nResult == 9);
  nResult = CIEC_ANY::cast<CIEC_REAL>(nReal);
  BOOST_TEST(nResult == 10);
  nResult = CIEC_ANY::cast<CIEC_REAL>(nLreal);
  BOOST_TEST(nResult == 11);
  nResult = CIEC_ANY::cast<CIEC_REAL>(nDword);
  BOOST_TEST(nResult == 10);
}

BOOST_AUTO_TEST_CASE(Ctor_cast_to_REAL)
{
  CIEC_REAL realfromSint(CIEC_SINT(-1));
  CIEC_REAL realfromInt(CIEC_INT(-2));
  CIEC_REAL realfromUsint(CIEC_USINT(1));
  CIEC_REAL realfromUint(CIEC_UINT(2));

  BOOST_TEST(static_cast<CIEC_SINT::TValueType>(realfromSint) == -1);
  BOOST_TEST(static_cast<CIEC_INT::TValueType>(realfromInt) == -2);
  BOOST_TEST(static_cast<CIEC_USINT::TValueType>(realfromUsint) == 1);
  BOOST_TEST(static_cast<CIEC_UINT::TValueType>(realfromUint) == 2);
}

BOOST_AUTO_TEST_SUITE_END()

