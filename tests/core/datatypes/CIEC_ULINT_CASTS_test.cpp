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
//time
#include "../../../src/core/datatypes/forte_time.h"



  #include "../../../src/core/datatypes/forte_lword.h"
  #include "../../../src/core/datatypes/forte_lint.h"
  #include "../../../src/core/datatypes/forte_ulint.h"

#ifdef FORTE_USE_REAL_DATATYPE
  #include "../../../src/core/datatypes/forte_lreal.h"
#endif //FORTE_USE_REAL_DATATYPE


using namespace boost::unit_test;

BOOST_AUTO_TEST_SUITE(CIEC_ULINT_casts_function_test)


BOOST_AUTO_TEST_CASE(CASTS_ULINT_to_BOOL)
{
  CIEC_ULINT nTestULInt0;
  CIEC_ULINT nTestULInt1;
  CIEC_ULINT nTestULInt2147483648;
  CIEC_ULINT nTestULInt18446744073709551615;

  CIEC_BOOL bTestBool;

//check initial values
  BOOST_CHECK_EQUAL(nTestULInt0, 0ULL);
  BOOST_CHECK_EQUAL(nTestULInt1, 0ULL);
  BOOST_CHECK_EQUAL(nTestULInt2147483648, 0ULL);
  BOOST_CHECK_EQUAL(nTestULInt18446744073709551615, 0ULL);

  nTestULInt0=0ULL;
  nTestULInt1=1ULL;
  nTestULInt2147483648=2147483648ULL;
  nTestULInt18446744073709551615=18446744073709551615ULL;

  BOOST_CHECK_EQUAL(nTestULInt0, 0ULL);
  BOOST_CHECK_EQUAL(nTestULInt1, 1ULL);
  BOOST_CHECK_EQUAL(nTestULInt2147483648, 2147483648ULL);
  BOOST_CHECK_EQUAL(nTestULInt18446744073709551615, 18446744073709551615ULL);

//checks
  bTestBool.setValue(nTestULInt0);
  BOOST_CHECK_EQUAL(false, bTestBool);
  bTestBool.setValue(nTestULInt1);
  BOOST_CHECK_EQUAL(true, bTestBool);
  bTestBool.setValue(nTestULInt2147483648);
  BOOST_CHECK_EQUAL(true, bTestBool);
  bTestBool.setValue(nTestULInt18446744073709551615);
  BOOST_CHECK_EQUAL(true, bTestBool);

}

