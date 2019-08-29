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
#ifdef FORTE_USE_64BIT_DATATYPES
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
//Time
#include "../../../src/core/datatypes/forte_time.h"


  #include "../../../src/core/datatypes/forte_lword.h"
  #include "../../../src/core/datatypes/forte_lint.h"
  #include "../../../src/core/datatypes/forte_ulint.h"

#ifdef FORTE_USE_REAL_DATATYPE
  #include "../../../src/core/datatypes/forte_lreal.h"
#endif //FORTE_USE_REAL_DATATYPE

using namespace boost::unit_test;

BOOST_AUTO_TEST_SUITE(CIEC_LWORD_casts_function_test)


BOOST_AUTO_TEST_CASE(CASTS_LWORD_to_BOOL)
{
  CIEC_LWORD nTestLWord0;
  CIEC_LWORD nTestLWord1;
  CIEC_LWORD nTestLWord4294967296;
  CIEC_LWORD nTestLWord18446744073709551615;

  CIEC_BOOL bTestBool;

//check initial values
  BOOST_CHECK_EQUAL(nTestLWord0, 0ULL);
  BOOST_CHECK_EQUAL(nTestLWord1, 0ULL);
  BOOST_CHECK_EQUAL(nTestLWord4294967296, 0ULL);
  BOOST_CHECK_EQUAL(nTestLWord18446744073709551615, 0ULL);

  nTestLWord0=0x00;
  nTestLWord1=0x01;
  nTestLWord4294967296=4294967296ULL;
  nTestLWord18446744073709551615=18446744073709551615ULL;

  BOOST_CHECK_EQUAL(nTestLWord0, 0U);
  BOOST_CHECK_EQUAL(nTestLWord1, 1U);
  BOOST_CHECK_EQUAL(nTestLWord4294967296, 4294967296ULL);
  BOOST_CHECK_EQUAL(nTestLWord18446744073709551615, 18446744073709551615ULL);

//checks
  bTestBool.setValue(nTestLWord0);
  BOOST_CHECK_EQUAL(bTestBool, false);
  bTestBool.setValue(nTestLWord1);
  BOOST_CHECK_EQUAL(bTestBool, true);
  bTestBool.setValue(nTestLWord4294967296);
  BOOST_CHECK_EQUAL(bTestBool, true);
  bTestBool.setValue(nTestLWord18446744073709551615);
  BOOST_CHECK_EQUAL(bTestBool, true);

}

BOOST_AUTO_TEST_CASE(CASTS_LWORD_to_BITDTs)
{
  CIEC_LWORD nTestLWord0;
  CIEC_LWORD nTestLWord1;
  CIEC_LWORD nTestLWord4294967296;
  CIEC_LWORD nTestLWord18446744073709551615;

  CIEC_BYTE nTestByte;
  CIEC_WORD nTestWord;
  CIEC_DWORD nTestDWord;

//check initial values
  BOOST_CHECK_EQUAL(nTestLWord0, 0ULL);
  BOOST_CHECK_EQUAL(nTestLWord1, 0ULL);
  BOOST_CHECK_EQUAL(nTestLWord4294967296, 0ULL);
  BOOST_CHECK_EQUAL(nTestLWord18446744073709551615, 0ULL);

  nTestLWord0=0;
  nTestLWord1=1;
  nTestLWord4294967296=4294967296LL;
  nTestLWord18446744073709551615=18446744073709551615ULL;

  BOOST_CHECK_EQUAL(nTestLWord0, 0ULL);
  BOOST_CHECK_EQUAL(nTestLWord1, 1ULL);
  BOOST_CHECK_EQUAL(nTestLWord4294967296, 4294967296ULL);
  BOOST_CHECK_EQUAL(nTestLWord18446744073709551615, 18446744073709551615ULL);

//toByte
  nTestByte.setValue(nTestLWord0);
  BOOST_CHECK_EQUAL(nTestByte, 0);
  nTestByte.setValue(nTestLWord1);
  BOOST_CHECK_EQUAL(nTestByte, 1);
  nTestByte.setValue(nTestLWord4294967296);
  BOOST_CHECK_EQUAL(nTestByte, 0);
  nTestByte.setValue(nTestLWord18446744073709551615);
  BOOST_CHECK_EQUAL(nTestByte, 255);

//toWord
  nTestWord.setValue(nTestLWord0);
  BOOST_CHECK_EQUAL(nTestWord, 0);
  nTestWord.setValue(nTestLWord1);
  BOOST_CHECK_EQUAL(nTestWord, 1);
  nTestWord.setValue(nTestLWord4294967296);
  BOOST_CHECK_EQUAL(nTestWord, 0);
  nTestWord.setValue(nTestLWord18446744073709551615);
  BOOST_CHECK_EQUAL(nTestWord, 65535);

//toDWord
  nTestDWord.setValue(nTestLWord0);
  BOOST_CHECK_EQUAL(nTestDWord, 0UL);
  nTestDWord.setValue(nTestLWord1);
  BOOST_CHECK_EQUAL(nTestDWord, 1UL);
  nTestDWord.setValue(nTestLWord4294967296);
  BOOST_CHECK_EQUAL(nTestDWord, 0UL);
  nTestDWord.setValue(nTestLWord18446744073709551615);
  BOOST_CHECK_EQUAL(nTestDWord, 4294967295UL);

}

