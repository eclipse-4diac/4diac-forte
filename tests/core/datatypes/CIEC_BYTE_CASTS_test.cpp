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

BOOST_AUTO_TEST_SUITE(CIEC_BYTE_casts_function_test)


BOOST_AUTO_TEST_CASE(CASTS_BYTE_to_BOOL)
{
  CIEC_BYTE nTestByte0;
  CIEC_BYTE nTestByte1;
  CIEC_BYTE nTestByte45;
  CIEC_BYTE nTestByte255;

  CIEC_BOOL bTestBool;

//check initial values
  BOOST_CHECK_EQUAL(nTestByte0, 0U);
  BOOST_CHECK_EQUAL(nTestByte1, 0U);
  BOOST_CHECK_EQUAL(nTestByte45, 0U);
  BOOST_CHECK_EQUAL(nTestByte255, 0U);

  nTestByte0=0U;
  nTestByte1=1U;
  nTestByte45=45U;
  nTestByte255=255U;

  BOOST_CHECK_EQUAL(nTestByte0, 0U);
  BOOST_CHECK_EQUAL(nTestByte1, 1U);
  BOOST_CHECK_EQUAL(nTestByte45, 45U);
  BOOST_CHECK_EQUAL(nTestByte255, 255U);

//checks
  bTestBool.setValue(nTestByte0);
  BOOST_CHECK_EQUAL(bTestBool, false);
  bTestBool.setValue(nTestByte1);
  BOOST_CHECK_EQUAL(bTestBool, true);
  bTestBool.setValue(nTestByte45);
  BOOST_CHECK_EQUAL(true, bTestBool);
  bTestBool.setValue(nTestByte255);
  BOOST_CHECK_EQUAL(true, bTestBool);

}

BOOST_AUTO_TEST_CASE(CASTS_BYTE_to_BITDTs)
{
  CIEC_BYTE nTestByte0;
  CIEC_BYTE nTestByte1;
  CIEC_BYTE nTestByte45;
  CIEC_BYTE nTestByte255;

  CIEC_WORD nTestWord;
  CIEC_DWORD nTestDWord;
#ifdef FORTE_USE_64BIT_DATATYPES
  CIEC_DWORD nTestLWord;
#endif //FORTE_USE_64BIT_DATATYPES

//check initial values
  BOOST_CHECK_EQUAL(nTestByte0, 0U);
  BOOST_CHECK_EQUAL(nTestByte1, 0U);
  BOOST_CHECK_EQUAL(nTestByte45, 0U);
  BOOST_CHECK_EQUAL(nTestByte255, 0U);

  nTestByte0=0U;
  nTestByte1=1U;
  nTestByte45=45U;
  nTestByte255=255U;

  BOOST_CHECK_EQUAL(nTestByte0, 0U);
  BOOST_CHECK_EQUAL(nTestByte1, 1U);
  BOOST_CHECK_EQUAL(nTestByte45, 45U);
  BOOST_CHECK_EQUAL(nTestByte255, 255U);

//toWord
  nTestWord.setValue(nTestByte0);
  BOOST_CHECK_EQUAL(nTestWord, 0U);
  nTestWord.setValue(nTestByte1);
  BOOST_CHECK_EQUAL(nTestWord, 1U);
  nTestWord.setValue(nTestByte45);
  BOOST_CHECK_EQUAL(nTestWord, 45U);
  nTestWord.setValue(nTestByte255);
  BOOST_CHECK_EQUAL(nTestWord, 255U);

//toDWord
  nTestDWord.setValue(nTestByte0);
  BOOST_CHECK_EQUAL(nTestDWord, 0UL);
  nTestDWord.setValue(nTestByte1);
  BOOST_CHECK_EQUAL(nTestDWord, 1UL);
  nTestDWord.setValue(nTestByte45);
  BOOST_CHECK_EQUAL(nTestDWord, 45UL);
  nTestDWord.setValue(nTestByte255);
  BOOST_CHECK_EQUAL(nTestDWord, 255UL);


#ifdef FORTE_USE_64BIT_DATATYPES
//toLWord
  nTestLWord.setValue(nTestByte0);
  BOOST_CHECK_EQUAL(nTestLWord, 0ULL);
  nTestLWord.setValue(nTestByte1);
  BOOST_CHECK_EQUAL(nTestLWord, 1ULL);
  nTestLWord.setValue(nTestByte45);
  BOOST_CHECK_EQUAL(nTestLWord, 45ULL);
  nTestLWord.setValue(nTestByte255);
  BOOST_CHECK_EQUAL(nTestLWord, 255ULL);

#endif
}