BOOST_AUTO_TEST_CASE(CASTS_ULINT_to_BITDTs)
{
  CIEC_ULINT nTestULInt0;
  CIEC_ULINT nTestULInt1;
  CIEC_ULINT nTestULInt2147483648;
  CIEC_ULINT nTestULInt18446744073709551615;

  CIEC_BYTE nTestByte;
  CIEC_WORD nTestWord;
  CIEC_DWORD nTestDWord;
  CIEC_LWORD nTestLWord;

//check initial values
  BOOST_CHECK_EQUAL(nTestULInt0, 0ULL);
  BOOST_CHECK_EQUAL(nTestULInt1, 0ULL);
  BOOST_CHECK_EQUAL(nTestULInt2147483648, 0ULL);
  BOOST_CHECK_EQUAL(nTestULInt18446744073709551615, 0ULL);

  nTestULInt0=0ULL;
  nTestULInt1=1ULL;
  nTestULInt2147483648=2147483648ULL;
  nTestULInt18446744073709551615=18446744073709551615ULL;

  BOOST_CHECK_EQUAL(nTestULInt0, 0ULL);
  BOOST_CHECK_EQUAL(nTestULInt1, 1ULL);
  BOOST_CHECK_EQUAL(nTestULInt2147483648, 2147483648ULL);
  BOOST_CHECK_EQUAL(nTestULInt18446744073709551615, 18446744073709551615ULL);

//toByte
  nTestByte.setValue(nTestULInt0);
  BOOST_CHECK_EQUAL(nTestByte, 0U);
  nTestByte.setValue(nTestULInt1);
  BOOST_CHECK_EQUAL(nTestByte, 1U);
  nTestByte.setValue(nTestULInt2147483648);
  BOOST_CHECK_EQUAL(nTestByte, 0U);
  nTestByte.setValue(nTestULInt18446744073709551615);
  BOOST_CHECK_EQUAL(nTestByte, 255U);

//toWord
  nTestWord.setValue(nTestULInt0);
  BOOST_CHECK_EQUAL(nTestWord, 0U);
  nTestWord.setValue(nTestULInt1);
  BOOST_CHECK_EQUAL(nTestWord, 1U);
  nTestWord.setValue(nTestULInt2147483648);
  BOOST_CHECK_EQUAL(nTestWord, 0U);
  nTestWord.setValue(nTestULInt18446744073709551615);
  BOOST_CHECK_EQUAL(nTestWord, 65535U);

//toDWord
  nTestDWord.setValue(nTestULInt0);
  BOOST_CHECK_EQUAL(nTestDWord, 0UL);
  nTestDWord.setValue(nTestULInt1);
  BOOST_CHECK_EQUAL(nTestDWord, 1UL);
  nTestDWord.setValue(nTestULInt2147483648);
  BOOST_CHECK_EQUAL(nTestDWord, 2147483648UL);
  nTestDWord.setValue(nTestULInt18446744073709551615);
  BOOST_CHECK_EQUAL(nTestDWord, 4294967295UL);


//toLWord
  nTestLWord.setValue(nTestULInt0);
  BOOST_CHECK_EQUAL(nTestLWord, 0ULL);
  nTestLWord.setValue(nTestULInt1);
  BOOST_CHECK_EQUAL(nTestLWord, 1ULL);
  nTestLWord.setValue(nTestULInt2147483648);
  BOOST_CHECK_EQUAL(nTestLWord, 2147483648ULL);
  nTestLWord.setValue(nTestULInt18446744073709551615);
  BOOST_CHECK_EQUAL(nTestLWord, 18446744073709551615ULL);

}

