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

BOOST_AUTO_TEST_SUITE(CIEC_WORD_casts_function_test)

BOOST_AUTO_TEST_CASE(Implicit_cast_assignment)
{
  CIEC_WORD testee;
  BOOST_CHECK_EQUAL(testee, 0);

  testee = CIEC_BOOL(true);
  BOOST_CHECK_EQUAL(testee, 1);

  testee = CIEC_BOOL(false);
  BOOST_CHECK_EQUAL(testee, 0);
}

BOOST_AUTO_TEST_CASE(Implicit_cast_ctor)
{
  CIEC_WORD testee(CIEC_BOOL(true));
  BOOST_CHECK_EQUAL(testee, 1);
}

BOOST_AUTO_TEST_CASE(CASTS_WORD_to_BOOL)
{
  CIEC_WORD nTestWord0;
  CIEC_WORD nTestWord1;
  CIEC_WORD nTestWord256;
  CIEC_WORD nTestWord65535;

  CIEC_BOOL bTestBool;

//check initial values
  BOOST_CHECK_EQUAL(nTestWord0, 0U);
  BOOST_CHECK_EQUAL(nTestWord1, 0U);
  BOOST_CHECK_EQUAL(nTestWord256, 0U);
  BOOST_CHECK_EQUAL(nTestWord65535, 0U);

  nTestWord0 = CIEC_WORD(0x00);
  nTestWord1 = CIEC_WORD(0x01);
  nTestWord256 = CIEC_WORD(0x0100);
  nTestWord65535 = CIEC_WORD(0xffff);

  BOOST_CHECK_EQUAL(nTestWord0, 0U);
  BOOST_CHECK_EQUAL(nTestWord1, 1U);
  BOOST_CHECK_EQUAL(nTestWord256, 0x0100);
  BOOST_CHECK_EQUAL(nTestWord65535, 0xffff);

//checks
  bTestBool.setValue(nTestWord0);
  BOOST_CHECK_EQUAL(bTestBool, false);
  bTestBool.setValue(nTestWord1);
  BOOST_CHECK_EQUAL(bTestBool, true);
  bTestBool.setValue(nTestWord256);
  BOOST_CHECK_EQUAL(bTestBool, true);
  bTestBool.setValue(nTestWord65535);
  BOOST_CHECK_EQUAL(bTestBool, true);

}

BOOST_AUTO_TEST_CASE(CASTS_WORD_to_BITDTs)
{
  CIEC_WORD nTestWord0;
  CIEC_WORD nTestWord1;
  CIEC_WORD nTestWord256;
  CIEC_WORD nTestWord65535;

  CIEC_BYTE nTestByte;
  CIEC_DWORD nTestDWord;
  CIEC_LWORD nTestLWord;

//check initial values
  BOOST_CHECK_EQUAL(nTestWord0, 0U);
  BOOST_CHECK_EQUAL(nTestWord1, 0U);
  BOOST_CHECK_EQUAL(nTestWord256, 0U);
  BOOST_CHECK_EQUAL(nTestWord65535, 0U);

  nTestWord0 = CIEC_WORD(0U);
  nTestWord1 = CIEC_WORD(1U);
  nTestWord256 = CIEC_WORD(256U);
  nTestWord65535 = CIEC_WORD(65535U);

  BOOST_CHECK_EQUAL(nTestWord0, 0U);
  BOOST_CHECK_EQUAL(nTestWord1, 1U);
  BOOST_CHECK_EQUAL(nTestWord256, 256U);
  BOOST_CHECK_EQUAL(nTestWord65535, 65535U);

//toByte
  nTestByte.setValue(nTestWord0);
  BOOST_CHECK_EQUAL(nTestByte, 0U);
  nTestByte.setValue(nTestWord1);
  BOOST_CHECK_EQUAL(nTestByte, 1U);
  nTestByte.setValue(nTestWord256);
  BOOST_CHECK_EQUAL(nTestByte, 0U);
  nTestByte.setValue(nTestWord65535);
  BOOST_CHECK_EQUAL(nTestByte, 255U);

//toDWord
  nTestDWord.setValue(nTestWord0);
  BOOST_CHECK_EQUAL(nTestDWord, 0UL);
  nTestDWord.setValue(nTestWord1);
  BOOST_CHECK_EQUAL(nTestDWord, 1UL);
  nTestDWord.setValue(nTestWord256);
  BOOST_CHECK_EQUAL(nTestDWord, 256UL);
  nTestDWord.setValue(nTestWord65535);
  BOOST_CHECK_EQUAL(nTestDWord, 65535UL);

//toLWord
  nTestLWord.setValue(nTestWord0);
  BOOST_CHECK_EQUAL(nTestLWord, 0ULL);
  nTestLWord.setValue(nTestWord1);
  BOOST_CHECK_EQUAL(nTestLWord, 1ULL);
  nTestLWord.setValue(nTestWord256);
  BOOST_CHECK_EQUAL(nTestLWord, 256ULL);
  nTestLWord.setValue(nTestWord65535);
  BOOST_CHECK_EQUAL(nTestLWord, 65535ULL);
}

