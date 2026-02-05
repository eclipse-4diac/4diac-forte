/*******************************************************************************
 * Copyright (c) 2022, 2026 Primetals Technologies Austria GmbH
 *                          HR Agrartechnik GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Martin Melik-Merkumians
 *               - initial implementation and rework communication infrastructure
 *   Alois Zoitl - migrated data type toString to std::string
 *   Franz Höpfinger - add constexpr
 *******************************************************************************/

#pragma once

#include "forte/datatypes/forte_any_char.h"

namespace forte {
  /*!\ingroup COREDTS CIEC_BYTE represents the byte data type according to IEC 61131.
   */
  class CIEC_CHAR : public CIEC_ANY_CHAR {
      DECLARE_FIRMWARE_DATATYPE(CHAR)

    public:
      TForteByte *getDataPtr() override {
        return reinterpret_cast<TForteByte *>(&mData);
      }

      const TForteByte *getConstDataPtr() const override {
        return reinterpret_cast<const TForteByte *>(&mData);
      }
      using TValueType = TForteChar;
      [[deprecated("Please use the corresponding numeric_limits template")]]
      constexpr static size_t scmBitLength = 8U;

      [[deprecated("Please use the corresponding numeric_limits template")]]
      static constexpr TValueType scmMinVal = std::numeric_limits<TValueType>::min();
      [[deprecated("Please use the corresponding numeric_limits template")]]
      static constexpr TValueType scmMaxVal = std::numeric_limits<TValueType>::max();

      constexpr CIEC_CHAR() = default;

      constexpr CIEC_CHAR(const CIEC_CHAR &paValue) : CIEC_ANY_CHAR(), mData(paValue.mData) {
        mData = static_cast<TValueType>(static_cast<CIEC_CHAR::TValueType>(paValue));
      }

      constexpr explicit CIEC_CHAR(const TValueType paValue) : mData(paValue) {
      }

      ~CIEC_CHAR() override = default;

      CIEC_CHAR &operator=(const CIEC_CHAR &paValue) {
        mData = paValue.mData;
        return *this;
      }

      /*! \brief Converts CIEC_CHAR to elementary byte
       *
       *   Conversion operator for converting CIEC_CHAR to elementary byte
       */
      constexpr explicit operator TForteChar() const {
        return mData;
      }

      void toString(std::string &paTargetBuf) const override;

      int fromString(const char *paValue) override;

      EDataTypeID getDataTypeID() const final {
        return e_CHAR;
      }

    protected:
      TValueType mData = {};
  };

  inline bool operator==(const CIEC_CHAR &lhs, const CIEC_CHAR &rhs) {
    return static_cast<CIEC_CHAR::TValueType>(lhs) == static_cast<CIEC_CHAR::TValueType>(rhs);
  }

  constexpr inline CIEC_CHAR operator""_CHAR(char paValue) {
    return CIEC_CHAR(static_cast<CIEC_CHAR::TValueType>(paValue));
  }

  constexpr inline CIEC_CHAR operator""_CHAR(unsigned long long int paValue) {
    return CIEC_CHAR(static_cast<CIEC_CHAR::TValueType>(paValue));
  }

  template<>
  struct CDataTypeTrait<CIEC_CHAR> {
      static constexpr CIEC_ANY::EDataTypeID scmDataTypeId = CIEC_ANY::e_CHAR;
      static const StringId scmDataTypeName;
  };
} // namespace forte

namespace std {
  template<>
  struct numeric_limits<forte::CIEC_CHAR> : public forte::templates::numeric_limits<forte::CIEC_CHAR> {
      static constexpr size_t bitLength = 8U;
  };
} // namespace std
