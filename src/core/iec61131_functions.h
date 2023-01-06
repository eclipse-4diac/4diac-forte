/*******************************************************************************
 * Copyright (c) 2010 - 2013 ACIN, Profactor GmbH, fortiss GmbH, 2018 TU Vienna/ACIN
 *               2022 Primetals Technologies Austria GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Alois Zoitl, Monika Wenger, Ingo Hegny, Patrick Smejkal, Gerhard Ebenhofer,
 *    Matthias Plasch, Martin Melik Merkumians
 *      - initial implementation and rework communication infrastructure
 *    Martin Melik Merkumians - adds different type templates for IEC 61131-3 functions
 *      and adds several type guards
 *    Martin Melik Merkumians - Reworks binary operator templates to create correctly
 *      calculated and typed results
 *    Martin Melik Merkumians - reworks and fixes ADD and SUB for time types
 *******************************************************************************/
#ifndef IEC61131_FUNCTIONS_H_
#define IEC61131_FUNCTIONS_H_

#include "../arch/devlog.h"
#include "convert_functions.h"
#include <math.h>
#include <string.h>
#include "iec61131_cast_helper.h"
#include "./utils/staticassert.h"
#include "./datatypes/forte_struct.h"
#include "./datatypes/forte_array_common.h"
#include "./datatypes/forte_array_fixed.h"
#include "./datatypes/forte_array_variable.h"

#include <algorithm>

#ifdef CONCAT
# undef CONCAT
#endif

#ifdef VXWORKS
#define tanf(x) static_cast<TForteFloat>(tan(x))
#define log10f(x) static_cast<TForteFloat>(log10(x))
#define logf(x) static_cast<TForteFloat>(log(x))
#define cosf(x) static_cast<TForteFloat>(cos(x))
#define atanf(x) static_cast<TForteFloat>(atan(x))
#define asinf(x) static_cast<TForteFloat>(asin(x))
#define acosf(x) static_cast<TForteFloat>(acos(x))
#define expf(x) static_cast<TForteFloat>(exp(x))
#define sinf(x) static_cast<TForteFloat>(sin(x))
#define sqrtf(x) static_cast<TForteFloat>(sqrt(x))
#endif

#define GENERATE_APPLY_FUNCTION(func)  template<typename T> struct func ##_Function { \
  static const T call(const T& paIN1, const T& paIN2){ \
      return func(paIN1, paIN2); \
    }; \
};

template<typename T>
const T func_ABS(const T &paIN) {
  static_assert((std::is_base_of<CIEC_ANY_NUM, T>::value), "T not of ANY_NUM");
  return (0 > static_cast<typename T::TValueType>(paIN)) ? T(static_cast<typename T::TValueType>(paIN) * static_cast<typename T::TValueType>(-1)) : paIN;
}

inline const CIEC_REAL func_SQRT(const CIEC_REAL &paIN){
  return CIEC_REAL(sqrtf(paIN));
}

inline const CIEC_LREAL func_SQRT(const CIEC_LREAL &paIN){
  return CIEC_LREAL(sqrt(paIN));
}

inline const CIEC_REAL func_LN(const CIEC_REAL &paIN){
  return CIEC_REAL(logf(paIN));
}

inline const CIEC_LREAL func_LN(const CIEC_LREAL &paIN){
  return CIEC_LREAL(log(paIN));
}

inline const CIEC_REAL func_LOG(const CIEC_REAL &paIN){
  return CIEC_REAL(log10f(paIN));
}

inline const CIEC_LREAL func_LOG(const CIEC_LREAL &paIN){
  return CIEC_LREAL(log10(paIN));
}

inline const CIEC_REAL func_SIN(const CIEC_REAL &paIN){
  return CIEC_REAL(sinf(paIN));
}

inline const CIEC_LREAL func_SIN(const CIEC_LREAL &paIN){
  return CIEC_LREAL(sin(paIN));
}

inline const CIEC_REAL func_COS(const CIEC_REAL &paIN){
  return CIEC_REAL(cosf(paIN));
}

inline const CIEC_LREAL func_COS(const CIEC_LREAL &paIN){
  return CIEC_LREAL(cos(paIN));
}

inline const CIEC_REAL func_TAN(const CIEC_REAL &paIN){
    return CIEC_REAL(tanf(paIN));
}

inline const CIEC_LREAL func_TAN(const CIEC_LREAL &paIN){
  return CIEC_LREAL(tan(paIN));
}

inline const CIEC_REAL func_ASIN(const CIEC_REAL &paIN){
  return CIEC_REAL(asinf(paIN));
}

inline const CIEC_LREAL func_ASIN(const CIEC_LREAL &paIN){
  return CIEC_LREAL(asin(paIN));
}

inline const CIEC_REAL func_ACOS(const CIEC_REAL &paIN){
  return CIEC_REAL(acosf(paIN));
}

inline const CIEC_LREAL func_ACOS(const CIEC_LREAL &paIN){
  return CIEC_LREAL(acos(paIN));
}

inline const CIEC_REAL func_ATAN(const CIEC_REAL &paIN){
    return CIEC_REAL(atanf(paIN));
}

inline const CIEC_LREAL func_ATAN(const CIEC_LREAL &paIN){
  return CIEC_LREAL(atan(paIN));
}

inline const CIEC_REAL func_EXP(const CIEC_REAL &paIN){
  return CIEC_REAL(expf(paIN));
}

inline const CIEC_LREAL func_EXP(const CIEC_LREAL &paIN){
  return CIEC_LREAL(exp(paIN));
}

template<typename T> const T func_ROL(const T &paIn, const CIEC_ANY_INT &paN) {
  static_assert((std::is_base_of<CIEC_ANY_BIT, T>::value), "T not of ANY_BIT");
  if((true == paN.isSigned() && 0 <= paN.getSignedValue()) || false == paN.isSigned()) {
    return T(static_cast<typename T::TValueType>((paIn << paN.getUnsignedValue()) | (paIn >> (sizeof(typename T::TValueType) * 8 - paN.getUnsignedValue()))));
  }
  DEVLOG_ERROR("value of input N is less than zero");
  return T(static_cast<typename T::TValueType>(0));
}

template<typename T> const T func_ROR(const T &paIn, const CIEC_ANY_INT &paN) {
  static_assert((std::is_base_of<CIEC_ANY_BIT, T>::value), "T not of ANY_BIT");
  if((true == paN.isSigned() && 0 <= paN.getSignedValue()) || false == paN.isSigned()) {
    return T(static_cast<typename T::TValueType>((paIn >> paN.getUnsignedValue()) | (paIn << (sizeof(typename T::TValueType) * 8 - paN.getUnsignedValue()))));
  }
  DEVLOG_ERROR("value of input N is less than zero");
  return T(static_cast<typename T::TValueType>(0));
}

template<typename T> const T func_SHL(const T &paIn, const CIEC_ANY_INT &paN) {
  static_assert((std::is_base_of<CIEC_ANY_BIT, T>::value), "T not of ANY_BIT");
  if((true == paN.isSigned() && 0 <= paN.getSignedValue()) || false == paN.isSigned()) {
    return T(static_cast<typename T::TValueType>(paIn << paN.getUnsignedValue()));
  }
  DEVLOG_ERROR("value of input N is less than zero");
  return T(static_cast<typename T::TValueType>(0));
}
template<typename T> const T func_SHR(const T &paIn, const CIEC_ANY_INT &paN) {
  static_assert((std::is_base_of<CIEC_ANY_BIT, T>::value), "T not of ANY_BIT");
  if((true == paN.isSigned() && 0 <= paN.getSignedValue()) || false == paN.isSigned()) {
    return T(static_cast<typename T::TValueType>(paIn >> paN.getUnsignedValue()));
  }
  DEVLOG_ERROR("value of input N is less than zero");
  return T(static_cast<typename T::TValueType>(0));
}

template<> const CIEC_BOOL func_ROL(const CIEC_BOOL &paIn, const CIEC_ANY_INT &paN);

template<> const CIEC_BOOL func_ROR(const CIEC_BOOL &paIn, const CIEC_ANY_INT &paN);

template<> const CIEC_BOOL func_SHL(const CIEC_BOOL &paIn, const CIEC_ANY_INT &paN);

template<> const CIEC_BOOL func_SHR(const CIEC_BOOL &paIn, const CIEC_ANY_INT &paN);

template<typename T, typename U, template<typename A> class F, typename C> typename forte::core::mpl::get_castable_type<T, U>::type APPLY(const T &paIN1,
    const U &paIN2) {
  static_assert((forte::core::mpl::are_of_subtype<C, T, U>::value), "Template instantiation with incompatible types");
  using tImplicitCastType = typename forte::core::mpl::get_castable_type<T, U>::type;
  static_assert(!(std::is_same<tImplicitCastType, forte::core::mpl::NullType>::value), "No implicit cast possible");
  const tImplicitCastType Result(F<tImplicitCastType>::call(static_cast<tImplicitCastType>(paIN1), static_cast<tImplicitCastType>(paIN2)));
  return Result;
}

template<class T, class U>
class AndOperation {
    using resultType = typename forte::core::mpl::get_castable_type<T, U>::type;
  public:
    static resultType call(const T &paIN1, const U &paIN2) {
      return resultType(static_cast<resultType>(paIN1) & static_cast<resultType>(paIN2));
    }
  private:
    AndOperation() = default;
};

template<class T>
class AndOperation<T, CIEC_BOOL> {
    using resultType = typename forte::core::mpl::get_castable_type<T, CIEC_BOOL>::type;
  public:
    static resultType call(const T &paIN1, const CIEC_BOOL &paIN2) {
      return resultType(paIN1 & (paIN2 ? 1 : 0));
    }
  private:
    AndOperation() = default;
};

template<class T>
class AndOperation<CIEC_BOOL, T> {
    using resultType = typename forte::core::mpl::get_castable_type<T, CIEC_BOOL>::type;
  public:
    static resultType call(const CIEC_BOOL &paIN1, const T &paIN2) {
      return resultType((paIN1 ? 1 : 0) & paIN2);
    }
  private:
    AndOperation() = default;
};

