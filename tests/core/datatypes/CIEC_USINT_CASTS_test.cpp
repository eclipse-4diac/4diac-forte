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

#ifdef FORTE_USE_REAL_DATATYPE
  #include <boost/test/floating_point_comparison.hpp>

  #include "../../../src/core/datatypes/forte_real.h"
#endif //FORTE_USE_REAL_DATATYPE

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


#ifdef FORTE_USE_64BIT_DATATYPES
  #include "../../../src/core/datatypes/forte_lword.h"
  #include "../../../src/core/datatypes/forte_lint.h"
  #include "../../../src/core/datatypes/forte_ulint.h"

#ifdef FORTE_USE_REAL_DATATYPE
  #include "../../../src/core/datatypes/forte_lreal.h"
#endif //FORTE_USE_REAL_DATATYPE
#endif //FORTE_USE_64BIT_DATATYPES

using namespace boost::unit_test;

BOOST_AUTO_TEST_SUITE(CIEC_USINT_casts_function_test)


BOOST_AUTO_TEST_CASE(CASTS_USINT_to_BOOL)
{
  CIEC_USINT nTestUSInt0;
  CIEC_USINT nTestUSInt1;
  CIEC_USINT nTestUSInt45;
  CIEC_USINT nTestUSInt255;

  CIEC_BOOL bTestBool;

//check initial values
  BOOST_CHECK_EQUAL(nTestUSInt0, 0);
  BOOST_CHECK_EQUAL(nTestUSInt1, 0);
  BOOST_CHECK_EQUAL(nTestUSInt45, 0);
  BOOST_CHECK_EQUAL(nTestUSInt255, 0);

  nTestUSInt0=0;
  nTestUSInt1=1;
  nTestUSInt45=45;
  nTestUSInt255=255;

  BOOST_CHECK_EQUAL(nTestUSInt0, 0);
  BOOST_CHECK_EQUAL(nTestUSInt1, 1);
  BOOST_CHECK_EQUAL(nTestUSInt45, 45);
  BOOST_CHECK_EQUAL(nTestUSInt255, 255);

//checks
  bTestBool.setValue(nTestUSInt0);
  BOOST_CHECK_EQUAL(false, bTestBool);
  bTestBool.setValue(nTestUSInt1);
  BOOST_CHECK_EQUAL(true, bTestBool);
  bTestBool.setValue(nTestUSInt45);
  BOOST_CHECK_EQUAL(true, bTestBool);
  bTestBool.setValue(nTestUSInt255);
  BOOST_CHECK_EQUAL(true, bTestBool);

}