BOOST_AUTO_TEST_CASE(CASTS_LWORD_to_INTS)
{
  CIEC_LWORD nTestLWord0;
  CIEC_LWORD nTestLWord1;
  CIEC_LWORD nTestLWord4294967296;
  CIEC_LWORD nTestLWord18446744073709551615;

  CIEC_SINT nTestSInt;
  CIEC_USINT nTestUSInt;
  CIEC_INT nTestInt;
  CIEC_UINT nTestUInt;
  CIEC_DINT nTestDInt;
  CIEC_UDINT nTestUDInt;
  CIEC_LINT nTestLInt;
  CIEC_ULINT nTestULInt;


  //check initial values
    BOOST_CHECK_EQUAL(nTestLWord0, 0ULL);
    BOOST_CHECK_EQUAL(nTestLWord1, 0ULL);
    BOOST_CHECK_EQUAL(nTestLWord4294967296, 0ULL);
    BOOST_CHECK_EQUAL(nTestLWord18446744073709551615, 0ULL);

    nTestLWord0=0ULL;
    nTestLWord1=1ULL;
    nTestLWord4294967296=4294967296ULL;
    nTestLWord18446744073709551615=18446744073709551615ULL;

    BOOST_CHECK_EQUAL(nTestLWord0, 0ULL);
    BOOST_CHECK_EQUAL(nTestLWord1, 1ULL);
    BOOST_CHECK_EQUAL(nTestLWord4294967296, 4294967296ULL);
    BOOST_CHECK_EQUAL(nTestLWord18446744073709551615, 18446744073709551615ULL);

//toSINT
  nTestSInt.setValue(nTestLWord0);
  BOOST_CHECK_EQUAL(nTestSInt, 0);
  nTestSInt.setValue(nTestLWord1);
  BOOST_CHECK_EQUAL(nTestSInt, 1);
  nTestSInt.setValue(nTestLWord4294967296);
  BOOST_CHECK_EQUAL(nTestSInt, 0);
  nTestSInt.setValue(nTestLWord18446744073709551615);
  BOOST_CHECK_EQUAL(nTestSInt, -1);

//toUSINT
  nTestUSInt.setValue(nTestLWord0);
  BOOST_CHECK_EQUAL(nTestUSInt, 0);
  nTestUSInt.setValue(nTestLWord1);
  BOOST_CHECK_EQUAL(nTestUSInt, 1);
  nTestUSInt.setValue(nTestLWord4294967296);
  BOOST_CHECK_EQUAL(nTestUSInt, 0);
  nTestUSInt.setValue(nTestLWord18446744073709551615);
  BOOST_CHECK_EQUAL(nTestUSInt, 255);


//toINT
  nTestInt.setValue(nTestLWord0);
  BOOST_CHECK_EQUAL(nTestInt, 0);
  nTestInt.setValue(nTestLWord1);
  BOOST_CHECK_EQUAL(nTestInt, 1);
  nTestInt.setValue(nTestLWord4294967296);
  BOOST_CHECK_EQUAL(nTestInt, 0);
  nTestInt.setValue(nTestLWord18446744073709551615);
  BOOST_CHECK_EQUAL(nTestInt, -1);

//toUINT
  nTestUInt.setValue(nTestLWord0);
  BOOST_CHECK_EQUAL(nTestUInt, 0);
  nTestUInt.setValue(nTestLWord1);
  BOOST_CHECK_EQUAL(nTestUInt, 1);
  nTestUInt.setValue(nTestLWord4294967296);
  BOOST_CHECK_EQUAL(nTestUInt, 0);
  nTestUInt.setValue(nTestLWord18446744073709551615);
  BOOST_CHECK_EQUAL(nTestUInt, 65535);

//toDINT
  nTestDInt.setValue(nTestLWord0);
  BOOST_CHECK_EQUAL(nTestDInt, 0);
  nTestDInt.setValue(nTestLWord1);
  BOOST_CHECK_EQUAL(nTestDInt, 1);
  nTestDInt.setValue(nTestLWord4294967296);
  BOOST_CHECK_EQUAL(nTestDInt, 0);
  nTestDInt.setValue(nTestLWord18446744073709551615);
  BOOST_CHECK_EQUAL(nTestDInt, -1);

//toUDINT
  nTestUDInt.setValue(nTestLWord0);
  BOOST_CHECK_EQUAL(nTestUDInt, 0U);
  nTestUDInt.setValue(nTestLWord1);
  BOOST_CHECK_EQUAL(nTestUDInt, 1U);
  nTestUDInt.setValue(nTestLWord4294967296);
  BOOST_CHECK_EQUAL(nTestUDInt, 0U);
  nTestUDInt.setValue(nTestLWord18446744073709551615);
  BOOST_CHECK_EQUAL(nTestUDInt, 4294967295UL);

//toLINT
  nTestLInt.setValue(nTestLWord0);
  BOOST_CHECK_EQUAL(nTestLInt, 0);
  nTestLInt.setValue(nTestLWord1);
  BOOST_CHECK_EQUAL(nTestLInt, 1);
  nTestLInt.setValue(nTestLWord4294967296);
  BOOST_CHECK_EQUAL(nTestLInt, 4294967296LL);
  nTestLInt.setValue(nTestLWord18446744073709551615);
  BOOST_CHECK_EQUAL(nTestLInt, -1LL);

//toULINT
  nTestULInt.setValue(nTestLWord0);
  BOOST_CHECK_EQUAL(nTestULInt, 0ULL);
  nTestULInt.setValue(nTestLWord1);
  BOOST_CHECK_EQUAL(nTestULInt, 1ULL);
  nTestULInt.setValue(nTestLWord4294967296);
  BOOST_CHECK_EQUAL(nTestULInt, 4294967296ULL);
  nTestULInt.setValue(nTestLWord18446744073709551615);
  BOOST_CHECK_EQUAL(nTestULInt, 18446744073709551615ULL);
}

