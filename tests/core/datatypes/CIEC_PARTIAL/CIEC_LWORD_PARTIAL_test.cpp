/*******************************************************************************
 * Copyright (c) 2011 - 2012 ACIN, nxtControl, 2018 TU Vienna/ACIN
 *               2020 Johannes Kepler University Linz
 *               2022 Primetals Technologies Austria GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Martin Melik Merkumians, Ingo Hegny, Alois Zoitl, Stanislav Meduna - initial API and implementation and/or initial documentation
 *   Ernst Blecha - Adds partial access tests
 *   Martin Melik Merkumians - Updates test for changes in partial
 *******************************************************************************/

#include <boost/test/unit_test.hpp>

//BOOLEAN
#include "../../../src/core/datatypes/forte_bool.h"
//BIT-Datatypes
#include "../../../src/core/datatypes/forte_byte.h"
#include "../../../src/core/datatypes/forte_word.h"
#include "../../../src/core/datatypes/forte_dword.h"
#include "../../../src/core/datatypes/forte_lword.h"

#include "../../../src/core/datatypes/forte_sint.h"

#include "CIEC_PARTIAL_test.h"

using namespace boost::unit_test;

BOOST_AUTO_TEST_SUITE(ANY_BIT_PARTIAL)

BOOST_AUTO_TEST_SUITE(CIEC_ANY_BIT_PARTIAL_LWORD)

BOOST_AUTO_TEST_CASE(PARTIAL_ACCESS_LWORD_INITVALUES_CHECK_BIT)
{
  CIEC_LWORD nTestLWord;

  test4X_0(nTestLWord, 60);
  test4X_0(nTestLWord, 56);
  test4X_0(nTestLWord, 52);
  test4X_0(nTestLWord, 48);

  test4X_0(nTestLWord, 44);
  test4X_0(nTestLWord, 40);
  test4X_0(nTestLWord, 36);
  test4X_0(nTestLWord, 32);

  test4X_0(nTestLWord, 28);
  test4X_0(nTestLWord, 24);
  test4X_0(nTestLWord, 20);
  test4X_0(nTestLWord, 16);

  test4X_0(nTestLWord, 12);
  test4X_0(nTestLWord, 8);
  test4X_0(nTestLWord, 4);
  test4X_0(nTestLWord, 0);

}

BOOST_AUTO_TEST_CASE(PARTIAL_ACCESS_LWORD_INITVALUES_CHECK_BYTE)
{
  CIEC_LWORD nTestLWord;

  BOOST_CHECK_EQUAL((nTestLWord.partial<CIEC_BYTE>(0)),0);
  BOOST_CHECK_EQUAL((nTestLWord.partial<CIEC_BYTE>(1)),0);
  BOOST_CHECK_EQUAL((nTestLWord.partial<CIEC_BYTE>(2)),0);
  BOOST_CHECK_EQUAL((nTestLWord.partial<CIEC_BYTE>(3)),0);
  BOOST_CHECK_EQUAL((nTestLWord.partial<CIEC_BYTE>(4)),0);
  BOOST_CHECK_EQUAL((nTestLWord.partial<CIEC_BYTE>(5)),0);
  BOOST_CHECK_EQUAL((nTestLWord.partial<CIEC_BYTE>(6)),0);
  BOOST_CHECK_EQUAL((nTestLWord.partial<CIEC_BYTE>(7)),0);

}

BOOST_AUTO_TEST_CASE(PARTIAL_ACCESS_LWORD_INITVALUES_CHECK_WORD)
{
  CIEC_LWORD nTestLWord;

  BOOST_CHECK_EQUAL((nTestLWord.partial<CIEC_WORD>(0)),0);
  BOOST_CHECK_EQUAL((nTestLWord.partial<CIEC_WORD>(1)),0);
  BOOST_CHECK_EQUAL((nTestLWord.partial<CIEC_WORD>(2)),0);
  BOOST_CHECK_EQUAL((nTestLWord.partial<CIEC_WORD>(3)),0);

}

BOOST_AUTO_TEST_CASE(PARTIAL_ACCESS_LWORD_INITVALUES_CHECK_DWORD)
{
  CIEC_LWORD nTestLWord;

  BOOST_CHECK_EQUAL((nTestLWord.partial<CIEC_DWORD>(0)),0);
  BOOST_CHECK_EQUAL((nTestLWord.partial<CIEC_DWORD>(1)),0);

}

BOOST_AUTO_TEST_CASE(PARTIAL_ACCESS_LWORD_CONST_INIT)
{
  const CIEC_DWORD nTestDword(0x42);
  CIEC_LWORD mTestLword;

  mTestLword.partial<CIEC_DWORD>(0) = nTestDword;

  BOOST_CHECK_EQUAL(mTestLword, 0x42);
}

BOOST_AUTO_TEST_CASE(PARTIAL_ACCESS_LWORD_VALUE_CHECK_BIT)
{
  CIEC_LWORD nTestLWord;

  nTestLWord = CIEC_LWORD(0xDEADBEEFCAFEBABEU);

  test4X_D(nTestLWord, 60)
  test4X_E(nTestLWord, 56)
  test4X_A(nTestLWord, 52)
  test4X_D(nTestLWord, 48)

  test4X_B(nTestLWord, 44)
  test4X_E(nTestLWord, 40)
  test4X_E(nTestLWord, 36)
  test4X_F(nTestLWord, 32)

  test4X_C(nTestLWord, 28)
  test4X_A(nTestLWord, 24)
  test4X_F(nTestLWord, 20)
  test4X_E(nTestLWord, 16)

  test4X_B(nTestLWord, 12)
  test4X_A(nTestLWord, 8)
  test4X_B(nTestLWord, 4)
  test4X_E(nTestLWord, 0)

}

BOOST_AUTO_TEST_CASE(PARTIAL_ACCESS_LWORD_VALUE_CHECK_BYTE)
{
  CIEC_LWORD nTestLWord;

  nTestLWord = CIEC_LWORD(0xDEADBEEFCAFEBABEU);

  BOOST_CHECK_EQUAL((nTestLWord.partial<CIEC_BYTE>(0)), 0xBE);
  BOOST_CHECK_EQUAL((nTestLWord.partial<CIEC_BYTE>(1)), 0xBA);
  BOOST_CHECK_EQUAL((nTestLWord.partial<CIEC_BYTE>(2)), 0xFE);
  BOOST_CHECK_EQUAL((nTestLWord.partial<CIEC_BYTE>(3)), 0xCA);
  BOOST_CHECK_EQUAL((nTestLWord.partial<CIEC_BYTE>(4)), 0xEF);
  BOOST_CHECK_EQUAL((nTestLWord.partial<CIEC_BYTE>(5)), 0xBE);
  BOOST_CHECK_EQUAL((nTestLWord.partial<CIEC_BYTE>(6)), 0xAD);
  BOOST_CHECK_EQUAL((nTestLWord.partial<CIEC_BYTE>(7)), 0xDE);

}

BOOST_AUTO_TEST_CASE(PARTIAL_ACCESS_LWORD_VALUE_CHECK_WORD)
{
  CIEC_LWORD nTestLWord;

  nTestLWord = CIEC_LWORD(0xDEADBEEFCAFEBABEU);

  BOOST_CHECK_EQUAL((nTestLWord.partial<CIEC_WORD>(0)), 0xBABE);
  BOOST_CHECK_EQUAL((nTestLWord.partial<CIEC_WORD>(1)), 0xCAFE);
  BOOST_CHECK_EQUAL((nTestLWord.partial<CIEC_WORD>(2)), 0xBEEF);
  BOOST_CHECK_EQUAL((nTestLWord.partial<CIEC_WORD>(3)), 0xDEAD);

}

