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

BOOST_AUTO_TEST_SUITE(AT_VARIABLE_DWORD)

BOOST_AUTO_TEST_CASE(AT_VARIABLE_DWORD_READ_BIT)
{
  CIEC_DWORD nTestDWord;

  nTestDWord = 0xFFFFFFFF;

  ARRAY_AT<CIEC_BOOL, CIEC_DWORD, 0, 31> testArray(nTestDWord);

  for(size_t i=0;i<32;i++) {
    BOOST_CHECK_EQUAL(testArray[i], true);
    BOOST_CHECK_EQUAL(testArray.getAccessedOutOfBounds(), false);
  }

}

BOOST_AUTO_TEST_CASE(AT_VARIABLE_DWORD_READ_BIT_OUTOFBOUNDS)
{
  CIEC_DWORD nTestDWord;
  ARRAY_AT<CIEC_BOOL, CIEC_DWORD, 0, 31> testArray(nTestDWord);

  BOOST_CHECK_EQUAL(testArray[32], testArray[32]);
  BOOST_CHECK_EQUAL(testArray.getAccessedOutOfBounds(), true);

}

BOOST_AUTO_TEST_CASE(AT_VARIABLE_DWORD_WRITE_BIT)
{
  CIEC_DWORD nTestDWord;

  ARRAY_AT<CIEC_BOOL, CIEC_DWORD, 0, 31> testArray(nTestDWord);

  for(size_t i=0;i<32;i++) {
    nTestDWord = 0xFFFFFFFF;
    testArray[i] = false;
    for(size_t j=0;j<32;j++) {
      BOOST_CHECK_EQUAL(testArray[j], i!=j);
    }
    BOOST_CHECK_EQUAL(testArray.getAccessedOutOfBounds(), false);
  }

}

BOOST_AUTO_TEST_CASE(AT_VARIABLE_DWORD_WRITE_BIT_OUTOFBOUNDS)
{
  CIEC_DWORD nTestDWord;
  ARRAY_AT<CIEC_BOOL, CIEC_DWORD, 0, 31> testArray(nTestDWord);

  testArray[32] = true;
  BOOST_CHECK_EQUAL(testArray.getAccessedOutOfBounds(), true);
  BOOST_CHECK_EQUAL(testArray[32], testArray[32]);

}

BOOST_AUTO_TEST_CASE(AT_VARIABLE_DWORD_READ_BYTE)
{
  CIEC_DWORD nTestDWord;

  nTestDWord = 0xFFFFFFFF;

  ARRAY_AT<CIEC_BYTE, CIEC_DWORD, 0, 3> testArray(nTestDWord);

  for(size_t i=0;i<4;i++) {
    BOOST_CHECK_EQUAL((int)testArray[i], 0xFF);
    BOOST_CHECK_EQUAL(testArray.getAccessedOutOfBounds(), false);
  }

}

BOOST_AUTO_TEST_CASE(AT_VARIABLE_DWORD_READ_BYTE_OUTOFBOUNDS)
{
  CIEC_DWORD nTestDWord;
  ARRAY_AT<CIEC_BYTE, CIEC_DWORD, 0, 3> testArray(nTestDWord);

  BOOST_CHECK_EQUAL((int)testArray[4], (int)testArray[4]);
  BOOST_CHECK_EQUAL(testArray.getAccessedOutOfBounds(), true);

}

BOOST_AUTO_TEST_CASE(AT_VARIABLE_DWORD_WRITE_BYTE)
{
  CIEC_DWORD nTestDWord;

  ARRAY_AT<CIEC_BYTE, CIEC_DWORD, 0, 3> testArray(nTestDWord);

  for(size_t i=0;i<4;i++) {
    nTestDWord = 0xFFFFFFFF;
    testArray[i] = false;
    for(size_t j=0;j<4;j++) {
      BOOST_CHECK_EQUAL(testArray[j], i!=j ? 0xFF : false);
    }
    BOOST_CHECK_EQUAL(testArray.getAccessedOutOfBounds(), false);
  }

}