BOOST_AUTO_TEST_CASE(CASTS_WORD_to_INTS)
{
  CIEC_WORD nTestWord0;
  CIEC_WORD nTestWord1;
  CIEC_WORD nTestWord256;
  CIEC_WORD nTestWord65535;

  CIEC_SINT nTestSInt;
  CIEC_USINT nTestUSInt;
  CIEC_INT nTestInt;
  CIEC_UINT nTestUInt;
  CIEC_DINT nTestDInt;
  CIEC_UDINT nTestUDInt;
  CIEC_LINT nTestLInt;
  CIEC_ULINT nTestULInt;

  //check initial values
    BOOST_CHECK_EQUAL(nTestWord0, 0U);
    BOOST_CHECK_EQUAL(nTestWord1, 0U);
    BOOST_CHECK_EQUAL(nTestWord256, 0U);
    BOOST_CHECK_EQUAL(nTestWord65535, 0U);

    nTestWord0 = CIEC_WORD(0U);
    nTestWord1 = CIEC_WORD(1U);
    nTestWord256 = CIEC_WORD(256U);
    nTestWord65535 = CIEC_WORD(65535U);

    BOOST_CHECK_EQUAL(nTestWord0, 0U);
    BOOST_CHECK_EQUAL(nTestWord1, 1U);
    BOOST_CHECK_EQUAL(nTestWord256, 256U);
    BOOST_CHECK_EQUAL(nTestWord65535, 65535U);

//toSINT
  nTestSInt.setValue(nTestWord0);
  BOOST_TEST(0 == static_cast<CIEC_SINT::TValueType>(nTestSInt));
  nTestSInt.setValue(nTestWord1);
  BOOST_TEST(1 == static_cast<CIEC_SINT::TValueType>(nTestSInt));
  nTestSInt.setValue(nTestWord256);
  BOOST_TEST(0 == static_cast<CIEC_SINT::TValueType>(nTestSInt));
  nTestSInt.setValue(nTestWord65535);
  BOOST_TEST(-1 == static_cast<CIEC_SINT::TValueType>(nTestSInt));

  // toUSINT
  nTestUSInt.setValue(nTestWord0);
  BOOST_CHECK_EQUAL(static_cast<CIEC_USINT::TValueType>(nTestUSInt), 0U);
  nTestUSInt.setValue(nTestWord1);
  BOOST_CHECK_EQUAL(static_cast<CIEC_USINT::TValueType>(nTestUSInt), 1U);
  nTestUSInt.setValue(nTestWord256);
  BOOST_CHECK_EQUAL(static_cast<CIEC_USINT::TValueType>(nTestUSInt), 0U);
  nTestUSInt.setValue(nTestWord65535);
  BOOST_CHECK_EQUAL(static_cast<CIEC_USINT::TValueType>(nTestUSInt), 255U);


//toINT
  nTestInt.setValue(nTestWord0);
  BOOST_TEST(0 == static_cast<CIEC_INT::TValueType>(nTestInt));
  nTestInt.setValue(nTestWord1);
  BOOST_TEST(1 == static_cast<CIEC_INT::TValueType>(nTestInt));
  nTestInt.setValue(nTestWord256);
  BOOST_TEST(256 == static_cast<CIEC_INT::TValueType>(nTestInt));
  nTestInt.setValue(nTestWord65535);
  BOOST_TEST(-1 == static_cast<CIEC_INT::TValueType>(nTestInt));

//toUINT
  nTestUInt.setValue(nTestWord0);
  BOOST_CHECK_EQUAL(static_cast<CIEC_UINT::TValueType>(nTestUInt), 0U);
  nTestUInt.setValue(nTestWord1);
  BOOST_CHECK_EQUAL(static_cast<CIEC_UINT::TValueType>(nTestUInt), 1U);
  nTestUInt.setValue(nTestWord256);
  BOOST_CHECK_EQUAL(static_cast<CIEC_UINT::TValueType>(nTestUInt), 256U);
  nTestUInt.setValue(nTestWord65535);
  BOOST_CHECK_EQUAL(static_cast<CIEC_UINT::TValueType>(nTestUInt), 65535U);

//toDINT
  nTestDInt.setValue(nTestWord0);
  BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(nTestDInt), 0);
  nTestDInt.setValue(nTestWord1);
  BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(nTestDInt), 1);
  nTestDInt.setValue(nTestWord256);
  BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(nTestDInt), 256);
  nTestDInt.setValue(nTestWord65535);
  BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(nTestDInt), 65535);

