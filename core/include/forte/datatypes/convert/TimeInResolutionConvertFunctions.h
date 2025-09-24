/*******************************************************************************
 * Copyright (c) 2019, 2024 TU Vienna/ACIN
 *                          Martin Erich Jobst
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Martin Melik-Merkumians - adds non standard time to number conversions
 *    Martin Erich Jobst - add templated versions for ANY_DURATION
 *******************************************************************************/

#pragma once

#include "forte/datatypes/forte_dint.h"
#include "forte/datatypes/forte_lint.h"
#include "forte/datatypes/forte_lreal.h"
#include "forte/datatypes/forte_udint.h"
#include "forte/datatypes/forte_ulint.h"
#include "forte/datatypes/forte_string.h"
#include "forte/datatypes/forte_time.h"

#include "forte/iec61131_cast_helper.h"
#include "forte/util/forte_constants.h"

namespace forte {
  /*********************************************************************************************
   *   TIME_IN_<RESOLUTION>_TO_* functions - custom non-standard functions
   *********************************************************************************************/
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
  template<typename T, typename U>
  inline T func_TIME_IN_S_TO(const U &paValue) {
    static_assert((std::is_base_of_v<CIEC_ANY_NUM, T>), "T not of ANY_NUM");
    static_assert((std::is_base_of_v<CIEC_ANY_DURATION, U>), "U not of ANY_DURATION");
    if (std::is_base_of_v<CIEC_ANY_REAL, T>) {
      return T(static_cast<typename T::TValueType>(paValue.getInNanoSeconds()) /
               static_cast<typename T::TValueType>(util::constants::cNanosecondsPerSecond));
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
  template<typename T, typename U>
  inline T func_TIME_IN_MS_TO(const U &paValue) {
    static_assert((std::is_base_of_v<CIEC_ANY_NUM, T>), "T not of ANY_NUM");
    static_assert((std::is_base_of_v<CIEC_ANY_DURATION, U>), "U not of ANY_DURATION");
    if (std::is_base_of_v<CIEC_ANY_REAL, T>) {
      return T(static_cast<typename T::TValueType>(paValue.getInNanoSeconds()) *
               (static_cast<typename T::TValueType>(util::constants::cMillisecondsPerSecond) /
                static_cast<typename T::TValueType>(util::constants::cNanosecondsPerSecond)));
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
  template<typename T, typename U>
  inline T func_TIME_IN_US_TO(const U &paValue) {
    static_assert((std::is_base_of_v<CIEC_ANY_NUM, T>), "T not of ANY_NUM");
    static_assert((std::is_base_of_v<CIEC_ANY_DURATION, U>), "U not of ANY_DURATION");
    if (std::is_base_of_v<CIEC_ANY_REAL, T>) {
      return T(static_cast<typename T::TValueType>(paValue.getInNanoSeconds()) *
               (static_cast<typename T::TValueType>(util::constants::cMicrosecondsPerSecond) /
                static_cast<typename T::TValueType>(util::constants::cNanosecondsPerSecond)));
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
  template<typename T, typename U>
  inline T func_TIME_IN_NS_TO(const U &paValue) {
    static_assert((std::is_base_of_v<CIEC_ANY_NUM, T>), "T not of ANY_NUM");
    static_assert((std::is_base_of_v<CIEC_ANY_DURATION, U>), "U not of ANY_DURATION");
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
  template<typename T>
  inline CIEC_LINT func_TIME_IN_S_TO_LINT(const T &paValue) {
    return func_TIME_IN_S_TO<CIEC_LINT>(paValue);
  }

  /**
   * @brief conversion function from TIME to LINT in milliseconds
   *
   * @param paValue TIME value to convert
   * @return TIME value in milliseconds
   */
  template<typename T>
  inline CIEC_LINT func_TIME_IN_MS_TO_LINT(const T &paValue) {
    return func_TIME_IN_MS_TO<CIEC_LINT>(paValue);
  }

  /**
   * @brief conversion function from TIME to LINT in microseconds
   *
   * @param paValue TIME value to convert
   * @return TIME value in microseconds
   */
  template<typename T>
  inline CIEC_LINT func_TIME_IN_US_TO_LINT(const T &paValue) {
    return func_TIME_IN_US_TO<CIEC_LINT>(paValue);
  }

  /**
   * @brief conversion function from TIME to LINT in nanoseconds
   *
   * @param paValue TIME value to convert
   * @return TIME value in nanoseconds
   */
  template<typename T>
  inline CIEC_LINT func_TIME_IN_NS_TO_LINT(const T &paValue) {
    return func_TIME_IN_NS_TO<CIEC_LINT>(paValue);
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
  template<typename T>
  inline CIEC_ULINT func_TIME_IN_S_TO_ULINT(const T &paValue) {
    return func_TIME_IN_S_TO<CIEC_ULINT>(paValue);
  }

  /**
   * @brief conversion function from TIME to ULINT in milliseconds
   *
   * @param paValue TIME value to convert
   * @return TIME value in milliseconds
   */
  template<typename T>
  inline CIEC_ULINT func_TIME_IN_MS_TO_ULINT(const T &paValue) {
    return func_TIME_IN_MS_TO<CIEC_ULINT>(paValue);
  }

  /**
   * @brief conversion function from TIME to ULINT in microseconds
   *
   * @param paValue TIME value to convert
   * @return TIME value in microseconds
   */
  template<typename T>
  inline CIEC_ULINT func_TIME_IN_US_TO_ULINT(const T &paValue) {
    return func_TIME_IN_US_TO<CIEC_ULINT>(paValue);
  }

  /**
   * @brief conversion function from TIME to ULINT in nanoseconds
   *
   * @param paValue TIME value to convert
   * @return TIME value in nanoseconds
   */
  template<typename T>
  inline CIEC_ULINT func_TIME_IN_NS_TO_ULINT(const T &paValue) {
    return func_TIME_IN_NS_TO<CIEC_ULINT>(paValue);
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
  template<typename T>
  inline CIEC_LREAL func_TIME_IN_S_TO_LREAL(const T &paValue) {
    return func_TIME_IN_S_TO<CIEC_LREAL>(paValue);
  }

  /**
   * @brief conversion function from TIME to LREAL in milliseconds
   *
   * @param paValue TIME value to convert
   * @return TIME value in milliseconds
   */
  template<typename T>
  inline CIEC_LREAL func_TIME_IN_MS_TO_LREAL(const T &paValue) {
    return func_TIME_IN_MS_TO<CIEC_LREAL>(paValue);
  }

  /**
   * @brief conversion function from TIME to LREAL in microseconds
   *
   * @param paValue TIME value to convert
   * @return TIME value in microseconds
   */
  template<typename T>
  inline CIEC_LREAL func_TIME_IN_US_TO_LREAL(const T &paValue) {
    return func_TIME_IN_US_TO<CIEC_LREAL>(paValue);
  }

  /**
   * @brief conversion function from TIME to LREAL in nanoseconds
   *
   * @param paValue TIME value to convert
   * @return TIME value in nanoseconds
   */
  template<typename T>
  inline CIEC_LREAL func_TIME_IN_NS_TO_LREAL(const T &paValue) {
    return func_TIME_IN_NS_TO<CIEC_LREAL>(paValue);
  }

  /********************************************************************
   *  TO_UDINT functions
   ********************************************************************/

  /**
   * @brief conversion function from TIME to UDINT in seconds
   *
   * @param paValue TIME value to convert
   * @return TIME value in seconds
   */
  template<typename T>
  inline CIEC_UDINT func_TIME_IN_S_TO_UDINT(const T &paValue) {
    return func_TIME_IN_S_TO<CIEC_UDINT>(paValue);
  }

  /**
   * @brief conversion function from TIME to UDINT in milliseconds
   *
   * @param paValue TIME value to convert
   * @return TIME value in milliseconds
   */
  template<typename T>
  inline CIEC_UDINT func_TIME_IN_MS_TO_UDINT(const T &paValue) {
    return func_TIME_IN_MS_TO<CIEC_UDINT>(paValue);
  }

  /**
   * @brief conversion function from TIME to UDINT in microseconds
   *
   * @param paValue TIME value to convert
   * @return TIME value in microseconds
   */
  template<typename T>
  inline CIEC_UDINT func_TIME_IN_US_TO_UDINT(const T &paValue) {
    return func_TIME_IN_US_TO<CIEC_UDINT>(paValue);
  }

  /**
   * @brief conversion function from TIME to UDINT in nanoseconds
   *
   * @param paValue TIME value to convert
   * @return TIME value in nanoseconds
   */
  template<typename T>
  inline CIEC_UDINT func_TIME_IN_NS_TO_UDINT(const T &paValue) {
    return func_TIME_IN_NS_TO<CIEC_UDINT>(paValue);
  }

  /********************************************************************
   *  TO_DINT functions
   ********************************************************************/

  /**
   * @brief conversion function from TIME to DINT in seconds
   *
   * @param paValue TIME value to convert
   * @return TIME value in seconds
   */
  template<typename T>
  inline CIEC_DINT func_TIME_IN_S_TO_DINT(const T &paValue) {
    return func_TIME_IN_S_TO<CIEC_DINT>(paValue);
  }

  /**
   * @brief conversion function from TIME to DINT in milliseconds
   *
   * @param paValue TIME value to convert
   * @return TIME value in milliseconds
   */
  template<typename T>
  inline CIEC_DINT func_TIME_IN_MS_TO_DINT(const T &paValue) {
    return func_TIME_IN_MS_TO<CIEC_DINT>(paValue);
  }

  /**
   * @brief conversion function from TIME to DINT in microseconds
   *
   * @param paValue TIME value to convert
   * @return TIME value in microseconds
   */
  template<typename T>
  inline CIEC_DINT func_TIME_IN_US_TO_DINT(const T &paValue) {
    return func_TIME_IN_US_TO<CIEC_DINT>(paValue);
  }

  /**
   * @brief conversion function from TIME to DINT in nanoseconds
   *
   * @param paValue TIME value to convert
   * @return TIME value in nanoseconds
   */
  template<typename T>
  inline CIEC_DINT func_TIME_IN_NS_TO_DINT(const T &paValue) {
    return func_TIME_IN_NS_TO<CIEC_DINT>(paValue);
  }
} // namespace forte