BOOST_AUTO_TEST_CASE(PARTIAL_ACCESS_LWORD_VALUE_CHECK_DWORD)
{
  CIEC_LWORD nTestLWord;

  nTestLWord = CIEC_LWORD(0xDEADBEEFCAFEBABEU);

  BOOST_CHECK_EQUAL((nTestLWord.partial<CIEC_DWORD>(0)), 0xCAFEBABEU);
  BOOST_CHECK_EQUAL((nTestLWord.partial<CIEC_DWORD>(1)), 0xDEADBEEFU);

}

BOOST_AUTO_TEST_CASE(PARTIAL_ACCESS_LWORD_ASSIGN_BIT_CHECK_BIT)
{
  CIEC_LWORD nTestLWord;

  set4X_B(nTestLWord,60);
  set4X_A(nTestLWord,56);
  set4X_D(nTestLWord,52);
  set4X_C(nTestLWord,48);
  set4X_A(nTestLWord,44);
  set4X_B(nTestLWord,40);
  set4X_1(nTestLWord,36);
  set4X_E(nTestLWord,32);
  set4X_B(nTestLWord,28);
  set4X_A(nTestLWord,24);
  set4X_A(nTestLWord,20);
  set4X_D(nTestLWord,16);
  set4X_F(nTestLWord,12);
  set4X_0(nTestLWord,8);
  set4X_0(nTestLWord,4);
  set4X_D(nTestLWord,0);

  test4X_B(nTestLWord,60);
  test4X_A(nTestLWord,56);
  test4X_D(nTestLWord,52);
  test4X_C(nTestLWord,48);
  test4X_A(nTestLWord,44);
  test4X_B(nTestLWord,40);
  test4X_1(nTestLWord,36);
  test4X_E(nTestLWord,32);
  test4X_B(nTestLWord,28);
  test4X_A(nTestLWord,24);
  test4X_A(nTestLWord,20);
  test4X_D(nTestLWord,16);
  test4X_F(nTestLWord,12);
  test4X_0(nTestLWord,8);
  test4X_0(nTestLWord,4);
  test4X_D(nTestLWord,0);

}

BOOST_AUTO_TEST_CASE(PARTIAL_ACCESS_LWORD_ASSIGN_BIT_CHECK_BYTE)
{
  CIEC_LWORD nTestLWord;

  set4X_B(nTestLWord,60);
  set4X_A(nTestLWord,56);
  set4X_D(nTestLWord,52);
  set4X_C(nTestLWord,48);
  set4X_A(nTestLWord,44);
  set4X_B(nTestLWord,40);
  set4X_1(nTestLWord,36);
  set4X_E(nTestLWord,32);
  set4X_B(nTestLWord,28);
  set4X_A(nTestLWord,24);
  set4X_A(nTestLWord,20);
  set4X_D(nTestLWord,16);
  set4X_F(nTestLWord,12);
  set4X_0(nTestLWord,8);
  set4X_0(nTestLWord,4);
  set4X_D(nTestLWord,0);

  BOOST_CHECK_EQUAL((nTestLWord.partial<CIEC_BYTE>(7)), 0xBAU);
  BOOST_CHECK_EQUAL((nTestLWord.partial<CIEC_BYTE>(6)), 0xDCU);
  BOOST_CHECK_EQUAL((nTestLWord.partial<CIEC_BYTE>(5)), 0xABU);
  BOOST_CHECK_EQUAL((nTestLWord.partial<CIEC_BYTE>(4)), 0x1EU);
  BOOST_CHECK_EQUAL((nTestLWord.partial<CIEC_BYTE>(3)), 0xBAU);
  BOOST_CHECK_EQUAL((nTestLWord.partial<CIEC_BYTE>(2)), 0xADU);
  BOOST_CHECK_EQUAL((nTestLWord.partial<CIEC_BYTE>(1)), 0xF0U);
  BOOST_CHECK_EQUAL((nTestLWord.partial<CIEC_BYTE>(0)), 0x0DU);

}

BOOST_AUTO_TEST_CASE(PARTIAL_ACCESS_LWORD_ASSIGN_BIT_CHECK_WORD)
{
  CIEC_LWORD nTestLWord;

  set4X_B(nTestLWord,60);
  set4X_A(nTestLWord,56);
  set4X_D(nTestLWord,52);
  set4X_C(nTestLWord,48);
  set4X_A(nTestLWord,44);
  set4X_B(nTestLWord,40);
  set4X_1(nTestLWord,36);
  set4X_E(nTestLWord,32);
  set4X_B(nTestLWord,28);
  set4X_A(nTestLWord,24);
  set4X_A(nTestLWord,20);
  set4X_D(nTestLWord,16);
  set4X_F(nTestLWord,12);
  set4X_0(nTestLWord,8);
  set4X_0(nTestLWord,4);
  set4X_D(nTestLWord,0);

  BOOST_CHECK_EQUAL((nTestLWord.partial<CIEC_WORD>(3)), 0xBADCU);
  BOOST_CHECK_EQUAL((nTestLWord.partial<CIEC_WORD>(2)), 0xAB1EU);
  BOOST_CHECK_EQUAL((nTestLWord.partial<CIEC_WORD>(1)), 0xBAADU);
  BOOST_CHECK_EQUAL((nTestLWord.partial<CIEC_WORD>(0)), 0xF00DU);

}

BOOST_AUTO_TEST_CASE(PARTIAL_ACCESS_LWORD_ASSIGN_BIT_CHECK_DWORD)
{
  CIEC_LWORD nTestLWord;

  set4X_B(nTestLWord,60);
  set4X_A(nTestLWord,56);
  set4X_D(nTestLWord,52);
  set4X_C(nTestLWord,48);
  set4X_A(nTestLWord,44);
  set4X_B(nTestLWord,40);
  set4X_1(nTestLWord,36);
  set4X_E(nTestLWord,32);
  set4X_B(nTestLWord,28);
  set4X_A(nTestLWord,24);
  set4X_A(nTestLWord,20);
  set4X_D(nTestLWord,16);
  set4X_F(nTestLWord,12);
  set4X_0(nTestLWord,8);
  set4X_0(nTestLWord,4);
  set4X_D(nTestLWord,0);

  BOOST_CHECK_EQUAL((nTestLWord.partial<CIEC_DWORD>(1)), 0xBADCAB1EU);
  BOOST_CHECK_EQUAL((nTestLWord.partial<CIEC_DWORD>(0)), 0xBAADF00DU);

}

BOOST_AUTO_TEST_CASE(PARTIAL_ACCESS_LWORD_ASSIGN_BIT_CHECK_LWORD)
{
  CIEC_LWORD nTestLWord;

  set4X_B(nTestLWord,60);
  set4X_A(nTestLWord,56);
  set4X_D(nTestLWord,52);
  set4X_C(nTestLWord,48);
  set4X_A(nTestLWord,44);
  set4X_B(nTestLWord,40);
  set4X_1(nTestLWord,36);
  set4X_E(nTestLWord,32);
  set4X_B(nTestLWord,28);
  set4X_A(nTestLWord,24);
  set4X_A(nTestLWord,20);
  set4X_D(nTestLWord,16);
  set4X_F(nTestLWord,12);
  set4X_0(nTestLWord,8);
  set4X_0(nTestLWord,4);
  set4X_D(nTestLWord,0);

  BOOST_CHECK_EQUAL(nTestLWord, 0xBADCAB1EBAADF00DU);

}

