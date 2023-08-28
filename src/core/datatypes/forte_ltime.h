/*******************************************************************************
 * Copyright (c) 2005, 2023 Profactor GmbH, ACIN, TU Wien/ACIN
 *                          Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Thomas Strasser, Ingomar Müller, Alois Zoitl, Gerhard Ebenhofer,
 *    Monika Wenger, Ingo Hegny, Martin Melik Merkumians,
 *      - initial implementation and rework communication infrastructure
 *    Martin Melik-Merkumians - added getInNanoSeconds, setFromNanoSeconds,
 *      changed defines to constants, added literal parsing for micro and
 *      nanoseconds, removed built-in type operator=, removed operator++
 *    Martin Jobst - add user-defined literal
 *******************************************************************************/
#ifndef _FORTE_LTIME_H_
#define _FORTE_LTIME_H_

#include "forte_any_duration.h"

#include "forte_time.h"

/*!\ingroup COREDTS CIEC_TIME represents the time data types according to IEC 61131.
 */

class CIEC_LTIME final : public CIEC_ANY_DURATION {
  DECLARE_FIRMWARE_DATATYPE(LTIME)
 
  private:
    static constexpr char csmMinLTimeValue[] = "LT#-106751d23h47m16s854ms775us808ns";
    static constexpr char csmZeroNanoSecondLTimeValue[] = "LT#0ns";

  public:
    CIEC_LTIME() = default;

    CIEC_LTIME(const CIEC_LTIME &paValue) : CIEC_ANY_DURATION() {
      setValueSimple(paValue);
    }

    CIEC_LTIME(const CIEC_TIME &paValue) : CIEC_ANY_DURATION() {
      setValue(paValue);
    }

    explicit CIEC_LTIME(const TValueType paValue){
      setLargestInt(paValue);
    }

    ~CIEC_LTIME() override = default;

    CIEC_LTIME& operator =(const CIEC_LTIME &paValue){
      // Simple value assignment - no self assignment check needed
      setValueSimple(paValue);
      return *this;
    }

    CIEC_LTIME &operator=(const CIEC_TIME &paValue) {
      // Simple value assignment - no self assignment check needed
      setValue(paValue);
      return *this;
    }

    CIEC_LTIME operator-() const {
      return CIEC_TIME(-1 * *this);
    }

    operator TValueType() const {
      return getLargestInt();
    }

    EDataTypeID getDataTypeID() const override {
      return CIEC_ANY::e_LTIME;
    }

    /*! \brief Converts string value to data type value
     *
     *   This command implements a conversion function from IEC61131
     *   data type (string format) to a C++ conform type.
     *   This function is necessary for communication with a proper engineering system.
     *   \param paValue string buffer
     *   \return number of bytes taken used from the buffer
     *        -1 on on error
     */
    int fromString(const char *paValue) override;
    /*! \brief Converts data type value to string
     *
     *   This command implements a conversion function from C++ data type
     *   to IEC61131 conform data type (string format).
     *   This function is necessary for communication with a proper engineering system.
     *   \param paValue Pointer to the buffer String
     *   \param paBufferSize Size of the given buffer
     *   \return number of bytes used in the buffer without trailing 0x00
     *           -1 on error
     */
    int toString(char* paValue, size_t paBufferSize) const override;

    /*! \brief calculates buffer size needed for toString conversion
     */
    size_t getToStringBufferSize() const override;

    TValueType getInDays() const;
    TValueType getInHours() const;
    TValueType getInMinutes() const;
    TValueType getInSeconds() const;
    TValueType getInMilliSeconds() const;
    TValueType getInMicroSeconds() const;
    TValueType getInNanoSeconds() const;

    void setFromSeconds(TValueType paValue);
    void setFromMilliSeconds(TValueType paValue);
    void setFromMicroSeconds(TValueType paValue);
    void setFromNanoSeconds(TValueType paValue);
};

inline CIEC_LTIME operator ""_LTIME(unsigned long long int paValue) {
  return CIEC_LTIME(static_cast<CIEC_LTIME::TValueType>(paValue));
}

#endif /*_FORTE_LTIME_H_*/
