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

BOOST_AUTO_TEST_SUITE(AT_VARIABLE_BYTE)

BOOST_AUTO_TEST_CASE(AT_VARIABLE_BYTE_TO_BYTE_FROM_DWORD)
{
  CIEC_DWORD nTestDword = 3;
  CIEC_DWORD nTestDwordB = 3;

  ARRAY_AT<CIEC_BYTE, CIEC_DWORD,0,3> a(nTestDword);
  ARRAY_AT<CIEC_BYTE, CIEC_DWORD,0,3> b(nTestDwordB);

  b[0] = 0;
  b[1] = 1;
  b[2] = 2;
  b[3] = 3;

  a[0] = b[3];
  a[1] = b[2];
  a[2] = b[1];
  a[3] = b[0];

  BOOST_CHECK_EQUAL((int)a[0],3);
  BOOST_CHECK_EQUAL((int)a[1],2);
  BOOST_CHECK_EQUAL((int)a[2],1);
  BOOST_CHECK_EQUAL((int)a[3],0);

}

BOOST_AUTO_TEST_CASE(AT_VARIABLE_BYTE_READ)
{
  CIEC_BYTE nTestByte;

  nTestByte = 0xFF;

  ARRAY_AT<CIEC_BOOL, CIEC_BYTE, 0, 7> testArray(nTestByte);

  for(size_t i=0;i<8;i++) {
    BOOST_CHECK_EQUAL(testArray[i], true);
  }

}

BOOST_AUTO_TEST_CASE(AT_VARIABLE_BYTE_WRITE_CHECK_TEMPORARY)
{
  CIEC_BYTE nTestByte;

  nTestByte = 0xFF;

  ARRAY_AT<CIEC_BOOL, CIEC_BYTE, 0, 7> testArray(nTestByte);

  testArray[0]=false;

  BOOST_CHECK_EQUAL(testArray[0], false);

}

BOOST_AUTO_TEST_CASE(AT_VARIABLE_BYTE_WRITE_CHECK_TARGET_BIT0)
{
  CIEC_BYTE nTestByte;

  nTestByte = 0xFF;

  ARRAY_AT<CIEC_BOOL, CIEC_BYTE, 0, 7> testArray(nTestByte);

  testArray[0] = false;

  BOOST_CHECK_EQUAL((int)nTestByte, 0xFF-0x01);

}

BOOST_AUTO_TEST_CASE(AT_VARIABLE_BYTE_WRITE_CHECK_TARGET_BIT1)
{
  CIEC_BYTE nTestByte;

  nTestByte = 0xFF;

  ARRAY_AT<CIEC_BOOL, CIEC_BYTE, 0, 7> testArray(nTestByte);

  testArray[1] = false;

  BOOST_CHECK_EQUAL((int)nTestByte, 0xFF-0x02);

}

BOOST_AUTO_TEST_CASE(AT_VARIABLE_BYTE_WRITE_CHECK_TARGET_BIT2)
{
  CIEC_BYTE nTestByte;

  nTestByte = 0xFF;

  ARRAY_AT<CIEC_BOOL, CIEC_BYTE, 0, 7> testArray(nTestByte);

  testArray[2] = false;

  BOOST_CHECK_EQUAL((int)nTestByte, 0xFF-0x04);

}

BOOST_AUTO_TEST_CASE(AT_VARIABLE_BYTE_WRITE_CHECK_TARGET_BIT3)
{
  CIEC_BYTE nTestByte;

  nTestByte = 0xFF;

  ARRAY_AT<CIEC_BOOL, CIEC_BYTE, 0, 7> testArray(nTestByte);

  testArray[3] = false;

  BOOST_CHECK_EQUAL((int)nTestByte, 0xFF-0x08);

}

BOOST_AUTO_TEST_CASE(AT_VARIABLE_BYTE_WRITE_CHECK_TARGET_BIT4)
{
  CIEC_BYTE nTestByte;

  nTestByte = 0xFF;

  ARRAY_AT<CIEC_BOOL, CIEC_BYTE, 0, 7> testArray(nTestByte);

  testArray[4] = false;

  BOOST_CHECK_EQUAL((int)nTestByte, 0xFF-0x10);

}

BOOST_AUTO_TEST_CASE(AT_VARIABLE_BYTE_WRITE_CHECK_TARGET_BIT5)
{
  CIEC_BYTE nTestByte;

  nTestByte = 0xFF;

  ARRAY_AT<CIEC_BOOL, CIEC_BYTE, 0, 7> testArray(nTestByte);

  testArray[5] = false;

  BOOST_CHECK_EQUAL((int)nTestByte, 0xFF-0x20);

}

BOOST_AUTO_TEST_CASE(AT_VARIABLE_BYTE_WRITE_CHECK_TARGET_BIT6)
{
  CIEC_BYTE nTestByte;

  nTestByte = 0xFF;

  ARRAY_AT<CIEC_BOOL, CIEC_BYTE, 0, 7> testArray(nTestByte);

  testArray[6] = false;

  BOOST_CHECK_EQUAL((int)nTestByte, 0xFF-0x40);

}

BOOST_AUTO_TEST_CASE(AT_VARIABLE_BYTE_WRITE_CHECK_TARGET_BIT7)
{
  CIEC_BYTE nTestByte;

  nTestByte = 0xFF;

  ARRAY_AT<CIEC_BOOL, CIEC_BYTE, 0, 7> testArray(nTestByte);

  testArray[7] = false;

  BOOST_CHECK_EQUAL((int)nTestByte, 0xFF-0x80);
  BOOST_CHECK_EQUAL(testArray.getAccessedOutOfBounds(), false);

}

BOOST_AUTO_TEST_CASE(AT_VARIABLE_BYTE_WRITE_CHECK_TARGET_OUTOFRANGE)
{
  CIEC_BYTE nTestByte;

  nTestByte = 0xFF;

  ARRAY_AT<CIEC_BOOL, CIEC_BYTE, 0, 7> testArray(nTestByte);

  testArray[8] = false;

  BOOST_CHECK_EQUAL((int)nTestByte, 0xFF);
  BOOST_CHECK_EQUAL(testArray.getAccessedOutOfBounds(), true);

}

BOOST_AUTO_TEST_CASE(AT_VARIABLE_BYTE_READ_OUTOFRANGE)
{
  CIEC_BYTE nTestByte;

  nTestByte = 0xFF;

  ARRAY_AT<CIEC_BOOL, CIEC_BYTE, 0, 7> testArray(nTestByte);

  BOOST_CHECK_EQUAL(testArray[8], testArray[8]);
  BOOST_CHECK_EQUAL(testArray.getAccessedOutOfBounds(), true);

}

BOOST_AUTO_TEST_CASE(AT_VARIABLE_BYTE_MOVING_BIT)
{
  CIEC_BYTE nTestByte;

  nTestByte = 0x0;

  ARRAY_AT<CIEC_BOOL, CIEC_BYTE, 0, 7> testArray(nTestByte);

  for (size_t i=0; i < 8; i++) {
    testArray[i] = true;
    BOOST_CHECK_EQUAL(nTestByte, (TForteByte)1 << i);
    testArray[i] = false;
  }

}

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