#ifdef FORTE_USE_REAL_DATATYPE
BOOST_AUTO_TEST_CASE(CASTS_LWORD_to_REAL)
{
  CIEC_LWORD nTestLWord0;
  CIEC_LWORD nTestLWord1;
  CIEC_LWORD nTestLWord4294967296;
  CIEC_LWORD nTestLWord18446744073709551615;

  CIEC_REAL nTestReal;

//check initial values
  BOOST_CHECK_EQUAL(nTestLWord0, 0ULL);
  BOOST_CHECK_EQUAL(nTestLWord1, 0ULL);
  BOOST_CHECK_EQUAL(nTestLWord4294967296, 0ULL);
  BOOST_CHECK_EQUAL(nTestLWord18446744073709551615, 0ULL);

  nTestLWord0=0ULL;
  nTestLWord1=1ULL;
  nTestLWord4294967296=4294967296ULL;
  nTestLWord18446744073709551615=18446744073709551615ULL;

  BOOST_CHECK_EQUAL(nTestLWord0, 0ULL);
  BOOST_CHECK_EQUAL(nTestLWord1, 1ULL);
  BOOST_CHECK_EQUAL(nTestLWord4294967296, 4294967296ULL);
  BOOST_CHECK_EQUAL(nTestLWord18446744073709551615, 18446744073709551615ULL);

//toREAL
  nTestReal.setValue(nTestLWord0);
  BOOST_CHECK_EQUAL(0.0f, nTestReal);
  nTestReal.setValue(nTestLWord1);
  BOOST_CHECK_EQUAL(1.0f, nTestReal); 
  nTestReal.setValue(nTestLWord4294967296);
  BOOST_CHECK_EQUAL(4294967296.0f, nTestReal); 
  nTestReal.setValue(nTestLWord18446744073709551615);
  BOOST_CHECK_EQUAL(18446744073709551615.0f, nTestReal); 


  CIEC_LREAL nTestLReal;

//toLREAL
  nTestLReal.setValue(nTestLWord0);
  BOOST_CHECK_EQUAL(nTestLReal, 0.0);
  nTestLReal.setValue(nTestLWord1);
  BOOST_CHECK_EQUAL(nTestLReal, 1.0); 
  nTestLReal.setValue(nTestLWord4294967296);
  BOOST_CHECK_EQUAL(nTestLReal, 4294967296.0); 
  nTestLReal.setValue(nTestLWord18446744073709551615);
  BOOST_CHECK_EQUAL(nTestLReal, 18446744073709551615.0); 

}
#endif //FORTE_USE_REAL_DATATYPE


