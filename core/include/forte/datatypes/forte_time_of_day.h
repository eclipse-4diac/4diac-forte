/*******************************************************************************
 * Copyright (c) 2008, 2025 nxtControl GmbH, ACIN, Profactor GmbH, fortiss GmbH,
 *                          Primetals Technologies Austria GmbH,
 *                          Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Stanislav Meduna, Alois Zoitl, Gerhard Ebenhofer, Martin Melik Merkumians,
 *     Monika Wenger
 *      - initial implementation and rework communication infrastructure
 *   Martin Melik Merkumians - make TForteUInt64 constructor explicit,
 *      removed built-in type operator=, removed operator++, update timebase to
 *      nanoseconds
 *   Martin Jobst - add user-defined literal
 *   Alois Zoitl  - migrated data type toString to std::string
 *******************************************************************************/

#pragma once

#include "forte/datatypes/forte_any_date.h"

namespace forte {
  /*!\ingroup COREDTS CIEC_TIME_OF_DAY represents the time data types according to IEC 61131.
   */

  class CIEC_TIME_OF_DAY final : public CIEC_ANY_DATE {
      DECLARE_FIRMWARE_DATATYPE(TIME_OF_DAY)
    public:
      [[deprecated("Please use the corresponding numeric_limits template")]]
      constexpr static size_t scmBitLength = 64U;
      [[deprecated("Please use the corresponding numeric_limits template")]]
      static constexpr TValueType scmMinVal = std::numeric_limits<TValueType>::min();
      [[deprecated("Please use the corresponding numeric_limits template")]]
      static constexpr TValueType scmMaxVal = std::numeric_limits<TValueType>::max();

      CIEC_TIME_OF_DAY() = default;

      CIEC_TIME_OF_DAY(const CIEC_TIME_OF_DAY &paValue) : CIEC_ANY_DATE() {
        setValueSimple(paValue);
      }

      explicit CIEC_TIME_OF_DAY(const TValueType paValue) {
        setTUINT64(paValue);
      }

      ~CIEC_TIME_OF_DAY() override = default;

      CIEC_TIME_OF_DAY &operator=(const CIEC_TIME_OF_DAY &paValue) {
        // Simple value assignment - no self assignment check needed
        setValueSimple(paValue);
        return *this;
      }

      /*! \brief Converts CIEC_TIME_OF_DAY to elementary 64 bit unsigned integer
       *
       *   Conversion operator for converting CIEC_TIME_OF_DAY to elementary 64 bit unsigned integer
       */
      explicit operator TForteUInt64() const {
        return getTUINT64();
      }

      EDataTypeID getDataTypeID() const override {
        return e_TIME_OF_DAY;
      }

      /*! \brief Converts string value to data type value
       *
       *   This command implements a conversion function from IEC 61131
       *   data type (string format) to a C++ conform type.
       *   This function is necessary for communication with a proper engineering system.
       *   \param paValue Pointer to the given String
       *   \return Can be the following response:
       *   \return number of bytes taken used from the buffer
       *        -1 on on error
       */
      int fromString(const char *paValue) override;
      /*! \brief Converts data type value to string
       *
       *   This command implements a conversion function from C++ data type
       *   to IEC 61131 conform data type (string format).
       *   This function is necessary for communication with a proper engineering system.
       *   \param paTargetBuf Reference to the buffer String
       */
      void toString(std::string &paTargetBuf) const override;
  };

  inline CIEC_TIME_OF_DAY operator""_TIME_OF_DAY(unsigned long long int paValue) {
    return CIEC_TIME_OF_DAY(static_cast<CIEC_TIME_OF_DAY::TValueType>(paValue));
  }

  template<>
  struct CDataTypeTrait<CIEC_TIME_OF_DAY> {
      static constexpr CIEC_ANY::EDataTypeID scmDataTypeId = CIEC_ANY::e_TIME_OF_DAY;
      static const StringId scmDataTypeName;
  };
} // namespace forte

namespace std {
  template<>
  struct numeric_limits<forte::CIEC_TIME_OF_DAY> : public forte::templates::numeric_limits<forte::CIEC_TIME_OF_DAY> {
      static constexpr size_t bitLength = 64U;
  };
} // namespace std
