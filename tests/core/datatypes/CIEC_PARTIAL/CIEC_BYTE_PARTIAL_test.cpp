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

#include "../../../src/core/datatypes/forte_sint.h"

#include "CIEC_PARTIAL_test.h"

using namespace boost::unit_test;

BOOST_AUTO_TEST_SUITE(ANY_BIT_PARTIAL)
BOOST_AUTO_TEST_SUITE(CIEC_ANY_BIT_PARTIAL_BYTE)

BOOST_AUTO_TEST_CASE(PARTIAL_ACCESS_BYTE_INITVALUES) {
  CIEC_BYTE nTestByte;

  test4X_0(nTestByte,4);
  test4X_0(nTestByte,0);

}

BOOST_AUTO_TEST_CASE(PARTIAL_ACCESS_BYTE_CONST_INIT) {
  const CIEC_BOOL nTestBool(true);
  CIEC_BYTE mTestByte;

  mTestByte.partial<CIEC_BOOL>(4) = nTestBool;

  BOOST_CHECK_EQUAL(mTestByte, 16);
}

BOOST_AUTO_TEST_CASE(PARTIAL_ACCESS_BYTE_VALUE1_CHECK_BYTE) {
  CIEC_BYTE nTestByte(0xBE);
  BOOST_CHECK_EQUAL(nTestByte, 0xBEU);

}

BOOST_AUTO_TEST_CASE(PARTIAL_ACCESS_BYTE_VALUE2_CHECK_BYTE) {
  CIEC_BYTE nTestByte(0xBA);
  BOOST_CHECK_EQUAL(nTestByte, 0xBAU);

}

BOOST_AUTO_TEST_CASE(PARTIAL_ACCESS_BYTE_VALUE1_CHECK_BIT) {
  CIEC_BYTE nTestByte(0xBE);
  test4X_B(nTestByte,4);
  test4X_E(nTestByte,0);

}

BOOST_AUTO_TEST_CASE(PARTIAL_ACCESS_BYTE_VALUE2_CHECK_BIT) {
  CIEC_BYTE nTestByte(0xBA);
  test4X_B(nTestByte,4);
  test4X_A(nTestByte,0);

}

BOOST_AUTO_TEST_CASE(PARTIAL_ACCESS_BYTE_VALUE_CHANGE_CHECK_BYTE)
{
  CIEC_BYTE nTestByte(0xBE);
  nTestByte = CIEC_BYTE(0xBA);
  BOOST_CHECK_EQUAL(nTestByte, 0xBAU);

}

BOOST_AUTO_TEST_CASE(PARTIAL_ACCESS_BYTE_VALUE_CHANGE_CHECK_BIT)
{
  CIEC_BYTE nTestByte(0xBE);
  nTestByte = CIEC_BYTE(0xBA);
  test4X_B(nTestByte,4);
  test4X_A(nTestByte,0);

}

BOOST_AUTO_TEST_CASE(PARTIAL_ACCESS_BYTE_ASSIGN_BIT_CHANGE_BYTE_CHECK_BYTE_A)
{
  CIEC_BYTE nTestByte;

  set4X_0(nTestByte,4);
  set4X_D(nTestByte,0);

  nTestByte = CIEC_BYTE(0xFE);

  BOOST_CHECK_EQUAL(nTestByte, 0xFEU);

}

BOOST_AUTO_TEST_CASE(PARTIAL_ACCESS_BYTE_ASSIGN_BIT_CHANGE_BYTE_CHECK_BYTE_B) {
  CIEC_BYTE nTestByte;

  set4X_0(nTestByte,4);
  set4X_D(nTestByte,0);

  nTestByte = CIEC_BYTE(0xFE);

  nTestByte = CIEC_BYTE(0x22);

  BOOST_CHECK_EQUAL(nTestByte, 0x22U);

}

