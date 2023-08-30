/*******************************************************************************
 * Copyright (c) 2018 TU Vienna/ACIN
 *               2022, 2023 Primetals Technologies Austria GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Martin Melik Merkumians
 *      - initial implementation of template metaprograms
 *      - adds macro functions for easier defintion of allowed casts, metaprograms
 *      for is_base_of including is_base_helper, conditional, and is_same
 *      - adds get_equivalent_CIEC_class template
 *      - added cast helper for MUL and DIV which can be mixed with ANY_DURATION
 *      - adds custom return type calculation for ADD and SUB, needed for
 *          ANY_DURATION types
 *      - replaced custom templates with std templates, removed unused ones
 *******************************************************************************/

#include "datatype.h"

#include <type_traits>
#include <functional>

#ifndef SRC_CORE_IEC61131_CAST_HELPER_CPP_
#define SRC_CORE_IEC61131_CAST_HELPER_CPP_

class CIEC_ANY_NUM;
class CIEC_ANY_DURATION;

class CIEC_TIME;
class CIEC_TIME_OF_DAY;
class CIEC_DATE;
class CIEC_DATE_AND_TIME;

class CIEC_LTIME;
class CIEC_LTIME_OF_DAY;
class CIEC_LDATE;
class CIEC_LDATE_AND_TIME;

class CIEC_BOOL;
class CIEC_BYTE;
class CIEC_WORD;
class CIEC_DWORD;
class CIEC_LWORD;

class CIEC_ANY_UNSIGNED;
class CIEC_USINT;
class CIEC_UINT;
class CIEC_UDINT;
class CIEC_ULINT;

class CIEC_ANY_INT;
class CIEC_ANY_SIGNED;
class CIEC_SINT;
class CIEC_INT;
class CIEC_DINT;
class CIEC_LINT;
class CIEC_REAL;
class CIEC_LREAL;

class CIEC_ANY_CHARS;
class CIEC_ANY_CHAR;
class CIEC_CHAR;
class CIEC_WCHAR;
class CIEC_ANY_STRING;
class CIEC_STRING;
class CIEC_WSTRING;

#define ALLOW_IMPLICIT_CAST(from, to) \
template<> struct implicit_cast<from, to>{ \
    typedef to type; \
    implicit_cast() = delete; \
};

#define ALLOW_EXPLICIT_CAST(from, to) \
template<> struct explicit_cast<from, to>{ \
    typedef to type; \
    explicit_cast() = delete; \
};

namespace forte {
  namespace core {
    namespace mpl {

      struct NullType{
          NullType() = delete;
      };

      template<typename CommonSubtype, typename T, typename U>
      struct are_of_subtype {
        enum {
          value = (std::is_base_of<CommonSubtype, T>::value && std::is_base_of<CommonSubtype, U>::value)
        };
        are_of_subtype() = delete;
      };

      /** @brief Are T and U subtyped of CommonSubType?
       *
       * @tparam CommonSubType the supertype to be checked against
       * @tparam T type 1
       * @tparam U type 2
       * @return NullType if not known, default CIEC class otherwise
       */
      template<typename CommonSubtype, typename T, typename U>
      constexpr auto are_of_subtype_v = are_of_subtype<CommonSubtype, T, U>::value;

      /* For PARTIAL classes, which derive from their base CIEC class, which can be used to identify the CIEC class*/
      template<typename T> struct get_equivalent_CIEC_class {
        typedef typename std::conditional<std::is_base_of<CIEC_BOOL, T>::value, CIEC_BOOL, typename std::conditional<std::is_base_of<CIEC_BYTE, T>::value, CIEC_BYTE, typename std::conditional<std::is_base_of<CIEC_WORD, T>::value, CIEC_WORD, typename std::conditional<std::is_base_of<CIEC_DWORD, T>::value, CIEC_DWORD, typename std::conditional<std::is_base_of<CIEC_LWORD, T>::value, CIEC_LWORD, NullType>::type>::type>::type>::type>::type type;
        get_equivalent_CIEC_class() = delete;
      };

      /** @brief What is the CIEC class of a CIEC class or specialist subtype?
       *
       * @param T type to be checked
       * @return NullType if not known, default CIEC class otherwise
       */
      template <typename T>
      using get_equivalent_CIEC_class_t = typename get_equivalent_CIEC_class<T>::type;

      /* invalid implicit casts */
      template<typename T, typename U> struct implicit_cast {
        typedef NullType type;
        implicit_cast() = delete;
      };

      /* Self-casts */
      template <typename T>
      struct implicit_cast<T, T> {
        typedef T type;
        implicit_cast() = delete;
      };

      /** @brief is T implicitly castable to U?
       * 
       * @tparam T type to be cast
       * @tparam U goal type of the cast
       * @return NullType if not castable, type U if castable
      */
      template <typename T, typename U>
      using implicit_cast_t = typename implicit_cast<T, U>::type;

      template<typename T, typename U>
      constexpr auto is_implicitly_castable_v = std::is_same_v<implicit_cast_t<T,U>, U>;