template<>
class AndOperation<CIEC_BOOL, CIEC_BOOL> {
  public:
    static CIEC_BOOL call(const CIEC_BOOL &paIN1, const CIEC_BOOL &paIN2) {
      return CIEC_BOOL(paIN1 && paIN2);
    }
  private:
    AndOperation() = default;
};

template<typename T, typename U>
typename forte::core::mpl::get_castable_type<typename forte::core::mpl::get_equivalent_CIEC_class<T>::type, typename forte::core::mpl::get_equivalent_CIEC_class<U>::type>::type func_AND(const T &paIN1, const U &paIN2) {
    using tTClass = typename forte::core::mpl::get_equivalent_CIEC_class<T>::type;
    using tUClass = typename forte::core::mpl::get_equivalent_CIEC_class<U>::type;
    static_assert((forte::core::mpl::are_of_subtype<CIEC_ANY_BIT, tTClass, tUClass>::value), "Template instantiation with incompatible types");
    using tImplicitCastType = typename forte::core::mpl::get_castable_type<tTClass, tUClass>::type;
    static_assert(!(std::is_same<tImplicitCastType, forte::core::mpl::NullType>::value), "No implicit cast possible");
    return AndOperation<tTClass, tUClass>::call(paIN1, paIN2);
}

template<class T, class U>
class OrOperation {
    using resultType = typename forte::core::mpl::get_castable_type<T, U>::type;
  public:
    static resultType call(const T &paIN1, const U &paIN2) {
      return resultType(static_cast<resultType>(paIN1) | static_cast<resultType>(paIN2));
    }
  private:
    OrOperation() = default;
};

template<class T>
class OrOperation<T, CIEC_BOOL> {
    using resultType = typename forte::core::mpl::get_castable_type<T, CIEC_BOOL>::type;
  public:
    static resultType call(const T &paIN1, const CIEC_BOOL &paIN2) {
      return resultType(paIN1 | (paIN2 ? 1 : 0));
    }
  private:
    OrOperation() = default;
};

template<class T>
class OrOperation<CIEC_BOOL, T> {
    using resultType = typename forte::core::mpl::get_castable_type<T, CIEC_BOOL>::type;
  public:
    static resultType call(const CIEC_BOOL &paIN1, const T &paIN2) {
      return resultType((paIN1 ? 1 : 0) | paIN2);
    }
  private:
    OrOperation() = default;
};

template<>
class OrOperation<CIEC_BOOL, CIEC_BOOL> {
  public:
    static CIEC_BOOL call(const CIEC_BOOL &paIN1, const CIEC_BOOL &paIN2) {
      return CIEC_BOOL(paIN1 || paIN2);
    }
  private:
    OrOperation() = default;
};


template<typename T, typename U>
typename forte::core::mpl::get_castable_type<typename forte::core::mpl::get_equivalent_CIEC_class<T>::type, typename forte::core::mpl::get_equivalent_CIEC_class<U>::type>::type func_OR(const T &paIN1, const U &paIN2) {
  using tTClass = typename forte::core::mpl::get_equivalent_CIEC_class<T>::type;
  using tUClass = typename forte::core::mpl::get_equivalent_CIEC_class<U>::type;
  static_assert((forte::core::mpl::are_of_subtype<CIEC_ANY_BIT, tTClass, tUClass>::value), "Template instantiation with incompatible types");
  using tImplicitCastType = typename forte::core::mpl::get_castable_type<tTClass, tUClass>::type;
  static_assert(!(std::is_same<tImplicitCastType, forte::core::mpl::NullType>::value), "No implicit cast possible");
  return OrOperation<tTClass, tUClass>::call(paIN1, paIN2);
}
template<class T, class U>
class XorOperation {
  using resultType = typename forte::core::mpl::get_castable_type<T, U>::type;
public:
  static resultType call(const T &paIN1, const U &paIN2)
  {
      return resultType(static_cast<resultType>(paIN1) ^ static_cast<resultType>(paIN2));
    }
  private:
    XorOperation() = default;
};

template<class T>
class XorOperation<T, CIEC_BOOL> {
  using resultType = typename forte::core::mpl::get_castable_type<T, CIEC_BOOL>::type;
  public:
    static resultType call(const T &paIN1, const CIEC_BOOL &paIN2) {
      return resultType(paIN1 ^ (paIN2 ? 1 : 0));
    }
  private:
    XorOperation() = default;
};

template<class T>
class XorOperation<CIEC_BOOL, T> {
    using resultType = typename forte::core::mpl::get_castable_type<T, CIEC_BOOL>::type;
  public:
    static resultType call(const CIEC_BOOL &paIN1, const T &paIN2) {
      return resultType((paIN1 ? 1 : 0) ^ paIN2);
    }
  private:
    XorOperation() = default;
};

template<>
class XorOperation<CIEC_BOOL, CIEC_BOOL> {
  public:
    static CIEC_BOOL call(const CIEC_BOOL &paIN1, const CIEC_BOOL &paIN2) {
      return CIEC_BOOL(paIN1 != paIN2);
    }
  private:
    XorOperation() = default;
};

template<typename T, typename U>
typename forte::core::mpl::get_castable_type<typename forte::core::mpl::get_equivalent_CIEC_class<T>::type, typename forte::core::mpl::get_equivalent_CIEC_class<U>::type>::type func_XOR(const T &paIN1, const U &paIN2) {
  using tTClass = typename forte::core::mpl::get_equivalent_CIEC_class<T>::type ;
  using tUClass = typename forte::core::mpl::get_equivalent_CIEC_class<U>::type;
  static_assert((forte::core::mpl::are_of_subtype<CIEC_ANY_BIT, tTClass, tUClass>::value), "Template instantiation with incompatible types");
  using tImplicitCastType = typename forte::core::mpl::get_castable_type<tTClass, tUClass>::type;
  static_assert(!(std::is_same<tImplicitCastType, forte::core::mpl::NullType>::value), "No implicit cast possible");
  return XorOperation<tTClass, tUClass>::call(paIN1, paIN2);
}

template <typename T>
const typename forte::core::mpl::get_equivalent_CIEC_class<T>::type func_NOT(const T& paIN) {
  static_assert((std::is_base_of<CIEC_ANY_BIT, T>::value), "NOT function only for ANY_BIT");
  using tResultType = typename forte::core::mpl::get_equivalent_CIEC_class<T>::type;
  if constexpr (std::is_same<tResultType, CIEC_BOOL>::value) {
    return tResultType(!paIN);
  } else {
    return tResultType(~paIN);
  }
}

template<typename T, typename U> const CIEC_BOOL func_GT(const T &paIN1, const U &paIN2) {
  static_assert((forte::core::mpl::are_of_subtype<CIEC_ANY_ELEMENTARY, T, U>::value), "Template instantiation with incompatible types");
  if constexpr (forte::core::mpl::are_of_subtype<CIEC_ANY_BIT, T, U>::value) { //ANY_BITs can be also partial accesses
    using tTClass = typename forte::core::mpl::get_equivalent_CIEC_class<T>::type ;
    using tUClass = typename forte::core::mpl::get_equivalent_CIEC_class<U>::type;
    using tImplicitCastType = typename forte::core::mpl::get_castable_type<tTClass, tUClass>::type;
    static_assert(!(std::is_same<tImplicitCastType, forte::core::mpl::NullType>::value), "No implicit cast possible");
    return func_GT<tImplicitCastType>(static_cast<tImplicitCastType>(paIN1), static_cast<tImplicitCastType>(paIN2));
  } else {
    using tImplicitCastType = typename forte::core::mpl::get_castable_type<T, U>::type;
    static_assert(!(std::is_same<tImplicitCastType, forte::core::mpl::NullType>::value), "No implicit cast possible");
    return func_GT<tImplicitCastType>(static_cast<tImplicitCastType>(paIN1), static_cast<tImplicitCastType>(paIN2));
  }
}

template<typename T> const CIEC_BOOL func_GT(const T &paIN1, const T &paIN2) {
  if constexpr (forte::core::mpl::has_greater_v<T>) {
    return CIEC_BOOL(paIN1 > paIN2);
  } else {
    return CIEC_BOOL(static_cast<typename T::TValueType>(paIN1) > static_cast<typename T::TValueType>(paIN2));
  }
}

template<typename T, typename U> const CIEC_BOOL func_EQ(const T &paIN1, const U &paIN2) {
  if constexpr (forte::core::mpl::are_of_subtype<CIEC_ANY_BIT, T, U>::value) { //ANY_BITs can be also partial accesses
    using tTClass = typename forte::core::mpl::get_equivalent_CIEC_class<T>::type;
    using tUClass = typename forte::core::mpl::get_equivalent_CIEC_class<U>::type;
    using tImplicitCastType = typename forte::core::mpl::get_castable_type<tTClass, tUClass>::type ;
    static_assert(!(std::is_same<tImplicitCastType, forte::core::mpl::NullType>::value), "No implicit cast possible");
    return func_EQ<tImplicitCastType>(static_cast<tImplicitCastType>(paIN1), static_cast<tImplicitCastType>(paIN2));
  } else {
    using tImplicitCastType = typename forte::core::mpl::get_castable_type<T, U>::type;
    static_assert(!(std::is_same<tImplicitCastType, forte::core::mpl::NullType>::value), "No implicit cast possible");
    return func_EQ<tImplicitCastType>(static_cast<tImplicitCastType>(paIN1), static_cast<tImplicitCastType>(paIN2));
  }
}

template<typename T> const CIEC_BOOL func_EQ(const T &paIN1, const T &paIN2) {
  if constexpr (forte::core::mpl::has_equality_v<T>) {
    return CIEC_BOOL(paIN1 == paIN2);
  } else {
    return CIEC_BOOL(static_cast<typename T::TValueType>(paIN1) == static_cast<typename T::TValueType>(paIN2));
  }
}

