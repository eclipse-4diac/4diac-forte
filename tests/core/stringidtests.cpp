/*******************************************************************************
 * Copyright (c) 2025 Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Martin Erich Jobst - initial implementation
 *******************************************************************************/

#include "forte/stringid.h"

#include "forte_boost_output_support.h"

#include <boost/test/unit_test.hpp>

using namespace forte::literals;

namespace forte::test {

  BOOST_AUTO_TEST_SUITE(StringIdTests)

  BOOST_AUTO_TEST_CASE(invalid) {
    StringId invalid{};
    BOOST_TEST(invalid == StringId());
    BOOST_TEST(invalid == false);
    BOOST_TEST(invalid.size() == 0);
    BOOST_TEST(invalid.empty() == true);
    BOOST_TEST(invalid.data() == nullptr);
  }

  BOOST_AUTO_TEST_CASE(literal) {
    auto first = "4diac"_STRID;
    auto second = "4diac"_STRID;
    BOOST_TEST(first == second);
    BOOST_TEST(first.size() == 5);
    BOOST_TEST(first.empty() == false);
    BOOST_TEST(first.data() == "4diac");
  }

  BOOST_AUTO_TEST_CASE(emptyLiteral) {
    auto first = ""_STRID;
    auto second = ""_STRID;
    BOOST_TEST(first == second);
    BOOST_TEST(first.size() == 0);
    BOOST_TEST(first.empty() == true);
    BOOST_TEST(first.data() == "");
  }

  BOOST_AUTO_TEST_CASE(insert) {
    auto literal = "4diac"_STRID;
    StringId first = StringId::insert("4diac");
    StringId second = StringId::insert("4diac");
    BOOST_TEST(first == second);
    BOOST_TEST(first == literal);
    BOOST_TEST(first.data() == second.data());
    BOOST_TEST(first.data() == literal.data());
  }

  BOOST_AUTO_TEST_CASE(lookup) {
    auto literal = "4diac"_STRID;
    StringId first = StringId::lookup("4diac");
    StringId second = StringId::lookup("4diac");
    StringId invalid = StringId::lookup("stringidtest invalid string");
    BOOST_TEST(first == second);
    BOOST_TEST(first == literal);
    BOOST_TEST(first.data() == second.data());
    BOOST_TEST(first.data() == literal.data());
    BOOST_TEST(invalid == StringId());
    BOOST_TEST(invalid == false);
  }

  BOOST_AUTO_TEST_CASE(compare) {
    BOOST_TEST("4diac"_STRID == "4diac"_STRID);
    BOOST_TEST("4diac!"_STRID != "4diac"_STRID);
    BOOST_TEST((("4diac"_STRID <=> "4diac"_STRID) == 0));
    BOOST_TEST((("4diac"_STRID <=> "4diac!"_STRID) != 0));
  }

  BOOST_AUTO_TEST_SUITE_END()

} // namespace forte::test
