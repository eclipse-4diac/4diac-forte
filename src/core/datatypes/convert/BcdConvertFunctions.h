/*******************************************************************************
 * Copyright (c) 2010 - 2015 TU Vienna/ACIN, Profactor GmbH, fortiss GmbH,
 *                      2018-2019 TU Vienna/ACIN
 *               2022, 2023 Primetals Technologies Austria GmbH
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
 *      - adds more *_BCD_TO_** functions
 *******************************************************************************/

#ifndef SRC_CORE_DATATYPES_CONVERT_BCDCONVERTFUNCTIONS_H_
#define SRC_CORE_DATATYPES_CONVERT_BCDCONVERTFUNCTIONS_H_

//********************************************************************************************
//   *_BCD_TO_**  functions
//********************************************************************************************

template<typename U, typename T>
const U func_BCD_TO(const T& paVal) {
  static_assert(std::is_base_of_v<CIEC_ANY_BIT, T>);
  static_assert(std::is_base_of_v<CIEC_ANY_INT, U>);
  using SourcePrimitive = typename T::TValueType;
  using TargetPrimitive = typename U::TValueType;
  constexpr size_t sourceWidth = sizeof(SourcePrimitive);
  const SourcePrimitive sourceValue = static_cast<SourcePrimitive>(paVal);
  TargetPrimitive targetValue = 0;
  TargetPrimitive multiplicator = 1;
  for(size_t i = 0; i < sourceWidth; ++i) {
    targetValue += static_cast<TargetPrimitive>(((sourceValue >> (i * 8)) & 0x0F) * multiplicator);
    multiplicator *= 10;
    targetValue += static_cast<TargetPrimitive>(((sourceValue >> (i * 8 + 4)) & 0x0F) * multiplicator);
    multiplicator *= 10;
  }
  return U(targetValue);
}

/**** BYTE_BCD_TO_* ****/
inline const CIEC_USINT func_BYTE_BCD_TO_USINT(const CIEC_BYTE &paVal) {
  return func_BCD_TO<CIEC_USINT>(paVal);
}

inline const CIEC_UINT func_BYTE_BCD_TO_UINT(const CIEC_BYTE &paVal) {
  return func_BCD_TO<CIEC_UINT>(paVal);
}

inline const CIEC_UDINT func_BYTE_BCD_TO_UDINT(const CIEC_BYTE &paVal) {
  return func_BCD_TO<CIEC_UDINT>(paVal);
}

inline const CIEC_ULINT func_BYTE_BCD_TO_ULINT(const CIEC_BYTE &paVal) {
  return func_BCD_TO<CIEC_ULINT>(paVal);
}

/**** WORD_BCD_TO_* ****/
inline const CIEC_USINT func_WORD_BCD_TO_USINT(const CIEC_WORD &paVal) {
  return func_BCD_TO<CIEC_USINT>(paVal);
}

inline const CIEC_UINT func_WORD_BCD_TO_UINT(const CIEC_WORD &paVal) {
  return func_BCD_TO<CIEC_UINT>(paVal);
}

inline const CIEC_UDINT func_WORD_BCD_TO_UDINT(const CIEC_WORD &paVal) {
  return func_BCD_TO<CIEC_UDINT>(paVal);
}

inline const CIEC_ULINT func_WORD_BCD_TO_ULINT(const CIEC_WORD &paVal) {
  return func_BCD_TO<CIEC_ULINT>(paVal);
}

/**** DWORD_BCD_TO_* ****/
inline const CIEC_USINT func_DWORD_BCD_TO_USINT(const CIEC_DWORD &paVal) {
  return func_BCD_TO<CIEC_USINT>(paVal);
}

inline const CIEC_UINT func_DWORD_BCD_TO_UINT(const CIEC_DWORD &paVal) {
  return func_BCD_TO<CIEC_UINT>(paVal);
}

inline const CIEC_UDINT func_DWORD_BCD_TO_UDINT(const CIEC_DWORD &paVal) {
  return func_BCD_TO<CIEC_UDINT>(paVal);
}