      /* BOOL casts */
      ALLOW_IMPLICIT_CAST(CIEC_BOOL, CIEC_BYTE)
      ALLOW_IMPLICIT_CAST(CIEC_BOOL, CIEC_WORD)
      ALLOW_IMPLICIT_CAST(CIEC_BOOL, CIEC_DWORD)
      ALLOW_IMPLICIT_CAST(CIEC_BOOL, CIEC_LWORD)

      /* BYTE casts */
      ALLOW_IMPLICIT_CAST(CIEC_BYTE, CIEC_WORD)
      ALLOW_IMPLICIT_CAST(CIEC_BYTE, CIEC_DWORD)
      ALLOW_IMPLICIT_CAST(CIEC_BYTE, CIEC_LWORD)

      /*WORD casts */
      ALLOW_IMPLICIT_CAST(CIEC_WORD, CIEC_DWORD)
      ALLOW_IMPLICIT_CAST(CIEC_WORD, CIEC_LWORD)

      /* DWORD casts */
      ALLOW_IMPLICIT_CAST(CIEC_DWORD, CIEC_LWORD)

      /* ANY_INT casts*/
      ALLOW_IMPLICIT_CAST(CIEC_ANY_INT, CIEC_SINT)
      ALLOW_IMPLICIT_CAST(CIEC_ANY_INT, CIEC_INT)
      ALLOW_IMPLICIT_CAST(CIEC_ANY_INT, CIEC_DINT)
      ALLOW_IMPLICIT_CAST(CIEC_ANY_INT, CIEC_LINT)
      ALLOW_IMPLICIT_CAST(CIEC_ANY_INT, CIEC_USINT)
      ALLOW_IMPLICIT_CAST(CIEC_ANY_INT, CIEC_UINT)
      ALLOW_IMPLICIT_CAST(CIEC_ANY_INT, CIEC_UDINT)
      ALLOW_IMPLICIT_CAST(CIEC_ANY_INT, CIEC_ULINT)

      /* ANY_UNSIGNED casts*/
      ALLOW_IMPLICIT_CAST(CIEC_ANY_UNSIGNED, CIEC_USINT)
      ALLOW_IMPLICIT_CAST(CIEC_ANY_UNSIGNED, CIEC_UINT)
      ALLOW_IMPLICIT_CAST(CIEC_ANY_UNSIGNED, CIEC_UDINT)
      ALLOW_IMPLICIT_CAST(CIEC_ANY_UNSIGNED, CIEC_ULINT)

      /* USINT casts*/
      ALLOW_IMPLICIT_CAST(CIEC_USINT, CIEC_UINT)
      ALLOW_IMPLICIT_CAST(CIEC_USINT, CIEC_UDINT)
      ALLOW_IMPLICIT_CAST(CIEC_USINT, CIEC_ULINT)
      ALLOW_IMPLICIT_CAST(CIEC_USINT, CIEC_INT)
      ALLOW_IMPLICIT_CAST(CIEC_USINT, CIEC_DINT)
      ALLOW_IMPLICIT_CAST(CIEC_USINT, CIEC_LINT)
      ALLOW_IMPLICIT_CAST(CIEC_USINT, CIEC_REAL)
      ALLOW_IMPLICIT_CAST(CIEC_USINT, CIEC_LREAL)

      /* UINT casts*/
      ALLOW_IMPLICIT_CAST(CIEC_UINT, CIEC_UDINT)
      ALLOW_IMPLICIT_CAST(CIEC_UINT, CIEC_ULINT)
      ALLOW_IMPLICIT_CAST(CIEC_UINT, CIEC_DINT)
      ALLOW_IMPLICIT_CAST(CIEC_UINT, CIEC_LINT)
      ALLOW_IMPLICIT_CAST(CIEC_UINT, CIEC_REAL)
      ALLOW_IMPLICIT_CAST(CIEC_UINT, CIEC_LREAL)

      /* UDINT casts*/
      ALLOW_IMPLICIT_CAST(CIEC_UDINT, CIEC_ULINT)
      ALLOW_IMPLICIT_CAST(CIEC_UDINT, CIEC_LINT)
      ALLOW_IMPLICIT_CAST(CIEC_UDINT, CIEC_LREAL)

      /* ANY_SIGNED casts*/
      ALLOW_IMPLICIT_CAST(CIEC_ANY_SIGNED, CIEC_SINT)
      ALLOW_IMPLICIT_CAST(CIEC_ANY_SIGNED, CIEC_INT)
      ALLOW_IMPLICIT_CAST(CIEC_ANY_SIGNED, CIEC_DINT)
      ALLOW_IMPLICIT_CAST(CIEC_ANY_SIGNED, CIEC_LINT)

      /* SINT casts */
      ALLOW_IMPLICIT_CAST(CIEC_SINT, CIEC_INT)
      ALLOW_IMPLICIT_CAST(CIEC_SINT, CIEC_DINT)
      ALLOW_IMPLICIT_CAST(CIEC_SINT, CIEC_LINT)
      ALLOW_IMPLICIT_CAST(CIEC_SINT, CIEC_REAL)
      ALLOW_IMPLICIT_CAST(CIEC_SINT, CIEC_LREAL)

