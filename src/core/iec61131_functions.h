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
 *******************************************************************************/
#ifndef IEC61131_FUNCTIONS_H_
#define IEC61131_FUNCTIONS_H_

#include "../arch/devlog.h"
#include "convert_functions.h"
#include <math.h>
#include <string.h>
#include "iec61131_cast_helper.h"
#include "./utils/staticassert.h"

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

#ifdef MAX
#undef MAX
#endif

#ifdef MIN
#undef MIN
#endif

#define GENERATE_APPLY_FUNCTION(func)  template<typename T> struct func ##_Function { \
  static const T call(const T& pa_roIN1, const T& pa_roIN2){ \
      return func(pa_roIN1, pa_roIN2); \
    }; \
};

template<typename T>
const T func_ABS(const T &pa_roIN) {
  FORTE_STATIC_ASSERT((forte::core::mpl::is_base_of<CIEC_ANY_NUM, T>::value), TNotOfAnyNum);
  return (0 > pa_roIN) ? T(static_cast<typename T::TValueType>(pa_roIN * static_cast<typename T::TValueType>(-1))) : pa_roIN;
}

#ifdef FORTE_USE_REAL_DATATYPE
  inline const CIEC_REAL func_SQRT(const CIEC_REAL &pa_roIN){
    return CIEC_REAL(sqrtf(pa_roIN));
  }

# ifdef FORTE_USE_64BIT_DATATYPES
  inline const CIEC_LREAL func_SQRT(const CIEC_LREAL &pa_roIN){
    return CIEC_LREAL(sqrt(pa_roIN));
  }
# endif //FORTE_USE_64BIT_DATATYPES

#endif //#ifdef FORTE_USE_REAL_DATATYPE

#ifdef FORTE_USE_REAL_DATATYPE
  inline const CIEC_REAL func_LN(const CIEC_REAL &pa_roIN){
      return CIEC_REAL(logf(pa_roIN));
    }

# ifdef FORTE_USE_64BIT_DATATYPES
    inline const CIEC_LREAL func_LN(const CIEC_LREAL &pa_roIN){
      return CIEC_LREAL(log(pa_roIN));
    }
# endif //FORTE_USE_64BIT_DATATYPES

#endif //#ifdef FORTE_USE_REAL_DATATYPE

#ifdef FORTE_USE_REAL_DATATYPE
  inline const CIEC_REAL func_LOG(const CIEC_REAL &pa_roIN){
      return CIEC_REAL(log10f(pa_roIN));
    }

# ifdef FORTE_USE_64BIT_DATATYPES
    inline const CIEC_LREAL func_LOG(const CIEC_LREAL &pa_roIN){
      return CIEC_LREAL(log10(pa_roIN));
    }
# endif //FORTE_USE_64BIT_DATATYPES
#endif //#ifdef FORTE_USE_REAL_DATATYPE

#ifdef FORTE_USE_REAL_DATATYPE
  inline const CIEC_REAL func_SIN(const CIEC_REAL &pa_roIN){
      return CIEC_REAL(sinf(pa_roIN));
    }

# ifdef FORTE_USE_64BIT_DATATYPES
    inline const CIEC_LREAL func_SIN(const CIEC_LREAL &pa_roIN){
      return CIEC_LREAL(sin(pa_roIN));
    }
# endif //FORTE_USE_64BIT_DATATYPES

#endif //#ifdef FORTE_USE_REAL_DATATYPE

#ifdef FORTE_USE_REAL_DATATYPE
  inline const CIEC_REAL func_COS(const CIEC_REAL &pa_roIN){
      return CIEC_REAL(cosf(pa_roIN));
    }

# ifdef FORTE_USE_64BIT_DATATYPES
    inline const CIEC_LREAL func_COS(const CIEC_LREAL &pa_roIN){
      return CIEC_LREAL(cos(pa_roIN));
    }
# endif //FORTE_USE_64BIT_DATATYPES
#endif //#ifdef FORTE_USE_REAL_DATATYPE

#ifdef FORTE_USE_REAL_DATATYPE
  inline const CIEC_REAL func_TAN(const CIEC_REAL &pa_roIN){
      return CIEC_REAL(tanf(pa_roIN));
    }

# ifdef FORTE_USE_64BIT_DATATYPES
    inline const CIEC_LREAL func_TAN(const CIEC_LREAL &pa_roIN){
      return CIEC_LREAL(tan(pa_roIN));
    }
# endif //FORTE_USE_64BIT_DATATYPES
#endif //#ifdef FORTE_USE_REAL_DATATYPE

#ifdef FORTE_USE_REAL_DATATYPE
  inline const CIEC_REAL func_ASIN(const CIEC_REAL &pa_roIN){
      return CIEC_REAL(asinf(pa_roIN));
    }

# ifdef FORTE_USE_64BIT_DATATYPES
    inline const CIEC_LREAL func_ASIN(const CIEC_LREAL &pa_roIN){
      return CIEC_LREAL(asin(pa_roIN));
    }
# endif //FORTE_USE_64BIT_DATATYPES

#endif //#ifdef FORTE_USE_REAL_DATATYPE

#ifdef FORTE_USE_REAL_DATATYPE
  inline const CIEC_REAL func_ACOS(const CIEC_REAL &pa_roIN){
      return CIEC_REAL(acosf(pa_roIN));
    }

# ifdef FORTE_USE_64BIT_DATATYPES
    inline const CIEC_LREAL func_ACOS(const CIEC_LREAL &pa_roIN){
      return CIEC_LREAL(acos(pa_roIN));
    }
# endif //FORTE_USE_64BIT_DATATYPES
#endif //#ifdef FORTE_USE_REAL_DATATYPE

