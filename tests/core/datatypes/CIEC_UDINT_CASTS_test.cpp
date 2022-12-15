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
//Time
#include "../../../src/core/datatypes/forte_time.h"

#include "../../../src/core/datatypes/forte_lword.h"
#include "../../../src/core/datatypes/forte_lint.h"
#include "../../../src/core/datatypes/forte_ulint.h"

#include <boost/test/tools/floating_point_comparison.hpp>
#include "../../../src/core/datatypes/forte_real.h"
#include "../../../src/core/datatypes/forte_lreal.h"

using namespace boost::unit_test;

BOOST_AUTO_TEST_SUITE(CIEC_UDINT_casts_function_test)


BOOST_AUTO_TEST_CASE(CASTS_UDINT_to_BOOL)
{
  CIEC_UDINT nTestUDInt0;
  CIEC_UDINT nTestUDInt1;
  CIEC_UDINT nTestUDInt65536;
  CIEC_UDINT nTestUDInt4294967295;

  CIEC_BOOL bTestBool;

//check initial values
  BOOST_CHECK_EQUAL(static_cast<CIEC_UDINT::TValueType>(nTestUDInt0), 0UL);
  BOOST_CHECK_EQUAL(static_cast<CIEC_UDINT::TValueType>(nTestUDInt1), 0UL);
  BOOST_CHECK_EQUAL(static_cast<CIEC_UDINT::TValueType>(nTestUDInt65536), 0UL);
  BOOST_CHECK_EQUAL(static_cast<CIEC_UDINT::TValueType>(nTestUDInt4294967295), 0UL);

  nTestUDInt0 = CIEC_UDINT(0UL);
  nTestUDInt1 = CIEC_UDINT(1UL);
  nTestUDInt65536 = CIEC_UDINT(65536UL);
  nTestUDInt4294967295 = CIEC_UDINT(4294967295UL);

  BOOST_CHECK_EQUAL(static_cast<CIEC_UDINT::TValueType>(nTestUDInt0), 0UL);
  BOOST_CHECK_EQUAL(static_cast<CIEC_UDINT::TValueType>(nTestUDInt1), 1UL);
  BOOST_CHECK_EQUAL(static_cast<CIEC_UDINT::TValueType>(nTestUDInt65536), 65536UL);
  BOOST_CHECK_EQUAL(static_cast<CIEC_UDINT::TValueType>(nTestUDInt4294967295), 4294967295UL);

//checks
  bTestBool.setValue(nTestUDInt0);
  BOOST_CHECK_EQUAL(false, bTestBool);
  bTestBool.setValue(nTestUDInt1);
  BOOST_CHECK_EQUAL(true, bTestBool);
  bTestBool.setValue(nTestUDInt65536);
  BOOST_CHECK_EQUAL(true, bTestBool);
  bTestBool.setValue(nTestUDInt4294967295);
  BOOST_CHECK_EQUAL(true, bTestBool);

}