      /* INT casts */
      ALLOW_IMPLICIT_CAST(CIEC_INT, CIEC_DINT)
      ALLOW_IMPLICIT_CAST(CIEC_INT, CIEC_LINT)
      ALLOW_IMPLICIT_CAST(CIEC_INT, CIEC_REAL)
      ALLOW_IMPLICIT_CAST(CIEC_INT, CIEC_LREAL)

      /* DINT casts */
      ALLOW_IMPLICIT_CAST(CIEC_DINT, CIEC_LINT)
      ALLOW_IMPLICIT_CAST(CIEC_DINT, CIEC_LREAL)

// REAL implicit casts
      ALLOW_IMPLICIT_CAST(CIEC_REAL, CIEC_LREAL)

// Explicit casts
      /* invalid explicit casts */
      template<typename T, typename U> struct explicit_cast{
          typedef NullType type;
          explicit_cast() = delete;
      };

      /** @brief is T explicitly castable to U?
       * 
       * @tparam T type to be cast
       * @tparam U goal type of the cast
       * @return NullType if not castable, type U if castable
      */
      template <typename T, typename U>
      using explicit_cast_t = typename explicit_cast<T, U>::type;

      template<typename T, typename U>
      constexpr auto is_explicitly_castable_v = std::is_same_v<explicit_cast_t<T,U>, U>;

// BOOL explicit casts
      ALLOW_EXPLICIT_CAST(CIEC_BOOL, CIEC_USINT)
      ALLOW_EXPLICIT_CAST(CIEC_BOOL, CIEC_UINT)
      ALLOW_EXPLICIT_CAST(CIEC_BOOL, CIEC_UDINT)
      ALLOW_EXPLICIT_CAST(CIEC_BOOL, CIEC_ULINT)
      ALLOW_EXPLICIT_CAST(CIEC_BOOL, CIEC_SINT)
      ALLOW_EXPLICIT_CAST(CIEC_BOOL, CIEC_INT)
      ALLOW_EXPLICIT_CAST(CIEC_BOOL, CIEC_DINT)
      ALLOW_EXPLICIT_CAST(CIEC_BOOL, CIEC_LINT)

// BYTE explicit casts
      ALLOW_EXPLICIT_CAST(CIEC_BYTE, CIEC_BOOL)
      ALLOW_EXPLICIT_CAST(CIEC_BYTE, CIEC_USINT)
      ALLOW_EXPLICIT_CAST(CIEC_BYTE, CIEC_UINT)
      ALLOW_EXPLICIT_CAST(CIEC_BYTE, CIEC_UDINT)
      ALLOW_EXPLICIT_CAST(CIEC_BYTE, CIEC_ULINT)
      ALLOW_EXPLICIT_CAST(CIEC_BYTE, CIEC_SINT)
      ALLOW_EXPLICIT_CAST(CIEC_BYTE, CIEC_INT)
      ALLOW_EXPLICIT_CAST(CIEC_BYTE, CIEC_DINT)
      ALLOW_EXPLICIT_CAST(CIEC_BYTE, CIEC_LINT)
      ALLOW_EXPLICIT_CAST(CIEC_BYTE, CIEC_CHAR)

      // WORD explicit casts
      ALLOW_EXPLICIT_CAST(CIEC_WORD, CIEC_BOOL)
      ALLOW_EXPLICIT_CAST(CIEC_WORD, CIEC_BYTE)
      ALLOW_EXPLICIT_CAST(CIEC_WORD, CIEC_USINT)
      ALLOW_EXPLICIT_CAST(CIEC_WORD, CIEC_UINT)
      ALLOW_EXPLICIT_CAST(CIEC_WORD, CIEC_UDINT)
      ALLOW_EXPLICIT_CAST(CIEC_WORD, CIEC_ULINT)
      ALLOW_EXPLICIT_CAST(CIEC_WORD, CIEC_SINT)
      ALLOW_EXPLICIT_CAST(CIEC_WORD, CIEC_INT)
      ALLOW_EXPLICIT_CAST(CIEC_WORD, CIEC_DINT)
      ALLOW_EXPLICIT_CAST(CIEC_WORD, CIEC_LINT)
      ALLOW_EXPLICIT_CAST(CIEC_WORD, CIEC_WCHAR)

