/*******************************************************************************
 * Copyright (c) 2011 - 2013, 2018 ACIN, fortiss GmbH, 2019 TU Wien/ACIN
 *               2022 Primetals Technologies Austria GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Alois Zoitl, Monika Wenger
 *      - initial implementation and rework communication infrastructure
 *    Martin Melik Merkumians - adds NOW and NOW_MONOTONIC functions,
 *        changes for removed implicit constructor for primitive types
 *******************************************************************************/

#include "forte/datatypes/forte_bool.h"
#include "forte/datatypes/forte_sint.h"
#include "forte/datatypes/forte_int.h"
#include "forte/datatypes/forte_dint.h"
#include "forte/datatypes/forte_lint.h"
#include "forte/datatypes/forte_usint.h"
#include "forte/datatypes/forte_uint.h"
#include "forte/datatypes/forte_udint.h"
#include "forte/datatypes/forte_ulint.h"
#include "forte/datatypes/forte_byte.h"
#include "forte/datatypes/forte_word.h"
#include "forte/datatypes/forte_dword.h"
#include "forte/datatypes/forte_lword.h"
#include "forte/datatypes/forte_date.h"
#include "forte/datatypes/forte_time_of_day.h"
#include "forte/datatypes/forte_date_and_time.h"
#include "forte/datatypes/forte_time.h"
#include "forte/datatypes/forte_char.h"
#include "forte/datatypes/forte_wchar.h"
#include "forte/datatypes/forte_ldate.h"
#include "forte/datatypes/forte_ltime_of_day.h"
#include "forte/datatypes/forte_ldate_and_time.h"
#include "forte/datatypes/forte_real.h"
#include "forte/datatypes/forte_lreal.h"
#include "forte/datatypes/forte_string.h"
#include "forte/datatypes/forte_wstring.h"
#include "forte/iec61131_functions/endianess.h"
#include "forte/datatypes/forte_struct.h"
#include "forte/datatypes/forte_array_common.h"
#include "forte/datatypes/forte_any.h"

namespace forte {
  void swapEndianessInplace(CIEC_ARRAY &paValue) {
    for (intmax_t i = paValue.getLowerBound(); i <= paValue.getUpperBound(); i++) {
      CIEC_ANY &element = paValue[i];
      switch (element.getDataTypeID()) {
        case CIEC_ANY::e_BOOL: element.setValue(swapEndianess(reinterpret_cast<const CIEC_BOOL &>(element))); break;
        case CIEC_ANY::e_SINT: element.setValue(swapEndianess(reinterpret_cast<const CIEC_SINT &>(element))); break;
        case CIEC_ANY::e_INT: element.setValue(swapEndianess(reinterpret_cast<const CIEC_INT &>(element))); break;
        case CIEC_ANY::e_DINT: element.setValue(swapEndianess(reinterpret_cast<const CIEC_DINT &>(element))); break;
        case CIEC_ANY::e_LINT: element.setValue(swapEndianess(reinterpret_cast<const CIEC_LINT &>(element))); break;
        case CIEC_ANY::e_USINT: element.setValue(swapEndianess(reinterpret_cast<const CIEC_USINT &>(element))); break;
        case CIEC_ANY::e_UINT: element.setValue(swapEndianess(reinterpret_cast<const CIEC_UINT &>(element))); break;
        case CIEC_ANY::e_UDINT: element.setValue(swapEndianess(reinterpret_cast<const CIEC_UDINT &>(element))); break;
        case CIEC_ANY::e_ULINT: element.setValue(swapEndianess(reinterpret_cast<const CIEC_ULINT &>(element))); break;
        case CIEC_ANY::e_BYTE: element.setValue(swapEndianess(reinterpret_cast<const CIEC_BYTE &>(element))); break;
        case CIEC_ANY::e_WORD: element.setValue(swapEndianess(reinterpret_cast<const CIEC_WORD &>(element))); break;
        case CIEC_ANY::e_DWORD: element.setValue(swapEndianess(reinterpret_cast<const CIEC_DWORD &>(element))); break;
        case CIEC_ANY::e_LWORD: element.setValue(swapEndianess(reinterpret_cast<const CIEC_LWORD &>(element))); break;
        case CIEC_ANY::e_DATE: element.setValue(swapEndianess(reinterpret_cast<const CIEC_DATE &>(element))); break;
        case CIEC_ANY::e_TIME_OF_DAY:
          element.setValue(swapEndianess(reinterpret_cast<const CIEC_TIME_OF_DAY &>(element)));
          break;
        case CIEC_ANY::e_DATE_AND_TIME:
          element.setValue(swapEndianess(reinterpret_cast<const CIEC_DATE_AND_TIME &>(element)));
          break;
        case CIEC_ANY::e_TIME: element.setValue(swapEndianess(reinterpret_cast<const CIEC_TIME &>(element))); break;
        case CIEC_ANY::e_CHAR: element.setValue(swapEndianess(reinterpret_cast<const CIEC_CHAR &>(element))); break;
        case CIEC_ANY::e_WCHAR: element.setValue(swapEndianess(reinterpret_cast<const CIEC_WCHAR &>(element))); break;
        case CIEC_ANY::e_LDATE: element.setValue(swapEndianess(reinterpret_cast<const CIEC_LDATE &>(element))); break;
        case CIEC_ANY::e_LTIME_OF_DAY:
          element.setValue(swapEndianess(reinterpret_cast<const CIEC_LTIME_OF_DAY &>(element)));
          break;
        case CIEC_ANY::e_LDATE_AND_TIME:
          element.setValue(swapEndianess(reinterpret_cast<const CIEC_LDATE_AND_TIME &>(element)));
          break;
        case CIEC_ANY::e_LTIME: element.setValue(swapEndianess(reinterpret_cast<const CIEC_LDATE &>(element))); break;
        case CIEC_ANY::e_REAL: element.setValue(swapEndianess(reinterpret_cast<const CIEC_REAL &>(element))); break;
        case CIEC_ANY::e_LREAL: element.setValue(swapEndianess(reinterpret_cast<const CIEC_LREAL &>(element))); break;
        case CIEC_ANY::e_STRING: element.setValue(swapEndianess(reinterpret_cast<const CIEC_STRING &>(element))); break;
        case CIEC_ANY::e_WSTRING:
          element.setValue(swapEndianess(reinterpret_cast<const CIEC_WSTRING &>(element)));
          break;
        case CIEC_ANY::e_ARRAY: swapEndianessInplace(reinterpret_cast<CIEC_ARRAY &>(element)); break;
        case CIEC_ANY::e_STRUCT: swapEndianessInplace(reinterpret_cast<CIEC_STRUCT &>(element)); break;
        default: break; // do nothing
      }
    }
  }