BOOST_AUTO_TEST_CASE(CASTS_UDINT_to_BITDTs)
{
  CIEC_UDINT nTestUDInt0;
  CIEC_UDINT nTestUDInt1;
  CIEC_UDINT nTestUDInt65536;
  CIEC_UDINT nTestUDInt4294967295;

  CIEC_BYTE nTestByte;
  CIEC_WORD nTestWord;
  CIEC_DWORD nTestDWord;
  CIEC_LWORD nTestLWord;

//check initial values
  BOOST_CHECK_EQUAL(static_cast<CIEC_UDINT::TValueType>(nTestUDInt0), 0UL);
  BOOST_CHECK_EQUAL(static_cast<CIEC_UDINT::TValueType>(nTestUDInt1), 0UL);
  BOOST_CHECK_EQUAL(static_cast<CIEC_UDINT::TValueType>(nTestUDInt65536), 0UL);
  BOOST_CHECK_EQUAL(static_cast<CIEC_UDINT::TValueType>(nTestUDInt4294967295), 0UL);

  nTestUDInt0 = CIEC_UDINT(0UL);
  nTestUDInt1 = CIEC_UDINT(1UL);
  nTestUDInt65536 = CIEC_UDINT(65536UL);
  nTestUDInt4294967295 = CIEC_UDINT(4294967295UL);

  BOOST_CHECK_EQUAL(static_cast<CIEC_UDINT::TValueType>(nTestUDInt0), 0UL);
  BOOST_CHECK_EQUAL(static_cast<CIEC_UDINT::TValueType>(nTestUDInt1), 1UL);
  BOOST_CHECK_EQUAL(static_cast<CIEC_UDINT::TValueType>(nTestUDInt65536), 65536UL);
  BOOST_CHECK_EQUAL(static_cast<CIEC_UDINT::TValueType>(nTestUDInt4294967295), 4294967295UL);

//toByte
  nTestByte.setValue(nTestUDInt0);
  BOOST_CHECK_EQUAL(nTestByte, 0U);
  nTestByte.setValue(nTestUDInt1);
  BOOST_CHECK_EQUAL(nTestByte, 1U);
  nTestByte.setValue(nTestUDInt65536);
  BOOST_CHECK_EQUAL(nTestByte, 0U);
  nTestByte.setValue(nTestUDInt4294967295);
  BOOST_CHECK_EQUAL(nTestByte, 255U);

//toWord
  nTestWord.setValue(nTestUDInt0);
  BOOST_CHECK_EQUAL(nTestWord, 0U);
  nTestWord.setValue(nTestUDInt1);
  BOOST_CHECK_EQUAL(nTestWord, 1U);
  nTestWord.setValue(nTestUDInt65536);
  BOOST_CHECK_EQUAL(nTestWord, 0U);
  nTestWord.setValue(nTestUDInt4294967295);
  BOOST_CHECK_EQUAL(nTestWord, 65535U);

//toDWord
  nTestDWord.setValue(nTestUDInt0);
  BOOST_CHECK_EQUAL(nTestDWord, 0UL);
  nTestDWord.setValue(nTestUDInt1);
  BOOST_CHECK_EQUAL(nTestDWord, 1UL);
  nTestDWord.setValue(nTestUDInt65536);
  BOOST_CHECK_EQUAL(nTestDWord, 65536UL);
  nTestDWord.setValue(nTestUDInt4294967295);
  BOOST_CHECK_EQUAL(nTestDWord, 4294967295UL);

//toLWord
  nTestLWord.setValue(nTestUDInt0);
  BOOST_CHECK_EQUAL(nTestLWord, 0ULL);
  nTestLWord.setValue(nTestUDInt1);
  BOOST_CHECK_EQUAL(nTestLWord, 1ULL);
  nTestLWord.setValue(nTestUDInt65536);
  BOOST_CHECK_EQUAL(nTestLWord, 65536ULL);
  nTestLWord.setValue(nTestUDInt4294967295);
  BOOST_CHECK_EQUAL(nTestLWord, 4294967295ULL);
}