#ifdef FORTE_USE_REAL_DATATYPE
  inline const CIEC_REAL func_ATAN(const CIEC_REAL &pa_roIN){
      return CIEC_REAL(atanf(pa_roIN));
    }

# ifdef FORTE_USE_64BIT_DATATYPES
    inline const CIEC_LREAL func_ATAN(const CIEC_LREAL &pa_roIN){
      return CIEC_LREAL(atan(pa_roIN));
    }
# endif //FORTE_USE_64BIT_DATATYPES
#endif //#ifdef FORTE_USE_REAL_DATATYPE

#ifdef FORTE_USE_REAL_DATATYPE
  inline const CIEC_REAL func_EXP(const CIEC_REAL &pa_roIN){
      return CIEC_REAL(expf(pa_roIN));
    }

# ifdef FORTE_USE_64BIT_DATATYPES
    inline const CIEC_LREAL func_EXP(const CIEC_LREAL &pa_roIN){
      return CIEC_LREAL(exp(pa_roIN));
    }
# endif //FORTE_USE_64BIT_DATATYPES
#endif //#ifdef FORTE_USE_REAL_DATATYPE

template<typename T> const T func_ROL(const T &paIn, const CIEC_ANY_INT &paN) {
  FORTE_STATIC_ASSERT((forte::core::mpl::is_base_of<CIEC_ANY_BIT, T>::value), TNotOfAnyBit);
  if((true == paN.isSigned() && 0 <= paN.getSignedValue()) || false == paN.isSigned()) {
    return T(static_cast<typename T::TValueType>((paIn << paN.getUnsignedValue()) | (paIn >> (sizeof(typename T::TValueType) * 8 - paN.getUnsignedValue()))));
  }
  DEVLOG_ERROR("value of input N is less than zero");
  return T(static_cast<typename T::TValueType>(0));
}

template<typename T> const T func_ROR(const T &paIn, const CIEC_ANY_INT &paN) {
  FORTE_STATIC_ASSERT((forte::core::mpl::is_base_of<CIEC_ANY_BIT, T>::value), TNotOfAnyBit);
  if((true == paN.isSigned() && 0 <= paN.getSignedValue()) || false == paN.isSigned()) {
    return T(static_cast<typename T::TValueType>((paIn >> paN.getUnsignedValue()) | (paIn << (sizeof(typename T::TValueType) * 8 - paN.getUnsignedValue()))));
  }
  DEVLOG_ERROR("value of input N is less than zero");
  return T(static_cast<typename T::TValueType>(0));
}

