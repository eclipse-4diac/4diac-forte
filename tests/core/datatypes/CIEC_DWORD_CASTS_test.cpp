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

#include <boost/test/tools/floating_point_comparison.hpp>
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

BOOST_AUTO_TEST_SUITE(CIEC_DWORD_casts_function_test)

BOOST_AUTO_TEST_CASE(Implicit_bool_cast_assignment)
{
  CIEC_DWORD testee;
  BOOST_CHECK_EQUAL(testee, 0);

  testee = CIEC_BOOL(true);
  BOOST_CHECK_EQUAL(testee, 1);

  testee = CIEC_BOOL(false);
  BOOST_CHECK_EQUAL(testee, 0);
}

BOOST_AUTO_TEST_CASE(Implicit_bool_cast_ctor)
{
  CIEC_DWORD testee(CIEC_BOOL(true));
  BOOST_CHECK_EQUAL(testee, 1);
}

BOOST_AUTO_TEST_CASE(CASTS_DWORD_to_BOOL)
{
  CIEC_DWORD nTestDWord0;
  CIEC_DWORD nTestDWord1;
  CIEC_DWORD nTestDWord65536;
  CIEC_DWORD nTestDWord4294967295;

  CIEC_BOOL bTestBool;

//check initial values
  BOOST_CHECK_EQUAL(nTestDWord0, 0UL);
  BOOST_CHECK_EQUAL(nTestDWord1, 0UL);
  BOOST_CHECK_EQUAL(nTestDWord65536, 0UL);
  BOOST_CHECK_EQUAL(nTestDWord4294967295, 0UL);

  nTestDWord0 = CIEC_DWORD(0x00);
  nTestDWord1 = CIEC_DWORD(0x01);
  nTestDWord65536 = CIEC_DWORD(0x010000);
  nTestDWord4294967295 = CIEC_DWORD(0xffffffff);

  BOOST_CHECK_EQUAL(nTestDWord0, 0UL);
  BOOST_CHECK_EQUAL(nTestDWord1, 1UL);
  BOOST_CHECK_EQUAL(nTestDWord65536, 65536UL);
  BOOST_CHECK_EQUAL(nTestDWord4294967295, 4294967295UL);

//checks
  bTestBool.setValue(nTestDWord0);
  BOOST_CHECK_EQUAL(bTestBool, false);
  bTestBool.setValue(nTestDWord1);
  BOOST_CHECK_EQUAL(bTestBool, true);
  bTestBool.setValue(nTestDWord65536);
  BOOST_CHECK_EQUAL(bTestBool, true);
  bTestBool.setValue(nTestDWord4294967295);
  BOOST_CHECK_EQUAL(bTestBool, true);

}