BOOST_AUTO_TEST_CASE(CASTS_USINT_to_BITDTs)
{
  CIEC_USINT nTestUSInt0;
  CIEC_USINT nTestUSInt1;
  CIEC_USINT nTestUSInt45;
  CIEC_USINT nTestUSInt255;

  CIEC_BYTE nTestByte;
  CIEC_WORD nTestWord;
  CIEC_DWORD nTestDWord;
#ifdef FORTE_USE_64BIT_DATATYPES
  CIEC_DWORD nTestLWord;
#endif //FORTE_USE_64BIT_DATATYPES

//check initial values
  BOOST_CHECK_EQUAL(nTestUSInt0, 0U);
  BOOST_CHECK_EQUAL(nTestUSInt1, 0U);
  BOOST_CHECK_EQUAL(nTestUSInt45, 0U);
  BOOST_CHECK_EQUAL(nTestUSInt255, 0U);

  nTestUSInt0=0U;
  nTestUSInt1=1U;
  nTestUSInt45=45U;
  nTestUSInt255=255U;

  BOOST_CHECK_EQUAL(nTestUSInt0, 0U);
  BOOST_CHECK_EQUAL(nTestUSInt1, 1U);
  BOOST_CHECK_EQUAL(nTestUSInt45, 45U);
  BOOST_CHECK_EQUAL(nTestUSInt255, 255U);

//toByte
  nTestByte.setValue(nTestUSInt0);
  BOOST_CHECK_EQUAL(nTestByte, 0U);
  nTestByte.setValue(nTestUSInt1);
  BOOST_CHECK_EQUAL(nTestByte, 1U);
  nTestByte.setValue(nTestUSInt45);
  BOOST_CHECK_EQUAL(nTestByte, 45U);
  nTestByte.setValue(nTestUSInt255);
  BOOST_CHECK_EQUAL(nTestByte, 255U);

//toWord
  nTestWord.setValue(nTestUSInt0);
  BOOST_CHECK_EQUAL(nTestWord, 0U);
  nTestWord.setValue(nTestUSInt1);
  BOOST_CHECK_EQUAL(nTestWord, 1U);
  nTestWord.setValue(nTestUSInt45);
  BOOST_CHECK_EQUAL(nTestWord, 45U);
  nTestWord.setValue(nTestUSInt255);
  BOOST_CHECK_EQUAL(nTestWord, 255U);

//toDWord
  nTestDWord.setValue(nTestUSInt0);
  BOOST_CHECK_EQUAL(nTestDWord, 0UL);
  nTestDWord.setValue(nTestUSInt1);
  BOOST_CHECK_EQUAL(nTestDWord, 1UL);
  nTestDWord.setValue(nTestUSInt45);
  BOOST_CHECK_EQUAL(nTestDWord, 45UL);
  nTestDWord.setValue(nTestUSInt255);
  BOOST_CHECK_EQUAL(nTestDWord, 255UL);


#ifdef FORTE_USE_64BIT_DATATYPES
//toLWord
  nTestLWord.setValue(nTestUSInt0);
  BOOST_CHECK_EQUAL(nTestLWord, 0ULL);
  nTestLWord.setValue(nTestUSInt1);
  BOOST_CHECK_EQUAL(nTestLWord, 1ULL);
  nTestLWord.setValue(nTestUSInt45);
  BOOST_CHECK_EQUAL(nTestLWord, 45ULL);
  nTestLWord.setValue(nTestUSInt255);
  BOOST_CHECK_EQUAL(nTestLWord, 255ULL);

#endif
}

