/*******************************************************************************
 * Copyright (c) 2011 - 2012 ACIN, nxtControl
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Ingo Hegny, Alois Zoitl, Stanislav Meduna - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include <boost/test/unit_test.hpp>

#ifdef FORTE_USE_REAL_DATATYPE
#include <boost/test/floating_point_comparison.hpp>

#include "../../../src/core/datatypes/forte_real.h"
#endif

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

#ifdef FORTE_USE_64BIT_DATATYPES
#include "../../../src/core/datatypes/forte_lword.h"
#include "../../../src/core/datatypes/forte_lint.h"
#include "../../../src/core/datatypes/forte_ulint.h"

#ifdef FORTE_USE_LREAL_DATATYPE
#include "../../../src/core/datatypes/forte_lreal.h"
#endif //FORTE_USE_LREAL_DATATYPE
#endif //FORTE_USE_64BIT_DATATYPES
using namespace boost::unit_test;

BOOST_AUTO_TEST_SUITE(CIEC_SINT_casts_function_test)

BOOST_AUTO_TEST_CASE(CASTS_SINT_to_BOOL)
{
  CIEC_SINT nTestSInt0;
  CIEC_SINT nTestSInt1;
  CIEC_SINT nTestSInt45;
  CIEC_SINT nTestSInt127;

  CIEC_BOOL bTestBool;

//check initial values
  BOOST_CHECK_EQUAL(nTestSInt0, 0);
  BOOST_CHECK_EQUAL(nTestSInt1, 0);
  BOOST_CHECK_EQUAL(nTestSInt45, 0);
  BOOST_CHECK_EQUAL(nTestSInt127, 0);

  nTestSInt0=0;
  nTestSInt1=1;
  nTestSInt45=45;
  nTestSInt127=127;

  BOOST_CHECK_EQUAL(nTestSInt0, 0);
  BOOST_CHECK_EQUAL(nTestSInt1, 1);
  BOOST_CHECK_EQUAL(nTestSInt45, 45);
  BOOST_CHECK_EQUAL(nTestSInt127, 127);

//checks
  bTestBool.setValue(nTestSInt0);
  BOOST_CHECK_EQUAL(bTestBool, false);
  bTestBool.setValue(nTestSInt1);
  BOOST_CHECK_EQUAL(bTestBool, true);
  bTestBool.setValue(nTestSInt45);
  BOOST_CHECK_EQUAL(bTestBool, true);
  bTestBool.setValue(nTestSInt127);
  BOOST_CHECK_EQUAL(bTestBool, true);

}

BOOST_AUTO_TEST_CASE(CASTS_SINT_to_BITDTs)
{
  CIEC_SINT nTestSInt0;
  CIEC_SINT nTestSInt1;
  CIEC_SINT nTestSInt45;
  CIEC_SINT nTestSInt127;

  CIEC_BYTE nTestByte;
  CIEC_WORD nTestWord;
  CIEC_DWORD nTestDWord;
#ifdef FORTE_USE_64BIT_DATATYPES
  CIEC_DWORD nTestLWord;
#endif //FORTE_USE_64BIT_DATATYPES
//check initial values
  BOOST_CHECK_EQUAL(nTestSInt0, 0);
  BOOST_CHECK_EQUAL(nTestSInt1, 0);
  BOOST_CHECK_EQUAL(nTestSInt45, 0);
  BOOST_CHECK_EQUAL(nTestSInt127, 0);

  nTestSInt0=0;
  nTestSInt1=1;
  nTestSInt45=45;
  nTestSInt127=127;

  BOOST_CHECK_EQUAL(nTestSInt0, 0);
  BOOST_CHECK_EQUAL(nTestSInt1, 1);
  BOOST_CHECK_EQUAL(nTestSInt45, 45);
  BOOST_CHECK_EQUAL(nTestSInt127, 127);

//toByte
  nTestByte.setValue(nTestSInt0);
  BOOST_CHECK_EQUAL(nTestByte, 0U);
  nTestByte.setValue(nTestSInt1);
  BOOST_CHECK_EQUAL(nTestByte, 1U);
  nTestByte.setValue(nTestSInt45);
  BOOST_CHECK_EQUAL(nTestByte, 45U);
  nTestByte.setValue(nTestSInt127);
  BOOST_CHECK_EQUAL(nTestByte, 127U);

//toWord
  nTestWord.setValue(nTestSInt0);
  BOOST_CHECK_EQUAL(nTestWord, 0U);
  nTestWord.setValue(nTestSInt1);
  BOOST_CHECK_EQUAL(nTestWord, 1U);
  nTestWord.setValue(nTestSInt45);
  BOOST_CHECK_EQUAL(nTestWord, 45U);
  nTestWord.setValue(nTestSInt127);
  BOOST_CHECK_EQUAL(nTestWord, 127U);

//toDWord
  nTestDWord.setValue(nTestSInt0);
  BOOST_CHECK_EQUAL(nTestDWord, 0UL);
  nTestDWord.setValue(nTestSInt1);
  BOOST_CHECK_EQUAL(nTestDWord, 1UL);
  nTestDWord.setValue(nTestSInt45);
  BOOST_CHECK_EQUAL(nTestDWord, 45UL);
  nTestDWord.setValue(nTestSInt127);
  BOOST_CHECK_EQUAL(nTestDWord, 127UL);

#ifdef FORTE_USE_64BIT_DATATYPES
//toLWord
  nTestLWord.setValue(nTestSInt0);
  BOOST_CHECK_EQUAL(nTestLWord, 0ULL);
  nTestLWord.setValue(nTestSInt1);
  BOOST_CHECK_EQUAL(nTestLWord, 1ULL);
  nTestLWord.setValue(nTestSInt45);
  BOOST_CHECK_EQUAL(nTestLWord, 45ULL);
  nTestLWord.setValue(nTestSInt127);
  BOOST_CHECK_EQUAL(nTestLWord, 127ULL);

#endif
}

BOOST_AUTO_TEST_CASE(CASTS_SINT_to_INTS)
{
  CIEC_SINT nTestSInt0;
  CIEC_SINT nTestSInt1;
  CIEC_SINT nTestSInt45;
  CIEC_SINT nTestSInt127;
  CIEC_SINT nTestSInt_1;

  CIEC_USINT nTestUSInt;
  CIEC_INT nTestInt;
  CIEC_UINT nTestUInt;
  CIEC_DINT nTestDInt;
  CIEC_UDINT nTestUDInt;
#ifdef FORTE_USE_64BIT_DATATYPES
  CIEC_LINT nTestLInt;
  CIEC_ULINT nTestULInt;
#endif //FORTE_USE_64BIT_DATATYPES
  //check initial values
  BOOST_CHECK_EQUAL(nTestSInt0, 0);
  BOOST_CHECK_EQUAL(nTestSInt1, 0);
  BOOST_CHECK_EQUAL(nTestSInt45, 0);
  BOOST_CHECK_EQUAL(nTestSInt127, 0);
  BOOST_CHECK_EQUAL(nTestSInt_1, 0);

  nTestSInt0=0;
  nTestSInt1=1;
  nTestSInt45=45;
  nTestSInt127=127;
  nTestSInt_1=-1;

  BOOST_CHECK_EQUAL(nTestSInt0, 0);
  BOOST_CHECK_EQUAL(nTestSInt1, 1);
  BOOST_CHECK_EQUAL(nTestSInt45, 45);
  BOOST_CHECK_EQUAL(nTestSInt127, 127);
  BOOST_CHECK_EQUAL(nTestSInt_1, -1);

//toUSINT
  nTestUSInt.setValue(nTestSInt0);
  BOOST_CHECK_EQUAL(nTestUSInt, 0U);
  nTestUSInt.setValue(nTestSInt1);
  BOOST_CHECK_EQUAL(nTestUSInt, 1U);
  nTestUSInt.setValue(nTestSInt45);
  BOOST_CHECK_EQUAL(nTestUSInt, 45U);
  nTestUSInt.setValue(nTestSInt127);
  BOOST_CHECK_EQUAL(nTestUSInt, 127U);
  nTestUSInt.setValue(nTestSInt_1);
  BOOST_CHECK_EQUAL(nTestUSInt, 255U);

//toINT
  nTestInt.setValue(nTestSInt0);
  BOOST_CHECK_EQUAL(nTestInt, 0);
  nTestInt.setValue(nTestSInt1);
  BOOST_CHECK_EQUAL(nTestInt, 1);
  nTestInt.setValue(nTestSInt45);
  BOOST_CHECK_EQUAL(nTestInt, 45);
  nTestInt.setValue(nTestSInt127);
  BOOST_CHECK_EQUAL(nTestInt, 127);
  nTestInt.setValue(nTestSInt_1);
  BOOST_CHECK_EQUAL(nTestInt, -1);

//toUINT
  nTestUInt.setValue(nTestSInt0);
  BOOST_CHECK_EQUAL(nTestUInt, 0U);
  nTestUInt.setValue(nTestSInt1);
  BOOST_CHECK_EQUAL(nTestUInt, 1U);
  nTestUInt.setValue(nTestSInt45);
  BOOST_CHECK_EQUAL(nTestUInt, 45U);
  nTestUInt.setValue(nTestSInt127);
  BOOST_CHECK_EQUAL(nTestUInt, 127U);

//toDINT
  nTestDInt.setValue(nTestSInt0);
  BOOST_CHECK_EQUAL(nTestDInt, 0L);
  nTestDInt.setValue(nTestSInt1);
  BOOST_CHECK_EQUAL(nTestDInt, 1L);
  nTestDInt.setValue(nTestSInt45);
  BOOST_CHECK_EQUAL(nTestDInt, 45L);
  nTestDInt.setValue(nTestSInt127);
  BOOST_CHECK_EQUAL(nTestDInt, 127L);
  nTestDInt.setValue(nTestSInt_1);
  BOOST_CHECK_EQUAL(nTestDInt, -1L);

//toUDINT
  nTestUDInt.setValue(nTestSInt0);
  BOOST_CHECK_EQUAL(nTestUDInt, 0UL);
  nTestUDInt.setValue(nTestSInt1);
  BOOST_CHECK_EQUAL(nTestUDInt, 1UL);
  nTestUDInt.setValue(nTestSInt45);
  BOOST_CHECK_EQUAL(nTestUDInt, 45UL);
  nTestUDInt.setValue(nTestSInt127);
  BOOST_CHECK_EQUAL(nTestUDInt, 127UL);

#ifdef FORTE_USE_64BIT_DATATYPES
//toLINT
  nTestLInt.setValue(nTestSInt0);
  BOOST_CHECK_EQUAL(nTestLInt, 0LL);
  nTestLInt.setValue(nTestSInt1);
  BOOST_CHECK_EQUAL(nTestLInt, 1LL);
  nTestLInt.setValue(nTestSInt45);
  BOOST_CHECK_EQUAL(nTestLInt, 45LL);
  nTestLInt.setValue(nTestSInt127);
  BOOST_CHECK_EQUAL(nTestLInt, 127LL);
  nTestLInt.setValue(nTestSInt_1);
  BOOST_CHECK_EQUAL(nTestLInt, -1LL);

//toULINT
  nTestULInt.setValue(nTestSInt0);
  BOOST_CHECK_EQUAL(nTestULInt, 0ULL);
  nTestULInt.setValue(nTestSInt1);
  BOOST_CHECK_EQUAL(nTestULInt, 1ULL);
  nTestULInt.setValue(nTestSInt45);
  BOOST_CHECK_EQUAL(nTestULInt, 45ULL);
  nTestULInt.setValue(nTestSInt127);
  BOOST_CHECK_EQUAL(nTestULInt, 127ULL);
#endif
}

#ifdef FORTE_USE_REAL_DATATYPE
BOOST_AUTO_TEST_CASE(CASTS_SINT_to_REAL)
{
  CIEC_SINT nTestSInt0;
  CIEC_SINT nTestSInt1;
  CIEC_SINT nTestSInt45;
  CIEC_SINT nTestSInt127;

  CIEC_REAL nTestReal;

//check initial values
  BOOST_CHECK_EQUAL(nTestSInt0, 0);
  BOOST_CHECK_EQUAL(nTestSInt1, 0);
  BOOST_CHECK_EQUAL(nTestSInt45, 0);
  BOOST_CHECK_EQUAL(nTestSInt127, 0);

  nTestSInt0=0;
  nTestSInt1=1;
  nTestSInt45=45;
  nTestSInt127=127;

  BOOST_CHECK_EQUAL(nTestSInt0, 0);
  BOOST_CHECK_EQUAL(nTestSInt1, 1);
  BOOST_CHECK_EQUAL(nTestSInt45, 45);
  BOOST_CHECK_EQUAL(nTestSInt127, 127);

//toREAL
  nTestReal.setValue(nTestSInt0);
  BOOST_CHECK_EQUAL(nTestReal, 0.0f);
  nTestReal.setValue(nTestSInt1);
  BOOST_CHECK_EQUAL(nTestReal, 1.0f);
  nTestReal.setValue(nTestSInt45);
  BOOST_CHECK_EQUAL(nTestReal, 45.0f);
  nTestReal.setValue(nTestSInt127);
  BOOST_CHECK_EQUAL(nTestReal, 127.0f);

#ifdef FORTE_USE_64BIT_DATATYPES
#ifdef FORTE_USE_LREAL_DATATYPE
  CIEC_LREAL nTestLReal;

//toLREAL
  nTestLReal.setValue(nTestSInt0);
  BOOST_CHECK_EQUAL(nTestLReal, 0.0f);
  nTestLReal.setValue(nTestSInt1);
  BOOST_CHECK_EQUAL(nTestLReal, 1.0f);
  nTestLReal.setValue(nTestSInt45);
  BOOST_CHECK_EQUAL(nTestLReal, 45.0f);
  nTestLReal.setValue(nTestSInt127);
  BOOST_CHECK_EQUAL(nTestLReal, 127.0f);
#endif //FORTE_USE_LREAL_DATATYPE
#endif //FORTE_USE_64BIT_DATATYPES
}
#endif //FORTE_USE_REAL_DATATYPE
BOOST_AUTO_TEST_CASE(CASTS_SINT_to_TIME)
{
  CIEC_SINT nTestSInt0;
  CIEC_SINT nTestSInt1;
  CIEC_SINT nTestSInt45;
  CIEC_SINT nTestSInt127;

  CIEC_TIME nTestTime;

//check initial values
  BOOST_CHECK_EQUAL(nTestSInt0, 0);
  BOOST_CHECK_EQUAL(nTestSInt1, 0);
  BOOST_CHECK_EQUAL(nTestSInt45, 0);
  BOOST_CHECK_EQUAL(nTestSInt127, 0);

  nTestSInt0=0;
  nTestSInt1=1;
  nTestSInt45=45;
  nTestSInt127=127;

  BOOST_CHECK_EQUAL(nTestSInt0, 0);
  BOOST_CHECK_EQUAL(nTestSInt1, 1);
  BOOST_CHECK_EQUAL(nTestSInt45, 45);
  BOOST_CHECK_EQUAL(nTestSInt127, 127);

//toTime
  nTestTime.setValue(nTestSInt0);
  BOOST_CHECK_EQUAL(0, nTestTime);
  nTestTime.setValue(nTestSInt1);
  BOOST_CHECK_EQUAL(1, nTestTime);
  nTestTime.setValue(nTestSInt45);
  BOOST_CHECK_EQUAL(45, nTestTime);
  nTestTime.setValue(nTestSInt127);
  BOOST_CHECK_EQUAL(127, nTestTime);
}

BOOST_AUTO_TEST_CASE(SINT_Castable_test)
{
  bool bUp, bDown;

  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_SINT, CIEC_ANY::e_BOOL, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(bDown);

  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_SINT, CIEC_ANY::e_SINT, bUp, bDown)); BOOST_CHECK(bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_SINT, CIEC_ANY::e_INT, bUp, bDown)); BOOST_CHECK(bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_SINT, CIEC_ANY::e_DINT, bUp, bDown)); BOOST_CHECK(bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_SINT, CIEC_ANY::e_LINT, bUp, bDown)); BOOST_CHECK(bUp); BOOST_CHECK(! bDown);

  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_SINT, CIEC_ANY::e_USINT, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_SINT, CIEC_ANY::e_UINT, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_SINT, CIEC_ANY::e_UDINT, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_SINT, CIEC_ANY::e_ULINT, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(bDown);

  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_SINT, CIEC_ANY::e_BYTE, bUp, bDown)); BOOST_CHECK(bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_SINT, CIEC_ANY::e_WORD, bUp, bDown)); BOOST_CHECK(bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_SINT, CIEC_ANY::e_DWORD, bUp, bDown)); BOOST_CHECK(bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_SINT, CIEC_ANY::e_LWORD, bUp, bDown)); BOOST_CHECK(bUp); BOOST_CHECK(! bDown);

  BOOST_CHECK(!CIEC_ANY::isCastable(CIEC_ANY::e_SINT, CIEC_ANY::e_DATE, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(!CIEC_ANY::isCastable(CIEC_ANY::e_SINT, CIEC_ANY::e_TIME_OF_DAY, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(!CIEC_ANY::isCastable(CIEC_ANY::e_SINT, CIEC_ANY::e_DATE_AND_TIME, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(!CIEC_ANY::isCastable(CIEC_ANY::e_SINT, CIEC_ANY::e_TIME, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(! bDown);

  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_SINT, CIEC_ANY::e_REAL, bUp, bDown)); BOOST_CHECK(bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_SINT, CIEC_ANY::e_LREAL, bUp, bDown)); BOOST_CHECK(bUp); BOOST_CHECK(! bDown);

  BOOST_CHECK(!CIEC_ANY::isCastable(CIEC_ANY::e_SINT, CIEC_ANY::e_STRING, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(!CIEC_ANY::isCastable(CIEC_ANY::e_SINT, CIEC_ANY::e_WSTRING, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(! bDown);
}

BOOST_AUTO_TEST_SUITE_END()
