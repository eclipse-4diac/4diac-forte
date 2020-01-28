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

#include "CIEC_PARTIAL_test.h"

using namespace boost::unit_test;

BOOST_AUTO_TEST_SUITE(CIEC_ANY_BIT_PARTIAL)

BOOST_AUTO_TEST_CASE(PARTIAL_ACCESS_BYTE_INITVALUES)
{
  CIEC_BYTE nTestByte;

  test4X_0(nTestByte,4);
  test4X_0(nTestByte,0);

}

BOOST_AUTO_TEST_CASE(PARTIAL_ACCESS_BYTE_VALUE1_CHECK_BYTE)
{
  CIEC_BYTE nTestByte;

  nTestByte=0xBE;
  BOOST_CHECK_EQUAL(nTestByte, 0xBEU);

}

BOOST_AUTO_TEST_CASE(PARTIAL_ACCESS_BYTE_VALUE2_CHECK_BYTE)
{
  CIEC_BYTE nTestByte;

  nTestByte=0xBA;
  BOOST_CHECK_EQUAL(nTestByte, 0xBAU);

}

BOOST_AUTO_TEST_CASE(PARTIAL_ACCESS_BYTE_VALUE1_CHECK_BIT)
{
  CIEC_BYTE nTestByte;

  nTestByte=0xBE;
  test4X_B(nTestByte,4);
  test4X_E(nTestByte,0);

}

BOOST_AUTO_TEST_CASE(PARTIAL_ACCESS_BYTE_VALUE2_CHECK_BIT)
{
  CIEC_BYTE nTestByte;

  nTestByte=0xBA;
  test4X_B(nTestByte,4);
  test4X_A(nTestByte,0);

}

BOOST_AUTO_TEST_CASE(PARTIAL_ACCESS_BYTE_VALUE_CHANGE_CHECK_BYTE)
{
  CIEC_BYTE nTestByte;

  nTestByte=0xBE;
  nTestByte=0xBA;
  BOOST_CHECK_EQUAL(nTestByte, 0xBAU);

}

BOOST_AUTO_TEST_CASE(PARTIAL_ACCESS_BYTE_VALUE_CHANGE_CHECK_BIT)
{
  CIEC_BYTE nTestByte;

  nTestByte=0xBE;
  nTestByte=0xBA;
  test4X_B(nTestByte,4);
  test4X_A(nTestByte,0);

}

BOOST_AUTO_TEST_CASE(PARTIAL_ACCESS_BYTE_ASSIGN_BIT_CHANGE_BYTE_CHECK_BYTE_A)
{
  CIEC_BYTE nTestByte;

  set4X_0(nTestByte,4);
  set4X_D(nTestByte,0);

  nTestByte = 0xFE;

  BOOST_CHECK_EQUAL(nTestByte, 0xFEU);

}

BOOST_AUTO_TEST_CASE(PARTIAL_ACCESS_BYTE_ASSIGN_BIT_CHANGE_BYTE_CHECK_BYTE_B)
{
  CIEC_BYTE nTestByte;

  set4X_0(nTestByte,4);
  set4X_D(nTestByte,0);

  nTestByte = 0xFE;

  nTestByte = 0x22;

  BOOST_CHECK_EQUAL(nTestByte, 0x22U);

}

BOOST_AUTO_TEST_CASE(PARTIAL_ACCESS_BYTE_ASSIGN_BIT_CHECK_BIT)
{
  CIEC_BYTE nTestByte;

  set4X_0(nTestByte,4);
  set4X_D(nTestByte,0);

  test4X_0(nTestByte,4);
  test4X_D(nTestByte,0);

}

BOOST_AUTO_TEST_CASE(PARTIAL_ACCESS_BYTE_ASSIGN_BIT_CHANGE_BYTE_CHECK_BIT_A)
{
  CIEC_BYTE nTestByte;

  set4X_0(nTestByte,4);
  set4X_D(nTestByte,0);

  nTestByte = 0xFE;

  test4X_F(nTestByte,4);
  test4X_E(nTestByte,0);

}

BOOST_AUTO_TEST_CASE(PARTIAL_ACCESS_BYTE_ASSIGN_BIT_CHANGE_BYTE_CHECK_BIT_B)
{
  CIEC_BYTE nTestByte;

  set4X_0(nTestByte,4);
  set4X_D(nTestByte,0);

  nTestByte = 0xFE;

  nTestByte = 0x22;

  test4X_2(nTestByte,4);
  test4X_2(nTestByte,0);

}

BOOST_AUTO_TEST_SUITE_END()