BOOST_AUTO_TEST_CASE(CASTS_USINT_to_INTS)
{
  CIEC_USINT nTestUSInt0;
  CIEC_USINT nTestUSInt1;
  CIEC_USINT nTestUSInt45;
  CIEC_USINT nTestUSInt255;

  CIEC_SINT nTestSInt;
  CIEC_INT nTestInt;
  CIEC_UINT nTestUInt;
  CIEC_DINT nTestDInt;
  CIEC_UDINT nTestUDInt;
#ifdef FORTE_USE_64BIT_DATATYPES
  CIEC_LINT nTestLInt;
  CIEC_ULINT nTestULInt;
#endif //FORTE_USE_64BIT_DATATYPES

  //check initial values
    BOOST_CHECK_EQUAL(nTestUSInt0, 0U);
    BOOST_CHECK_EQUAL(nTestUSInt1, 0U);
    BOOST_CHECK_EQUAL(nTestUSInt45, 0U);
    BOOST_CHECK_EQUAL(nTestUSInt255, 0U);

    nTestUSInt0=0U;
    nTestUSInt1=1U;
    nTestUSInt45=45U;
    nTestUSInt255=255U;

    BOOST_CHECK_EQUAL(nTestUSInt0, 0U);
    BOOST_CHECK_EQUAL(nTestUSInt1, 1U);
    BOOST_CHECK_EQUAL(nTestUSInt45, 45U);
    BOOST_CHECK_EQUAL(nTestUSInt255, 255U);

//toSINT
  nTestSInt.setValue(nTestUSInt0);
  BOOST_CHECK_EQUAL(nTestSInt, 0);
  nTestSInt.setValue(nTestUSInt1);
  BOOST_CHECK_EQUAL(nTestSInt, 1);
  nTestSInt.setValue(nTestUSInt45);
  BOOST_CHECK_EQUAL(nTestSInt, 45);
  nTestSInt.setValue(nTestUSInt255);
  BOOST_CHECK_EQUAL(nTestSInt, -1);


//toINT
  nTestInt.setValue(nTestUSInt0);
  BOOST_CHECK_EQUAL(nTestInt, 0);
  nTestInt.setValue(nTestUSInt1);
  BOOST_CHECK_EQUAL(nTestInt, 1);
  nTestInt.setValue(nTestUSInt45);
  BOOST_CHECK_EQUAL(nTestInt, 45);
  nTestInt.setValue(nTestUSInt255);
  BOOST_CHECK_EQUAL(nTestInt, 255);

//toUINT
  nTestUInt.setValue(nTestUSInt0);
  BOOST_CHECK_EQUAL(nTestUInt, 0U);
  nTestUInt.setValue(nTestUSInt1);
  BOOST_CHECK_EQUAL(nTestUInt, 1U);
  nTestUInt.setValue(nTestUSInt45);
  BOOST_CHECK_EQUAL(nTestUInt, 45U);
  nTestUInt.setValue(nTestUSInt255);
  BOOST_CHECK_EQUAL(nTestUInt, 255U);

//toDINT
  nTestDInt.setValue(nTestUSInt0);
  BOOST_CHECK_EQUAL(nTestDInt, 0);
  nTestDInt.setValue(nTestUSInt1);
  BOOST_CHECK_EQUAL(nTestDInt, 1);
  nTestDInt.setValue(nTestUSInt45);
  BOOST_CHECK_EQUAL(nTestDInt, 45);
  nTestDInt.setValue(nTestUSInt255);
  BOOST_CHECK_EQUAL(nTestDInt, 255);

//toUDINT
  nTestUDInt.setValue(nTestUSInt0);
  BOOST_CHECK_EQUAL(nTestUDInt, 0UL);
  nTestUDInt.setValue(nTestUSInt1);
  BOOST_CHECK_EQUAL(nTestUDInt, 1UL);
  nTestUDInt.setValue(nTestUSInt45);
  BOOST_CHECK_EQUAL(nTestUDInt, 45UL);
  nTestUDInt.setValue(nTestUSInt255);
  BOOST_CHECK_EQUAL(nTestUDInt, 255UL);

#ifdef FORTE_USE_64BIT_DATATYPES
//toLINT
  nTestLInt.setValue(nTestUSInt0);
  BOOST_CHECK_EQUAL(nTestLInt, 0LL);
  nTestLInt.setValue(nTestUSInt1);
  BOOST_CHECK_EQUAL(nTestLInt, 1LL);
  nTestLInt.setValue(nTestUSInt45);
  BOOST_CHECK_EQUAL(nTestLInt, 45LL);
  nTestLInt.setValue(nTestUSInt255);
  BOOST_CHECK_EQUAL(nTestLInt, 255LL);

//toULINT
  nTestULInt.setValue(nTestUSInt0);
  BOOST_CHECK_EQUAL(nTestULInt, 0ULL);
  nTestULInt.setValue(nTestUSInt1);
  BOOST_CHECK_EQUAL(nTestULInt, 1ULL);
  nTestULInt.setValue(nTestUSInt45);
  BOOST_CHECK_EQUAL(nTestULInt, 45ULL);
  nTestULInt.setValue(nTestUSInt255);
  BOOST_CHECK_EQUAL(nTestULInt, 255ULL);
#endif
}

