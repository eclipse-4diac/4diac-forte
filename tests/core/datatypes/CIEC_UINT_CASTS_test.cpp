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
#include "forte_boost_output_support.h"

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

#include <boost/test/tools/floating_point_comparison.hpp>
#include "../../../src/core/datatypes/forte_real.h"
#include "../../../src/core/datatypes/forte_lreal.h"

using namespace boost::unit_test;

BOOST_AUTO_TEST_SUITE(CIEC_UINT_casts_function_test)


BOOST_AUTO_TEST_CASE(CASTS_UINT_to_BOOL)
{
  CIEC_UINT nTestUInt0;
  CIEC_UINT nTestUInt1;
  CIEC_UINT nTestUInt256;
  CIEC_UINT nTestUInt65535;

  CIEC_BOOL bTestBool;

//check initial values
  BOOST_CHECK_EQUAL(static_cast<CIEC_UINT::TValueType>(nTestUInt0), 0);
  BOOST_CHECK_EQUAL(static_cast<CIEC_UINT::TValueType>(nTestUInt1), 0);
  BOOST_CHECK_EQUAL(static_cast<CIEC_UINT::TValueType>(nTestUInt256), 0);
  BOOST_CHECK_EQUAL(static_cast<CIEC_UINT::TValueType>(nTestUInt65535), 0);

  nTestUInt0 = CIEC_UINT(0);
  nTestUInt1 = CIEC_UINT(1);
  nTestUInt256 = CIEC_UINT(256);
  nTestUInt65535 = CIEC_UINT(65535);

  BOOST_CHECK_EQUAL(static_cast<CIEC_UINT::TValueType>(nTestUInt0), 0U);
  BOOST_CHECK_EQUAL(static_cast<CIEC_UINT::TValueType>(nTestUInt1), 1U);
  BOOST_CHECK_EQUAL(static_cast<CIEC_UINT::TValueType>(nTestUInt256), 256U);
  BOOST_CHECK_EQUAL(static_cast<CIEC_UINT::TValueType>(nTestUInt65535), 65535U);

  // checks
  bTestBool.setValue(nTestUInt0);
  BOOST_CHECK_EQUAL(false, bTestBool);
  bTestBool.setValue(nTestUInt1);
  BOOST_CHECK_EQUAL(true, bTestBool);
  bTestBool.setValue(nTestUInt256);
  BOOST_CHECK_EQUAL(true, bTestBool);
  bTestBool.setValue(nTestUInt65535);
  BOOST_CHECK_EQUAL(true, bTestBool);

}

