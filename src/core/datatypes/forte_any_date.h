/*******************************************************************************
 * Copyright (c) 2005 - 2013 Profactor GmbH, ACIN, nxtControl GmbH, fortiss GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Thomas Strasser, Ingomar Müller, Martin Melik Merkumians, Alois Zoitl,
 *    Monika Wenger, Stansilav Meduna
 *      - initial implementation and rework communication infrastructure
 *******************************************************************************/
#ifndef _ANY_DAT_H_
#define _ANY_DAT_H_

#include "forte_any_elementary.h"

/*!\ingroup COREDTS IIEC_ANY_DATE represents any date data types according to IEC 61131.
 */
class CIEC_ANY_DATE : public CIEC_ANY_ELEMENTARY{
  public:
    typedef TForteUInt64 TValueType;
    
    ~CIEC_ANY_DATE() override = default;

    /*! Retrieve the current timezone
     *
     * Can be sed to adjust mktime()-related values
     *
     * @return Number of minutes west (negative) or east (positive), not taking DST into consideration
     */
    static TForteInt32 getTimeZoneOffset();

    /*! set the value of the data according to the given values
     *
     */
    bool setDateAndTime(struct tm &paTM, unsigned int paMilliSec);

    /*! retrieve the time struct for the current value stored in this Data variable
     */
    struct tm *getTimeStruct(struct tm *const paTimeStruct) const;

    /*! Get the milliseconds part of the data value
     */
    unsigned int getMilliSeconds() const;

    template <typename T, std::enable_if_t<std::is_base_of_v<CIEC_ANY_DATE, T>, int> = 0>
    friend bool operator==(const T& paLeft, const T& paRight) {
      return static_cast<typename T::TValueType>(paLeft) == static_cast<typename T::TValueType>(paRight);
    }

    template <typename T, std::enable_if_t<std::is_base_of_v<CIEC_ANY_DATE, T>, int> = 0>
    friend bool operator!=(const T& paLeft, const T& paRight) {
      return !(paLeft == paRight);
    }

    template <typename T, std::enable_if_t<std::is_base_of_v<CIEC_ANY_DATE, T>, int> = 0>
    friend bool operator<(const T& paLeft, const T& paRight) {
      return static_cast<typename T::TValueType>(paLeft) < static_cast<typename T::TValueType>(paRight);
    }

    template <typename T, std::enable_if_t<std::is_base_of_v<CIEC_ANY_DATE, T>, int> = 0>
    friend bool operator>(const T& paLeft, const T& paRight) {
      return static_cast<typename T::TValueType>(paLeft) > static_cast<typename T::TValueType>(paRight);
    }

    template <typename T, std::enable_if_t<std::is_base_of_v<CIEC_ANY_DATE, T>, int> = 0>
    friend bool operator<=(const T& paLeft, const T& paRight) {
      return paLeft == paRight || paLeft < paRight;
    }

    template <typename T, std::enable_if_t<std::is_base_of_v<CIEC_ANY_DATE, T>, int> = 0>
    friend bool operator>=(const T& paLeft, const T& paRight) {
      return paLeft == paRight || paLeft > paRight;
    }

  protected:
    CIEC_ANY_DATE() = default;

  private:
    static TForteInt32 smTimeZoneOffset;
};

#endif /*_MANY_DAT_H_*/
