/*******************************************************************************
 * Copyright (c) 2022 Davor Cihlar
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Davor Cihlar  - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include <boost/test/unit_test.hpp>
#include "../../../src/core/util/mixedStorage.h"

#if BOOST_VERSION < 106400
    BOOST_TEST_DONT_PRINT_LOG_VALUE(std::nullptr_t)
#endif

using namespace forte::core::util;


BOOST_AUTO_TEST_SUITE(MixedStorage_Test)
  static constexpr const char blob[] = "hello";
  static constexpr const char* blob_end = blob + sizeof(blob);

  BOOST_AUTO_TEST_CASE(MixedStorage_ReserveDoesAnything) {
    CMixedStorage uut;
    BOOST_CHECK_EQUAL(uut.data(), nullptr);
    uut.reserve(16);
    BOOST_CHECK_NE(uut.data(), nullptr);
  }

  BOOST_AUTO_TEST_CASE(MixedStorage_InitiallyEmpty) {
    CMixedStorage uut;
    BOOST_CHECK_EQUAL(uut.size(), 0);
  }

  BOOST_AUTO_TEST_CASE(MixedStorage_WriteChar) {
    CMixedStorage uut;
    auto ptr = uut.write('c');
    BOOST_CHECK_EQUAL(uut.size(), 1);
    BOOST_CHECK_EQUAL(ptr, uut.data());
    BOOST_CHECK_EQUAL(*(char*)uut.data(), 'c');
  }

  BOOST_AUTO_TEST_CASE(MixedStorage_WriteInt) {
    CMixedStorage uut;
    auto ptr = uut.write((int)42);
    BOOST_CHECK_EQUAL(uut.size(), sizeof(int));
    BOOST_CHECK_EQUAL(ptr, uut.data());
    BOOST_CHECK_EQUAL(*(int*)uut.data(), 42);
  }

  void check_blob_in_uut(CMixedStorage &uut) {
    BOOST_CHECK_EQUAL_COLLECTIONS(reinterpret_cast<char*>(uut.data()), reinterpret_cast<char*>(uut.end()), blob, blob_end);
  }

  BOOST_AUTO_TEST_CASE(MixedStorage_WriteBeginEnd) {
    CMixedStorage uut;
    auto ptr = uut.write(blob, blob_end);
    BOOST_CHECK(uut.size() == sizeof(blob));
    BOOST_CHECK_EQUAL(ptr, uut.data());
    check_blob_in_uut(uut);
  }

  BOOST_AUTO_TEST_CASE(MixedStorage_WriteBeginSize) {
    CMixedStorage uut;
    auto ptr = uut.write(blob, sizeof(blob));
    BOOST_CHECK(uut.size() == sizeof(blob));
    BOOST_CHECK_EQUAL(ptr, uut.data());
    check_blob_in_uut(uut);
  }

  BOOST_AUTO_TEST_CASE(MixedStorage_ValidEnd) {
    CMixedStorage uut;
    uut.write('c');
    //Note: don't use BOOST_CHECK_EQUAL because it will try to compare C strings
    BOOST_CHECK(uut.end<char>() == reinterpret_cast<char*>(uut.data()) + 1);
  }

  BOOST_AUTO_TEST_CASE(MixedStorage_WriteMultiple) {
    CMixedStorage uut;
    uut.write('c');
    auto ptr = uut.write('+');
    //Note: don't use BOOST_CHECK_EQUAL because it will try to compare C strings
    BOOST_CHECK(reinterpret_cast<char*>(uut.data()) + 1 == ptr);
  }

  BOOST_AUTO_TEST_SUITE_END()