BOOST_AUTO_TEST_CASE(CASTS_DWORD_to_BITDTs)
{
  CIEC_DWORD nTestDWord0;
  CIEC_DWORD nTestDWord1;
  CIEC_DWORD nTestDWord65536;
  CIEC_DWORD nTestDWord4294967295;

  CIEC_BYTE nTestByte;
  CIEC_WORD nTestWord;
  CIEC_LWORD nTestLWord;

//check initial values
  BOOST_CHECK_EQUAL(nTestDWord0, 0UL);
  BOOST_CHECK_EQUAL(nTestDWord1, 0UL);
  BOOST_CHECK_EQUAL(nTestDWord65536, 0UL);
  BOOST_CHECK_EQUAL(nTestDWord4294967295, 0UL);

  nTestDWord0 = CIEC_DWORD(0UL);
  nTestDWord1 = CIEC_DWORD(1UL);
  nTestDWord65536 = CIEC_DWORD(65536UL);
  nTestDWord4294967295 = CIEC_DWORD(4294967295UL);

  BOOST_CHECK_EQUAL(nTestDWord0, 0UL);
  BOOST_CHECK_EQUAL(nTestDWord1, 1UL);
  BOOST_CHECK_EQUAL(nTestDWord65536, 65536UL);
  BOOST_CHECK_EQUAL(nTestDWord4294967295, 4294967295UL);

//toByte
  nTestByte.setValue(nTestDWord0);
  BOOST_CHECK_EQUAL(nTestByte, 0U);
  nTestByte.setValue(nTestDWord1);
  BOOST_CHECK_EQUAL(nTestByte, 1U);
  nTestByte.setValue(nTestDWord65536);
  BOOST_CHECK_EQUAL(nTestByte, 0U);
  nTestByte.setValue(nTestDWord4294967295);
  BOOST_CHECK_EQUAL(nTestByte, 255U);

//toWord
  nTestWord.setValue(nTestDWord0);
  BOOST_CHECK_EQUAL(nTestWord, 0U);
  nTestWord.setValue(nTestDWord1);
  BOOST_CHECK_EQUAL(nTestWord, 1U);
  nTestWord.setValue(nTestDWord65536);
  BOOST_CHECK_EQUAL(nTestWord, 0U);
  nTestWord.setValue(nTestDWord4294967295);
  BOOST_CHECK_EQUAL(nTestWord, 65535U);

//toLWord
  nTestLWord.setValue(nTestDWord0);
  BOOST_CHECK_EQUAL(nTestLWord, 0ULL);
  nTestLWord.setValue(nTestDWord1);
  BOOST_CHECK_EQUAL(nTestLWord, 1ULL);
  nTestLWord.setValue(nTestDWord65536);
  BOOST_CHECK_EQUAL(nTestLWord, 65536ULL);
  nTestLWord.setValue(nTestDWord4294967295);
  BOOST_CHECK_EQUAL(nTestLWord, 4294967295ULL);
}

