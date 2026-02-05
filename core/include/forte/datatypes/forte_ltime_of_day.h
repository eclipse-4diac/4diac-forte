/*******************************************************************************
 * Copyright (c) 2008, 2026 nxtControl GmbH, ACIN, Profactor GmbH, fortiss GmbH
 *                          Primetals Technologies Austria GmbH,
 *                          HR Agrartechnik GmbH, Martin Erich Jobst
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
 *                  removed built-in type operator=, removed operator++
 *   Martin Jobst - add user-defined literal
 *   Alois Zoitl  - migrated data type toString to std::string
 *   Franz Höpfinger - add constexpr
 *******************************************************************************/

#pragma once

#include "forte/datatypes/forte_any_date.h"

#include "forte/datatypes/forte_time_of_day.h"

namespace forte {
  /*!\ingroup COREDTS CIEC_LTIME_OF_DAY represents the time data types according to IEC 61131.
   */

  class CIEC_LTIME_OF_DAY final : public CIEC_ANY_DATE {
      DECLARE_FIRMWARE_DATATYPE(LTIME_OF_DAY)
    public:
      TForteByte *getDataPtr() override {
        return reinterpret_cast<TForteByte *>(&mData);
      }

      const TForteByte *getConstDataPtr() const override {
        return reinterpret_cast<const TForteByte *>(&mData);
      }
      [[deprecated("Please use the corresponding numeric_limits template")]]
      constexpr static size_t scmBitLength = 64U;
      [[deprecated("Please use the corresponding numeric_limits template")]]
      static constexpr TValueType scmMinVal = std::numeric_limits<TValueType>::min();
      [[deprecated("Please use the corresponding numeric_limits template")]]
      static constexpr TValueType scmMaxVal = std::numeric_limits<TValueType>::max();

      constexpr CIEC_LTIME_OF_DAY() = default;

      constexpr CIEC_LTIME_OF_DAY(const CIEC_LTIME_OF_DAY &paValue) : CIEC_ANY_DATE(), mData(paValue.mData) {
        mData = static_cast<TValueType>(static_cast<CIEC_LTIME_OF_DAY::TValueType>(paValue));
      }

      CIEC_LTIME_OF_DAY(const CIEC_TIME_OF_DAY &paValue) : CIEC_ANY_DATE() {
        setValue(paValue);
      }

      constexpr explicit CIEC_LTIME_OF_DAY(const TValueType paValue) : mData(paValue) {
      }

      ~CIEC_LTIME_OF_DAY() override = default;

      CIEC_LTIME_OF_DAY &operator=(const CIEC_LTIME_OF_DAY &paValue) {
        mData = paValue.mData;
        return *this;
      }

      CIEC_LTIME_OF_DAY &operator=(const CIEC_TIME_OF_DAY &paValue) {
        // Simple value assignment - no self assignment check needed
        setValue(paValue);
        return *this;
      }

      /*! \brief Converts CIEC_SINT to elementary 32 bit integer
       *
       *   Conversion operator for converting CIEC_SINT to elementary 32 bit integer
       */
      constexpr operator TForteUInt64() const {
        return getTUINT64();
      }

      EDataTypeID getDataTypeID() const override {
        return e_LTIME_OF_DAY;
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

    protected:
      TValueType mData = {};
  };

  constexpr inline CIEC_LTIME_OF_DAY operator""_LTIME_OF_DAY(unsigned long long int paValue) {
    return CIEC_LTIME_OF_DAY(static_cast<CIEC_LTIME_OF_DAY::TValueType>(paValue));
  }

  template<>
  struct CDataTypeTrait<CIEC_LTIME_OF_DAY> {
      static constexpr CIEC_ANY::EDataTypeID scmDataTypeId = CIEC_ANY::e_LTIME_OF_DAY;
      static const StringId scmDataTypeName;
  };
} // namespace forte

namespace std {
  template<>
  struct numeric_limits<forte::CIEC_LTIME_OF_DAY> : public forte::templates::numeric_limits<forte::CIEC_LTIME_OF_DAY> {
      static constexpr size_t bitLength = 64U;
  };
} // namespace std