BOOST_AUTO_TEST_CASE(CASTS_UINT_to_BITDTs)
{
  CIEC_UINT nTestUInt0;
  CIEC_UINT nTestUInt1;
  CIEC_UINT nTestUInt256;
  CIEC_UINT nTestUInt65535;

  CIEC_BYTE nTestByte;
  CIEC_WORD nTestWord;
  CIEC_DWORD nTestDWord;
  CIEC_LWORD nTestLWord;

//check initial values
  BOOST_CHECK_EQUAL(static_cast<CIEC_UINT::TValueType>(nTestUInt0), 0U);
  BOOST_CHECK_EQUAL(static_cast<CIEC_UINT::TValueType>(nTestUInt1), 0U);
  BOOST_CHECK_EQUAL(static_cast<CIEC_UINT::TValueType>(nTestUInt256), 0U);
  BOOST_CHECK_EQUAL(static_cast<CIEC_UINT::TValueType>(nTestUInt65535), 0U);

  nTestUInt0 = CIEC_UINT(0);
  nTestUInt1 = CIEC_UINT(1);
  nTestUInt256 = CIEC_UINT(256);
  nTestUInt65535 = CIEC_UINT(65535);

  BOOST_CHECK_EQUAL(static_cast<CIEC_UINT::TValueType>(nTestUInt0), 0U);
  BOOST_CHECK_EQUAL(static_cast<CIEC_UINT::TValueType>(nTestUInt1), 1U);
  BOOST_CHECK_EQUAL(static_cast<CIEC_UINT::TValueType>(nTestUInt256), 256U);
  BOOST_CHECK_EQUAL(static_cast<CIEC_UINT::TValueType>(nTestUInt65535), 65535U);

  // toByte
  nTestByte.setValue(nTestUInt0);
  BOOST_CHECK_EQUAL(nTestByte, 0);
  nTestByte.setValue(nTestUInt1);
  BOOST_CHECK_EQUAL(nTestByte, 1);
  nTestByte.setValue(nTestUInt256);
  BOOST_CHECK_EQUAL(nTestByte, 0);
  nTestByte.setValue(nTestUInt65535);
  BOOST_CHECK_EQUAL(nTestByte, 255);

//toWord
  nTestWord.setValue(nTestUInt0);
  BOOST_CHECK_EQUAL(nTestWord, 0);
  nTestWord.setValue(nTestUInt1);
  BOOST_CHECK_EQUAL(nTestWord, 1);
  nTestWord.setValue(nTestUInt256);
  BOOST_CHECK_EQUAL(nTestWord, 256);
  nTestWord.setValue(nTestUInt65535);
  BOOST_CHECK_EQUAL(nTestWord, 65535);

//toDWord
  nTestDWord.setValue(nTestUInt0);
  BOOST_CHECK_EQUAL(nTestDWord, 0UL);
  nTestDWord.setValue(nTestUInt1);
  BOOST_CHECK_EQUAL(nTestDWord, 1UL);
  nTestDWord.setValue(nTestUInt256);
  BOOST_CHECK_EQUAL(nTestDWord, 256UL);
  nTestDWord.setValue(nTestUInt65535);
  BOOST_CHECK_EQUAL(nTestDWord, 65535UL);

//toLWord
  nTestLWord.setValue(nTestUInt0);
  BOOST_CHECK_EQUAL(nTestLWord, 0ULL);
  nTestLWord.setValue(nTestUInt1);
  BOOST_CHECK_EQUAL(nTestLWord, 1ULL);
  nTestLWord.setValue(nTestUInt256);
  BOOST_CHECK_EQUAL(nTestLWord, 256ULL);
  nTestLWord.setValue(nTestUInt65535);
  BOOST_CHECK_EQUAL(nTestLWord, 65535ULL);
}

