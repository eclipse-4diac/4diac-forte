/*******************************************************************************
 * Copyright (c) 2015 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl  - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include <boost/test/unit_test.hpp>
#include "../../src/core/mgmcmdstruct.h"

/*!Tests for the TNameIdentifier class used to store name identifier lists
 *
 * Although it officially holds CstringDicitionalry::TStringIds we can use for most tests normal strings
 * as there exists an own test for StringDict and TSringIds are just ints.
 *
 */
BOOST_AUTO_TEST_SUITE(TNameIdentifierTests)

  BOOST_AUTO_TEST_CASE(newEmptyNameList){
    forte::core::TNameIdentifier identifier;

    BOOST_CHECK(identifier.isEmpty());

    BOOST_CHECK_EQUAL(0, identifier.size());
  }

  BOOST_AUTO_TEST_CASE(capacity){
    //check that we have the capacity as configured in cmake
    forte::core::TNameIdentifier identifier;

    for(size_t i = 0; i < FORTE_MGM_MAX_SUPPORTED_NAME_HIERARCHY; i++){
      BOOST_CHECK(identifier.pushBack(static_cast<CStringDictionary::TStringId>(i)));
      BOOST_CHECK_EQUAL(false, identifier.isEmpty());
    }

    BOOST_CHECK_EQUAL(false, identifier.pushBack(10));

    identifier.clear();
    BOOST_CHECK(identifier.isEmpty());
  }

  BOOST_AUTO_TEST_CASE(pushBack){
    forte::core::TNameIdentifier identifier;

    identifier.pushBack(5);
    BOOST_CHECK_EQUAL(1, identifier.size());
    BOOST_CHECK_EQUAL(5, identifier[0]);
    BOOST_CHECK_EQUAL(5, identifier.back());
    BOOST_CHECK_EQUAL(5, identifier.front());

    identifier.pushBack(10);
    BOOST_CHECK_EQUAL(2, identifier.size());
    BOOST_CHECK_EQUAL(5, identifier[0]);
    BOOST_CHECK_EQUAL(10, identifier[1]);
    BOOST_CHECK_EQUAL(10, identifier.back());
    BOOST_CHECK_EQUAL(5, identifier.front());

    identifier.pushBack(215);
    BOOST_CHECK_EQUAL(3, identifier.size());
    BOOST_CHECK_EQUAL(5, identifier[0]);
    BOOST_CHECK_EQUAL(10, identifier[1]);
    BOOST_CHECK_EQUAL(215, identifier[2]);
    BOOST_CHECK_EQUAL(215, identifier.back());
    BOOST_CHECK_EQUAL(5, identifier.front());

    identifier.pushBack(3564);
    BOOST_CHECK_EQUAL(4, identifier.size());
    BOOST_CHECK_EQUAL(5, identifier[0]);
    BOOST_CHECK_EQUAL(10, identifier[1]);
    BOOST_CHECK_EQUAL(215, identifier[2]);
    BOOST_CHECK_EQUAL(3564, identifier[3]);
    BOOST_CHECK_EQUAL(3564, identifier.back());
    BOOST_CHECK_EQUAL(5, identifier.front());
  }

  BOOST_AUTO_TEST_CASE(popBack){
    forte::core::TNameIdentifier identifier;
    //fill with testdata
    for(size_t i = 0; i < FORTE_MGM_MAX_SUPPORTED_NAME_HIERARCHY; i++){
      identifier.pushBack(static_cast<CStringDictionary::TStringId>(i));
    }

    for(size_t i = 0; i < FORTE_MGM_MAX_SUPPORTED_NAME_HIERARCHY; i++){
      BOOST_CHECK_EQUAL((FORTE_MGM_MAX_SUPPORTED_NAME_HIERARCHY - 1 - i), identifier.back());
      BOOST_CHECK_EQUAL((FORTE_MGM_MAX_SUPPORTED_NAME_HIERARCHY - i), identifier.size());
      identifier.popBack();
      BOOST_CHECK_EQUAL((FORTE_MGM_MAX_SUPPORTED_NAME_HIERARCHY - 1 - i), identifier.size());
    }
    BOOST_CHECK(identifier.isEmpty());
  }

  BOOST_AUTO_TEST_CASE(iterator){
    forte::core::TNameIdentifier identifier;
    //fill with testdata
    for(size_t i = 0; i < FORTE_MGM_MAX_SUPPORTED_NAME_HIERARCHY; i++){
      identifier.pushBack(static_cast<CStringDictionary::TStringId>(i));
    }

    int i = 0;
    for(forte::core::TNameIdentifier::CIterator it(identifier.begin()); it != identifier.end(); ++it, ++i){
      BOOST_CHECK_EQUAL(i, *it);
    }
    BOOST_CHECK_EQUAL(FORTE_MGM_MAX_SUPPORTED_NAME_HIERARCHY, i);  //we should have exactly visited each element once but started at zero
  }

  BOOST_AUTO_TEST_SUITE_END()