BOOST_AUTO_TEST_CASE(PARTIAL_ACCESS_LWORD_ASSIGN_BYTE_CHECK_BIT)
{
  CIEC_LWORD nTestLWord;

  nTestLWord.partial<CIEC_BYTE>(7) = CIEC_BYTE(0xDE);
  nTestLWord.partial<CIEC_BYTE>(6) = CIEC_BYTE(0xCA);
  nTestLWord.partial<CIEC_BYTE>(5) = CIEC_BYTE(0xFB);
  nTestLWord.partial<CIEC_BYTE>(4) = CIEC_BYTE(0xAD);
  nTestLWord.partial<CIEC_BYTE>(3) = CIEC_BYTE(0xBA);
  nTestLWord.partial<CIEC_BYTE>(2) = CIEC_BYTE(0xD0);
  nTestLWord.partial<CIEC_BYTE>(1) = CIEC_BYTE(0xCA);
  nTestLWord.partial<CIEC_BYTE>(0) = CIEC_BYTE(0xFE);

  test4X_D(nTestLWord,60);
  test4X_E(nTestLWord,56);
  test4X_C(nTestLWord,52);
  test4X_A(nTestLWord,48);
  test4X_F(nTestLWord,44);
  test4X_B(nTestLWord,40);
  test4X_A(nTestLWord,36);
  test4X_D(nTestLWord,32);
  test4X_B(nTestLWord,28);
  test4X_A(nTestLWord,24);
  test4X_D(nTestLWord,20);
  test4X_0(nTestLWord,16);
  test4X_C(nTestLWord,12);
  test4X_A(nTestLWord,8);
  test4X_F(nTestLWord,4);
  test4X_E(nTestLWord,0);

}

BOOST_AUTO_TEST_CASE(PARTIAL_ACCESS_LWORD_ASSIGN_BYTE_CHECK_BYTE)
{
  CIEC_LWORD nTestLWord;

  nTestLWord.partial<CIEC_BYTE>(7) = CIEC_BYTE(0xDE);
  nTestLWord.partial<CIEC_BYTE>(6) = CIEC_BYTE(0xCA);
  nTestLWord.partial<CIEC_BYTE>(5) = CIEC_BYTE(0xFB);
  nTestLWord.partial<CIEC_BYTE>(4) = CIEC_BYTE(0xAD);
  nTestLWord.partial<CIEC_BYTE>(3) = CIEC_BYTE(0xBA);
  nTestLWord.partial<CIEC_BYTE>(2) = CIEC_BYTE(0xD0);
  nTestLWord.partial<CIEC_BYTE>(1) = CIEC_BYTE(0xCA);
  nTestLWord.partial<CIEC_BYTE>(0) = CIEC_BYTE(0xFE);

  BOOST_CHECK_EQUAL((nTestLWord.partial<CIEC_BYTE>(7)), 0xDEU);
  BOOST_CHECK_EQUAL((nTestLWord.partial<CIEC_BYTE>(6)), 0xCAU);
  BOOST_CHECK_EQUAL((nTestLWord.partial<CIEC_BYTE>(5)), 0xFBU);
  BOOST_CHECK_EQUAL((nTestLWord.partial<CIEC_BYTE>(4)), 0xADU);
  BOOST_CHECK_EQUAL((nTestLWord.partial<CIEC_BYTE>(3)), 0xBAU);
  BOOST_CHECK_EQUAL((nTestLWord.partial<CIEC_BYTE>(2)), 0xD0U);
  BOOST_CHECK_EQUAL((nTestLWord.partial<CIEC_BYTE>(1)), 0xCAU);
  BOOST_CHECK_EQUAL((nTestLWord.partial<CIEC_BYTE>(0)), 0xFEU);

}

BOOST_AUTO_TEST_CASE(PARTIAL_ACCESS_LWORD_ASSIGN_BYTE_CHECK_WORD)
{
  CIEC_LWORD nTestLWord;

  nTestLWord.partial<CIEC_BYTE>(7) = CIEC_BYTE(0xDE);
  nTestLWord.partial<CIEC_BYTE>(6) = CIEC_BYTE(0xCA);
  nTestLWord.partial<CIEC_BYTE>(5) = CIEC_BYTE(0xFB);
  nTestLWord.partial<CIEC_BYTE>(4) = CIEC_BYTE(0xAD);
  nTestLWord.partial<CIEC_BYTE>(3) = CIEC_BYTE(0xBA);
  nTestLWord.partial<CIEC_BYTE>(2) = CIEC_BYTE(0xD0);
  nTestLWord.partial<CIEC_BYTE>(1) = CIEC_BYTE(0xCA);
  nTestLWord.partial<CIEC_BYTE>(0) = CIEC_BYTE(0xFE);

  BOOST_CHECK_EQUAL((nTestLWord.partial<CIEC_WORD>(3)), 0xDECAU);
  BOOST_CHECK_EQUAL((nTestLWord.partial<CIEC_WORD>(2)), 0xFBADU);
  BOOST_CHECK_EQUAL((nTestLWord.partial<CIEC_WORD>(1)), 0xBAD0U);
  BOOST_CHECK_EQUAL((nTestLWord.partial<CIEC_WORD>(0)), 0xCAFEU);
}

BOOST_AUTO_TEST_CASE(PARTIAL_ACCESS_LWORD_ASSIGN_BYTE_CHECK_DWORD)
{
  CIEC_LWORD nTestLWord;

  nTestLWord.partial<CIEC_BYTE>(7) = CIEC_BYTE(0xDE);
  nTestLWord.partial<CIEC_BYTE>(6) = CIEC_BYTE(0xCA);
  nTestLWord.partial<CIEC_BYTE>(5) = CIEC_BYTE(0xFB);
  nTestLWord.partial<CIEC_BYTE>(4) = CIEC_BYTE(0xAD);
  nTestLWord.partial<CIEC_BYTE>(3) = CIEC_BYTE(0xBA);
  nTestLWord.partial<CIEC_BYTE>(2) = CIEC_BYTE(0xD0);
  nTestLWord.partial<CIEC_BYTE>(1) = CIEC_BYTE(0xCA);
  nTestLWord.partial<CIEC_BYTE>(0) = CIEC_BYTE(0xFE);

  BOOST_CHECK_EQUAL((nTestLWord.partial<CIEC_DWORD>(1)), 0xDECAFBADU);
  BOOST_CHECK_EQUAL((nTestLWord.partial<CIEC_DWORD>(0)), 0xBAD0CAFEU);
}

BOOST_AUTO_TEST_CASE(PARTIAL_ACCESS_LWORD_ASSIGN_BYTE_CHECK_LWORD)
{
  CIEC_LWORD nTestLWord;

  nTestLWord.partial<CIEC_BYTE>(7) = CIEC_BYTE(0xDE);
  nTestLWord.partial<CIEC_BYTE>(6) = CIEC_BYTE(0xCA);
  nTestLWord.partial<CIEC_BYTE>(5) = CIEC_BYTE(0xFB);
  nTestLWord.partial<CIEC_BYTE>(4) = CIEC_BYTE(0xAD);
  nTestLWord.partial<CIEC_BYTE>(3) = CIEC_BYTE(0xBA);
  nTestLWord.partial<CIEC_BYTE>(2) = CIEC_BYTE(0xD0);
  nTestLWord.partial<CIEC_BYTE>(1) = CIEC_BYTE(0xCA);
  nTestLWord.partial<CIEC_BYTE>(0) = CIEC_BYTE(0xFE);

  BOOST_CHECK_EQUAL(nTestLWord, 0xDECAFBADBAD0CAFEU);
}

