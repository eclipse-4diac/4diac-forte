/*******************************************************************************
 * Copyright (c) 2010, 2025 TU Vienna/ACIN, Profactor GmbH, fortiss GmbH,
 *                          HIT robot group, Monika Wenger,
 *                          Primetals Technologies Austria GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Ingo Hegny, Monika Wenger, Carolyn Oates, Patrick Smejkal,
 *     Matthias Plasch
 *      - initial implementation and rework communication infrastructure
 *   Martin Melik-Merkumians - fixes DT_TO_TOD
 *   Martin Melik-Merkumians - removes invalid casts, update implementation
 *     to use new cast function
 *   Zhao Xin -fixes string conversion
 *   Monika Wenger - func_ANY_AS_STRING
 *   Alois Zoitl  - migrated data type toString to std::string
 *******************************************************************************/

#pragma once

#include "convert_function_utils.h"

#include "forte/datatypes/forte_any.h"
#include "forte/datatypes/forte_string.h"

#include "TimeOfDayToConvertFunctions.h"
#include "DateAndTimeToConvertFunctions.h"
#include "LDateAndTimeToConvertFunctions.h"
#include "CharToConvertFunctions.h"
#include "WcharToConvertFunctions.h"
#include "WstringToConvertFunctions.h"
#include "StringToConvertFunctions.h"
#include "LrealToConvertFunctions.h"
#include "RealToConvertFunctions.h"
#include "TimeToConvertFunctions.h"
#include "BoolToConvertFunctions.h"
#include "ByteToConvertFunctions.h"
#include "DintToConvertFunctions.h"
#include "DwordToConvertFunctions.h"
#include "LwordToConvertFunctions.h"
#include "UdintToConvertFunctions.h"
#include "UintToConvertFunctions.h"
#include "UlintToConvertFunctions.h"
#include "UsintToConvertFunctions.h"
#include "WordToConvertFunctions.h"
#include "IntToConvertFunctions.h"
#include "LintToConvertFunctions.h"
#include "SintToConvertFunctions.h"
#include "BcdConvertFunctions.h"
#include "TimeInResolutionConvertFunctions.h"
#include "overloadedConvertFunctions.h"

namespace forte {
  /*! \file
   * This file implements the type conversion functions as defined by IEC 61131-3 in subclause 2.5.1.5.1
   */

  inline CIEC_STRING func_ANY_AS_STRING(const CIEC_ANY &paVal) {
    CIEC_STRING string;
    stringConverter(string, paVal.unwrap());
    return string;
  }
} // namespace forte
