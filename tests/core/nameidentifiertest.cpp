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
#include "forte/mgmcmd.h"

using namespace forte::literals;

namespace forte::test {

  /*!Tests for the TNameIdentifier class used to store name identifier lists
   *
   * Although it officially holds CstringDicitionalry::TStringIds we can use for most tests normal strings
   * as there exists an own test for StringDict and TSringIds are just ints.
   *
   */
  BOOST_AUTO_TEST_SUITE(TNameIdentifierTests)

  BOOST_AUTO_TEST_CASE(newEmptyNameList) {
    TNameIdentifier identifier;

    BOOST_CHECK(identifier.empty());

    BOOST_CHECK_EQUAL(0, identifier.size());
  }

  BOOST_AUTO_TEST_CASE(newListInitializedNameList) {
    TNameIdentifier identifier{"17"_STRID, "4"_STRID, "21"_STRID};

    BOOST_TEST(identifier.size() == 3);
    BOOST_TEST(identifier[0] == "17"_STRID);
    BOOST_TEST(identifier[1] == "4"_STRID);
    BOOST_TEST(identifier[2] == "21"_STRID);
  }

  BOOST_AUTO_TEST_CASE(newIteratorInitializedNameList) {
    auto values = std::array{"17"_STRID, "4"_STRID, "21"_STRID};
    TNameIdentifier identifier(values.begin(), values.end());

    BOOST_TEST(identifier.size() == 3);
    BOOST_TEST(identifier[0] == "17"_STRID);
    BOOST_TEST(identifier[1] == "4"_STRID);
    BOOST_TEST(identifier[2] == "21"_STRID);
  }

  BOOST_AUTO_TEST_CASE(capacity) {
    // check that we have the capacity as configured in cmake
    TNameIdentifier identifier;

    for (size_t i = 0; i < cgMaxSupportedNameHierarchy; i++) {
      BOOST_CHECK(identifier.push_back(StringId::insert(std::to_string(i))));
      BOOST_CHECK_EQUAL(false, identifier.empty());
    }

    BOOST_CHECK_EQUAL(nullptr, identifier.try_push_back("10"_STRID));

    identifier.clear();
    BOOST_CHECK(identifier.empty());
  }

  BOOST_AUTO_TEST_CASE(push_back) {
    TNameIdentifier identifier;

    identifier.push_back("5"_STRID);
    BOOST_CHECK_EQUAL(1, identifier.size());
    BOOST_CHECK_EQUAL("5"_STRID, identifier[0]);
    BOOST_CHECK_EQUAL("5"_STRID, identifier.back());
    BOOST_CHECK_EQUAL("5"_STRID, identifier.front());

    identifier.push_back("10"_STRID);
    BOOST_CHECK_EQUAL(2, identifier.size());
    BOOST_CHECK_EQUAL("5"_STRID, identifier[0]);
    BOOST_CHECK_EQUAL("10"_STRID, identifier[1]);
    BOOST_CHECK_EQUAL("10"_STRID, identifier.back());
    BOOST_CHECK_EQUAL("5"_STRID, identifier.front());

    identifier.push_back("215"_STRID);
    BOOST_CHECK_EQUAL(3, identifier.size());
    BOOST_CHECK_EQUAL("5"_STRID, identifier[0]);
    BOOST_CHECK_EQUAL("10"_STRID, identifier[1]);
    BOOST_CHECK_EQUAL("215"_STRID, identifier[2]);
    BOOST_CHECK_EQUAL("215"_STRID, identifier.back());
    BOOST_CHECK_EQUAL("5"_STRID, identifier.front());

    identifier.push_back("3564"_STRID);
    BOOST_CHECK_EQUAL(4, identifier.size());
    BOOST_CHECK_EQUAL("5"_STRID, identifier[0]);
    BOOST_CHECK_EQUAL("10"_STRID, identifier[1]);
    BOOST_CHECK_EQUAL("215"_STRID, identifier[2]);
    BOOST_CHECK_EQUAL("3564"_STRID, identifier[3]);
    BOOST_CHECK_EQUAL("3564"_STRID, identifier.back());
    BOOST_CHECK_EQUAL("5"_STRID, identifier.front());
  }

  BOOST_AUTO_TEST_CASE(pop_back) {
    TNameIdentifier identifier;
    // fill with testdata
    for (size_t i = 0; i < cgMaxSupportedNameHierarchy; i++) {
      identifier.push_back(StringId::insert(std::to_string(i)));
    }

    for (size_t i = 0; i < cgMaxSupportedNameHierarchy; i++) {
      BOOST_TEST(std::to_string(cgMaxSupportedNameHierarchy - 1 - i) == identifier.back().get());
      BOOST_TEST(cgMaxSupportedNameHierarchy - i == identifier.size());
      identifier.pop_back();
      BOOST_TEST(cgMaxSupportedNameHierarchy - 1 - i == identifier.size());
    }
    BOOST_CHECK(identifier.empty());
  }

  BOOST_AUTO_TEST_CASE(iterator) {
    TNameIdentifier identifier;
    // fill with testdata
    for (size_t i = 0; i < cgMaxSupportedNameHierarchy; i++) {
      identifier.push_back(StringId::insert(std::to_string(i)));
    }

    int i = 0;
    for (const auto &it : identifier) {
      BOOST_CHECK_EQUAL(StringId::insert(std::to_string(i++)), it);
    }
    BOOST_CHECK_EQUAL(cgMaxSupportedNameHierarchy,
                      i); // we should have exactly visited each element once but started at zero
  }

  BOOST_AUTO_TEST_SUITE_END()

} // namespace forte::test