//toUDINT
  nTestUDInt.setValue(nTestWord0);
  BOOST_CHECK_EQUAL(static_cast<CIEC_UDINT::TValueType>(nTestUDInt), 0UL);
  nTestUDInt.setValue(nTestWord1);
  BOOST_CHECK_EQUAL(static_cast<CIEC_UDINT::TValueType>(nTestUDInt), 1UL);
  nTestUDInt.setValue(nTestWord256);
  BOOST_CHECK_EQUAL(static_cast<CIEC_UDINT::TValueType>(nTestUDInt), 256UL);
  nTestUDInt.setValue(nTestWord65535);
  BOOST_CHECK_EQUAL(static_cast<CIEC_UDINT::TValueType>(nTestUDInt), 65535UL);

//toLINT
  nTestLInt.setValue(nTestWord0);
  BOOST_CHECK_EQUAL(static_cast<CIEC_LINT::TValueType>(nTestLInt), 0LL);
  nTestLInt.setValue(nTestWord1);
  BOOST_CHECK_EQUAL(static_cast<CIEC_LINT::TValueType>(nTestLInt), 1LL);
  nTestLInt.setValue(nTestWord256);
  BOOST_CHECK_EQUAL(static_cast<CIEC_LINT::TValueType>(nTestLInt), 256LL);
  nTestLInt.setValue(nTestWord65535);
  BOOST_CHECK_EQUAL(static_cast<CIEC_LINT::TValueType>(nTestLInt), 65535LL);

  // toULINT
  nTestULInt.setValue(nTestWord0);
  BOOST_CHECK_EQUAL(static_cast<CIEC_ULINT::TValueType>(nTestULInt), 0ULL);
  nTestULInt.setValue(nTestWord1);
  BOOST_CHECK_EQUAL(static_cast<CIEC_ULINT::TValueType>(nTestULInt), 1ULL);
  nTestULInt.setValue(nTestWord256);
  BOOST_CHECK_EQUAL(static_cast<CIEC_ULINT::TValueType>(nTestULInt), 256ULL);
  nTestULInt.setValue(nTestWord65535);
  BOOST_CHECK_EQUAL(static_cast<CIEC_ULINT::TValueType>(nTestULInt), 65535ULL);
}

BOOST_AUTO_TEST_CASE(CASTS_WORD_to_REAL)
{
  CIEC_WORD nTestWord0;
  CIEC_WORD nTestWord1;
  CIEC_WORD nTestWord256;
  CIEC_WORD nTestWord65535;

  CIEC_REAL nTestReal;

//check initial values
  BOOST_CHECK_EQUAL(nTestWord0, 0U);
  BOOST_CHECK_EQUAL(nTestWord1, 0U);
  BOOST_CHECK_EQUAL(nTestWord256, 0U);
  BOOST_CHECK_EQUAL(nTestWord65535, 0U);

  nTestWord0 = CIEC_WORD(0U);
  nTestWord1 = CIEC_WORD(1U);
  nTestWord256 = CIEC_WORD(256U);
  nTestWord65535 = CIEC_WORD(65535U);

  BOOST_CHECK_EQUAL(nTestWord0, 0U);
  BOOST_CHECK_EQUAL(nTestWord1, 1U);
  BOOST_CHECK_EQUAL(nTestWord256, 256U);
  BOOST_CHECK_EQUAL(nTestWord65535, 65535U);

//toREAL
  nTestReal.setValue(nTestWord0);
  BOOST_CHECK_EQUAL(0.0f, nTestReal);
  nTestReal.setValue(nTestWord1);
  BOOST_CHECK_EQUAL(1.4013e-45f, nTestReal);
  nTestReal.setValue(nTestWord256);
  BOOST_CHECK_EQUAL(3.58732e-43f, nTestReal);
  nTestReal.setValue(nTestWord65535);
  BOOST_CHECK_EQUAL(9.18341e-41f, nTestReal);

  CIEC_LREAL nTestLReal;

//toLREAL
  nTestLReal.setValue(nTestWord0);
  BOOST_CHECK_EQUAL(0.0, nTestLReal);
  nTestLReal.setValue(nTestWord1);
  BOOST_CHECK_EQUAL(4.94066e-324, nTestLReal);
  nTestLReal.setValue(nTestWord256);
  BOOST_CHECK_EQUAL(1.26481e-321, nTestLReal);
  nTestLReal.setValue(nTestWord65535);
  BOOST_CHECK_EQUAL(3.23786e-319, nTestLReal);
}