BOOST_AUTO_TEST_CASE(PARTIAL_ACCESS_LWORD_ASSIGN_WORD_CHECK_BIT)
{
  CIEC_LWORD nTestLWord;

  nTestLWord.partial<CIEC_WORD>(3) = CIEC_WORD(0x00FE);
  nTestLWord.partial<CIEC_WORD>(2) = CIEC_WORD(0xEDFA);
  nTestLWord.partial<CIEC_WORD>(1) = CIEC_WORD(0xCEC0);
  nTestLWord.partial<CIEC_WORD>(0) = CIEC_WORD(0xFFEE);

  test4X_0(nTestLWord,60);
  test4X_0(nTestLWord,56);
  test4X_F(nTestLWord,52);
  test4X_E(nTestLWord,48);
  test4X_E(nTestLWord,44);
  test4X_D(nTestLWord,40);
  test4X_F(nTestLWord,36);
  test4X_A(nTestLWord,32);
  test4X_C(nTestLWord,28);
  test4X_E(nTestLWord,24);
  test4X_C(nTestLWord,20);
  test4X_0(nTestLWord,16);
  test4X_F(nTestLWord,12);
  test4X_F(nTestLWord,8);
  test4X_E(nTestLWord,4);
  test4X_E(nTestLWord,0);
}

BOOST_AUTO_TEST_CASE(PARTIAL_ACCESS_LWORD_ASSIGN_WORD_CHECK_BYTE)
{
  CIEC_LWORD nTestLWord;

  nTestLWord.partial<CIEC_WORD>(3) = CIEC_WORD(0x00FE);
  nTestLWord.partial<CIEC_WORD>(2) = CIEC_WORD(0xEDFA);
  nTestLWord.partial<CIEC_WORD>(1) = CIEC_WORD(0xCEC0);
  nTestLWord.partial<CIEC_WORD>(0) = CIEC_WORD(0xFFEE);

  BOOST_CHECK_EQUAL((nTestLWord.partial<CIEC_BYTE>(7)), 0x00U);
  BOOST_CHECK_EQUAL((nTestLWord.partial<CIEC_BYTE>(6)), 0xFEU);
  BOOST_CHECK_EQUAL((nTestLWord.partial<CIEC_BYTE>(5)), 0xEDU);
  BOOST_CHECK_EQUAL((nTestLWord.partial<CIEC_BYTE>(4)), 0xFAU);
  BOOST_CHECK_EQUAL((nTestLWord.partial<CIEC_BYTE>(3)), 0xCEU);
  BOOST_CHECK_EQUAL((nTestLWord.partial<CIEC_BYTE>(2)), 0xC0U);
  BOOST_CHECK_EQUAL((nTestLWord.partial<CIEC_BYTE>(1)), 0xFFU);
  BOOST_CHECK_EQUAL((nTestLWord.partial<CIEC_BYTE>(0)), 0xEEU);
}

BOOST_AUTO_TEST_CASE(PARTIAL_ACCESS_LWORD_ASSIGN_WORD_CHECK_WORD)
{
  CIEC_LWORD nTestLWord;

  nTestLWord.partial<CIEC_WORD>(3) = CIEC_WORD(0x00FE);
  nTestLWord.partial<CIEC_WORD>(2) = CIEC_WORD(0xEDFA);
  nTestLWord.partial<CIEC_WORD>(1) = CIEC_WORD(0xCEC0);
  nTestLWord.partial<CIEC_WORD>(0) = CIEC_WORD(0xFFEE);

  BOOST_CHECK_EQUAL((nTestLWord.partial<CIEC_WORD>(3)), 0x00FEU);
  BOOST_CHECK_EQUAL((nTestLWord.partial<CIEC_WORD>(2)), 0xEDFAU);
  BOOST_CHECK_EQUAL((nTestLWord.partial<CIEC_WORD>(1)), 0xCEC0U);
  BOOST_CHECK_EQUAL((nTestLWord.partial<CIEC_WORD>(0)), 0xFFEEU);
}

BOOST_AUTO_TEST_CASE(PARTIAL_ACCESS_LWORD_ASSIGN_WORD_CHECK_DWORD)
{
  CIEC_LWORD nTestLWord;

  nTestLWord.partial<CIEC_WORD>(3) = CIEC_WORD(0x00FE);
  nTestLWord.partial<CIEC_WORD>(2) = CIEC_WORD(0xEDFA);
  nTestLWord.partial<CIEC_WORD>(1) = CIEC_WORD(0xCEC0);
  nTestLWord.partial<CIEC_WORD>(0) = CIEC_WORD(0xFFEE);

  BOOST_CHECK_EQUAL((nTestLWord.partial<CIEC_DWORD>(1)), 0x00FEEDFAU);
  BOOST_CHECK_EQUAL((nTestLWord.partial<CIEC_DWORD>(0)), 0xCEC0FFEEU);
}

BOOST_AUTO_TEST_CASE(PARTIAL_ACCESS_LWORD_ASSIGN_WORD_CHECK_LWORD)
{
  CIEC_LWORD nTestLWord;

  nTestLWord.partial<CIEC_WORD>(3) = CIEC_WORD(0x00FE);
  nTestLWord.partial<CIEC_WORD>(2) = CIEC_WORD(0xEDFA);
  nTestLWord.partial<CIEC_WORD>(1) = CIEC_WORD(0xCEC0);
  nTestLWord.partial<CIEC_WORD>(0) = CIEC_WORD(0xFFEE);

  BOOST_CHECK_EQUAL(nTestLWord, 0x00FEEDFACEC0FFEEU);
}

BOOST_AUTO_TEST_CASE(PARTIAL_ACCESS_LWORD_ASSIGN_DWORD_CHECK_BIT)
{
  CIEC_LWORD nTestLWord;

  nTestLWord.partial<CIEC_DWORD>(0) = CIEC_DWORD(0xADD511FE);
  nTestLWord.partial<CIEC_DWORD>(1) = CIEC_DWORD(0xC0CAC01A);

  test4X_C(nTestLWord,60);
  test4X_0(nTestLWord,56);
  test4X_C(nTestLWord,52);
  test4X_A(nTestLWord,48);
  test4X_C(nTestLWord,44);
  test4X_0(nTestLWord,40);
  test4X_1(nTestLWord,36);
  test4X_A(nTestLWord,32);
  test4X_A(nTestLWord,28);
  test4X_D(nTestLWord,24);
  test4X_D(nTestLWord,20);
  test4X_5(nTestLWord,16);
  test4X_1(nTestLWord,12);
  test4X_1(nTestLWord,8);
  test4X_F(nTestLWord,4);
  test4X_E(nTestLWord,0);
}