      // DWORD explicit casts
      ALLOW_EXPLICIT_CAST(CIEC_DWORD, CIEC_BOOL)
      ALLOW_EXPLICIT_CAST(CIEC_DWORD, CIEC_BYTE)
      ALLOW_EXPLICIT_CAST(CIEC_DWORD, CIEC_WORD)
      ALLOW_EXPLICIT_CAST(CIEC_DWORD, CIEC_USINT)
      ALLOW_EXPLICIT_CAST(CIEC_DWORD, CIEC_UINT)
      ALLOW_EXPLICIT_CAST(CIEC_DWORD, CIEC_UDINT)
      ALLOW_EXPLICIT_CAST(CIEC_DWORD, CIEC_ULINT)
      ALLOW_EXPLICIT_CAST(CIEC_DWORD, CIEC_SINT)
      ALLOW_EXPLICIT_CAST(CIEC_DWORD, CIEC_INT)
      ALLOW_EXPLICIT_CAST(CIEC_DWORD, CIEC_DINT)
      ALLOW_EXPLICIT_CAST(CIEC_DWORD, CIEC_LINT)
      ALLOW_EXPLICIT_CAST(CIEC_DWORD, CIEC_REAL)

    // LWORD explicit casts
      ALLOW_EXPLICIT_CAST(CIEC_LWORD, CIEC_BOOL)
      ALLOW_EXPLICIT_CAST(CIEC_LWORD, CIEC_BYTE)
      ALLOW_EXPLICIT_CAST(CIEC_LWORD, CIEC_WORD)
      ALLOW_EXPLICIT_CAST(CIEC_LWORD, CIEC_DWORD)
      ALLOW_EXPLICIT_CAST(CIEC_LWORD, CIEC_USINT)
      ALLOW_EXPLICIT_CAST(CIEC_LWORD, CIEC_UINT)
      ALLOW_EXPLICIT_CAST(CIEC_LWORD, CIEC_UDINT)
      ALLOW_EXPLICIT_CAST(CIEC_LWORD, CIEC_ULINT)
      ALLOW_EXPLICIT_CAST(CIEC_LWORD, CIEC_SINT)
      ALLOW_EXPLICIT_CAST(CIEC_LWORD, CIEC_INT)
      ALLOW_EXPLICIT_CAST(CIEC_LWORD, CIEC_DINT)
      ALLOW_EXPLICIT_CAST(CIEC_LWORD, CIEC_LINT)
      ALLOW_EXPLICIT_CAST(CIEC_LWORD, CIEC_LREAL)

// USINT explicit casts
      ALLOW_EXPLICIT_CAST(CIEC_USINT, CIEC_SINT)
      ALLOW_EXPLICIT_CAST(CIEC_USINT, CIEC_BYTE)
      ALLOW_EXPLICIT_CAST(CIEC_USINT, CIEC_WORD)
      ALLOW_EXPLICIT_CAST(CIEC_USINT, CIEC_DWORD)
      ALLOW_EXPLICIT_CAST(CIEC_USINT, CIEC_LWORD)

// UINT explicit casts
      ALLOW_EXPLICIT_CAST(CIEC_UINT, CIEC_USINT)
      ALLOW_EXPLICIT_CAST(CIEC_UINT, CIEC_SINT)
      ALLOW_EXPLICIT_CAST(CIEC_UINT, CIEC_INT)
      ALLOW_EXPLICIT_CAST(CIEC_UINT, CIEC_BYTE)
      ALLOW_EXPLICIT_CAST(CIEC_UINT, CIEC_WORD)
      ALLOW_EXPLICIT_CAST(CIEC_UINT, CIEC_DWORD)
      ALLOW_EXPLICIT_CAST(CIEC_UINT, CIEC_LWORD)

// UDINT explicit casts
      ALLOW_EXPLICIT_CAST(CIEC_UDINT, CIEC_USINT)
      ALLOW_EXPLICIT_CAST(CIEC_UDINT, CIEC_UINT)
      ALLOW_EXPLICIT_CAST(CIEC_UDINT, CIEC_SINT)
      ALLOW_EXPLICIT_CAST(CIEC_UDINT, CIEC_INT)
      ALLOW_EXPLICIT_CAST(CIEC_UDINT, CIEC_DINT)
      ALLOW_EXPLICIT_CAST(CIEC_UDINT, CIEC_BYTE)
      ALLOW_EXPLICIT_CAST(CIEC_UDINT, CIEC_WORD)
      ALLOW_EXPLICIT_CAST(CIEC_UDINT, CIEC_DWORD)
      ALLOW_EXPLICIT_CAST(CIEC_UDINT, CIEC_LWORD)
      ALLOW_EXPLICIT_CAST(CIEC_UDINT, CIEC_REAL)

// ULINT explicit casts
      ALLOW_EXPLICIT_CAST(CIEC_ULINT, CIEC_USINT)
      ALLOW_EXPLICIT_CAST(CIEC_ULINT, CIEC_UINT)
      ALLOW_EXPLICIT_CAST(CIEC_ULINT, CIEC_UDINT)
      ALLOW_EXPLICIT_CAST(CIEC_ULINT, CIEC_SINT)
      ALLOW_EXPLICIT_CAST(CIEC_ULINT, CIEC_INT)
      ALLOW_EXPLICIT_CAST(CIEC_ULINT, CIEC_DINT)
      ALLOW_EXPLICIT_CAST(CIEC_ULINT, CIEC_LINT)
      ALLOW_EXPLICIT_CAST(CIEC_ULINT, CIEC_BYTE)
      ALLOW_EXPLICIT_CAST(CIEC_ULINT, CIEC_WORD)
      ALLOW_EXPLICIT_CAST(CIEC_ULINT, CIEC_DWORD)
      ALLOW_EXPLICIT_CAST(CIEC_ULINT, CIEC_LWORD)
      ALLOW_EXPLICIT_CAST(CIEC_ULINT, CIEC_REAL)
      ALLOW_EXPLICIT_CAST(CIEC_ULINT, CIEC_LREAL)

// SINT explicit casts
      ALLOW_EXPLICIT_CAST(CIEC_SINT, CIEC_USINT)
      ALLOW_EXPLICIT_CAST(CIEC_SINT, CIEC_UINT)
      ALLOW_EXPLICIT_CAST(CIEC_SINT, CIEC_UDINT)
      ALLOW_EXPLICIT_CAST(CIEC_SINT, CIEC_ULINT)
      ALLOW_EXPLICIT_CAST(CIEC_SINT, CIEC_BYTE)
      ALLOW_EXPLICIT_CAST(CIEC_SINT, CIEC_WORD)
      ALLOW_EXPLICIT_CAST(CIEC_SINT, CIEC_DWORD)
      ALLOW_EXPLICIT_CAST(CIEC_SINT, CIEC_LWORD)

// INT explicit casts
      ALLOW_EXPLICIT_CAST(CIEC_INT, CIEC_SINT)
      ALLOW_EXPLICIT_CAST(CIEC_INT, CIEC_USINT)
      ALLOW_EXPLICIT_CAST(CIEC_INT, CIEC_UINT)
      ALLOW_EXPLICIT_CAST(CIEC_INT, CIEC_UDINT)
      ALLOW_EXPLICIT_CAST(CIEC_INT, CIEC_ULINT)
      ALLOW_EXPLICIT_CAST(CIEC_INT, CIEC_BYTE)
      ALLOW_EXPLICIT_CAST(CIEC_INT, CIEC_WORD)
      ALLOW_EXPLICIT_CAST(CIEC_INT, CIEC_DWORD)
      ALLOW_EXPLICIT_CAST(CIEC_INT, CIEC_LWORD)

// DINT explicit casts
      ALLOW_EXPLICIT_CAST(CIEC_DINT, CIEC_SINT)
      ALLOW_EXPLICIT_CAST(CIEC_DINT, CIEC_INT)
      ALLOW_EXPLICIT_CAST(CIEC_DINT, CIEC_USINT)
      ALLOW_EXPLICIT_CAST(CIEC_DINT, CIEC_UINT)
      ALLOW_EXPLICIT_CAST(CIEC_DINT, CIEC_UDINT)
      ALLOW_EXPLICIT_CAST(CIEC_DINT, CIEC_ULINT)
      ALLOW_EXPLICIT_CAST(CIEC_DINT, CIEC_BYTE)
      ALLOW_EXPLICIT_CAST(CIEC_DINT, CIEC_WORD)
      ALLOW_EXPLICIT_CAST(CIEC_DINT, CIEC_DWORD)
      ALLOW_EXPLICIT_CAST(CIEC_DINT, CIEC_LWORD)
      ALLOW_EXPLICIT_CAST(CIEC_DINT, CIEC_REAL)

// LINT explicit casts
      ALLOW_EXPLICIT_CAST(CIEC_LINT, CIEC_SINT)
      ALLOW_EXPLICIT_CAST(CIEC_LINT, CIEC_INT)
      ALLOW_EXPLICIT_CAST(CIEC_LINT, CIEC_DINT)
      ALLOW_EXPLICIT_CAST(CIEC_LINT, CIEC_USINT)
      ALLOW_EXPLICIT_CAST(CIEC_LINT, CIEC_UINT)
      ALLOW_EXPLICIT_CAST(CIEC_LINT, CIEC_UDINT)
      ALLOW_EXPLICIT_CAST(CIEC_LINT, CIEC_ULINT)
      ALLOW_EXPLICIT_CAST(CIEC_LINT, CIEC_BYTE)
      ALLOW_EXPLICIT_CAST(CIEC_LINT, CIEC_WORD)
      ALLOW_EXPLICIT_CAST(CIEC_LINT, CIEC_DWORD)
      ALLOW_EXPLICIT_CAST(CIEC_LINT, CIEC_LWORD)
      ALLOW_EXPLICIT_CAST(CIEC_LINT, CIEC_REAL)
      ALLOW_EXPLICIT_CAST(CIEC_LINT, CIEC_LREAL)

// REAL explicit casts
      ALLOW_EXPLICIT_CAST(CIEC_REAL, CIEC_SINT)
      ALLOW_EXPLICIT_CAST(CIEC_REAL, CIEC_INT)
      ALLOW_EXPLICIT_CAST(CIEC_REAL, CIEC_DINT)
      ALLOW_EXPLICIT_CAST(CIEC_REAL, CIEC_LINT)
      ALLOW_EXPLICIT_CAST(CIEC_REAL, CIEC_USINT)
      ALLOW_EXPLICIT_CAST(CIEC_REAL, CIEC_UINT)
      ALLOW_EXPLICIT_CAST(CIEC_REAL, CIEC_UDINT)
      ALLOW_EXPLICIT_CAST(CIEC_REAL, CIEC_ULINT)
      ALLOW_EXPLICIT_CAST(CIEC_REAL, CIEC_DWORD)

// LREAL explicit casts
      ALLOW_EXPLICIT_CAST(CIEC_LREAL, CIEC_REAL)
      ALLOW_EXPLICIT_CAST(CIEC_LREAL, CIEC_SINT)
      ALLOW_EXPLICIT_CAST(CIEC_LREAL, CIEC_INT)
      ALLOW_EXPLICIT_CAST(CIEC_LREAL, CIEC_DINT)
      ALLOW_EXPLICIT_CAST(CIEC_LREAL, CIEC_LINT)
      ALLOW_EXPLICIT_CAST(CIEC_LREAL, CIEC_USINT)
      ALLOW_EXPLICIT_CAST(CIEC_LREAL, CIEC_UINT)
      ALLOW_EXPLICIT_CAST(CIEC_LREAL, CIEC_UDINT)
      ALLOW_EXPLICIT_CAST(CIEC_LREAL, CIEC_ULINT)
      ALLOW_EXPLICIT_CAST(CIEC_LREAL, CIEC_LWORD)

