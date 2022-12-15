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

BOOST_AUTO_TEST_SUITE(CIEC_INT_casts_function_test)

BOOST_AUTO_TEST_CASE(CASTS_INT_to_BOOL) {
  CIEC_INT nTestInt0;
  CIEC_INT nTestInt1;
  CIEC_INT nTestInt256;
  CIEC_INT nTestInt32767;

  CIEC_BOOL bTestBool;

//check initial values
  BOOST_TEST(0 == static_cast<CIEC_INT::TValueType>(nTestInt0));
  BOOST_TEST(0 == static_cast<CIEC_INT::TValueType>(nTestInt1));
  BOOST_TEST(0 == static_cast<CIEC_INT::TValueType>(nTestInt256));
  BOOST_TEST(0 == static_cast<CIEC_INT::TValueType>(nTestInt32767));

  nTestInt0 = CIEC_INT(0);
  nTestInt1 = CIEC_INT(1);
  nTestInt256 = CIEC_INT(256);
  nTestInt32767 = CIEC_INT(32767);

  BOOST_TEST(0 == static_cast<CIEC_INT::TValueType>(nTestInt0));
  BOOST_TEST(1 == static_cast<CIEC_INT::TValueType>(nTestInt1));
  BOOST_TEST(256 == static_cast<CIEC_INT::TValueType>(nTestInt256));
  BOOST_TEST(32767 == static_cast<CIEC_INT::TValueType>(nTestInt32767));

//checks
  bTestBool.setValue(nTestInt0);
  BOOST_CHECK_EQUAL(false, bTestBool);
  bTestBool.setValue(nTestInt1);
  BOOST_CHECK_EQUAL(true, bTestBool);
  bTestBool.setValue(nTestInt256);
  BOOST_CHECK_EQUAL(true, bTestBool);
  bTestBool.setValue(nTestInt32767);
  BOOST_CHECK_EQUAL(true, bTestBool);
}

BOOST_AUTO_TEST_CASE(CASTS_INT_to_BITDTs) {
  CIEC_INT nTestInt0;
  CIEC_INT nTestInt1;
  CIEC_INT nTestInt256;
  CIEC_INT nTestInt32767;

  CIEC_BYTE nTestByte;
  CIEC_WORD nTestWord;
  CIEC_DWORD nTestDWord;
  CIEC_LWORD nTestLWord;

//check initial values
  BOOST_TEST(0 == static_cast<CIEC_INT::TValueType>(nTestInt0));
  BOOST_TEST(0 == static_cast<CIEC_INT::TValueType>(nTestInt1));
  BOOST_TEST(0 == static_cast<CIEC_INT::TValueType>(nTestInt256));
  BOOST_TEST(0 == static_cast<CIEC_INT::TValueType>(nTestInt32767));

  nTestInt0 = CIEC_INT(0);
  nTestInt1 = CIEC_INT(1);
  nTestInt256 = CIEC_INT(256);
  nTestInt32767 = CIEC_INT(32767);

  BOOST_TEST(0 == static_cast<CIEC_INT::TValueType>(nTestInt0));
  BOOST_TEST(1 == static_cast<CIEC_INT::TValueType>(nTestInt1));
  BOOST_TEST(256 == static_cast<CIEC_INT::TValueType>(nTestInt256));
  BOOST_TEST(32767 == static_cast<CIEC_INT::TValueType>(nTestInt32767));

  // toByte
  nTestByte.setValue(nTestInt0);
  BOOST_CHECK_EQUAL(nTestByte, 0U);
  nTestByte.setValue(nTestInt1);
  BOOST_CHECK_EQUAL(nTestByte, 1U);
  nTestByte.setValue(nTestInt256);
  BOOST_CHECK_EQUAL(nTestByte, 0U);
  nTestByte.setValue(nTestInt32767);
  BOOST_CHECK_EQUAL(nTestByte, 255U);

//toWord
  nTestWord.setValue(nTestInt0);
  BOOST_CHECK_EQUAL(nTestWord, 0U);
  nTestWord.setValue(nTestInt1);
  BOOST_CHECK_EQUAL(nTestWord, 1U);
  nTestWord.setValue(nTestInt256);
  BOOST_CHECK_EQUAL(nTestWord, 256U);
  nTestWord.setValue(nTestInt32767);
  BOOST_CHECK_EQUAL(nTestWord, 32767U);

//toDWord
  nTestDWord.setValue(nTestInt0);
  BOOST_CHECK_EQUAL(nTestDWord, 0UL);
  nTestDWord.setValue(nTestInt1);
  BOOST_CHECK_EQUAL(nTestDWord, 1UL);
  nTestDWord.setValue(nTestInt256);
  BOOST_CHECK_EQUAL(nTestDWord, 256UL);
  nTestDWord.setValue(nTestInt32767);
  BOOST_CHECK_EQUAL(nTestDWord, 32767UL);

//toLWord
  nTestLWord.setValue(nTestInt0);
  BOOST_CHECK_EQUAL(nTestLWord, 0ULL);
  nTestLWord.setValue(nTestInt1);
  BOOST_CHECK_EQUAL(nTestLWord, 1ULL);
  nTestLWord.setValue(nTestInt256);
  BOOST_CHECK_EQUAL(nTestLWord, 256ULL);
  nTestLWord.setValue(nTestInt32767);
  BOOST_CHECK_EQUAL(nTestLWord, 32767ULL);
}