BOOST_AUTO_TEST_CASE(PARTIAL_ACCESS_LWORD_ASSIGN_DWORD_CHECK_BYTE)
{
  CIEC_LWORD nTestLWord;

  nTestLWord.partial<CIEC_DWORD>(0) = CIEC_DWORD(0xADD511FE);
  nTestLWord.partial<CIEC_DWORD>(1) = CIEC_DWORD(0xC0CAC01A);

  BOOST_CHECK_EQUAL((nTestLWord.partial<CIEC_BYTE>(7)), 0xC0U);
  BOOST_CHECK_EQUAL((nTestLWord.partial<CIEC_BYTE>(6)), 0xCAU);
  BOOST_CHECK_EQUAL((nTestLWord.partial<CIEC_BYTE>(5)), 0xC0U);
  BOOST_CHECK_EQUAL((nTestLWord.partial<CIEC_BYTE>(4)), 0x1AU);
  BOOST_CHECK_EQUAL((nTestLWord.partial<CIEC_BYTE>(3)), 0xADU);
  BOOST_CHECK_EQUAL((nTestLWord.partial<CIEC_BYTE>(2)), 0xD5U);
  BOOST_CHECK_EQUAL((nTestLWord.partial<CIEC_BYTE>(1)), 0x11U);
  BOOST_CHECK_EQUAL((nTestLWord.partial<CIEC_BYTE>(0)), 0xFEU);
}

BOOST_AUTO_TEST_CASE(PARTIAL_ACCESS_LWORD_ASSIGN_DWORD_CHECK_WORD)
{
  CIEC_LWORD nTestLWord;

  nTestLWord.partial<CIEC_DWORD>(0) = CIEC_DWORD(0xADD511FE);
  nTestLWord.partial<CIEC_DWORD>(1) = CIEC_DWORD(0xC0CAC01A);

  BOOST_CHECK_EQUAL((nTestLWord.partial<CIEC_WORD>(3)), 0xC0CAU);
  BOOST_CHECK_EQUAL((nTestLWord.partial<CIEC_WORD>(2)), 0xC01AU);
  BOOST_CHECK_EQUAL((nTestLWord.partial<CIEC_WORD>(1)), 0xADD5U);
  BOOST_CHECK_EQUAL((nTestLWord.partial<CIEC_WORD>(0)), 0x11FEU);
}

BOOST_AUTO_TEST_CASE(PARTIAL_ACCESS_LWORD_ASSIGN_DWORD_CHECK_DWORD)
{
  CIEC_LWORD nTestLWord;

  nTestLWord.partial<CIEC_DWORD>(0) = CIEC_DWORD(0xADD511FE);
  nTestLWord.partial<CIEC_DWORD>(1) = CIEC_DWORD(0xC0CAC01A);

  BOOST_CHECK_EQUAL((nTestLWord.partial<CIEC_DWORD>(1)), 0xC0CAC01AU);
  BOOST_CHECK_EQUAL((nTestLWord.partial<CIEC_DWORD>(0)), 0xADD511FEU);
}

BOOST_AUTO_TEST_CASE(PARTIAL_ACCESS_LWORD_ASSIGN_DWORD_CHECK_LWORD)
{
  CIEC_LWORD nTestLWord;

  nTestLWord.partial<CIEC_DWORD>(0) = CIEC_DWORD(0xADD511FE);
  nTestLWord.partial<CIEC_DWORD>(1) = CIEC_DWORD(0xC0CAC01A);

  BOOST_CHECK_EQUAL(nTestLWord, 0xC0CAC01AADD511FEU);
}

BOOST_AUTO_TEST_CASE(PARTIAL_ACCESS_LWORD_SETVALUE_DWORD_CHECK_LWORD)
{
  CIEC_LWORD nTestLWord;
  CIEC_DWORD nTestDWord;

  nTestDWord = CIEC_DWORD(0xADD511FE);
  nTestLWord.partial<CIEC_DWORD>(0).setValue(nTestDWord);

  nTestDWord = CIEC_DWORD(0xC0CAC01A);
  nTestLWord.partial<CIEC_DWORD>(1).setValue(nTestDWord);

  BOOST_CHECK_EQUAL(nTestLWord, 0xC0CAC01AADD511FEU);
}

BOOST_AUTO_TEST_CASE(PARTIAL_ACCESS_LWORD_SETVALUE_WORD_CHECK_LWORD)
{
  CIEC_LWORD nTestLWord;
  CIEC_WORD nTestWord;

  nTestWord = CIEC_WORD(0x11FE);
  nTestLWord.partial<CIEC_WORD>(0).setValue(nTestWord);

  nTestWord = CIEC_WORD(0xADD5);
  nTestLWord.partial<CIEC_WORD>(1).setValue(nTestWord);

  nTestWord = CIEC_WORD(0xC01A);
  nTestLWord.partial<CIEC_WORD>(2).setValue(nTestWord);

  nTestWord = CIEC_WORD(0xC0CA);
  nTestLWord.partial<CIEC_WORD>(3).setValue(nTestWord);

  BOOST_CHECK_EQUAL(nTestLWord, 0xC0CAC01AADD511FEU);
}

BOOST_AUTO_TEST_CASE(PARTIAL_ACCESS_LWORD_ASSIGN_CHAIN_DWORD_BYTE_CHECK_LWORD)
{
  CIEC_LWORD nTestLWord;

  nTestLWord.partial<CIEC_DWORD>(1).partial<CIEC_BYTE>(3) = CIEC_BYTE(0xDE);
  nTestLWord.partial<CIEC_DWORD>(1).partial<CIEC_BYTE>(2) = CIEC_BYTE(0xCA);
  nTestLWord.partial<CIEC_DWORD>(1).partial<CIEC_BYTE>(1) = CIEC_BYTE(0xFB);
  nTestLWord.partial<CIEC_DWORD>(1).partial<CIEC_BYTE>(0) = CIEC_BYTE(0xAD);
  nTestLWord.partial<CIEC_DWORD>(0).partial<CIEC_BYTE>(3) = CIEC_BYTE(0xBA);
  nTestLWord.partial<CIEC_DWORD>(0).partial<CIEC_BYTE>(2) = CIEC_BYTE(0xD0);
  nTestLWord.partial<CIEC_DWORD>(0).partial<CIEC_BYTE>(1) = CIEC_BYTE(0xCA);
  nTestLWord.partial<CIEC_DWORD>(0).partial<CIEC_BYTE>(0) = CIEC_BYTE(0xFE);

  BOOST_CHECK_EQUAL(nTestLWord, 0xDECAFBADBAD0CAFEU);
}

BOOST_AUTO_TEST_CASE(PARTIAL_ACCESS_LWORD_ASSIGN_CHAIN_DWORD_WORD_CHECK_LWORD)
{
  CIEC_LWORD nTestLWord;

  nTestLWord.partial<CIEC_DWORD>(1).partial<CIEC_WORD>(1) = CIEC_WORD(0x00FE);
  nTestLWord.partial<CIEC_DWORD>(1).partial<CIEC_WORD>(0) = CIEC_WORD(0xEDFA);
  nTestLWord.partial<CIEC_DWORD>(0).partial<CIEC_WORD>(1) = CIEC_WORD(0xCEC0);
  nTestLWord.partial<CIEC_DWORD>(0).partial<CIEC_WORD>(0) = CIEC_WORD(0xFFEE);

  BOOST_CHECK_EQUAL(nTestLWord, 0x00FEEDFACEC0FFEEU);
}

