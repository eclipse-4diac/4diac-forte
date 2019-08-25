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

BOOST_AUTO_TEST_SUITE(CIEC_DINT_casts_function_test)


BOOST_AUTO_TEST_CASE(CASTS_DINT_to_BOOL)
{
  CIEC_DINT nTestDInt0;
  CIEC_DINT nTestDInt1;
  CIEC_DINT nTestDInt32768;
  CIEC_DINT nTestDInt2147483647;

  CIEC_BOOL bTestBool;

//check initial values
  BOOST_CHECK_EQUAL(nTestDInt0, 0);
  BOOST_CHECK_EQUAL(nTestDInt1, 0);
  BOOST_CHECK_EQUAL(nTestDInt32768, 0);
  BOOST_CHECK_EQUAL(nTestDInt2147483647, 0);

  nTestDInt0=0;
  nTestDInt1=1;
  nTestDInt32768=32768;
  nTestDInt2147483647=2147483647;

  BOOST_CHECK_EQUAL(nTestDInt0, 0);
  BOOST_CHECK_EQUAL(nTestDInt1, 1);
  BOOST_CHECK_EQUAL(nTestDInt32768, 32768);
  BOOST_CHECK_EQUAL(nTestDInt2147483647, 2147483647);

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
#ifdef FORTE_USE_64BIT_DATATYPES
  CIEC_DWORD nTestLWord;
#endif //FORTE_USE_64BIT_DATATYPES

//check initial values
  BOOST_CHECK_EQUAL(nTestDInt0, 0);
  BOOST_CHECK_EQUAL(nTestDInt1, 0);
  BOOST_CHECK_EQUAL(nTestDInt32768, 0);
  BOOST_CHECK_EQUAL(nTestDInt2147483647, 0);

  nTestDInt0=0;
  nTestDInt1=1;
  nTestDInt32768=32768;
  nTestDInt2147483647=2147483647;

  BOOST_CHECK_EQUAL(nTestDInt0, 0);
  BOOST_CHECK_EQUAL(nTestDInt1, 1);
  BOOST_CHECK_EQUAL(nTestDInt32768, 32768);
  BOOST_CHECK_EQUAL(nTestDInt2147483647, 2147483647);

//toByte
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


#ifdef FORTE_USE_64BIT_DATATYPES
//toLWord
  nTestLWord.setValue(nTestDInt0);
  BOOST_CHECK_EQUAL(nTestLWord, 0ULL);
  nTestLWord.setValue(nTestDInt1);
  BOOST_CHECK_EQUAL(nTestLWord, 1ULL);
  nTestLWord.setValue(nTestDInt32768);
  BOOST_CHECK_EQUAL(nTestLWord, 32768ULL);
  nTestLWord.setValue(nTestDInt2147483647);
  BOOST_CHECK_EQUAL(nTestLWord, 2147483647ULL);

#endif
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
#ifdef FORTE_USE_64BIT_DATATYPES
  CIEC_LINT nTestLInt;
  CIEC_ULINT nTestULInt;
#endif //FORTE_USE_64BIT_DATATYPES

  //check initial values
    BOOST_CHECK_EQUAL(nTestDInt0, 0);
    BOOST_CHECK_EQUAL(nTestDInt1, 0);
    BOOST_CHECK_EQUAL(nTestDInt32768, 0);
    BOOST_CHECK_EQUAL(nTestDInt2147483647, 0);
    BOOST_CHECK_EQUAL(nTestDInt_1, 0);

    nTestDInt0=0;
    nTestDInt1=1;
    nTestDInt32768=32768;
    nTestDInt2147483647=2147483647;
    nTestDInt_1=-1;

    BOOST_CHECK_EQUAL(nTestDInt0, 0);
    BOOST_CHECK_EQUAL(nTestDInt1, 1);
    BOOST_CHECK_EQUAL(nTestDInt32768, 32768);
    BOOST_CHECK_EQUAL(nTestDInt2147483647, 2147483647);
    BOOST_CHECK_EQUAL(nTestDInt_1, -1);

//toUSINT
  nTestUSInt.setValue(nTestDInt0);
  BOOST_CHECK_EQUAL(nTestUSInt, 0U);
  nTestUSInt.setValue(nTestDInt1);
  BOOST_CHECK_EQUAL(nTestUSInt, 1U);
  nTestUSInt.setValue(nTestDInt32768);
  BOOST_CHECK_EQUAL(nTestUSInt, 0U);
  nTestUSInt.setValue(nTestDInt2147483647);
  BOOST_CHECK_EQUAL(nTestUSInt, 255U);
  nTestUSInt.setValue(nTestDInt_1);
  BOOST_CHECK_EQUAL(nTestUSInt, 255U);


//toSINT
  nTestSInt.setValue(nTestDInt0);
  BOOST_CHECK_EQUAL(nTestSInt, 0);
  nTestSInt.setValue(nTestDInt1);
  BOOST_CHECK_EQUAL(nTestSInt, 1);
  nTestSInt.setValue(nTestDInt32768);
  BOOST_CHECK_EQUAL(nTestSInt, 0);
  nTestSInt.setValue(nTestDInt2147483647);
  BOOST_CHECK_EQUAL(nTestSInt, -1);
  nTestSInt.setValue(nTestDInt_1);
  BOOST_CHECK_EQUAL(nTestSInt, -1);

//toUINT
  nTestUInt.setValue(nTestDInt0);
  BOOST_CHECK_EQUAL(nTestUInt, 0U);
  nTestUInt.setValue(nTestDInt1);
  BOOST_CHECK_EQUAL(nTestUInt, 1U);
  nTestUInt.setValue(nTestDInt32768);
  BOOST_CHECK_EQUAL(nTestUInt, 32768U);
  nTestUInt.setValue(nTestDInt2147483647);
  BOOST_CHECK_EQUAL(nTestUInt, 65535U);
  nTestUInt.setValue(nTestDInt_1);
  BOOST_CHECK_EQUAL(nTestUInt, 65535U);

//toINT
  nTestInt.setValue(nTestDInt0);
  BOOST_CHECK_EQUAL(nTestInt, 0);
  nTestInt.setValue(nTestDInt1);
  BOOST_CHECK_EQUAL(nTestInt, 1);
  nTestInt.setValue(nTestDInt32768);
  BOOST_CHECK_EQUAL(nTestInt, -32768);
  nTestInt.setValue(nTestDInt2147483647);
  BOOST_CHECK_EQUAL(nTestInt, -1);
  nTestInt.setValue(nTestDInt_1);
  BOOST_CHECK_EQUAL(nTestInt, -1);

//toUDINT
  nTestUDInt.setValue(nTestDInt0);
  BOOST_CHECK_EQUAL(nTestUDInt, 0U);
  nTestUDInt.setValue(nTestDInt1);
  BOOST_CHECK_EQUAL(nTestUDInt, 1U);
  nTestUDInt.setValue(nTestDInt32768);
  BOOST_CHECK_EQUAL(nTestUDInt, 32768U);
  nTestUDInt.setValue(nTestDInt2147483647);
  BOOST_CHECK_EQUAL(nTestUDInt, 2147483647U);

#ifdef FORTE_USE_64BIT_DATATYPES
//toLINT
  nTestLInt.setValue(nTestDInt0);
  BOOST_CHECK_EQUAL(nTestLInt, 0LL);
  nTestLInt.setValue(nTestDInt1);
  BOOST_CHECK_EQUAL(nTestLInt, 1LL);
  nTestLInt.setValue(nTestDInt32768);
  BOOST_CHECK_EQUAL(nTestLInt, 32768LL);
  nTestLInt.setValue(nTestDInt2147483647);
  BOOST_CHECK_EQUAL(nTestLInt, 2147483647LL);
  nTestLInt.setValue(nTestDInt_1);
  BOOST_CHECK_EQUAL(nTestLInt, -1LL);

//toULINT
  nTestULInt.setValue(nTestDInt0);
  BOOST_CHECK_EQUAL(nTestULInt, 0ULL);
  nTestULInt.setValue(nTestDInt1);
  BOOST_CHECK_EQUAL(nTestULInt, 1ULL);
  nTestULInt.setValue(nTestDInt32768);
  BOOST_CHECK_EQUAL(nTestULInt, 32768ULL);
  nTestULInt.setValue(nTestDInt2147483647);
  BOOST_CHECK_EQUAL(nTestULInt, 2147483647ULL);
#endif
}