template<typename T, typename U> const CIEC_BOOL func_GE(const T &paIN1, const U &paIN2) {
  static_assert((forte::core::mpl::are_of_subtype<CIEC_ANY_ELEMENTARY, T, U>::value), "Template instantiation with incompatible types");
  if constexpr (forte::core::mpl::are_of_subtype<CIEC_ANY_BIT, T, U>::value) { //ANY_BITs can be also partial accesses
    using tTClass = typename forte::core::mpl::get_equivalent_CIEC_class<T>::type;
    using tUClass = typename forte::core::mpl::get_equivalent_CIEC_class<U>::type;
    using tImplicitCastType = typename forte::core::mpl::get_castable_type<tTClass, tUClass>::type ;
    static_assert(!(std::is_same<tImplicitCastType, forte::core::mpl::NullType>::value), "No implicit cast possible");
    return func_GE<tImplicitCastType>(static_cast<tImplicitCastType>(paIN1), static_cast<tImplicitCastType>(paIN2));
  } else {
    using tImplicitCastType = typename forte::core::mpl::get_castable_type<T, U>::type;
    static_assert(!(std::is_same<tImplicitCastType, forte::core::mpl::NullType>::value), "No implicit cast possible");
    return func_GE<tImplicitCastType>(static_cast<tImplicitCastType>(paIN1), static_cast<tImplicitCastType>(paIN2));
  }
}

template<typename T> const CIEC_BOOL func_GE(const T &paIN1, const T &paIN2) {
  if constexpr (forte::core::mpl::has_greater_equal_v<T>) {
    return CIEC_BOOL(paIN1 >= paIN2);
  } else {
    return CIEC_BOOL(static_cast<typename T::TValueType>(paIN1) >= static_cast<typename T::TValueType>(paIN2));
  }
}

template<typename T, typename U> const CIEC_BOOL func_LE(const T &paIN1, const U &paIN2) {
  static_assert((forte::core::mpl::are_of_subtype<CIEC_ANY_ELEMENTARY, T, U>::value), "Template instantiation with incompatible types");
  if constexpr (forte::core::mpl::are_of_subtype<CIEC_ANY_BIT, T, U>::value) { //ANY_BITs can be also partial accesses
    using tTClass = typename forte::core::mpl::get_equivalent_CIEC_class<T>::type;
    using tUClass = typename forte::core::mpl::get_equivalent_CIEC_class<U>::type;
    using tImplicitCastType = typename forte::core::mpl::get_castable_type<tTClass, tUClass>::type;
    static_assert(!(std::is_same<tImplicitCastType, forte::core::mpl::NullType>::value), "No implicit cast possible");
    return func_LE<tImplicitCastType>(static_cast<tImplicitCastType>(paIN1), static_cast<tImplicitCastType>(paIN2));
  } else {
    using tImplicitCastType = typename forte::core::mpl::get_castable_type<T, U>::type;
    static_assert(!(std::is_same<tImplicitCastType, forte::core::mpl::NullType>::value), "No implicit cast possible");
    return func_LE<tImplicitCastType>(static_cast<tImplicitCastType>(paIN1), static_cast<tImplicitCastType>(paIN2));
  }
}

template<typename T> const CIEC_BOOL func_LE(const T &paIN1, const T &paIN2) {
  if constexpr (forte::core::mpl::has_less_equal_v<T>) {
    return CIEC_BOOL(paIN1 <= paIN2);
  } else {
    return CIEC_BOOL(static_cast<typename T::TValueType>(paIN1) <= static_cast<typename T::TValueType>(paIN2));
  }
}

template<typename T, typename U> const CIEC_BOOL func_LT(const T &paIN1, const U &paIN2) {
  static_assert((forte::core::mpl::are_of_subtype<CIEC_ANY_ELEMENTARY, T, U>::value), "Template instantiation with incompatible types");
  if constexpr (forte::core::mpl::are_of_subtype<CIEC_ANY_BIT, T, U>::value) { //ANY_BITs can be also partial accesses
    using tTClass = typename forte::core::mpl::get_equivalent_CIEC_class<T>::type;
    using tUClass = typename forte::core::mpl::get_equivalent_CIEC_class<U>::type;
    using tImplicitCastType = typename forte::core::mpl::get_castable_type<tTClass, tUClass>::type;
    static_assert(!(std::is_same<tImplicitCastType, forte::core::mpl::NullType>::value), "No implicit cast possible");
    return func_LT<tImplicitCastType>(static_cast<tImplicitCastType>(paIN1), static_cast<tImplicitCastType>(paIN2));
  } else {
    using tImplicitCastType = typename forte::core::mpl::get_castable_type<T, U>::type;
    static_assert(!(std::is_same<tImplicitCastType, forte::core::mpl::NullType>::value), "No implicit cast possible");
    return func_LT<tImplicitCastType>(static_cast<tImplicitCastType>(paIN1), static_cast<tImplicitCastType>(paIN2));
  }
}

template<typename T> const CIEC_BOOL func_LT(const T &paIN1, const T &paIN2) {
  if constexpr (forte::core::mpl::has_less_v<T>) {
    return CIEC_BOOL(paIN1 < paIN2);
  } else {
    return CIEC_BOOL(static_cast<typename T::TValueType>(paIN1) < static_cast<typename T::TValueType>(paIN2));
  }
}

template<typename T, typename U> const CIEC_BOOL func_NE(const T &paIN1, const U &paIN2) {
  if constexpr (forte::core::mpl::are_of_subtype<CIEC_ANY_BIT, T, U>::value) { //ANY_BITs can be also partial accesses
    using tTClass = typename forte::core::mpl::get_equivalent_CIEC_class<T>::type;
    using tUClass = typename forte::core::mpl::get_equivalent_CIEC_class<U>::type;
    using tImplicitCastType = typename forte::core::mpl::get_castable_type<tTClass, tUClass>::type ;
    static_assert(!(std::is_same<tImplicitCastType, forte::core::mpl::NullType>::value), "No implicit cast possible");
    return func_NE<tImplicitCastType>(static_cast<tImplicitCastType>(paIN1), static_cast<tImplicitCastType>(paIN2));
  } else {
    using tImplicitCastType = typename forte::core::mpl::get_castable_type<T, U>::type;
    static_assert(!(std::is_same<tImplicitCastType, forte::core::mpl::NullType>::value), "No implicit cast possible");
    return func_NE<tImplicitCastType>(static_cast<tImplicitCastType>(paIN1), static_cast<tImplicitCastType>(paIN2));
  }
}

template<typename T> const CIEC_BOOL func_NE(const T &paIN1, const T &paIN2) {
  if constexpr (forte::core::mpl::has_inequality_v<T>) {
    return CIEC_BOOL(paIN1 != paIN2);
  } else {
    return CIEC_BOOL(static_cast<typename T::TValueType>(paIN1) != static_cast<typename T::TValueType>(paIN2));
  }
}

template<typename T, typename U> const typename forte::core::mpl::get_castable_type<T, U>::type func_SEL(const CIEC_BOOL &G, const T &IN0, const U &IN1) {
  return (G.operator bool()) ? IN1 : IN0;
}

GENERATE_APPLY_FUNCTION(func_MAX)
template<typename T, typename U> typename forte::core::mpl::get_castable_type<T, U>::type func_MAX(const T &paIN1, const U &paIN2) {
  return APPLY<T, U, func_MAX_Function, CIEC_ANY_ELEMENTARY>(paIN1, paIN2);
}

template<typename T> const T func_MAX(const T &paIN1, const T &paIN2) {
  if(static_cast<typename T::TValueType>(paIN1) > static_cast<typename T::TValueType>(paIN2)) {
    return paIN1;
  } else {
    return paIN2;
  }
}

GENERATE_APPLY_FUNCTION(func_MIN)
template<typename T, typename U> typename forte::core::mpl::get_castable_type<T, U>::type func_MIN(const T &paIN1, const U &paIN2) {
  return APPLY<T, U, func_MIN_Function, CIEC_ANY_ELEMENTARY>(paIN1, paIN2);
}

template<typename T> const T func_MIN(const T &paIN1, const T &paIN2) {
  if(static_cast<typename T::TValueType>(paIN1) < static_cast<typename T::TValueType>(paIN2)) {
    return paIN1;
  } else {
    return paIN2;
  }
}

template<typename T, typename U, typename V>
const typename forte::core::mpl::get_castable_type<typename forte::core::mpl::get_castable_type<T, U>::type, V>::type func_LIMIT(const T &pa_roMN, const U &paIN,
    const V &pa_roMX) {
  using tImplicitType = typename forte::core::mpl::get_castable_type<typename forte::core::mpl::get_castable_type<T, U>::type, V>::type;
  return func_MIN(func_MAX(static_cast<tImplicitType>(paIN), static_cast<tImplicitType>(pa_roMN)), static_cast<tImplicitType>(pa_roMX));
}

GENERATE_APPLY_FUNCTION(func_MOD)
template<typename T, typename U> typename forte::core::mpl::get_castable_type<T, U>::type func_MOD(const T &paIN1, const U &paIN2) {
  return APPLY<T, U, func_MOD_Function, CIEC_ANY_INT>(paIN1, paIN2);
}

template<typename T> const T func_MOD(const T &paIN1, const T &paIN2) {
  if(0 == static_cast<typename T::TValueType>(paIN2)) {
    return T(0);
  }
  return T(paIN1.getSignedValue() % paIN2.getSignedValue());
}
template<typename T> const T func_MOVE(const T &paIN) {
  return T(paIN);
}

template<typename T, typename U> const T func_EXPT(const T &paIN1, const U &paIN2) {
  static_assert((std::is_base_of<CIEC_ANY_REAL, T>::value), "IN1 not of type ANY_REAL");
  static_assert((std::is_base_of<CIEC_ANY_NUM, U>::value), "IN2 not of type ANY_NUM");
  return T(pow(paIN1, paIN2));
}

//specialisation of function for base type double.
//TODO consider how this and similar problems can be better solved with type traits and more generically
inline double func_EXPT(double paIN1, double paIN2) {
  return pow(paIN1, paIN2);
}

/*************** ADD ********************/

const CIEC_TIME func_ADD_TIME(const CIEC_TIME &paIN1, const CIEC_TIME &paIN2);
const CIEC_TIME_OF_DAY func_ADD_TOD_TIME(const CIEC_TIME_OF_DAY &paIN1, const CIEC_TIME &paIN2);
const CIEC_DATE_AND_TIME func_ADD_DT_TIME(const CIEC_DATE_AND_TIME &paIN1, const CIEC_TIME &paIN2);