BOOST_AUTO_TEST_CASE(CASTS_BYTE_to_INTS)
{
  CIEC_BYTE nTestByte0;
  CIEC_BYTE nTestByte1;
  CIEC_BYTE nTestByte45;
  CIEC_BYTE nTestByte255;

  CIEC_SINT nTestSInt;
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
    BOOST_CHECK_EQUAL(nTestByte0, 0U);
    BOOST_CHECK_EQUAL(nTestByte1, 0U);
    BOOST_CHECK_EQUAL(nTestByte45, 0U);
    BOOST_CHECK_EQUAL(nTestByte255, 0U);

    nTestByte0=0U;
    nTestByte1=1U;
    nTestByte45=45U;
    nTestByte255=255U;

    BOOST_CHECK_EQUAL(nTestByte0, 0U);
    BOOST_CHECK_EQUAL(nTestByte1, 1U);
    BOOST_CHECK_EQUAL(nTestByte45, 45U);
    BOOST_CHECK_EQUAL(nTestByte255, 255U);

//toSINT
  nTestSInt.setValue(nTestByte0);
  BOOST_CHECK_EQUAL(nTestSInt, 0);
  nTestSInt.setValue(nTestByte1);
  BOOST_CHECK_EQUAL(nTestSInt, 1);
  nTestSInt.setValue(nTestByte45);
  BOOST_CHECK_EQUAL(nTestSInt, 45);
  nTestSInt.setValue(nTestByte255);
  BOOST_CHECK_EQUAL(nTestSInt, -1);

//toUSINT
  nTestUSInt.setValue(nTestByte0);
  BOOST_CHECK_EQUAL(nTestUSInt, 0U);
  nTestUSInt.setValue(nTestByte1);
  BOOST_CHECK_EQUAL(nTestUSInt, 1U);
  nTestUSInt.setValue(nTestByte45);
  BOOST_CHECK_EQUAL(nTestUSInt, 45U);
  nTestUSInt.setValue(nTestByte255);
  BOOST_CHECK_EQUAL(nTestUSInt, 255U);


//toINT
  nTestInt.setValue(nTestByte0);
  BOOST_CHECK_EQUAL(nTestInt, 0);
  nTestInt.setValue(nTestByte1);
  BOOST_CHECK_EQUAL(nTestInt, 1);
  nTestInt.setValue(nTestByte45);
  BOOST_CHECK_EQUAL(nTestInt, 45);
  nTestInt.setValue(nTestByte255);
  BOOST_CHECK_EQUAL(nTestInt, 255);

//toUINT
  nTestUInt.setValue(nTestByte0);
  BOOST_CHECK_EQUAL(nTestUInt, 0U);
  nTestUInt.setValue(nTestByte1);
  BOOST_CHECK_EQUAL(nTestUInt, 1U);
  nTestUInt.setValue(nTestByte45);
  BOOST_CHECK_EQUAL(nTestUInt, 45U);
  nTestUInt.setValue(nTestByte255);
  BOOST_CHECK_EQUAL(nTestUInt, 255U);

//toDINT
  nTestDInt.setValue(nTestByte0);
  BOOST_CHECK_EQUAL(nTestDInt, 0L);
  nTestDInt.setValue(nTestByte1);
  BOOST_CHECK_EQUAL(nTestDInt, 1L);
  nTestDInt.setValue(nTestByte45);
  BOOST_CHECK_EQUAL(nTestDInt, 45L);
  nTestDInt.setValue(nTestByte255);
  BOOST_CHECK_EQUAL(nTestDInt, 255L);

//toUDINT
  nTestUDInt.setValue(nTestByte0);
  BOOST_CHECK_EQUAL(nTestUDInt, 0UL);
  nTestUDInt.setValue(nTestByte1);
  BOOST_CHECK_EQUAL(nTestUDInt, 1UL);
  nTestUDInt.setValue(nTestByte45);
  BOOST_CHECK_EQUAL(nTestUDInt, 45UL);
  nTestUDInt.setValue(nTestByte255);
  BOOST_CHECK_EQUAL(nTestUDInt, 255UL);

#ifdef FORTE_USE_64BIT_DATATYPES
//toLINT
  nTestLInt.setValue(nTestByte0);
  BOOST_CHECK_EQUAL(nTestLInt, 0LL);
  nTestLInt.setValue(nTestByte1);
  BOOST_CHECK_EQUAL(nTestLInt, 1LL);
  nTestLInt.setValue(nTestByte45);
  BOOST_CHECK_EQUAL(nTestLInt, 45LL);
  nTestLInt.setValue(nTestByte255);
  BOOST_CHECK_EQUAL(nTestLInt, 255LL);

//toULINT
  nTestULInt.setValue(nTestByte0);
  BOOST_CHECK_EQUAL(nTestULInt, 0ULL);
  nTestULInt.setValue(nTestByte1);
  BOOST_CHECK_EQUAL(nTestULInt, 1ULL);
  nTestULInt.setValue(nTestByte45);
  BOOST_CHECK_EQUAL(nTestULInt, 45ULL);
  nTestULInt.setValue(nTestByte255);
  BOOST_CHECK_EQUAL(nTestULInt, 255ULL);
#endif
}

