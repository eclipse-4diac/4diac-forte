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

#include "forte/datatypes/forte_bool.h"
#include "forte/datatypes/forte_byte.h"
#include "forte/datatypes/forte_dint.h"
#include "forte/datatypes/forte_dword.h"
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
#include "forte/datatypes/forte_word.h"
#include "forte/datatypes/forte_string.h"
#include "forte/datatypes/forte_wstring.h"
#include "forte/datatypes/forte_time.h"
#include "forte/datatypes/forte_array.h"

namespace forte {
  //********************************************************************************************
  //   WSTRING_TO_*  functions
  //********************************************************************************************
  inline CIEC_BOOL func_WSTRING_AS_BOOL(const CIEC_WSTRING &paVal) {
    CIEC_BOOL tempVal;
    const char *pacBuffer = paVal.getValue();
    tempVal.fromString(pacBuffer);
    return tempVal;
  }

  inline CIEC_BYTE func_WSTRING_AS_BYTE(const CIEC_WSTRING &paVal) {
    CIEC_BYTE tempVal;
    const char *pacBuffer = paVal.getValue();
    tempVal.fromString(pacBuffer);
    return tempVal;
  }

  inline CIEC_DWORD func_WSTRING_AS_DWORD(const CIEC_WSTRING &paVal) {
    CIEC_DWORD tempVal;
    const char *pacBuffer = paVal.getValue();
    tempVal.fromString(pacBuffer);
    return tempVal;
  }

  inline CIEC_WORD func_WSTRING_AS_WORD(const CIEC_WSTRING &paVal) {
    CIEC_WORD tempVal;
    const char *pacBuffer = paVal.getValue();
    tempVal.fromString(pacBuffer);
    return tempVal;
  }

  inline CIEC_DINT func_WSTRING_AS_DINT(const CIEC_WSTRING &paVal) {
    CIEC_DINT tempVal;
    const char *pacBuffer = paVal.getValue();
    tempVal.fromString(pacBuffer);
    return tempVal;
  }

  inline CIEC_INT func_WSTRING_AS_INT(const CIEC_WSTRING &paVal) {
    CIEC_INT tempVal;
    const char *pacBuffer = paVal.getValue();
    tempVal.fromString(pacBuffer);
    return tempVal;
  }

  inline CIEC_UINT func_WSTRING_AS_UINT(const CIEC_WSTRING &paVal) {
    CIEC_UINT tempVal;
    const char *pacBuffer = paVal.getValue();
    tempVal.fromString(pacBuffer);
    return tempVal;
  }

  inline CIEC_UDINT func_WSTRING_AS_UDINT(const CIEC_WSTRING &paVal) {
    CIEC_UDINT tempVal;
    const char *pacBuffer = paVal.getValue();
    tempVal.fromString(pacBuffer);
    return tempVal;
  }

  inline CIEC_USINT func_WSTRING_AS_USINT(const CIEC_WSTRING &paVal) {
    CIEC_USINT tempVal;
    const char *pacBuffer = paVal.getValue();
    tempVal.fromString(pacBuffer);
    return tempVal;
  }

  inline CIEC_STRING func_WSTRING_TO_STRING(const CIEC_WSTRING &paVal) {
    const char *pacBuffer = paVal.getValue();
    CIEC_STRING tempVal(pacBuffer, strlen(pacBuffer)); // TODO: implementation wrong, needs to be fixed with WSTRING
    return tempVal;
  }

  inline CIEC_TIME func_WSTRING_AS_TIME(const CIEC_WSTRING &paVal) {
    CIEC_TIME tempVal;
    const char *pacBuffer = paVal.getValue();
    tempVal.fromString(pacBuffer);
    return tempVal;
  }

  inline CIEC_SINT func_WSTRING_AS_SINT(const CIEC_WSTRING &paVal) {
    CIEC_SINT tempVal;
    const char *pacBuffer = paVal.getValue();
    tempVal.fromString(pacBuffer);
    return tempVal;
  }

  inline CIEC_REAL func_WSTRING_AS_REAL(const CIEC_WSTRING &paVal) {
    CIEC_REAL tempVal;
    const char *pacBuffer = paVal.getValue();
    tempVal.fromString(pacBuffer);
    return tempVal;
  }

  inline CIEC_LREAL func_WSTRING_AS_LREAL(const CIEC_WSTRING &paVal) {
    CIEC_LREAL tempVal;
    const char *pacBuffer = paVal.getValue();
    tempVal.fromString(pacBuffer);
    return tempVal;
  }

  inline CIEC_LINT func_WSTRING_AS_LINT(const CIEC_WSTRING &paVal) {
    CIEC_LINT tempVal;
    const char *pacBuffer = paVal.getValue();
    tempVal.fromString(pacBuffer);
    return tempVal;
  }

  inline CIEC_LWORD func_WSTRING_AS_LWORD(const CIEC_WSTRING &paVal) {
    CIEC_LWORD tempVal;
    const char *pacBuffer = paVal.getValue();
    tempVal.fromString(pacBuffer);
    return tempVal;
  }

  inline CIEC_ULINT func_WSTRING_AS_ULINT(const CIEC_WSTRING &paVal) {
    CIEC_ULINT tempVal;
    const char *pacBuffer = paVal.getValue();
    tempVal.fromString(pacBuffer);
    return tempVal;
  }
} // namespace forte
