/*******************************************************************************
 * Copyright (c) 2019 TU Vienna/ACIN
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Martin Melik-Merkumians - adds non standard time to number conversions
 *******************************************************************************/

#ifndef SRC_CORE_DATATYPES_CONVERT_TIMEINRESOLUTIONCONVERTFUNCTIONS_H_
#define SRC_CORE_DATATYPES_CONVERT_TIMEINRESOLUTIONCONVERTFUNCTIONS_H_

#include "iec61131_cast_helper.h"
#include "forte_constants.h"

/*********************************************************************************************
 *   TIME_IN_<RESOLUTION>_TO_* functions - custom non-standard functions
 *********************************************************************************************/
#ifdef FORTE_USE_64BIT_DATATYPES

template<typename T>
inline const T TIME_IN_S_TO(const CIEC_TIME &paValue) {
  FORTE_STATIC_ASSERT((forte::core::mpl::is_base_of<CIEC_ANY_NUM, T>::value), TNotOfAnyNum);
  if(forte::core::mpl::is_base_of<CIEC_ANY_REAL, T>::value) {
    return T(
      static_cast<typename T::TValueType>(paValue.getInNanoSeconds()) / static_cast<typename T::TValueType>(forte::core::constants::cNanosecondsPerSecond));
  } else {
    return T(static_cast<typename T::TValueType>(paValue.getInSeconds()));
  }
}

template<typename T>
inline const T TIME_IN_MS_TO(const CIEC_TIME &paValue) {
  FORTE_STATIC_ASSERT((forte::core::mpl::is_base_of<CIEC_ANY_NUM, T>::value), TNotOfAnyNum);
  if(forte::core::mpl::is_base_of<CIEC_ANY_REAL, T>::value) {
    return T(
      static_cast<typename T::TValueType>(paValue.getInNanoSeconds())
        * (static_cast<typename T::TValueType>(forte::core::constants::cMillisecondsPerSecond)
          / static_cast<typename T::TValueType>(forte::core::constants::cNanosecondsPerSecond)));
  } else {
    return T(static_cast<typename T::TValueType>(paValue.getInMilliSeconds()));
  }
}

template<typename T>
inline const T TIME_IN_US_TO(const CIEC_TIME &paValue) {
  FORTE_STATIC_ASSERT((forte::core::mpl::is_base_of<CIEC_ANY_NUM, T>::value), TNotOfAnyNum);
  if(forte::core::mpl::is_base_of<CIEC_ANY_REAL, T>::value) {
    return T(
      static_cast<typename T::TValueType>(paValue.getInNanoSeconds())
        * (static_cast<typename T::TValueType>(forte::core::constants::cMicrosecondsPerSecond)
          / static_cast<typename T::TValueType>(forte::core::constants::cNanosecondsPerSecond)));
  } else {
    return T(static_cast<typename T::TValueType>(paValue.getInMicroSeconds()));
  }
}

template<typename T>
inline const T TIME_IN_NS_TO(const CIEC_TIME &paValue) {
  return T(static_cast<typename T::TValueType>(paValue.getInNanoSeconds()));
}

/********************************************************************
 *  TO_LINT functions
 ********************************************************************/

inline const CIEC_LINT TIME_IN_S_TO_LINT(const CIEC_TIME &paValue) {
  return TIME_IN_S_TO<CIEC_LINT>(paValue);
}

inline const CIEC_LINT TIME_IN_MS_TO_LINT(const CIEC_TIME &paValue) {
  return TIME_IN_MS_TO<CIEC_LINT>(paValue);
}

inline const CIEC_LINT TIME_IN_US_TO_LINT(const CIEC_TIME &paValue) {
  return TIME_IN_US_TO<CIEC_LINT>(paValue);
}

inline const CIEC_LINT TIME_IN_NS_TO_LINT(const CIEC_TIME &paValue) {
  return TIME_IN_NS_TO<CIEC_LINT>(paValue);
}

/********************************************************************
 *  TO_UINT functions
 ********************************************************************/

inline const CIEC_ULINT TIME_IN_S_TO_ULINT(const CIEC_TIME &paValue) {
  return TIME_IN_S_TO<CIEC_ULINT>(paValue);
}

inline const CIEC_ULINT TIME_IN_MS_TO_ULINT(const CIEC_TIME &paValue) {
  return TIME_IN_MS_TO<CIEC_ULINT>(paValue);
}

inline const CIEC_ULINT TIME_IN_US_TO_ULINT(const CIEC_TIME &paValue) {
  return TIME_IN_US_TO<CIEC_ULINT>(paValue);
}

inline const CIEC_ULINT TIME_IN_NS_TO_ULINT(const CIEC_TIME &paValue) {
  return TIME_IN_NS_TO<CIEC_ULINT>(paValue);
}

/********************************************************************
 *  TO_LREAL functions
 ********************************************************************/

inline const CIEC_LREAL TIME_IN_S_TO_LREAL(const CIEC_TIME &paValue) {
  return TIME_IN_S_TO<CIEC_LREAL>(paValue);
}

inline const CIEC_LREAL TIME_IN_MS_TO_LREAL(const CIEC_TIME &paValue) {
  return TIME_IN_MS_TO<CIEC_LREAL>(paValue);
}

inline const CIEC_LREAL TIME_IN_US_TO_LREAL(const CIEC_TIME &paValue) {
  return TIME_IN_US_TO<CIEC_LREAL>(paValue);
}

#endif /* FORTE_USE_64BIT_DATATYPES */

#endif /* SRC_CORE_DATATYPES_CONVERT_TIMEINRESOLUTIONCONVERTFUNCTIONS_H_ */