  void swapEndianessInplace(CIEC_STRUCT &paValue) {
    for (size_t i = 0; i < paValue.getStructSize(); ++i) {
      CIEC_ANY &member = *paValue.getMember(i);
      switch (member.getDataTypeID()) {
        case CIEC_ANY::e_BOOL: member.setValue(swapEndianess(static_cast<CIEC_BOOL &>(member))); break;
        case CIEC_ANY::e_SINT: member.setValue(swapEndianess(static_cast<CIEC_SINT &>(member))); break;
        case CIEC_ANY::e_INT: member.setValue(swapEndianess(static_cast<CIEC_INT &>(member))); break;
        case CIEC_ANY::e_DINT: member.setValue(swapEndianess(static_cast<CIEC_DINT &>(member))); break;
        case CIEC_ANY::e_LINT: member.setValue(swapEndianess(static_cast<CIEC_LINT &>(member))); break;
        case CIEC_ANY::e_USINT: member.setValue(swapEndianess(static_cast<CIEC_USINT &>(member))); break;
        case CIEC_ANY::e_UINT: member.setValue(swapEndianess(static_cast<CIEC_UINT &>(member))); break;
        case CIEC_ANY::e_UDINT: member.setValue(swapEndianess(static_cast<CIEC_UDINT &>(member))); break;
        case CIEC_ANY::e_ULINT: member.setValue(swapEndianess(static_cast<CIEC_ULINT &>(member))); break;
        case CIEC_ANY::e_BYTE: member.setValue(swapEndianess(static_cast<CIEC_BYTE &>(member))); break;
        case CIEC_ANY::e_WORD: member.setValue(swapEndianess(static_cast<CIEC_WORD &>(member))); break;
        case CIEC_ANY::e_DWORD: member.setValue(swapEndianess(static_cast<CIEC_DWORD &>(member))); break;
        case CIEC_ANY::e_LWORD: member.setValue(swapEndianess(static_cast<CIEC_LWORD &>(member))); break;
        case CIEC_ANY::e_DATE: member.setValue(swapEndianess(static_cast<CIEC_DATE &>(member))); break;
        case CIEC_ANY::e_TIME_OF_DAY: member.setValue(swapEndianess(static_cast<CIEC_TIME_OF_DAY &>(member))); break;
        case CIEC_ANY::e_DATE_AND_TIME:
          member.setValue(swapEndianess(static_cast<CIEC_DATE_AND_TIME &>(member)));
          break;
        case CIEC_ANY::e_TIME: member.setValue(swapEndianess(static_cast<CIEC_TIME &>(member))); break;
        case CIEC_ANY::e_CHAR: member.setValue(swapEndianess(static_cast<CIEC_CHAR &>(member))); break;
        case CIEC_ANY::e_WCHAR: member.setValue(swapEndianess(static_cast<CIEC_WCHAR &>(member))); break;
        case CIEC_ANY::e_LDATE: member.setValue(swapEndianess(static_cast<CIEC_LDATE &>(member))); break;
        case CIEC_ANY::e_LTIME_OF_DAY: member.setValue(swapEndianess(static_cast<CIEC_LTIME_OF_DAY &>(member))); break;
        case CIEC_ANY::e_LDATE_AND_TIME:
          member.setValue(swapEndianess(static_cast<CIEC_LDATE_AND_TIME &>(member)));
          break;
        case CIEC_ANY::e_LTIME: member.setValue(swapEndianess(static_cast<CIEC_LDATE &>(member))); break;
        case CIEC_ANY::e_REAL: member.setValue(swapEndianess(static_cast<CIEC_REAL &>(member))); break;
        case CIEC_ANY::e_LREAL: member.setValue(swapEndianess(static_cast<CIEC_LREAL &>(member))); break;
        case CIEC_ANY::e_STRING: member.setValue(swapEndianess(static_cast<CIEC_STRING &>(member))); break;
        case CIEC_ANY::e_WSTRING: member.setValue(swapEndianess(static_cast<CIEC_WSTRING &>(member))); break;
        case CIEC_ANY::e_ARRAY: swapEndianessInplace(static_cast<CIEC_ARRAY &>(member)); break;
        case CIEC_ANY::e_STRUCT: swapEndianessInplace(static_cast<CIEC_STRUCT &>(member)); break;
        default: break; // do nothing
      }
    }
  }

} // namespace forte