BOOST_AUTO_TEST_CASE(CASTS_UINT_to_INTS)
{
  CIEC_UINT nTestUInt0;
  CIEC_UINT nTestUInt1;
  CIEC_UINT nTestUInt256;
  CIEC_UINT nTestUInt65535;

  CIEC_SINT nTestSInt;
  CIEC_USINT nTestUSInt;
  CIEC_INT nTestInt;
  CIEC_DINT nTestDInt;
  CIEC_UDINT nTestUDInt;
  CIEC_LINT nTestLInt;
  CIEC_ULINT nTestULInt;

  //check initial values
  BOOST_CHECK_EQUAL(static_cast<CIEC_UINT::TValueType>(nTestUInt0), 0U);
  BOOST_CHECK_EQUAL(static_cast<CIEC_UINT::TValueType>(nTestUInt1), 0U);
  BOOST_CHECK_EQUAL(static_cast<CIEC_UINT::TValueType>(nTestUInt256), 0U);
  BOOST_CHECK_EQUAL(static_cast<CIEC_UINT::TValueType>(nTestUInt65535), 0U);

  nTestUInt0 = CIEC_UINT(0U);
  nTestUInt1 = CIEC_UINT(1U);
  nTestUInt256 = CIEC_UINT(256U);
  nTestUInt65535 = CIEC_UINT(65535U);

  BOOST_CHECK_EQUAL(static_cast<CIEC_UINT::TValueType>(nTestUInt0), 0U);
  BOOST_CHECK_EQUAL(static_cast<CIEC_UINT::TValueType>(nTestUInt1), 1U);
  BOOST_CHECK_EQUAL(static_cast<CIEC_UINT::TValueType>(nTestUInt256), 256U);
  BOOST_CHECK_EQUAL(static_cast<CIEC_UINT::TValueType>(nTestUInt65535), 65535U);

  // toSINT
  nTestSInt.setValue(nTestUInt0);
  BOOST_TEST(0 == static_cast<CIEC_SINT::TValueType>(nTestSInt));
  nTestSInt.setValue(nTestUInt1);
  BOOST_TEST(1 == static_cast<CIEC_SINT::TValueType>(nTestSInt));
  nTestSInt.setValue(nTestUInt256);
  BOOST_TEST(0 == static_cast<CIEC_SINT::TValueType>(nTestSInt));
  nTestSInt.setValue(nTestUInt65535);
  BOOST_TEST(-1 == static_cast<CIEC_SINT::TValueType>(nTestSInt));

  // toUSINT
  nTestUSInt.setValue(nTestUInt0);
  BOOST_CHECK_EQUAL(static_cast<CIEC_USINT::TValueType>(nTestUSInt), 0U);
  nTestUSInt.setValue(nTestUInt1);
  BOOST_CHECK_EQUAL(static_cast<CIEC_USINT::TValueType>(nTestUSInt), 1U);
  nTestUSInt.setValue(nTestUInt256);
  BOOST_CHECK_EQUAL(static_cast<CIEC_USINT::TValueType>(nTestUSInt), 0U);
  nTestUSInt.setValue(nTestUInt65535);
  BOOST_CHECK_EQUAL(static_cast<CIEC_USINT::TValueType>(nTestUSInt), 255U);

  // toINT
  nTestInt.setValue(nTestUInt0);
  BOOST_TEST(0 == static_cast<CIEC_INT::TValueType>(nTestInt));
  nTestInt.setValue(nTestUInt1);
  BOOST_TEST(1 == static_cast<CIEC_INT::TValueType>(nTestInt));
  nTestInt.setValue(nTestUInt256);
  BOOST_TEST(256 == static_cast<CIEC_INT::TValueType>(nTestInt));
  nTestInt.setValue(nTestUInt65535);
  BOOST_TEST(-1 == static_cast<CIEC_INT::TValueType>(nTestInt));

  // toDINT
  nTestDInt.setValue(nTestUInt0);
  BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(nTestDInt), 0);
  nTestDInt.setValue(nTestUInt1);
  BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(nTestDInt), 1);
  nTestDInt.setValue(nTestUInt256);
  BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(nTestDInt), 256);
  nTestDInt.setValue(nTestUInt65535);
  BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(nTestDInt), 65535);

  // toUDINT
  nTestUDInt.setValue(nTestUInt0);
  BOOST_CHECK_EQUAL(static_cast<CIEC_UDINT::TValueType>(nTestUDInt), 0UL);
  nTestUDInt.setValue(nTestUInt1);
  BOOST_CHECK_EQUAL(static_cast<CIEC_UDINT::TValueType>(nTestUDInt), 1UL);
  nTestUDInt.setValue(nTestUInt256);
  BOOST_CHECK_EQUAL(static_cast<CIEC_UDINT::TValueType>(nTestUDInt), 256UL);
  nTestUDInt.setValue(nTestUInt65535);
  BOOST_CHECK_EQUAL(static_cast<CIEC_UDINT::TValueType>(nTestUDInt), 65535UL);

  // toLINT
  nTestLInt.setValue(nTestUInt0);
  BOOST_CHECK_EQUAL(static_cast<CIEC_LINT::TValueType>(nTestLInt), 0LL);
  nTestLInt.setValue(nTestUInt1);
  BOOST_CHECK_EQUAL(static_cast<CIEC_LINT::TValueType>(nTestLInt), 1LL);
  nTestLInt.setValue(nTestUInt256);
  BOOST_CHECK_EQUAL(static_cast<CIEC_LINT::TValueType>(nTestLInt), 256LL);
  nTestLInt.setValue(nTestUInt65535);
  BOOST_CHECK_EQUAL(static_cast<CIEC_LINT::TValueType>(nTestLInt), 65535LL);

  // toULINT
  nTestULInt.setValue(nTestUInt0);
  BOOST_CHECK_EQUAL(static_cast<CIEC_ULINT::TValueType>(nTestULInt), 0ULL);
  nTestULInt.setValue(nTestUInt1);
  BOOST_CHECK_EQUAL(static_cast<CIEC_ULINT::TValueType>(nTestULInt), 1ULL);
  nTestULInt.setValue(nTestUInt256);
  BOOST_CHECK_EQUAL(static_cast<CIEC_ULINT::TValueType>(nTestULInt), 256ULL);
  nTestULInt.setValue(nTestUInt65535);
  BOOST_CHECK_EQUAL(static_cast<CIEC_ULINT::TValueType>(nTestULInt), 65535ULL);
}

