/*******************************************************************************
 * Copyright (c) 2005, 2026 Profactor GmbH, ACIN, fortiss GmbH,
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
 *   Thomas Strasser, Ingomar Müller, Alois Zoitl, Gerhard Ebenhofer,
 *     Ingo Hegny, Monika Wenger
 *      - initial implementation and rework communication infrastructure,
 *   Martin Melik Merkumians - make TForteUInt16 constructor explicit,
 *        removed built-in type operator=, added castable CIEC types operator=
 *   Martin Jobst - add user-defined literal
 *   Alois Zoitl  - migrated data type toString to std::string
 *   Franz Höpfinger - add constexpr
 *******************************************************************************/

#pragma once

#include "forte/datatypes/forte_any_unsigned.h"
#include "forte/datatypes/forte_usint.h"
#include <limits>

namespace forte {
  /*!\ingroup COREDTS CIEC_UINT represents the uint data type according to IEC 61131.
   */
  class CIEC_UINT final : public CIEC_ANY_UNSIGNED {
      DECLARE_FIRMWARE_DATATYPE(UINT)

    public:
      TForteByte *getDataPtr() override {
        return reinterpret_cast<TForteByte *>(&mData);
      }

      const TForteByte *getConstDataPtr() const override {
        return reinterpret_cast<const TForteByte *>(&mData);
      }
      using TValueType = TForteUInt16;
      [[deprecated("Please use the corresponding numeric_limits template")]]
      constexpr static size_t scmBitLength = 16U;
      [[deprecated("Please use the corresponding numeric_limits template")]]
      static constexpr TValueType scmMinVal = std::numeric_limits<TValueType>::min();
      [[deprecated("Please use the corresponding numeric_limits template")]]
      static constexpr TValueType scmMaxVal = std::numeric_limits<TValueType>::max();

      constexpr CIEC_UINT() = default;

      constexpr CIEC_UINT(const CIEC_UINT &paValue) : CIEC_ANY_UNSIGNED(), mData(paValue.mData) {
        mData = static_cast<TValueType>(static_cast<CIEC_UINT::TValueType>(paValue));
      }

      constexpr CIEC_UINT(const CIEC_USINT &paValue) : CIEC_ANY_UNSIGNED() {
        mData = static_cast<TValueType>(static_cast<CIEC_USINT::TValueType>(paValue));
      }

      template<typename T,
               std::enable_if_t<std::is_same_v<typename mpl::implicit_cast_t<T, CIEC_UINT>, CIEC_UINT>, int> = 0>
      constexpr explicit CIEC_UINT(const T &paValue) :
          CIEC_ANY_UNSIGNED(),
          mData(static_cast<TValueType>(static_cast<TValueType>(paValue))) {
      }

      constexpr explicit CIEC_UINT(const TValueType paValue) : mData(paValue) {
      }

      ~CIEC_UINT() override = default;

      CIEC_UINT &operator=(const CIEC_UINT &paValue) {
        mData = paValue.mData;
        return *this;
      }

      template<typename T,
               std::enable_if_t<std::is_same_v<typename mpl::implicit_cast_t<T, CIEC_UINT>, CIEC_UINT>, int> = 0>
      CIEC_UINT &operator=(const T &paValue) {
        mData = static_cast<TValueType>(static_cast<typename T::TValueType>(paValue));
        return *this;
      }

      /*! \brief Converts CIEC_UDINT to elementary unsigned 16 bit integer
       *
       *   Conversion operator for converting CIEC_UDINT to elementary unsigned 16 bit integer
       */
      constexpr explicit operator TForteUInt16() const {
        return mData;
      }

      EDataTypeID getDataTypeID() const override {
        return e_UINT;
      }

    protected:
      TValueType mData = {};
  };

  constexpr inline CIEC_UINT operator""_UINT(unsigned long long int paValue) {
    return CIEC_UINT(static_cast<CIEC_UINT::TValueType>(paValue));
  }

  template<>
  struct CDataTypeTrait<CIEC_UINT> {
      static constexpr CIEC_ANY::EDataTypeID scmDataTypeId = CIEC_ANY::e_UINT;
      static const StringId scmDataTypeName;
  };
} // namespace forte

namespace std {
  template<>
  struct numeric_limits<forte::CIEC_UINT> : public forte::templates::numeric_limits<forte::CIEC_UINT> {
      static constexpr size_t bitLength = 16U;
  };
} // namespace std