#ifdef FORTE_USE_REAL_DATATYPE
BOOST_AUTO_TEST_CASE(CASTS_BYTE_to_REAL)
{
  CIEC_BYTE nTestByte0;
  CIEC_BYTE nTestByte1;
  CIEC_BYTE nTestByte45;
  CIEC_BYTE nTestByte255;

  CIEC_REAL nTestReal;

//check initial values
  BOOST_CHECK_EQUAL(nTestByte0, 0U);
  BOOST_CHECK_EQUAL(nTestByte1, 0U);
  BOOST_CHECK_EQUAL(nTestByte45, 0U);
  BOOST_CHECK_EQUAL(nTestByte255, 0U);

  nTestByte0=0U;
  nTestByte1=1U;
  nTestByte45=45U;
  nTestByte255=255U;

  BOOST_CHECK_EQUAL(nTestByte0, 0U);
  BOOST_CHECK_EQUAL(nTestByte1, 1U);
  BOOST_CHECK_EQUAL(nTestByte45, 45U);
  BOOST_CHECK_EQUAL(nTestByte255, 255U);

//toREAL
  nTestReal.setValue(nTestByte0);
  BOOST_CHECK_EQUAL(0.0f, nTestReal);
  nTestReal.setValue(nTestByte1);
  BOOST_CHECK_EQUAL(1.0f, nTestReal); 
  nTestReal.setValue(nTestByte45);
  BOOST_CHECK_EQUAL(45.0f, nTestReal); 
  nTestReal.setValue(nTestByte255);
  BOOST_CHECK_EQUAL(255.0f, nTestReal); 


#ifdef FORTE_USE_64BIT_DATATYPES
  CIEC_LREAL nTestLReal;

//toLREAL
  nTestLReal.setValue(nTestByte0);
  BOOST_CHECK_EQUAL(0.0, nTestLReal);
  nTestLReal.setValue(nTestByte1);
  BOOST_CHECK_EQUAL(1.0, nTestLReal); 
  nTestLReal.setValue(nTestByte45);
  BOOST_CHECK_EQUAL(45.0, nTestLReal); 
  nTestLReal.setValue(nTestByte255);
  BOOST_CHECK_EQUAL(255.0, nTestLReal); 
#endif //FORTE_USE_64BIT_DATATYPES

}
#endif //FORTE_USE_REAL_DATATYPE


BOOST_AUTO_TEST_CASE(CASTS_BYTE_to_TIME)
{
  CIEC_BYTE nTestByte0;
  CIEC_BYTE nTestByte1;
  CIEC_BYTE nTestByte45;
  CIEC_BYTE nTestByte255;

  CIEC_TIME nTestTime;

//check initial values
  BOOST_CHECK_EQUAL(nTestByte0, 0U);
  BOOST_CHECK_EQUAL(nTestByte1, 0U);
  BOOST_CHECK_EQUAL(nTestByte45, 0U);
  BOOST_CHECK_EQUAL(nTestByte255, 0U);

  nTestByte0=0U;
  nTestByte1=1U;
  nTestByte45=45U;
  nTestByte255=255U;

  BOOST_CHECK_EQUAL(nTestByte0, 0U);
  BOOST_CHECK_EQUAL(nTestByte1, 1U);
  BOOST_CHECK_EQUAL(nTestByte45, 45U);
  BOOST_CHECK_EQUAL(nTestByte255, 255U);

//toTime
  nTestTime.setValue(nTestByte0);
  BOOST_CHECK_EQUAL(0, nTestTime);
  nTestTime.setValue(nTestByte1);
  BOOST_CHECK_EQUAL(1, nTestTime);
  nTestTime.setValue(nTestByte45);
  BOOST_CHECK_EQUAL(45, nTestTime);
  nTestTime.setValue(nTestByte255);
  BOOST_CHECK_EQUAL(255, nTestTime);
}