BOOST_AUTO_TEST_CASE(CASTS_INT_to_INTS) {
  CIEC_INT nTestInt0;
  CIEC_INT nTestInt1;
  CIEC_INT nTestInt256;
  CIEC_INT nTestInt32767;
  CIEC_INT nTestInt_1;

  CIEC_USINT nTestUSInt;
  CIEC_SINT nTestSInt;
  CIEC_UINT nTestUInt;
  CIEC_DINT nTestDInt;
  CIEC_UDINT nTestUDInt;
  CIEC_LINT nTestLInt;
  CIEC_ULINT nTestULInt;

  //check initial values
  BOOST_TEST(0 == static_cast<CIEC_INT::TValueType>(nTestInt0));
  BOOST_TEST(0 == static_cast<CIEC_INT::TValueType>(nTestInt1));
  BOOST_TEST(0 == static_cast<CIEC_INT::TValueType>(nTestInt256));
  BOOST_TEST(0 == static_cast<CIEC_INT::TValueType>(nTestInt32767));
  BOOST_TEST(0 == static_cast<CIEC_INT::TValueType>(nTestInt_1));

  nTestInt0 = CIEC_INT(0);
  nTestInt1 = CIEC_INT(1);
  nTestInt256 = CIEC_INT(256);
  nTestInt32767 = CIEC_INT(32767);
  nTestInt_1 = CIEC_INT(-1);

  BOOST_TEST(0 == static_cast<CIEC_INT::TValueType>(nTestInt0));
  BOOST_TEST(1 == static_cast<CIEC_INT::TValueType>(nTestInt1));
  BOOST_TEST(256 == static_cast<CIEC_INT::TValueType>(nTestInt256));
  BOOST_TEST(32767 == static_cast<CIEC_INT::TValueType>(nTestInt32767));
  BOOST_TEST(-1 == static_cast<CIEC_INT::TValueType>(nTestInt_1));

  // toUSINT
  nTestUSInt.setValue(nTestInt0);
  BOOST_CHECK_EQUAL(static_cast<CIEC_USINT::TValueType>(nTestUSInt), 0U);
  nTestUSInt.setValue(nTestInt1);
  BOOST_CHECK_EQUAL(static_cast<CIEC_USINT::TValueType>(nTestUSInt), 1U);
  nTestUSInt.setValue(nTestInt256);
  BOOST_CHECK_EQUAL(static_cast<CIEC_USINT::TValueType>(nTestUSInt), 0U);
  nTestUSInt.setValue(nTestInt32767);
  BOOST_CHECK_EQUAL(static_cast<CIEC_USINT::TValueType>(nTestUSInt), 255U);
  nTestUSInt.setValue(nTestInt_1);
  BOOST_CHECK_EQUAL(static_cast<CIEC_USINT::TValueType>(nTestUSInt), 255U);

//toSINT
  nTestSInt.setValue(nTestInt0);
  BOOST_TEST(0 == static_cast<CIEC_SINT::TValueType>(nTestSInt));
  nTestSInt.setValue(nTestInt1);
  BOOST_TEST(1 == static_cast<CIEC_SINT::TValueType>(nTestSInt));
  nTestSInt.setValue(nTestInt256);
  BOOST_TEST(0 == static_cast<CIEC_SINT::TValueType>(nTestSInt));
  nTestSInt.setValue(nTestInt32767);
  BOOST_TEST(-1 == static_cast<CIEC_SINT::TValueType>(nTestSInt));

//toUINT
  nTestUInt.setValue(nTestInt0);
  BOOST_CHECK_EQUAL(static_cast<CIEC_UINT::TValueType>(nTestUInt), 0U);
  nTestUInt.setValue(nTestInt1);
  BOOST_CHECK_EQUAL(static_cast<CIEC_UINT::TValueType>(nTestUInt), 1U);
  nTestUInt.setValue(nTestInt256);
  BOOST_CHECK_EQUAL(static_cast<CIEC_UINT::TValueType>(nTestUInt), 256U);
  nTestUInt.setValue(nTestInt32767);
  BOOST_CHECK_EQUAL(static_cast<CIEC_UINT::TValueType>(nTestUInt), 32767U);
  nTestUInt.setValue(nTestInt_1);
  BOOST_CHECK_EQUAL(static_cast<CIEC_UINT::TValueType>(nTestUInt), 65535U);

//toDINT
  nTestDInt.setValue(nTestInt0);
  BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(nTestDInt), 0);
  nTestDInt.setValue(nTestInt1);
  BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(nTestDInt), 1);
  nTestDInt.setValue(nTestInt256);
  BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(nTestDInt), 256);
  nTestDInt.setValue(nTestInt32767);
  BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(nTestDInt), 32767);
  nTestDInt.setValue(nTestInt_1);
  BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(nTestDInt), -1);