const CIEC_LTIME func_ADD_LTIME(const CIEC_LTIME &paIN1, const CIEC_LTIME &paIN2);
const CIEC_LTIME_OF_DAY func_ADD_LTOD_LTIME(const CIEC_LTIME_OF_DAY &paIN1, const CIEC_LTIME &paIN2);
const CIEC_LDATE_AND_TIME func_ADD_LDT_LTIME(const CIEC_LDATE_AND_TIME &paIN1, const CIEC_LTIME &paIN2);

template <class T, class U>
class AddOperation {
  using resultType = typename forte::core::mpl::get_add_operator_result_type<T, U>::type;

public:
  static resultType call(const T &paIN1, const U &paIN2) {
    return resultType(static_cast<typename T::TValueType>(paIN1) + static_cast<typename U::TValueType>(paIN2));
  }
  AddOperation() = delete;
};

template <>
class AddOperation<CIEC_TIME_OF_DAY, CIEC_TIME> {
  using resultType = typename forte::core::mpl::get_add_operator_result_type<CIEC_TIME_OF_DAY, CIEC_TIME>::type;

public:
  static resultType call(const CIEC_TIME_OF_DAY &paIN1, const CIEC_TIME &paIN2) {
    return func_ADD_TOD_TIME(paIN1, paIN2);
  }
  AddOperation() = delete;
};

template <>
class AddOperation<CIEC_DATE_AND_TIME, CIEC_TIME> {
  using resultType = typename forte::core::mpl::get_add_operator_result_type<CIEC_DATE_AND_TIME, CIEC_TIME>::type;

public:
  static resultType call(const CIEC_DATE_AND_TIME &paIN1, const CIEC_TIME &paIN2) {
    return func_ADD_DT_TIME(paIN1, paIN2);
  }
  AddOperation() = delete;
};

template <>
class AddOperation<CIEC_LTIME_OF_DAY, CIEC_LTIME> {
  using resultType = typename forte::core::mpl::get_add_operator_result_type<CIEC_LTIME_OF_DAY, CIEC_LTIME>::type;

public:
  static resultType call(const CIEC_LTIME_OF_DAY &paIN1, const CIEC_LTIME &paIN2) {
    return func_ADD_LTOD_LTIME(paIN1, paIN2);
  }
  AddOperation() = delete;
};

template <>
class AddOperation<CIEC_LDATE_AND_TIME, CIEC_LTIME> {
  using resultType = typename forte::core::mpl::get_add_operator_result_type<CIEC_LDATE_AND_TIME, CIEC_LTIME>::type;

public:
  static resultType call(const CIEC_LDATE_AND_TIME &paIN1, const CIEC_LTIME &paIN2) {
    return func_ADD_LDT_LTIME(paIN1, paIN2);
  }
  AddOperation() = delete;
};

template <typename R = forte::core::mpl::NullType, typename T, typename U>
auto func_ADD(const T &paIN1, const U &paIN2) -> typename forte::core::mpl::get_add_operator_result_type<T, U>::type {
  using deductedType = typename forte::core::mpl::get_add_operator_result_type<T, U>::type;
  if constexpr (!std::is_same<R, forte::core::mpl::NullType>::value) {
    static_assert(std::is_same<deductedType, R>::value, "Deducted type and requested type do not match!\n");
  }
  return AddOperation<T, U>::call(paIN1, paIN2);
}

/*************** MUL ********************/
template <class T, class U>
class MulOperation {
  using resultType = typename forte::core::mpl::get_mul_operator_result_type<T, U>::type;

public:
  static resultType call(const T &paIN1, const U &paIN2) {
    return resultType(static_cast<typename T::TValueType>(paIN1) * static_cast<typename U::TValueType>(paIN2));
  }
  MulOperation() = delete;
};

template <class U>
class MulOperation<CIEC_TIME, U> {
  using resultType = typename forte::core::mpl::get_mul_operator_result_type<CIEC_TIME, U>::type;

public:
  static resultType call(const CIEC_TIME &paIN1, const U &paIN2) {
    return func_MUL_TIME(paIN1, paIN2);
  }
  MulOperation() = delete;
};

template <class U>
class MulOperation<CIEC_LTIME, U> {
  using resultType = typename forte::core::mpl::get_mul_operator_result_type<CIEC_LTIME, U>::type;

public:
  static resultType call(const CIEC_LTIME &paIN1, const U &paIN2) {
    return func_MUL_LTIME(paIN1, paIN2);
  }
  MulOperation() = delete;
};

template <typename R = forte::core::mpl::NullType, typename T, typename U>
auto func_MUL(const T &paIN1, const U &paIN2) -> typename forte::core::mpl::get_mul_operator_result_type<T, U>::type {
  using deductedType = typename forte::core::mpl::get_mul_operator_result_type<T, U>::type;
  if constexpr (!std::is_same<R, forte::core::mpl::NullType>::value) {
    static_assert(std::is_same<deductedType, R>::value, "Deducted type and requested type do not match!\n");
  }
  return MulOperation<T, U>::call(paIN1, paIN2);
}

/*********** SUB *************/
const CIEC_TIME func_SUB_TIME(const CIEC_TIME &paIN1, const CIEC_TIME &paIN2);
const CIEC_TIME_OF_DAY func_SUB_TOD_TIME(const CIEC_TIME_OF_DAY &paIN1, const CIEC_TIME &paIN2);
const CIEC_DATE_AND_TIME func_SUB_DT_TIME(const CIEC_DATE_AND_TIME &paIN1, const CIEC_TIME &paIN2);
const CIEC_TIME func_SUB_DT_DT(const CIEC_DATE_AND_TIME &paIN1, const CIEC_DATE_AND_TIME &paIN2);
const CIEC_TIME func_SUB_TOD_TOD(const CIEC_TIME_OF_DAY &paIN1, const CIEC_TIME_OF_DAY &paIN2);
const CIEC_TIME func_SUB_DATE_DATE(const CIEC_DATE &paIN1, const CIEC_DATE &paIN2);

const CIEC_LTIME func_SUB_LTIME(const CIEC_LTIME &paIN1, const CIEC_LTIME &paIN2);
const CIEC_LTIME_OF_DAY func_SUB_LTOD_LTIME(const CIEC_LTIME_OF_DAY &paIN1, const CIEC_LTIME &paIN2);
const CIEC_LDATE_AND_TIME func_SUB_LDT_LTIME(const CIEC_LDATE_AND_TIME &paIN1, const CIEC_LTIME &paIN2);
const CIEC_LTIME func_SUB_LDT_LDT(const CIEC_LDATE_AND_TIME &paIN1, const CIEC_LDATE_AND_TIME &paIN2);
const CIEC_LTIME func_SUB_LTOD_LTOD(const CIEC_LTIME_OF_DAY &paIN1, const CIEC_LTIME_OF_DAY &paIN2);
const CIEC_LTIME func_SUB_LDATE_LDATE(const CIEC_LDATE &paIN1, const CIEC_LDATE &paIN2);

template <class T, class U>
class SubOperation {
  using resultType = typename forte::core::mpl::get_sub_operator_result_type<T, U>::type;

public:
  static resultType call(const T &paIN1, const U &paIN2) {
    return resultType(static_cast<typename resultType::TValueType>(paIN1) - static_cast<typename resultType::TValueType>(paIN2));
  }
  SubOperation() = delete;
};

template <>
class SubOperation<CIEC_TIME_OF_DAY, CIEC_TIME> {
  using resultType = typename forte::core::mpl::get_sub_operator_result_type<CIEC_TIME_OF_DAY, CIEC_TIME>::type;

public:
  static resultType call(const CIEC_TIME_OF_DAY &paIN1, const CIEC_TIME &paIN2) {
    return func_SUB_TOD_TIME(paIN1, paIN2);
  }
  SubOperation() = delete;
};

template <>
class SubOperation<CIEC_DATE_AND_TIME, CIEC_TIME> {
  using resultType = typename forte::core::mpl::get_sub_operator_result_type<CIEC_DATE_AND_TIME, CIEC_TIME>::type;

public:
  static resultType call(const CIEC_DATE_AND_TIME &paIN1, const CIEC_TIME &paIN2) {
    return func_SUB_DT_TIME(paIN1, paIN2);
  }
  SubOperation() = delete;
};

template <>
class SubOperation<CIEC_DATE_AND_TIME, CIEC_DATE_AND_TIME> {
  using resultType = typename forte::core::mpl::get_sub_operator_result_type<CIEC_DATE_AND_TIME, CIEC_DATE_AND_TIME>::type;

public:
  static resultType call(const CIEC_DATE_AND_TIME &paIN1, const CIEC_DATE_AND_TIME &paIN2) {
    return func_SUB_DT_DT(paIN1, paIN2);
  }
  SubOperation() = delete;
};

template <>
class SubOperation<CIEC_TIME_OF_DAY, CIEC_TIME_OF_DAY> {
  using resultType = typename forte::core::mpl::get_sub_operator_result_type<CIEC_TIME_OF_DAY, CIEC_TIME_OF_DAY>::type;

public:
  static resultType call(const CIEC_TIME_OF_DAY &paIN1, const CIEC_TIME_OF_DAY &paIN2) {
    return func_SUB_TOD_TOD(paIN1, paIN2);
  }
  SubOperation() = delete;
};

template <>
class SubOperation<CIEC_DATE, CIEC_DATE> {
  using resultType = typename forte::core::mpl::get_sub_operator_result_type<CIEC_DATE, CIEC_DATE>::type;

public:
  static resultType call(const CIEC_DATE &paIN1, const CIEC_DATE &paIN2) {
    return func_SUB_DATE_DATE(paIN1, paIN2);
  }
  SubOperation() = delete;
};

template <>
class SubOperation<CIEC_LTIME_OF_DAY, CIEC_LTIME> {
  using resultType = typename forte::core::mpl::get_sub_operator_result_type<CIEC_LTIME_OF_DAY, CIEC_LTIME>::type;

public:
  static resultType call(const CIEC_LTIME_OF_DAY &paIN1, const CIEC_LTIME &paIN2) {
    return func_SUB_LTOD_LTIME(paIN1, paIN2);
  }
  SubOperation() = delete;
};

template <>
class SubOperation<CIEC_LDATE_AND_TIME, CIEC_LTIME> {
  using resultType = typename forte::core::mpl::get_sub_operator_result_type<CIEC_LDATE_AND_TIME, CIEC_LTIME>::type;

public:
  static resultType call(const CIEC_LDATE_AND_TIME &paIN1, const CIEC_LTIME &paIN2) {
    return func_SUB_LDT_LTIME(paIN1, paIN2);
  }
  SubOperation() = delete;
};

