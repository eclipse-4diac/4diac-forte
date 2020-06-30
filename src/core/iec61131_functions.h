/*******************************************************************************
 * Copyright (c) 2010 - 2013 ACIN, Profactor GmbH, fortiss GmbH, 2018 TU Vienna/ACIN
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
const T ABS(const T &pa_roIN) {
  FORTE_STATIC_ASSERT((forte::core::mpl::is_base_of<CIEC_ANY_NUM, T>::value), TNotOfAnyNum);
  return (0 > pa_roIN) ? T(static_cast<typename T::TValueType>(pa_roIN * static_cast<typename T::TValueType>(-1))) : pa_roIN;
}

#ifdef FORTE_USE_REAL_DATATYPE
  inline const CIEC_REAL SQRT(const CIEC_REAL &pa_roIN){
    return CIEC_REAL(sqrtf(pa_roIN));
  }

# ifdef FORTE_USE_64BIT_DATATYPES
  inline const CIEC_LREAL SQRT(const CIEC_LREAL &pa_roIN){
    return CIEC_LREAL(sqrt(pa_roIN));
  }
# endif //FORTE_USE_64BIT_DATATYPES

#endif //#ifdef FORTE_USE_REAL_DATATYPE

#ifdef FORTE_USE_REAL_DATATYPE
  inline const CIEC_REAL LN(const CIEC_REAL &pa_roIN){
      return CIEC_REAL(logf(pa_roIN));
    }

# ifdef FORTE_USE_64BIT_DATATYPES
    inline const CIEC_LREAL LN(const CIEC_LREAL &pa_roIN){
      return CIEC_LREAL(log(pa_roIN));
    }
# endif //FORTE_USE_64BIT_DATATYPES

#endif //#ifdef FORTE_USE_REAL_DATATYPE

#ifdef FORTE_USE_REAL_DATATYPE
  inline const CIEC_REAL LOG(const CIEC_REAL &pa_roIN){
      return CIEC_REAL(log10f(pa_roIN));
    }

# ifdef FORTE_USE_64BIT_DATATYPES
    inline const CIEC_LREAL LOG(const CIEC_LREAL &pa_roIN){
      return CIEC_LREAL(log10(pa_roIN));
    }
# endif //FORTE_USE_64BIT_DATATYPES
#endif //#ifdef FORTE_USE_REAL_DATATYPE

#ifdef FORTE_USE_REAL_DATATYPE
  inline const CIEC_REAL SIN(const CIEC_REAL &pa_roIN){
      return CIEC_REAL(sinf(pa_roIN));
    }

# ifdef FORTE_USE_64BIT_DATATYPES
    inline const CIEC_LREAL SIN(const CIEC_LREAL &pa_roIN){
      return CIEC_LREAL(sin(pa_roIN));
    }
# endif //FORTE_USE_64BIT_DATATYPES

#endif //#ifdef FORTE_USE_REAL_DATATYPE

#ifdef FORTE_USE_REAL_DATATYPE
  inline const CIEC_REAL COS(const CIEC_REAL &pa_roIN){
      return CIEC_REAL(cosf(pa_roIN));
    }

# ifdef FORTE_USE_64BIT_DATATYPES
    inline const CIEC_LREAL COS(const CIEC_LREAL &pa_roIN){
      return CIEC_LREAL(cos(pa_roIN));
    }
# endif //FORTE_USE_64BIT_DATATYPES
#endif //#ifdef FORTE_USE_REAL_DATATYPE

#ifdef FORTE_USE_REAL_DATATYPE
  inline const CIEC_REAL TAN(const CIEC_REAL &pa_roIN){
      return CIEC_REAL(tanf(pa_roIN));
    }

# ifdef FORTE_USE_64BIT_DATATYPES
    inline const CIEC_LREAL TAN(const CIEC_LREAL &pa_roIN){
      return CIEC_LREAL(tan(pa_roIN));
    }
# endif //FORTE_USE_64BIT_DATATYPES
#endif //#ifdef FORTE_USE_REAL_DATATYPE

#ifdef FORTE_USE_REAL_DATATYPE
  inline const CIEC_REAL ASIN(const CIEC_REAL &pa_roIN){
      return CIEC_REAL(asinf(pa_roIN));
    }

# ifdef FORTE_USE_64BIT_DATATYPES
    inline const CIEC_LREAL ASIN(const CIEC_LREAL &pa_roIN){
      return CIEC_LREAL(asin(pa_roIN));
    }
# endif //FORTE_USE_64BIT_DATATYPES

#endif //#ifdef FORTE_USE_REAL_DATATYPE

#ifdef FORTE_USE_REAL_DATATYPE
  inline const CIEC_REAL ACOS(const CIEC_REAL &pa_roIN){
      return CIEC_REAL(acosf(pa_roIN));
    }

# ifdef FORTE_USE_64BIT_DATATYPES
    inline const CIEC_LREAL ACOS(const CIEC_LREAL &pa_roIN){
      return CIEC_LREAL(acos(pa_roIN));
    }
# endif //FORTE_USE_64BIT_DATATYPES
#endif //#ifdef FORTE_USE_REAL_DATATYPE

#ifdef FORTE_USE_REAL_DATATYPE
  inline const CIEC_REAL ATAN(const CIEC_REAL &pa_roIN){
      return CIEC_REAL(atanf(pa_roIN));
    }

# ifdef FORTE_USE_64BIT_DATATYPES
    inline const CIEC_LREAL ATAN(const CIEC_LREAL &pa_roIN){
      return CIEC_LREAL(atan(pa_roIN));
    }
# endif //FORTE_USE_64BIT_DATATYPES
#endif //#ifdef FORTE_USE_REAL_DATATYPE

#ifdef FORTE_USE_REAL_DATATYPE
  inline const CIEC_REAL EXP(const CIEC_REAL &pa_roIN){
      return CIEC_REAL(expf(pa_roIN));
    }

# ifdef FORTE_USE_64BIT_DATATYPES
    inline const CIEC_LREAL EXP(const CIEC_LREAL &pa_roIN){
      return CIEC_LREAL(exp(pa_roIN));
    }
# endif //FORTE_USE_64BIT_DATATYPES
#endif //#ifdef FORTE_USE_REAL_DATATYPE

template<typename T> const T ROL(const T &paIn, const CIEC_ANY_INT &paN) {
  FORTE_STATIC_ASSERT((forte::core::mpl::is_base_of<CIEC_ANY_BIT, T>::value), TNotOfAnyBit);
  if((true == paN.isSigned() && 0 <= paN.getSignedValue()) || false == paN.isSigned()) {
    return T(static_cast<typename T::TValueType>((paIn << paN.getUnsignedValue()) | (paIn >> (sizeof(typename T::TValueType) * 8 - paN.getUnsignedValue()))));
  }
  DEVLOG_ERROR("value of input N is less than zero");
  return T(static_cast<typename T::TValueType>(0));
}

template<typename T> const T ROR(const T &paIn, const CIEC_ANY_INT &paN) {
  FORTE_STATIC_ASSERT((forte::core::mpl::is_base_of<CIEC_ANY_BIT, T>::value), TNotOfAnyBit);
  if((true == paN.isSigned() && 0 <= paN.getSignedValue()) || false == paN.isSigned()) {
    return T(static_cast<typename T::TValueType>((paIn >> paN.getUnsignedValue()) | (paIn << (sizeof(typename T::TValueType) * 8 - paN.getUnsignedValue()))));
  }
  DEVLOG_ERROR("value of input N is less than zero");
  return T(static_cast<typename T::TValueType>(0));
}

template<typename T> const T SHL(const T &paIn, const CIEC_ANY_INT &paN) {
  FORTE_STATIC_ASSERT((forte::core::mpl::is_base_of<CIEC_ANY_BIT, T>::value), TNotOfAnyBit);
  if((true == paN.isSigned() && 0 <= paN.getSignedValue()) || false == paN.isSigned()) {
    return T(static_cast<typename T::TValueType>(paIn << paN.getUnsignedValue()));
  }
  DEVLOG_ERROR("value of input N is less than zero");
  return T(static_cast<typename T::TValueType>(0));
}
template<typename T> const T SHR(const T &paIn, const CIEC_ANY_INT &paN) {
  FORTE_STATIC_ASSERT((forte::core::mpl::is_base_of<CIEC_ANY_BIT, T>::value), TNotOfAnyBit);
  if((true == paN.isSigned() && 0 <= paN.getSignedValue()) || false == paN.isSigned()) {
    return T(static_cast<typename T::TValueType>(paIn >> paN.getUnsignedValue()));
  }
  DEVLOG_ERROR("value of input N is less than zero");
  return T(static_cast<typename T::TValueType>(0));
}

template<> const CIEC_BOOL ROL(const CIEC_BOOL &paIn, const CIEC_ANY_INT &paN);

template<> const CIEC_BOOL ROR(const CIEC_BOOL &paIn, const CIEC_ANY_INT &paN);

template<> const CIEC_BOOL SHL(const CIEC_BOOL &paIn, const CIEC_ANY_INT &paN);

template<> const CIEC_BOOL SHR(const CIEC_BOOL &paIn, const CIEC_ANY_INT &paN);

template<typename T, typename U, template<typename A> class F, typename C> typename forte::core::mpl::get_castable_type<T, U>::type APPLY(const T &pa_roIN1,
    const U &pa_roIN2) {
  FORTE_STATIC_ASSERT((forte::core::mpl::are_of_subtype<C, T, U>::value), TemplateInstantiationWithIncompatibleTypes);
  typedef typename forte::core::mpl::get_castable_type<T, U>::type tImplicitCastType;
  FORTE_STATIC_ASSERT(!(forte::core::mpl::is_same<tImplicitCastType, forte::core::mpl::NullType>::value), NoImplicitCastPossible);
  const tImplicitCastType Result(F<tImplicitCastType>::call(static_cast<tImplicitCastType>(pa_roIN1), static_cast<tImplicitCastType>(pa_roIN2)));
  return Result;
}

GENERATE_APPLY_FUNCTION(AND)
template<typename T, typename U> typename forte::core::mpl::get_castable_type<T, U>::type AND(const T &pa_roIN1, const U &pa_roIN2) {
  return APPLY<T, U, AND_Function, CIEC_ANY_BIT>(pa_roIN1, pa_roIN2);
}

template<typename T> const T AND(const T &pa_roIN1, const T &pa_roIN2) {
  return T((typename T::TValueType) (pa_roIN1 & pa_roIN2));
}

GENERATE_APPLY_FUNCTION(OR)
template<typename T, typename U> typename forte::core::mpl::get_castable_type<T, U>::type OR(const T &pa_roIN1, const U &pa_roIN2) {
  return APPLY<T, U, OR_Function, CIEC_ANY_BIT>(pa_roIN1, pa_roIN2);
}

template<typename T> const T OR(const T &pa_roIN1, const T &pa_roIN2) {
  return T((typename T::TValueType) (pa_roIN1 | pa_roIN2));
}

GENERATE_APPLY_FUNCTION(XOR)
template<typename T, typename U> typename forte::core::mpl::get_castable_type<T, U>::type XOR(const T &pa_roIN1, const U &pa_roIN2) {
  return APPLY<T, U, XOR_Function, CIEC_ANY_BIT>(pa_roIN1, pa_roIN2);
}

template<typename T> const T XOR(const T &pa_roIN1, const T &pa_roIN2) {
  return T((typename T::TValueType) (pa_roIN1 ^ pa_roIN2));
}

template<typename T> const T NOT(const T &pa_roIN) {
  FORTE_STATIC_ASSERT((forte::core::mpl::is_base_of<CIEC_ANY_BIT, T>::value), NotOnlyForCiecAnyBit);
  return T((typename T::TValueType) (~pa_roIN));
}

template<> const CIEC_BOOL NOT<CIEC_BOOL>(const CIEC_BOOL &pa_roIN);

GENERATE_APPLY_FUNCTION(GT)
template<typename T, typename U> typename forte::core::mpl::get_castable_type<T, U>::type GT(const T &pa_roIN1, const U &pa_roIN2) {
  return APPLY<T, U, GT_Function, CIEC_ANY_ELEMENTARY>(pa_roIN1, pa_roIN2);
}

template<typename T> const CIEC_BOOL GT(const T &pa_roIN1, const T &pa_roIN2) {
  CIEC_BOOL temp = false;
  if(pa_roIN1 > pa_roIN2) {
    temp = true;
  }
  return temp;
}

GENERATE_APPLY_FUNCTION(EQ)
template<typename T, typename U> typename forte::core::mpl::get_castable_type<T, U>::type EQ(const T &pa_roIN1, const U &pa_roIN2) {
  return APPLY<T, U, EQ_Function, CIEC_ANY_ELEMENTARY>(pa_roIN1, pa_roIN2);
}

template<typename T> const CIEC_BOOL EQ(const T &pa_roIN1, const T &pa_roIN2) {
  CIEC_BOOL temp = false;
  if(pa_roIN1 == pa_roIN2) {
    temp = true;
  }
  return temp;
}

GENERATE_APPLY_FUNCTION(GE)
template<typename T, typename U> typename forte::core::mpl::get_castable_type<T, U>::type GE(const T &pa_roIN1, const U &pa_roIN2) {
  return APPLY<T, U, GE_Function, CIEC_ANY_ELEMENTARY>(pa_roIN1, pa_roIN2);
}

template<typename T> const CIEC_BOOL GE(const T &pa_roIN1, const T &pa_roIN2) {
  CIEC_BOOL temp = false;
  if(pa_roIN1 >= pa_roIN2) {
    temp = true;
  }
  return temp;
}

GENERATE_APPLY_FUNCTION(LE)
template<typename T, typename U> typename forte::core::mpl::get_castable_type<T, U>::type LE(const T &pa_roIN1, const U &pa_roIN2) {
  return APPLY<T, U, LE_Function, CIEC_ANY_ELEMENTARY>(pa_roIN1, pa_roIN2);
}

template<typename T> const CIEC_BOOL LE(const T &pa_roIN1, const T &pa_roIN2) {
  CIEC_BOOL temp = false;
  if(pa_roIN1 <= pa_roIN2) {
    temp = true;
  }
  return temp;
}

GENERATE_APPLY_FUNCTION(LT)
template<typename T, typename U> typename forte::core::mpl::get_castable_type<T, U>::type LT(const T &pa_roIN1, const U &pa_roIN2) {
  return APPLY<T, U, LT_Function, CIEC_ANY_ELEMENTARY>(pa_roIN1, pa_roIN2);
}

template<typename T> const CIEC_BOOL LT(const T &pa_roIN1, const T &pa_roIN2) {
  CIEC_BOOL temp = false;
  if(pa_roIN1 < pa_roIN2) {
    temp = true;
  }
  return temp;
}

GENERATE_APPLY_FUNCTION(NE)
template<typename T, typename U> typename forte::core::mpl::get_castable_type<T, U>::type NE(const T &pa_roIN1, const U &pa_roIN2) {
  return APPLY<T, U, NE_Function, CIEC_ANY_ELEMENTARY>(pa_roIN1, pa_roIN2);
}

template<typename T> const CIEC_BOOL NE(const T &pa_roIN1, const T &pa_roIN2) {
  CIEC_BOOL temp = false;
  if(pa_roIN1 != pa_roIN2) {
    temp = true;
  }
  return temp;
}

template<typename T, typename U> const typename forte::core::mpl::get_castable_type<T, U>::type SEL(const CIEC_BOOL &G, const T &IN0, const U &IN1) {
  return (G.operator bool()) ? IN1 : IN0;
}

GENERATE_APPLY_FUNCTION(MAX)
template<typename T, typename U> typename forte::core::mpl::get_castable_type<T, U>::type MAX(const T &pa_roIN1, const U &pa_roIN2) {
  return APPLY<T, U, MAX_Function, CIEC_ANY_ELEMENTARY>(pa_roIN1, pa_roIN2);
}

template<typename T> const T MAX(const T &pa_roIN1, const T &pa_roIN2) {
  if(pa_roIN1 > pa_roIN2) {
    return pa_roIN1;
  } else {
    return pa_roIN2;
  }
}

GENERATE_APPLY_FUNCTION(MIN)
template<typename T, typename U> typename forte::core::mpl::get_castable_type<T, U>::type MIN(const T &pa_roIN1, const U &pa_roIN2) {
  return APPLY<T, U, MAX_Function, CIEC_ANY_ELEMENTARY>(pa_roIN1, pa_roIN2);
}

template<typename T> const T MIN(const T &pa_roIN1, const T &pa_roIN2) {
  if(pa_roIN1 < pa_roIN2) {
    return pa_roIN1;
  } else {
    return pa_roIN2;
  }
}

template<typename T, typename U, typename V>
const typename forte::core::mpl::get_castable_type<typename forte::core::mpl::get_castable_type<T, U>::type, V>::type LIMIT(const T &pa_roMN, const U &pa_roIN,
    const V &pa_roMX) {
  typedef typename forte::core::mpl::get_castable_type<typename forte::core::mpl::get_castable_type<T, U>::type, V>::type tImplicitType;
  return MIN(MAX(static_cast<tImplicitType>(pa_roIN), static_cast<tImplicitType>(pa_roMN)), static_cast<tImplicitType>(pa_roMX));
}

GENERATE_APPLY_FUNCTION(MOD)
template<typename T, typename U> typename forte::core::mpl::get_castable_type<T, U>::type MOD(const T &pa_roIN1, const U &pa_roIN2) {
  return APPLY<T, U, MOD_Function, CIEC_ANY_INT>(pa_roIN1, pa_roIN2);
}

template<typename T> const T MOD(const T &pa_roIN1, const T &pa_roIN2) {
  if(0 == pa_roIN2) {
    return static_cast<typename T::TValueType>(0);
  }
  return static_cast<typename T::TValueType>(pa_roIN1.getSignedValue() % pa_roIN2.getSignedValue());
}
template<typename T> const T MOVE(const T &pa_roIN) {
  return static_cast<typename T::TValueType>(pa_roIN);
}

template<typename T, typename U> const T EXPT(const T &pa_roIN1, const U &pa_roIN2) {
  FORTE_STATIC_ASSERT((forte::core::mpl::is_base_of<CIEC_ANY_REAL, T>::value), In1NotOfTypeAnyReal);
  FORTE_STATIC_ASSERT((forte::core::mpl::is_base_of<CIEC_ANY_NUM, T>::value), In1NotOfTypeAnyNum);
  return static_cast<typename T::TValueType>(pow(pa_roIN1, pa_roIN2));
}

//specialisation of function for base type double.
//TODO consider how this and similar problems can be better solved with type traits and more generically
inline double EXPT(double pa_roIN1, double pa_roIN2) {
  return pow(pa_roIN1, pa_roIN2);
}

template<typename T, typename U, template<typename A> class F, template<typename B> class G> typename forte::core::mpl::get_castable_type<T, U>::type APPLY_WITH_LITERALS(
    const T &pa_roIN1, const U &pa_roIN2) {
  typedef typename forte::core::mpl::get_castable_type<T, U>::type tImplicitCastType;
  FORTE_STATIC_ASSERT(!(forte::core::mpl::is_same<tImplicitCastType, forte::core::mpl::NullType>::value), NoImplicitCastPossible);
  const tImplicitCastType Result(
    forte::core::mpl::conditional<forte::core::mpl::and_<forte::core::mpl::is_scalar<T>, forte::core::mpl::is_scalar<T> >::value,
        F<typename forte::core::mpl::get_castable_type<T, U>::type>,
        G<typename forte::core::mpl::get_castable_type<T, U>::type> >::type::call(static_cast<tImplicitCastType>(pa_roIN1),static_cast<tImplicitCastType>(pa_roIN2)));
  return Result;
}

GENERATE_APPLY_FUNCTION(ADD)

template<typename T> struct ADD_Scalar {
    static T call(const T pa_roIN1, const T pa_roIN2) {
      return pa_roIN1 + pa_roIN2;
    }
};

template<typename T, typename U> typename forte::core::mpl::get_castable_type<T, U>::type ADD(const T &pa_roIN1, const U &pa_roIN2) {
  return APPLY_WITH_LITERALS<T, U, ADD_Scalar, ADD_Function>(pa_roIN1, pa_roIN2);
}

template<typename T> const T ADD(const T &pa_roIN1, const T &pa_roIN2) {
  return pa_roIN1 + pa_roIN2;
}

GENERATE_APPLY_FUNCTION(MUL)

template<typename T> struct MUL_Scalar {
    static T call(const T pa_roIN1, const T pa_roIN2) {
      return pa_roIN1 * pa_roIN2;
    }
};

template<typename T, typename U> typename forte::core::mpl::get_castable_type<T, U>::type MUL(const T &pa_roIN1, const U &pa_roIN2) {
  return APPLY_WITH_LITERALS<T, U, MUL_Scalar, MUL_Function>(pa_roIN1, pa_roIN2);
}

template<typename T> const T MUL(const T &pa_roIN1, const T &pa_roIN2) {
  return pa_roIN1 * pa_roIN2;
}

GENERATE_APPLY_FUNCTION(SUB)

template<typename T> struct SUB_Scalar {
    static T call(const T pa_roIN1, const T pa_roIN2) {
      return pa_roIN1 - pa_roIN2;
    }
};

template<typename T, typename U> typename forte::core::mpl::get_castable_type<T, U>::type SUB(const T &pa_roIN1, const U &pa_roIN2) {
  return APPLY_WITH_LITERALS<T, U, SUB_Scalar, SUB_Function>(pa_roIN1, pa_roIN2);
}

template<typename T> const T SUB(const T &pa_roIN1, const T &pa_roIN2) {
  return pa_roIN1 - pa_roIN2;
}

GENERATE_APPLY_FUNCTION(DIV)

template<typename T> struct DIV_Scalar {
    static T call(const T pa_roIN1, const T pa_roIN2) {
      T temp(0);
      if(0 != static_cast<typename T::TValueType>(pa_roIN2)) {
        temp = static_cast<typename T::TValueType>(pa_roIN1) / static_cast<typename T::TValueType>(pa_roIN2);
      }
      return temp;
    }
};

template<typename T, typename U> typename forte::core::mpl::get_castable_type<T, U>::type DIV(const T &pa_roIN1, const U &pa_roIN2) {
  return APPLY_WITH_LITERALS<T, U, DIV_Scalar, DIV_Function>(pa_roIN1, pa_roIN2);
}

template<typename T> const T DIV(const T &pa_roIN1, const T &pa_roIN2) {
  T temp(0);
  if(0 != static_cast<typename T::TValueType>(pa_roIN2)) {
    temp = static_cast<typename T::TValueType>(pa_roIN1) / static_cast<typename T::TValueType>(pa_roIN2);
  }
  return temp;
}

#ifdef FORTE_USE_REAL_DATATYPE
TForteInt32 TRUNC(const CIEC_REAL& pa_roIN);
#endif
#ifdef FORTE_USE_LREAL_DATATYPE
TForteInt64 TRUNC(const CIEC_LREAL& pa_roIN);
#endif

#ifdef FORTE_USE_64BIT_DATATYPES
const CIEC_TIME_OF_DAY ADD_TOD_TIME(const CIEC_TIME_OF_DAY& pa_roIN1, const CIEC_TIME& pa_roIN2);
const CIEC_DATE_AND_TIME ADD_DT_TIME(const CIEC_DATE_AND_TIME& pa_roIN1, const CIEC_TIME& pa_roIN2);
const CIEC_TIME_OF_DAY SUB_TOD_TIME(const CIEC_TIME_OF_DAY& pa_roIN1, const CIEC_TIME& pa_roIN2);
const CIEC_DATE_AND_TIME SUB_DT_TIME(const CIEC_DATE_AND_TIME& pa_roIN1, const CIEC_TIME& pa_roIN2);
const CIEC_TIME SUB_DT_DT(const CIEC_DATE_AND_TIME& pa_roIN1, const CIEC_DATE_AND_TIME& pa_roIN2);
const CIEC_TIME SUB_TOD_TOD(const CIEC_TIME_OF_DAY &pa_roIN1, const CIEC_TIME_OF_DAY &pa_roIN2);
const CIEC_TIME SUB_DATE_DATE(const CIEC_DATE &pa_roIN1, const CIEC_DATE &pa_roIN2);
const CIEC_DATE_AND_TIME CONCAT_DATE_TOD(const CIEC_DATE& pa_roIN1, const CIEC_TIME_OF_DAY& pa_roIN2);
#endif

#ifdef FORTE_USE_64BIT_DATATYPES
template<typename T> const CIEC_TIME MULTIME(const CIEC_TIME& pa_roIN1, const T& pa_roIN2){
  return static_cast<TForteInt64>(pa_roIN1 * pa_roIN2);
}
#else
template<typename T> const CIEC_TIME MULTIME(const CIEC_TIME &pa_roIN1, const T &pa_roIN2) {
  return static_cast<TForteInt32>(pa_roIN1 * pa_roIN2);
}
#endif

#ifdef FORTE_USE_64BIT_DATATYPES
template<typename T> const CIEC_TIME DIVTIME(const CIEC_TIME& pa_roIN1, const T& pa_roIN2){
  if(0 != pa_roIN2){
    return static_cast<TForteInt64>(pa_roIN1 / pa_roIN2);
  }else{
    return pa_roIN1;
  }
}
#else
template<typename T> const CIEC_TIME DIVTIME(const CIEC_TIME &pa_roIN1, const T &pa_roIN2) {
  if(0 != pa_roIN2) {
    return static_cast<TForteInt32>(pa_roIN1 / pa_roIN2);
  } else {
    return pa_roIN1;
  }
}
#endif

#ifdef FORTE_USE_64BIT_DATATYPES
template<typename T> CIEC_ULINT LEN(const T& pa_rsVal){
  return CIEC_ULINT(pa_rsVal.length());
}
#else
template<typename T> CIEC_UDINT LEN(const T &pa_rsVal) {
  return CIEC_UDINT(pa_rsVal.length());
}
#endif

template<typename T> const T LEFT(const T &pa_rsIn, const CIEC_ANY_INT &pa_roL) {
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
      return temp.getValue();
    }
  }
}
template<typename T> const T RIGHT(const T &pa_rsIn, const CIEC_ANY_INT &pa_roL) {
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
      return temp.getValue();
    }
  }
}

template<typename T> const T MID(const T &pa_rsIn, const CIEC_ANY_INT &pa_roL, const CIEC_ANY_INT &pa_roP) {
  if(true == pa_roP.isSigned() && 0 > pa_roP.getSignedValue()) {
    DEVLOG_ERROR("value of input P is less than zero\n");
    return pa_rsIn;
  } else {
    CIEC_INT len_right = static_cast<TForteInt16>(pa_rsIn.length() - pa_roP.getUnsignedValue() + 1);
    CIEC_INT len_left = static_cast<TForteInt16>(pa_roL.getUnsignedValue());
    return LEFT(RIGHT(pa_rsIn, len_right), len_left);
  }
}

template<typename T> const T CONCAT(const T &paIn1, const T &paIn2) {
  FORTE_STATIC_ASSERT((forte::core::mpl::is_base_of<CIEC_ANY_STRING, T>::value), TNotOfAnyString);
  T temp(paIn1);
  temp.reserve(static_cast<TForteUInt16>(paIn1.length() + paIn2.length()));
  temp.append(paIn2.getValue(), paIn2.length());
  return temp;
}

#if __cplusplus >= 201103L //stdc11
template<typename T, typename ... Args> const T CONCAT(const T &pa_rsIn1, Args ... args) {
  return CONCAT(pa_rsIn1, CONCAT(args...));
}
#endif

template<typename T> const T INSERT(const T &paIn1, const T &paIn2, const CIEC_ANY_INT &paP) {
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
  CIEC_INT positionRight = static_cast<TForteInt16>(paIn1.length() - paP.getSignedValue());
  return CONCAT(CONCAT(LEFT(paIn1, paP), paIn2), RIGHT(paIn1, positionRight));
}

#ifdef DELETE
#undef DELETE
#endif

template<typename T> const T DELETE(const T &paIn, const CIEC_ANY_INT &paL, const CIEC_ANY_INT &paP) {
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

  CIEC_UINT positionRight = static_cast<CIEC_UINT::TValueType>(paIn.length() - (L + P - 1));
  CIEC_UINT positionLeft = static_cast<CIEC_UINT::TValueType>(P - 1);
  return CONCAT(LEFT(paIn, positionLeft), RIGHT(paIn, positionRight));
}

template<typename T> const T REPLACE(const T &paIn1, const T &paIn2, const CIEC_ANY_INT &paL, const CIEC_ANY_INT &paP) {
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

  CIEC_INT positionRight = static_cast<CIEC_INT::TValueType>(paIn1.length() - (L + P - 1));
  CIEC_INT positionLeft = static_cast<CIEC_INT::TValueType>(P - 1);
  return CONCAT(CONCAT(LEFT(paIn1, positionLeft), paIn2), RIGHT(paIn1, positionRight));
}

CIEC_ANY_INT FIND(const CIEC_ANY_STRING &pa_rsIn1, const CIEC_ANY_STRING &pa_rsIn2);

template<typename T> const T TOUPPER(const T &paIn) {
  T temp(paIn);
  char *current = temp.getValue();
  for(size_t i = 0; i <= paIn.length(); ++i) {
    current[i] = static_cast<char>(toupper(current[i]));
  }
  return temp;
}

template<typename T> const T TOLOWER(const T &paIn) {
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
const CIEC_TIME NOW_MONOTONIC();

/**
 * @brief returns current local date and time - may be subject to time jumps
 *
 * @return CIEC_DATE_AND_TIME of the current local time
 */
const CIEC_DATE_AND_TIME NOW();

#endif /* IEC61131_FUNCTIONS_H_ */