BOOST_AUTO_TEST_CASE(CASTS_UDINT_to_INTS)
{
  CIEC_UDINT nTestUDInt0;
  CIEC_UDINT nTestUDInt1;
  CIEC_UDINT nTestUDInt65536;
  CIEC_UDINT nTestUDInt4294967295;

  CIEC_SINT nTestSInt;
  CIEC_USINT nTestUSInt;
  CIEC_INT nTestInt;
  CIEC_UINT nTestUInt;
  CIEC_DINT nTestDInt;
  CIEC_LINT nTestLInt;
  CIEC_ULINT nTestULInt;

  //check initial values
  BOOST_CHECK_EQUAL(static_cast<CIEC_UDINT::TValueType>(nTestUDInt0), 0UL);
  BOOST_CHECK_EQUAL(static_cast<CIEC_UDINT::TValueType>(nTestUDInt1), 0UL);
  BOOST_CHECK_EQUAL(static_cast<CIEC_UDINT::TValueType>(nTestUDInt65536), 0UL);
  BOOST_CHECK_EQUAL(static_cast<CIEC_UDINT::TValueType>(nTestUDInt4294967295), 0UL);

  nTestUDInt0 = CIEC_UDINT(0UL);
  nTestUDInt1 = CIEC_UDINT(1UL);
  nTestUDInt65536 = CIEC_UDINT(65536UL);
  nTestUDInt4294967295 = CIEC_UDINT(4294967295UL);

  BOOST_CHECK_EQUAL(static_cast<CIEC_UDINT::TValueType>(nTestUDInt0), 0UL);
  BOOST_CHECK_EQUAL(static_cast<CIEC_UDINT::TValueType>(nTestUDInt1), 1UL);
  BOOST_CHECK_EQUAL(static_cast<CIEC_UDINT::TValueType>(nTestUDInt65536), 65536UL);
  BOOST_CHECK_EQUAL(static_cast<CIEC_UDINT::TValueType>(nTestUDInt4294967295), 4294967295UL);

//toSINT
  nTestSInt.setValue(nTestUDInt0);
  BOOST_TEST(0 == static_cast<CIEC_SINT::TValueType>(nTestSInt));
  nTestSInt.setValue(nTestUDInt1);
  BOOST_TEST(1 == static_cast<CIEC_SINT::TValueType>(nTestSInt));
  nTestSInt.setValue(nTestUDInt65536);
  BOOST_TEST(0 == static_cast<CIEC_SINT::TValueType>(nTestSInt));
  nTestSInt.setValue(nTestUDInt4294967295);
  BOOST_TEST(-1 == static_cast<CIEC_SINT::TValueType>(nTestSInt));

//toUSINT
  nTestUSInt.setValue(nTestUDInt0);
  BOOST_TEST(0U == static_cast<CIEC_USINT::TValueType>(nTestUSInt));
  nTestUSInt.setValue(nTestUDInt1);
  BOOST_TEST(1U == static_cast<CIEC_USINT::TValueType>(nTestUSInt));
  nTestUSInt.setValue(nTestUDInt65536);
  BOOST_TEST(0U == static_cast<CIEC_USINT::TValueType>(nTestUSInt));
  nTestUSInt.setValue(nTestUDInt4294967295);
  BOOST_TEST(255U == static_cast<CIEC_USINT::TValueType>(nTestUSInt));

//toINT
  nTestInt.setValue(nTestUDInt0);
  BOOST_TEST(0 == static_cast<CIEC_INT::TValueType>(nTestInt));
  nTestInt.setValue(nTestUDInt1);
  BOOST_TEST(1 == static_cast<CIEC_INT::TValueType>(nTestInt));
  nTestInt.setValue(nTestUDInt65536);
  BOOST_TEST(0 == static_cast<CIEC_INT::TValueType>(nTestInt));
  nTestInt.setValue(nTestUDInt4294967295);
  BOOST_TEST(-1 == static_cast<CIEC_INT::TValueType>(nTestInt));

//toUINT
  nTestUInt.setValue(nTestUDInt0);
  BOOST_CHECK_EQUAL(static_cast<CIEC_UINT::TValueType>(nTestUInt), 0U);
  nTestUInt.setValue(nTestUDInt1);
  BOOST_CHECK_EQUAL(static_cast<CIEC_UINT::TValueType>(nTestUInt), 1U);
  nTestUInt.setValue(nTestUDInt65536);
  BOOST_CHECK_EQUAL(static_cast<CIEC_UINT::TValueType>(nTestUInt), 0U);
  nTestUInt.setValue(nTestUDInt4294967295);
  BOOST_CHECK_EQUAL(static_cast<CIEC_UINT::TValueType>(nTestUInt), 65535U);

  // toDINT
  nTestDInt.setValue(nTestUDInt0);
  BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(nTestDInt), 0L);
  nTestDInt.setValue(nTestUDInt1);
  BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(nTestDInt), 1L);
  nTestDInt.setValue(nTestUDInt65536);
  BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(nTestDInt), 65536L);
  nTestDInt.setValue(nTestUDInt4294967295);
  BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(nTestDInt), -1L);

//toLINT
  nTestLInt.setValue(nTestUDInt0);
  BOOST_CHECK_EQUAL(static_cast<CIEC_LINT::TValueType>(nTestLInt), 0LL);
  nTestLInt.setValue(nTestUDInt1);
  BOOST_CHECK_EQUAL(static_cast<CIEC_LINT::TValueType>(nTestLInt), 1LL);
  nTestLInt.setValue(nTestUDInt65536);
  BOOST_CHECK_EQUAL(static_cast<CIEC_LINT::TValueType>(nTestLInt), 65536LL);
  nTestLInt.setValue(nTestUDInt4294967295);
  BOOST_CHECK_EQUAL(static_cast<CIEC_LINT::TValueType>(nTestLInt), 4294967295LL);

  // toULINT
  nTestULInt.setValue(nTestUDInt0);
  BOOST_CHECK_EQUAL(static_cast<CIEC_ULINT::TValueType>(nTestULInt), 0ULL);
  nTestULInt.setValue(nTestUDInt1);
  BOOST_CHECK_EQUAL(static_cast<CIEC_ULINT::TValueType>(nTestULInt), 1ULL);
  nTestULInt.setValue(nTestUDInt65536);
  BOOST_CHECK_EQUAL(static_cast<CIEC_ULINT::TValueType>(nTestULInt), 65536ULL);
  nTestULInt.setValue(nTestUDInt4294967295);
  BOOST_CHECK_EQUAL(static_cast<CIEC_ULINT::TValueType>(nTestULInt), 4294967295ULL);
}