template <>
class SubOperation<CIEC_LDATE_AND_TIME, CIEC_LDATE_AND_TIME> {
  using resultType = typename forte::core::mpl::get_sub_operator_result_type<CIEC_LDATE_AND_TIME, CIEC_LDATE_AND_TIME>::type;

public:
  static resultType call(const CIEC_LDATE_AND_TIME &paIN1, const CIEC_LDATE_AND_TIME &paIN2) {
    return func_SUB_LDT_LDT(paIN1, paIN2);
  }
  SubOperation() = delete;
};

template <>
class SubOperation<CIEC_LTIME_OF_DAY, CIEC_LTIME_OF_DAY> {
  using resultType = typename forte::core::mpl::get_sub_operator_result_type<CIEC_LTIME_OF_DAY, CIEC_LTIME_OF_DAY>::type;

public:
  static resultType call(const CIEC_LTIME_OF_DAY &paIN1, const CIEC_LTIME_OF_DAY &paIN2) {
    return func_SUB_LTOD_LTOD(paIN1, paIN2);
  }
  SubOperation() = delete;
};

template <>
class SubOperation<CIEC_LDATE, CIEC_LDATE> {
  using resultType = typename forte::core::mpl::get_sub_operator_result_type<CIEC_LDATE, CIEC_LDATE>::type;

public:
  static resultType call(const CIEC_LDATE &paIN1, const CIEC_LDATE &paIN2) {
    return func_SUB_LDATE_LDATE(paIN1, paIN2);
  }
  SubOperation() = delete;
};

template <typename R = forte::core::mpl::NullType, typename T, typename U>
auto func_SUB(const T &paIN1, const U &paIN2) -> typename forte::core::mpl::get_sub_operator_result_type<T, U>::type {
  using deductedType = typename forte::core::mpl::get_sub_operator_result_type<T, U>::type;
  if constexpr (!std::is_same<R, forte::core::mpl::NullType>::value) {
    static_assert(std::is_same<deductedType, R>::value, "Deducted type and requested type do not match!\n");
  }
  return SubOperation<T, U>::call(paIN1, paIN2);
}

template <class T, class U>
class DivOperation {
  using resultType = typename forte::core::mpl::get_castable_type<T, U>::type;

public:
  static resultType call(const T &paIN1, const U &paIN2) {
    if(0 != static_cast<typename U::TValueType>(paIN2)) {
      return resultType(static_cast<typename T::TValueType>(paIN1) / static_cast<typename U::TValueType>(paIN2));
    }
    DEVLOG_ERROR("Division by zero!\n");
    return resultType(0); // Divisor is 0
  }
  DivOperation() = delete;
};

// Time division only works with TIME as dividend
template <class U>
class DivOperation<CIEC_TIME, U> {
  using resultType = typename forte::core::mpl::get_div_operator_result_type<CIEC_TIME, U>::type;

public:
  static resultType call(const CIEC_TIME &paIN1, const U &paIN2) {
    return func_DIV_TIME(paIN1, paIN2);
  }
  DivOperation() = delete;
};

template <class U>
class DivOperation<CIEC_LTIME, U> {
  using resultType =  typename forte::core::mpl::get_div_operator_result_type<CIEC_LTIME, U>::type;

public:
  static resultType call(const CIEC_LTIME &paIN1, const U &paIN2) {
    return func_DIV_LTIME(paIN1, paIN2);
  }
  DivOperation() = delete;
};


template <typename R = forte::core::mpl::NullType, typename T, typename U>
typename forte::core::mpl::get_div_operator_result_type<T, U>::type func_DIV(const T &paIN1, const U &paIN2) {
  using deductedType = typename forte::core::mpl::get_div_operator_result_type<T, U>::type;
  if constexpr (!std::is_same<R, forte::core::mpl::NullType>::value) {
    static_assert(std::is_same<deductedType, R>::value, "Deducted type and requested type do not match!\n");
  }
  return DivOperation<T, U>::call(paIN1, paIN2);
}

template <typename T>
T func_MINUS(const T& paIN) {
  return paIN.operator-();
}

template <typename T>
const T func_TRUNC(const CIEC_REAL &paIN) {
  return T(static_cast<typename T::TValueType>(static_cast<TForteInt32>(paIN)));
}

const CIEC_SINT func_TRUNC_SINT(const CIEC_REAL &paIN);
const CIEC_INT func_TRUNC_INT(const CIEC_REAL &paIN);
const CIEC_DINT func_TRUNC_DINT(const CIEC_REAL &paIN);
const CIEC_LINT func_TRUNC_LINT(const CIEC_REAL &paIN);
const CIEC_USINT func_TRUNC_USINT(const CIEC_REAL &paIN);
const CIEC_UINT func_TRUNC_UINT(const CIEC_REAL &paIN);
const CIEC_UDINT func_TRUNC_UDINT(const CIEC_REAL &paIN);
const CIEC_ULINT func_TRUNC_ULINT(const CIEC_REAL &paIN);

const CIEC_SINT func_REAL_TRUNC_SINT(const CIEC_REAL &paIN);
const CIEC_INT func_REAL_TRUNC_INT(const CIEC_REAL &paIN);
const CIEC_DINT func_REAL_TRUNC_DINT(const CIEC_REAL &paIN);
const CIEC_LINT func_REAL_TRUNC_LINT(const CIEC_REAL &paIN);
const CIEC_USINT func_REAL_TRUNC_USINT(const CIEC_REAL &paIN);
const CIEC_UINT func_REAL_TRUNC_UINT(const CIEC_REAL &paIN);
const CIEC_UDINT func_REAL_TRUNC_UDINT(const CIEC_REAL &paIN);
const CIEC_ULINT func_REAL_TRUNC_ULINT(const CIEC_REAL &paIN);

template <typename T>
const T func_TRUNC(const CIEC_LREAL &paIN) {
  return T(static_cast<typename T::TValueType>(static_cast<TForteInt64>(paIN)));
}

const CIEC_SINT func_TRUNC_SINT(const CIEC_LREAL &paIN);
const CIEC_INT func_TRUNC_INT(const CIEC_LREAL &paIN);
const CIEC_DINT func_TRUNC_DINT(const CIEC_LREAL &paIN);
const CIEC_LINT func_TRUNC_LINT(const CIEC_LREAL &paIN);
const CIEC_USINT func_TRUNC_USINT(const CIEC_LREAL &paIN);
const CIEC_UINT func_TRUNC_UINT(const CIEC_LREAL &paIN);
const CIEC_UDINT func_TRUNC_UDINT(const CIEC_LREAL &paIN);
const CIEC_ULINT func_TRUNC_ULINT(const CIEC_LREAL &paIN);

const CIEC_SINT func_LREAL_TRUNC_SINT(const CIEC_LREAL &paIN);
const CIEC_INT func_LREAL_TRUNC_INT(const CIEC_LREAL &paIN);
const CIEC_DINT func_LREAL_TRUNC_DINT(const CIEC_LREAL &paIN);
const CIEC_LINT func_LREAL_TRUNC_LINT(const CIEC_LREAL &paIN);
const CIEC_USINT func_LREAL_TRUNC_USINT(const CIEC_LREAL &paIN);
const CIEC_UINT func_LREAL_TRUNC_UINT(const CIEC_LREAL &paIN);
const CIEC_UDINT func_LREAL_TRUNC_UDINT(const CIEC_LREAL &paIN);
const CIEC_ULINT func_LREAL_TRUNC_ULINT(const CIEC_LREAL &paIN);

template<typename T> const CIEC_TIME func_MUL_TIME(const CIEC_TIME& paIN1, const T& paIN2){
  return CIEC_TIME(static_cast<CIEC_TIME::TValueType>(paIN1) * static_cast<typename T::TValueType>(paIN2));
}

template<typename T> const CIEC_LTIME func_MUL_LTIME(const CIEC_LTIME &paIN1, const T &paIN2) {
  return CIEC_LTIME(static_cast<CIEC_LTIME::TValueType>(paIN1) * static_cast<typename T::TValueType>(paIN2));
}

template<typename T> const CIEC_TIME func_DIV_TIME(const CIEC_TIME& paIN1, const T& paIN2) {
  if(0 != static_cast<typename T::TValueType>(paIN2)) {
    return CIEC_TIME(static_cast<CIEC_TIME::TValueType>(paIN1) / static_cast<typename T::TValueType>(paIN2));
  } else {
    DEVLOG_ERROR("Division by zero!\n");
    return CIEC_TIME(0);
  }
}

template<typename T> const CIEC_LTIME func_DIV_LTIME(const CIEC_LTIME &paIN1, const T &paIN2) {
  if(0 != static_cast<typename T::TValueType>(paIN2)) {
    return CIEC_LTIME(static_cast<CIEC_LTIME::TValueType>(paIN1) / static_cast<typename T::TValueType>(paIN2));
  } else {
    DEVLOG_ERROR("Division by zero!\n");
    return CIEC_LTIME(0);
  }
}

template<typename T> CIEC_ANY_INT func_LEN(const T& paVal){
  return CIEC_ANY_INT(paVal.length());
}

template<typename T> const T func_LEFT(const T &paIn, const CIEC_ANY_INT &paL) {
  if(true == paL.isSigned() && 0 > paL.getSignedValue()) {
    DEVLOG_ERROR("value of input L is less than zero");
    return paIn;
  } else {
    if(paIn.length() < paL.getUnsignedValue()) {
      DEVLOG_ERROR("string shorter than input L");
      return paIn;
    } else {
      T temp;
      temp.reserve(static_cast<TForteUInt16>(paL.getUnsignedValue()));
      memcpy(temp.getValue(), paIn.getValue(), static_cast<TForteUInt16>(paL.getUnsignedValue()));
      temp.getValue()[paL.getUnsignedValue()] = '\0';
      temp.assign(temp.getValue(), paL.getUnsignedValue());
      return temp;
    }
  }
}

