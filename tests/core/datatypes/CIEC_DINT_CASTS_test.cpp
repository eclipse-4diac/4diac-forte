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

BOOST_AUTO_TEST_SUITE(CIEC_DINT_casts_function_test)


BOOST_AUTO_TEST_CASE(CASTS_DINT_to_BOOL)
{
  CIEC_DINT nTestDInt0;
  CIEC_DINT nTestDInt1;
  CIEC_DINT nTestDInt32768;
  CIEC_DINT nTestDInt2147483647;

  CIEC_BOOL bTestBool;

//check initial values
  BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(nTestDInt0), 0);
  BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(nTestDInt1), 0);
  BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(nTestDInt32768), 0);
  BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(nTestDInt2147483647), 0);

  nTestDInt0 = CIEC_DINT(0);
  nTestDInt1 = CIEC_DINT(1);
  nTestDInt32768 = CIEC_DINT(32768);
  nTestDInt2147483647 = CIEC_DINT(2147483647);

  BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(nTestDInt0), 0);
  BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(nTestDInt1), 1);
  BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(nTestDInt32768), 32768);
  BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(nTestDInt2147483647), 2147483647);

//checks
  bTestBool.setValue(nTestDInt0);
  BOOST_CHECK_EQUAL(false, bTestBool);
  bTestBool.setValue(nTestDInt1);
  BOOST_CHECK_EQUAL(true, bTestBool);
  bTestBool.setValue(nTestDInt32768);
  BOOST_CHECK_EQUAL(true, bTestBool);
  bTestBool.setValue(nTestDInt2147483647);
  BOOST_CHECK_EQUAL(true, bTestBool);

}

BOOST_AUTO_TEST_CASE(CASTS_DINT_to_BITDTs)
{
  CIEC_DINT nTestDInt0;
  CIEC_DINT nTestDInt1;
  CIEC_DINT nTestDInt32768;
  CIEC_DINT nTestDInt2147483647;

  CIEC_BYTE nTestByte;
  CIEC_WORD nTestWord;
  CIEC_DWORD nTestDWord;
  CIEC_LWORD nTestLWord;

//check initial values
  BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(nTestDInt0), 0);
  BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(nTestDInt1), 0);
  BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(nTestDInt32768), 0);
  BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(nTestDInt2147483647), 0);

  nTestDInt0 = CIEC_DINT(0);
  nTestDInt1 = CIEC_DINT(1);
  nTestDInt32768 = CIEC_DINT(32768);
  nTestDInt2147483647 = CIEC_DINT(2147483647);

  BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(nTestDInt0), 0);
  BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(nTestDInt1), 1);
  BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(nTestDInt32768), 32768);
  BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(nTestDInt2147483647), 2147483647);

  // toByte
  nTestByte.setValue(nTestDInt0);
  BOOST_CHECK_EQUAL(nTestByte, 0U);
  nTestByte.setValue(nTestDInt1);
  BOOST_CHECK_EQUAL(nTestByte, 1U);
  nTestByte.setValue(nTestDInt32768);
  BOOST_CHECK_EQUAL(nTestByte, 0U);
  nTestByte.setValue(nTestDInt2147483647);
  BOOST_CHECK_EQUAL(nTestByte, 255U);

//toWord
  nTestWord.setValue(nTestDInt0);
  BOOST_CHECK_EQUAL(nTestWord, 0U);
  nTestWord.setValue(nTestDInt1);
  BOOST_CHECK_EQUAL(nTestWord, 1U);
  nTestWord.setValue(nTestDInt32768);
  BOOST_CHECK_EQUAL(nTestWord, 32768U);
  nTestWord.setValue(nTestDInt2147483647);
  BOOST_CHECK_EQUAL(nTestWord, 65535U);

//toDWord
  nTestDWord.setValue(nTestDInt0);
  BOOST_CHECK_EQUAL(nTestDWord, 0UL);
  nTestDWord.setValue(nTestDInt1);
  BOOST_CHECK_EQUAL(nTestDWord, 1UL);
  nTestDWord.setValue(nTestDInt32768);
  BOOST_CHECK_EQUAL(nTestDWord, 32768UL);
  nTestDWord.setValue(nTestDInt2147483647);
  BOOST_CHECK_EQUAL(nTestDWord, 2147483647UL);

//toLWord
  nTestLWord.setValue(nTestDInt0);
  BOOST_CHECK_EQUAL(nTestLWord, 0ULL);
  nTestLWord.setValue(nTestDInt1);
  BOOST_CHECK_EQUAL(nTestLWord, 1ULL);
  nTestLWord.setValue(nTestDInt32768);
  BOOST_CHECK_EQUAL(nTestLWord, 32768ULL);
  nTestLWord.setValue(nTestDInt2147483647);
  BOOST_CHECK_EQUAL(nTestLWord, 2147483647ULL);
}