BOOST_AUTO_TEST_CASE(CASTS_UDINT_to_REAL)
{
  CIEC_UDINT nTestUDInt0;
  CIEC_UDINT nTestUDInt1;
  CIEC_UDINT nTestUDInt65536;
  CIEC_UDINT nTestUDInt4294967295;

  CIEC_REAL nTestReal;

//check initial values
  BOOST_CHECK_EQUAL(static_cast<CIEC_UDINT::TValueType>(nTestUDInt0), 0UL);
  BOOST_CHECK_EQUAL(static_cast<CIEC_UDINT::TValueType>(nTestUDInt1), 0UL);
  BOOST_CHECK_EQUAL(static_cast<CIEC_UDINT::TValueType>(nTestUDInt65536), 0UL);
  BOOST_CHECK_EQUAL(static_cast<CIEC_UDINT::TValueType>(nTestUDInt4294967295), 0UL);

  nTestUDInt0 = CIEC_UDINT(0UL);
  nTestUDInt1 = CIEC_UDINT(1UL);
  nTestUDInt65536 = CIEC_UDINT(65536UL);
  nTestUDInt4294967295 = CIEC_UDINT(4294967295UL);

  BOOST_CHECK_EQUAL(static_cast<CIEC_UDINT::TValueType>(nTestUDInt0), 0UL);
  BOOST_CHECK_EQUAL(static_cast<CIEC_UDINT::TValueType>(nTestUDInt1), 1UL);
  BOOST_CHECK_EQUAL(static_cast<CIEC_UDINT::TValueType>(nTestUDInt65536), 65536UL);
  BOOST_CHECK_EQUAL(static_cast<CIEC_UDINT::TValueType>(nTestUDInt4294967295), 4294967295UL);

//toREAL
  nTestReal.setValue(nTestUDInt0);
  BOOST_CHECK_EQUAL(0.0f, nTestReal);
  nTestReal.setValue(nTestUDInt1);
  BOOST_CHECK_EQUAL(1.0f, nTestReal); 
  nTestReal.setValue(nTestUDInt65536);
  BOOST_CHECK_EQUAL(65536.0f, nTestReal); 
  nTestReal.setValue(nTestUDInt4294967295);
  BOOST_CHECK_EQUAL(4294967295.0f, nTestReal); 

  CIEC_LREAL nTestLReal;

//toLREAL
  nTestLReal.setValue(nTestUDInt0);
  BOOST_CHECK_EQUAL(0.0, nTestLReal);
  nTestLReal.setValue(nTestUDInt1);
  BOOST_CHECK_EQUAL(1.0, nTestLReal); 
  nTestLReal.setValue(nTestUDInt65536);
  BOOST_CHECK_EQUAL(65536.0, nTestLReal); 
  nTestLReal.setValue(nTestUDInt4294967295);
  BOOST_CHECK_EQUAL(4294967295.0,nTestLReal); 
}