template<typename T> const T func_SHL(const T &paIn, const CIEC_ANY_INT &paN) {
  FORTE_STATIC_ASSERT((forte::core::mpl::is_base_of<CIEC_ANY_BIT, T>::value), TNotOfAnyBit);
  if((true == paN.isSigned() && 0 <= paN.getSignedValue()) || false == paN.isSigned()) {
    return T(static_cast<typename T::TValueType>(paIn << paN.getUnsignedValue()));
  }
  DEVLOG_ERROR("value of input N is less than zero");
  return T(static_cast<typename T::TValueType>(0));
}
template<typename T> const T func_SHR(const T &paIn, const CIEC_ANY_INT &paN) {
  FORTE_STATIC_ASSERT((forte::core::mpl::is_base_of<CIEC_ANY_BIT, T>::value), TNotOfAnyBit);
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

template<typename T, typename U, template<typename A> class F, typename C> typename forte::core::mpl::get_castable_type<T, U>::type APPLY(const T &pa_roIN1,
    const U &pa_roIN2) {
  FORTE_STATIC_ASSERT((forte::core::mpl::are_of_subtype<C, T, U>::value), TemplateInstantiationWithIncompatibleTypes);
  typedef typename forte::core::mpl::get_castable_type<T, U>::type tImplicitCastType;
  FORTE_STATIC_ASSERT(!(forte::core::mpl::is_same<tImplicitCastType, forte::core::mpl::NullType>::value), NoImplicitCastPossible);
  const tImplicitCastType Result(F<tImplicitCastType>::call(static_cast<tImplicitCastType>(pa_roIN1), static_cast<tImplicitCastType>(pa_roIN2)));
  return Result;
}

template<class T, class U>
class AndOperation {
    typedef typename forte::core::mpl::get_castable_type<T, U>::type resultType;
  public:
    static resultType call(const T &pa_roIN1, const U &pa_roIN2) {
      return resultType(static_cast<resultType>(pa_roIN1) & static_cast<resultType>(pa_roIN2));
    }
  private:
    AndOperation() {
    }
};

template<class T>
class AndOperation<T, CIEC_BOOL> {
    typedef typename forte::core::mpl::get_castable_type<T, CIEC_BOOL>::type resultType;
  public:
    static resultType call(const T &pa_roIN1, const CIEC_BOOL &pa_roIN2) {
      return resultType(pa_roIN1 & (pa_roIN2 ? 1 : 0));
    }
  private:
    AndOperation() {
    }
};

template<class T>
class AndOperation<CIEC_BOOL, T> {
    typedef typename forte::core::mpl::get_castable_type<T, CIEC_BOOL>::type resultType;
  public:
    static resultType call(const CIEC_BOOL &pa_roIN1, const T &pa_roIN2) {
      return resultType((pa_roIN1 ? 1 : 0) & pa_roIN2);
    }
  private:
    AndOperation() {
    }
};

template<>
class AndOperation<CIEC_BOOL, CIEC_BOOL> {
  public:
    static CIEC_BOOL call(const CIEC_BOOL &pa_roIN1, const CIEC_BOOL &pa_roIN2) {
      return CIEC_BOOL(pa_roIN1 && pa_roIN2);
    }
  private:
    AndOperation() {
    }
};

template<typename T, typename U>
typename forte::core::mpl::get_castable_type<typename forte::core::mpl::get_equivalent_CIEC_class<T>::type, typename forte::core::mpl::get_equivalent_CIEC_class<U>::type>::type func_AND(const T &pa_roIN1, const U &pa_roIN2) {
  typedef typename forte::core::mpl::get_equivalent_CIEC_class<T>::type tTClass;
  typedef typename forte::core::mpl::get_equivalent_CIEC_class<U>::type tUClass;
  FORTE_STATIC_ASSERT((forte::core::mpl::are_of_subtype<CIEC_ANY_BIT, tTClass, tUClass>::value), TemplateInstantiationWithIncompatibleTypes);
  typedef typename forte::core::mpl::get_castable_type<tTClass, tUClass>::type tImplicitCastType;
  FORTE_STATIC_ASSERT(!(forte::core::mpl::is_same<tImplicitCastType, forte::core::mpl::NullType>::value), NoImplicitCastPossible);
  return AndOperation<tTClass, tUClass>::call(pa_roIN1, pa_roIN2);
}

template<class T, class U>
class OrOperation {
    typedef typename forte::core::mpl::get_castable_type<T, U>::type resultType;
  public:
    static resultType call(const T &pa_roIN1, const U &pa_roIN2) {
      return resultType(static_cast<resultType>(pa_roIN1) | static_cast<resultType>(pa_roIN2));
    }
  private:
    OrOperation() {
    }
};

template<class T>
class OrOperation<T, CIEC_BOOL> {
    typedef typename forte::core::mpl::get_castable_type<T, CIEC_BOOL>::type resultType;
  public:
    static resultType call(const T &pa_roIN1, const CIEC_BOOL &pa_roIN2) {
      return resultType(pa_roIN1 | (pa_roIN2 ? 1 : 0));
    }
  private:
    OrOperation() {
    }
};

template<class T>
class OrOperation<CIEC_BOOL, T> {
    typedef typename forte::core::mpl::get_castable_type<T, CIEC_BOOL>::type resultType;
  public:
    static resultType call(const CIEC_BOOL &pa_roIN1, const T &pa_roIN2) {
      return resultType((pa_roIN1 ? 1 : 0) | pa_roIN2);
    }
  private:
    OrOperation() {
    }
};

template<>
class OrOperation<CIEC_BOOL, CIEC_BOOL> {
  public:
    static CIEC_BOOL call(const CIEC_BOOL &pa_roIN1, const CIEC_BOOL &pa_roIN2) {
      return CIEC_BOOL(pa_roIN1 || pa_roIN2);
    }
  private:
    OrOperation() {
    }
};


template<typename T, typename U>
typename forte::core::mpl::get_castable_type<typename forte::core::mpl::get_equivalent_CIEC_class<T>::type, typename forte::core::mpl::get_equivalent_CIEC_class<U>::type>::type func_OR(const T &pa_roIN1, const U &pa_roIN2) {
  typedef typename forte::core::mpl::get_equivalent_CIEC_class<T>::type tTClass;
  typedef typename forte::core::mpl::get_equivalent_CIEC_class<U>::type tUClass;
  FORTE_STATIC_ASSERT((forte::core::mpl::are_of_subtype<CIEC_ANY_BIT, tTClass, tUClass>::value), TemplateInstantiationWithIncompatibleTypes);
  typedef typename forte::core::mpl::get_castable_type<tTClass, tUClass>::type tImplicitCastType;
  FORTE_STATIC_ASSERT(!(forte::core::mpl::is_same<tImplicitCastType, forte::core::mpl::NullType>::value), NoImplicitCastPossible);
  return OrOperation<tTClass, tUClass>::call(pa_roIN1, pa_roIN2);
}
template<class T, class U>
class XorOperation {
    typedef typename forte::core::mpl::get_castable_type<T, U>::type resultType;
  public:
    static resultType call(const T &pa_roIN1, const U &pa_roIN2) {
      return resultType(static_cast<resultType>(pa_roIN1) ^ static_cast<resultType>(pa_roIN2));
    }
  private:
    XorOperation() {
    }
};

template<class T>
class XorOperation<T, CIEC_BOOL> {
    typedef typename forte::core::mpl::get_castable_type<T, CIEC_BOOL>::type resultType;
  public:
    static resultType call(const T &pa_roIN1, const CIEC_BOOL &pa_roIN2) {
      return resultType(pa_roIN1 ^ (pa_roIN2 ? 1 : 0));
    }
  private:
    XorOperation() {
    }
};

template<class T>
class XorOperation<CIEC_BOOL, T> {
    typedef typename forte::core::mpl::get_castable_type<T, CIEC_BOOL>::type resultType;
  public:
    static resultType call(const CIEC_BOOL &pa_roIN1, const T &pa_roIN2) {
      return resultType((pa_roIN1 ? 1 : 0) ^ pa_roIN2);
    }
  private:
    XorOperation() {
    }
};

template<>
class XorOperation<CIEC_BOOL, CIEC_BOOL> {
  public:
    static CIEC_BOOL call(const CIEC_BOOL &pa_roIN1, const CIEC_BOOL &pa_roIN2) {
      return CIEC_BOOL(pa_roIN1 != pa_roIN2);
    }
  private:
    XorOperation() {
    }
};

template<typename T, typename U>
typename forte::core::mpl::get_castable_type<typename forte::core::mpl::get_equivalent_CIEC_class<T>::type, typename forte::core::mpl::get_equivalent_CIEC_class<U>::type>::type func_XOR(const T &pa_roIN1, const U &pa_roIN2) {
  typedef typename forte::core::mpl::get_equivalent_CIEC_class<T>::type tTClass;
  typedef typename forte::core::mpl::get_equivalent_CIEC_class<U>::type tUClass;
  FORTE_STATIC_ASSERT((forte::core::mpl::are_of_subtype<CIEC_ANY_BIT, tTClass, tUClass>::value), TemplateInstantiationWithIncompatibleTypes);
  typedef typename forte::core::mpl::get_castable_type<tTClass, tUClass>::type tImplicitCastType;
  FORTE_STATIC_ASSERT(!(forte::core::mpl::is_same<tImplicitCastType, forte::core::mpl::NullType>::value), NoImplicitCastPossible);
  return XorOperation<tTClass, tUClass>::call(pa_roIN1, pa_roIN2);
}

template <typename T>
const typename forte::core::mpl::get_equivalent_CIEC_class<T>::type func_NOT(const T& paIN) {
  FORTE_STATIC_ASSERT((forte::core::mpl::is_base_of<CIEC_ANY_BIT, T>::value), NotFunctionOnlyForCiecAnyBit);
  typedef typename forte::core::mpl::get_equivalent_CIEC_class<T>::type tResultType;
  if constexpr (std::is_same<tResultType, CIEC_BOOL>::value) {
    return tResultType(!paIN);
  } else {
    return tResultType(~paIN);
  }
}

template<typename T, typename U> const CIEC_BOOL func_GT(const T &pa_roIN1, const U &pa_roIN2) {
  FORTE_STATIC_ASSERT((forte::core::mpl::are_of_subtype<CIEC_ANY_ELEMENTARY, T, U>::value), TemplateInstantiationWithIncompatibleTypes);
  FORTE_STATIC_ASSERT(!(forte::core::mpl::are_of_subtype<CIEC_ANY_BIT, T, U>::value), AnyBitTypesHaveNoNaturalOrder);
  typedef typename forte::core::mpl::get_castable_type<T, U>::type tImplicitCastType;
  FORTE_STATIC_ASSERT(!(forte::core::mpl::is_same<tImplicitCastType, forte::core::mpl::NullType>::value), NoImplicitCastPossible);
  return func_GT<tImplicitCastType>(static_cast<tImplicitCastType>(pa_roIN1), static_cast<tImplicitCastType>(pa_roIN2));
}

template<typename T> const CIEC_BOOL func_GT(const T &pa_roIN1, const T &pa_roIN2) {
  return CIEC_BOOL(pa_roIN1 > pa_roIN2);
}

template<typename T, typename U> const CIEC_BOOL func_EQ(const T &pa_roIN1, const U &pa_roIN2) {
  if constexpr (forte::core::mpl::are_of_subtype<CIEC_ANY_BIT, T, U>::value) { //ANY_BITs can be also partial accesses
    typedef typename forte::core::mpl::get_equivalent_CIEC_class<T>::type tTClass;
    typedef typename forte::core::mpl::get_equivalent_CIEC_class<U>::type tUClass;
    typedef typename forte::core::mpl::get_castable_type<tTClass, tUClass>::type tImplicitCastType;
    return func_EQ<tImplicitCastType>(static_cast<tImplicitCastType>(pa_roIN1), static_cast<tImplicitCastType>(pa_roIN2));
  } else {
    typedef typename forte::core::mpl::get_castable_type<T, U>::type tImplicitCastType;
    FORTE_STATIC_ASSERT(!(forte::core::mpl::is_same<tImplicitCastType, forte::core::mpl::NullType>::value), NoImplicitCastPossible);
    return func_EQ<tImplicitCastType>(static_cast<tImplicitCastType>(pa_roIN1), static_cast<tImplicitCastType>(pa_roIN2));
  }
}

template<typename T> const CIEC_BOOL func_EQ(const T &pa_roIN1, const T &pa_roIN2) {
  return CIEC_BOOL(pa_roIN1 == pa_roIN2);
}

template<typename T, typename U> const CIEC_BOOL func_GE(const T &pa_roIN1, const U &pa_roIN2) {
  FORTE_STATIC_ASSERT((forte::core::mpl::are_of_subtype<CIEC_ANY_ELEMENTARY, T, U>::value), TemplateInstantiationWithIncompatibleTypes);
  FORTE_STATIC_ASSERT(!(forte::core::mpl::are_of_subtype<CIEC_ANY_BIT, T, U>::value), AnyBitTypesHaveNoNaturalOrder);
  typedef typename forte::core::mpl::get_castable_type<T, U>::type tImplicitCastType;
  FORTE_STATIC_ASSERT(!(forte::core::mpl::is_same<tImplicitCastType, forte::core::mpl::NullType>::value), NoImplicitCastPossible);
  return func_GE<tImplicitCastType>(static_cast<tImplicitCastType>(pa_roIN1), static_cast<tImplicitCastType>(pa_roIN2));
}

template<typename T> const CIEC_BOOL func_GE(const T &pa_roIN1, const T &pa_roIN2) {
  return CIEC_BOOL(pa_roIN1 >= pa_roIN2);
}

template<typename T, typename U> const CIEC_BOOL func_LE(const T &pa_roIN1, const U &pa_roIN2) {
  FORTE_STATIC_ASSERT((forte::core::mpl::are_of_subtype<CIEC_ANY_ELEMENTARY, T, U>::value), TemplateInstantiationWithIncompatibleTypes);
  FORTE_STATIC_ASSERT(!(forte::core::mpl::are_of_subtype<CIEC_ANY_BIT, T, U>::value), AnyBitTypesHaveNoNaturalOrder);
  typedef typename forte::core::mpl::get_castable_type<T, U>::type tImplicitCastType;
  FORTE_STATIC_ASSERT(!(forte::core::mpl::is_same<tImplicitCastType, forte::core::mpl::NullType>::value), NoImplicitCastPossible);
  return func_LE<tImplicitCastType>(static_cast<tImplicitCastType>(pa_roIN1), static_cast<tImplicitCastType>(pa_roIN2));
}

template<typename T> const CIEC_BOOL func_LE(const T &pa_roIN1, const T &pa_roIN2) {
  return CIEC_BOOL(pa_roIN1 <= pa_roIN2);
}

template<typename T, typename U> const CIEC_BOOL func_LT(const T &pa_roIN1, const U &pa_roIN2) {
  FORTE_STATIC_ASSERT((forte::core::mpl::are_of_subtype<CIEC_ANY_ELEMENTARY, T, U>::value), TemplateInstantiationWithIncompatibleTypes);
  FORTE_STATIC_ASSERT(!(forte::core::mpl::are_of_subtype<CIEC_ANY_BIT, T, U>::value), AnyBitTypesHaveNoNaturalOrder);
  typedef typename forte::core::mpl::get_castable_type<T, U>::type tImplicitCastType;
  FORTE_STATIC_ASSERT(!(forte::core::mpl::is_same<tImplicitCastType, forte::core::mpl::NullType>::value), NoImplicitCastPossible);
  return func_LT<tImplicitCastType>(static_cast<tImplicitCastType>(pa_roIN1), static_cast<tImplicitCastType>(pa_roIN2));
}

template<typename T> const CIEC_BOOL func_LT(const T &pa_roIN1, const T &pa_roIN2) {
  return CIEC_BOOL(pa_roIN1 < pa_roIN2);
}

template<typename T, typename U> const CIEC_BOOL func_NE(const T &pa_roIN1, const U &pa_roIN2) {
  if constexpr (forte::core::mpl::are_of_subtype<CIEC_ANY_BIT, T, U>::value) { //ANY_BITs can be also partial accesses
    typedef typename forte::core::mpl::get_equivalent_CIEC_class<T>::type tTClass;
    typedef typename forte::core::mpl::get_equivalent_CIEC_class<U>::type tUClass;
    typedef typename forte::core::mpl::get_castable_type<tTClass, tUClass>::type tImplicitCastType;
    return func_NE<tImplicitCastType>(static_cast<tImplicitCastType>(pa_roIN1), static_cast<tImplicitCastType>(pa_roIN2));
  } else {
    typedef typename forte::core::mpl::get_castable_type<T, U>::type tImplicitCastType;
    FORTE_STATIC_ASSERT(!(forte::core::mpl::is_same<tImplicitCastType, forte::core::mpl::NullType>::value), NoImplicitCastPossible);
    return func_NE<tImplicitCastType>(static_cast<tImplicitCastType>(pa_roIN1), static_cast<tImplicitCastType>(pa_roIN2));
  }
}

template<typename T> const CIEC_BOOL func_NE(const T &pa_roIN1, const T &pa_roIN2) {
  return CIEC_BOOL(pa_roIN1 != pa_roIN2);
}

template<typename T, typename U> const typename forte::core::mpl::get_castable_type<T, U>::type func_SEL(const CIEC_BOOL &G, const T &IN0, const U &IN1) {
  return (G.operator bool()) ? IN1 : IN0;
}

GENERATE_APPLY_FUNCTION(func_MAX)
template<typename T, typename U> typename forte::core::mpl::get_castable_type<T, U>::type func_MAX(const T &pa_roIN1, const U &pa_roIN2) {
  return APPLY<T, U, func_MAX_Function, CIEC_ANY_ELEMENTARY>(pa_roIN1, pa_roIN2);
}

template<typename T> const T func_MAX(const T &pa_roIN1, const T &pa_roIN2) {
  if(pa_roIN1 > pa_roIN2) {
    return pa_roIN1;
  } else {
    return pa_roIN2;
  }
}

GENERATE_APPLY_FUNCTION(func_MIN)
template<typename T, typename U> typename forte::core::mpl::get_castable_type<T, U>::type func_MIN(const T &pa_roIN1, const U &pa_roIN2) {
  return APPLY<T, U, func_MIN_Function, CIEC_ANY_ELEMENTARY>(pa_roIN1, pa_roIN2);
}

template<typename T> const T func_MIN(const T &pa_roIN1, const T &pa_roIN2) {
  if(pa_roIN1 < pa_roIN2) {
    return pa_roIN1;
  } else {
    return pa_roIN2;
  }
}

template<typename T, typename U, typename V>
const typename forte::core::mpl::get_castable_type<typename forte::core::mpl::get_castable_type<T, U>::type, V>::type func_LIMIT(const T &pa_roMN, const U &pa_roIN,
    const V &pa_roMX) {
  typedef typename forte::core::mpl::get_castable_type<typename forte::core::mpl::get_castable_type<T, U>::type, V>::type tImplicitType;
  return func_MIN(func_MAX(static_cast<tImplicitType>(pa_roIN), static_cast<tImplicitType>(pa_roMN)), static_cast<tImplicitType>(pa_roMX));
}

GENERATE_APPLY_FUNCTION(func_MOD)
template<typename T, typename U> typename forte::core::mpl::get_castable_type<T, U>::type func_MOD(const T &pa_roIN1, const U &pa_roIN2) {
  return APPLY<T, U, func_MOD_Function, CIEC_ANY_INT>(pa_roIN1, pa_roIN2);
}

template<typename T> const T func_MOD(const T &pa_roIN1, const T &pa_roIN2) {
  if(0 == pa_roIN2) {
    return T(0);
  }
  return T(pa_roIN1.getSignedValue() % pa_roIN2.getSignedValue());
}
template<typename T> const T func_MOVE(const T &pa_roIN) {
  return T(pa_roIN);
}

template<typename T, typename U> const T func_EXPT(const T &pa_roIN1, const U &pa_roIN2) {
  FORTE_STATIC_ASSERT((forte::core::mpl::is_base_of<CIEC_ANY_REAL, T>::value), In1NotOfTypeAnyReal);
  FORTE_STATIC_ASSERT((forte::core::mpl::is_base_of<CIEC_ANY_NUM, T>::value), In1NotOfTypeAnyNum);
  return T(pow(pa_roIN1, pa_roIN2));
}

//specialisation of function for base type double.
//TODO consider how this and similar problems can be better solved with type traits and more generically
inline double func_EXPT(double pa_roIN1, double pa_roIN2) {
  return pow(pa_roIN1, pa_roIN2);
}

template<typename T, typename U, template<typename A> class F, template<typename B> class G> typename forte::core::mpl::get_castable_type<T, U>::type APPLY_WITH_LITERALS(
    const T &pa_roIN1, const U &pa_roIN2) {
  typedef typename forte::core::mpl::get_castable_type<T, U>::type tImplicitCastType;
  FORTE_STATIC_ASSERT(!(forte::core::mpl::is_same<tImplicitCastType, forte::core::mpl::NullType>::value), NoImplicitCastPossible);
  const tImplicitCastType Result(
    forte::core::mpl::conditional<forte::core::mpl::and_<forte::core::mpl::is_scalar<T>, forte::core::mpl::is_scalar<T> >::value,
        F<typename forte::core::mpl::get_castable_type<T, U>::type>, G<typename forte::core::mpl::get_castable_type<T, U>::type> >::type::call(
      static_cast<tImplicitCastType>(pa_roIN1), static_cast<tImplicitCastType>(pa_roIN2)));
  return Result;
}

GENERATE_APPLY_FUNCTION(func_ADD)

template<typename T> struct func_ADD_Scalar {
    static T call(const T pa_roIN1, const T pa_roIN2) {
      return T(pa_roIN1 + pa_roIN2);
    }
};

template<typename T, typename U> typename forte::core::mpl::get_castable_type<T, U>::type func_ADD(const T &pa_roIN1, const U &pa_roIN2) {
  return APPLY_WITH_LITERALS<T, U, func_ADD_Scalar, func_ADD_Function>(pa_roIN1, pa_roIN2);
}

template<typename T> const T func_ADD(const T &pa_roIN1, const T &pa_roIN2) {
  return T(pa_roIN1 + pa_roIN2);
}

template <class T, class U>
class MulOperation {
  typedef typename forte::core::mpl::get_castable_type_duration_mixed_operations<T, U>::type resultType;

public:
  static resultType call(const T &pa_roIN1, const U &pa_roIN2) {
    return resultType(static_cast<resultType>(pa_roIN1) * static_cast<resultType>(pa_roIN2));
  }
  MulOperation() = delete;
};

template <class T>
class MulOperation<T, CIEC_TIME> {
  typedef typename forte::core::mpl::get_castable_type_duration_mixed_operations<T, CIEC_TIME>::type resultType;

public:
  static resultType call(const T &pa_roIN1, const CIEC_TIME &pa_roIN2) {
    return func_MULTIME(pa_roIN2, pa_roIN1);
  }
  MulOperation() = delete;
};

template <class T>
class MulOperation<CIEC_TIME, T>
{
  typedef typename forte::core::mpl::get_castable_type_duration_mixed_operations<T, CIEC_TIME>::type resultType;

public:
  static resultType call(const CIEC_TIME &pa_roIN1, const T &pa_roIN2) {
    return func_MULTIME(pa_roIN1, pa_roIN2);
  }

  MulOperation() = delete;
};

template <typename T, typename U>
typename forte::core::mpl::get_castable_type_duration_mixed_operations<T, U>::type func_MUL(const T &pa_roIN1, const U &pa_roIN2){
  return MulOperation<T, U>::call(pa_roIN1, pa_roIN2);
}

GENERATE_APPLY_FUNCTION(func_SUB)

    template <typename T>
    struct func_SUB_Scalar
{
  static T call(const T pa_roIN1, const T pa_roIN2)
  {
    return T(pa_roIN1 - pa_roIN2);
  }
};

template<typename T, typename U> typename forte::core::mpl::get_castable_type<T, U>::type func_SUB(const T &pa_roIN1, const U &pa_roIN2) {
  return APPLY_WITH_LITERALS<T, U, func_SUB_Scalar, func_SUB_Function>(pa_roIN1, pa_roIN2);
}

template<typename T> const T func_SUB(const T &pa_roIN1, const T &pa_roIN2) {
  return T(pa_roIN1 - pa_roIN2);
}

template <class T, class U>
class DivOperation {
  typedef typename forte::core::mpl::get_castable_type<T, U>::type resultType;

public:
  static resultType call(const T &pa_roIN1, const U &pa_roIN2) {
    if(0 != pa_roIN2) {
      return resultType(static_cast<resultType>(pa_roIN1) / static_cast<resultType>(pa_roIN2));
    }
    return resultType(0); // Divisor is 0
  }

  DivOperation() = delete;
};

// Time division only works with TIME as dividend
template <class T>
class DivOperation<CIEC_TIME, T> {
  typedef typename forte::core::mpl::get_castable_type_duration_mixed_operations<T, CIEC_TIME>::type resultType;

public:
  static resultType call(const CIEC_TIME &pa_roIN1, const T &pa_roIN2) {
    return func_DIVTIME(pa_roIN1, pa_roIN2);
  }

  DivOperation() = delete;
};

template <typename T, typename U>
typename forte::core::mpl::get_castable_type_duration_mixed_operations<T, U>::type func_DIV(const T &pa_roIN1, const U &pa_roIN2) {
  return DivOperation<T, U>::call(pa_roIN1, pa_roIN2);
}

template <typename T>
T func_MINUS(const T& paIN) {
  return paIN.operator-();
}

#ifdef FORTE_USE_REAL_DATATYPE
TForteInt32 func_TRUNC(const CIEC_REAL &pa_roIN);
#endif
#ifdef FORTE_USE_LREAL_DATATYPE
TForteInt64 func_TRUNC(const CIEC_LREAL &pa_roIN);
#endif

#ifdef FORTE_USE_64BIT_DATATYPES
const CIEC_TIME_OF_DAY func_ADD_TOD_TIME(const CIEC_TIME_OF_DAY& pa_roIN1, const CIEC_TIME& pa_roIN2);
const CIEC_DATE_AND_TIME func_ADD_DT_TIME(const CIEC_DATE_AND_TIME& pa_roIN1, const CIEC_TIME& pa_roIN2);
const CIEC_TIME_OF_DAY func_SUB_TOD_TIME(const CIEC_TIME_OF_DAY& pa_roIN1, const CIEC_TIME& pa_roIN2);
const CIEC_DATE_AND_TIME func_SUB_DT_TIME(const CIEC_DATE_AND_TIME& pa_roIN1, const CIEC_TIME& pa_roIN2);
const CIEC_TIME func_SUB_DT_DT(const CIEC_DATE_AND_TIME& pa_roIN1, const CIEC_DATE_AND_TIME& pa_roIN2);
const CIEC_TIME func_SUB_TOD_TOD(const CIEC_TIME_OF_DAY &pa_roIN1, const CIEC_TIME_OF_DAY &pa_roIN2);
const CIEC_TIME func_SUB_DATE_DATE(const CIEC_DATE &pa_roIN1, const CIEC_DATE &pa_roIN2);
const CIEC_DATE_AND_TIME func_CONCAT_DATE_TOD(const CIEC_DATE& pa_roIN1, const CIEC_TIME_OF_DAY& pa_roIN2);
#endif

#ifdef FORTE_USE_64BIT_DATATYPES
template<typename T> const CIEC_TIME func_MULTIME(const CIEC_TIME& pa_roIN1, const T& pa_roIN2){
  return CIEC_TIME(pa_roIN1 * pa_roIN2);
}
#else
template<typename T> const CIEC_TIME func_MULTIME(const CIEC_TIME &pa_roIN1, const T &pa_roIN2) {
  return CIEC_TIME(pa_roIN1 * pa_roIN2);
}
#endif

#ifdef FORTE_USE_64BIT_DATATYPES
template<typename T> const CIEC_TIME func_DIVTIME(const CIEC_TIME& pa_roIN1, const T& pa_roIN2){
  if(0 != pa_roIN2){
    return CIEC_TIME(pa_roIN1 / pa_roIN2);
  }else{
    return pa_roIN1;
  }
}
#else
template<typename T> const CIEC_TIME func_DIVTIME(const CIEC_TIME &pa_roIN1, const T &pa_roIN2) {
  if(0 != pa_roIN2) {
    return CIEC_TIME(pa_roIN1 / pa_roIN2);
  } else {
    return pa_roIN1;
  }
}
#endif

#ifdef FORTE_USE_64BIT_DATATYPES
template<typename T> CIEC_ULINT func_LEN(const T& pa_rsVal){
  return CIEC_ULINT(pa_rsVal.length());
}
#else
template<typename T> CIEC_UDINT func_LEN(const T &pa_rsVal) {
  return CIEC_UDINT(pa_rsVal.length());
}
#endif

template<typename T> const T func_LEFT(const T &pa_rsIn, const CIEC_ANY_INT &pa_roL) {
  if(true == pa_roL.isSigned() && 0 > pa_roL.getSignedValue()) {
    DEVLOG_ERROR("value of input L is less than zero");
    return pa_rsIn;
  } else {
    if(pa_rsIn.length() < pa_roL.getUnsignedValue()) {
      DEVLOG_ERROR("string shorter than input L");
      return pa_rsIn;
    } else {
      T temp;
      temp.reserve(static_cast<TForteUInt16>(pa_roL.getUnsignedValue()));
      memcpy(temp.getValue(), pa_rsIn.getValue(), static_cast<TForteUInt16>(pa_roL.getUnsignedValue()));
      temp.getValue()[pa_roL.getUnsignedValue()] = '\0';
      temp.assign(temp.getValue(), pa_roL.getUnsignedValue());
      return temp;
    }
  }
}

template<typename T> const T func_RIGHT(const T &pa_rsIn, const CIEC_ANY_INT &pa_roL) {
  if(true == pa_roL.isSigned() && 0 > pa_roL.getSignedValue()) {
    DEVLOG_ERROR("value of input L is less than zero");
    return pa_rsIn;
  } else {
    if(pa_rsIn.length() < pa_roL.getUnsignedValue()) {
      DEVLOG_ERROR("string shorter than input L");
      return pa_rsIn;
    } else {
      T temp;
      temp.reserve(static_cast<TForteUInt16>(pa_roL.getUnsignedValue()));
      memcpy(temp.getValue(), pa_rsIn.getValue() + (pa_rsIn.length() - static_cast<TForteUInt16>(pa_roL.getUnsignedValue())),
        static_cast<TForteUInt16>(pa_roL.getUnsignedValue()));
      temp.getValue()[pa_roL.getUnsignedValue()] = '\0';
      temp.assign(temp.getValue(), pa_roL.getUnsignedValue());
      return temp;
    }
  }
}

template<typename T> const T func_MID(const T &pa_rsIn, const CIEC_ANY_INT &pa_roL, const CIEC_ANY_INT &pa_roP) {
  if(true == pa_roP.isSigned() && 0 > pa_roP.getSignedValue()) {
    DEVLOG_ERROR("value of input P is less than zero\n");
    return pa_rsIn;
  } else {
    CIEC_INT len_right = CIEC_INT(pa_rsIn.length() - pa_roP.getUnsignedValue() + 1);
    CIEC_INT len_left = CIEC_INT(pa_roL.getUnsignedValue());
    return func_LEFT(func_RIGHT(pa_rsIn, len_right), len_left);
  }
}

template<typename T> const T func_CONCAT(const T &paIn1, const T &paIn2) {
  FORTE_STATIC_ASSERT((forte::core::mpl::is_base_of<CIEC_ANY_STRING, T>::value), TNotOfAnyString);
  T temp(paIn1);
  temp.reserve(static_cast<TForteUInt16>(paIn1.length() + paIn2.length()));
  temp.append(paIn2.getValue(), paIn2.length());
  return temp;
}

#if __cplusplus >= 201103L //stdc11
template<typename T, typename ... Args> const T func_CONCAT(const T &pa_rsIn1, Args ... args) {
  return func_CONCAT(pa_rsIn1, func_CONCAT(args...));
}
#endif

template<typename T> const T func_INSERT(const T &paIn1, const T &paIn2, const CIEC_ANY_INT &paP) {
  if(CIEC_UINT::scm_nMaxVal < (paIn1.length() + paIn2.length())) {
    DEVLOG_ERROR("result would be longer than maximum allowed length");
    return paIn1;
  }
  if(paP.isSigned() && paP.getSignedValue() < 0) {
    DEVLOG_ERROR("P has to be larger than 0!\n");
    return paIn1;
  }
  const CIEC_ANY::TLargestUIntValueType P = paP.isSigned() ? static_cast<CIEC_ANY::TLargestUIntValueType>(paP.getSignedValue()) : paP.getUnsignedValue();
  if(P > paIn1.length()) {
    DEVLOG_ERROR("P exceeds input string length!\n");
    return paIn1;
  }
  CIEC_INT positionRight = CIEC_INT(paIn1.length() - paP.getSignedValue());
  return func_CONCAT(func_CONCAT(func_LEFT(paIn1, paP), paIn2), func_RIGHT(paIn1, positionRight));
}

#ifdef DELETE
#undef DELETE
#endif

template<typename T> const T func_DELETE(const T &paIn, const CIEC_ANY_INT &paL, const CIEC_ANY_INT &paP) {
  if(paL.isSigned() && paL.getSignedValue() < 0) {
    DEVLOG_ERROR("L has to be larger than 0!\n");
    return paIn;
  }

  if(paP.isSigned() && paP.getSignedValue() < 0) {
    DEVLOG_ERROR("P has to be larger than 0!\n");
    return paIn;
  }

  const CIEC_ANY::TLargestUIntValueType L = paL.isSigned() ? static_cast<CIEC_ANY::TLargestUIntValueType>(paL.getSignedValue()) : paL.getUnsignedValue();
  const CIEC_ANY::TLargestUIntValueType P = paP.isSigned() ? static_cast<CIEC_ANY::TLargestUIntValueType>(paP.getSignedValue()) : paP.getUnsignedValue();

  if((P + L) > paIn.length()) {
    DEVLOG_ERROR("DELETE exceeds length of string!\n");
    return paIn;
  }

  CIEC_UINT positionRight = CIEC_UINT(paIn.length() - (L + P - 1));
  CIEC_UINT positionLeft = CIEC_UINT(P - 1);
  return func_CONCAT(func_LEFT(paIn, positionLeft), func_RIGHT(paIn, positionRight));
}

template<typename T> const T func_REPLACE(const T &paIn1, const T &paIn2, const CIEC_ANY_INT &paL, const CIEC_ANY_INT &paP) {
  if(paL.isSigned() && paL.getSignedValue() < 0) {
    DEVLOG_ERROR("L has to be larger than 0!\n");
    return paIn1;
  }

  if(paP.isSigned() && paP.getSignedValue() < 0) {
    DEVLOG_ERROR("P has to be larger than 0!\n");
    return paIn1;
  }

  const CIEC_ANY::TLargestUIntValueType L = paL.isSigned() ? static_cast<CIEC_ANY::TLargestUIntValueType>(paL.getSignedValue()) : paL.getUnsignedValue();
  const CIEC_ANY::TLargestUIntValueType P = paP.isSigned() ? static_cast<CIEC_ANY::TLargestUIntValueType>(paP.getSignedValue()) : paP.getUnsignedValue();

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

CIEC_ANY_INT func_FIND(const CIEC_ANY_STRING &pa_rsIn1, const CIEC_ANY_STRING &pa_rsIn2);

template <typename T>
const T func_TOUPPER(const T &paIn)
{
  T temp(paIn);
  char *current = temp.getValue();
  for(size_t i = 0; i <= paIn.length(); ++i) {
    current[i] = static_cast<char>(toupper(current[i]));
  }
  return temp;
}

template <typename T>
const T func_TOLOWER(const T &paIn)
{
  T temp(paIn);
  char *current = temp.getValue();
  for(size_t i = 0; i <= paIn.length(); ++i) {
    current[i] = static_cast<char>(tolower(current[i]));
  }
  return temp;
}

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

#endif /* IEC61131_FUNCTIONS_H_ */