template<typename T> const T func_RIGHT(const T &paIn, const CIEC_ANY_INT &paL) {
  if(true == paL.isSigned() && 0 > paL.getSignedValue()) {
    DEVLOG_ERROR("value of input L is less than zero");
    return paIn;
  } else {
    if(paIn.length() < paL.getUnsignedValue()) {
      DEVLOG_ERROR("string shorter than input L");
      return paIn;
    } else {
      T temp;
      temp.reserve(static_cast<TForteUInt16>(paL.getUnsignedValue()));
      memcpy(temp.getValue(), paIn.getValue() + (paIn.length() - static_cast<TForteUInt16>(paL.getUnsignedValue())),
        static_cast<TForteUInt16>(paL.getUnsignedValue()));
      temp.getValue()[paL.getUnsignedValue()] = '\0';
      temp.assign(temp.getValue(), paL.getUnsignedValue());
      return temp;
    }
  }
}

template<typename T> const T func_MID(const T &paIn, const CIEC_ANY_INT &paL, const CIEC_ANY_INT &paP) {
  if(true == paP.isSigned() && 0 > paP.getSignedValue()) {
    DEVLOG_ERROR("value of input P is less than zero\n");
    return paIn;
  } else {
    CIEC_INT len_right = CIEC_INT(paIn.length() - paP.getUnsignedValue() + 1);
    CIEC_INT len_left = CIEC_INT(paL.getUnsignedValue());
    return func_LEFT(func_RIGHT(paIn, len_right), len_left);
  }
}

template<typename T> const T func_CONCAT(const T &paIn1, const T &paIn2) {
  static_assert((std::is_base_of<CIEC_ANY_STRING, T>::value), "T not of ANY_STRING");
  T temp(paIn1);
  temp.reserve(static_cast<TForteUInt16>(paIn1.length() + paIn2.length()));
  temp.append(paIn2.getValue(), paIn2.length());
  return temp;
}

#if __cplusplus >= 201103L //stdc11
template<typename T, typename ... Args> const T func_CONCAT(const T &paIn1, Args ... args) {
  return func_CONCAT(paIn1, func_CONCAT(args...));
}
#endif

template<typename T> const T func_INSERT(const T &paIn1, const T &paIn2, const CIEC_ANY_INT &paP) {
  if(CIEC_UINT::scm_nMaxVal < (paIn1.length() + paIn2.length())) {
    DEVLOG_ERROR("result would be longer than maximum allowed length");
    return paIn1;
  }
  const CIEC_ANY::TLargestUIntValueType P = paP.isSigned() ? static_cast<CIEC_ANY::TLargestUIntValueType>(std::max(CIEC_ANY::TLargestIntValueType(0), paP.getSignedValue())) : paP.getUnsignedValue();

  if(P == 0) {
    DEVLOG_ERROR("INSERT called with P equal or lower than 0!\n");
    return paIn1;
  }
  
  if(P > paIn1.length()) {
    DEVLOG_ERROR("INSERT called with P exceeding input string length!\n");
    return paIn1;
  }
  CIEC_INT positionRight = CIEC_INT(paIn1.length() - paP.getSignedValue());
  return func_CONCAT(func_CONCAT(func_LEFT(paIn1, paP), paIn2), func_RIGHT(paIn1, positionRight));
}

template<typename T> const T func_DELETE(const T &paIn, const CIEC_ANY_INT &paL, const CIEC_ANY_INT &paP) {
  const CIEC_ANY::TLargestUIntValueType L = paL.isSigned() ? static_cast<CIEC_ANY::TLargestUIntValueType>(std::max(CIEC_ANY::TLargestIntValueType(0), paL.getSignedValue())) : paL.getUnsignedValue();

  const CIEC_ANY::TLargestUIntValueType P = paP.isSigned() ? static_cast<CIEC_ANY::TLargestUIntValueType>(std::max(CIEC_ANY::TLargestIntValueType(0), paP.getSignedValue())) : paP.getUnsignedValue();

  if(L == 0) {
    DEVLOG_ERROR("DELETE called with L equal or less than 0!\n");
    return paIn;
  }

  if(P == 0) {
    DEVLOG_ERROR("DELETE called with P equal or less than 0!\n");
    return paIn;
  }

  if((P + L) > paIn.length()) {
    DEVLOG_ERROR("DELETE called with delete length exceeding the length of string!\n");
    return paIn;
  }

  CIEC_UINT positionRight = CIEC_UINT(paIn.length() - (L + P - 1));
  CIEC_UINT positionLeft = CIEC_UINT(P - 1);
  return func_CONCAT(func_LEFT(paIn, positionLeft), func_RIGHT(paIn, positionRight));
}

template<typename T> const T func_REPLACE(const T &paIn1, const T &paIn2, const CIEC_ANY_INT &paL, const CIEC_ANY_INT &paP) {
  const CIEC_ANY::TLargestUIntValueType L = paL.isSigned() ? static_cast<CIEC_ANY::TLargestUIntValueType>(std::max(CIEC_ANY::TLargestIntValueType(0), paL.getSignedValue())) : paL.getUnsignedValue();
  const CIEC_ANY::TLargestUIntValueType P = paP.isSigned() ? static_cast<CIEC_ANY::TLargestUIntValueType>(std::max(CIEC_ANY::TLargestIntValueType(0), paP.getSignedValue())) : paP.getUnsignedValue();

  if(L == 0) {
    DEVLOG_ERROR("REPLACE called with L equal or less to 0!\n");
    return paIn1;
  }

  if(P == 0) {
    DEVLOG_ERROR("REPLACE called with P equal or less to 0!\n");
    return paIn1;
  }

  if((P + L) > paIn1.length()) {
    DEVLOG_ERROR("REPLACE outside string boundaries!\n");
    return paIn1;
  }

  if((P + L + paIn2.length()) > CIEC_UINT::scm_nMaxVal) {
    DEVLOG_ERROR("REPLACE exceeds length of string!\n");
  }

  CIEC_INT positionRight = CIEC_INT(paIn1.length() - (L + P - 1));
  CIEC_INT positionLeft = CIEC_INT(P - 1);
  return func_CONCAT(func_CONCAT(func_LEFT(paIn1, positionLeft), paIn2), func_RIGHT(paIn1, positionRight));
}

CIEC_ANY_INT func_FIND(const CIEC_ANY_STRING &paIn1, const CIEC_ANY_STRING &paIn2);

template <typename T>
const T func_TOUPPER(const T &paIn) {
  T temp(paIn);
  char *current = temp.getValue();
  for(size_t i = 0; i <= paIn.length(); ++i) {
    current[i] = static_cast<char>(toupper(current[i]));
  }
  return temp;
}

template <typename T>
const T func_TOLOWER(const T &paIn) {
  T temp(paIn);
  char *current = temp.getValue();
  for(size_t i = 0; i <= paIn.length(); ++i) {
    current[i] = static_cast<char>(tolower(current[i]));
  }
  return temp;
}

const CIEC_DATE_AND_TIME func_CONCAT_DATE_TOD(const CIEC_DATE &paIN1, const CIEC_TIME_OF_DAY &paIN2);

const CIEC_DATE func_CONCAT_DATE(const CIEC_ANY_INT &YEAR, const CIEC_ANY_INT &MONTH, const CIEC_ANY_INT &DAY);

const CIEC_DATE_AND_TIME func_CONCAT_DT(const CIEC_ANY_INT &YEAR, const CIEC_ANY_INT &MONTH, const CIEC_ANY_INT &DAY, const CIEC_ANY_INT &HOUR, const CIEC_ANY_INT &MINUTE, const CIEC_ANY_INT &SECOND, const CIEC_ANY_INT &MILLISECOND);

const CIEC_TIME_OF_DAY func_CONCAT_TOD(const CIEC_ANY_INT &HOUR, const CIEC_ANY_INT &MINUTE, const CIEC_ANY_INT &SECOND, const CIEC_ANY_INT &MILLISECOND);

const CIEC_LDATE_AND_TIME func_CONCAT_LDATE_LTOD(const CIEC_LDATE &paIN1, const CIEC_LTIME_OF_DAY &paIN2);

const CIEC_LDATE func_CONCAT_LDATE(const CIEC_ANY_INT &YEAR, const CIEC_ANY_INT &MONTH, const CIEC_ANY_INT &DAY);

const CIEC_LDATE_AND_TIME func_CONCAT_LDT(const CIEC_ANY_INT &YEAR, const CIEC_ANY_INT &MONTH, const CIEC_ANY_INT &DAY, const CIEC_ANY_INT &HOUR, const CIEC_ANY_INT &MINUTE, const CIEC_ANY_INT &SECOND, const CIEC_ANY_INT &MILLISECOND);

const CIEC_LTIME_OF_DAY func_CONCAT_LTOD(const CIEC_ANY_INT &HOUR, const CIEC_ANY_INT &MINUTE, const CIEC_ANY_INT &SECOND, const CIEC_ANY_INT &MILLISECOND);

void func_SPLIT_DATE(const CIEC_DATE &paValue, CIEC_ANY_INT &YEAR, CIEC_ANY_INT &MONTH, CIEC_ANY_INT &DAY);

void func_SPLIT_LDATE(const CIEC_LDATE &paValue, CIEC_ANY_INT &YEAR, CIEC_ANY_INT &MONTH, CIEC_ANY_INT &DAY);

void func_SPLIT_TOD(const CIEC_TIME_OF_DAY &paValue, CIEC_ANY_INT &HOUR, CIEC_ANY_INT &MINUTE, CIEC_ANY_INT &SECOND, CIEC_ANY_INT &MILLISECOND);

void func_SPLIT_LTOD(const CIEC_LTIME_OF_DAY &paValue, CIEC_ANY_INT &HOUR, CIEC_ANY_INT &MINUTE, CIEC_ANY_INT &SECOND, CIEC_ANY_INT &MILLISECOND);

void func_SPLIT_DT(const CIEC_DATE_AND_TIME &paValue, CIEC_ANY_INT &YEAR, CIEC_ANY_INT &MONTH, CIEC_ANY_INT &DAY, CIEC_ANY_INT &HOUR, CIEC_ANY_INT &MINUTE, CIEC_ANY_INT &SECOND, CIEC_ANY_INT &MILLISECOND);

