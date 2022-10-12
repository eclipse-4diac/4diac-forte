/*******************************************************************************
 * Copyright (c) 2010 - 2015 TU Vienna/ACIN, Profactor GmbH, fortiss GmbH,
 *                      2018-2019 TU Vienna/ACIN
 *               2022 Primetals Technologies Austria GmbH
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
 *    Martin Melik-Merkumians - fixes DT_TO_TOD, removes invalid casts,
 *        update implementation to use new cast function, changes for removed 
 *        implicit constructor for primitve types
 *******************************************************************************/

#ifndef SRC_CORE_DATATYPES_CONVERT_BCDCONVERTFUNCTIONS_H_
#define SRC_CORE_DATATYPES_CONVERT_BCDCONVERTFUNCTIONS_H_

//********************************************************************************************
//   *_BCD_TO_**  functions
//********************************************************************************************
inline const CIEC_USINT func_BYTE_BCD_TO_USINT(const CIEC_BYTE &paVal) {
  return CIEC_USINT(static_cast<TForteUInt8>((paVal / 16 * 10) + (paVal % 16)));
}

inline const CIEC_UINT func_WORD_BCD_TO_UINT(const CIEC_WORD &paVal) {
  return CIEC_UINT(
    static_cast<TForteUInt16>(func_BYTE_BCD_TO_USINT(CIEC_BYTE(static_cast<TForteByte>(paVal >> 8))) * 100
      + func_BYTE_BCD_TO_USINT(CIEC_BYTE(static_cast<TForteByte>(paVal & 0xff)))));
}

inline const CIEC_UDINT func_DWORD_BCD_TO_UDINT(const CIEC_DWORD &paVal) {
  return CIEC_UDINT(
    static_cast<TForteUInt32>(func_WORD_BCD_TO_UINT(CIEC_WORD(static_cast<TForteWord>(paVal >> 16))) * 10000
      + func_WORD_BCD_TO_UINT(CIEC_WORD(static_cast<TForteWord>(paVal & 0xffff)))));
}

inline const CIEC_ULINT func_LWORD_BCD_TO_ULINT(const CIEC_LWORD &paVal){
  return CIEC_ULINT(static_cast<TForteUInt64>(func_DWORD_BCD_TO_UDINT(CIEC_DWORD(static_cast<TForteDWord>(paVal >> 32))) * 100000000 + func_DWORD_BCD_TO_UDINT(CIEC_DWORD(static_cast<TForteDWord>(paVal & 0xffffffff)))));
}

//********************************************************************************************
//   **_TO_BCD_*  functions
//********************************************************************************************
inline const CIEC_BYTE func_USINT_TO_BCD_BYTE(const CIEC_USINT &paVal) {
  return CIEC_BYTE(static_cast<TForteByte>(paVal / 10 * 16 + paVal % 10));
}

inline const CIEC_WORD func_UINT_TO_BCD_WORD(const CIEC_UINT &paVal) {
  return CIEC_WORD(
    static_cast<TForteWord>((static_cast<TForteWord>(func_USINT_TO_BCD_BYTE(CIEC_USINT(static_cast<TForteUInt8>(paVal / 100))) << 8))
      + static_cast<TForteWord>(func_USINT_TO_BCD_BYTE(CIEC_USINT(static_cast<TForteUInt8>(paVal % 100))))));
}

inline const CIEC_DWORD func_UDINT_TO_BCD_DWORD(const CIEC_UDINT &paVal) {
  return CIEC_DWORD(
    static_cast<TForteDWord>((static_cast<TForteDWord>(func_UINT_TO_BCD_WORD(CIEC_UINT(static_cast<TForteUInt16>(paVal / 10000)))) << 16)
      + static_cast<TForteDWord>(func_UINT_TO_BCD_WORD(CIEC_UINT(static_cast<TForteUInt16>(paVal % 10000))))));
}

inline const CIEC_LWORD func_ULINT_TO_BCD_LWORD(const CIEC_ULINT &paVal){
  return CIEC_LWORD(static_cast<TForteLWord>((static_cast<TForteLWord>(func_UDINT_TO_BCD_DWORD(CIEC_UDINT(static_cast<TForteUInt32>(paVal / 100000000)))) << 32)
   + static_cast<TForteLWord>(func_UDINT_TO_BCD_DWORD(CIEC_UDINT(static_cast<TForteUInt32>(paVal % 100000000))))));
}

#endif /* SRC_CORE_DATATYPES_CONVERT_BCDCONVERTFUNCTIONS_H_ */
