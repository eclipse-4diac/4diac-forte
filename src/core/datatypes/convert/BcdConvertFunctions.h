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

/**** *_BCD_TO_* ****/
template<typename T>
const CIEC_USINT func_BCD_TO_USINT(const T &paVal) {
  return func_BCD_TO<CIEC_USINT>(paVal);
}

template<typename T>
const CIEC_UINT func_BCD_TO_UINT(const T &paVal) {
  return func_BCD_TO<CIEC_UINT>(paVal);
}

template<typename T>
const CIEC_UDINT func_BCD_TO_UDINT(const T &paVal) {
  return func_BCD_TO<CIEC_UDINT>(paVal);
}

template<typename T>
const CIEC_ULINT func_BCD_TO_ULINT(const T &paVal) {
  return func_BCD_TO<CIEC_ULINT>(paVal);
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

template<typename U, typename T>
const U func_TO_BCD(const T& paVal) {
  static_assert(std::is_base_of_v<CIEC_ANY_BIT, U>);
  static_assert(std::is_base_of_v<CIEC_ANY_INT, T>);
  using SourcePrimitive = typename T::TValueType;
  using TargetPrimitive = typename U::TValueType;
  constexpr size_t targetWidth = sizeof(TargetPrimitive);

  SourcePrimitive sourceValue = static_cast<SourcePrimitive>(paVal);
  SourcePrimitive divisor = 10;

  TargetPrimitive bcdValue = 0;
  for(size_t i = 0; i < targetWidth; ++i) {
    bcdValue += static_cast<TargetPrimitive>((sourceValue % divisor) << (i * 8));
    sourceValue /= divisor;
    bcdValue += static_cast<TargetPrimitive>((sourceValue % divisor) << (i * 8 + 4));
    sourceValue /= divisor;
  }
  return U(bcdValue);
}

/**** *_TO_BCD_* ****/
template<typename T>
const CIEC_BYTE func_TO_BCD_BYTE(const T &paVal) {
  return func_TO_BCD<CIEC_BYTE>(paVal);
}

template<typename T>
const CIEC_WORD func_TO_BCD_WORD(const T &paVal) {
  return func_TO_BCD<CIEC_WORD>(paVal);
}

template<typename T>
const CIEC_DWORD func_TO_BCD_DWORD(const T &paVal) {
  return func_TO_BCD<CIEC_DWORD>(paVal);
}

template<typename T>
const CIEC_LWORD func_TO_BCD_LWORD(const T &paVal) {
  return func_TO_BCD<CIEC_LWORD>(paVal);
}

/**** USINT_TO_BCD_* ****/
inline const CIEC_BYTE func_USINT_TO_BCD_BYTE(const CIEC_USINT &paVal) {
  return func_TO_BCD<CIEC_BYTE>(paVal);
}

inline const CIEC_WORD func_USINT_TO_BCD_WORD(const CIEC_USINT &paVal) {
  return func_TO_BCD<CIEC_WORD>(paVal);
}

inline const CIEC_DWORD func_USINT_TO_BCD_DWORD(const CIEC_USINT &paVal) {
  return func_TO_BCD<CIEC_DWORD>(paVal);
}

inline const CIEC_LWORD func_USINT_TO_BCD_LWORD(const CIEC_USINT &paVal) {
  return func_TO_BCD<CIEC_LWORD>(paVal);
}

/**** UINT_TO_BCD_* ****/
inline const CIEC_BYTE func_UINT_TO_BCD_BYTE(const CIEC_UINT &paVal) {
  return func_TO_BCD<CIEC_BYTE>(paVal);
}

inline const CIEC_WORD func_UINT_TO_BCD_WORD(const CIEC_UINT &paVal) {
  return func_TO_BCD<CIEC_WORD>(paVal);
}

inline const CIEC_DWORD func_UINT_TO_BCD_DWORD(const CIEC_UINT &paVal) {
  return func_TO_BCD<CIEC_DWORD>(paVal);
}

inline const CIEC_LWORD func_UINT_TO_BCD_LWORD(const CIEC_UINT &paVal) {
  return func_TO_BCD<CIEC_LWORD>(paVal);
}

/**** UDINT_TO_BCD_* ****/
inline const CIEC_BYTE func_UDINT_TO_BCD_BYTE(const CIEC_UDINT &paVal) {
  return func_TO_BCD<CIEC_BYTE>(paVal);
}

inline const CIEC_WORD func_UDINT_TO_BCD_WORD(const CIEC_UDINT &paVal) {
  return func_TO_BCD<CIEC_WORD>(paVal);
}

inline const CIEC_DWORD func_UDINT_TO_BCD_DWORD(const CIEC_UDINT &paVal) {
  return func_TO_BCD<CIEC_DWORD>(paVal);
}

inline const CIEC_LWORD func_UDINT_TO_BCD_LWORD(const CIEC_UDINT &paVal) {
  return func_TO_BCD<CIEC_LWORD>(paVal);
}

/**** ULINT_TO_BCD_* ****/
inline const CIEC_BYTE func_ULINT_TO_BCD_BYTE(const CIEC_ULINT &paVal) {
  return func_TO_BCD<CIEC_BYTE>(paVal);
}

inline const CIEC_WORD func_ULINT_TO_BCD_WORD(const CIEC_ULINT &paVal) {
  return func_TO_BCD<CIEC_WORD>(paVal);
}

inline const CIEC_DWORD func_ULINT_TO_BCD_DWORD(const CIEC_ULINT &paVal) {
  return func_TO_BCD<CIEC_DWORD>(paVal);
}

inline const CIEC_LWORD func_ULINT_TO_BCD_LWORD(const CIEC_ULINT &paVal) {
  return func_TO_BCD<CIEC_LWORD>(paVal);
}

#endif /* SRC_CORE_DATATYPES_CONVERT_BCDCONVERTFUNCTIONS_H_ */
