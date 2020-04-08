/*******************************************************************************
 * Copyright (c) 2011 - 2012 ACIN, nxtControl, 2018 TU Vienna/ACIN
 *               2020 Johannes Kepler University Linz
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Martin Melik Merkumians, Ingo Hegny, Alois Zoitl, Stanislav Meduna - initial API and implementation and/or initial documentation
 *   Ernst Blecha - Adds partial access tests
 *******************************************************************************/

#include <boost/test/unit_test.hpp>

/*#ifdef FORTE_USE_REAL_DATATYPE
  #include <boost/test/floating_point_comparison.hpp>

  #include "../../../src/core/datatypes/forte_real.h"
#endif //FORTE_USE_REAL_DATATYPE*/

//BOOLEAN
#include "../../../src/core/datatypes/forte_bool.h"
//BIT-Datatypes
#include "../../../src/core/datatypes/forte_byte.h"
#include "../../../src/core/datatypes/forte_word.h"

#include "CIEC_PARTIAL_test.h"

using namespace boost::unit_test;

BOOST_AUTO_TEST_SUITE(CIEC_ANY_BIT_PARTIAL)
BOOST_AUTO_TEST_SUITE(CIEC_ANY_BIT_PARTIAL_WORD)

BOOST_AUTO_TEST_CASE(PARTIAL_ACCESS_WORD_INITVALUES_CHECK_BIT)
{
  CIEC_WORD nTestWord;

  test4X_0(nTestWord, 12)
  test4X_0(nTestWord, 8)
  test4X_0(nTestWord, 4)
  test4X_0(nTestWord, 0)

}

BOOST_AUTO_TEST_CASE(PARTIAL_ACCESS_WORD_INITVALUES_CHECK_BYTE)
{
  CIEC_WORD nTestWord;

  BOOST_CHECK_EQUAL((nTestWord.partial<CIEC_BYTE,0>()),0);
  BOOST_CHECK_EQUAL((nTestWord.partial<CIEC_BYTE,1>()),0);

}

BOOST_AUTO_TEST_CASE(PARTIAL_ACCESS_WORD_CONST_INIT)
{
  const CIEC_BYTE nTestByte(0x42);
  CIEC_WORD mTestWord;

  mTestWord.partial<CIEC_BYTE,0>() = nTestByte;

  BOOST_CHECK_EQUAL(mTestWord, 0x42);
}

BOOST_AUTO_TEST_CASE(PARTIAL_ACCESS_WORD_VALUE_CHECK_WORD)
{
  CIEC_WORD nTestWord;

  nTestWord=0xBABE;

  BOOST_CHECK_EQUAL(nTestWord, 0xBABE);

}

BOOST_AUTO_TEST_CASE(PARTIAL_ACCESS_WORD_VALUE_CHECK_BYTE)
{
  CIEC_WORD nTestWord;

  nTestWord=0xBABE;

  BOOST_CHECK_EQUAL((nTestWord.partial<CIEC_BYTE,0>()), 0xBE);
  BOOST_CHECK_EQUAL((nTestWord.partial<CIEC_BYTE,1>()), 0xBA);

}

BOOST_AUTO_TEST_CASE(PARTIAL_ACCESS_WORD_VALUE_CHECK_BIT)
{
  CIEC_WORD nTestWord;

  nTestWord=0xBABE;

  test4X_B(nTestWord, 12)
  test4X_A(nTestWord, 8)
  test4X_B(nTestWord, 4)
  test4X_E(nTestWord, 0)

}

BOOST_AUTO_TEST_CASE(PARTIAL_ACCESS_WORD_ASSIGN_BIT_CHECK_BIT)
{
  CIEC_WORD nTestWord;

  set4X_F(nTestWord,12);
  set4X_0(nTestWord,8);
  set4X_0(nTestWord,4);
  set4X_D(nTestWord,0);

  test4X_F(nTestWord,12);
  test4X_0(nTestWord,8);
  test4X_0(nTestWord,4);
  test4X_D(nTestWord,0);

}

BOOST_AUTO_TEST_CASE(PARTIAL_ACCESS_WORD_ASSIGN_BIT_CHECK_BYTE)
{
  CIEC_WORD nTestWord;

  set4X_F(nTestWord,12);
  set4X_0(nTestWord,8);
  set4X_0(nTestWord,4);
  set4X_D(nTestWord,0);

  BOOST_CHECK_EQUAL((nTestWord.partial<CIEC_BYTE,1>()), 0xF0U);
  BOOST_CHECK_EQUAL((nTestWord.partial<CIEC_BYTE,0>()), 0x0DU);

}

