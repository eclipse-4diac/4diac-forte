/*******************************************************************************
 * Copyright (c) 2011 - 2012 ACIN, nxtControl
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Ingo Hegny, Alois Zoitl, Stanislav Meduna - initial API and implementation and/or initial documentation
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


#include "../../../src/core/datatypes/forte_lword.h"
#include "../../../src/core/datatypes/forte_lint.h"
#include "../../../src/core/datatypes/forte_ulint.h"
#include "../../../src/core/datatypes/forte_lreal.h"

using namespace boost::unit_test;

BOOST_AUTO_TEST_SUITE(CIEC_BOOL_casts_function_test)

BOOST_AUTO_TEST_CASE(CASTS_BOOL_to_BITS)
{
  CIEC_BOOL bTestBool_true;
  CIEC_BOOL bTestBool_false;

  CIEC_BYTE nTestByte;
  CIEC_WORD nTestWord;
  CIEC_DWORD nTestDWord;
  CIEC_LWORD nTestLWord;

  //initial value must be 0 = false
  BOOST_CHECK_EQUAL(bTestBool_false, false);
  BOOST_CHECK_EQUAL(bTestBool_true, false);

  bTestBool_false = CIEC_BOOL(false);
  bTestBool_true = CIEC_BOOL(true);

  BOOST_CHECK_EQUAL(bTestBool_true, true);

//toByte
  nTestByte.setValue(bTestBool_false);
  BOOST_CHECK_EQUAL(nTestByte, 0U);
  nTestByte.setValue(bTestBool_true);
  BOOST_CHECK_EQUAL(nTestByte, 1U);

//toWord
  nTestWord.setValue(bTestBool_false);
  BOOST_CHECK_EQUAL(nTestWord, 0U);
  nTestWord.setValue(bTestBool_true);
  BOOST_CHECK_EQUAL(nTestWord, 1U);

//toDWord
  nTestDWord.setValue(bTestBool_false);
  BOOST_CHECK_EQUAL(nTestDWord, 0UL);
  nTestDWord.setValue(bTestBool_true);
  BOOST_CHECK_EQUAL(nTestDWord, 1UL);


//toDWord
  nTestLWord.setValue(bTestBool_false);
  BOOST_CHECK_EQUAL(nTestLWord, 0ULL);
  nTestLWord.setValue(bTestBool_true);
  BOOST_CHECK_EQUAL(nTestLWord, 1ULL);
}

BOOST_AUTO_TEST_CASE(CASTS_BOOL_to_INTS)
{
  CIEC_BOOL bTestBool_true;
  CIEC_BOOL bTestBool_false;

  CIEC_SINT nTestSInt;
  CIEC_USINT nTestUSInt;
  CIEC_INT nTestInt;
  CIEC_UINT nTestUInt;
  CIEC_DINT nTestDInt;
  CIEC_UDINT nTestUDInt;
  CIEC_LINT nTestLInt;
  CIEC_ULINT nTestULInt;

  //initial value must be 0 = false
  BOOST_CHECK_EQUAL(bTestBool_false, false);
  BOOST_CHECK_EQUAL(bTestBool_true, false);

  bTestBool_false = CIEC_BOOL(false);
  bTestBool_true = CIEC_BOOL(true);

  BOOST_CHECK_EQUAL(bTestBool_true, true);

//toSINT
  nTestSInt.setValue(bTestBool_false);
  BOOST_TEST(0 == static_cast<CIEC_SINT::TValueType>(nTestSInt));
  nTestSInt.setValue(bTestBool_true);
  BOOST_TEST(1 == static_cast<CIEC_SINT::TValueType>(nTestSInt));

//toUSINT
  nTestUSInt.setValue(bTestBool_false);
  BOOST_CHECK_EQUAL(static_cast<CIEC_USINT::TValueType>(nTestUSInt), 0U);
  nTestUSInt.setValue(bTestBool_true);
  BOOST_CHECK_EQUAL(static_cast<CIEC_USINT::TValueType>(nTestUSInt), 1U);

//toINT
  nTestInt.setValue(bTestBool_false);
  BOOST_TEST(0 == static_cast<CIEC_INT::TValueType>(nTestInt));
  nTestInt.setValue(bTestBool_true);
  BOOST_TEST(1 == static_cast<CIEC_INT::TValueType>(nTestInt));

//toUINT
  nTestUInt.setValue(bTestBool_false);
  BOOST_CHECK_EQUAL(static_cast<CIEC_UINT::TValueType>(nTestUInt), 0U);
  nTestUInt.setValue(bTestBool_true);
  BOOST_CHECK_EQUAL(static_cast<CIEC_UINT::TValueType>(nTestUInt), 1U);

// toDINT
  nTestDInt.setValue(bTestBool_false);
  BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(nTestDInt), 0);
  nTestDInt.setValue(bTestBool_true);
  BOOST_CHECK_EQUAL(static_cast<CIEC_DINT::TValueType>(nTestDInt), 1);

//toUDINT
  nTestUDInt.setValue(bTestBool_false);
  BOOST_CHECK_EQUAL(static_cast<CIEC_UDINT::TValueType>(nTestUDInt), 0UL);
  nTestUDInt.setValue(bTestBool_true);
  BOOST_CHECK_EQUAL(static_cast<CIEC_UDINT::TValueType>(nTestUDInt), 1UL);

//toLINT
  nTestLInt.setValue(bTestBool_false);
  BOOST_CHECK_EQUAL(static_cast<CIEC_LINT::TValueType>(nTestLInt), 0LL);
  nTestLInt.setValue(bTestBool_true);
  BOOST_CHECK_EQUAL(static_cast<CIEC_LINT::TValueType>(nTestLInt), 1LL);

// toULINT
  nTestULInt.setValue(bTestBool_false);
  BOOST_CHECK_EQUAL(static_cast<CIEC_ULINT::TValueType>(nTestULInt), 0ULL);
  nTestULInt.setValue(bTestBool_true);
  BOOST_CHECK_EQUAL(static_cast<CIEC_ULINT::TValueType>(nTestULInt), 1ULL);
}

BOOST_AUTO_TEST_CASE(CASTS_BOOL_to_REAL)
{
  CIEC_BOOL bTestBool_true;
  CIEC_BOOL bTestBool_false;
  CIEC_REAL nTestReal;

  //initial value must be 0 = false
  BOOST_CHECK_EQUAL(bTestBool_false, false);
  BOOST_CHECK_EQUAL(bTestBool_true, false);

  bTestBool_false = CIEC_BOOL(false);
  bTestBool_true = CIEC_BOOL(true);

  BOOST_CHECK_EQUAL(bTestBool_true, true);

//toREAL
  nTestReal.setValue(bTestBool_false);
  BOOST_CHECK_EQUAL(0.0f, nTestReal);
  nTestReal.setValue(bTestBool_true);
  BOOST_CHECK_EQUAL(1.0f, nTestReal);

  CIEC_LREAL nTestLReal;

//toLREAL
  nTestLReal.setValue(bTestBool_false);
  BOOST_CHECK_EQUAL(0.0, nTestLReal);
  nTestLReal.setValue(bTestBool_true);
  BOOST_CHECK_EQUAL(1.0, nTestLReal);
}

BOOST_AUTO_TEST_CASE(BOOL_Castable_test)
{
  bool bUp, bDown;

  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_BOOL, CIEC_ANY::e_BOOL, bUp, bDown)); BOOST_CHECK(bUp); BOOST_CHECK(! bDown);

  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_BOOL, CIEC_ANY::e_SINT, bUp, bDown)); BOOST_CHECK(bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_BOOL, CIEC_ANY::e_INT, bUp, bDown)); BOOST_CHECK(bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_BOOL, CIEC_ANY::e_DINT, bUp, bDown)); BOOST_CHECK(bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_BOOL, CIEC_ANY::e_LINT, bUp, bDown)); BOOST_CHECK(bUp); BOOST_CHECK(! bDown);

  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_BOOL, CIEC_ANY::e_USINT, bUp, bDown)); BOOST_CHECK(bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_BOOL, CIEC_ANY::e_UINT, bUp, bDown)); BOOST_CHECK(bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_BOOL, CIEC_ANY::e_UDINT, bUp, bDown)); BOOST_CHECK(bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_BOOL, CIEC_ANY::e_ULINT, bUp, bDown)); BOOST_CHECK(bUp); BOOST_CHECK(! bDown);

  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_BOOL, CIEC_ANY::e_BYTE, bUp, bDown)); BOOST_CHECK(bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_BOOL, CIEC_ANY::e_WORD, bUp, bDown)); BOOST_CHECK(bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_BOOL, CIEC_ANY::e_DWORD, bUp, bDown)); BOOST_CHECK(bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_BOOL, CIEC_ANY::e_LWORD, bUp, bDown)); BOOST_CHECK(bUp); BOOST_CHECK(! bDown);

  BOOST_CHECK(!CIEC_ANY::isCastable(CIEC_ANY::e_BOOL, CIEC_ANY::e_DATE, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(!CIEC_ANY::isCastable(CIEC_ANY::e_BOOL, CIEC_ANY::e_TIME_OF_DAY, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(!CIEC_ANY::isCastable(CIEC_ANY::e_BOOL, CIEC_ANY::e_DATE_AND_TIME, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(!CIEC_ANY::isCastable(CIEC_ANY::e_BOOL, CIEC_ANY::e_TIME, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(! bDown);

  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_BOOL, CIEC_ANY::e_REAL, bUp, bDown)); BOOST_CHECK(bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_BOOL, CIEC_ANY::e_LREAL, bUp, bDown)); BOOST_CHECK(bUp); BOOST_CHECK(! bDown);

  BOOST_CHECK(!CIEC_ANY::isCastable(CIEC_ANY::e_BOOL, CIEC_ANY::e_STRING, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(!CIEC_ANY::isCastable(CIEC_ANY::e_BOOL, CIEC_ANY::e_WSTRING, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(! bDown);
}

BOOST_AUTO_TEST_SUITE_END()