BOOST_AUTO_TEST_CASE(CASTS_LWORD_to_Time)
{
  CIEC_LWORD nTestLWord0;
  CIEC_LWORD nTestLWord1;
  CIEC_LWORD nTestLWord4294967296;
  CIEC_LWORD nTestLWord18446744073709551615;

  CIEC_TIME nTestTime;

//check initial values
  BOOST_CHECK_EQUAL(nTestLWord0, 0ULL);
  BOOST_CHECK_EQUAL(nTestLWord1, 0ULL);
  BOOST_CHECK_EQUAL(nTestLWord4294967296, 0ULL);
  BOOST_CHECK_EQUAL(nTestLWord18446744073709551615, 0ULL);

  nTestLWord0=0ULL;
  nTestLWord1=1ULL;
  nTestLWord4294967296=4294967296ULL;
  nTestLWord18446744073709551615=18446744073709551615ULL;

  BOOST_CHECK_EQUAL(nTestLWord0, 0ULL);
  BOOST_CHECK_EQUAL(nTestLWord1, 1ULL);
  BOOST_CHECK_EQUAL(nTestLWord4294967296, 4294967296ULL);
  BOOST_CHECK_EQUAL(nTestLWord18446744073709551615, 18446744073709551615ULL);

//toTime
#ifdef FORTE_USE_64BIT_DATATYPES
  nTestTime.setValue(nTestLWord0);
  BOOST_CHECK_EQUAL(0, nTestTime);
  nTestTime.setValue(nTestLWord1);
  BOOST_CHECK_EQUAL(1, nTestTime);
  nTestTime.setValue(nTestLWord4294967296);
  BOOST_CHECK_EQUAL(4294967296LL, nTestTime);
  nTestTime.setValue(nTestLWord18446744073709551615);
  BOOST_CHECK_EQUAL(-1LL, nTestTime);
#else
  nTestTime.setValue(nTestLWord0);
  BOOST_CHECK_EQUAL(0, nTestTime);
  nTestTime.setValue(nTestLWord1);
  BOOST_CHECK_EQUAL(1, nTestTime);
  nTestTime.setValue(nTestLWord4294967296);
  BOOST_CHECK_EQUAL(0, nTestTime);
  nTestTime.setValue(nTestLWord18446744073709551615);
  BOOST_CHECK_EQUAL(-1, nTestTime);
#endif
}


BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_CASE(LWORD_Castable_test)
{
  bool bUp, bDown;

  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_LWORD, CIEC_ANY::e_BOOL, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(bDown);

  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_LWORD, CIEC_ANY::e_SINT, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_LWORD, CIEC_ANY::e_INT, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_LWORD, CIEC_ANY::e_DINT, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_LWORD, CIEC_ANY::e_LINT, bUp, bDown)); BOOST_CHECK(bUp); BOOST_CHECK(! bDown);

  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_LWORD, CIEC_ANY::e_USINT, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_LWORD, CIEC_ANY::e_UINT, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_LWORD, CIEC_ANY::e_UDINT, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_LWORD, CIEC_ANY::e_ULINT, bUp, bDown)); BOOST_CHECK(bUp); BOOST_CHECK(! bDown);

  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_LWORD, CIEC_ANY::e_BYTE, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_LWORD, CIEC_ANY::e_WORD, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_LWORD, CIEC_ANY::e_DWORD, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_LWORD, CIEC_ANY::e_LWORD, bUp, bDown)); BOOST_CHECK(bUp); BOOST_CHECK(! bDown);

  BOOST_CHECK(!CIEC_ANY::isCastable(CIEC_ANY::e_LWORD, CIEC_ANY::e_DATE, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(!CIEC_ANY::isCastable(CIEC_ANY::e_LWORD, CIEC_ANY::e_TIME_OF_DAY, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(!CIEC_ANY::isCastable(CIEC_ANY::e_LWORD, CIEC_ANY::e_DATE_AND_TIME, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(!CIEC_ANY::isCastable(CIEC_ANY::e_LWORD, CIEC_ANY::e_TIME, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(! bDown);

  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_LWORD, CIEC_ANY::e_REAL, bUp, bDown)); BOOST_CHECK(bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_LWORD, CIEC_ANY::e_LREAL, bUp, bDown)); BOOST_CHECK(bUp); BOOST_CHECK(! bDown);

  BOOST_CHECK(!CIEC_ANY::isCastable(CIEC_ANY::e_LWORD, CIEC_ANY::e_STRING, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(!CIEC_ANY::isCastable(CIEC_ANY::e_LWORD, CIEC_ANY::e_WSTRING, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(! bDown);
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

  CIEC_LWORD nResult(0);

  nResult = CIEC_ANY::cast<CIEC_LWORD>(nUsint);
  BOOST_TEST(nResult == 2);
  nResult = CIEC_ANY::cast<CIEC_LWORD>(nUint);
  BOOST_TEST(nResult == 3);
  nResult = CIEC_ANY::cast<CIEC_LWORD>(nUdint);
  BOOST_TEST(nResult == 4);
  nResult = CIEC_ANY::cast<CIEC_LWORD>(nUlint);
  BOOST_TEST(nResult == 5);
  nResult = CIEC_ANY::cast<CIEC_LWORD>(nSint);
  BOOST_TEST(nResult == 6);
  nResult = CIEC_ANY::cast<CIEC_LWORD>(nInt);
  BOOST_TEST(nResult == 7);
  nResult = CIEC_ANY::cast<CIEC_LWORD>(nDint);
  BOOST_TEST(nResult == 8);
  nResult = CIEC_ANY::cast<CIEC_LWORD>(nLint);
  BOOST_TEST(nResult == 9);
  nResult = CIEC_ANY::cast<CIEC_LWORD>(nLreal);
  BOOST_TEST(nResult == 0x4026000000000000);
  nResult = CIEC_ANY::cast<CIEC_LWORD>(nByte);
  BOOST_TEST(nResult == 12);
  nResult = CIEC_ANY::cast<CIEC_LWORD>(nWord);
  BOOST_TEST(nResult == 13);
  nResult = CIEC_ANY::cast<CIEC_LWORD>(nDword);
  BOOST_TEST(nResult == 14);
  nResult = CIEC_ANY::cast<CIEC_LWORD>(nLword);
  BOOST_TEST(nResult == 15);
  nResult = CIEC_ANY::cast<CIEC_LWORD>(bBool);
  BOOST_TEST(nResult == true);
}

#endif //FORTE_USE_64BIT_DATATYPES