BOOST_AUTO_TEST_CASE(PARTIAL_ACCESS_WORD_ASSIGN_BIT_CHECK_WORD)
{
  CIEC_WORD nTestWord;

  set4X_F(nTestWord,12);
  set4X_0(nTestWord,8);
  set4X_0(nTestWord,4);
  set4X_D(nTestWord,0);

  BOOST_CHECK_EQUAL(nTestWord, 0xF00DU);

}

BOOST_AUTO_TEST_CASE(PARTIAL_ACCESS_WORD_ASSIGN_BYTE_CHECK_BIT)
{
  CIEC_WORD nTestWord;

  nTestWord.partial<CIEC_BYTE,1>() = 0xCA;
  nTestWord.partial<CIEC_BYTE,0>() = 0xFE;

  test4X_C(nTestWord,12);
  test4X_A(nTestWord,8);
  test4X_F(nTestWord,4);
  test4X_E(nTestWord,0);

}

BOOST_AUTO_TEST_CASE(PARTIAL_ACCESS_WORD_ASSIGN_BYTE_CHECK_BYTE)
{
  CIEC_WORD nTestWord;

  nTestWord.partial<CIEC_BYTE,1>() = 0xCA;
  nTestWord.partial<CIEC_BYTE,0>() = 0xFE;

  BOOST_CHECK_EQUAL((nTestWord.partial<CIEC_BYTE,1>()), 0xCAU);
  BOOST_CHECK_EQUAL((nTestWord.partial<CIEC_BYTE,0>()), 0xFEU);

}

BOOST_AUTO_TEST_CASE(PARTIAL_ACCESS_WORD_ASSIGN_BYTE_CHECK_WORD)
{
  CIEC_WORD nTestWord;

  nTestWord.partial<CIEC_BYTE,1>() = 0xCA;
  nTestWord.partial<CIEC_BYTE,0>() = 0xFE;

  BOOST_CHECK_EQUAL(nTestWord, 0xCAFEU);

}

BOOST_AUTO_TEST_CASE(PARTIAL_ACCESS_WORD_ASSIGN_WORD_CHECK_BIT)
{
  CIEC_WORD nTestWord;

  nTestWord = 0xFFEE;

  test4X_F(nTestWord,12);
  test4X_F(nTestWord,8);
  test4X_E(nTestWord,4);
  test4X_E(nTestWord,0);

}

BOOST_AUTO_TEST_CASE(PARTIAL_ACCESS_WORD_ASSIGN_WORD_CHECK_BYTE)
{
  CIEC_WORD nTestWord;

  nTestWord = 0xFFEE;

  BOOST_CHECK_EQUAL((nTestWord.partial<CIEC_BYTE,1>()), 0xFFU);
  BOOST_CHECK_EQUAL((nTestWord.partial<CIEC_BYTE,0>()), 0xEEU);

}

BOOST_AUTO_TEST_CASE(PARTIAL_ACCESS_WORD_ASSIGN_WORD_CHECK_WORD)
{
  CIEC_WORD nTestWord;

  nTestWord = 0xFFEE;

  BOOST_CHECK_EQUAL(nTestWord, 0xFFEEU);

}

BOOST_AUTO_TEST_CASE(PARTIAL_ACCESS_WORD_ASSIGN_CHAIN_BYTE_BIT_CHECK_BIT)
{
  CIEC_WORD nTestWord;

  set4X_1(nTestWord,12);
  set4X_1(nTestWord,8);
  set4X_F(nTestWord,4);
  set4X_E(nTestWord,0);

  test4X_1(nTestWord,12);
  test4X_1(nTestWord,8);
  test4X_F(nTestWord,4);
  test4X_E(nTestWord,0);

}

BOOST_AUTO_TEST_CASE(PARTIAL_ACCESS_WORD_ASSIGN_CHAIN_BYTE_BIT_CHECK_BYTE)
{
  CIEC_WORD nTestWord;

  set4X_1(nTestWord,12);
  set4X_1(nTestWord,8);
  set4X_F(nTestWord,4);
  set4X_E(nTestWord,0);

  BOOST_CHECK_EQUAL((nTestWord.partial<CIEC_BYTE,1>()), 0x11U);
  BOOST_CHECK_EQUAL((nTestWord.partial<CIEC_BYTE,0>()), 0xFEU);

}

BOOST_AUTO_TEST_CASE(PARTIAL_ACCESS_WORD_ASSIGN_CHAIN_BYTE_BIT_CHECK_WORD)
{
  CIEC_WORD nTestWord;

  set4X_1(nTestWord,12);
  set4X_1(nTestWord,8);
  set4X_F(nTestWord,4);
  set4X_E(nTestWord,0);

  BOOST_CHECK_EQUAL(nTestWord, 0x11FEU);

}

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
