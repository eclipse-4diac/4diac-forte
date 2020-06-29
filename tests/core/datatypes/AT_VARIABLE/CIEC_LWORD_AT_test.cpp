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
 *   Ernst Blecha - add at-variable tests
 *******************************************************************************/

#include <boost/test/unit_test.hpp>

#include "../../../src/core/datatypes/forte_array_at.h"

using namespace boost::unit_test;

BOOST_AUTO_TEST_SUITE(AT_VARIABLE)

BOOST_AUTO_TEST_SUITE(AT_VARIABLE_LWORD)

BOOST_AUTO_TEST_CASE(AT_VARIABLE_LWORD_READ_BIT)
{
  CIEC_LWORD nTestLWord;

  nTestLWord = 0xFFFFFFFFFFFFFFFFU;

  ARRAY_AT<CIEC_BOOL, CIEC_LWORD, 0, 63> testArray(nTestLWord);

  for(size_t i=0;i<64;i++) {
    BOOST_CHECK_EQUAL(testArray[i], true);
    BOOST_CHECK_EQUAL(testArray.getAccessedOutOfBounds(), false);
  }

}

BOOST_AUTO_TEST_CASE(AT_VARIABLE_LWORD_READ_BIT_OUTOFBOUNDS)
{
  CIEC_LWORD nTestLWord;
  ARRAY_AT<CIEC_BOOL, CIEC_LWORD, 0, 63> testArray(nTestLWord);

  BOOST_CHECK_EQUAL(testArray[64], testArray[64]);
  BOOST_CHECK_EQUAL(testArray.getAccessedOutOfBounds(), true);

}

BOOST_AUTO_TEST_CASE(AT_VARIABLE_LWORD_WRITE_BIT)
{
  CIEC_LWORD nTestLWord;

  ARRAY_AT<CIEC_BOOL, CIEC_LWORD, 0, 63> testArray(nTestLWord);

  for(size_t i=0;i<64;i++) {
    nTestLWord = 0xFFFFFFFFFFFFFFFFU;
    testArray[i] = false;
    for(size_t j=0;j<64;j++) {
      BOOST_CHECK_EQUAL(testArray[j], i!=j);
    }
    BOOST_CHECK_EQUAL(testArray.getAccessedOutOfBounds(), false);
  }

}

BOOST_AUTO_TEST_CASE(AT_VARIABLE_LWORD_WRITE_BIT_OUTOFBOUNDS)
{
  CIEC_LWORD nTestLWord;
  ARRAY_AT<CIEC_BOOL, CIEC_LWORD, 0, 63> testArray(nTestLWord);

  testArray[64] = true;
  BOOST_CHECK_EQUAL(testArray.getAccessedOutOfBounds(), true);
  BOOST_CHECK_EQUAL(testArray[64], testArray[64]);

}

BOOST_AUTO_TEST_CASE(AT_VARIABLE_LWORD_READ_BYTE)
{
  CIEC_LWORD nTestLWord;

  nTestLWord = 0xFFFFFFFFFFFFFFFFU;

  ARRAY_AT<CIEC_BYTE, CIEC_LWORD, 0, 7> testArray(nTestLWord);

  for(size_t i=0;i<8;i++) {
    BOOST_CHECK_EQUAL((int)testArray[i], 0xFF);
    BOOST_CHECK_EQUAL(testArray.getAccessedOutOfBounds(), false);
  }

}

BOOST_AUTO_TEST_CASE(AT_VARIABLE_LWORD_READ_BYTE_OUTOFBOUNDS)
{
  CIEC_LWORD nTestLWord;
  ARRAY_AT<CIEC_BYTE, CIEC_LWORD, 0, 7> testArray(nTestLWord);

  BOOST_CHECK_EQUAL((int)testArray[8], (int)testArray[8]);
  BOOST_CHECK_EQUAL(testArray.getAccessedOutOfBounds(), true);

}