BOOST_AUTO_TEST_CASE(CASTS_UINT_to_REAL)
{
  CIEC_UINT nTestUInt0;
  CIEC_UINT nTestUInt1;
  CIEC_UINT nTestUInt256;
  CIEC_UINT nTestUInt65535;

  CIEC_REAL nTestReal;

//check initial values
  BOOST_CHECK_EQUAL(static_cast<CIEC_UINT::TValueType>(nTestUInt0), 0U);
  BOOST_CHECK_EQUAL(static_cast<CIEC_UINT::TValueType>(nTestUInt1), 0U);
  BOOST_CHECK_EQUAL(static_cast<CIEC_UINT::TValueType>(nTestUInt256), 0U);
  BOOST_CHECK_EQUAL(static_cast<CIEC_UINT::TValueType>(nTestUInt65535), 0U);

  nTestUInt0 = CIEC_UINT(0U);
  nTestUInt1 = CIEC_UINT(1U);
  nTestUInt256 = CIEC_UINT(256U);
  nTestUInt65535 = CIEC_UINT(65535U);

  BOOST_CHECK_EQUAL(static_cast<CIEC_UINT::TValueType>(nTestUInt0), 0U);
  BOOST_CHECK_EQUAL(static_cast<CIEC_UINT::TValueType>(nTestUInt1), 1U);
  BOOST_CHECK_EQUAL(static_cast<CIEC_UINT::TValueType>(nTestUInt256), 256U);
  BOOST_CHECK_EQUAL(static_cast<CIEC_UINT::TValueType>(nTestUInt65535), 65535U);

  // toREAL
  nTestReal.setValue(nTestUInt0);
  BOOST_CHECK_EQUAL(0.0f, nTestReal);
  nTestReal.setValue(nTestUInt1);
  BOOST_CHECK_EQUAL(1.0f, nTestReal); 
  nTestReal.setValue(nTestUInt256);
  BOOST_CHECK_EQUAL(256.0f, nTestReal); 
  nTestReal.setValue(nTestUInt65535);
  BOOST_CHECK_EQUAL(65535.0f, nTestReal); 

  CIEC_LREAL nTestLReal;

//toLREAL
  nTestLReal.setValue(nTestUInt0);
  BOOST_CHECK_EQUAL(0.0, nTestLReal);
  nTestLReal.setValue(nTestUInt1);
  BOOST_CHECK_EQUAL(1.0, nTestLReal); 
  nTestLReal.setValue(nTestUInt256);
  BOOST_CHECK_EQUAL(256.0, nTestLReal); 
  nTestLReal.setValue(nTestUInt65535);
  BOOST_CHECK_EQUAL(65535.0, nTestLReal);
}

