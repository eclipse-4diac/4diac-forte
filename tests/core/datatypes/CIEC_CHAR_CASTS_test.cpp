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

#include "../../../src/core/datatypes/forte_char.h"
#include "../../../src/core/datatypes/forte_wchar.h"
#include "../../../src/core/datatypes/forte_usint.h"
#include "../../../src/core/datatypes/forte_byte.h"
#include "../../../src/core/datatypes/forte_word.h"
#include "../../../src/core/datatypes/forte_dword.h"
#include "../../../src/core/datatypes/forte_lword.h"
#include "../../../src/core/datatypes/forte_string.h"

BOOST_AUTO_TEST_SUITE(CIEC_CHAR_CASTS_test)

BOOST_AUTO_TEST_CASE(ToUsint_test) {
  CIEC_CHAR source('a');
  CIEC_USINT result;
  result = func_CHAR_TO_USINT(source);
  BOOST_CHECK_EQUAL(static_cast<CIEC_USINT::TValueType>(result), 97);
}

BOOST_AUTO_TEST_CASE(ToByte_test) {
  CIEC_CHAR source('a');
  CIEC_BYTE result;
  result = func_CHAR_TO_BYTE(source);
  BOOST_CHECK_EQUAL(static_cast<CIEC_BYTE::TValueType>(result), 97);
}

BOOST_AUTO_TEST_CASE(ToWord_test) {
  CIEC_CHAR source('a');
  CIEC_WORD result;
  result = func_CHAR_TO_WORD(source);
  BOOST_CHECK_EQUAL(static_cast<CIEC_WORD::TValueType>(result), 97);
}

BOOST_AUTO_TEST_CASE(ToDWord_test) {
  CIEC_CHAR source('a');
  CIEC_DWORD result;
  result = func_CHAR_TO_DWORD(source);
  BOOST_CHECK_EQUAL(static_cast<CIEC_DWORD::TValueType>(result), 97);
}

BOOST_AUTO_TEST_CASE(ToLWord_test) {
  CIEC_CHAR source('a');
  CIEC_LWORD result;
  result = func_CHAR_TO_LWORD(source);
  BOOST_CHECK_EQUAL(static_cast<CIEC_LWORD::TValueType>(result), 97);
}

BOOST_AUTO_TEST_CASE(ToWChar_test) {
  CIEC_CHAR source('a');
  CIEC_WCHAR result;
  result = func_CHAR_TO_WCHAR(source);
  BOOST_CHECK_EQUAL(static_cast<CIEC_WCHAR::TValueType>(result), 97);
}

BOOST_AUTO_TEST_CASE(ToString_test) {
  CIEC_CHAR source('a');
  CIEC_STRING result;
  CIEC_STRING expected("a"_STRING);
  result = func_CHAR_TO_STRING(source);
  BOOST_CHECK_EQUAL(result, expected);
}

BOOST_AUTO_TEST_SUITE_END()
