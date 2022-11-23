/*******************************************************************************
 * Copyright (c) 2022 Primetals Technologies Austria GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Martin Melik-Merkumians
 *      - initial implementation and rework communication infrastructure
 *******************************************************************************/

#pragma once

#ifndef _FORTE_BOOST_OUTPUT_SUPPORT_H_
#define _FORTE_BOOST_OUTPUT_SUPPORT_H_

#include <iostream>
#include "forte_char.h"
#include "forte_wchar.h"
#include "forte_string.h"
#include "forte_wstring.h"
#include "forte_time_of_day.h"

inline
std::ostream& boost_test_print_type(std::ostream &out, const CIEC_TIME &variable) {
  char outputString[variable.getToStringBufferSize()];
  variable.toString(outputString, sizeof(outputString));
  out << outputString;
  return out;
}

inline
std::ostream& boost_test_print_type(std::ostream &out, const CIEC_TIME_OF_DAY &variable) {
  char outputString[variable.getToStringBufferSize()];
  variable.toString(outputString, sizeof(outputString));
  out << outputString;
  return out;
}

inline
std::ostream& boost_test_print_type(std::ostream &out, const CIEC_CHAR &variable) {
  char outputString[variable.getToStringBufferSize()];
  variable.toString(outputString, sizeof(outputString));
  out << outputString;
  return out;
}

inline
std::ostream& boost_test_print_type(std::ostream &out, const CIEC_WCHAR &variable) {
  char outputString[variable.getToStringBufferSize()];
  variable.toString(outputString, sizeof(outputString));
  out << outputString;
  return out;
}

inline
std::ostream& boost_test_print_type(std::ostream &out, const CIEC_STRING &variable) {
  char outputString[variable.getToStringBufferSize()];
  variable.toString(outputString, sizeof(outputString));
  out << outputString;
  return out;
}

inline
std::ostream& boost_test_print_type(std::ostream &out, const CIEC_WSTRING &variable) {
  char outputString[variable.getToStringBufferSize()];
  variable.toString(outputString, sizeof(outputString));
  out << outputString;
  return out;
}

#endif