/*******************************************************************************
 * Copyright (c) 2018 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include <boost/test/unit_test.hpp>
#include "../../../src/core/datatypes/forte_any.h"
#include "../../../src/core/datatypes/forte_any_bit.h"
#include "../../../src/core/datatypes/forte_any_date.h"
#include "../../../src/core/datatypes/forte_any_derived.h"
#include "../../../src/core/datatypes/forte_any_elementary.h"
#include "../../../src/core/datatypes/forte_any_int.h"
#include "../../../src/core/datatypes/forte_any_magnitude.h"
#include "../../../src/core/datatypes/forte_any_num.h"
#include "../../../src/core/datatypes/forte_any_real.h"
#include "../../../src/core/datatypes/forte_any_string.h"

BOOST_AUTO_TEST_SUITE(CIEC_ARRAY_function_test)
  void performToStringTests(CIEC_ANY &paTestee){
    char buffer[10];

    for(int i = 0; i < 9; i++){
      BOOST_CHECK_EQUAL(paTestee.toString(buffer, i), -1);
    }

    BOOST_CHECK_EQUAL(paTestee.toString(buffer, 9), strlen("ND (ANY)"));
    BOOST_CHECK_EQUAL(0, strcmp(buffer, "ND (ANY)"));

    BOOST_CHECK_EQUAL(paTestee.toString(buffer, 10), strlen("ND (ANY)"));
    BOOST_CHECK_EQUAL(0, strcmp(buffer, "ND (ANY)"));

  }


  BOOST_AUTO_TEST_CASE(ANY_toString_Test){
    CIEC_ANY *poTest = CIEC_ANY::createDataType(0);
    performToStringTests(*poTest);
    delete poTest;
  }

  BOOST_AUTO_TEST_CASE(ANY_BIT_toString_Test){
    CIEC_ANY *poTest = CIEC_ANY_BIT::createDataType(0);
    performToStringTests(*poTest);
    delete poTest;
  }

  BOOST_AUTO_TEST_CASE(ANY_DERIVED_toString_Test){
    CIEC_ANY *poTest = CIEC_ANY_DERIVED::createDataType(0);
    performToStringTests(*poTest);
    delete poTest;
  }

  BOOST_AUTO_TEST_CASE(ANY_ELEMENTARY_toString_Test){
    CIEC_ANY *poTest = CIEC_ANY_ELEMENTARY::createDataType(0);
    performToStringTests(*poTest);
    delete poTest;
  }

  BOOST_AUTO_TEST_CASE(ANY_INT_toString_Test){
    CIEC_ANY *poTest = CIEC_ANY_INT::createDataType(0);
    performToStringTests(*poTest);
    delete poTest;
  }

  BOOST_AUTO_TEST_CASE(ANY_MAGNITUDE_toString_Test){
    CIEC_ANY *poTest = CIEC_ANY_MAGNITUDE::createDataType(0);
    performToStringTests(*poTest);
    delete poTest;
  }

  BOOST_AUTO_TEST_CASE(ANY_NUM_toString_Test){
    CIEC_ANY *poTest = CIEC_ANY_NUM::createDataType(0);
    performToStringTests(*poTest);
    delete poTest;
  }

  BOOST_AUTO_TEST_CASE(ANY_REAL_toString_Test){
    CIEC_ANY *poTest = CIEC_ANY_REAL::createDataType(0);
    performToStringTests(*poTest);
    delete poTest;
  }

  BOOST_AUTO_TEST_CASE(ANY_STRING_toString_Test){
    CIEC_ANY *poTest = CIEC_ANY_STRING::createDataType(0);
    performToStringTests(*poTest);
    delete poTest;
  }



BOOST_AUTO_TEST_SUITE_END()