BOOST_AUTO_TEST_CASE(AT_VARIABLE_DWORD_WRITE_BYTE_OUTOFBOUNDS)
{
  CIEC_DWORD nTestDWord;
  ARRAY_AT<CIEC_BYTE, CIEC_DWORD, 0, 3> testArray(nTestDWord);

  testArray[4] = true;
  BOOST_CHECK_EQUAL(testArray[4], testArray[4]);
  BOOST_CHECK_EQUAL(testArray.getAccessedOutOfBounds(), true);

}

BOOST_AUTO_TEST_CASE(AT_VARIABLE_DWORD_READ_WORD)
{
  CIEC_DWORD nTestDWord;

  nTestDWord = 0xFFFFFFFF;

  ARRAY_AT<CIEC_WORD, CIEC_DWORD, 0, 1> testArray(nTestDWord);

  for(size_t i=0;i<2;i++) {
    BOOST_CHECK_EQUAL((int)testArray[i], 0xFFFF);
    BOOST_CHECK_EQUAL(testArray.getAccessedOutOfBounds(), false);
  }

}

BOOST_AUTO_TEST_CASE(AT_VARIABLE_DWORD_READ_WORD_OUTOFBOUNDS)
{
  CIEC_DWORD nTestDWord;
  ARRAY_AT<CIEC_WORD, CIEC_DWORD, 0, 1> testArray(nTestDWord);

  BOOST_CHECK_EQUAL((int)testArray[2], (int)testArray[2]);
  BOOST_CHECK_EQUAL(testArray.getAccessedOutOfBounds(), true);

}

BOOST_AUTO_TEST_CASE(AT_VARIABLE_DWORD_WRITE_WORD)
{
  CIEC_DWORD nTestDWord;

  ARRAY_AT<CIEC_WORD, CIEC_DWORD, 0, 1> testArray(nTestDWord);

  for(size_t i=0;i<2;i++) {
    nTestDWord = 0xFFFFFFFF;
    testArray[i] = false;
    for(size_t j=0;j<2;j++) {
      BOOST_CHECK_EQUAL(testArray[j], i!=j ? 0xFFFF : false);
    }
    BOOST_CHECK_EQUAL(testArray.getAccessedOutOfBounds(), false);
  }

}

BOOST_AUTO_TEST_CASE(AT_VARIABLE_DWORD_WRITE_WORD_OUTOFBOUNDS)
{
  CIEC_DWORD nTestDWord;
  ARRAY_AT<CIEC_WORD, CIEC_DWORD, 0, 1> testArray(nTestDWord);

  testArray[2] = true;
  BOOST_CHECK_EQUAL(testArray[2], testArray[2]);
  BOOST_CHECK_EQUAL(testArray.getAccessedOutOfBounds(), true);

}

BOOST_AUTO_TEST_CASE(AT_VARIABLE_DWORD_MOVING_BIT)
{
  CIEC_DWORD nTestDWord;

  nTestDWord = 0x0;

  ARRAY_AT<CIEC_BOOL, CIEC_DWORD, 0, 31> testArray(nTestDWord);

  for (size_t i=0; i < 32; i++) {
    testArray[i] = true;
    BOOST_CHECK_EQUAL(nTestDWord, (TForteDWord)1 << i);
    testArray[i] = false;
  }

}

BOOST_AUTO_TEST_CASE(AT_VARIABLE_DWORD_MOVING_BYTE)
{
  CIEC_DWORD nTestDWord;

  nTestDWord = 0x0;

  ARRAY_AT<CIEC_BYTE, CIEC_DWORD, 0, 3> testArray(nTestDWord);

  for (size_t i=0; i < 4; i++) {
    testArray[i] = 0xFF;
    BOOST_CHECK_EQUAL(nTestDWord, (TForteDWord)0xFF << i*8);
    testArray[i] = 0;
  }

}

BOOST_AUTO_TEST_CASE(AT_VARIABLE_DWORD_MOVING_WORD)
{
  CIEC_DWORD nTestDWord;

  nTestDWord = 0x0;

  ARRAY_AT<CIEC_WORD, CIEC_DWORD, 0, 1> testArray(nTestDWord);

  for (size_t i=0; i < 2; i++) {
    testArray[i] = 0xF00D;
    BOOST_CHECK_EQUAL(nTestDWord, (TForteDWord)0xF00D << i*16);
    testArray[i] = 0;
  }

}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()