BOOST_AUTO_TEST_CASE(CASTS_DINT_to_INTS)
{
  CIEC_DINT nTestDInt0;
  CIEC_DINT nTestDInt1;
  CIEC_DINT nTestDInt32768;
  CIEC_DINT nTestDInt2147483647;
  CIEC_DINT nTestDInt_1;

  CIEC_USINT nTestUSInt;
  CIEC_SINT nTestSInt;
  CIEC_UINT nTestUInt;
  CIEC_INT nTestInt;
  CIEC_UDINT nTestUDInt;
  CIEC_LINT nTestLInt;
  CIEC_ULINT nTestULInt;

  //check initial values
  BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(nTestDInt0), 0);
  BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(nTestDInt1), 0);
  BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(nTestDInt32768), 0);
  BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(nTestDInt2147483647), 0);
  BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(nTestDInt_1), 0);

  nTestDInt0 = CIEC_DINT(0);
  nTestDInt1 = CIEC_DINT(1);
  nTestDInt32768 = CIEC_DINT(32768);
  nTestDInt2147483647 = CIEC_DINT(2147483647);
  nTestDInt_1 = CIEC_DINT(-1);

  BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(nTestDInt0), 0);
  BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(nTestDInt1), 1);
  BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(nTestDInt32768), 32768);
  BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(nTestDInt2147483647), 2147483647);
  BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(nTestDInt_1), -1);

//toUSINT
  nTestUSInt.setValue(nTestDInt0);
  BOOST_CHECK_EQUAL(static_cast<CIEC_USINT::TValueType>(nTestUSInt), 0U);
  nTestUSInt.setValue(nTestDInt1);
  BOOST_CHECK_EQUAL(static_cast<CIEC_USINT::TValueType>(nTestUSInt), 1U);
  nTestUSInt.setValue(nTestDInt32768);
  BOOST_CHECK_EQUAL(static_cast<CIEC_USINT::TValueType>(nTestUSInt), 0U);
  nTestUSInt.setValue(nTestDInt2147483647);
  BOOST_CHECK_EQUAL(static_cast<CIEC_USINT::TValueType>(nTestUSInt), 255U);
  nTestUSInt.setValue(nTestDInt_1);
  BOOST_CHECK_EQUAL(static_cast<CIEC_USINT::TValueType>(nTestUSInt), 255U);


//toSINT
  nTestSInt.setValue(nTestDInt0);
  BOOST_TEST(0 == static_cast<CIEC_SINT::TValueType>(nTestSInt));
  nTestSInt.setValue(nTestDInt1);
  BOOST_TEST(1 == static_cast<CIEC_SINT::TValueType>(nTestSInt));
  nTestSInt.setValue(nTestDInt32768);
  BOOST_TEST(0 == static_cast<CIEC_SINT::TValueType>(nTestSInt));
  nTestSInt.setValue(nTestDInt2147483647);
  BOOST_TEST(-1 == static_cast<CIEC_SINT::TValueType>(nTestSInt));
  nTestSInt.setValue(nTestDInt_1);
  BOOST_TEST(-1 == static_cast<CIEC_SINT::TValueType>(nTestSInt));

//toUINT
  nTestUInt.setValue(nTestDInt0);
  BOOST_CHECK_EQUAL(static_cast<CIEC_UINT::TValueType>(nTestUInt), 0U);
  nTestUInt.setValue(nTestDInt1);
  BOOST_CHECK_EQUAL(static_cast<CIEC_UINT::TValueType>(nTestUInt), 1U);
  nTestUInt.setValue(nTestDInt32768);
  BOOST_CHECK_EQUAL(static_cast<CIEC_UINT::TValueType>(nTestUInt), 32768U);
  nTestUInt.setValue(nTestDInt2147483647);
  BOOST_CHECK_EQUAL(static_cast<CIEC_UINT::TValueType>(nTestUInt), 65535U);
  nTestUInt.setValue(nTestDInt_1);
  BOOST_CHECK_EQUAL(static_cast<CIEC_UINT::TValueType>(nTestUInt), 65535U);

  // toINT
  nTestInt.setValue(nTestDInt0);
  BOOST_TEST(0 == static_cast<CIEC_INT::TValueType>(nTestInt));
  nTestInt.setValue(nTestDInt1);
  BOOST_TEST(1 == static_cast<CIEC_INT::TValueType>(nTestInt));
  nTestInt.setValue(nTestDInt32768);
  BOOST_TEST(-32768 == static_cast<CIEC_INT::TValueType>(nTestInt));
  nTestInt.setValue(nTestDInt2147483647);
  BOOST_TEST(-1 == static_cast<CIEC_INT::TValueType>(nTestInt));
  nTestInt.setValue(nTestDInt_1);
  BOOST_TEST(-1 == static_cast<CIEC_INT::TValueType>(nTestInt));

