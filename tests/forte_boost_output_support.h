/*******************************************************************************
 * Copyright (c) 2022, 2025 Primetals Technologies Austria GmbH
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
 *   Alois Zoitl  - migrated data type toString to std::string
 *******************************************************************************/

#pragma once

#include <iostream>
#include "forte/datatypes/forte_lreal.h"
#include "forte/datatypes/forte_char.h"
#include "forte/datatypes/forte_wchar.h"
#include "forte/datatypes/forte_string.h"
#include "forte/datatypes/forte_string_fixed.h"
#include "forte/datatypes/forte_wstring.h"
#include "forte/datatypes/forte_time.h"
#include "forte/datatypes/forte_time_of_day.h"
#include "forte/datatypes/forte_date.h"
#include "forte/datatypes/forte_date_and_time.h"
#include "forte/datatypes/forte_ltime.h"
#include "forte/datatypes/forte_ltime_of_day.h"
#include "forte/datatypes/forte_ldate.h"
#include "forte/datatypes/forte_ldate_and_time.h"
#include "forte/funcbloc.h"
#include "forte/mgmcmd.h"
#include "forte/stringid.h"

namespace forte {
  inline std::ostream &boost_test_print_type(std::ostream &out, const StringId &variable) {
    out << static_cast<std::string_view>(variable);
    return out;
  }

  inline std::ostream &boost_test_print_type(std::ostream &out, const CIEC_LREAL &variable) {
    std::string outputString;
    variable.toString(outputString);
    out << outputString.data();
    return out;
  }

  inline std::ostream &boost_test_print_type(std::ostream &out, const CIEC_TIME &variable) {
    std::string outputString;
    variable.toString(outputString);
    out << outputString.data();
    return out;
  }

  inline std::ostream &boost_test_print_type(std::ostream &out, const CIEC_TIME_OF_DAY &variable) {
    std::string outputString;
    variable.toString(outputString);
    out << outputString.data();
    return out;
  }

  inline std::ostream &boost_test_print_type(std::ostream &out, const CIEC_DATE &variable) {
    std::string outputString;
    variable.toString(outputString);
    out << outputString.data();
    return out;
  }

  inline std::ostream &boost_test_print_type(std::ostream &out, const CIEC_DATE_AND_TIME &variable) {
    std::string outputString;
    variable.toString(outputString);
    out << outputString.data();
    return out;
  }

  inline std::ostream &boost_test_print_type(std::ostream &out, const CIEC_LTIME &variable) {
    std::string outputString;
    variable.toString(outputString);
    out << outputString.data();
    return out;
  }

  inline std::ostream &boost_test_print_type(std::ostream &out, const CIEC_LTIME_OF_DAY &variable) {
    std::string outputString;
    variable.toString(outputString);
    out << outputString.data();
    return out;
  }

  inline std::ostream &boost_test_print_type(std::ostream &out, const CIEC_LDATE &variable) {
    std::string outputString;
    variable.toString(outputString);
    out << outputString.data();
    return out;
  }

  inline std::ostream &boost_test_print_type(std::ostream &out, const CIEC_LDATE_AND_TIME &variable) {
    std::string outputString;
    variable.toString(outputString);
    out << outputString.data();
    return out;
  }

  inline std::ostream &boost_test_print_type(std::ostream &out, const CIEC_CHAR &variable) {
    std::string outputString;
    variable.toString(outputString);
    out << outputString.data();
    return out;
  }

  inline std::ostream &boost_test_print_type(std::ostream &out, const CIEC_WCHAR &variable) {
    std::string outputString;
    variable.toString(outputString);
    out << outputString.data();
    return out;
  }

  inline std::ostream &boost_test_print_type(std::ostream &out, const CIEC_STRING &variable) {
    out << variable.getStorage();
    return out;
  }

  template<size_t maxLength>
  inline std::ostream &boost_test_print_type(std::ostream &out, const CIEC_STRING_FIXED<maxLength> &variable) {
    out << variable.getStorage();
    return out;
  }

  inline std::ostream &boost_test_print_type(std::ostream &out, const CIEC_WSTRING &variable) {
    std::string outputString;
    variable.toString(outputString);
    out << outputString.data();
    return out;
  }

  inline std::ostream &boost_test_print_type(std::ostream &out, const EMGMResponse &variable) {
    out << static_cast<std::underlying_type_t<EMGMResponse>>(variable);
    return out;
  }

  inline std::ostream &boost_test_print_type(std::ostream &out, const CFunctionBlock::E_FBStates &variable) {
    out << static_cast<std::underlying_type_t<CFunctionBlock::E_FBStates>>(variable);
    return out;
  }
} // namespace forte