inline const CIEC_ULINT func_DWORD_BCD_TO_ULINT(const CIEC_DWORD &paVal) {
  return func_BCD_TO<CIEC_ULINT>(paVal);
}

/**** LWORD_BCD_TO_* ****/
inline const CIEC_USINT func_LWORD_BCD_TO_USINT(const CIEC_LWORD &paVal) {
  return func_BCD_TO<CIEC_USINT>(paVal);
}

inline const CIEC_UINT func_LWORD_BCD_TO_UINT(const CIEC_LWORD &paVal) {
  return func_BCD_TO<CIEC_UINT>(paVal);
}

inline const CIEC_UDINT func_LWORD_BCD_TO_UDINT(const CIEC_LWORD &paVal) {
  return func_BCD_TO<CIEC_UDINT>(paVal);
}

inline const CIEC_ULINT func_LWORD_BCD_TO_ULINT(const CIEC_LWORD &paVal) {
  return func_BCD_TO<CIEC_ULINT>(paVal);
}

//********************************************************************************************
//   **_TO_BCD_*  functions
//********************************************************************************************

inline const CIEC_BYTE func_USINT_TO_BCD_BYTE(const CIEC_USINT &paVal) {
  const CIEC_USINT::TValueType valuePrimitive = static_cast<CIEC_USINT::TValueType>(paVal);
  return CIEC_BYTE(static_cast<CIEC_BYTE::TValueType>(valuePrimitive / 10 * 16 + valuePrimitive % 10));
  //return func_TO_BCD<CIEC_BYTE>(paVal);
}

inline const CIEC_WORD func_UINT_TO_BCD_WORD(const CIEC_UINT &paVal) {
  const CIEC_UINT::TValueType primitiveValue = static_cast<CIEC_UINT::TValueType>(paVal);
  const CIEC_WORD::TValueType lowerDigits = static_cast<CIEC_WORD::TValueType>(func_USINT_TO_BCD_BYTE(CIEC_USINT(static_cast<CIEC_USINT::TValueType>(primitiveValue / 100)))) << 8;
  const CIEC_WORD::TValueType upperDigits = static_cast<CIEC_WORD::TValueType>(func_USINT_TO_BCD_BYTE(CIEC_USINT(static_cast<CIEC_USINT::TValueType>(primitiveValue % 100))));

  return CIEC_WORD(lowerDigits + upperDigits);
}

inline const CIEC_DWORD func_UDINT_TO_BCD_DWORD(const CIEC_UDINT &paVal) {
  const CIEC_UDINT::TValueType primitiveValue = static_cast<CIEC_UDINT::TValueType>(paVal);
  const CIEC_DWORD::TValueType upperDigits = static_cast<CIEC_DWORD::TValueType>(func_UINT_TO_BCD_WORD(CIEC_UINT(static_cast<CIEC_UINT::TValueType>(primitiveValue / 10000)))) << 16;
  const CIEC_DWORD::TValueType lowerDigits = static_cast<CIEC_DWORD::TValueType>(func_UINT_TO_BCD_WORD(CIEC_UINT(static_cast<CIEC_UINT::TValueType>(primitiveValue % 10000))));
  return CIEC_DWORD(upperDigits + lowerDigits);
}

inline const CIEC_LWORD func_ULINT_TO_BCD_LWORD(const CIEC_ULINT &paVal) {
  const CIEC_ULINT::TValueType primitiveValue = static_cast<CIEC_ULINT::TValueType>(paVal);
  const CIEC_LWORD::TValueType upperDigits = static_cast<CIEC_LWORD::TValueType>(func_UDINT_TO_BCD_DWORD(CIEC_UDINT(static_cast<CIEC_UDINT::TValueType>(primitiveValue / 100000000)))) << 32;
  const CIEC_LWORD::TValueType lowerDigits = static_cast<CIEC_LWORD::TValueType>(func_UDINT_TO_BCD_DWORD(CIEC_UDINT(static_cast<CIEC_UDINT::TValueType>(primitiveValue % 100000000))));

  return CIEC_LWORD(upperDigits + lowerDigits);
}

#endif /* SRC_CORE_DATATYPES_CONVERT_BCDCONVERTFUNCTIONS_H_ */