BOOST_AUTO_TEST_CASE(CASTS_ULINT_to_INTS)
{
  CIEC_ULINT nTestULInt0;
  CIEC_ULINT nTestULInt1;
  CIEC_ULINT nTestULInt2147483648;
  CIEC_ULINT nTestULInt18446744073709551615;

  CIEC_USINT nTestUSInt;
  CIEC_SINT nTestSInt;
  CIEC_UINT nTestUInt;
  CIEC_INT nTestInt;
  CIEC_DINT nTestDInt;
  CIEC_UDINT nTestUDInt;
  CIEC_LINT nTestLInt;

  //check initial values
    BOOST_CHECK_EQUAL(nTestULInt0, 0ULL);
    BOOST_CHECK_EQUAL(nTestULInt1, 0ULL);
    BOOST_CHECK_EQUAL(nTestULInt2147483648, 0ULL);
    BOOST_CHECK_EQUAL(nTestULInt18446744073709551615, 0ULL);

    nTestULInt0=0;
    nTestULInt1=1;
    nTestULInt2147483648=2147483648ULL;
    nTestULInt18446744073709551615=18446744073709551615ULL;

    BOOST_CHECK_EQUAL(nTestULInt0, 0ULL);
    BOOST_CHECK_EQUAL(nTestULInt1, 1ULL);
    BOOST_CHECK_EQUAL(nTestULInt2147483648, 2147483648ULL);
    BOOST_CHECK_EQUAL(nTestULInt18446744073709551615, 18446744073709551615ULL);


//toUSINT
  nTestUSInt.setValue(nTestULInt0);
  BOOST_CHECK_EQUAL(nTestUSInt, 0U);
  nTestUSInt.setValue(nTestULInt1);
  BOOST_CHECK_EQUAL(nTestUSInt, 1U);
  nTestUSInt.setValue(nTestULInt2147483648);
  BOOST_CHECK_EQUAL(nTestUSInt, 0U);
  nTestUSInt.setValue(nTestULInt18446744073709551615);
  BOOST_CHECK_EQUAL(nTestUSInt, 255U);

//toSINT
  nTestSInt.setValue(nTestULInt0);
  BOOST_CHECK_EQUAL(nTestSInt, 0);
  nTestSInt.setValue(nTestULInt1);
  BOOST_CHECK_EQUAL(nTestSInt, 1);
  nTestSInt.setValue(nTestULInt2147483648);
  BOOST_CHECK_EQUAL(nTestSInt, 0);
  nTestSInt.setValue(nTestULInt18446744073709551615);
  BOOST_CHECK_EQUAL(nTestSInt, -1);

//toUINT
  nTestUInt.setValue(nTestULInt0);
  BOOST_CHECK_EQUAL(nTestUInt, 0U);
  nTestUInt.setValue(nTestULInt1);
  BOOST_CHECK_EQUAL(nTestUInt, 1U);
  nTestUInt.setValue(nTestULInt2147483648);
  BOOST_CHECK_EQUAL(nTestUInt, 0U);
  nTestUInt.setValue(nTestULInt18446744073709551615);
  BOOST_CHECK_EQUAL(nTestUInt, 65535U);


//toINT
  nTestInt.setValue(nTestULInt0);
  BOOST_CHECK_EQUAL(nTestInt, 0);
  nTestInt.setValue(nTestULInt1);
  BOOST_CHECK_EQUAL(nTestInt, 1);
  nTestInt.setValue(nTestULInt2147483648);
  BOOST_CHECK_EQUAL(nTestInt, 0);
  nTestInt.setValue(nTestULInt18446744073709551615);
  BOOST_CHECK_EQUAL(nTestInt, -1);

//toDINT
  nTestDInt.setValue(nTestULInt0);
  BOOST_CHECK_EQUAL(nTestDInt, 0);
  nTestDInt.setValue(nTestULInt1);
  BOOST_CHECK_EQUAL(nTestDInt, 1);
  nTestDInt.setValue(nTestULInt2147483648);
  BOOST_CHECK_EQUAL(nTestDInt, -2147483648LL);
  nTestDInt.setValue(nTestULInt18446744073709551615);
  BOOST_CHECK_EQUAL(nTestDInt, -1);

  //toUDINT
  nTestUDInt.setValue(nTestULInt0);
  BOOST_CHECK_EQUAL(nTestUDInt, 0U);
  nTestUDInt.setValue(nTestULInt1);
  BOOST_CHECK_EQUAL(nTestUDInt, 1U);
  nTestUDInt.setValue(nTestULInt2147483648);
  BOOST_CHECK_EQUAL(nTestUDInt, 2147483648U);
  nTestUDInt.setValue(nTestULInt18446744073709551615);
  BOOST_CHECK_EQUAL(nTestUDInt, 4294967295UL);


//toLINT
  nTestLInt.setValue(nTestULInt0);
  BOOST_CHECK_EQUAL(nTestLInt, 0);
  nTestLInt.setValue(nTestULInt1);
  BOOST_CHECK_EQUAL(nTestLInt, 1);
  nTestLInt.setValue(nTestULInt2147483648);
  BOOST_CHECK_EQUAL(nTestLInt, 2147483648LL);
  nTestLInt.setValue(nTestULInt18446744073709551615);
  BOOST_CHECK_EQUAL(nTestLInt, -1LL);


}

