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

BOOST_AUTO_TEST_SUITE(CIEC_WORD_casts_function_test)


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

  nTestWord0=0x00;
  nTestWord1=0x01;
  nTestWord256=0x0100;
  nTestWord65535=0xffff;

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
#ifdef FORTE_USE_64BIT_DATATYPES
  CIEC_DWORD nTestLWord;
#endif //FORTE_USE_64BIT_DATATYPES

//check initial values
  BOOST_CHECK_EQUAL(nTestWord0, 0U);
  BOOST_CHECK_EQUAL(nTestWord1, 0U);
  BOOST_CHECK_EQUAL(nTestWord256, 0U);
  BOOST_CHECK_EQUAL(nTestWord65535, 0U);

  nTestWord0=0U;
  nTestWord1=1U;
  nTestWord256=256U;
  nTestWord65535=65535U;

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


#ifdef FORTE_USE_64BIT_DATATYPES
//toLWord
  nTestLWord.setValue(nTestWord0);
  BOOST_CHECK_EQUAL(nTestLWord, 0ULL);
  nTestLWord.setValue(nTestWord1);
  BOOST_CHECK_EQUAL(nTestLWord, 1ULL);
  nTestLWord.setValue(nTestWord256);
  BOOST_CHECK_EQUAL(nTestLWord, 256ULL);
  nTestLWord.setValue(nTestWord65535);
  BOOST_CHECK_EQUAL(nTestLWord, 65535ULL);

#endif
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
#ifdef FORTE_USE_64BIT_DATATYPES
  CIEC_LINT nTestLInt;
  CIEC_ULINT nTestULInt;
#endif //FORTE_USE_64BIT_DATATYPES

  //check initial values
    BOOST_CHECK_EQUAL(nTestWord0, 0U);
    BOOST_CHECK_EQUAL(nTestWord1, 0U);
    BOOST_CHECK_EQUAL(nTestWord256, 0U);
    BOOST_CHECK_EQUAL(nTestWord65535, 0U);

    nTestWord0=0U;
    nTestWord1=1U;
    nTestWord256=256U;
    nTestWord65535=65535U;

    BOOST_CHECK_EQUAL(nTestWord0, 0U);
    BOOST_CHECK_EQUAL(nTestWord1, 1U);
    BOOST_CHECK_EQUAL(nTestWord256, 256U);
    BOOST_CHECK_EQUAL(nTestWord65535, 65535U);

//toSINT
  nTestSInt.setValue(nTestWord0);
  BOOST_CHECK_EQUAL(nTestSInt, 0);
  nTestSInt.setValue(nTestWord1);
  BOOST_CHECK_EQUAL(nTestSInt, 1);
  nTestSInt.setValue(nTestWord256);
  BOOST_CHECK_EQUAL(nTestSInt, 0);
  nTestSInt.setValue(nTestWord65535);
  BOOST_CHECK_EQUAL(nTestSInt, -1);

//toUSINT
  nTestUSInt.setValue(nTestWord0);
  BOOST_CHECK_EQUAL(nTestUSInt, 0U);
  nTestUSInt.setValue(nTestWord1);
  BOOST_CHECK_EQUAL(nTestUSInt, 1U);
  nTestUSInt.setValue(nTestWord256);
  BOOST_CHECK_EQUAL(nTestUSInt, 0U);
  nTestUSInt.setValue(nTestWord65535);
  BOOST_CHECK_EQUAL(nTestUSInt, 255U);


//toINT
  nTestInt.setValue(nTestWord0);
  BOOST_CHECK_EQUAL(nTestInt, 0);
  nTestInt.setValue(nTestWord1);
  BOOST_CHECK_EQUAL(nTestInt, 1);
  nTestInt.setValue(nTestWord256);
  BOOST_CHECK_EQUAL(nTestInt, 256);
  nTestInt.setValue(nTestWord65535);
  BOOST_CHECK_EQUAL(nTestInt, -1);

//toUINT
  nTestUInt.setValue(nTestWord0);
  BOOST_CHECK_EQUAL(nTestUInt, 0U);
  nTestUInt.setValue(nTestWord1);
  BOOST_CHECK_EQUAL(nTestUInt, 1U);
  nTestUInt.setValue(nTestWord256);
  BOOST_CHECK_EQUAL(nTestUInt, 256U);
  nTestUInt.setValue(nTestWord65535);
  BOOST_CHECK_EQUAL(nTestUInt, 65535U);

//toDINT
  nTestDInt.setValue(nTestWord0);
  BOOST_CHECK_EQUAL(nTestDInt, 0);
  nTestDInt.setValue(nTestWord1);
  BOOST_CHECK_EQUAL(nTestDInt, 1);
  nTestDInt.setValue(nTestWord256);
  BOOST_CHECK_EQUAL(nTestDInt, 256);
  nTestDInt.setValue(nTestWord65535);
  BOOST_CHECK_EQUAL(nTestDInt, 65535);

