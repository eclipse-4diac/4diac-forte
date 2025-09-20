/*******************************************************************************
 * Copyright (c) 2005, 2025 Pr3factor GmbH, ACIN, fortiss GmbH
 *                          Primetals Technologies Austria GmbH
 *                          Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Thomas Strasser, Ingomar Müller, Alois Zoitl, Gerhard Ebenhofer,
 *     Ingo Hegny, Martin Melik Merkumians, Monika Wenger
 *                - initial implementation and rework communication infrastructure
 *   Martin Melik Merkumians - make TForteFloat constructor explicit, removed
 *                  built-in type operator=, added castable CIEC types operator=
 *   Martin Jobst - add equals function
 *                - add user-defined literal
 *   Alois Zoitl  - migrated data type toString to std::string
 *******************************************************************************/

#pragma once

#include "forte/datatypes/forte_any_real.h"

#include "forte/datatypes/forte_int.h"
#include "forte/datatypes/forte_sint.h"
#include "forte/datatypes/forte_uint.h"
#include "forte/datatypes/forte_usint.h"

namespace forte {
  /*!\ingroup COREDTS CIEC_REAL represents the real data type according to IEC 61131.
   */
  class CIEC_REAL final : public CIEC_ANY_REAL {
      DECLARE_FIRMWARE_DATATYPE(REAL)

    public:
      using TValueType = TForteFloat;
      [[deprecated("Please use the corresponding numeric_limits template")]]
      constexpr static size_t scmBitLength = 32U;
      [[deprecated("Please use the corresponding numeric_limits template")]]
      static constexpr TValueType scmMinVal = std::numeric_limits<TValueType>::min();
      [[deprecated("Please use the corresponding numeric_limits template")]]
      static constexpr TValueType scmMaxVal = std::numeric_limits<TValueType>::max();

      CIEC_REAL() = default;

      CIEC_REAL(const CIEC_REAL &paValue) : CIEC_ANY_REAL() {
        setValueSimple(paValue);
      }

      CIEC_REAL(const CIEC_SINT &paValue) : CIEC_ANY_REAL() {
        setValue(paValue);
      }

      CIEC_REAL(const CIEC_INT &paValue) : CIEC_ANY_REAL() {
        setValue(paValue);
      }

      CIEC_REAL(const CIEC_USINT &paValue) : CIEC_ANY_REAL() {
        setValue(paValue);
      }

      CIEC_REAL(const CIEC_UINT &paValue) : CIEC_ANY_REAL() {
        setValue(paValue);
      }

      explicit CIEC_REAL(const TValueType paValue) {
        setTFLOAT(paValue);
      }

      ~CIEC_REAL() override = default;

      CIEC_REAL &operator=(const CIEC_REAL &paValue) {
        // Simple value assignment - no self assignment check needed
        setValueSimple(paValue);
        return *this;
      }

      CIEC_REAL &operator=(const CIEC_INT &paValue) {
        setValue(paValue);
        return *this;
      }

      CIEC_REAL &operator=(const CIEC_UINT &paValue) {
        setValue(paValue);
        return *this;
      }

      CIEC_REAL &operator=(const CIEC_SINT &paValue) {
        setValue(paValue);
        return *this;
      }

      CIEC_REAL &operator=(const CIEC_USINT &paValue) {
        setValue(paValue);
        return *this;
      }

      CIEC_REAL operator-() const {
        return CIEC_REAL(-1.0f * *this);
      }

      /*! \brief Converts CIEC_REAL to elementary 32 bit float
       *
       *   Conversion operator for converting CIEC_REAL to elementary 32 bit float
       */
      operator TForteFloat() const {
        return getTFLOAT();
      }

      void setValue(const CIEC_ANY &paValue) override;

      void reset() override {
        setTFLOAT(0.0f);
      }

      EDataTypeID getDataTypeID() const override {
        return e_REAL;
      }

      /*! \brief Converts string value to data type value
       *
       *   This command implements a conversion function from IEC 61131
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
       *   to IEC 61131 conform data type (string format).
       *   This function is necessary for communication with a proper engineering system.
       *   \param paTargetBuf Reference to the buffer String
       */
      void toString(std::string &paTargetBuf) const override;

      [[nodiscard]] bool equals(const CIEC_ANY &paOther) const override {
        if (paOther.getDataTypeID() == e_REAL) {
          return getTFLOAT() == static_cast<const CIEC_REAL &>(paOther).getTFLOAT();
        }
        return false;
      }

      /*! \brief Casts TForteFloat values to CIEC data types
       *  \param paSrcValue Source value for the cast
       *  \param paDestValue Destination CIEC object of the cast
       */
      static void castRealData(const CIEC_REAL &paSrcValue, CIEC_ANY &paDestValue);
  };

  inline CIEC_REAL operator""_REAL(unsigned long long int paValue) {
    return CIEC_REAL(static_cast<CIEC_REAL::TValueType>(paValue));
  }

  inline CIEC_REAL operator""_REAL(long double paValue) {
    return CIEC_REAL(static_cast<CIEC_REAL::TValueType>(paValue));
  }

  template<>
  struct CDataTypeTrait<CIEC_REAL> {
      static constexpr CIEC_ANY::EDataTypeID scmDataTypeId = CIEC_ANY::e_REAL;
      static const StringId scmDataTypeName;
  };
} // namespace forte

namespace std {
  template<>
  struct numeric_limits<forte::CIEC_REAL> : public forte::templates::numeric_limits<forte::CIEC_REAL> {
      static constexpr size_t bitLength = 32U;
  };
} // namespace std
