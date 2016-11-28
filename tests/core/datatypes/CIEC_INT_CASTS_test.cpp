/*******************************************************************************
 * Copyright (c) 2011 - 2012 ACIN, nxtControl
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Martin Melik Merkumians, Ingo Hegny, Alois Zoitl, Stanislav Meduna - initial API and implementation and/or initial documentation
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

BOOST_AUTO_TEST_SUITE(CIEC_INT_casts_function_test)


BOOST_AUTO_TEST_CASE(CASTS_INT_to_BOOL)
{
  CIEC_INT nTestInt0;
  CIEC_INT nTestInt1;
  CIEC_INT nTestInt256;
  CIEC_INT nTestInt32767;

  CIEC_BOOL bTestBool;

//check initial values
  BOOST_CHECK_EQUAL(nTestInt0, 0);
  BOOST_CHECK_EQUAL(nTestInt1, 0);
  BOOST_CHECK_EQUAL(nTestInt256, 0);
  BOOST_CHECK_EQUAL(nTestInt32767, 0);

  nTestInt0=0;
  nTestInt1=1;
  nTestInt256=256;
  nTestInt32767=32767;

  BOOST_CHECK_EQUAL(nTestInt0, 0);
  BOOST_CHECK_EQUAL(nTestInt1, 1);
  BOOST_CHECK_EQUAL(nTestInt256, 256);
  BOOST_CHECK_EQUAL(nTestInt32767, 32767);

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

BOOST_AUTO_TEST_CASE(CASTS_INT_to_BITDTs)
{
  CIEC_INT nTestInt0;
  CIEC_INT nTestInt1;
  CIEC_INT nTestInt256;
  CIEC_INT nTestInt32767;

  CIEC_BYTE nTestByte;
  CIEC_WORD nTestWord;
  CIEC_DWORD nTestDWord;
#ifdef FORTE_USE_64BIT_DATATYPES
  CIEC_DWORD nTestLWord;
#endif //FORTE_USE_64BIT_DATATYPES

//check initial values
  BOOST_CHECK_EQUAL(nTestInt0, 0);
  BOOST_CHECK_EQUAL(nTestInt1, 0);
  BOOST_CHECK_EQUAL(nTestInt256, 0);
  BOOST_CHECK_EQUAL(nTestInt32767, 0);

  nTestInt0=0;
  nTestInt1=1;
  nTestInt256=256;
  nTestInt32767=32767;

  BOOST_CHECK_EQUAL(nTestInt0, 0);
  BOOST_CHECK_EQUAL(nTestInt1, 1);
  BOOST_CHECK_EQUAL(nTestInt256, 256);
  BOOST_CHECK_EQUAL(nTestInt32767, 32767);

//toByte
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


#ifdef FORTE_USE_64BIT_DATATYPES
//toLWord
  nTestLWord.setValue(nTestInt0);
  BOOST_CHECK_EQUAL(nTestLWord, 0ULL);
  nTestLWord.setValue(nTestInt1);
  BOOST_CHECK_EQUAL(nTestLWord, 1ULL);
  nTestLWord.setValue(nTestInt256);
  BOOST_CHECK_EQUAL(nTestLWord, 256ULL);
  nTestLWord.setValue(nTestInt32767);
  BOOST_CHECK_EQUAL(nTestLWord, 32767ULL);

#endif
}

BOOST_AUTO_TEST_CASE(CASTS_INT_to_INTS)
{
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
#ifdef FORTE_USE_64BIT_DATATYPES
  CIEC_LINT nTestLInt;
  CIEC_ULINT nTestULInt;
#endif //FORTE_USE_64BIT_DATATYPES

  //check initial values
    BOOST_CHECK_EQUAL(nTestInt0, 0);
    BOOST_CHECK_EQUAL(nTestInt1, 0);
    BOOST_CHECK_EQUAL(nTestInt256, 0);
    BOOST_CHECK_EQUAL(nTestInt32767, 0);
    BOOST_CHECK_EQUAL(nTestInt_1, 0);

    nTestInt0=0;
    nTestInt1=1;
    nTestInt256=256;
    nTestInt32767=32767;
    nTestInt_1=-1;

    BOOST_CHECK_EQUAL(nTestInt0, 0);
    BOOST_CHECK_EQUAL(nTestInt1, 1);
    BOOST_CHECK_EQUAL(nTestInt256, 256);
    BOOST_CHECK_EQUAL(nTestInt32767, 32767);
    BOOST_CHECK_EQUAL(nTestInt_1, -1);

//toUSINT
  nTestUSInt.setValue(nTestInt0);
  BOOST_CHECK_EQUAL(nTestUSInt, 0U);
  nTestUSInt.setValue(nTestInt1);
  BOOST_CHECK_EQUAL(nTestUSInt, 1U);
  nTestUSInt.setValue(nTestInt256);
  BOOST_CHECK_EQUAL(nTestUSInt, 0U);
  nTestUSInt.setValue(nTestInt32767);
  BOOST_CHECK_EQUAL(nTestUSInt, 255U);
  nTestUSInt.setValue(nTestInt_1);
  BOOST_CHECK_EQUAL(nTestUSInt, 255U);


//toSINT
  nTestSInt.setValue(nTestInt0);
  BOOST_CHECK_EQUAL(nTestSInt, 0);
  nTestSInt.setValue(nTestInt1);
  BOOST_CHECK_EQUAL(nTestSInt, 1);
  nTestSInt.setValue(nTestInt256);
  BOOST_CHECK_EQUAL(nTestSInt, 0);
  nTestSInt.setValue(nTestInt32767);
  BOOST_CHECK_EQUAL(nTestSInt, -1);

//toUINT
  nTestUInt.setValue(nTestInt0);
  BOOST_CHECK_EQUAL(nTestUInt, 0U);
  nTestUInt.setValue(nTestInt1);
  BOOST_CHECK_EQUAL(nTestUInt, 1U);
  nTestUInt.setValue(nTestInt256);
  BOOST_CHECK_EQUAL(nTestUInt, 256U);
  nTestUInt.setValue(nTestInt32767);
  BOOST_CHECK_EQUAL(nTestUInt, 32767U);
  nTestUInt.setValue(nTestInt_1);
  BOOST_CHECK_EQUAL(nTestUInt, 65535U);

//toDINT
  nTestDInt.setValue(nTestInt0);
  BOOST_CHECK_EQUAL(nTestDInt, 0);
  nTestDInt.setValue(nTestInt1);
  BOOST_CHECK_EQUAL(nTestDInt, 1);
  nTestDInt.setValue(nTestInt256);
  BOOST_CHECK_EQUAL(nTestDInt, 256);
  nTestDInt.setValue(nTestInt32767);
  BOOST_CHECK_EQUAL(nTestDInt, 32767);
  nTestDInt.setValue(nTestInt_1);
  BOOST_CHECK_EQUAL(nTestDInt, -1);

//toUDINT
  nTestUDInt.setValue(nTestInt0);
  BOOST_CHECK_EQUAL(nTestUDInt, 0UL);
  nTestUDInt.setValue(nTestInt1);
  BOOST_CHECK_EQUAL(nTestUDInt, 1UL);
  nTestUDInt.setValue(nTestInt256);
  BOOST_CHECK_EQUAL(nTestUDInt, 256UL);
  nTestUDInt.setValue(nTestInt32767);
  BOOST_CHECK_EQUAL(nTestUDInt, 32767UL);

#ifdef FORTE_USE_64BIT_DATATYPES
//toLINT
  nTestLInt.setValue(nTestInt0);
  BOOST_CHECK_EQUAL(nTestLInt, 0LL);
  nTestLInt.setValue(nTestInt1);
  BOOST_CHECK_EQUAL(nTestLInt, 1LL);
  nTestLInt.setValue(nTestInt256);
  BOOST_CHECK_EQUAL(nTestLInt, 256LL);
  nTestLInt.setValue(nTestInt32767);
  BOOST_CHECK_EQUAL(nTestLInt, 32767LL);
  nTestLInt.setValue(nTestInt_1);
  BOOST_CHECK_EQUAL(nTestLInt, -1LL);

//toULINT
  nTestULInt.setValue(nTestInt0);
  BOOST_CHECK_EQUAL(nTestULInt, 0ULL);
  nTestULInt.setValue(nTestInt1);
  BOOST_CHECK_EQUAL(nTestULInt, 1ULL);
  nTestULInt.setValue(nTestInt256);
  BOOST_CHECK_EQUAL(nTestULInt, 256ULL);
  nTestULInt.setValue(nTestInt32767);
  BOOST_CHECK_EQUAL(nTestULInt, 32767ULL);
#endif
}

#ifdef FORTE_USE_REAL_DATATYPE
BOOST_AUTO_TEST_CASE(CASTS_INT_to_REAL)
{
  CIEC_INT nTestInt0;
  CIEC_INT nTestInt1;
  CIEC_INT nTestInt256;
  CIEC_INT nTestInt32767;

  CIEC_REAL nTestReal;

//check initial values
  BOOST_CHECK_EQUAL(nTestInt0, 0);
  BOOST_CHECK_EQUAL(nTestInt1, 0);
  BOOST_CHECK_EQUAL(nTestInt256, 0);
  BOOST_CHECK_EQUAL(nTestInt32767, 0);

  nTestInt0=0;
  nTestInt1=1;
  nTestInt256=256;
  nTestInt32767=32767;

  BOOST_CHECK_EQUAL(nTestInt0, 0);
  BOOST_CHECK_EQUAL(nTestInt1, 1);
  BOOST_CHECK_EQUAL(nTestInt256, 256);
  BOOST_CHECK_EQUAL(nTestInt32767, 32767);

//toREAL
  nTestReal.setValue(nTestInt0);
  BOOST_CHECK_EQUAL(0.0f, nTestReal);
  nTestReal.setValue(nTestInt1);
  BOOST_CHECK_EQUAL(1.0f, nTestReal); 
  nTestReal.setValue(nTestInt256);
  BOOST_CHECK_EQUAL(256.0f, nTestReal); 
  nTestReal.setValue(nTestInt32767);
  BOOST_CHECK_EQUAL(32767.0f, nTestReal); 


#ifdef FORTE_USE_64BIT_DATATYPES
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
#endif //FORTE_USE_64BIT_DATATYPES

}
#endif //FORTE_USE_REAL_DATATYPE

BOOST_AUTO_TEST_CASE(CASTS_INT_to_TIME)
{
  CIEC_INT nTestInt0;
  CIEC_INT nTestInt1;
  CIEC_INT nTestInt256;
  CIEC_INT nTestInt32767;
  CIEC_INT nTestInt_1;

  CIEC_TIME nTestTime;

  //check initial values
    BOOST_CHECK_EQUAL(nTestInt0, 0);
    BOOST_CHECK_EQUAL(nTestInt1, 0);
    BOOST_CHECK_EQUAL(nTestInt256, 0);
    BOOST_CHECK_EQUAL(nTestInt32767, 0);
    BOOST_CHECK_EQUAL(nTestInt_1, 0);

    nTestInt0=0;
    nTestInt1=1;
    nTestInt256=256;
    nTestInt32767=32767;
    nTestInt_1=-1;

    BOOST_CHECK_EQUAL(nTestInt0, 0);
    BOOST_CHECK_EQUAL(nTestInt1, 1);
    BOOST_CHECK_EQUAL(nTestInt256, 256);
    BOOST_CHECK_EQUAL(nTestInt32767, 32767);
    BOOST_CHECK_EQUAL(nTestInt_1, -1);

//toTIME
  nTestTime.setValue(nTestInt0);
  BOOST_CHECK_EQUAL(nTestTime, 0);
  nTestTime.setValue(nTestInt1);
  BOOST_CHECK_EQUAL(nTestTime, 1);
  nTestTime.setValue(nTestInt256);
  BOOST_CHECK_EQUAL(nTestTime, 256);
  nTestTime.setValue(nTestInt32767);
  BOOST_CHECK_EQUAL(nTestTime, 32767);
  nTestTime.setValue(nTestInt_1);
  BOOST_CHECK_EQUAL(nTestTime, -1);
}

BOOST_AUTO_TEST_CASE(INT_Castable_test)
{
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

BOOST_AUTO_TEST_SUITE_END()
