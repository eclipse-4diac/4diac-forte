/*******************************************************************************
 * Copyright (c) 2010 - 2015 TU Vienna/ACIN, Profactor GmbH, fortiss GmbH,
 *    2018-2019 TU Vienna/ACIN
 *   2021 HIT robot group
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
 *    Zhao Xin -fixes string conversion 
 *******************************************************************************/
#ifndef SRC_CORE_DATATYPES_CONVERT_CONVERT_FUNCTIONS_H_
#define SRC_CORE_DATATYPES_CONVERT_CONVERT_FUNCTIONS_H_

#include "forte_any.h"
#include "forte_bool.h"
#include "forte_byte.h"
#include "forte_dint.h"
#include "forte_dword.h"
#include "forte_int.h"
#include "forte_lint.h"
#include "forte_lreal.h"
#include "forte_lword.h"
#include "forte_real.h"
#include "forte_sint.h"
#include "forte_udint.h"
#include "forte_uint.h"
#include "forte_ulint.h"
#include "forte_usint.h"
#include "forte_word.h"
#include "forte_string.h"
#include "forte_wstring.h"
#include "forte_time.h"
#include "forte_time_of_day.h"
#include "forte_date_and_time.h"
#include "forte_date.h"
#include "forte_struct.h"
#include "forte_array.h"
#include <math.h>

//some compilers don't have this definition or is behind __STDC_CONSTANT_MACROS.
//since this definition is only used here, we define it instead of enabling __STDC_CONSTANT_MACROS globally,
//because it's not easy to detect which compilers will or not have this definition
#ifndef UINT64_C
# define UINT64_C(x)   (x##ULL)
#endif

/*! \file
 * This file implements the type conversion functions as defined by IEC 61131-3 in subclause 2.5.1.5.1
 */

inline void stringConverter(CIEC_ANY_STRING &paString, const CIEC_ANY &paVal) {
    size_t bufferSize = paVal.getToStringBufferSize();

    paString.reserve(static_cast<TForteUInt16>(bufferSize));
    char *pacBuffer = paString.getValue();
    int nWrittenBytes = paVal.toString(pacBuffer, bufferSize);
    nWrittenBytes = nWrittenBytes > -1 ? nWrittenBytes : 0;
    paString.assign(pacBuffer, static_cast<TForteUInt16>(nWrittenBytes));
}

#include "DateAndTimeToConvertFunctions.h"
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

#endif /* SRC_CORE_DATATYPES_CONVERT_CONVERT_FUNCTIONS_H_ */