//toUDINT
  nTestUDInt.setValue(nTestWord0);
  BOOST_CHECK_EQUAL(nTestUDInt, 0UL);
  nTestUDInt.setValue(nTestWord1);
  BOOST_CHECK_EQUAL(nTestUDInt, 1UL);
  nTestUDInt.setValue(nTestWord256);
  BOOST_CHECK_EQUAL(nTestUDInt, 256UL);
  nTestUDInt.setValue(nTestWord65535);
  BOOST_CHECK_EQUAL(nTestUDInt, 65535UL);

#ifdef FORTE_USE_64BIT_DATATYPES
//toLINT
  nTestLInt.setValue(nTestWord0);
  BOOST_CHECK_EQUAL(nTestLInt, 0LL);
  nTestLInt.setValue(nTestWord1);
  BOOST_CHECK_EQUAL(nTestLInt, 1LL);
  nTestLInt.setValue(nTestWord256);
  BOOST_CHECK_EQUAL(nTestLInt, 256LL);
  nTestLInt.setValue(nTestWord65535);
  BOOST_CHECK_EQUAL(nTestLInt, 65535LL);

//toULINT
  nTestULInt.setValue(nTestWord0);
  BOOST_CHECK_EQUAL(nTestULInt, 0ULL);
  nTestULInt.setValue(nTestWord1);
  BOOST_CHECK_EQUAL(nTestULInt, 1ULL);
  nTestULInt.setValue(nTestWord256);
  BOOST_CHECK_EQUAL(nTestULInt, 256ULL);
  nTestULInt.setValue(nTestWord65535);
  BOOST_CHECK_EQUAL(nTestULInt, 65535ULL);
#endif
}

#ifdef FORTE_USE_REAL_DATATYPE
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

  nTestWord0=0U;
  nTestWord1=1U;
  nTestWord256=256U;
  nTestWord65535=65535U;

  BOOST_CHECK_EQUAL(nTestWord0, 0U);
  BOOST_CHECK_EQUAL(nTestWord1, 1U);
  BOOST_CHECK_EQUAL(nTestWord256, 256U);
  BOOST_CHECK_EQUAL(nTestWord65535, 65535U);

//toREAL
  nTestReal.setValue(nTestWord0);
  BOOST_CHECK_EQUAL(0.0f, nTestReal);
  nTestReal.setValue(nTestWord1);
  BOOST_CHECK_EQUAL(1.0f, nTestReal); 
  nTestReal.setValue(nTestWord256);
  BOOST_CHECK_EQUAL(256.0f, nTestReal); 
  nTestReal.setValue(nTestWord65535);
  BOOST_CHECK_EQUAL(65535.0f, nTestReal); 


#ifdef FORTE_USE_64BIT_DATATYPES
  CIEC_LREAL nTestLReal;

//toLREAL
  nTestLReal.setValue(nTestWord0);
  BOOST_CHECK_EQUAL(0.0, nTestLReal);
  nTestLReal.setValue(nTestWord1);
  BOOST_CHECK_EQUAL(1.0, nTestLReal); 
  nTestLReal.setValue(nTestWord256);
  BOOST_CHECK_EQUAL(256.0, nTestLReal); 
  nTestLReal.setValue(nTestWord65535);
  BOOST_CHECK_EQUAL(65535.0, nTestLReal); 
#endif //FORTE_USE_64BIT_DATATYPES

}
#endif //FORTE_USE_REAL_DATATYPE


BOOST_AUTO_TEST_CASE(CASTS_WORD_to_TIME)
{
  CIEC_WORD nTestWord0;
  CIEC_WORD nTestWord1;
  CIEC_WORD nTestWord256;
  CIEC_WORD nTestWord65535;

  CIEC_TIME nTestTime;

//check initial values
  BOOST_CHECK_EQUAL(nTestWord0, 0U);
  BOOST_CHECK_EQUAL(nTestWord1, 0U);
  BOOST_CHECK_EQUAL(nTestWord256, 0U);
  BOOST_CHECK_EQUAL(nTestWord65535, 0U);

  nTestWord0=0U;
  nTestWord1=1U;
  nTestWord256=256U;
  nTestWord65535=65535U;

  BOOST_CHECK_EQUAL(nTestWord0, 0U);
  BOOST_CHECK_EQUAL(nTestWord1, 1U);
  BOOST_CHECK_EQUAL(nTestWord256, 256U);
  BOOST_CHECK_EQUAL(nTestWord65535, 65535U);

//toTime
  nTestTime.setValue(nTestWord0);
  BOOST_CHECK_EQUAL(0, nTestTime);
  nTestTime.setValue(nTestWord1);
  BOOST_CHECK_EQUAL(1, nTestTime);
  nTestTime.setValue(nTestWord256);
  BOOST_CHECK_EQUAL(256, nTestTime);
  nTestTime.setValue(nTestWord65535);
  BOOST_CHECK_EQUAL(65535, nTestTime);

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