BOOST_AUTO_TEST_CASE(PARTIAL_ACCESS_LWORD_ASSIGN_CHAIN_DWORD_WORD_BIT_CHECK_LWORD)
{
  CIEC_LWORD nTestLWord;

  set4X_C((nTestLWord.partial<CIEC_DWORD>(1).partial<CIEC_WORD>(1)),12);
  set4X_0((nTestLWord.partial<CIEC_DWORD>(1).partial<CIEC_WORD>(1)),8);
  set4X_C((nTestLWord.partial<CIEC_DWORD>(1).partial<CIEC_WORD>(1)),4);
  set4X_A((nTestLWord.partial<CIEC_DWORD>(1).partial<CIEC_WORD>(1)),0);
  set4X_C((nTestLWord.partial<CIEC_DWORD>(1).partial<CIEC_WORD>(0)),12);
  set4X_0((nTestLWord.partial<CIEC_DWORD>(1).partial<CIEC_WORD>(0)),8);
  set4X_1((nTestLWord.partial<CIEC_DWORD>(1).partial<CIEC_WORD>(0)),4);
  set4X_A((nTestLWord.partial<CIEC_DWORD>(1).partial<CIEC_WORD>(0)),0);
  set4X_A((nTestLWord.partial<CIEC_DWORD>(0).partial<CIEC_WORD>(1)),12);
  set4X_D((nTestLWord.partial<CIEC_DWORD>(0).partial<CIEC_WORD>(1)),8);
  set4X_D((nTestLWord.partial<CIEC_DWORD>(0).partial<CIEC_WORD>(1)),4);
  set4X_5((nTestLWord.partial<CIEC_DWORD>(0).partial<CIEC_WORD>(1)),0);
  set4X_1((nTestLWord.partial<CIEC_DWORD>(0).partial<CIEC_WORD>(0)),12);
  set4X_1((nTestLWord.partial<CIEC_DWORD>(0).partial<CIEC_WORD>(0)),8);
  set4X_F((nTestLWord.partial<CIEC_DWORD>(0).partial<CIEC_WORD>(0)),4);
  set4X_E((nTestLWord.partial<CIEC_DWORD>(0).partial<CIEC_WORD>(0)),0);

  BOOST_CHECK_EQUAL(nTestLWord, 0xC0CAC01AADD511FEU);
}

BOOST_AUTO_TEST_CASE(PARTIAL_ACCESS_LWORD_ASSIGN_CHAIN_DWORD_WORD_BYTE_BIT_CHECK_LWORD)
{
  CIEC_LWORD nTestLWord;

  set4X_0((nTestLWord.partial<CIEC_DWORD>(1).partial<CIEC_WORD>(1).partial<CIEC_BYTE>(1)),4);
  set4X_0((nTestLWord.partial<CIEC_DWORD>(1).partial<CIEC_WORD>(1).partial<CIEC_BYTE>(1)),0);
  set4X_F((nTestLWord.partial<CIEC_DWORD>(1).partial<CIEC_WORD>(1).partial<CIEC_BYTE>(0)),4);
  set4X_E((nTestLWord.partial<CIEC_DWORD>(1).partial<CIEC_WORD>(1).partial<CIEC_BYTE>(0)),0);
  set4X_E((nTestLWord.partial<CIEC_DWORD>(1).partial<CIEC_WORD>(0).partial<CIEC_BYTE>(1)),4);
  set4X_D((nTestLWord.partial<CIEC_DWORD>(1).partial<CIEC_WORD>(0).partial<CIEC_BYTE>(1)),0);
  set4X_F((nTestLWord.partial<CIEC_DWORD>(1).partial<CIEC_WORD>(0).partial<CIEC_BYTE>(0)),4);
  set4X_A((nTestLWord.partial<CIEC_DWORD>(1).partial<CIEC_WORD>(0).partial<CIEC_BYTE>(0)),0);
  set4X_C((nTestLWord.partial<CIEC_DWORD>(0).partial<CIEC_WORD>(1).partial<CIEC_BYTE>(1)),4);
  set4X_E((nTestLWord.partial<CIEC_DWORD>(0).partial<CIEC_WORD>(1).partial<CIEC_BYTE>(1)),0);
  set4X_C((nTestLWord.partial<CIEC_DWORD>(0).partial<CIEC_WORD>(1).partial<CIEC_BYTE>(0)),4);
  set4X_0((nTestLWord.partial<CIEC_DWORD>(0).partial<CIEC_WORD>(1).partial<CIEC_BYTE>(0)),0);
  set4X_F((nTestLWord.partial<CIEC_DWORD>(0).partial<CIEC_WORD>(0).partial<CIEC_BYTE>(1)),4);
  set4X_F((nTestLWord.partial<CIEC_DWORD>(0).partial<CIEC_WORD>(0).partial<CIEC_BYTE>(1)),0);
  set4X_E((nTestLWord.partial<CIEC_DWORD>(0).partial<CIEC_WORD>(0).partial<CIEC_BYTE>(0)),4);
  set4X_E((nTestLWord.partial<CIEC_DWORD>(0).partial<CIEC_WORD>(0).partial<CIEC_BYTE>(0)),0);

  BOOST_CHECK_EQUAL(nTestLWord, 0x00FEEDFACEC0FFEEU);
}

BOOST_AUTO_TEST_CASE(PARTIAL_ACCESS_LWORD_ASSIGN_CHECK_CHAIN_LWORD_DWORD_WORD)
{

  CIEC_LWORD nTestLWord;

  nTestLWord = CIEC_LWORD(0x00FEEDFACEC0FFEEU);

  BOOST_CHECK_EQUAL((nTestLWord.partial<CIEC_DWORD>(1).partial<CIEC_WORD>(1)), 0x00FEU);
  BOOST_CHECK_EQUAL((nTestLWord.partial<CIEC_DWORD>(1).partial<CIEC_WORD>(0)), 0xEDFAU);
  BOOST_CHECK_EQUAL((nTestLWord.partial<CIEC_DWORD>(0).partial<CIEC_WORD>(1)), 0xCEC0U);
  BOOST_CHECK_EQUAL((nTestLWord.partial<CIEC_DWORD>(0).partial<CIEC_WORD>(0)), 0xFFEEU);
}

BOOST_AUTO_TEST_CASE(PARTIAL_ACCESS_LWORD_ASSIGN_CHECK_CHAIN_LWORD_DWORD_BYTE)
{

  CIEC_LWORD nTestLWord;

  nTestLWord = CIEC_LWORD(0x00FEEDFACEC0FFEEU);

  BOOST_CHECK_EQUAL((nTestLWord.partial<CIEC_DWORD>(1).partial<CIEC_BYTE>(3)), 0x00U);
  BOOST_CHECK_EQUAL((nTestLWord.partial<CIEC_DWORD>(1).partial<CIEC_BYTE>(2)), 0xFEU);
  BOOST_CHECK_EQUAL((nTestLWord.partial<CIEC_DWORD>(1).partial<CIEC_BYTE>(1)), 0xEDU);
  BOOST_CHECK_EQUAL((nTestLWord.partial<CIEC_DWORD>(1).partial<CIEC_BYTE>(0)), 0xFAU);
  BOOST_CHECK_EQUAL((nTestLWord.partial<CIEC_DWORD>(0).partial<CIEC_BYTE>(3)), 0xCEU);
  BOOST_CHECK_EQUAL((nTestLWord.partial<CIEC_DWORD>(0).partial<CIEC_BYTE>(2)), 0xC0U);
  BOOST_CHECK_EQUAL((nTestLWord.partial<CIEC_DWORD>(0).partial<CIEC_BYTE>(1)), 0xFFU);
  BOOST_CHECK_EQUAL((nTestLWord.partial<CIEC_DWORD>(0).partial<CIEC_BYTE>(0)), 0xEEU);
}