BOOST_AUTO_TEST_CASE(AT_VARIABLE_LWORD_WRITE_BYTE)
{
  CIEC_LWORD nTestLWord;

  ARRAY_AT<CIEC_BYTE, CIEC_LWORD, 0, 7> testArray(nTestLWord);

  for(size_t i=0;i<8;i++) {
    nTestLWord = 0xFFFFFFFFFFFFFFFFU;
    testArray[i] = false;
    for(size_t j=0;j<8;j++) {
      BOOST_CHECK_EQUAL(testArray[j], i!=j ? 0xFF : false);
    }
    BOOST_CHECK_EQUAL(testArray.getAccessedOutOfBounds(), false);
  }

}

BOOST_AUTO_TEST_CASE(AT_VARIABLE_LWORD_WRITE_BYTE_OUTOFBOUNDS)
{
  CIEC_LWORD nTestLWord;
  ARRAY_AT<CIEC_BYTE, CIEC_LWORD, 0, 7> testArray(nTestLWord);

  testArray[8] = true;
  BOOST_CHECK_EQUAL(testArray[8], testArray[8]);
  BOOST_CHECK_EQUAL(testArray.getAccessedOutOfBounds(), true);

}

BOOST_AUTO_TEST_CASE(AT_VARIABLE_LWORD_READ_WORD)
{
  CIEC_LWORD nTestLWord;

  nTestLWord = 0xFFFFFFFFFFFFFFFFU;

  ARRAY_AT<CIEC_WORD, CIEC_LWORD, 0, 3> testArray(nTestLWord);

  for(size_t i=0;i<4;i++) {
    BOOST_CHECK_EQUAL((int)testArray[i], 0xFFFF);
    BOOST_CHECK_EQUAL(testArray.getAccessedOutOfBounds(), false);
  }

}

BOOST_AUTO_TEST_CASE(AT_VARIABLE_LWORD_READ_WORD_OUTOFBOUNDS)
{
  CIEC_LWORD nTestLWord;
  ARRAY_AT<CIEC_WORD, CIEC_LWORD, 0, 3> testArray(nTestLWord);

  BOOST_CHECK_EQUAL((int)testArray[4], (int)testArray[4]);
  BOOST_CHECK_EQUAL(testArray.getAccessedOutOfBounds(), true);

}

BOOST_AUTO_TEST_CASE(AT_VARIABLE_LWORD_WRITE_WORD)
{
  CIEC_LWORD nTestLWord;

  ARRAY_AT<CIEC_WORD, CIEC_LWORD, 0, 3> testArray(nTestLWord);

  for(size_t i=0;i<4;i++) {
    nTestLWord = 0xFFFFFFFFFFFFFFFFU;
    testArray[i] = false;
    for(size_t j=0;j<4;j++) {
      BOOST_CHECK_EQUAL(testArray[j], i!=j ? 0xFFFF : false);
    }
    BOOST_CHECK_EQUAL(testArray.getAccessedOutOfBounds(), false);
  }

}

BOOST_AUTO_TEST_CASE(AT_VARIABLE_LWORD_WRITE_WORD_OUTOFBOUNDS)
{
  CIEC_LWORD nTestLWord;
  ARRAY_AT<CIEC_WORD, CIEC_LWORD, 0, 3> testArray(nTestLWord);

  testArray[4] = true;
  BOOST_CHECK_EQUAL(testArray[4], testArray[4]);
  BOOST_CHECK_EQUAL(testArray.getAccessedOutOfBounds(), true);

}

BOOST_AUTO_TEST_CASE(AT_VARIABLE_LWORD_READ_DWORD)
{
  CIEC_LWORD nTestLWord;

  nTestLWord = 0xFFFFFFFFFFFFFFFFU;

  ARRAY_AT<CIEC_DWORD, CIEC_LWORD, 0, 1> testArray(nTestLWord);

  for(size_t i=0;i<2;i++) {
    BOOST_CHECK_EQUAL(testArray[i], 0xFFFFFFFF);
    BOOST_CHECK_EQUAL(testArray.getAccessedOutOfBounds(), false);
  }

}