//toUDINT
  nTestUDInt.setValue(nTestInt0);
  BOOST_CHECK_EQUAL(static_cast<CIEC_UDINT::TValueType>(nTestUDInt), 0UL);
  nTestUDInt.setValue(nTestInt1);
  BOOST_CHECK_EQUAL(static_cast<CIEC_UDINT::TValueType>(nTestUDInt), 1UL);
  nTestUDInt.setValue(nTestInt256);
  BOOST_CHECK_EQUAL(static_cast<CIEC_UDINT::TValueType>(nTestUDInt), 256UL);
  nTestUDInt.setValue(nTestInt32767);
  BOOST_CHECK_EQUAL(static_cast<CIEC_UDINT::TValueType>(nTestUDInt), 32767UL);

//toLINT
  nTestLInt.setValue(nTestInt0);
  BOOST_CHECK_EQUAL(static_cast<CIEC_LINT::TValueType>(nTestLInt), 0LL);
  nTestLInt.setValue(nTestInt1);
  BOOST_CHECK_EQUAL(static_cast<CIEC_LINT::TValueType>(nTestLInt), 1LL);
  nTestLInt.setValue(nTestInt256);
  BOOST_CHECK_EQUAL(static_cast<CIEC_LINT::TValueType>(nTestLInt), 256LL);
  nTestLInt.setValue(nTestInt32767);
  BOOST_CHECK_EQUAL(static_cast<CIEC_LINT::TValueType>(nTestLInt), 32767LL);
  nTestLInt.setValue(nTestInt_1);
  BOOST_CHECK_EQUAL(static_cast<CIEC_LINT::TValueType>(nTestLInt), -1LL);

  // toULINT
  nTestULInt.setValue(nTestInt0);
  BOOST_CHECK_EQUAL(static_cast<CIEC_ULINT::TValueType>(nTestULInt), 0ULL);
  nTestULInt.setValue(nTestInt1);
  BOOST_CHECK_EQUAL(static_cast<CIEC_ULINT::TValueType>(nTestULInt), 1ULL);
  nTestULInt.setValue(nTestInt256);
  BOOST_CHECK_EQUAL(static_cast<CIEC_ULINT::TValueType>(nTestULInt), 256ULL);
  nTestULInt.setValue(nTestInt32767);
  BOOST_CHECK_EQUAL(static_cast<CIEC_ULINT::TValueType>(nTestULInt), 32767ULL);
}