//toUDINT
  nTestUDInt.setValue(nTestDInt0);
  BOOST_CHECK_EQUAL(static_cast<CIEC_UDINT::TValueType>(nTestUDInt), 0U);
  nTestUDInt.setValue(nTestDInt1);
  BOOST_CHECK_EQUAL(static_cast<CIEC_UDINT::TValueType>(nTestUDInt), 1U);
  nTestUDInt.setValue(nTestDInt32768);
  BOOST_CHECK_EQUAL(static_cast<CIEC_UDINT::TValueType>(nTestUDInt), 32768U);
  nTestUDInt.setValue(nTestDInt2147483647);
  BOOST_CHECK_EQUAL(static_cast<CIEC_UDINT::TValueType>(nTestUDInt), 2147483647U);

//toLINT
  nTestLInt.setValue(nTestDInt0);
  BOOST_CHECK_EQUAL(static_cast<CIEC_LINT::TValueType>(nTestLInt), 0LL);
  nTestLInt.setValue(nTestDInt1);
  BOOST_CHECK_EQUAL(static_cast<CIEC_LINT::TValueType>(nTestLInt), 1LL);
  nTestLInt.setValue(nTestDInt32768);
  BOOST_CHECK_EQUAL(static_cast<CIEC_LINT::TValueType>(nTestLInt), 32768LL);
  nTestLInt.setValue(nTestDInt2147483647);
  BOOST_CHECK_EQUAL(static_cast<CIEC_LINT::TValueType>(nTestLInt), 2147483647LL);
  nTestLInt.setValue(nTestDInt_1);
  BOOST_CHECK_EQUAL(static_cast<CIEC_LINT::TValueType>(nTestLInt), -1LL);

  // toULINT
  nTestULInt.setValue(nTestDInt0);
  BOOST_CHECK_EQUAL(static_cast<CIEC_ULINT::TValueType>(nTestULInt), 0ULL);
  nTestULInt.setValue(nTestDInt1);
  BOOST_CHECK_EQUAL(static_cast<CIEC_ULINT::TValueType>(nTestULInt), 1ULL);
  nTestULInt.setValue(nTestDInt32768);
  BOOST_CHECK_EQUAL(static_cast<CIEC_ULINT::TValueType>(nTestULInt), 32768ULL);
  nTestULInt.setValue(nTestDInt2147483647);
  BOOST_CHECK_EQUAL(static_cast<CIEC_ULINT::TValueType>(nTestULInt), 2147483647ULL);
}

BOOST_AUTO_TEST_CASE(CASTS_DINT_to_REAL)
{
  CIEC_DINT nTestDInt0;
  CIEC_DINT nTestDInt1;
  CIEC_DINT nTestDInt32768;
  CIEC_DINT nTestDInt2147483647;

  CIEC_REAL nTestReal;

//check initial values
  BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(nTestDInt0), 0);
  BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(nTestDInt1), 0);
  BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(nTestDInt32768), 0);
  BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(nTestDInt2147483647), 0);

  nTestDInt0 = CIEC_DINT(0);
  nTestDInt1 = CIEC_DINT(1);
  nTestDInt32768 = CIEC_DINT(32768);
  nTestDInt2147483647 = CIEC_DINT(2147483647);

  BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(nTestDInt0), 0);
  BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(nTestDInt1), 1);
  BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(nTestDInt32768), 32768);
  BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(nTestDInt2147483647), 2147483647);

  // toREAL
  nTestReal.setValue(nTestDInt0);
  BOOST_CHECK_EQUAL(0.0f, nTestReal);
  nTestReal.setValue(nTestDInt1);
  BOOST_CHECK_EQUAL(1.0f, nTestReal); 
  nTestReal.setValue(nTestDInt32768);
  BOOST_CHECK_EQUAL(32768.0f, nTestReal); 
  nTestReal.setValue(nTestDInt2147483647);
  BOOST_CHECK_EQUAL(2147483647.0f, nTestReal); 

  CIEC_LREAL nTestLReal;

//toLREAL
  nTestLReal.setValue(nTestDInt0);
  BOOST_CHECK_EQUAL(0.0, nTestLReal);
  nTestLReal.setValue(nTestDInt1);
  BOOST_CHECK_EQUAL(1.0, nTestLReal); 
  nTestLReal.setValue(nTestDInt32768);
  BOOST_CHECK_EQUAL(32768.0, nTestLReal); 
  nTestLReal.setValue(nTestDInt2147483647);
  BOOST_CHECK_EQUAL(2147483647.0, nTestLReal); 
}

