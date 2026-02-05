/*******************************************************************************
 * Copyright (c) 2022, 2026 Primetals Technologies Austria GmbH
 *                          HR Agrartechnik GmbH, Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Martin Melik-Merkumians
 *      - initial implementation and rework communication infrastructure
 *   Martin Jobst - add user-defined literal
 *   Alois Zoitl  - migrated data type toString to std::string
 *   Franz Höpfinger - add constexpr
 *******************************************************************************/

#pragma once

#include "forte/datatypes/forte_any_char.h"

namespace forte {
  /*!\ingroup COREDTS CIEC_BYTE represents the byte data type according to IEC 61131.
   */
  class CIEC_WCHAR : public CIEC_ANY_CHAR {
      DECLARE_FIRMWARE_DATATYPE(WCHAR)

    public:
      TForteByte *getDataPtr() override {
        return reinterpret_cast<TForteByte *>(&mData);
      }

      const TForteByte *getConstDataPtr() const override {
        return reinterpret_cast<const TForteByte *>(&mData);
      }
      using TValueType = TForteWChar;
      [[deprecated("Please use the corresponding numeric_limits template")]]
      constexpr static size_t scmBitLength = 16U;
      [[deprecated("Please use the corresponding numeric_limits template")]]
      static constexpr TValueType scmMinVal = std::numeric_limits<TValueType>::min();
      [[deprecated("Please use the corresponding numeric_limits template")]]
      static constexpr TValueType scmMaxVal = std::numeric_limits<TValueType>::max();

      constexpr CIEC_WCHAR() = default;

      constexpr CIEC_WCHAR(const CIEC_WCHAR &paValue) : CIEC_ANY_CHAR(), mData(paValue.mData) {
        mData = static_cast<TValueType>(static_cast<CIEC_WCHAR::TValueType>(paValue));
      }

      constexpr explicit CIEC_WCHAR(const TValueType paValue) : mData(paValue) {
      }

      ~CIEC_WCHAR() override = default;

      CIEC_WCHAR &operator=(const CIEC_WCHAR &paValue) {
        mData = paValue.mData;
        return *this;
      }

      /*! \brief Converts CIEC_WCHAR to elementary byte
       *
       *   Conversion operator for converting CIEC_WCHAR to elementary byte
       */
      constexpr explicit operator TForteWChar() const {
        return mData;
      }

      void toString(std::string &paTargetBuf) const override;

      int fromString(const char *paValue) override;

      EDataTypeID getDataTypeID() const override {
        return e_WCHAR;
      }

    protected:
      TValueType mData = {};
  };

  constexpr inline CIEC_WCHAR operator""_WCHAR(char16_t paValue) {
    return CIEC_WCHAR(static_cast<CIEC_WCHAR::TValueType>(paValue));
  }

  constexpr inline CIEC_WCHAR operator""_WCHAR(unsigned long long int paValue) {
    return CIEC_WCHAR(static_cast<CIEC_WCHAR::TValueType>(paValue));
  }

  template<>
  struct CDataTypeTrait<CIEC_WCHAR> {
      static constexpr CIEC_ANY::EDataTypeID scmDataTypeId = CIEC_ANY::e_WCHAR;
      static const StringId scmDataTypeName;
  };
} // namespace forte

namespace std {
  template<>
  struct numeric_limits<forte::CIEC_WCHAR> : public forte::templates::numeric_limits<forte::CIEC_WCHAR> {
      static constexpr size_t bitLength = 16U;
  };
} // namespace std