#ifdef FORTE_USE_REAL_DATATYPE
BOOST_AUTO_TEST_CASE(CASTS_DINT_to_REAL)
{
  CIEC_DINT nTestDInt0;
  CIEC_DINT nTestDInt1;
  CIEC_DINT nTestDInt32768;
  CIEC_DINT nTestDInt2147483647;

  CIEC_REAL nTestReal;

//check initial values
  BOOST_CHECK_EQUAL(nTestDInt0, 0);
  BOOST_CHECK_EQUAL(nTestDInt1, 0);
  BOOST_CHECK_EQUAL(nTestDInt32768, 0);
  BOOST_CHECK_EQUAL(nTestDInt2147483647, 0);

  nTestDInt0=0;
  nTestDInt1=1;
  nTestDInt32768=32768;
  nTestDInt2147483647=2147483647;

  BOOST_CHECK_EQUAL(nTestDInt0, 0);
  BOOST_CHECK_EQUAL(nTestDInt1, 1);
  BOOST_CHECK_EQUAL(nTestDInt32768, 32768);
  BOOST_CHECK_EQUAL(nTestDInt2147483647, 2147483647);

//toREAL
  nTestReal.setValue(nTestDInt0);
  BOOST_CHECK_EQUAL(0.0f, nTestReal);
  nTestReal.setValue(nTestDInt1);
  BOOST_CHECK_EQUAL(1.0f, nTestReal); 
  nTestReal.setValue(nTestDInt32768);
  BOOST_CHECK_EQUAL(32768.0f, nTestReal); 
  nTestReal.setValue(nTestDInt2147483647);
  BOOST_CHECK_EQUAL(2147483647.0f, nTestReal); 


#ifdef FORTE_USE_64BIT_DATATYPES
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
#endif //FORTE_USE_64BIT_DATATYPES

}
#endif //FORTE_USE_REAL_DATATYPE