      // CHAR casts
      ALLOW_IMPLICIT_CAST(CIEC_CHAR, CIEC_STRING)
      ALLOW_EXPLICIT_CAST(CIEC_CHAR, CIEC_USINT)
      ALLOW_EXPLICIT_CAST(CIEC_CHAR, CIEC_BYTE)
      ALLOW_EXPLICIT_CAST(CIEC_CHAR, CIEC_WORD)
      ALLOW_EXPLICIT_CAST(CIEC_CHAR, CIEC_DWORD)
      ALLOW_EXPLICIT_CAST(CIEC_CHAR, CIEC_LWORD)
      ALLOW_EXPLICIT_CAST(CIEC_CHAR, CIEC_WCHAR)
      ALLOW_EXPLICIT_CAST(CIEC_CHAR, CIEC_WSTRING)

      // WCHAR casts
      ALLOW_IMPLICIT_CAST(CIEC_WCHAR, CIEC_WSTRING)
      ALLOW_EXPLICIT_CAST(CIEC_WCHAR, CIEC_WORD)
      ALLOW_EXPLICIT_CAST(CIEC_WCHAR, CIEC_DWORD)
      ALLOW_EXPLICIT_CAST(CIEC_WCHAR, CIEC_LWORD)
      ALLOW_EXPLICIT_CAST(CIEC_WCHAR, CIEC_CHAR)