BOOST_AUTO_TEST_CASE(UDINT_Castable_test)
{
  bool bUp, bDown;

  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_UDINT, CIEC_ANY::e_BOOL, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(bDown);

  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_UDINT, CIEC_ANY::e_SINT, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_UDINT, CIEC_ANY::e_INT, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_UDINT, CIEC_ANY::e_DINT, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_UDINT, CIEC_ANY::e_LINT, bUp, bDown)); BOOST_CHECK(bUp); BOOST_CHECK(! bDown);

  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_UDINT, CIEC_ANY::e_USINT, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_UDINT, CIEC_ANY::e_UINT, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_UDINT, CIEC_ANY::e_UDINT, bUp, bDown)); BOOST_CHECK(bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_UDINT, CIEC_ANY::e_ULINT, bUp, bDown)); BOOST_CHECK(bUp); BOOST_CHECK(! bDown);

  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_UDINT, CIEC_ANY::e_BYTE, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_UDINT, CIEC_ANY::e_WORD, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_UDINT, CIEC_ANY::e_DWORD, bUp, bDown)); BOOST_CHECK(bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_UDINT, CIEC_ANY::e_LWORD, bUp, bDown)); BOOST_CHECK(bUp); BOOST_CHECK(! bDown);

  BOOST_CHECK(!CIEC_ANY::isCastable(CIEC_ANY::e_UDINT, CIEC_ANY::e_DATE, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(!CIEC_ANY::isCastable(CIEC_ANY::e_UDINT, CIEC_ANY::e_TIME_OF_DAY, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(!CIEC_ANY::isCastable(CIEC_ANY::e_UDINT, CIEC_ANY::e_DATE_AND_TIME, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(!CIEC_ANY::isCastable(CIEC_ANY::e_UDINT, CIEC_ANY::e_TIME, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(! bDown);

  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_UDINT, CIEC_ANY::e_REAL, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_UDINT, CIEC_ANY::e_LREAL, bUp, bDown)); BOOST_CHECK(bUp); BOOST_CHECK(! bDown);

  BOOST_CHECK(!CIEC_ANY::isCastable(CIEC_ANY::e_UDINT, CIEC_ANY::e_STRING, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(!CIEC_ANY::isCastable(CIEC_ANY::e_UDINT, CIEC_ANY::e_WSTRING, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(! bDown);
}

BOOST_AUTO_TEST_CASE(Explict_cast_operator_to_UDINT)
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

  CIEC_UDINT nResult(0);

  nResult = CIEC_ANY::cast<CIEC_UDINT>(nUsint);
  BOOST_TEST(static_cast<CIEC_UDINT::TValueType>(nResult) == 2);
  nResult = CIEC_ANY::cast<CIEC_UDINT>(nUint);
  BOOST_TEST(static_cast<CIEC_UDINT::TValueType>(nResult) == 3);
  nResult = CIEC_ANY::cast<CIEC_UDINT>(nUdint);
  BOOST_TEST(static_cast<CIEC_UDINT::TValueType>(nResult) == 4);
  nResult = CIEC_ANY::cast<CIEC_UDINT>(nUlint);
  BOOST_TEST(static_cast<CIEC_UDINT::TValueType>(nResult) == 5);
  nResult = CIEC_ANY::cast<CIEC_UDINT>(nSint);
  BOOST_TEST(static_cast<CIEC_UDINT::TValueType>(nResult) == 6);
  nResult = CIEC_ANY::cast<CIEC_UDINT>(nInt);
  BOOST_TEST(static_cast<CIEC_UDINT::TValueType>(nResult) == 7);
  nResult = CIEC_ANY::cast<CIEC_UDINT>(nDint);
  BOOST_TEST(static_cast<CIEC_UDINT::TValueType>(nResult) == 8);
  nResult = CIEC_ANY::cast<CIEC_UDINT>(nLint);
  BOOST_TEST(static_cast<CIEC_UDINT::TValueType>(nResult) == 9);
  nResult = CIEC_ANY::cast<CIEC_UDINT>(nReal);
  BOOST_TEST(static_cast<CIEC_UDINT::TValueType>(nResult) == 10);
  nResult = CIEC_ANY::cast<CIEC_UDINT>(nLreal);
  BOOST_TEST(static_cast<CIEC_UDINT::TValueType>(nResult) == 11);
  nResult = CIEC_ANY::cast<CIEC_UDINT>(nByte);
  BOOST_TEST(static_cast<CIEC_UDINT::TValueType>(nResult) == 12);
  nResult = CIEC_ANY::cast<CIEC_UDINT>(nWord);
  BOOST_TEST(static_cast<CIEC_UDINT::TValueType>(nResult) == 13);
  nResult = CIEC_ANY::cast<CIEC_UDINT>(nDword);
  BOOST_TEST(static_cast<CIEC_UDINT::TValueType>(nResult) == 14);
  nResult = CIEC_ANY::cast<CIEC_UDINT>(nLword);
  BOOST_TEST(static_cast<CIEC_UDINT::TValueType>(nResult) == 15);
  nResult = CIEC_ANY::cast<CIEC_UDINT>(bBool);
  BOOST_TEST(static_cast<CIEC_UDINT::TValueType>(nResult) == true);
}

BOOST_AUTO_TEST_SUITE_END()
