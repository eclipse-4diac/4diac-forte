/*******************************************************************************
 * Copyright (c) 2011 - 2013 ACIN, nxtControl, fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Martin Melik Merkumians, Ingo Hegny, Alois Zoitl, Stanislav Meduna - initial API and implementation and/or initial documentation
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
//TIME
#include "../../../src/core/datatypes/forte_time.h"



  #include "../../../src/core/datatypes/forte_lword.h"
  #include "../../../src/core/datatypes/forte_lint.h"
  #include "../../../src/core/datatypes/forte_ulint.h"

#ifdef FORTE_USE_REAL_DATATYPE
  #include "../../../src/core/datatypes/forte_lreal.h"
#endif //FORTE_USE_REAL_DATATYPE


using namespace boost::unit_test;

BOOST_AUTO_TEST_SUITE(CIEC_LINT_casts_function_test)


BOOST_AUTO_TEST_CASE(CASTS_LINT_to_BOOL)
{
  CIEC_LINT nTestLInt0;
  CIEC_LINT nTestLInt1;
  CIEC_LINT nTestLInt2147483648;
  CIEC_LINT nTestLInt9223372036854775807;

  CIEC_BOOL bTestBool;

//check initial values
  BOOST_CHECK_EQUAL(nTestLInt0, 0);
  BOOST_CHECK_EQUAL(nTestLInt1, 0);
  BOOST_CHECK_EQUAL(nTestLInt2147483648, 0);
  BOOST_CHECK_EQUAL(nTestLInt9223372036854775807, 0);

  nTestLInt0=0;
  nTestLInt1=1;
  nTestLInt2147483648=2147483648LL;
  nTestLInt9223372036854775807=9223372036854775807LL;

  BOOST_CHECK_EQUAL(nTestLInt0, 0);
  BOOST_CHECK_EQUAL(nTestLInt1, 1);
  BOOST_CHECK_EQUAL(nTestLInt2147483648, 2147483648LL);
  BOOST_CHECK_EQUAL(nTestLInt9223372036854775807, 9223372036854775807LL);

//checks
  bTestBool.setValue(nTestLInt0);
  BOOST_CHECK_EQUAL(false, bTestBool);
  bTestBool.setValue(nTestLInt1);
  BOOST_CHECK_EQUAL(true, bTestBool);
  bTestBool.setValue(nTestLInt2147483648);
  BOOST_CHECK_EQUAL(true, bTestBool);
  bTestBool.setValue(nTestLInt9223372036854775807);
  BOOST_CHECK_EQUAL(true, bTestBool);

}

BOOST_AUTO_TEST_CASE(CASTS_LINT_to_BITDTs)
{
  CIEC_LINT nTestLInt0;
  CIEC_LINT nTestLInt1;
  CIEC_LINT nTestLInt2147483648;
  CIEC_LINT nTestLInt9223372036854775807;

  CIEC_BYTE nTestByte;
  CIEC_WORD nTestWord;
  CIEC_DWORD nTestDWord;
  CIEC_LWORD nTestLWord;

//check initial values
  BOOST_CHECK_EQUAL(nTestLInt0, 0);
  BOOST_CHECK_EQUAL(nTestLInt1, 0);
  BOOST_CHECK_EQUAL(nTestLInt2147483648, 0);
  BOOST_CHECK_EQUAL(nTestLInt9223372036854775807, 0);

  nTestLInt0=0;
  nTestLInt1=1;
  nTestLInt2147483648=2147483648LL;
  nTestLInt9223372036854775807=9223372036854775807LL;

  BOOST_CHECK_EQUAL(nTestLInt0, 0);
  BOOST_CHECK_EQUAL(nTestLInt1, 1);
  BOOST_CHECK_EQUAL(nTestLInt2147483648, 2147483648LL);
  BOOST_CHECK_EQUAL(nTestLInt9223372036854775807, 9223372036854775807LL);

//toByte
  nTestByte.setValue(nTestLInt0);
  BOOST_CHECK_EQUAL(nTestByte, 0U);
  nTestByte.setValue(nTestLInt1);
  BOOST_CHECK_EQUAL(nTestByte, 1U);
  nTestByte.setValue(nTestLInt2147483648);
  BOOST_CHECK_EQUAL(nTestByte, 0U);
  nTestByte.setValue(nTestLInt9223372036854775807);
  BOOST_CHECK_EQUAL(nTestByte, 255U);

//toWord
  nTestWord.setValue(nTestLInt0);
  BOOST_CHECK_EQUAL(nTestWord, 0U);
  nTestWord.setValue(nTestLInt1);
  BOOST_CHECK_EQUAL(nTestWord, 1U);
  nTestWord.setValue(nTestLInt2147483648);
  BOOST_CHECK_EQUAL(nTestWord, 0U);
  nTestWord.setValue(nTestLInt9223372036854775807);
  BOOST_CHECK_EQUAL(nTestWord, 65535U);

//toDWord
  nTestDWord.setValue(nTestLInt0);
  BOOST_CHECK_EQUAL(nTestDWord, 0UL);
  nTestDWord.setValue(nTestLInt1);
  BOOST_CHECK_EQUAL(nTestDWord, 1UL);
  nTestDWord.setValue(nTestLInt2147483648);
  BOOST_CHECK_EQUAL(nTestDWord, 2147483648UL);
  nTestDWord.setValue(nTestLInt9223372036854775807);
  BOOST_CHECK_EQUAL(nTestDWord, 4294967295UL);


//toLWord
  nTestLWord.setValue(nTestLInt0);
  BOOST_CHECK_EQUAL(nTestLWord, 0ULL);
  nTestLWord.setValue(nTestLInt1);
  BOOST_CHECK_EQUAL(nTestLWord, 1ULL);
  nTestLWord.setValue(nTestLInt2147483648);
  BOOST_CHECK_EQUAL(nTestLWord, 2147483648ULL);
  nTestLWord.setValue(nTestLInt9223372036854775807);
  BOOST_CHECK_EQUAL(nTestLWord, 9223372036854775807ULL);

}

BOOST_AUTO_TEST_CASE(CASTS_LINT_to_INTS)
{
  CIEC_LINT nTestLInt0;
  CIEC_LINT nTestLInt1;
  CIEC_LINT nTestLInt2147483648;
  CIEC_LINT nTestLInt9223372036854775807;
  CIEC_LINT nTestLInt_1;

  CIEC_USINT nTestUSInt;
  CIEC_SINT nTestSInt;
  CIEC_UINT nTestUInt;
  CIEC_INT nTestInt;
  CIEC_DINT nTestDInt;
  CIEC_UDINT nTestUDInt;
  CIEC_ULINT nTestULInt;

  //check initial values
    BOOST_CHECK_EQUAL(nTestLInt0, 0);
    BOOST_CHECK_EQUAL(nTestLInt1, 0);
    BOOST_CHECK_EQUAL(nTestLInt2147483648, 0);
    BOOST_CHECK_EQUAL(nTestLInt9223372036854775807, 0);
    BOOST_CHECK_EQUAL(nTestLInt_1, 0);

    nTestLInt0=0;
    nTestLInt1=1;
    nTestLInt2147483648=2147483648LL;
    nTestLInt9223372036854775807=9223372036854775807LL;
    nTestLInt_1=-1LL;

    BOOST_CHECK_EQUAL(nTestLInt0, 0);
    BOOST_CHECK_EQUAL(nTestLInt1, 1);
    BOOST_CHECK_EQUAL(nTestLInt2147483648, 2147483648LL);
    BOOST_CHECK_EQUAL(nTestLInt9223372036854775807, 9223372036854775807LL);
    BOOST_CHECK_EQUAL(nTestLInt_1, -1);

//toUSINT
  nTestUSInt.setValue(nTestLInt0);
  BOOST_CHECK_EQUAL(nTestUSInt, 0U);
  nTestUSInt.setValue(nTestLInt1);
  BOOST_CHECK_EQUAL(nTestUSInt, 1U);
  nTestUSInt.setValue(nTestLInt2147483648);
  BOOST_CHECK_EQUAL(nTestUSInt, 0U);
  nTestUSInt.setValue(nTestLInt9223372036854775807);
  BOOST_CHECK_EQUAL(nTestUSInt, 255U);
  nTestUSInt.setValue(nTestLInt_1);
  BOOST_CHECK_EQUAL(nTestUSInt, 255U);


//toSINT
  nTestSInt.setValue(nTestLInt0);
  BOOST_CHECK_EQUAL(nTestSInt, 0);
  nTestSInt.setValue(nTestLInt1);
  BOOST_CHECK_EQUAL(nTestSInt, 1);
  nTestSInt.setValue(nTestLInt2147483648);
  BOOST_CHECK_EQUAL(nTestSInt, 0);
  nTestSInt.setValue(nTestLInt9223372036854775807);
  BOOST_CHECK_EQUAL(nTestSInt, -1);
  nTestSInt.setValue(nTestLInt_1);
  BOOST_CHECK_EQUAL(nTestSInt, -1);

//toUINT
  nTestUInt.setValue(nTestLInt0);
  BOOST_CHECK_EQUAL(nTestUInt, 0U);
  nTestUInt.setValue(nTestLInt1);
  BOOST_CHECK_EQUAL(nTestUInt, 1U);
  nTestUInt.setValue(nTestLInt2147483648);
  BOOST_CHECK_EQUAL(nTestUInt, 0U);
  nTestUInt.setValue(nTestLInt9223372036854775807);
  BOOST_CHECK_EQUAL(nTestUInt, 65535U);
  nTestUInt.setValue(nTestLInt_1);
  BOOST_CHECK_EQUAL(nTestUInt, 65535U);

//toINT
  nTestInt.setValue(nTestLInt0);
  BOOST_CHECK_EQUAL(nTestInt, 0);
  nTestInt.setValue(nTestLInt1);
  BOOST_CHECK_EQUAL(nTestInt, 1);
  nTestInt.setValue(nTestLInt2147483648);
  BOOST_CHECK_EQUAL(nTestInt, 0);
  nTestInt.setValue(nTestLInt9223372036854775807);
  BOOST_CHECK_EQUAL(nTestInt, -1);
  nTestInt.setValue(nTestLInt_1);
  BOOST_CHECK_EQUAL(nTestInt, -1);

//toDINT
  nTestDInt.setValue(nTestLInt0);
  BOOST_CHECK_EQUAL(nTestDInt, 0);
  nTestDInt.setValue(nTestLInt1);
  BOOST_CHECK_EQUAL(nTestDInt, 1);
  nTestDInt.setValue(nTestLInt2147483648);
  BOOST_CHECK_EQUAL(nTestDInt, -2147483648LL);
  nTestDInt.setValue(nTestLInt9223372036854775807);
  BOOST_CHECK_EQUAL(nTestDInt, -1);
  nTestDInt.setValue(nTestLInt_1);
  BOOST_CHECK_EQUAL(nTestDInt, -1);

  //toUDINT
  nTestUDInt.setValue(nTestLInt0);
  BOOST_CHECK_EQUAL(nTestUDInt, 0UL);
  nTestUDInt.setValue(nTestLInt1);
  BOOST_CHECK_EQUAL(nTestUDInt, 1UL);
  nTestUDInt.setValue(nTestLInt2147483648);
  BOOST_CHECK_EQUAL(nTestUDInt, 2147483648UL);
  nTestUDInt.setValue(nTestLInt9223372036854775807);
  BOOST_CHECK_EQUAL(nTestUDInt, 4294967295UL);


//toULINT
  nTestULInt.setValue(nTestLInt0);
  BOOST_CHECK_EQUAL(nTestULInt, 0ULL);
  nTestULInt.setValue(nTestLInt1);
  BOOST_CHECK_EQUAL(nTestULInt, 1ULL);
  nTestULInt.setValue(nTestLInt2147483648);
  BOOST_CHECK_EQUAL(nTestULInt, 2147483648ULL);
  nTestULInt.setValue(nTestLInt9223372036854775807);
  BOOST_CHECK_EQUAL(nTestULInt, 9223372036854775807ULL);
  nTestULInt.setValue(nTestLInt_1);
  BOOST_CHECK_EQUAL(nTestULInt, 18446744073709551615ULL);

}

#ifdef FORTE_USE_REAL_DATATYPE
BOOST_AUTO_TEST_CASE(CASTS_LINT_to_REAL)
{
  CIEC_LINT nTestLInt0;
  CIEC_LINT nTestLInt1;
  CIEC_LINT nTestLInt2147483648;
  CIEC_LINT nTestLInt9223372036854775807;

  CIEC_REAL nTestReal;

//check initial values
  BOOST_CHECK_EQUAL(nTestLInt0, 0);
  BOOST_CHECK_EQUAL(nTestLInt1, 0);
  BOOST_CHECK_EQUAL(nTestLInt2147483648, 0);
  BOOST_CHECK_EQUAL(nTestLInt9223372036854775807, 0);

  nTestLInt0=0LL;
  nTestLInt1=1LL;
  nTestLInt2147483648=2147483648LL;
  nTestLInt9223372036854775807=9223372036854775807LL;

  BOOST_CHECK_EQUAL(nTestLInt0, 0LL);
  BOOST_CHECK_EQUAL(nTestLInt1, 1LL);
  BOOST_CHECK_EQUAL(nTestLInt2147483648, 2147483648LL);
  BOOST_CHECK_EQUAL(nTestLInt9223372036854775807, 9223372036854775807LL);

//toREAL
  nTestReal.setValue(nTestLInt0);
  BOOST_CHECK_EQUAL(0.0f, nTestReal);
  nTestReal.setValue(nTestLInt1);
  BOOST_CHECK_EQUAL(1.0f, nTestReal); 
  nTestReal.setValue(nTestLInt2147483648);
  BOOST_CHECK_EQUAL(2147483648.0f, nTestReal); 
  nTestReal.setValue(nTestLInt9223372036854775807);
  BOOST_CHECK_EQUAL(9223372036854775807.0f, nTestReal); 



  CIEC_LREAL nTestLReal;

//toLREAL
  nTestLReal.setValue(nTestLInt0);
  BOOST_CHECK_EQUAL(0.0, nTestLReal);
  nTestLReal.setValue(nTestLInt1);
  BOOST_CHECK_EQUAL(1.0, nTestLReal); 
  nTestLReal.setValue(nTestLInt2147483648);
  BOOST_CHECK_EQUAL(2147483648.0, nTestLReal); 
  nTestLReal.setValue(nTestLInt9223372036854775807);
  BOOST_CHECK_EQUAL(9223372036854775807.0, nTestLReal); 


}
#endif //FORTE_USE_REAL_DATATYPE

BOOST_AUTO_TEST_CASE(CASTS_LINT_to_TIME)
{
  CIEC_LINT nTestLInt0;
  CIEC_LINT nTestLInt1;
  CIEC_LINT nTestLInt2147483648;
  CIEC_LINT nTestLInt9223372036854775807;

  CIEC_TIME nTestTime;

//check initial values
  BOOST_CHECK_EQUAL(nTestLInt0, 0);
  BOOST_CHECK_EQUAL(nTestLInt1, 0);
  BOOST_CHECK_EQUAL(nTestLInt2147483648, 0);
  BOOST_CHECK_EQUAL(nTestLInt9223372036854775807, 0);

  nTestLInt0=0LL;
  nTestLInt1=1LL;
  nTestLInt2147483648=2147483648LL;
  nTestLInt9223372036854775807=9223372036854775807LL;

  BOOST_CHECK_EQUAL(nTestLInt0, 0LL);
  BOOST_CHECK_EQUAL(nTestLInt1, 1LL);
  BOOST_CHECK_EQUAL(nTestLInt2147483648, 2147483648LL);
  BOOST_CHECK_EQUAL(nTestLInt9223372036854775807, 9223372036854775807LL);

//toTIME
#ifdef FORTE_USE_64BIT_DATATYPES
  nTestTime.setValue(nTestLInt0);
  BOOST_CHECK_EQUAL(0L, nTestTime);
  nTestTime.setValue(nTestLInt1);
  BOOST_CHECK_EQUAL(1L, nTestTime);
  nTestTime.setValue(nTestLInt2147483648);
  BOOST_CHECK_EQUAL(2147483648LL, nTestTime);
  nTestTime.setValue(nTestLInt9223372036854775807);
  BOOST_CHECK_EQUAL(9223372036854775807LL, nTestTime);
#else
  nTestTime.setValue(nTestLInt0);
  BOOST_CHECK_EQUAL(0L, nTestTime);
  nTestTime.setValue(nTestLInt1);
  BOOST_CHECK_EQUAL(1L, nTestTime);
  nTestTime.setValue(nTestLInt2147483648);
  BOOST_CHECK_EQUAL(-2147483648L, nTestTime);
  nTestTime.setValue(nTestLInt9223372036854775807);
  BOOST_CHECK_EQUAL(-1L, nTestTime);
#endif
}

BOOST_AUTO_TEST_CASE(LINT_Castable_test)
{
  bool bUp, bDown;

  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_LINT, CIEC_ANY::e_BOOL, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(bDown);

  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_LINT, CIEC_ANY::e_SINT, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_LINT, CIEC_ANY::e_INT, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_LINT, CIEC_ANY::e_DINT, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_LINT, CIEC_ANY::e_LINT, bUp, bDown)); BOOST_CHECK(bUp); BOOST_CHECK(! bDown);

  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_LINT, CIEC_ANY::e_USINT, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_LINT, CIEC_ANY::e_UINT, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_LINT, CIEC_ANY::e_UDINT, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_LINT, CIEC_ANY::e_ULINT, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(bDown);

  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_LINT, CIEC_ANY::e_BYTE, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_LINT, CIEC_ANY::e_WORD, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_LINT, CIEC_ANY::e_DWORD, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_LINT, CIEC_ANY::e_LWORD, bUp, bDown)); BOOST_CHECK(bUp); BOOST_CHECK(! bDown);

  BOOST_CHECK(!CIEC_ANY::isCastable(CIEC_ANY::e_LINT, CIEC_ANY::e_DATE, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(!CIEC_ANY::isCastable(CIEC_ANY::e_LINT, CIEC_ANY::e_TIME_OF_DAY, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(!CIEC_ANY::isCastable(CIEC_ANY::e_LINT, CIEC_ANY::e_DATE_AND_TIME, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_LINT, CIEC_ANY::e_TIME, bUp, bDown)); BOOST_CHECK(bUp); BOOST_CHECK(! bDown);

  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_LINT, CIEC_ANY::e_REAL, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(bDown);
  BOOST_CHECK(CIEC_ANY::isCastable(CIEC_ANY::e_LINT, CIEC_ANY::e_LREAL, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(bDown);

  BOOST_CHECK(!CIEC_ANY::isCastable(CIEC_ANY::e_LINT, CIEC_ANY::e_STRING, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(! bDown);
  BOOST_CHECK(!CIEC_ANY::isCastable(CIEC_ANY::e_LINT, CIEC_ANY::e_WSTRING, bUp, bDown)); BOOST_CHECK(! bUp); BOOST_CHECK(! bDown);
}

BOOST_AUTO_TEST_SUITE_END()
#endif //FORTE_USE_64BIT_DATATYPES