BOOST_AUTO_TEST_CASE(CASTS_INT_to_REAL) {
  CIEC_INT nTestInt0;
  CIEC_INT nTestInt1;
  CIEC_INT nTestInt256;
  CIEC_INT nTestInt32767;

  CIEC_REAL nTestReal;

//check initial values
  BOOST_TEST(0 == static_cast<CIEC_INT::TValueType>(nTestInt0));
  BOOST_TEST(0 == static_cast<CIEC_INT::TValueType>(nTestInt1));
  BOOST_TEST(0 == static_cast<CIEC_INT::TValueType>(nTestInt256));
  BOOST_TEST(0 == static_cast<CIEC_INT::TValueType>(nTestInt32767));

  nTestInt0 = CIEC_INT(0);
  nTestInt1 = CIEC_INT(1);
  nTestInt256 = CIEC_INT(256);
  nTestInt32767 = CIEC_INT(32767);

  BOOST_TEST(0 == static_cast<CIEC_INT::TValueType>(nTestInt0));
  BOOST_TEST(1 == static_cast<CIEC_INT::TValueType>(nTestInt1));
  BOOST_TEST(256 == static_cast<CIEC_INT::TValueType>(nTestInt256));
  BOOST_TEST(32767 == static_cast<CIEC_INT::TValueType>(nTestInt32767));

  // toREAL
  nTestReal.setValue(nTestInt0);
  BOOST_CHECK_EQUAL(0.0f, nTestReal);
  nTestReal.setValue(nTestInt1);
  BOOST_CHECK_EQUAL(1.0f, nTestReal); 
  nTestReal.setValue(nTestInt256);
  BOOST_CHECK_EQUAL(256.0f, nTestReal); 
  nTestReal.setValue(nTestInt32767);
  BOOST_CHECK_EQUAL(32767.0f, nTestReal); 

  CIEC_LREAL nTestLReal;

//toLREAL
  nTestLReal.setValue(nTestInt0);
  BOOST_CHECK_EQUAL(0.0, nTestLReal);
  nTestLReal.setValue(nTestInt1);
  BOOST_CHECK_EQUAL(1.0, nTestLReal); 
  nTestLReal.setValue(nTestInt256);
  BOOST_CHECK_EQUAL(256.0, nTestLReal); 
  nTestLReal.setValue(nTestInt32767);
  BOOST_CHECK_EQUAL(32767.0, nTestLReal); 
}

