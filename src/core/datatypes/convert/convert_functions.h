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
#ifndef SRC_CORE_DATATYPES_CONVERT_CONVERT_FUNCTIONS_H_
#define SRC_CORE_DATATYPES_CONVERT_CONVERT_FUNCTIONS_H_

#include "core/datatypes/forte_any.h"
#include "core/datatypes/forte_bool.h"
#include "core/datatypes/forte_byte.h"
#include "core/datatypes/forte_dint.h"
#include "core/datatypes/forte_dword.h"
#include "core/datatypes/forte_int.h"
#include "core/datatypes/forte_lint.h"
#include "core/datatypes/forte_lreal.h"
#include "core/datatypes/forte_lword.h"
#include "core/datatypes/forte_real.h"
#include "core/datatypes/forte_sint.h"
#include "core/datatypes/forte_udint.h"
#include "core/datatypes/forte_uint.h"
#include "core/datatypes/forte_ulint.h"
#include "core/datatypes/forte_usint.h"
#include "core/datatypes/forte_word.h"
#include "core/datatypes/forte_string.h"
#include "core/datatypes/forte_wstring.h"
#include "core/datatypes/forte_time.h"
#include "core/datatypes/forte_time_of_day.h"
#include "core/datatypes/forte_date_and_time.h"
#include "core/datatypes/forte_date.h"
#include "core/datatypes/forte_ltime.h"
#include "core/datatypes/forte_ltime_of_day.h"
#include "core/datatypes/forte_ldate_and_time.h"
#include "core/datatypes/forte_ldate.h"
#include "core/datatypes/forte_struct.h"
#include "core/datatypes/forte_array.h"
#include <math.h>

// some compilers don't have this definition or is behind __STDC_CONSTANT_MACROS.
// since this definition is only used here, we define it instead of enabling __STDC_CONSTANT_MACROS globally,
// because it's not easy to detect which compilers will or not have this definition
#ifndef UINT64_C
#define UINT64_C(x) (x##ULL)
#endif

/*! \file
 * This file implements the type conversion functions as defined by IEC 61131-3 in subclause 2.5.1.5.1
 */

inline void stringConverter(CIEC_WSTRING &paString, const CIEC_ANY &paVal) {
  std::string buffer;
  paVal.toString(buffer);
  paString = CIEC_WSTRING(buffer.c_str());
}

inline void stringConverter(CIEC_STRING &paString, const CIEC_ANY &paVal) {
  std::string buffer;
  paVal.toString(buffer);
  paString = CIEC_STRING(std::move(buffer));
}

inline const CIEC_STRING func_ANY_AS_STRING(const CIEC_ANY &paVal) {
  CIEC_STRING string;
  stringConverter(string, paVal.unwrap());
  return string;
}

#include "core/datatypes/convert/TimeOfDayToConvertFunctions.h"
#include "core/datatypes/convert/DateAndTimeToConvertFunctions.h"
#include "core/datatypes/convert/LDateAndTimeToConvertFunctions.h"
#include "core/datatypes/convert/CharToConvertFunctions.h"
#include "core/datatypes/convert/WcharToConvertFunctions.h"
#include "core/datatypes/convert/WstringToConvertFunctions.h"
#include "core/datatypes/convert/StringToConvertFunctions.h"
#include "core/datatypes/convert/LrealToConvertFunctions.h"
#include "core/datatypes/convert/RealToConvertFunctions.h"
#include "core/datatypes/convert/TimeToConvertFunctions.h"
#include "core/datatypes/convert/BoolToConvertFunctions.h"
#include "core/datatypes/convert/ByteToConvertFunctions.h"
#include "core/datatypes/convert/DintToConvertFunctions.h"
#include "core/datatypes/convert/DwordToConvertFunctions.h"
#include "core/datatypes/convert/LwordToConvertFunctions.h"
#include "core/datatypes/convert/UdintToConvertFunctions.h"
#include "core/datatypes/convert/UintToConvertFunctions.h"
#include "core/datatypes/convert/UlintToConvertFunctions.h"
#include "core/datatypes/convert/UsintToConvertFunctions.h"
#include "core/datatypes/convert/WordToConvertFunctions.h"
#include "core/datatypes/convert/IntToConvertFunctions.h"
#include "core/datatypes/convert/LintToConvertFunctions.h"
#include "core/datatypes/convert/SintToConvertFunctions.h"
#include "core/datatypes/convert/BcdConvertFunctions.h"
#include "core/datatypes/convert/TimeInResolutionConvertFunctions.h"
#include "core/datatypes/convert/overloadedConvertFunctions.h"

#endif /* SRC_CORE_DATATYPES_CONVERT_CONVERT_FUNCTIONS_H_ */