void func_SPLIT_LDT(const CIEC_LDATE_AND_TIME &paValue, CIEC_ANY_INT &YEAR, CIEC_ANY_INT &MONTH, CIEC_ANY_INT &DAY, CIEC_ANY_INT &HOUR, CIEC_ANY_INT &MINUTE, CIEC_ANY_INT &SECOND, CIEC_ANY_INT &MILLISECOND);

/** @brief Get day of the week Sunday = 0, 1 = Monday, etc
 * 
 * This function differs from the standard in taking an LDATE, which can be implicitly cast from a DATE
 */
CIEC_ANY_INT func_DAY_OF_WEEK(const CIEC_LDATE &paValue);

/**
 * @brief returns current monotonic clock value as time span in nanoseconds
 *
 * @return CIEC_TIME time span since monotonic clock start
 */
const CIEC_TIME func_NOW_MONOTONIC();

/**
 * @brief returns current local date and time - may be subject to time jumps
 *
 * @return CIEC_DATE_AND_TIME of the current local time
 */
const CIEC_DATE_AND_TIME func_NOW();

template <typename T>
T swapSimpleDataHelper(const T data) {
  constexpr size_t dataSize = sizeof(T);
  const char *const dataBytes = reinterpret_cast<const char *>(&data);
  T swappedData;
  char *const swappedDataBytes = reinterpret_cast<char*>(&swappedData);

  if constexpr (1 == dataSize) {
    return T(data);
  } else if constexpr(2 == dataSize) {
    *(swappedDataBytes) = *(dataBytes + 1);
    *(swappedDataBytes + 1) = *(dataBytes);
  } else if constexpr(4 == dataSize) {
    *(swappedDataBytes + 0) = *(dataBytes + 3);
    *(swappedDataBytes + 1) = *(dataBytes + 2);
    *(swappedDataBytes + 2) = *(dataBytes + 1);
    *(swappedDataBytes + 3) = *(dataBytes + 0);
  } else if constexpr(8 == dataSize) {
    *(swappedDataBytes + 0) = *(dataBytes + 7);
    *(swappedDataBytes + 1) = *(dataBytes + 6);
    *(swappedDataBytes + 2) = *(dataBytes + 5);
    *(swappedDataBytes + 3) = *(dataBytes + 4);
    *(swappedDataBytes + 4) = *(dataBytes + 3);
    *(swappedDataBytes + 5) = *(dataBytes + 2);
    *(swappedDataBytes + 6) = *(dataBytes + 1);
    *(swappedDataBytes + 7) = *(dataBytes + 0);
  } else {
    static_assert(true, "Size of data is unhandled");
  }
  return swappedData;
}

template <typename T>
typename std::enable_if_t<std::is_base_of_v<CIEC_ANY_MAGNITUDE, T>, T> swapEndianess(const T &paValue) {
  const typename T::TValueType data = static_cast<typename T::TValueType>(paValue);
  typename T::TValueType swappedData = swapSimpleDataHelper(data);
  return T(swappedData);
}

template <typename T>
typename std::enable_if_t<std::is_base_of_v<CIEC_ANY_BIT, T>, T> swapEndianess(const T &paValue) {
  const typename T::TValueType data = static_cast<typename T::TValueType>(paValue);
  typename T::TValueType swappedData = swapSimpleDataHelper(data);
  return T(swappedData);
}

template <typename T>
typename std::enable_if_t<std::is_base_of_v<CIEC_ANY_CHAR, T>, T> swapEndianess(const T &paValue) {
  const typename T::TValueType data = static_cast<typename T::TValueType>(paValue);
  typename T::TValueType swappedData = swapSimpleDataHelper(data);
  return T(swappedData);
}

template <typename T>
typename std::enable_if_t<std::is_base_of_v<CIEC_ANY_DATE, T>, T> swapEndianess(const T &paValue) {
  const typename T::TValueType data = static_cast<typename T::TValueType>(paValue);
  typename T::TValueType swappedData = swapSimpleDataHelper(data);
  return T(swappedData);
}

template <typename T>
typename std::enable_if_t<std::is_base_of_v<CIEC_ANY_STRING, T>, T> swapEndianess(const T &paValue) {
  if constexpr (std::is_same_v<CIEC_STRING, T>) {
    return T(paValue);
  } else if constexpr (std::is_same_v<CIEC_WSTRING, T>) {
    T reversed(paValue);
    const TForteUInt16 length = reversed.length();
    for(TForteUInt16 i = 0; i < length; i += sizeof(typename T::TValueType)) {
      *(reversed.getValue() + i) = *(paValue.getValue() + (i + 1));
      *(reversed.getValue() + (i + 1)) = *(paValue.getValue() + i);
    }
    return reversed;
  } else {
    static_assert(true, "Unhandled/unknown subtype of CIEC_ANY_STRING");
  }
}

CIEC_ARRAY<CIEC_ANY> swapEndianess(const CIEC_ARRAY<CIEC_ANY> &paValue);

template <typename T>
typename std::enable_if_t<std::is_base_of_v<CIEC_STRUCT, T>, T>
swapEndianess(const T &paValue) {
  T reversed(paValue);
  CIEC_ANY *members = reversed.getMembers();
  for (size_t i = 0; i < reversed.getStructSize(); ++i) {
    switch (members[i].getDataTypeID()) {
    case CIEC_ANY::e_BOOL:
      members[i].setValue(swapEndianess(static_cast<const CIEC_BOOL *>(members)[i]));
      break;
    case CIEC_ANY::e_SINT:
      members[i].setValue(swapEndianess(static_cast<const CIEC_SINT *>(members)[i]));
      break;
    case CIEC_ANY::e_INT:
      members[i].setValue(swapEndianess(static_cast<const CIEC_INT *>(members)[i]));
      break;
    case CIEC_ANY::e_DINT:
      members[i].setValue(swapEndianess(static_cast<const CIEC_DINT *>(members)[i]));
      break;
    case CIEC_ANY::e_LINT:
      members[i].setValue(swapEndianess(static_cast<const CIEC_LINT *>(members)[i]));
      break;
    case CIEC_ANY::e_USINT:
      members[i].setValue(swapEndianess(static_cast<const CIEC_USINT *>(members)[i]));
      break;
    case CIEC_ANY::e_UINT:
      members[i].setValue(swapEndianess(static_cast<const CIEC_UINT *>(members)[i]));
      break;
    case CIEC_ANY::e_UDINT:
      members[i].setValue(swapEndianess(static_cast<const CIEC_UDINT *>(members)[i]));
      break;
    case CIEC_ANY::e_ULINT:
      members[i].setValue(swapEndianess(static_cast<const CIEC_ULINT *>(members)[i]));
      break;
    case CIEC_ANY::e_BYTE:
      members[i].setValue(swapEndianess(static_cast<const CIEC_BYTE *>(members)[i]));
      break;
    case CIEC_ANY::e_WORD:
      members[i].setValue(swapEndianess(static_cast<const CIEC_WORD *>(members)[i]));
      break;
    case CIEC_ANY::e_DWORD:
      members[i].setValue(swapEndianess(static_cast<const CIEC_DWORD *>(members)[i]));
      break;
    case CIEC_ANY::e_LWORD:
      members[i].setValue(swapEndianess(static_cast<const CIEC_LWORD *>(members)[i]));
      break;
    case CIEC_ANY::e_DATE:
      members[i].setValue(swapEndianess(static_cast<const CIEC_DATE *>(members)[i]));
      break;
    case CIEC_ANY::e_TIME_OF_DAY:
      members[i].setValue(swapEndianess(static_cast<const CIEC_TIME_OF_DAY *>(members)[i]));
      break;
    case CIEC_ANY::e_DATE_AND_TIME:
      members[i].setValue(swapEndianess(static_cast<const CIEC_DATE_AND_TIME *>(members)[i]));
      break;
    case CIEC_ANY::e_TIME:
      members[i].setValue(swapEndianess(static_cast<const CIEC_TIME *>(members)[i]));
      break;
    case CIEC_ANY::e_CHAR:
      members[i].setValue(swapEndianess(static_cast<const CIEC_CHAR *>(members)[i]));
      break;
    case CIEC_ANY::e_WCHAR:
      members[i].setValue(swapEndianess(static_cast<const CIEC_WCHAR *>(members)[i]));
      break;
    case CIEC_ANY::e_LDATE:
      members[i].setValue(swapEndianess(static_cast<const CIEC_LDATE *>(members)[i]));
      break;
    case CIEC_ANY::e_LTIME_OF_DAY:
      members[i].setValue(swapEndianess(static_cast<const CIEC_LTIME_OF_DAY *>(members)[i]));
      break;
    case CIEC_ANY::e_LDATE_AND_TIME:
      members[i].setValue(swapEndianess(static_cast<const CIEC_LDATE_AND_TIME *>(members)[i]));
      break;
    case CIEC_ANY::e_LTIME:
      members[i].setValue(swapEndianess(static_cast<const CIEC_LDATE *>(members)[i]));
      break;
    case CIEC_ANY::e_REAL:
      members[i].setValue(swapEndianess(static_cast<const CIEC_REAL *>(members)[i]));
      break;
    case CIEC_ANY::e_LREAL:
      members[i].setValue(swapEndianess(static_cast<const CIEC_LREAL *>(members)[i]));
      break;
    case CIEC_ANY::e_STRING:
      members[i].setValue(swapEndianess(static_cast<const CIEC_STRING *>(members)[i]));
      break;
    case CIEC_ANY::e_WSTRING:
      members[i].setValue(swapEndianess(static_cast<const CIEC_WSTRING *>(members)[i]));
      break;
    case CIEC_ANY::e_ARRAY:
      members[i].setValue(swapEndianess(static_cast<const CIEC_ARRAY<CIEC_ANY> *>(members)[i]));
      break;
    case CIEC_ANY::e_STRUCT:
      members[i].setValue(swapEndianess(static_cast<const CIEC_STRUCT *>(members)[i]));
      break;
    default:
      break; //do nothing
    }
  }
  return reversed;
}

template <template <typename, intmax_t, intmax_t> typename T, typename U, intmax_t lowerBound, intmax_t upperBound>
typename std::enable_if_t<std::is_base_of_v<CIEC_ARRAY_FIXED<U, lowerBound, upperBound>, T<U, lowerBound, upperBound>>, T<U, lowerBound, upperBound>> swapEndianess(const T<U, lowerBound, upperBound> &paValue) {
  T<U, lowerBound, upperBound> reversed(paValue);
  for (auto iter = reversed.begin(); iter != reversed.end(); iter++) {
    *iter = swapEndianess(*iter);
  }
  return reversed;
}

