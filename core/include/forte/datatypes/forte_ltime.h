/*******************************************************************************
 * Copyright (c) 2005, 2025 Profactor GmbH, ACIN, TU Wien/ACIN
 *                          Martin Erich Jobst, Primetals Technologies Austria GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Thomas Strasser, Ingomar Müller, Alois Zoitl, Gerhard Ebenhofer,
 *     Monika Wenger, Ingo Hegny, Martin Melik Merkumians,
 *                - initial implementation and rework communication infrastructure
 *   Martin Melik-Merkumians - added getInNanoSeconds, setFromNanoSeconds, changed
 *                  defines to constants, added literal parsing for micro and
 *                  nanoseconds, removed built-in type operator=, removed operator++
 *   Martin Jobst - add user-defined literal
 *   Alois Zoitl  - migrated data type toString to std::string
 *******************************************************************************/

#pragma once

#include "forte/datatypes/forte_any_duration.h"

#include "forte/datatypes/forte_time.h"

namespace forte {
  /*!\ingroup COREDTS CIEC_TIME represents the time data types according to IEC 61131.
   */

  class CIEC_LTIME final : public CIEC_ANY_DURATION {
      DECLARE_FIRMWARE_DATATYPE(LTIME)

    public:
      [[deprecated("Please use the corresponding numeric_limits template")]]
      constexpr static size_t scmBitLength = 64U;
      [[deprecated("Please use the corresponding numeric_limits template")]]
      static constexpr TValueType scmMinVal = std::numeric_limits<TValueType>::min();
      [[deprecated("Please use the corresponding numeric_limits template")]]
      static constexpr TValueType scmMaxVal = std::numeric_limits<TValueType>::max();

      CIEC_LTIME() = default;

      CIEC_LTIME(const CIEC_LTIME &paValue) : CIEC_ANY_DURATION() {
        setValueSimple(paValue);
      }

      CIEC_LTIME(const CIEC_TIME &paValue) : CIEC_ANY_DURATION() {
        setValue(paValue);
      }

      explicit CIEC_LTIME(const TValueType paValue) {
        setLargestInt(paValue);
      }

      ~CIEC_LTIME() override = default;

      CIEC_LTIME &operator=(const CIEC_LTIME &paValue) {
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
        return e_LTIME;
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
       *   \param paTargetBuf Reference to the buffer String
       */
      void toString(std::string &paTargetBuf) const override;

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

  inline CIEC_LTIME operator""_LTIME(unsigned long long int paValue) {
    return CIEC_LTIME(static_cast<CIEC_LTIME::TValueType>(paValue));
  }

  template<>
  struct CDataTypeTrait<CIEC_LTIME> {
      static constexpr CIEC_ANY::EDataTypeID scmDataTypeId = CIEC_ANY::e_LTIME;
      static const StringId scmDataTypeName;
  };
} // namespace forte

namespace std {
  template<>
  struct numeric_limits<forte::CIEC_LTIME> : public forte::templates::numeric_limits<forte::CIEC_LTIME> {
      static constexpr size_t bitLength = 64U;
  };
} // namespace std
