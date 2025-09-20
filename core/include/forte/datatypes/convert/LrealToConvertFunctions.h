/*******************************************************************************
 * Copyright (c) 2010 - 2015 TU Vienna/ACIN, Profactor GmbH, fortiss GmbH,
 *    2018-2019 TU Vienna/ACIN
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Alois Zoitl, Ingo Hegny, Monika Wenger, Carolyn Oates, Patrick Smejkal,
 *    Matthias Plasch,
 *      - initial implementation and rework communication infrastructure
 *    Martin Melik-Merkumians - fixes DT_TO_TOD
 *    Martin Melik-Merkumians - removes invalid casts, update implementation
 *     to use new cast function
 *******************************************************************************/

#pragma once

#include "forte/datatypes/forte_any.h"
#include "forte/datatypes/forte_dint.h"
#include "forte/datatypes/forte_int.h"
#include "forte/datatypes/forte_lint.h"
#include "forte/datatypes/forte_lreal.h"
#include "forte/datatypes/forte_lword.h"
#include "forte/datatypes/forte_real.h"
#include "forte/datatypes/forte_sint.h"
#include "forte/datatypes/forte_udint.h"
#include "forte/datatypes/forte_uint.h"
#include "forte/datatypes/forte_ulint.h"
#include "forte/datatypes/forte_usint.h"
#include "forte/datatypes/forte_string.h"
#include "forte/datatypes/forte_wstring.h"

#include "convert_function_utils.h"

namespace forte {
  //********************************************************************************************
  //   LREAL_TO_*  functions
  //********************************************************************************************
  inline CIEC_LWORD func_LREAL_TO_LWORD(const CIEC_LREAL &paVal) {
    return CIEC_ANY::cast<CIEC_LWORD>(paVal);
  }

  inline CIEC_REAL func_LREAL_TO_REAL(const CIEC_LREAL &paVal) {
    return CIEC_ANY::cast<CIEC_REAL>(paVal);
  }

  inline CIEC_USINT func_LREAL_TO_USINT(const CIEC_LREAL &paVal) {
    return CIEC_ANY::cast<CIEC_USINT>(paVal);
  }

  inline CIEC_UDINT func_LREAL_TO_UDINT(const CIEC_LREAL &paVal) {
    return CIEC_ANY::cast<CIEC_UDINT>(paVal);
  }

  inline CIEC_UINT func_LREAL_TO_UINT(const CIEC_LREAL &paVal) {
    return CIEC_ANY::cast<CIEC_UINT>(paVal);
  }

  inline CIEC_ULINT func_LREAL_TO_ULINT(const CIEC_LREAL &paVal) {
    return CIEC_ANY::cast<CIEC_ULINT>(paVal);
  }

  inline CIEC_LINT func_LREAL_TO_LINT(const CIEC_LREAL &paVal) {
    return CIEC_ANY::cast<CIEC_LINT>(paVal);
  }

  inline CIEC_DINT func_LREAL_TO_DINT(const CIEC_LREAL &paVal) {
    return CIEC_ANY::cast<CIEC_DINT>(paVal);
  }

  inline CIEC_SINT func_LREAL_TO_SINT(const CIEC_LREAL &paVal) {
    return CIEC_ANY::cast<CIEC_SINT>(paVal);
  }

  inline CIEC_INT func_LREAL_TO_INT(const CIEC_LREAL &paVal) {
    return CIEC_ANY::cast<CIEC_INT>(paVal);
  }

  inline CIEC_STRING func_LREAL_AS_STRING(const CIEC_LREAL &paVal) {
    CIEC_STRING string;
    stringConverter(string, paVal);
    return string;
  }

  inline CIEC_WSTRING func_LREAL_AS_WSTRING(const CIEC_LREAL &paVal) {
    CIEC_WSTRING string;
    stringConverter(string, paVal);
    return string;
  }
} // namespace forte