      // STRING casts
      ALLOW_EXPLICIT_CAST(CIEC_STRING, CIEC_CHAR)
      ALLOW_EXPLICIT_CAST(CIEC_STRING, CIEC_WSTRING)

      // WSTRING casts
      ALLOW_EXPLICIT_CAST(CIEC_WSTRING, CIEC_WCHAR)
      ALLOW_EXPLICIT_CAST(CIEC_WSTRING, CIEC_STRING)

      template<typename T, typename U> struct get_castable_type{
        typedef typename std::conditional<std::is_same<NullType, typename implicit_cast<T, U>::type>::value, typename implicit_cast<U, T>::type, typename implicit_cast<T, U>::type>::type type;
        get_castable_type() = delete;
      };

      /** @brief Return the implicitly castable type of T and U
       * @tparam T type 1
       * @tparam U type 2
       * @return A common type or NullType if none exists
       */
      template <class T, class U>
      using get_castable_type_t = typename get_castable_type<T, U>::type;

      template<typename T, typename U> struct implicit_or_explicit_cast {
        typedef typename std::conditional<std::is_same<NullType, typename explicit_cast<T, U>::type>::value, typename implicit_cast<T, U>::type, typename explicit_cast<T, U>::type>::type type;
        implicit_or_explicit_cast() = delete;
      };

      /** @brief Return the implicitly or explicitly castable type of T and U
       * @tparam T type 1
       * @tparam U type 2
       * @return A common type or NullType if none exists
      */
      template<typename T, typename U>
      using implicit_or_explicit_cast_t = typename implicit_or_explicit_cast<T, U>::type;

      template <typename T, typename U>
      constexpr auto is_implicit_or_explicit_castable_v = (is_implicitly_castable_v<T, U> || is_explicitly_castable_v<T, U>);

      template <typename T, typename U>
      struct get_div_operator_result_type {
        typedef typename get_castable_type<T, U>::type type;
        get_div_operator_result_type() = delete;
      };

      template <typename U>
      struct get_div_operator_result_type<CIEC_TIME, U> {
        typedef typename std::conditional<std::is_base_of<CIEC_ANY_NUM, U>::value, CIEC_TIME, NullType>::type type;
        get_div_operator_result_type() = delete;
      };