BOOST_AUTO_TEST_CASE(BYTE_Castable_test)
{
  bool bUp, bDown;

  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_BYTE, CIEC_ANY::e_BOOL, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(bDown);

  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_BYTE, CIEC_ANY::e_SINT, bUp, bDown)); BOOST_CHECK(bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_BYTE, CIEC_ANY::e_INT, bUp, bDown)); BOOST_CHECK(bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_BYTE, CIEC_ANY::e_DINT, bUp, bDown)); BOOST_CHECK(bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_BYTE, CIEC_ANY::e_LINT, bUp, bDown)); BOOST_CHECK(bUp); BOOST_CHECK(! bDown);

  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_BYTE, CIEC_ANY::e_USINT, bUp, bDown)); BOOST_CHECK(bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_BYTE, CIEC_ANY::e_UINT, bUp, bDown)); BOOST_CHECK(bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_BYTE, CIEC_ANY::e_UDINT, bUp, bDown)); BOOST_CHECK(bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_BYTE, CIEC_ANY::e_ULINT, bUp, bDown)); BOOST_CHECK(bUp); BOOST_CHECK(! bDown);

  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_BYTE, CIEC_ANY::e_BYTE, bUp, bDown)); BOOST_CHECK(bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_BYTE, CIEC_ANY::e_WORD, bUp, bDown)); BOOST_CHECK(bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_BYTE, CIEC_ANY::e_DWORD, bUp, bDown)); BOOST_CHECK(bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_BYTE, CIEC_ANY::e_LWORD, bUp, bDown)); BOOST_CHECK(bUp); BOOST_CHECK(! bDown);

  BOOST_CHECK(!CIEC_ANY::isCastable(CIEC_ANY::e_BYTE, CIEC_ANY::e_DATE, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(!CIEC_ANY::isCastable(CIEC_ANY::e_BYTE, CIEC_ANY::e_TIME_OF_DAY, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(!CIEC_ANY::isCastable(CIEC_ANY::e_BYTE, CIEC_ANY::e_DATE_AND_TIME, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(!CIEC_ANY::isCastable(CIEC_ANY::e_BYTE, CIEC_ANY::e_TIME, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(! bDown);

  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_BYTE, CIEC_ANY::e_REAL, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_BYTE, CIEC_ANY::e_LREAL, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(bDown);

  BOOST_CHECK(!CIEC_ANY::isCastable(CIEC_ANY::e_BYTE, CIEC_ANY::e_STRING, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(!CIEC_ANY::isCastable(CIEC_ANY::e_BYTE, CIEC_ANY::e_WSTRING, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(! bDown);
}

BOOST_AUTO_TEST_CASE(Explict_cast_operator_to_BYTE)
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

  CIEC_BYTE nResult(0);

  nResult = CIEC_ANY::cast<CIEC_BYTE>(nUsint);
  BOOST_TEST(nResult == 2);
  nResult = CIEC_ANY::cast<CIEC_BYTE>(nUint);
  BOOST_TEST(nResult == 3);
  nResult = CIEC_ANY::cast<CIEC_BYTE>(nUdint);
  BOOST_TEST(nResult == 4);
  nResult = CIEC_ANY::cast<CIEC_BYTE>(nUlint);
  BOOST_TEST(nResult == 5);
  nResult = CIEC_ANY::cast<CIEC_BYTE>(nSint);
  BOOST_TEST(nResult == 6);
  nResult = CIEC_ANY::cast<CIEC_BYTE>(nInt);
  BOOST_TEST(nResult == 7);
  nResult = CIEC_ANY::cast<CIEC_BYTE>(nDint);
  BOOST_TEST(nResult == 8);
  nResult = CIEC_ANY::cast<CIEC_BYTE>(nLint);
  BOOST_TEST(nResult == 9);
  nResult = CIEC_ANY::cast<CIEC_BYTE>(nByte);
  BOOST_TEST(nResult == 12);
  nResult = CIEC_ANY::cast<CIEC_BYTE>(nWord);
  BOOST_TEST(nResult == 13);
  nResult = CIEC_ANY::cast<CIEC_BYTE>(nDword);
  BOOST_TEST(nResult == 14);
  nResult = CIEC_ANY::cast<CIEC_BYTE>(nLword);
  BOOST_TEST(nResult == 15);
  nResult = CIEC_ANY::cast<CIEC_BYTE>(bBool);
  BOOST_TEST(nResult == true);
}

BOOST_AUTO_TEST_SUITE_END()