#ifdef FORTE_USE_REAL_DATATYPE
BOOST_AUTO_TEST_CASE(CASTS_USINT_to_REAL)
{
  CIEC_USINT nTestUSInt0;
  CIEC_USINT nTestUSInt1;
  CIEC_USINT nTestUSInt45;
  CIEC_USINT nTestUSInt255;

  CIEC_REAL nTestReal;

//check initial values
  BOOST_CHECK_EQUAL(nTestUSInt0, 0U);
  BOOST_CHECK_EQUAL(nTestUSInt1, 0U);
  BOOST_CHECK_EQUAL(nTestUSInt45, 0U);
  BOOST_CHECK_EQUAL(nTestUSInt255, 0U);

  nTestUSInt0=0U;
  nTestUSInt1=1U;
  nTestUSInt45=45U;
  nTestUSInt255=255U;

  BOOST_CHECK_EQUAL(nTestUSInt0, 0U);
  BOOST_CHECK_EQUAL(nTestUSInt1, 1U);
  BOOST_CHECK_EQUAL(nTestUSInt45, 45U);
  BOOST_CHECK_EQUAL(nTestUSInt255, 255U);

//toREAL
  nTestReal.setValue(nTestUSInt0);
  BOOST_CHECK_EQUAL(0.0f, nTestReal);
  nTestReal.setValue(nTestUSInt1);
  BOOST_CHECK_EQUAL(1.0f, nTestReal); 
  nTestReal.setValue(nTestUSInt45);
  BOOST_CHECK_EQUAL(45.0f, nTestReal); 
  nTestReal.setValue(nTestUSInt255);
  BOOST_CHECK_EQUAL(255.0f, nTestReal); 


#ifdef FORTE_USE_64BIT_DATATYPES
  CIEC_LREAL nTestLReal;

//toLREAL
  nTestLReal.setValue(nTestUSInt0);
  BOOST_CHECK_EQUAL(0.0, nTestLReal);
  nTestLReal.setValue(nTestUSInt1);
  BOOST_CHECK_EQUAL(1.0, nTestLReal); 
  nTestLReal.setValue(nTestUSInt45);
  BOOST_CHECK_EQUAL(45.0, nTestLReal); 
  nTestLReal.setValue(nTestUSInt255);
  BOOST_CHECK_EQUAL(255.0, nTestLReal); 
#endif //FORTE_USE_64BIT_DATATYPES

}
#endif //FORTE_USE_REAL_DATATYPE

BOOST_AUTO_TEST_CASE(CASTS_USINT_to_TIME)
{
  CIEC_USINT nTestUSInt0;
  CIEC_USINT nTestUSInt1;
  CIEC_USINT nTestUSInt45;
  CIEC_USINT nTestUSInt255;

  CIEC_TIME nTestTime;

//check initial values
  BOOST_CHECK_EQUAL(nTestUSInt0, 0U);
  BOOST_CHECK_EQUAL(nTestUSInt1, 0U);
  BOOST_CHECK_EQUAL(nTestUSInt45, 0U);
  BOOST_CHECK_EQUAL(nTestUSInt255, 0U);

  nTestUSInt0=0U;
  nTestUSInt1=1U;
  nTestUSInt45=45U;
  nTestUSInt255=255U;

  BOOST_CHECK_EQUAL(nTestUSInt0, 0U);
  BOOST_CHECK_EQUAL(nTestUSInt1, 1U);
  BOOST_CHECK_EQUAL(nTestUSInt45, 45U);
  BOOST_CHECK_EQUAL(nTestUSInt255, 255U);

//toTime
  nTestTime.setValue(nTestUSInt0);
  BOOST_CHECK_EQUAL(0, nTestTime);
  nTestTime.setValue(nTestUSInt1);
  BOOST_CHECK_EQUAL(1, nTestTime);
  nTestTime.setValue(nTestUSInt45);
  BOOST_CHECK_EQUAL(45, nTestTime);
  nTestTime.setValue(nTestUSInt255);
  BOOST_CHECK_EQUAL(255, nTestTime);
}