BOOST_AUTO_TEST_CASE(INT_Castable_test) {
  bool bUp, bDown;

  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_INT, CIEC_ANY::e_BOOL, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(bDown);

  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_INT, CIEC_ANY::e_SINT, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_INT, CIEC_ANY::e_INT, bUp, bDown)); BOOST_CHECK(bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_INT, CIEC_ANY::e_DINT, bUp, bDown)); BOOST_CHECK(bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_INT, CIEC_ANY::e_LINT, bUp, bDown)); BOOST_CHECK(bUp); BOOST_CHECK(! bDown);

  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_INT, CIEC_ANY::e_USINT, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_INT, CIEC_ANY::e_UINT, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_INT, CIEC_ANY::e_UDINT, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_INT, CIEC_ANY::e_ULINT, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(bDown);

  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_INT, CIEC_ANY::e_BYTE, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_INT, CIEC_ANY::e_WORD, bUp, bDown)); BOOST_CHECK(bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_INT, CIEC_ANY::e_DWORD, bUp, bDown)); BOOST_CHECK(bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_INT, CIEC_ANY::e_LWORD, bUp, bDown)); BOOST_CHECK(bUp); BOOST_CHECK(! bDown);

  BOOST_CHECK(!CIEC_ANY::isCastable(CIEC_ANY::e_INT, CIEC_ANY::e_DATE, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(!CIEC_ANY::isCastable(CIEC_ANY::e_INT, CIEC_ANY::e_TIME_OF_DAY, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(!CIEC_ANY::isCastable(CIEC_ANY::e_INT, CIEC_ANY::e_DATE_AND_TIME, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(!CIEC_ANY::isCastable(CIEC_ANY::e_INT, CIEC_ANY::e_TIME, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(! bDown);

  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_INT, CIEC_ANY::e_REAL, bUp, bDown)); BOOST_CHECK(bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_INT, CIEC_ANY::e_LREAL, bUp, bDown)); BOOST_CHECK(bUp); BOOST_CHECK(! bDown);

  BOOST_CHECK(!CIEC_ANY::isCastable(CIEC_ANY::e_INT, CIEC_ANY::e_STRING, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(!CIEC_ANY::isCastable(CIEC_ANY::e_INT, CIEC_ANY::e_WSTRING, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(! bDown);
}

BOOST_AUTO_TEST_CASE(Explict_cast_operator_to_INT) {
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

  CIEC_INT nResult(0);

  nResult = CIEC_ANY::cast<CIEC_INT>(nUsint);
  BOOST_TEST(static_cast<CIEC_INT::TValueType>(nResult) == 2);
  nResult = CIEC_ANY::cast<CIEC_INT>(nUint);
  BOOST_TEST(static_cast<CIEC_INT::TValueType>(nResult) == 3);
  nResult = CIEC_ANY::cast<CIEC_INT>(nUdint);
  BOOST_TEST(static_cast<CIEC_INT::TValueType>(nResult) == 4);
  nResult = CIEC_ANY::cast<CIEC_INT>(nUlint);
  BOOST_TEST(static_cast<CIEC_INT::TValueType>(nResult) == 5);
  nResult = CIEC_ANY::cast<CIEC_INT>(nSint);
  BOOST_TEST(static_cast<CIEC_INT::TValueType>(nResult) == 6);
  nResult = CIEC_ANY::cast<CIEC_INT>(nInt);
  BOOST_TEST(static_cast<CIEC_INT::TValueType>(nResult) == 7);
  nResult = CIEC_ANY::cast<CIEC_INT>(nDint);
  BOOST_TEST(static_cast<CIEC_INT::TValueType>(nResult) == 8);
  nResult = CIEC_ANY::cast<CIEC_INT>(nLint);
  BOOST_TEST(static_cast<CIEC_INT::TValueType>(nResult) == 9);
  nResult = CIEC_ANY::cast<CIEC_INT>(nReal);
  BOOST_TEST(static_cast<CIEC_INT::TValueType>(nResult) == 10);
  nResult = CIEC_ANY::cast<CIEC_INT>(nLreal);
  BOOST_TEST(static_cast<CIEC_INT::TValueType>(nResult) == 11);
  nResult = CIEC_ANY::cast<CIEC_INT>(nByte);
  BOOST_TEST(static_cast<CIEC_INT::TValueType>(nResult) == 12);
  nResult = CIEC_ANY::cast<CIEC_INT>(nWord);
  BOOST_TEST(static_cast<CIEC_INT::TValueType>(nResult) == 13);
  nResult = CIEC_ANY::cast<CIEC_INT>(nDword);
  BOOST_TEST(static_cast<CIEC_INT::TValueType>(nResult) == 14);
  nResult = CIEC_ANY::cast<CIEC_INT>(nLword);
  BOOST_TEST(static_cast<CIEC_INT::TValueType>(nResult) == 15);
  nResult = CIEC_ANY::cast<CIEC_INT>(bBool);
  BOOST_TEST(static_cast<CIEC_INT::TValueType>(nResult) == true);
}

BOOST_AUTO_TEST_SUITE_END()