BOOST_AUTO_TEST_CASE(CASTS_DINT_to_TIME)
{
  CIEC_DINT nTestDInt0;
  CIEC_DINT nTestDInt1;
  CIEC_DINT nTestDInt32768;
  CIEC_DINT nTestDInt2147483647;

  CIEC_TIME nTestTime;

//check initial values
  BOOST_CHECK_EQUAL(nTestDInt0, 0);
  BOOST_CHECK_EQUAL(nTestDInt1, 0);
  BOOST_CHECK_EQUAL(nTestDInt32768, 0);
  BOOST_CHECK_EQUAL(nTestDInt2147483647, 0);

  nTestDInt0=0;
  nTestDInt1=1;
  nTestDInt32768=32768;
  nTestDInt2147483647=2147483647;

  BOOST_CHECK_EQUAL(nTestDInt0, 0);
  BOOST_CHECK_EQUAL(nTestDInt1, 1);
  BOOST_CHECK_EQUAL(nTestDInt32768, 32768);
  BOOST_CHECK_EQUAL(nTestDInt2147483647, 2147483647);

//toTIME
  nTestTime.setValue(nTestDInt0);
  BOOST_CHECK_EQUAL(0, nTestTime);
  nTestTime.setValue(nTestDInt1);
  BOOST_CHECK_EQUAL(1, nTestTime);
  nTestTime.setValue(nTestDInt32768);
  BOOST_CHECK_EQUAL(32768, nTestTime);
  nTestTime.setValue(nTestDInt2147483647);
  BOOST_CHECK_EQUAL(2147483647, nTestTime);

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

BOOST_AUTO_TEST_CASE(Explict_cast_operator_to_DINT)
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

  CIEC_DINT nResult(0);

  nResult = CIEC_ANY::cast<CIEC_DINT>(nUsint);
  BOOST_TEST(nResult == 2);
  nResult = CIEC_ANY::cast<CIEC_DINT>(nUint);
  BOOST_TEST(nResult == 3);
  nResult = CIEC_ANY::cast<CIEC_DINT>(nUdint);
  BOOST_TEST(nResult == 4);
  nResult = CIEC_ANY::cast<CIEC_DINT>(nUlint);
  BOOST_TEST(nResult == 5);
  nResult = CIEC_ANY::cast<CIEC_DINT>(nSint);
  BOOST_TEST(nResult == 6);
  nResult = CIEC_ANY::cast<CIEC_DINT>(nInt);
  BOOST_TEST(nResult == 7);
  nResult = CIEC_ANY::cast<CIEC_DINT>(nDint);
  BOOST_TEST(nResult == 8);
  nResult = CIEC_ANY::cast<CIEC_DINT>(nLint);
  BOOST_TEST(nResult == 9);
  nResult = CIEC_ANY::cast<CIEC_DINT>(nReal);
  BOOST_TEST(nResult == 10);
  nResult = CIEC_ANY::cast<CIEC_DINT>(nLreal);
  BOOST_TEST(nResult == 11);
  nResult = CIEC_ANY::cast<CIEC_DINT>(nByte);
  BOOST_TEST(nResult == 12);
  nResult = CIEC_ANY::cast<CIEC_DINT>(nWord);
  BOOST_TEST(nResult == 13);
  nResult = CIEC_ANY::cast<CIEC_DINT>(nDword);
  BOOST_TEST(nResult == 14);
  nResult = CIEC_ANY::cast<CIEC_DINT>(nLword);
  BOOST_TEST(nResult == 15);
  nResult = CIEC_ANY::cast<CIEC_DINT>(bBool);
  BOOST_TEST(nResult == true);
}

BOOST_AUTO_TEST_SUITE_END()