      template <typename U>
      struct get_div_operator_result_type<CIEC_LTIME, U> {
        typedef typename std::conditional<std::is_base_of<CIEC_ANY_NUM, U>::value, CIEC_LTIME, NullType>::type type;
        get_div_operator_result_type() = delete;
      };

      template <typename T, typename U>
      struct get_mul_operator_result_type {
        typedef typename get_castable_type<T, U>::type type;
        get_mul_operator_result_type() = delete;
      };

      template <typename U>
      struct get_mul_operator_result_type<CIEC_TIME, U> {
        typedef typename std::conditional<std::is_base_of<CIEC_ANY_NUM, U>::value, CIEC_TIME, NullType>::type type;
        get_mul_operator_result_type() = delete;
      };

      template <typename U>
      struct get_mul_operator_result_type<CIEC_LTIME, U> {
        typedef typename std::conditional<std::is_base_of<CIEC_ANY_NUM, U>::value, CIEC_LTIME, NullType>::type type;
        get_mul_operator_result_type() = delete;
      };

      template <typename T, typename U>
      struct get_add_operator_result_type {
        typedef typename get_castable_type<T, U>::type type;
        get_add_operator_result_type() = delete;
      };

      template <>
      struct get_add_operator_result_type<CIEC_TIME, CIEC_TIME> {
        typedef CIEC_TIME type;
        get_add_operator_result_type() = delete;
      };

      template <>
      struct get_add_operator_result_type<CIEC_TIME_OF_DAY, CIEC_TIME> {
        typedef CIEC_TIME_OF_DAY type;
        get_add_operator_result_type() = delete;
      };

      template <>
      struct get_add_operator_result_type<CIEC_DATE_AND_TIME, CIEC_TIME> {
        typedef CIEC_DATE_AND_TIME type;
        get_add_operator_result_type() = delete;
      };

      template <>
      struct get_add_operator_result_type<CIEC_LTIME, CIEC_LTIME> {
        typedef CIEC_LTIME type;
        get_add_operator_result_type() = delete;
      };

      template <>
      struct get_add_operator_result_type<CIEC_LTIME_OF_DAY, CIEC_LTIME> {
        typedef CIEC_LTIME_OF_DAY type;
        get_add_operator_result_type() = delete;
      };

      template <>
      struct get_add_operator_result_type<CIEC_LDATE_AND_TIME, CIEC_LTIME> {
        typedef CIEC_LDATE_AND_TIME type;
        get_add_operator_result_type() = delete;
      };

      template <typename T, typename U>
      struct get_sub_operator_result_type {
        typedef typename get_castable_type<T, U>::type type;
        get_sub_operator_result_type() = delete;
      };

      template <>
      struct get_sub_operator_result_type<CIEC_TIME, CIEC_TIME> {
        typedef CIEC_TIME type;
        get_sub_operator_result_type() = delete;
      };

      template <>
      struct get_sub_operator_result_type<CIEC_DATE, CIEC_DATE> {
        typedef CIEC_TIME type;
        get_sub_operator_result_type() = delete;
      };

      template <>
      struct get_sub_operator_result_type<CIEC_TIME_OF_DAY, CIEC_TIME> {
        typedef CIEC_TIME_OF_DAY type;
        get_sub_operator_result_type() = delete;
      };

      template <>
      struct get_sub_operator_result_type<CIEC_TIME_OF_DAY, CIEC_TIME_OF_DAY> {
        typedef CIEC_TIME type;
        get_sub_operator_result_type() = delete;
      };

      template <>
      struct get_sub_operator_result_type<CIEC_DATE_AND_TIME, CIEC_TIME> {
        typedef CIEC_DATE_AND_TIME type;
        get_sub_operator_result_type() = delete;
      };

      template <>
      struct get_sub_operator_result_type<CIEC_DATE_AND_TIME, CIEC_DATE_AND_TIME> {
        typedef CIEC_TIME type;
        get_sub_operator_result_type() = delete;
      };

      template <>
      struct get_sub_operator_result_type<CIEC_LTIME, CIEC_LTIME> {
        typedef CIEC_LTIME type;
        get_sub_operator_result_type() = delete;
      };

      template <>
      struct get_sub_operator_result_type<CIEC_LDATE, CIEC_LDATE> {
        typedef CIEC_LTIME type;
        get_sub_operator_result_type() = delete;
      };

      template <>
      struct get_sub_operator_result_type<CIEC_LTIME_OF_DAY, CIEC_LTIME> {
        typedef CIEC_LTIME_OF_DAY type;
        get_sub_operator_result_type() = delete;
      };

      template <>
      struct get_sub_operator_result_type<CIEC_LTIME_OF_DAY, CIEC_LTIME_OF_DAY> {
        typedef CIEC_LTIME type;
        get_sub_operator_result_type() = delete;
      };

      template <>
      struct get_sub_operator_result_type<CIEC_LDATE_AND_TIME, CIEC_LTIME> {
        typedef CIEC_LDATE_AND_TIME type;
        get_sub_operator_result_type() = delete;
      };