#ifdef FORTE_USE_REAL_DATATYPE
BOOST_AUTO_TEST_CASE(CASTS_ULINT_to_REAL)
{
  CIEC_ULINT nTestULInt0;
  CIEC_ULINT nTestULInt1;
  CIEC_ULINT nTestULInt2147483648;
  CIEC_ULINT nTestULInt18446744073709551615;

  CIEC_REAL nTestReal;

//check initial values
  BOOST_CHECK_EQUAL(nTestULInt0, 0ULL);
  BOOST_CHECK_EQUAL(nTestULInt1, 0ULL);
  BOOST_CHECK_EQUAL(nTestULInt2147483648, 0ULL);
  BOOST_CHECK_EQUAL(nTestULInt18446744073709551615, 0ULL);

  nTestULInt0=0ULL;
  nTestULInt1=1ULL;
  nTestULInt2147483648=2147483648ULL;
  nTestULInt18446744073709551615=18446744073709551615ULL;

  BOOST_CHECK_EQUAL(nTestULInt0, 0ULL);
  BOOST_CHECK_EQUAL(nTestULInt1, 1ULL);
  BOOST_CHECK_EQUAL(nTestULInt2147483648, 2147483648ULL);
  BOOST_CHECK_EQUAL(nTestULInt18446744073709551615, 18446744073709551615ULL);

//toREAL
  nTestReal.setValue(nTestULInt0);
  BOOST_CHECK_EQUAL(0.0f, nTestReal);
  nTestReal.setValue(nTestULInt1);
  BOOST_CHECK_EQUAL(1.0f, nTestReal); 
  nTestReal.setValue(nTestULInt2147483648);
  BOOST_CHECK_EQUAL(2147483648.0f, nTestReal); 
  nTestReal.setValue(nTestULInt18446744073709551615);
  BOOST_CHECK_EQUAL(18446744073709551615.0f, nTestReal); 



  CIEC_LREAL nTestLReal;

//toLREAL
  nTestLReal.setValue(nTestULInt0);
  BOOST_CHECK_EQUAL(0.0, nTestLReal);
  nTestLReal.setValue(nTestULInt1);
  BOOST_CHECK_EQUAL(1.0, nTestLReal); 
  nTestLReal.setValue(nTestULInt2147483648);
  BOOST_CHECK_EQUAL(2147483648.0, nTestLReal); 
  nTestLReal.setValue(nTestULInt18446744073709551615);
  BOOST_CHECK_EQUAL(18446744073709551615.0, nTestLReal); 
}
#endif //FORTE_USE_REAL_DATATYPE

BOOST_AUTO_TEST_CASE(CASTS_ULINT_to_TIME)
{
  CIEC_ULINT nTestULInt0;
  CIEC_ULINT nTestULInt1;
  CIEC_ULINT nTestULInt2147483648;
  CIEC_ULINT nTestULInt18446744073709551615;

  CIEC_TIME nTestTime;

//check initial values
  BOOST_CHECK_EQUAL(nTestULInt0, 0ULL);
  BOOST_CHECK_EQUAL(nTestULInt1, 0ULL);
  BOOST_CHECK_EQUAL(nTestULInt2147483648, 0ULL);
  BOOST_CHECK_EQUAL(nTestULInt18446744073709551615, 0ULL);

  nTestULInt0=0ULL;
  nTestULInt1=1ULL;
  nTestULInt2147483648=2147483648ULL;
  nTestULInt18446744073709551615=18446744073709551615ULL;

  BOOST_CHECK_EQUAL(nTestULInt0, 0ULL);
  BOOST_CHECK_EQUAL(nTestULInt1, 1ULL);
  BOOST_CHECK_EQUAL(nTestULInt2147483648, 2147483648ULL);
  BOOST_CHECK_EQUAL(nTestULInt18446744073709551615, 18446744073709551615ULL);

//toTime
  nTestTime.setValue(nTestULInt0);
  BOOST_CHECK_EQUAL(0, nTestTime);
  nTestTime.setValue(nTestULInt1);
  BOOST_CHECK_EQUAL(1, nTestTime);
  nTestTime.setValue(nTestULInt2147483648);
  BOOST_CHECK_EQUAL(2147483648L, nTestTime);
  nTestTime.setValue(nTestULInt18446744073709551615);
  BOOST_CHECK_EQUAL(-1, nTestTime);
}