BOOST_AUTO_TEST_CASE(AT_VARIABLE_LWORD_READ_DWORD_OUTOFBOUNDS)
{
  CIEC_LWORD nTestLWord;
  ARRAY_AT<CIEC_DWORD, CIEC_LWORD, 0, 1> testArray(nTestLWord);

  BOOST_CHECK_EQUAL(testArray[2], testArray[2]);
  BOOST_CHECK_EQUAL(testArray.getAccessedOutOfBounds(), true);

}

BOOST_AUTO_TEST_CASE(AT_VARIABLE_LWORD_WRITE_DWORD)
{
  CIEC_LWORD nTestLWord;

  ARRAY_AT<CIEC_DWORD, CIEC_LWORD, 0, 1> testArray(nTestLWord);

  for(size_t i=0;i<2;i++) {
    nTestLWord = 0xFFFFFFFFFFFFFFFFU;
    testArray[i] = false;
    for(size_t j=0;j<2;j++) {
      BOOST_CHECK_EQUAL(testArray[j], i!=j ? 0xFFFFFFFF : false);
    }
    BOOST_CHECK_EQUAL(testArray.getAccessedOutOfBounds(), false);
  }

}

BOOST_AUTO_TEST_CASE(AT_VARIABLE_LWORD_WRITE_DWORD_OUTOFBOUNDS)
{
  CIEC_LWORD nTestLWord;
  ARRAY_AT<CIEC_DWORD, CIEC_LWORD, 0, 1> testArray(nTestLWord);

  testArray[2] = true;
  BOOST_CHECK_EQUAL(testArray[2], testArray[2]);
  BOOST_CHECK_EQUAL(testArray.getAccessedOutOfBounds(), true);

}

BOOST_AUTO_TEST_CASE(AT_VARIABLE_LWORD_MOVING_BIT)
{
  CIEC_LWORD nTestLWord;

  nTestLWord = 0x0;

  ARRAY_AT<CIEC_BOOL, CIEC_LWORD, 0, 63> testArray(nTestLWord);

  for (size_t i=0; i < 64; i++) {
    testArray[i] = true;
    BOOST_CHECK_EQUAL(nTestLWord, (TForteLWord)1 << i);
    testArray[i] = false;
  }

}


BOOST_AUTO_TEST_CASE(AT_VARIABLE_LWORD_MOVING_BYTE)
{
  CIEC_LWORD nTestLWord;

  nTestLWord = 0x0;

  ARRAY_AT<CIEC_BYTE, CIEC_LWORD, 0, 7> testArray(nTestLWord);

  for (size_t i=0; i < 8; i++) {
    testArray[i] = 0xFF;
    BOOST_CHECK_EQUAL(nTestLWord, (TForteLWord)0xFF << i*8);
    testArray[i] = 0;
  }

}

BOOST_AUTO_TEST_CASE(AT_VARIABLE_LWORD_MOVING_WORD)
{
  CIEC_LWORD nTestLWord;

  nTestLWord = 0x0;

  ARRAY_AT<CIEC_WORD, CIEC_LWORD, 0, 3> testArray(nTestLWord);

  for (size_t i=0; i < 4; i++) {
    testArray[i] = 0xF00D;
    BOOST_CHECK_EQUAL(nTestLWord, (TForteLWord)0xF00D << i*16);
    testArray[i] = 0;
  }

}

BOOST_AUTO_TEST_CASE(AT_VARIABLE_LWORD_MOVING_DWORD)
{
  CIEC_LWORD nTestLWord;

  nTestLWord = 0x0;

  ARRAY_AT<CIEC_DWORD, CIEC_LWORD, 0, 1> testArray(nTestLWord);

  for (size_t i=0; i < 2; i++) {
    testArray[i] = 0xFC0FFEEF;
    BOOST_CHECK_EQUAL(nTestLWord, (TForteLWord)0xFC0FFEEF << i*32);
    testArray[i] = 0;
  }

}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()
