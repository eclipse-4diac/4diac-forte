/*******************************************************************************
 * Copyright (c) 2022 Primetals Technologies Austria GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Martin Melik Merkumians
 *      - initial implementation and rework communication infrastructure
 *******************************************************************************/
#ifndef _ANY_DURATION_H_
#define _ANY_DURATION_H_

#include "forte_any_magnitude.h"

/*!\ingroup COREDTS CIEC_ANY_DURATION represents the user defined data types according to
 *  IEC 61131.
 */
class CIEC_ANY_DURATION : public CIEC_ANY_MAGNITUDE {
public:
  /*! \brief The common C/C++ internal representation type for ANY_DURATION */
  typedef TLargestIntValueType TValueType;

  /*! \brief Defines the time base in units per second that will be used in the TIME data type
   *
   *  The default value will be 1 000 000 000, which means that the time bas is 1 ns.
   *  For Smaller devices which do not use 64 bit datatypes a time base of 1000 (i.e., 1ms)
   *  or 1000000 (i.e., 1micro s) may be suitable.
   */
  static constexpr uint_fast64_t csmForteTimeBaseUnitsPerNanoSecond = 1;
  static constexpr uint_fast64_t csmForteTimeBaseUnitsPerMicroSecond = csmForteTimeBaseUnitsPerNanoSecond * 1000;  
  static constexpr uint_fast64_t csmForteTimeBaseUnitsPerMilliSecond = csmForteTimeBaseUnitsPerMicroSecond * 1000; 
  static constexpr uint_fast64_t csmForteTimeBaseUnitsPerSecond = csmForteTimeBaseUnitsPerMilliSecond * 1000;     
  static constexpr uint_fast64_t csmForteTimeBaseUnitsPerMinute = csmForteTimeBaseUnitsPerSecond * 60;
  static constexpr uint_fast64_t csmForteTimeBaseUnitsPerHour = csmForteTimeBaseUnitsPerMinute * 60;
  static constexpr uint_fast64_t csmForteTimeBaseUnitsPerDay = csmForteTimeBaseUnitsPerHour *24;

  template <typename T, typename U, std::enable_if_t<std::conjunction_v<std::is_base_of<CIEC_ANY_DURATION, T>, std::is_base_of<CIEC_ANY_DURATION, U> >, int> = 0>
  friend bool operator==(const T &paLeft, const U &paRight) {
    return static_cast<typename T::TValueType>(paLeft) == static_cast<typename T::TValueType>(paRight);
  }

  template <typename T, typename U, std::enable_if_t<std::conjunction_v<std::is_base_of<CIEC_ANY_DURATION, T>, std::is_base_of<CIEC_ANY_DURATION, U> >, int> = 0>
  friend bool operator!=(const T &paLeft, const U &paRight) {
    return !(paLeft == paRight);
  }

  template <typename T, typename U, std::enable_if_t<std::conjunction_v<std::is_base_of<CIEC_ANY_DURATION, T>, std::is_base_of<CIEC_ANY_DURATION, U> >, int> = 0>
  friend bool operator<(const T &paLeft, const U &paRight) {
    return static_cast<typename T::TValueType>(paLeft) < static_cast<typename T::TValueType>(paRight);
  }

  template <typename T, typename U, std::enable_if_t<std::conjunction_v<std::is_base_of<CIEC_ANY_DURATION, T>, std::is_base_of<CIEC_ANY_DURATION, U> >, int> = 0>
  friend bool operator>(const T &paLeft, const U &paRight) {
    return static_cast<typename T::TValueType>(paLeft) > static_cast<typename T::TValueType>(paRight);
  }

  template <typename T, typename U, std::enable_if_t<std::conjunction_v<std::is_base_of<CIEC_ANY_DURATION, T>, std::is_base_of<CIEC_ANY_DURATION, U> >, int> = 0>
  friend bool operator<=(const T &paLeft, const U &paRight) {
    return paLeft == paRight || paLeft < paRight;
  }

  template <typename T, typename U, std::enable_if_t<std::conjunction_v<std::is_base_of<CIEC_ANY_DURATION, T>, std::is_base_of<CIEC_ANY_DURATION, U> >, int> = 0>
  friend bool operator>=(const T &paLeft, const U &paRight) {
    return paLeft == paRight || paLeft > paRight;
  }

  template <typename T, std::enable_if_t<std::is_base_of_v<CIEC_ANY_DURATION, T>, int> = 0>
  friend T operator+(const T& paLeft, const T& paRight) {
    return T(static_cast<typename T::TValueType>(paLeft) + static_cast<typename T::TValueType>(paRight));
  }

  template <typename T, std::enable_if_t<std::is_base_of_v<CIEC_ANY_DURATION, T>, int> = 0>
  friend T operator-(const T& paLeft, const T& paRight) {
    return T(static_cast<typename T::TValueType>(paLeft) - static_cast<typename T::TValueType>(paRight));
  }


  ~CIEC_ANY_DURATION() override = default;

  EDataTypeID getDataTypeID() const override {
    return CIEC_ANY::e_ANY;
  }

protected:
  CIEC_ANY_DURATION() = default;
  int timeElementsToString(int64_t paTimeElement, char *paValue, int paSize, size_t paBufferSize, const char *paUnit) const;

};

#endif /*_ANY_DURATION_H_*/