BOOST_AUTO_TEST_CASE(CASTS_DWORD_to_INTS)
{
  CIEC_DWORD nTestDWord0;
  CIEC_DWORD nTestDWord1;
  CIEC_DWORD nTestDWord65536;
  CIEC_DWORD nTestDWord4294967295;

  CIEC_SINT nTestSInt;
  CIEC_USINT nTestUSInt;
  CIEC_INT nTestInt;
  CIEC_UINT nTestUInt;
  CIEC_DINT nTestDInt;
  CIEC_UDINT nTestUDInt;
  CIEC_LINT nTestLInt;
  CIEC_ULINT nTestULInt;

  //check initial values
    BOOST_CHECK_EQUAL(nTestDWord0, 0UL);
    BOOST_CHECK_EQUAL(nTestDWord1, 0UL);
    BOOST_CHECK_EQUAL(nTestDWord65536, 0UL);
    BOOST_CHECK_EQUAL(nTestDWord4294967295, 0UL);

    nTestDWord0 = CIEC_DWORD(0UL);
    nTestDWord1 = CIEC_DWORD(1UL);
    nTestDWord65536 = CIEC_DWORD(65536UL);
    nTestDWord4294967295 = CIEC_DWORD(4294967295UL);

    BOOST_CHECK_EQUAL(nTestDWord0, 0UL);
    BOOST_CHECK_EQUAL(nTestDWord1, 1UL);
    BOOST_CHECK_EQUAL(nTestDWord65536, 65536UL);
    BOOST_CHECK_EQUAL(nTestDWord4294967295, 4294967295UL);

//toSINT
  nTestSInt.setValue(nTestDWord0);
  BOOST_TEST(0 == static_cast<CIEC_SINT::TValueType>(nTestSInt));
  nTestSInt.setValue(nTestDWord1);
  BOOST_TEST(1 == static_cast<CIEC_SINT::TValueType>(nTestSInt));
  nTestSInt.setValue(nTestDWord65536);
  BOOST_TEST(0 == static_cast<CIEC_SINT::TValueType>(nTestSInt));
  nTestSInt.setValue(nTestDWord4294967295);
  BOOST_TEST(-1 == static_cast<CIEC_SINT::TValueType>(nTestSInt));

  // toUSINT
  nTestUSInt.setValue(nTestDWord0);
  BOOST_CHECK_EQUAL(static_cast<CIEC_USINT::TValueType>(nTestUSInt), 0U);
  nTestUSInt.setValue(nTestDWord1);
  BOOST_CHECK_EQUAL(static_cast<CIEC_USINT::TValueType>(nTestUSInt), 1U);
  nTestUSInt.setValue(nTestDWord65536);
  BOOST_CHECK_EQUAL(static_cast<CIEC_USINT::TValueType>(nTestUSInt), 0U);
  nTestUSInt.setValue(nTestDWord4294967295);
  BOOST_CHECK_EQUAL(static_cast<CIEC_USINT::TValueType>(nTestUSInt), 255U);


//toINT
  nTestInt.setValue(nTestDWord0);
  BOOST_TEST(0 == static_cast<CIEC_INT::TValueType>(nTestInt));
  nTestInt.setValue(nTestDWord1);
  BOOST_TEST(1 == static_cast<CIEC_INT::TValueType>(nTestInt));
  nTestInt.setValue(nTestDWord65536);
  BOOST_TEST(0 == static_cast<CIEC_INT::TValueType>(nTestInt));
  nTestInt.setValue(nTestDWord4294967295);
  BOOST_TEST(-1 == static_cast<CIEC_INT::TValueType>(nTestInt));

//toUINT
  nTestUInt.setValue(nTestDWord0);
  BOOST_CHECK_EQUAL(static_cast<CIEC_UINT::TValueType>(nTestUInt), 0U);
  nTestUInt.setValue(nTestDWord1);
  BOOST_CHECK_EQUAL(static_cast<CIEC_UINT::TValueType>(nTestUInt), 1U);
  nTestUInt.setValue(nTestDWord65536);
  BOOST_CHECK_EQUAL(static_cast<CIEC_UINT::TValueType>(nTestUInt), 0U);
  nTestUInt.setValue(nTestDWord4294967295);
  BOOST_CHECK_EQUAL(static_cast<CIEC_UINT::TValueType>(nTestUInt), 65535U);

//toDINT
  nTestDInt.setValue(nTestDWord0);
  BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(nTestDInt), 0L);
  nTestDInt.setValue(nTestDWord1);
  BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(nTestDInt), 1L);
  nTestDInt.setValue(nTestDWord65536);
  BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(nTestDInt), 65536L);
  nTestDInt.setValue(nTestDWord4294967295);
  BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(nTestDInt), -1);

//toUDINT
  nTestUDInt.setValue(nTestDWord0);
  BOOST_CHECK_EQUAL(static_cast<CIEC_UDINT::TValueType>(nTestUDInt), 0UL);
  nTestUDInt.setValue(nTestDWord1);
  BOOST_CHECK_EQUAL(static_cast<CIEC_UDINT::TValueType>(nTestUDInt), 1UL);
  nTestUDInt.setValue(nTestDWord65536);
  BOOST_CHECK_EQUAL(static_cast<CIEC_UDINT::TValueType>(nTestUDInt), 65536UL);
  nTestUDInt.setValue(nTestDWord4294967295);
  BOOST_CHECK_EQUAL(static_cast<CIEC_UDINT::TValueType>(nTestUDInt), 4294967295UL);