template <template <typename> typename T, typename U>
typename std::enable_if_t<std::is_base_of_v<CIEC_ARRAY_VARIABLE<U>, T<U>>, T<U>> swapEndianess(const T<U> &paValue) {
  T<U> reversed(paValue);
  for (auto iter = reversed.begin(); iter != reversed.end(); iter++) {
    *iter = swapEndianess(*iter);
  }
  return reversed;
}

#ifdef FORTE_LITTLE_ENDIAN
template <typename T>
T func_TO_LITTLE_ENDIAN(const T &paValue) {
  return paValue;
}

template <typename T>
T func_FROM_LITTLE_ENDIAN(const T &paValue) {
  return paValue;
}

template <typename T>
typename std::enable_if_t<std::is_base_of_v<CIEC_ANY_MAGNITUDE, T>, T> func_TO_BIG_ENDIAN(const T &paValue) {
  return swapEndianess(paValue);
}

template <typename T>
typename std::enable_if_t<std::is_base_of_v<CIEC_ANY_BIT, T>, T> func_TO_BIG_ENDIAN(const T &paValue) {
  return swapEndianess(paValue);
}

template <typename T>
typename std::enable_if_t<std::is_base_of_v<CIEC_ANY_CHAR, T>, T> func_TO_BIG_ENDIAN(const T &paValue) {
  return swapEndianess(paValue);
}

template <typename T>
typename std::enable_if_t<std::is_base_of_v<CIEC_ANY_DATE, T>, T> func_TO_BIG_ENDIAN(const T &paValue) {
  return swapEndianess(paValue);
}

template <typename T>
typename std::enable_if_t<std::is_base_of_v<CIEC_ANY_STRING, T>, T> func_TO_BIG_ENDIAN(const T &paValue) {
  return swapEndianess(paValue);
}

CIEC_ARRAY<CIEC_ANY> func_TO_BIG_ENDIAN(const CIEC_ARRAY<CIEC_ANY> &paValue);

template <typename T>
typename std::enable_if_t<std::is_base_of_v<CIEC_STRUCT, T>, T> func_TO_BIG_ENDIAN(const T &paValue) {
  return swapEndianess(paValue);
}

template <template <typename, intmax_t, intmax_t> typename T, typename U, intmax_t lowerBound, intmax_t upperBound>
typename std::enable_if_t<std::is_base_of_v<CIEC_ARRAY_FIXED<U, lowerBound, upperBound>, T<U, lowerBound, upperBound>>, T<U, lowerBound, upperBound>> func_TO_BIG_ENDIAN(const T<U, lowerBound, upperBound> &paValue) {
  return swapEndianess(paValue);
}

template <template <typename> typename T, typename U>
typename std::enable_if_t<std::is_base_of_v<CIEC_ARRAY_VARIABLE<U>, T<U>>, T<U>> func_TO_BIG_ENDIAN(const T<U> &paValue) {
  return swapEndianess(paValue);
}

template <typename T>
typename std::enable_if_t<std::is_base_of_v<CIEC_ANY_MAGNITUDE, T>, T> func_FROM_BIG_ENDIAN(const T &paValue) {
  return swapEndianess(paValue);
}

template <typename T>
typename std::enable_if_t<std::is_base_of_v<CIEC_ANY_BIT, T>, T> func_FROM_BIG_ENDIAN(const T &paValue) {
  return swapEndianess(paValue);
}

template <typename T>
typename std::enable_if_t<std::is_base_of_v<CIEC_ANY_CHAR, T>, T> func_FROM_BIG_ENDIAN(const T &paValue) {
  return swapEndianess(paValue);
}

template <typename T>
typename std::enable_if_t<std::is_base_of_v<CIEC_ANY_DATE, T>, T> func_FROM_BIG_ENDIAN(const T &paValue) {
  return swapEndianess(paValue);
}

template <typename T>
typename std::enable_if_t<std::is_base_of_v<CIEC_ANY_STRING, T>, T> func_FROM_BIG_ENDIAN(const T &paValue) {
  return swapEndianess(paValue);
}

CIEC_ARRAY<CIEC_ANY> func_FROM_BIG_ENDIAN(const CIEC_ARRAY<CIEC_ANY> &paValue);

template <typename T>
typename std::enable_if_t<std::is_base_of_v<CIEC_STRUCT, T>, T> func_FROM_BIG_ENDIAN(const T &paValue) {
  return swapEndianess(paValue);
}

template <template <typename, intmax_t, intmax_t> typename T, typename U, intmax_t lowerBound, intmax_t upperBound>
typename std::enable_if_t<std::is_base_of_v<CIEC_ARRAY_FIXED<U, lowerBound, upperBound>, T<U, lowerBound, upperBound>>, T<U, lowerBound, upperBound>> func_FROM_BIG_ENDIAN(const T<U, lowerBound, upperBound> &paValue) {
  return swapEndianess(paValue);
}

template <template <typename> typename T, typename U>
typename std::enable_if_t<std::is_base_of_v<CIEC_ARRAY_VARIABLE<U>, T<U>>, T<U>> func_FROM_BIG_ENDIAN(const T<U> &paValue) {
  return swapEndianess(paValue);
}
#endif

#ifdef FORTE_BIG_ENDIAN
template <typename T>
typename std::enable_if_t<std::is_base_of_v<CIEC_ANY_MAGNITUDE, T>, T> func_TO_LITTLE_ENDIAN(const T &paValue) {
  return swapEndianess(paValue);
}

template <typename T>
typename std::enable_if_t<std::is_base_of_v<CIEC_ANY_BIT, T>, T> func_TO_LITTLE_ENDIAN(const T &paValue) {
  return swapEndianess(paValue);
}

template <typename T>
typename std::enable_if_t<std::is_base_of_v<CIEC_ANY_CHAR, T>, T> func_TO_LITTLE_ENDIAN(const T &paValue) {
  return swapEndianess(paValue);
}

template <typename T>
typename std::enable_if_t<std::is_base_of_v<CIEC_ANY_DATE, T>, T> func_TO_LITTLE_ENDIAN(const T &paValue) {
  return swapEndianess(paValue);
}

template <typename T>
typename std::enable_if_t<std::is_base_of_v<CIEC_ANY_STRING, T>, T> func_TO_LITTLE_ENDIAN(const T &paValue) {
  return swapEndianess(paValue);
}

CIEC_ARRAY<CIEC_ANY> func_TO_LITTLE_ENDIAN(const CIEC_ARRAY<CIEC_ANY> &paValue);

template <typename T>
typename std::enable_if_t<std::is_base_of_v<CIEC_STRUCT, T>, T> func_TO_LITTLE_ENDIAN(const T &paValue) {
  return swapEndianess(paValue);
}

template <template <typename, intmax_t, intmax_t> typename T, typename U, intmax_t lowerBound, intmax_t upperBound>
typename std::enable_if_t<std::is_base_of_v<CIEC_ARRAY_FIXED<U, lowerBound, upperBound>, T<U, lowerBound, upperBound>>, T<U, lowerBound, upperBound>> func_TO_LITTLE_ENDIAN(const T<U, lowerBound, upperBound> &paValue) {
  return swapEndianess(paValue);
}

template <template <typename> typename T, typename U>
typename std::enable_if_t<std::is_base_of_v<CIEC_ARRAY_VARIABLE<U>, T<U>>, T<U>> func_TO_LITTLE_ENDIAN(const T<U> &paValue) {
  return swapEndianess(paValue);
}

template <typename T>
typename std::enable_if_t<std::is_base_of_v<CIEC_ANY_MAGNITUDE, T>, T> func_FROM_LITTLE_ENDIAN(const T &paValue) {
  return swapEndianess(paValue);
}

template <typename T>
typename std::enable_if_t<std::is_base_of_v<CIEC_ANY_BIT, T>, T> func_FROM_LITTLE_ENDIAN(const T &paValue) {
  return swapEndianess(paValue);
}

template <typename T>
typename std::enable_if_t<std::is_base_of_v<CIEC_ANY_CHAR, T>, T> func_FROM_LITTLE_ENDIAN(const T &paValue) {
  return swapEndianess(paValue);
}

template <typename T>
typename std::enable_if_t<std::is_base_of_v<CIEC_ANY_DATE, T>, T> func_FROM_LITTLE_ENDIAN(const T &paValue) {
  return swapEndianess(paValue);
}

template <typename T>
typename std::enable_if_t<std::is_base_of_v<CIEC_ANY_STRING, T>, T> func_FROM_LITTLE_ENDIAN(const T &paValue) {
  return swapEndianess(paValue);
}

CIEC_ARRAY<CIEC_ANY> func_FROM_LITTLE_ENDIAN(const CIEC_ARRAY<CIEC_ANY> &paValue);

template <typename T>
typename std::enable_if_t<std::is_base_of_v<CIEC_STRUCT, T>, T> func_FROM_LITTLE_ENDIAN(const T &paValue) {
  return swapEndianess(paValue);
}

template <template <typename, intmax_t, intmax_t> typename T, typename U, intmax_t lowerBound, intmax_t upperBound>
typename std::enable_if_t<std::is_base_of_v<CIEC_ARRAY_FIXED<U, lowerBound, upperBound>, T<U, lowerBound, upperBound>>, T<U, lowerBound, upperBound>> func_FROM_LITTLE_ENDIAN(const T<U, lowerBound, upperBound> &paValue) {
  return swapEndianess(paValue);
}

template <template <typename> typename T, typename U>
typename std::enable_if_t<std::is_base_of_v<CIEC_ARRAY_VARIABLE<U>, T<U>>, T<U>> func_FROM_LITTLE_ENDIAN(const T<U> &paValue) {
  return swapEndianess(paValue);
}

template <typename T>
T func_TO_BIG_ENDIAN(const T &paValue) {
  return paValue;
}

template <typename T>
T func_FROM_BIG_ENDIAN(const T &paValue) {
  return paValue;
}
#endif

#endif /* IEC61131_FUNCTIONS_H_ */