BOOST_AUTO_TEST_CASE(PARTIAL_ACCESS_LWORD_ASSIGN_CHECK_CHAIN_LWORD_DWORD_BIT)
{
  CIEC_LWORD nTestLWord;

  nTestLWord = CIEC_LWORD(0x00FEEDFACEC0FFEEU);

  test4X_0((nTestLWord.partial<CIEC_DWORD>(1)),28);
  test4X_0((nTestLWord.partial<CIEC_DWORD>(1)),24);
  test4X_F((nTestLWord.partial<CIEC_DWORD>(1)),20);
  test4X_E((nTestLWord.partial<CIEC_DWORD>(1)),16);
  test4X_E((nTestLWord.partial<CIEC_DWORD>(1)),12);
  test4X_D((nTestLWord.partial<CIEC_DWORD>(1)),8);
  test4X_F((nTestLWord.partial<CIEC_DWORD>(1)),4);
  test4X_A((nTestLWord.partial<CIEC_DWORD>(1)),0);
  test4X_C((nTestLWord.partial<CIEC_DWORD>(0)),28);
  test4X_E((nTestLWord.partial<CIEC_DWORD>(0)),24);
  test4X_C((nTestLWord.partial<CIEC_DWORD>(0)),20);
  test4X_0((nTestLWord.partial<CIEC_DWORD>(0)),16);
  test4X_F((nTestLWord.partial<CIEC_DWORD>(0)),12);
  test4X_F((nTestLWord.partial<CIEC_DWORD>(0)),8);
  test4X_E((nTestLWord.partial<CIEC_DWORD>(0)),4);
  test4X_E((nTestLWord.partial<CIEC_DWORD>(0)),0);
}

BOOST_AUTO_TEST_CASE(PARTIAL_ACCESS_LWORD_ASSIGN_CHECK_CHAIN_LWORD_DWORD_WORD_BIT)
{
  CIEC_LWORD nTestLWord;

  nTestLWord = CIEC_LWORD(0x00FEEDFACEC0FFEEU);

  test4X_0((nTestLWord.partial<CIEC_DWORD>(1).partial<CIEC_WORD>(1)),12);
  test4X_0((nTestLWord.partial<CIEC_DWORD>(1).partial<CIEC_WORD>(1)),8);
  test4X_F((nTestLWord.partial<CIEC_DWORD>(1).partial<CIEC_WORD>(1)),4);
  test4X_E((nTestLWord.partial<CIEC_DWORD>(1).partial<CIEC_WORD>(1)),0);
  test4X_E((nTestLWord.partial<CIEC_DWORD>(1).partial<CIEC_WORD>(0)),12);
  test4X_D((nTestLWord.partial<CIEC_DWORD>(1).partial<CIEC_WORD>(0)),8);
  test4X_F((nTestLWord.partial<CIEC_DWORD>(1).partial<CIEC_WORD>(0)),4);
  test4X_A((nTestLWord.partial<CIEC_DWORD>(1).partial<CIEC_WORD>(0)),0);
  test4X_C((nTestLWord.partial<CIEC_DWORD>(0).partial<CIEC_WORD>(1)),12);
  test4X_E((nTestLWord.partial<CIEC_DWORD>(0).partial<CIEC_WORD>(1)),8);
  test4X_C((nTestLWord.partial<CIEC_DWORD>(0).partial<CIEC_WORD>(1)),4);
  test4X_0((nTestLWord.partial<CIEC_DWORD>(0).partial<CIEC_WORD>(1)),0);
  test4X_F((nTestLWord.partial<CIEC_DWORD>(0).partial<CIEC_WORD>(0)),12);
  test4X_F((nTestLWord.partial<CIEC_DWORD>(0).partial<CIEC_WORD>(0)),8);
  test4X_E((nTestLWord.partial<CIEC_DWORD>(0).partial<CIEC_WORD>(0)),4);
  test4X_E((nTestLWord.partial<CIEC_DWORD>(0).partial<CIEC_WORD>(0)),0);
}

BOOST_AUTO_TEST_CASE(PARTIAL_ACCESS_LWORD_ASSIGN_CHECK_CHAIN_LWORD_DWORD_WORD_BYTE_BIT)
{
  CIEC_LWORD nTestLWord;

  nTestLWord = CIEC_LWORD(0x00FEEDFACEC0FFEEU);

  test4X_0((nTestLWord.partial<CIEC_DWORD>(1).partial<CIEC_WORD>(1).partial<CIEC_BYTE>(1)),4);
  test4X_0((nTestLWord.partial<CIEC_DWORD>(1).partial<CIEC_WORD>(1).partial<CIEC_BYTE>(1)),0);
  test4X_F((nTestLWord.partial<CIEC_DWORD>(1).partial<CIEC_WORD>(1).partial<CIEC_BYTE>(0)),4);
  test4X_E((nTestLWord.partial<CIEC_DWORD>(1).partial<CIEC_WORD>(1).partial<CIEC_BYTE>(0)),0);
  test4X_E((nTestLWord.partial<CIEC_DWORD>(1).partial<CIEC_WORD>(0).partial<CIEC_BYTE>(1)),4);
  test4X_D((nTestLWord.partial<CIEC_DWORD>(1).partial<CIEC_WORD>(0).partial<CIEC_BYTE>(1)),0);
  test4X_F((nTestLWord.partial<CIEC_DWORD>(1).partial<CIEC_WORD>(0).partial<CIEC_BYTE>(0)),4);
  test4X_A((nTestLWord.partial<CIEC_DWORD>(1).partial<CIEC_WORD>(0).partial<CIEC_BYTE>(0)),0);
  test4X_C((nTestLWord.partial<CIEC_DWORD>(0).partial<CIEC_WORD>(1).partial<CIEC_BYTE>(1)),4);
  test4X_E((nTestLWord.partial<CIEC_DWORD>(0).partial<CIEC_WORD>(1).partial<CIEC_BYTE>(1)),0);
  test4X_C((nTestLWord.partial<CIEC_DWORD>(0).partial<CIEC_WORD>(1).partial<CIEC_BYTE>(0)),4);
  test4X_0((nTestLWord.partial<CIEC_DWORD>(0).partial<CIEC_WORD>(1).partial<CIEC_BYTE>(0)),0);
  test4X_F((nTestLWord.partial<CIEC_DWORD>(0).partial<CIEC_WORD>(0).partial<CIEC_BYTE>(1)),4);
  test4X_F((nTestLWord.partial<CIEC_DWORD>(0).partial<CIEC_WORD>(0).partial<CIEC_BYTE>(1)),0);
  test4X_E((nTestLWord.partial<CIEC_DWORD>(0).partial<CIEC_WORD>(0).partial<CIEC_BYTE>(0)),4);
  test4X_E((nTestLWord.partial<CIEC_DWORD>(0).partial<CIEC_WORD>(0).partial<CIEC_BYTE>(0)),0);
}