//toLINT
  nTestLInt.setValue(nTestDWord0);
  BOOST_CHECK_EQUAL(static_cast<CIEC_LINT::TValueType>(nTestLInt), 0LL);
  nTestLInt.setValue(nTestDWord1);
  BOOST_CHECK_EQUAL(static_cast<CIEC_LINT::TValueType>(nTestLInt), 1LL);
  nTestLInt.setValue(nTestDWord65536);
  BOOST_CHECK_EQUAL(static_cast<CIEC_LINT::TValueType>(nTestLInt), 65536LL);
  nTestLInt.setValue(nTestDWord4294967295);
  BOOST_CHECK_EQUAL(static_cast<CIEC_LINT::TValueType>(nTestLInt), 4294967295LL);

  // toULINT
  nTestULInt.setValue(nTestDWord0);
  BOOST_CHECK_EQUAL(static_cast<CIEC_ULINT::TValueType>(nTestULInt), 0ULL);
  nTestULInt.setValue(nTestDWord1);
  BOOST_CHECK_EQUAL(static_cast<CIEC_ULINT::TValueType>(nTestULInt), 1ULL);
  nTestULInt.setValue(nTestDWord65536);
  BOOST_CHECK_EQUAL(static_cast<CIEC_ULINT::TValueType>(nTestULInt), 65536ULL);
  nTestULInt.setValue(nTestDWord4294967295);
  BOOST_CHECK_EQUAL(static_cast<CIEC_ULINT::TValueType>(nTestULInt), 4294967295ULL);
}

BOOST_AUTO_TEST_CASE(CASTS_DWORD_to_REAL)
{
  CIEC_DWORD nTestDWord0;
  CIEC_DWORD nTestDWord1;
  CIEC_DWORD nTestDWord65536;
  CIEC_DWORD nTestDWord4294967295;

  CIEC_REAL nTestReal;

//check initial values
  BOOST_CHECK_EQUAL(nTestDWord0, 0UL);
  BOOST_CHECK_EQUAL(nTestDWord1, 0UL);
  BOOST_CHECK_EQUAL(nTestDWord65536, 0UL);
  BOOST_CHECK_EQUAL(nTestDWord4294967295, 0UL);

  nTestDWord0 = CIEC_DWORD(0UL);
  nTestDWord1 = CIEC_DWORD(1UL);
  nTestDWord65536 = CIEC_DWORD(65536UL);
  nTestDWord4294967295 = CIEC_DWORD(4294967295UL);

  BOOST_CHECK_EQUAL(nTestDWord0, 0UL);
  BOOST_CHECK_EQUAL(nTestDWord1, 1UL);
  BOOST_CHECK_EQUAL(nTestDWord65536, 65536UL);
  BOOST_CHECK_EQUAL(nTestDWord4294967295, 4294967295UL);

//toREAL
  nTestReal.setValue(nTestDWord0);
  BOOST_CHECK_EQUAL(0.0f, nTestReal);
  nTestReal.setValue(nTestDWord1);
  BOOST_CHECK_EQUAL(1.4013e-45f, nTestReal);
  nTestReal.setValue(nTestDWord65536);
  BOOST_CHECK_EQUAL(9.18355e-41f, nTestReal);
  nTestReal.setValue(nTestDWord4294967295);
  BOOST_CHECK(std::isnan(nTestReal));

  CIEC_LREAL nTestLReal;

//toLREAL
  nTestLReal.setValue(nTestDWord0);
  BOOST_CHECK_EQUAL(0.0, nTestLReal);
  nTestLReal.setValue(nTestDWord1);
  BOOST_CHECK_EQUAL(4.94066e-324, nTestLReal);
  nTestLReal.setValue(nTestDWord65536);
  BOOST_CHECK_EQUAL(3.23791e-319, nTestLReal);
  nTestLReal.setValue(nTestDWord4294967295);
  BOOST_TEST(2.1219957904712067e-314 == static_cast<CIEC_LREAL::TValueType>(nTestLReal));
}