      template <>
      struct get_sub_operator_result_type<CIEC_LDATE_AND_TIME, CIEC_LDATE_AND_TIME> {
        typedef CIEC_LTIME type;
        get_sub_operator_result_type() = delete;
      };

      template<typename T>
      struct get_any_chars_base_type {
        typedef std::conditional_t<std::is_base_of_v<CIEC_ANY_CHARS,T>, std::conditional_t<std::is_base_of_v<CIEC_STRING, T>, CIEC_STRING, std::conditional_t<std::is_base_of_v<CIEC_WSTRING, T>, CIEC_WSTRING, T> >, NullType> type;
        get_any_chars_base_type() = delete;
      };

      template <typename T>
      using get_any_chars_base_type_t = typename get_any_chars_base_type<T>::type;

      template <typename T, typename U>
      struct get_concat_result_type_helper {
        typedef NullType type;
        get_concat_result_type_helper() = delete;
      };

      template <>
      struct get_concat_result_type_helper<CIEC_STRING, CIEC_STRING> {
        typedef CIEC_STRING type;
        get_concat_result_type_helper() = delete;
      };

      template <>
      struct get_concat_result_type_helper<CIEC_STRING, CIEC_CHAR> {
        typedef CIEC_STRING type;
        get_concat_result_type_helper() = delete;
      };

      template <>
      struct get_concat_result_type_helper<CIEC_CHAR, CIEC_STRING> {
        typedef CIEC_STRING type;
        get_concat_result_type_helper() = delete;
      };

      template <>
      struct get_concat_result_type_helper<CIEC_CHAR, CIEC_CHAR> {
        typedef CIEC_STRING type;
        get_concat_result_type_helper() = delete;
      };

      template <>
      struct get_concat_result_type_helper<CIEC_WSTRING, CIEC_WSTRING> {
        typedef CIEC_WSTRING type;
        get_concat_result_type_helper() = delete;
      };

      template <>
      struct get_concat_result_type_helper<CIEC_WCHAR, CIEC_WSTRING> {
        typedef CIEC_WSTRING type;
        get_concat_result_type_helper() = delete;
      };

      template <>
      struct get_concat_result_type_helper<CIEC_WSTRING, CIEC_WCHAR> {
        typedef CIEC_WSTRING type;
        get_concat_result_type_helper() = delete;
      };

      template <>
      struct get_concat_result_type_helper<CIEC_WCHAR, CIEC_WCHAR> {
        typedef CIEC_WSTRING type;
        get_concat_result_type_helper() = delete;
      };

      template <typename T, typename U>
      using get_concat_result_type_helper_t = typename get_concat_result_type_helper<T, U>::type;

      template <typename T, typename U>
      struct get_concat_result_type {
        using tBaseType = get_any_chars_base_type_t<T>;
        using uBaseType = get_any_chars_base_type_t<U>;
        typedef get_concat_result_type_helper_t<tBaseType, uBaseType> type;
        get_concat_result_type() = delete;
      };

      template <typename T, typename U>
      using get_concat_result_type_t = typename get_concat_result_type<T, U>::type;

      template <class T, class R, class... Args>
      std::is_convertible<std::invoke_result_t<T, Args...>, R> is_invokable_test(int);

      template <class T, class R, class... Args>
      std::false_type is_invokable_test(...);

      template <class T, class R, class... Args>
      using is_invokable = decltype(is_invokable_test<T, R, Args...>(0));

      template <class T, class R, class... Args>
      constexpr auto is_invokable_v = is_invokable<T, R, Args...>::value;

      template <class L, class R = L>
      using has_equality = is_invokable<std::equal_to<>, bool, L, R>;
      template <class L, class R = L>
      constexpr auto has_equality_v = has_equality<L, R>::value;

      template <class L, class R = L>
      using has_inequality = is_invokable<std::not_equal_to<>, bool, L, R>;
      template <class L, class R = L>
      constexpr auto has_inequality_v = has_inequality<L, R>::value;

      template <class L, class R = L>
      using has_greater = is_invokable<std::greater<>, bool, L, R>;
      template <class L, class R = L>
      constexpr auto has_greater_v = has_greater<L, R>::value;

      template <class L, class R = L>
      using has_less = is_invokable<std::less<>, bool, L, R>;
      template <class L, class R = L>
      constexpr auto has_less_v = has_less<L, R>::value;

      template <class L, class R = L>
      using has_greater_equal = is_invokable<std::greater_equal<>, bool, L, R>;
      template <class L, class R = L>
      constexpr auto has_greater_equal_v = has_greater_equal<L, R>::value;

      template <class L, class R = L>
      using has_less_equal = is_invokable<std::less_equal<>, bool, L, R>;
      template <class L, class R = L>
      constexpr auto has_less_equal_v = has_less_equal<L, R>::value;
    }
  }
}

#endif /* SRC_CORE_IEC61131_CAST_HELPER_CPP_ */

