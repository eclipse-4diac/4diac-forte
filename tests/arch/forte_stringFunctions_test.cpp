/*******************************************************************************
 * Copyright (c) 2024 Samator Indo Gas
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Ketut Kumajaya - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include <boost/test/unit_test.hpp>

#include "../../../src/arch/forte_stringFunctions.h"

BOOST_AUTO_TEST_SUITE (forte_stringFunctions_test)

  BOOST_AUTO_TEST_CASE(wstringToString)
  {
    // "4diac 4 ever!" in Japanese
#if defined(_MSC_VER) || defined(__MINGW32__)
    const std::string testString = "4diac 4 \xe3\x81\xaf\xe4\xbb\x8a\xe3\x81\xbe\xe3\x81\xa7\xef\xbc\x81"; // u8"4diac 4 は今まで！"
    const std::wstring resultString = L"4diac 4 \xe3\x81\xaf\xe4\xbb\x8a\xe3\x81\xbe\xe3\x81\xa7\xef\xbc\x81"; // L"4diac 4 は今まで！"
#else
    // Set the new and save the previous locale
    // BUG: trigger std::runtime_error: locale::facet::_S_create_c_locale name not valid with musl
    std::locale prev_loc = std::locale::global(std::locale("C.UTF-8"));
    const std::string testString = u8"4diac 4 は今まで！"; // make sure to use UTF-8 text editor with Unicode support
    const std::wstring resultString = L"4diac 4 は今まで！";
#endif

    BOOST_TEST(strlen(testString.c_str()) == strlen(forte_wstringToString(resultString).c_str()));
    BOOST_TEST(testString.c_str() == forte_wstringToString(resultString).c_str());
#if !(defined(_MSC_VER) || defined(__MINGW32__))
    // Restore the previous locale
    std::locale::global(prev_loc);
#endif
  }

  BOOST_AUTO_TEST_CASE(stringToWstring)
  {
    // "4diac 4 ever!" in Chinese
#if defined(_MSC_VER) || defined(__MINGW32__)
    const std::wstring testString = L"4diac 4 \xe6\xb0\xb8\xe8\xbf\x9c\xef\xbc\x81"; // L"4diac 4 永远！"
    const std::string resultString = "4diac 4 \xe6\xb0\xb8\xe8\xbf\x9c\xef\xbc\x81"; // u8"4diac 4 永远！"
#else
    // Set the new and save the previous locale
    // BUG: trigger std::runtime_error: locale::facet::_S_create_c_locale name not valid with musl
    std::locale prev_loc = std::locale::global(std::locale("C.UTF-8"));
    const std::wstring testString = L"4diac 4 永远！"; // make sure to use UTF-8 text editor with Unicode support
    const std::string resultString = u8"4diac 4 永远！";
#endif

    BOOST_TEST(wcslen(testString.c_str()) == wcslen(forte_stringToWstring(resultString).c_str()));
    BOOST_TEST(testString.c_str() == forte_stringToWstring(resultString).c_str());
#if !(defined(_MSC_VER) || defined(__MINGW32__))
    // Restore the previous locale
    std::locale::global(prev_loc);
#endif
  }

BOOST_AUTO_TEST_SUITE_END()