BOOST_AUTO_TEST_CASE(ULINT_Castable_test)
{
  bool bUp, bDown;

  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_ULINT, CIEC_ANY::e_BOOL, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(bDown);

  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_ULINT, CIEC_ANY::e_SINT, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_ULINT, CIEC_ANY::e_INT, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_ULINT, CIEC_ANY::e_DINT, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_ULINT, CIEC_ANY::e_LINT, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(bDown);

  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_ULINT, CIEC_ANY::e_USINT, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_ULINT, CIEC_ANY::e_UINT, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_ULINT, CIEC_ANY::e_UDINT, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_ULINT, CIEC_ANY::e_ULINT, bUp, bDown)); BOOST_CHECK(bUp); BOOST_CHECK(! bDown);

  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_ULINT, CIEC_ANY::e_BYTE, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_ULINT, CIEC_ANY::e_WORD, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_ULINT, CIEC_ANY::e_DWORD, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_ULINT, CIEC_ANY::e_LWORD, bUp, bDown)); BOOST_CHECK(bUp); BOOST_CHECK(! bDown);

  BOOST_CHECK(!CIEC_ANY::isCastable(CIEC_ANY::e_ULINT, CIEC_ANY::e_DATE, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(!CIEC_ANY::isCastable(CIEC_ANY::e_ULINT, CIEC_ANY::e_TIME_OF_DAY, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(!CIEC_ANY::isCastable(CIEC_ANY::e_ULINT, CIEC_ANY::e_DATE_AND_TIME, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(!CIEC_ANY::isCastable(CIEC_ANY::e_ULINT, CIEC_ANY::e_TIME, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(! bDown);

  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_ULINT, CIEC_ANY::e_REAL, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_ULINT, CIEC_ANY::e_LREAL, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(bDown);

  BOOST_CHECK(!CIEC_ANY::isCastable(CIEC_ANY::e_ULINT, CIEC_ANY::e_STRING, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(!CIEC_ANY::isCastable(CIEC_ANY::e_ULINT, CIEC_ANY::e_WSTRING, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(! bDown);
}

BOOST_AUTO_TEST_CASE(Explict_cast_operator_to_ULINT)
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

  CIEC_UINT nResult(0);

  nResult = CIEC_ANY::cast<CIEC_ULINT>(nUsint);
  BOOST_TEST(nResult == 2);
  nResult = CIEC_ANY::cast<CIEC_ULINT>(nUint);
  BOOST_TEST(nResult == 3);
  nResult = CIEC_ANY::cast<CIEC_ULINT>(nUdint);
  BOOST_TEST(nResult == 4);
  nResult = CIEC_ANY::cast<CIEC_ULINT>(nUlint);
  BOOST_TEST(nResult == 5);
  nResult = CIEC_ANY::cast<CIEC_ULINT>(nSint);
  BOOST_TEST(nResult == 6);
  nResult = CIEC_ANY::cast<CIEC_ULINT>(nInt);
  BOOST_TEST(nResult == 7);
  nResult = CIEC_ANY::cast<CIEC_ULINT>(nDint);
  BOOST_TEST(nResult == 8);
  nResult = CIEC_ANY::cast<CIEC_ULINT>(nLint);
  BOOST_TEST(nResult == 9);
  nResult = CIEC_ANY::cast<CIEC_ULINT>(nReal);
  BOOST_TEST(nResult == 10);
  nResult = CIEC_ANY::cast<CIEC_ULINT>(nLreal);
  BOOST_TEST(nResult == 11);
  nResult = CIEC_ANY::cast<CIEC_ULINT>(nByte);
  BOOST_TEST(nResult == 12);
  nResult = CIEC_ANY::cast<CIEC_ULINT>(nWord);
  BOOST_TEST(nResult == 13);
  nResult = CIEC_ANY::cast<CIEC_ULINT>(nDword);
  BOOST_TEST(nResult == 14);
  nResult = CIEC_ANY::cast<CIEC_ULINT>(nLword);
  BOOST_TEST(nResult == 15);
  nResult = CIEC_ANY::cast<CIEC_ULINT>(bBool);
  BOOST_TEST(nResult == true);
}

BOOST_AUTO_TEST_SUITE_END()
#endif //FORTE_USE_64BIT_DATATYPES