BOOST_AUTO_TEST_CASE(DINT_Castable_test)
{
  bool bUp, bDown;

  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_DINT, CIEC_ANY::e_BOOL, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(bDown);

  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_DINT, CIEC_ANY::e_SINT, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_DINT, CIEC_ANY::e_INT, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_DINT, CIEC_ANY::e_DINT, bUp, bDown)); BOOST_CHECK(bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_DINT, CIEC_ANY::e_LINT, bUp, bDown)); BOOST_CHECK(bUp); BOOST_CHECK(! bDown);

  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_DINT, CIEC_ANY::e_USINT, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_DINT, CIEC_ANY::e_UINT, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_DINT, CIEC_ANY::e_UDINT, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_DINT, CIEC_ANY::e_ULINT, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(bDown);

  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_DINT, CIEC_ANY::e_BYTE, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_DINT, CIEC_ANY::e_WORD, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_DINT, CIEC_ANY::e_DWORD, bUp, bDown)); BOOST_CHECK(bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_DINT, CIEC_ANY::e_LWORD, bUp, bDown)); BOOST_CHECK(bUp); BOOST_CHECK(! bDown);

  BOOST_CHECK(!CIEC_ANY::isCastable(CIEC_ANY::e_DINT, CIEC_ANY::e_DATE, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(!CIEC_ANY::isCastable(CIEC_ANY::e_DINT, CIEC_ANY::e_TIME_OF_DAY, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(!CIEC_ANY::isCastable(CIEC_ANY::e_DINT, CIEC_ANY::e_DATE_AND_TIME, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(!CIEC_ANY::isCastable(CIEC_ANY::e_DINT, CIEC_ANY::e_TIME, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(! bDown);

  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_DINT, CIEC_ANY::e_REAL, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_DINT, CIEC_ANY::e_LREAL, bUp, bDown)); BOOST_CHECK(bUp); BOOST_CHECK(! bDown);

  BOOST_CHECK(!CIEC_ANY::isCastable(CIEC_ANY::e_DINT, CIEC_ANY::e_STRING, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(!CIEC_ANY::isCastable(CIEC_ANY::e_DINT, CIEC_ANY::e_WSTRING, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(! bDown);
}

BOOST_AUTO_TEST_CASE(Explict_cast_operator_to_DINT) {
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

  CIEC_DINT nResult(0);

  nResult = CIEC_ANY::cast<CIEC_DINT>(nUsint);
  BOOST_TEST(static_cast<CIEC_DINT::TValueType>(nResult) == 2);
  nResult = CIEC_ANY::cast<CIEC_DINT>(nUint);
  BOOST_TEST(static_cast<CIEC_DINT::TValueType>(nResult) == 3);
  nResult = CIEC_ANY::cast<CIEC_DINT>(nUdint);
  BOOST_TEST(static_cast<CIEC_DINT::TValueType>(nResult) == 4);
  nResult = CIEC_ANY::cast<CIEC_DINT>(nUlint);
  BOOST_TEST(static_cast<CIEC_DINT::TValueType>(nResult) == 5);
  nResult = CIEC_ANY::cast<CIEC_DINT>(nSint);
  BOOST_TEST(static_cast<CIEC_DINT::TValueType>(nResult) == 6);
  nResult = CIEC_ANY::cast<CIEC_DINT>(nInt);
  BOOST_TEST(static_cast<CIEC_DINT::TValueType>(nResult) == 7);
  nResult = CIEC_ANY::cast<CIEC_DINT>(nDint);
  BOOST_TEST(static_cast<CIEC_DINT::TValueType>(nResult) == 8);
  nResult = CIEC_ANY::cast<CIEC_DINT>(nLint);
  BOOST_TEST(static_cast<CIEC_DINT::TValueType>(nResult) == 9);
  nResult = CIEC_ANY::cast<CIEC_DINT>(nReal);
  BOOST_TEST(static_cast<CIEC_DINT::TValueType>(nResult) == 10);
  nResult = CIEC_ANY::cast<CIEC_DINT>(nLreal);
  BOOST_TEST(static_cast<CIEC_DINT::TValueType>(nResult) == 11);
  nResult = CIEC_ANY::cast<CIEC_DINT>(nByte);
  BOOST_TEST(static_cast<CIEC_DINT::TValueType>(nResult) == 12);
  nResult = CIEC_ANY::cast<CIEC_DINT>(nWord);
  BOOST_TEST(static_cast<CIEC_DINT::TValueType>(nResult) == 13);
  nResult = CIEC_ANY::cast<CIEC_DINT>(nDword);
  BOOST_TEST(static_cast<CIEC_DINT::TValueType>(nResult) == 14);
  nResult = CIEC_ANY::cast<CIEC_DINT>(nLword);
  BOOST_TEST(static_cast<CIEC_DINT::TValueType>(nResult) == 15);
  nResult = CIEC_ANY::cast<CIEC_DINT>(bBool);
  BOOST_TEST(static_cast<CIEC_DINT::TValueType>(nResult) == true);
}

BOOST_AUTO_TEST_SUITE_END()