BOOST_AUTO_TEST_CASE(PARTIAL_ACCESS_LWORD_ASSIGN_CHECK_CHAIN_LWORD_DWORD_BYTE_BIT)
{
  CIEC_LWORD nTestLWord;

  nTestLWord = CIEC_LWORD(0x00FEEDFACEC0FFEEU);

  BOOST_CHECK_EQUAL((nTestLWord.partial<CIEC_DWORD>(1).partial<CIEC_BYTE>(3)), 0x00U);
  BOOST_CHECK_EQUAL((nTestLWord.partial<CIEC_DWORD>(1).partial<CIEC_BYTE>(2)), 0xFEU);
  BOOST_CHECK_EQUAL((nTestLWord.partial<CIEC_DWORD>(1).partial<CIEC_BYTE>(1)), 0xEDU);
  BOOST_CHECK_EQUAL((nTestLWord.partial<CIEC_DWORD>(1).partial<CIEC_BYTE>(0)), 0xFAU);
  BOOST_CHECK_EQUAL((nTestLWord.partial<CIEC_DWORD>(0).partial<CIEC_BYTE>(3)), 0xCEU);
  BOOST_CHECK_EQUAL((nTestLWord.partial<CIEC_DWORD>(0).partial<CIEC_BYTE>(2)), 0xC0U);
  BOOST_CHECK_EQUAL((nTestLWord.partial<CIEC_DWORD>(0).partial<CIEC_BYTE>(1)), 0xFFU);
  BOOST_CHECK_EQUAL((nTestLWord.partial<CIEC_DWORD>(0).partial<CIEC_BYTE>(0)), 0xEEU);
}

BOOST_AUTO_TEST_CASE(PARTIAL_ACCESS_READ_WITH_IEC_TYPE_INDEX)
{
  CIEC_LWORD nLWord(0xBE);
  CIEC_BOOL bBool;
  bBool = nLWord.partial<CIEC_BOOL>(CIEC_SINT(0));
  BOOST_TEST(static_cast<CIEC_BOOL::TValueType>(bBool) == false);

  bBool = nLWord.partial<CIEC_BOOL>(CIEC_SINT(1));
  BOOST_TEST(static_cast<CIEC_BOOL::TValueType>(bBool) == true);
}

BOOST_AUTO_TEST_CASE(PARTIAL_ACCESS_WRITE_WITH_IEC_TYPE_INDEX)
{
  CIEC_LWORD nLWord;
  nLWord.partial<CIEC_BOOL>(CIEC_SINT(0)) = CIEC_BOOL(true);
  BOOST_TEST(static_cast<CIEC_LWORD::TValueType>(nLWord) == 1U);

  nLWord.partial<CIEC_BOOL>(CIEC_SINT(1)) = CIEC_BOOL(true);
  BOOST_TEST(static_cast<CIEC_LWORD::TValueType>(nLWord) == 3U);
}

BOOST_AUTO_TEST_CASE(PARTIAL_COPY_PARTIAL_BOOL_TO_OTHER_PARTIAL_BOOL) {
  CIEC_LWORD nLWord(0x000000000000001U);
  nLWord.partial<CIEC_BOOL>(CIEC_SINT(1)) = nLWord.partial<CIEC_BOOL>(CIEC_SINT(0));
  BOOST_TEST(static_cast<CIEC_LWORD::TValueType>(nLWord) == 0x03U);
}

BOOST_AUTO_TEST_CASE(PARTIAL_COPY_PARTIAL_BYTE_TO_OTHER_PARTIAL_BYTE) {
  CIEC_LWORD nLWord(0x00000000000000FEU);
  nLWord.partial<CIEC_BYTE>(CIEC_SINT(1)) = nLWord.partial<CIEC_BYTE>(CIEC_SINT(0));
  BOOST_TEST(static_cast<CIEC_LWORD::TValueType>(nLWord) == 0x0000FEFEU);
}

BOOST_AUTO_TEST_CASE(PARTIAL_COPY_PARTIAL_WORD_TO_OTHER_PARTIAL_WORD) {
  CIEC_LWORD nLWord(0x00000000000000FEU);
  nLWord.partial<CIEC_WORD>(CIEC_SINT(1)) = nLWord.partial<CIEC_WORD>(CIEC_SINT(0));
  BOOST_TEST(static_cast<CIEC_LWORD::TValueType>(nLWord) == 0x00FE00FEU);
}

BOOST_AUTO_TEST_CASE(PARTIAL_COPY_PARTIAL_DWORD_TO_OTHER_PARTIAL_DWORD) {
  CIEC_LWORD nLWord(0x00000000000000FEU);
  nLWord.partial<CIEC_DWORD>(CIEC_SINT(1)) = nLWord.partial<CIEC_DWORD>(CIEC_SINT(0));
  BOOST_TEST(static_cast<CIEC_LWORD::TValueType>(nLWord) == 0x000000FE000000FEU);
}

BOOST_AUTO_TEST_CASE(PARTIAL_COPY_PARTIAL_BOOL_TO_OTHER_PARTIAL_BYTE) {
  CIEC_LWORD nLWord(0x00000000000000FEU);
  nLWord.partial<CIEC_BYTE>(CIEC_SINT(1)) = nLWord.partial<CIEC_BOOL>(CIEC_SINT(1));
  BOOST_TEST(static_cast<CIEC_DWORD::TValueType>(nLWord) == 0x00000000000001FEU);
}

BOOST_AUTO_TEST_CASE(PARTIAL_COPY_PARTIAL_BOOL_TO_OTHER_PARTIAL_WORD) {
  CIEC_LWORD nLWord(0x00000000000000FEU);
  nLWord.partial<CIEC_WORD>(CIEC_SINT(1)) = nLWord.partial<CIEC_BOOL>(CIEC_SINT(1));
  BOOST_TEST(static_cast<CIEC_LWORD::TValueType>(nLWord) == 0x00000000000100FEU);
}

BOOST_AUTO_TEST_CASE(PARTIAL_COPY_PARTIAL_BYTE_TO_OTHER_PARTIAL_WORD) {
  CIEC_LWORD nLWord(0x00000000000000FEU);
  nLWord.partial<CIEC_WORD>(CIEC_SINT(1)) = nLWord.partial<CIEC_BYTE>(CIEC_SINT(0));
  BOOST_TEST(static_cast<CIEC_LWORD::TValueType>(nLWord) == 0x0000000000FE00FEU);
}

BOOST_AUTO_TEST_CASE(PARTIAL_COPY_PARTIAL_BOOL_TO_OTHER_PARTIAL_DWORD) {
  CIEC_LWORD nLWord(0x00000000000000FEU);
  nLWord.partial<CIEC_DWORD>(CIEC_SINT(1)) = nLWord.partial<CIEC_BOOL>(CIEC_SINT(1));
  BOOST_TEST(static_cast<CIEC_LWORD::TValueType>(nLWord) == 0x00000001000000FEU);
}

BOOST_AUTO_TEST_CASE(PARTIAL_COPY_PARTIAL_BYTE_TO_OTHER_PARTIAL_DWORD) {
  CIEC_LWORD nLWord(0x00000000000000FEU);
  nLWord.partial<CIEC_DWORD>(CIEC_SINT(1)) = nLWord.partial<CIEC_BYTE>(CIEC_SINT(0));
  BOOST_TEST(static_cast<CIEC_LWORD::TValueType>(nLWord) == 0x000000FE000000FEU);
}

BOOST_AUTO_TEST_CASE(PARTIAL_COPY_PARTIAL_WORD_TO_OTHER_PARTIAL_DWORD) {
  CIEC_LWORD nLWord(0x000000000000FEFEU);
  nLWord.partial<CIEC_DWORD>(CIEC_SINT(1)) = nLWord.partial<CIEC_WORD>(CIEC_SINT(0));
  BOOST_TEST(static_cast<CIEC_LWORD::TValueType>(nLWord) == 0x0000FEFE0000FEFEU);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()
