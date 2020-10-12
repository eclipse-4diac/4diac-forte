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
BOOST_AUTO_TEST_SUITE(AT_VARIABLE_WORD)

BOOST_AUTO_TEST_CASE(AT_VARIABLE_WORD_READ_BIT)
{
  CIEC_WORD nTestWord;

  nTestWord = 0xFFFF;

  ARRAY_AT<CIEC_BOOL, CIEC_WORD, 0, 15> testArray(nTestWord);

  for(size_t i=0;i<16;i++) {
    BOOST_CHECK_EQUAL(testArray[i], true);
    BOOST_CHECK_EQUAL(testArray.getAccessedOutOfBounds(), false);
  }

}

BOOST_AUTO_TEST_CASE(AT_VARIABLE_WORD_READ_BIT_OUTOFBOUNDS)
{
  CIEC_WORD nTestWord;
  ARRAY_AT<CIEC_BOOL, CIEC_WORD, 0, 15> testArray(nTestWord);

  BOOST_CHECK_EQUAL(testArray[16], testArray[16]);
  BOOST_CHECK_EQUAL(testArray.getAccessedOutOfBounds(), true);

}

BOOST_AUTO_TEST_CASE(AT_VARIABLE_WORD_WRITE_BIT)
{
  CIEC_WORD nTestWord;

  ARRAY_AT<CIEC_BOOL, CIEC_WORD, 0, 15> testArray(nTestWord);

  for(size_t i=0;i<16;i++) {
    nTestWord = 0xFFFF;
    testArray[i] = false;
    for(size_t j=0;j<16;j++) {
      BOOST_CHECK_EQUAL(testArray[j], i!=j);
    }
    BOOST_CHECK_EQUAL(testArray.getAccessedOutOfBounds(), false);
  }

}

BOOST_AUTO_TEST_CASE(AT_VARIABLE_WORD_WRITE_BIT_OUTOFBOUNDS)
{
  CIEC_WORD nTestWord;
  ARRAY_AT<CIEC_BOOL, CIEC_WORD, 0, 15> testArray(nTestWord);

  testArray[16] = true;
  BOOST_CHECK_EQUAL(testArray[16], testArray[16]);
  BOOST_CHECK_EQUAL(testArray.getAccessedOutOfBounds(), true);

}

BOOST_AUTO_TEST_CASE(AT_VARIABLE_WORD_READ_BYTE)
{
  CIEC_WORD nTestWord;

  nTestWord = 0xFFFF;

  ARRAY_AT<CIEC_BYTE, CIEC_WORD, 0, 1> testArray(nTestWord);

  for(size_t i=0;i<2;i++) {
    BOOST_CHECK_EQUAL((int)testArray[i], 0xFF);
    BOOST_CHECK_EQUAL(testArray.getAccessedOutOfBounds(), false);
  }

}

BOOST_AUTO_TEST_CASE(AT_VARIABLE_WORD_READ_BYTE_OUTOFBOUNDS)
{
  CIEC_WORD nTestWord;
  ARRAY_AT<CIEC_BYTE, CIEC_WORD, 0, 1> testArray(nTestWord);

  BOOST_CHECK_EQUAL((int)testArray[2], (int)testArray[2]);
  BOOST_CHECK_EQUAL(testArray.getAccessedOutOfBounds(), true);

}

BOOST_AUTO_TEST_CASE(AT_VARIABLE_WORD_WRITE_BYTE)
{
  CIEC_WORD nTestWord;

  ARRAY_AT<CIEC_BYTE, CIEC_WORD, 0, 1> testArray(nTestWord);

  for(size_t i=0;i<2;i++) {
    nTestWord = 0xFFFF;
    testArray[i] = false;
    for(size_t j=0;j<2;j++) {
      BOOST_CHECK_EQUAL(testArray[j], i!=j ? 0xFF : false);
    }
    BOOST_CHECK_EQUAL(testArray.getAccessedOutOfBounds(), false);
  }

}

BOOST_AUTO_TEST_CASE(AT_VARIABLE_WORD_WRITE_BYTE_OUTOFBOUNDS)
{
  CIEC_WORD nTestWord;
  ARRAY_AT<CIEC_BYTE, CIEC_WORD, 0, 1> testArray(nTestWord);

  testArray[2] = true;
  BOOST_CHECK_EQUAL(testArray[2], testArray[2]);
  BOOST_CHECK_EQUAL(testArray.getAccessedOutOfBounds(), true);

}

BOOST_AUTO_TEST_CASE(AT_VARIABLE_WORD_MOVING_BIT)
{
  CIEC_WORD nTestWord;

  nTestWord = 0x0;

  ARRAY_AT<CIEC_BOOL, CIEC_WORD, 0, 15> testArray(nTestWord);

  for (size_t i=0; i < 16; i++) {
    testArray[i] = true;
    BOOST_CHECK_EQUAL(nTestWord, (TForteWord)1 << i);
    testArray[i] = false;
  }

}

BOOST_AUTO_TEST_CASE(AT_VARIABLE_WORD_MOVING_BYTE)
{
  CIEC_WORD nTestWord;

  nTestWord = 0x0;

  ARRAY_AT<CIEC_BYTE, CIEC_WORD, 0, 1> testArray(nTestWord);

  for (size_t i=0; i < 2; i++) {
    testArray[i] = 0xFF;
    BOOST_CHECK_EQUAL(nTestWord, (TForteWord)0xFF << i*8);
    testArray[i] = 0;
  }

}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()