BOOST_AUTO_TEST_CASE(PARTIAL_ACCESS_BYTE_ASSIGN_BIT_CHECK_BIT) {
  CIEC_BYTE nTestByte;

  set4X_0(nTestByte,4);
  set4X_D(nTestByte,0);

  test4X_0(nTestByte,4);
  test4X_D(nTestByte,0);

}

BOOST_AUTO_TEST_CASE(PARTIAL_ACCESS_BYTE_ASSIGN_BIT_CHANGE_BYTE_CHECK_BIT_A) {
  CIEC_BYTE nTestByte;

  set4X_0(nTestByte,4);
  set4X_D(nTestByte,0);

  nTestByte = CIEC_BYTE(0xFE);

  test4X_F(nTestByte,4);
  test4X_E(nTestByte,0);

}

BOOST_AUTO_TEST_CASE(PARTIAL_ACCESS_BYTE_ASSIGN_BIT_CHANGE_BYTE_CHECK_BIT_B) {
  CIEC_BYTE nTestByte;

  set4X_0(nTestByte,4);
  set4X_D(nTestByte,0);

  nTestByte = CIEC_BYTE(0xFE);

  nTestByte = CIEC_BYTE(0x22);

  test4X_2(nTestByte,4);
  test4X_2(nTestByte,0);

}

BOOST_AUTO_TEST_CASE(PARTIAL_ACCESS_BYTE_ASSIGN_FROM_CONST) {
  const CIEC_BOOL nTestBool(true);

  CIEC_BYTE mTestByte;

  mTestByte.partial<CIEC_BOOL>(3) = nTestBool;

  BOOST_CHECK_EQUAL(mTestByte, 8);
}

BOOST_AUTO_TEST_CASE(PARTIAL_ACCESS_READ_WITH_IEC_TYPE_INDEX) {
  CIEC_BYTE nByte(0xBE);
  CIEC_BOOL bBool;
  bBool = nByte.partial<CIEC_BOOL>(CIEC_SINT(0));
  BOOST_TEST(static_cast<CIEC_BOOL::TValueType>(bBool) == false);

  bBool = nByte.partial<CIEC_BOOL>(CIEC_SINT(1));
  BOOST_TEST(static_cast<CIEC_BOOL::TValueType>(bBool) == true);
}

BOOST_AUTO_TEST_CASE(PARTIAL_ACCESS_WRITE_WITH_IEC_TYPE_INDEX) {
  CIEC_BYTE nByte;
  nByte.partial<CIEC_BOOL>(CIEC_SINT(0)) = CIEC_BOOL(true);
  BOOST_TEST(static_cast<CIEC_BYTE::TValueType>(nByte) == 1U);

  nByte.partial<CIEC_BOOL>(CIEC_SINT(1)) = CIEC_BOOL(true);
  BOOST_TEST(static_cast<CIEC_BYTE::TValueType>(nByte) == 3U);
}

BOOST_AUTO_TEST_CASE(PARTIAL_ACCESSOUT_OF_INDEX_ACCESS) {
  CIEC_BYTE nByte(0xFFU);
  nByte.partial<CIEC_BOOL>(CIEC_SINT(8)) = CIEC_BOOL(false);
  BOOST_TEST(static_cast<CIEC_BYTE::TValueType>(nByte) == 0xFFU);

  BOOST_TEST(static_cast<CIEC_BOOL::TValueType>(nByte.partial<CIEC_BOOL>(CIEC_SINT(8))) == false);
}

BOOST_AUTO_TEST_CASE(PARTIAL_COPY_PARTIAL_TO_OTHER_PARTIAL) {
  CIEC_BYTE nByte(0x01U);
  nByte.partial<CIEC_BOOL>(CIEC_SINT(1)) = nByte.partial<CIEC_BOOL>(CIEC_SINT(0));
  BOOST_TEST(static_cast<CIEC_BYTE::TValueType>(nByte) == 0x03U);
}

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
