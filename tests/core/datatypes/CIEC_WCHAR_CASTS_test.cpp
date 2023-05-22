/*******************************************************************************
 * Copyright (c) 2022 Primetals Technologies Austria GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Martin Melik Merkumians - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include <boost/test/unit_test.hpp>
#include "forte_boost_output_support.h"

#include "iec61131_functions.h"

#include "../../../src/core/datatypes/forte_wchar.h"
#include "../../../src/core/datatypes/forte_char.h"
#include "../../../src/core/datatypes/forte_word.h"
#include "../../../src/core/datatypes/forte_dword.h"
#include "../../../src/core/datatypes/forte_lword.h"
#include "../../../src/core/datatypes/forte_wstring.h"

BOOST_AUTO_TEST_SUITE(CIEC_WCHAR_CASTS_test)

BOOST_AUTO_TEST_CASE(ToChar_test) {
  CIEC_WCHAR source('a');
  CIEC_CHAR result;
  result = func_WCHAR_TO_CHAR(source);
  BOOST_CHECK_EQUAL(static_cast<CIEC_CHAR::TValueType>(result), 97);
}

BOOST_AUTO_TEST_CASE(ToWord_test) {
  CIEC_WCHAR source('a');
  CIEC_WORD result;
  result = func_WCHAR_TO_WORD(source);
  BOOST_CHECK_EQUAL(result, 97);
}

BOOST_AUTO_TEST_CASE(ToDWord_test) {
  CIEC_WCHAR source('a');
  CIEC_DWORD result;
  result = func_WCHAR_TO_DWORD(source);
  BOOST_CHECK_EQUAL(result, 97);
}

BOOST_AUTO_TEST_CASE(ToLWord_test) {
  CIEC_WCHAR source('a');
  CIEC_LWORD result;
  result = func_WCHAR_TO_LWORD(source);
  BOOST_CHECK_EQUAL(result, 97);
}

BOOST_AUTO_TEST_CASE(ToWString_test) {
  CIEC_WCHAR source('a');
  CIEC_WSTRING result;
  CIEC_WSTRING expected("a");
  result = func_WCHAR_TO_WSTRING(source);
  BOOST_CHECK_EQUAL(result, expected);
}

BOOST_AUTO_TEST_SUITE_END()