BOOST_AUTO_TEST_CASE(DWORD_Castable_test)
{
  bool bUp, bDown;

  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_DWORD, CIEC_ANY::e_BOOL, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(bDown);

  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_DWORD, CIEC_ANY::e_SINT, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_DWORD, CIEC_ANY::e_INT, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_DWORD, CIEC_ANY::e_DINT, bUp, bDown)); BOOST_CHECK(bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_DWORD, CIEC_ANY::e_LINT, bUp, bDown)); BOOST_CHECK(bUp); BOOST_CHECK(! bDown);

  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_DWORD, CIEC_ANY::e_USINT, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_DWORD, CIEC_ANY::e_UINT, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_DWORD, CIEC_ANY::e_UDINT, bUp, bDown)); BOOST_CHECK(bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_DWORD, CIEC_ANY::e_ULINT, bUp, bDown)); BOOST_CHECK(bUp); BOOST_CHECK(! bDown);

  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_DWORD, CIEC_ANY::e_BYTE, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_DWORD, CIEC_ANY::e_WORD, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_DWORD, CIEC_ANY::e_DWORD, bUp, bDown)); BOOST_CHECK(bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_DWORD, CIEC_ANY::e_LWORD, bUp, bDown)); BOOST_CHECK(bUp); BOOST_CHECK(! bDown);

  BOOST_CHECK(!CIEC_ANY::isCastable(CIEC_ANY::e_DWORD, CIEC_ANY::e_DATE, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(!CIEC_ANY::isCastable(CIEC_ANY::e_DWORD, CIEC_ANY::e_TIME_OF_DAY, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(!CIEC_ANY::isCastable(CIEC_ANY::e_DWORD, CIEC_ANY::e_DATE_AND_TIME, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(!CIEC_ANY::isCastable(CIEC_ANY::e_DWORD, CIEC_ANY::e_TIME, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(! bDown);

  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_DWORD, CIEC_ANY::e_REAL, bUp, bDown)); BOOST_CHECK(bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_DWORD, CIEC_ANY::e_LREAL, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(bDown);

  BOOST_CHECK(!CIEC_ANY::isCastable(CIEC_ANY::e_DWORD, CIEC_ANY::e_STRING, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(!CIEC_ANY::isCastable(CIEC_ANY::e_DWORD, CIEC_ANY::e_WSTRING, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(! bDown);
}

BOOST_AUTO_TEST_CASE(Explict_cast_operator_to_DWORD)
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
  CIEC_BYTE nByte(12);
  CIEC_WORD nWord(13);
  CIEC_DWORD nDword(14);
  CIEC_LWORD nLword(15);
  CIEC_BOOL bBool(true);

  CIEC_DWORD nResult(0);

  nResult = CIEC_ANY::cast<CIEC_DWORD>(nUsint);
  BOOST_TEST(nResult == 2);
  nResult = CIEC_ANY::cast<CIEC_DWORD>(nUint);
  BOOST_TEST(nResult == 3);
  nResult = CIEC_ANY::cast<CIEC_DWORD>(nUdint);
  BOOST_TEST(nResult == 4);
  nResult = CIEC_ANY::cast<CIEC_DWORD>(nUlint);
  BOOST_TEST(nResult == 5);
  nResult = CIEC_ANY::cast<CIEC_DWORD>(nSint);
  BOOST_TEST(nResult == 6);
  nResult = CIEC_ANY::cast<CIEC_DWORD>(nInt);
  BOOST_TEST(nResult == 7);
  nResult = CIEC_ANY::cast<CIEC_DWORD>(nDint);
  BOOST_TEST(nResult == 8);
  nResult = CIEC_ANY::cast<CIEC_DWORD>(nLint);
  BOOST_TEST(nResult == 9);
  nResult = CIEC_ANY::cast<CIEC_DWORD>(nReal);
  BOOST_TEST(nResult == 0x41200000);
  nResult = CIEC_ANY::cast<CIEC_DWORD>(nByte);
  BOOST_TEST(nResult == 12);
  nResult = CIEC_ANY::cast<CIEC_DWORD>(nWord);
  BOOST_TEST(nResult == 13);
  nResult = CIEC_ANY::cast<CIEC_DWORD>(nDword);
  BOOST_TEST(nResult == 14);
  nResult = CIEC_ANY::cast<CIEC_DWORD>(nLword);
  BOOST_TEST(nResult == 15);
  nResult = CIEC_ANY::cast<CIEC_DWORD>(bBool);
  BOOST_TEST(nResult == true);
}

BOOST_AUTO_TEST_SUITE_END()