BOOST_AUTO_TEST_CASE(WORD_Castable_test)
{
  bool bUp, bDown;

  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_WORD, CIEC_ANY::e_BOOL, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(bDown);

  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_WORD, CIEC_ANY::e_SINT, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_WORD, CIEC_ANY::e_INT, bUp, bDown)); BOOST_CHECK(bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_WORD, CIEC_ANY::e_DINT, bUp, bDown)); BOOST_CHECK(bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_WORD, CIEC_ANY::e_LINT, bUp, bDown)); BOOST_CHECK(bUp); BOOST_CHECK(! bDown);

  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_WORD, CIEC_ANY::e_USINT, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_WORD, CIEC_ANY::e_UINT, bUp, bDown)); BOOST_CHECK(bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_WORD, CIEC_ANY::e_UDINT, bUp, bDown)); BOOST_CHECK(bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_WORD, CIEC_ANY::e_ULINT, bUp, bDown)); BOOST_CHECK(bUp); BOOST_CHECK(! bDown);

  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_WORD, CIEC_ANY::e_BYTE, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_WORD, CIEC_ANY::e_WORD, bUp, bDown)); BOOST_CHECK(bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_WORD, CIEC_ANY::e_DWORD, bUp, bDown)); BOOST_CHECK(bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_WORD, CIEC_ANY::e_LWORD, bUp, bDown)); BOOST_CHECK(bUp); BOOST_CHECK(! bDown);

  BOOST_CHECK(!CIEC_ANY::isCastable(CIEC_ANY::e_WORD, CIEC_ANY::e_DATE, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(!CIEC_ANY::isCastable(CIEC_ANY::e_WORD, CIEC_ANY::e_TIME_OF_DAY, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(!CIEC_ANY::isCastable(CIEC_ANY::e_WORD, CIEC_ANY::e_DATE_AND_TIME, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(!CIEC_ANY::isCastable(CIEC_ANY::e_WORD, CIEC_ANY::e_TIME, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(! bDown);

  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_WORD, CIEC_ANY::e_REAL, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_WORD, CIEC_ANY::e_LREAL, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(bDown);

  BOOST_CHECK(!CIEC_ANY::isCastable(CIEC_ANY::e_WORD, CIEC_ANY::e_STRING, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(!CIEC_ANY::isCastable(CIEC_ANY::e_WORD, CIEC_ANY::e_WSTRING, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(! bDown);
}

BOOST_AUTO_TEST_CASE(Explict_cast_operator_to_WORD)
{
  CIEC_USINT nUsint(2);
  CIEC_UINT nUint(3);
  CIEC_UDINT nUdint(4);
  CIEC_ULINT nUlint(5);
  CIEC_SINT nSint(6);
  CIEC_INT nInt(7);
  CIEC_DINT nDint(8);
  CIEC_LINT nLint(9);
  CIEC_BYTE nByte(12);
  CIEC_WORD nWord(13);
  CIEC_DWORD nDword(14);
  CIEC_LWORD nLword(15);
  CIEC_BOOL bBool(true);

  CIEC_WORD nResult(0);

  nResult = CIEC_ANY::cast<CIEC_WORD>(nUsint);
  BOOST_TEST(nResult == 2);
  nResult = CIEC_ANY::cast<CIEC_WORD>(nUint);
  BOOST_TEST(nResult == 3);
  nResult = CIEC_ANY::cast<CIEC_WORD>(nUdint);
  BOOST_TEST(nResult == 4);
  nResult = CIEC_ANY::cast<CIEC_WORD>(nUlint);
  BOOST_TEST(nResult == 5);
  nResult = CIEC_ANY::cast<CIEC_WORD>(nSint);
  BOOST_TEST(nResult == 6);
  nResult = CIEC_ANY::cast<CIEC_WORD>(nInt);
  BOOST_TEST(nResult == 7);
  nResult = CIEC_ANY::cast<CIEC_WORD>(nDint);
  BOOST_TEST(nResult == 8);
  nResult = CIEC_ANY::cast<CIEC_WORD>(nLint);
  BOOST_TEST(nResult == 9);
  nResult = CIEC_ANY::cast<CIEC_WORD>(nByte);
  BOOST_TEST(nResult == 12);
  nResult = CIEC_ANY::cast<CIEC_WORD>(nWord);
  BOOST_TEST(nResult == 13);
  nResult = CIEC_ANY::cast<CIEC_WORD>(nDword);
  BOOST_TEST(nResult == 14);
  nResult = CIEC_ANY::cast<CIEC_WORD>(nLword);
  BOOST_TEST(nResult == 15);
  nResult = CIEC_ANY::cast<CIEC_WORD>(bBool);
  BOOST_TEST(nResult == true);
}

BOOST_AUTO_TEST_SUITE_END()
