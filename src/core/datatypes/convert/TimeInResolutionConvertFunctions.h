/*******************************************************************************
 * Copyright (c) 2019 TU Vienna/ACIN
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
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

/**
 * @brief template function for converting TIME in seconds to ANY_NUM type
 *
 * Template function for converting TIME values in seconds to a ANY_NUM type
 * A static compile guard is checking if the goal type is a subclass of ANY_NUM
 * to prevent invalid conversion
 *
 * @param paValue TIME value to convert
 * @return TIME value in seconds
 */
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

/**
 * @brief template function for converting TIME in milliseconds to ANY_NUM type
 *
 * Template function for converting TIME values in milliseconds to a ANY_NUM type
 * A static compile guard is checking if the goal type is a subclass of ANY_NUM
 * to prevent invalid conversion
 *
 * @param paValue TIME value to convert
 * @return TIME value in milliseconds
 */
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

/**
 * @brief template function for converting TIME in microseconds to ANY_NUM type
 *
 * Template function for converting TIME values in microseconds to a ANY_NUM type
 * A static compile guard is checking if the goal type is a subclass of ANY_NUM
 * to prevent invalid conversion
 *
 * @param paValue TIME value to convert
 * @return TIME value in microseconds
 */
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

/**
 * @brief template function for converting TIME in nanoseconds to ANY_NUM type
 *
 * Template function for converting TIME values in nanoseconds to a ANY_NUM type
 * A static compile guard is checking if the goal type is a subclass of ANY_NUM
 * to prevent invalid conversion
 *
 * @param paValue TIME value to convert
 * @return TIME value in nanoseconds
 */
template<typename T>
inline const T TIME_IN_NS_TO(const CIEC_TIME &paValue) {
  FORTE_STATIC_ASSERT((forte::core::mpl::is_base_of<CIEC_ANY_NUM, T>::value), TNotOfAnyNum);
  return T(static_cast<typename T::TValueType>(paValue.getInNanoSeconds()));
}

/********************************************************************
 *  TO_LINT functions
 ********************************************************************/
/**
 * @brief conversion function from TIME to LINT in seconds
 *
 *
 * @param paValue TIME value to convert
 * @return TIME value in seconds
 */
inline const CIEC_LINT TIME_IN_S_TO_LINT(const CIEC_TIME &paValue) {
  return TIME_IN_S_TO<CIEC_LINT>(paValue);
}

/**
 * @brief conversion function from TIME to LINT in milliseconds
 *
 * @param paValue TIME value to convert
 * @return TIME value in milliseconds
 */
inline const CIEC_LINT TIME_IN_MS_TO_LINT(const CIEC_TIME &paValue) {
  return TIME_IN_MS_TO<CIEC_LINT>(paValue);
}

/**
 * @brief conversion function from TIME to LINT in microseconds
 *
 * @param paValue TIME value to convert
 * @return TIME value in microseconds
 */
inline const CIEC_LINT TIME_IN_US_TO_LINT(const CIEC_TIME &paValue) {
  return TIME_IN_US_TO<CIEC_LINT>(paValue);
}

/**
 * @brief conversion function from TIME to LINT in nanoseconds
 *
 * @param paValue TIME value to convert
 * @return TIME value in nanoseconds
 */
inline const CIEC_LINT TIME_IN_NS_TO_LINT(const CIEC_TIME &paValue) {
  return TIME_IN_NS_TO<CIEC_LINT>(paValue);
}

/********************************************************************
 *  TO_ULINT functions
 ********************************************************************/

/**
 * @brief conversion function from TIME to ULINT in seconds
 *
 * @param paValue TIME value to convert
 * @return TIME value in seconds
 */
inline const CIEC_ULINT TIME_IN_S_TO_ULINT(const CIEC_TIME &paValue) {
  return TIME_IN_S_TO<CIEC_ULINT>(paValue);
}

/**
 * @brief conversion function from TIME to ULINT in milliseconds
 *
 * @param paValue TIME value to convert
 * @return TIME value in milliseconds
 */
inline const CIEC_ULINT TIME_IN_MS_TO_ULINT(const CIEC_TIME &paValue) {
  return TIME_IN_MS_TO<CIEC_ULINT>(paValue);
}

/**
 * @brief conversion function from TIME to ULINT in microseconds
 *
 * @param paValue TIME value to convert
 * @return TIME value in microseconds
 */
inline const CIEC_ULINT TIME_IN_US_TO_ULINT(const CIEC_TIME &paValue) {
  return TIME_IN_US_TO<CIEC_ULINT>(paValue);
}

/**
 * @brief conversion function from TIME to ULINT in nanoseconds
 *
 * @param paValue TIME value to convert
 * @return TIME value in nanoseconds
 */
inline const CIEC_ULINT TIME_IN_NS_TO_ULINT(const CIEC_TIME &paValue) {
  return TIME_IN_NS_TO<CIEC_ULINT>(paValue);
}

/********************************************************************
 *  TO_LREAL functions
 ********************************************************************/

/**
 * @brief conversion function from TIME to LREAL in seconds
 *
 * @param paValue TIME value to convert
 * @return TIME value in seconds
 */
inline const CIEC_LREAL TIME_IN_S_TO_LREAL(const CIEC_TIME &paValue) {
  return TIME_IN_S_TO<CIEC_LREAL>(paValue);
}

/**
 * @brief conversion function from TIME to LREAL in milliseconds
 *
 * @param paValue TIME value to convert
 * @return TIME value in milliseconds
 */
inline const CIEC_LREAL TIME_IN_MS_TO_LREAL(const CIEC_TIME &paValue) {
  return TIME_IN_MS_TO<CIEC_LREAL>(paValue);
}

/**
 * @brief conversion function from TIME to LREAL in microseconds
 *
 * @param paValue TIME value to convert
 * @return TIME value in microseconds
 */
inline const CIEC_LREAL TIME_IN_US_TO_LREAL(const CIEC_TIME &paValue) {
  return TIME_IN_US_TO<CIEC_LREAL>(paValue);
}

/**
 * @brief conversion function from TIME to LREAL in nanoseconds
 *
 * @param paValue TIME value to convert
 * @return TIME value in nanoseconds
 */
inline const CIEC_LREAL TIME_IN_NS_TO_LREAL(const CIEC_TIME &paValue) {
  return TIME_IN_NS_TO<CIEC_LREAL>(paValue);
}

#endif /* FORTE_USE_64BIT_DATATYPES */

#endif /* SRC_CORE_DATATYPES_CONVERT_TIMEINRESOLUTIONCONVERTFUNCTIONS_H_ */