BOOST_AUTO_TEST_CASE(USINT_Castable_test)
{
  bool bUp, bDown;

  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_USINT, CIEC_ANY::e_BOOL, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(bDown);

  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_USINT, CIEC_ANY::e_SINT, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_USINT, CIEC_ANY::e_INT, bUp, bDown)); BOOST_CHECK(bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_USINT, CIEC_ANY::e_DINT, bUp, bDown)); BOOST_CHECK(bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_USINT, CIEC_ANY::e_LINT, bUp, bDown)); BOOST_CHECK(bUp); BOOST_CHECK(! bDown);

  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_USINT, CIEC_ANY::e_USINT, bUp, bDown)); BOOST_CHECK(bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_USINT, CIEC_ANY::e_UINT, bUp, bDown)); BOOST_CHECK(bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_USINT, CIEC_ANY::e_UDINT, bUp, bDown)); BOOST_CHECK(bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_USINT, CIEC_ANY::e_ULINT, bUp, bDown)); BOOST_CHECK(bUp); BOOST_CHECK(! bDown);

  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_USINT, CIEC_ANY::e_BYTE, bUp, bDown)); BOOST_CHECK(bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_USINT, CIEC_ANY::e_WORD, bUp, bDown)); BOOST_CHECK(bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_USINT, CIEC_ANY::e_DWORD, bUp, bDown)); BOOST_CHECK(bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_USINT, CIEC_ANY::e_LWORD, bUp, bDown)); BOOST_CHECK(bUp); BOOST_CHECK(! bDown);

  BOOST_CHECK(!CIEC_ANY::isCastable(CIEC_ANY::e_USINT, CIEC_ANY::e_DATE, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(!CIEC_ANY::isCastable(CIEC_ANY::e_USINT, CIEC_ANY::e_TIME_OF_DAY, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(!CIEC_ANY::isCastable(CIEC_ANY::e_USINT, CIEC_ANY::e_DATE_AND_TIME, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(!CIEC_ANY::isCastable(CIEC_ANY::e_USINT, CIEC_ANY::e_TIME, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(! bDown);

  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_USINT, CIEC_ANY::e_REAL, bUp, bDown)); BOOST_CHECK(bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_USINT, CIEC_ANY::e_LREAL, bUp, bDown)); BOOST_CHECK(bUp); BOOST_CHECK(! bDown);

  BOOST_CHECK(!CIEC_ANY::isCastable(CIEC_ANY::e_USINT, CIEC_ANY::e_STRING, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(!CIEC_ANY::isCastable(CIEC_ANY::e_USINT, CIEC_ANY::e_WSTRING, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(! bDown);
}

BOOST_AUTO_TEST_CASE(Explict_cast_operator_to_USINT)
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

  CIEC_USINT nResult(0);

  nResult = CIEC_ANY::cast<CIEC_USINT>(nUsint);
  BOOST_TEST(nResult == 2);
  nResult = CIEC_ANY::cast<CIEC_USINT>(nUint);
  BOOST_TEST(nResult == 3);
  nResult = CIEC_ANY::cast<CIEC_USINT>(nUdint);
  BOOST_TEST(nResult == 4);
  nResult = CIEC_ANY::cast<CIEC_USINT>(nUlint);
  BOOST_TEST(nResult == 5);
  nResult = CIEC_ANY::cast<CIEC_USINT>(nSint);
  BOOST_TEST(nResult == 6);
  nResult = CIEC_ANY::cast<CIEC_USINT>(nInt);
  BOOST_TEST(nResult == 7);
  nResult = CIEC_ANY::cast<CIEC_USINT>(nDint);
  BOOST_TEST(nResult == 8);
  nResult = CIEC_ANY::cast<CIEC_USINT>(nLint);
  BOOST_TEST(nResult == 9);
  nResult = CIEC_ANY::cast<CIEC_USINT>(nReal);
  BOOST_TEST(nResult == 10);
  nResult = CIEC_ANY::cast<CIEC_USINT>(nLreal);
  BOOST_TEST(nResult == 11);
  nResult = CIEC_ANY::cast<CIEC_USINT>(nByte);
  BOOST_TEST(nResult == 12);
  nResult = CIEC_ANY::cast<CIEC_USINT>(nWord);
  BOOST_TEST(nResult == 13);
  nResult = CIEC_ANY::cast<CIEC_USINT>(nDword);
  BOOST_TEST(nResult == 14);
  nResult = CIEC_ANY::cast<CIEC_USINT>(nLword);
  BOOST_TEST(nResult == 15);
  nResult = CIEC_ANY::cast<CIEC_USINT>(bBool);
  BOOST_TEST(nResult == true);
}

BOOST_AUTO_TEST_SUITE_END()