BOOST_AUTO_TEST_CASE(UINT_Castable_test)
{
  bool bUp, bDown;

  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_UINT, CIEC_ANY::e_BOOL, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(bDown);

  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_UINT, CIEC_ANY::e_SINT, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_UINT, CIEC_ANY::e_INT, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_UINT, CIEC_ANY::e_DINT, bUp, bDown)); BOOST_CHECK(bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_UINT, CIEC_ANY::e_LINT, bUp, bDown)); BOOST_CHECK(bUp); BOOST_CHECK(! bDown);

  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_UINT, CIEC_ANY::e_USINT, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_UINT, CIEC_ANY::e_UINT, bUp, bDown)); BOOST_CHECK(bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_UINT, CIEC_ANY::e_UDINT, bUp, bDown)); BOOST_CHECK(bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_UINT, CIEC_ANY::e_ULINT, bUp, bDown)); BOOST_CHECK(bUp); BOOST_CHECK(! bDown);

  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_UINT, CIEC_ANY::e_BYTE, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_UINT, CIEC_ANY::e_WORD, bUp, bDown)); BOOST_CHECK(bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_UINT, CIEC_ANY::e_DWORD, bUp, bDown)); BOOST_CHECK(bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_UINT, CIEC_ANY::e_LWORD, bUp, bDown)); BOOST_CHECK(bUp); BOOST_CHECK(! bDown);

  BOOST_CHECK(!CIEC_ANY::isCastable(CIEC_ANY::e_UINT, CIEC_ANY::e_DATE, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(!CIEC_ANY::isCastable(CIEC_ANY::e_UINT, CIEC_ANY::e_TIME_OF_DAY, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(!CIEC_ANY::isCastable(CIEC_ANY::e_UINT, CIEC_ANY::e_DATE_AND_TIME, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(!CIEC_ANY::isCastable(CIEC_ANY::e_UINT, CIEC_ANY::e_TIME, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(! bDown);

  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_UINT, CIEC_ANY::e_REAL, bUp, bDown)); BOOST_CHECK(bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_UINT, CIEC_ANY::e_LREAL, bUp, bDown)); BOOST_CHECK(bUp); BOOST_CHECK(! bDown);

  BOOST_CHECK(!CIEC_ANY::isCastable(CIEC_ANY::e_UINT, CIEC_ANY::e_STRING, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(!CIEC_ANY::isCastable(CIEC_ANY::e_UINT, CIEC_ANY::e_WSTRING, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(! bDown);
}

BOOST_AUTO_TEST_CASE(Explict_cast_operator_to_UINT)
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
  CIEC_BYTE nByte(12);
  CIEC_WORD nWord(13);
  CIEC_DWORD nDword(14);
  CIEC_LWORD nLword(15);
  CIEC_BOOL bBool(true);

  CIEC_UINT nResult(0);

  nResult = CIEC_ANY::cast<CIEC_UINT>(nUsint);
  BOOST_TEST(static_cast<CIEC_UINT::TValueType>(nResult) == 2);
  nResult = CIEC_ANY::cast<CIEC_UINT>(nUint);
  BOOST_TEST(static_cast<CIEC_UINT::TValueType>(nResult) == 3);
  nResult = CIEC_ANY::cast<CIEC_UINT>(nUdint);
  BOOST_TEST(static_cast<CIEC_UINT::TValueType>(nResult) == 4);
  nResult = CIEC_ANY::cast<CIEC_UINT>(nUlint);
  BOOST_TEST(static_cast<CIEC_UINT::TValueType>(nResult) == 5);
  nResult = CIEC_ANY::cast<CIEC_UINT>(nSint);
  BOOST_TEST(static_cast<CIEC_UINT::TValueType>(nResult) == 6);
  nResult = CIEC_ANY::cast<CIEC_UINT>(nInt);
  BOOST_TEST(static_cast<CIEC_UINT::TValueType>(nResult) == 7);
  nResult = CIEC_ANY::cast<CIEC_UINT>(nDint);
  BOOST_TEST(static_cast<CIEC_UINT::TValueType>(nResult) == 8);
  nResult = CIEC_ANY::cast<CIEC_UINT>(nLint);
  BOOST_TEST(static_cast<CIEC_UINT::TValueType>(nResult) == 9);
  nResult = CIEC_ANY::cast<CIEC_UINT>(nReal);
  BOOST_TEST(static_cast<CIEC_UINT::TValueType>(nResult) == 10);
  nResult = CIEC_ANY::cast<CIEC_UINT>(nLreal);
  BOOST_TEST(static_cast<CIEC_UINT::TValueType>(nResult) == 11);
  nResult = CIEC_ANY::cast<CIEC_UINT>(nByte);
  BOOST_TEST(static_cast<CIEC_UINT::TValueType>(nResult) == 12);
  nResult = CIEC_ANY::cast<CIEC_UINT>(nWord);
  BOOST_TEST(static_cast<CIEC_UINT::TValueType>(nResult) == 13);
  nResult = CIEC_ANY::cast<CIEC_UINT>(nDword);
  BOOST_TEST(static_cast<CIEC_UINT::TValueType>(nResult) == 14);
  nResult = CIEC_ANY::cast<CIEC_UINT>(nLword);
  BOOST_TEST(static_cast<CIEC_UINT::TValueType>(nResult) == 15);
  nResult = CIEC_ANY::cast<CIEC_UINT>(bBool);
  BOOST_TEST(static_cast<CIEC_UINT::TValueType>(nResult) == true);
}

BOOST_AUTO_TEST_SUITE_END()
