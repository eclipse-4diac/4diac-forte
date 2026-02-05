/*******************************************************************************
 * Copyright (c) 2005, 2026 Profactor GmbH, ACIN,
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
 *     Ingo Hegny, Monika Wenger, Martin Melik Merkumians
 *      - initial implementation and rework communication infrastructure
 *   Martin Melik Merkumians - make TForteUInt32 constructor explicit,
 *      removed built-in type operator=, added castable CIEC types operator=
 *   Martin Jobst - add user-defined literal
 *   Alois Zoitl  - migrated data type toString to std::string
 *   Franz Höpfinger - add constexpr
 *******************************************************************************/

#pragma once

#include "forte/datatypes/forte_any_unsigned.h"
#include "forte/datatypes/forte_uint.h"
#include "forte/datatypes/forte_usint.h"
#include <limits>

namespace forte {
  /*!\ingroup COREDTS CIEC_UDINT represents the udint data type according to IEC 61131.
   */
  class CIEC_UDINT final : public CIEC_ANY_UNSIGNED {
      DECLARE_FIRMWARE_DATATYPE(UDINT)

    public:
      TForteByte *getDataPtr() override {
        return reinterpret_cast<TForteByte *>(&mData);
      }

      const TForteByte *getConstDataPtr() const override {
        return reinterpret_cast<const TForteByte *>(&mData);
      }
      using TValueType = TForteUInt32;
      [[deprecated("Please use the corresponding numeric_limits template")]]
      constexpr static size_t scmBitLength = 32U;
      [[deprecated("Please use the corresponding numeric_limits template")]]
      static constexpr TValueType scmMinVal = std::numeric_limits<TValueType>::min();
      [[deprecated("Please use the corresponding numeric_limits template")]]
      static constexpr TValueType scmMaxVal = std::numeric_limits<TValueType>::max();

      constexpr CIEC_UDINT() = default;

      constexpr CIEC_UDINT(const CIEC_UDINT &paValue) : CIEC_ANY_UNSIGNED(), mData(paValue.mData) {
      }

      constexpr CIEC_UDINT(const CIEC_UINT &paValue) :
          CIEC_ANY_UNSIGNED(),
          mData(static_cast<TValueType>(paValue.getSignedValue())) {
      }

      constexpr CIEC_UDINT(const CIEC_USINT &paValue) :
          CIEC_ANY_UNSIGNED(),
          mData(static_cast<TValueType>(paValue.getSignedValue())) {
      }

      constexpr explicit CIEC_UDINT(const CIEC_ANY_INT &paValue) :
          CIEC_ANY_UNSIGNED(),
          mData(static_cast<TValueType>(paValue.getSignedValue())) {
      }

      constexpr explicit CIEC_UDINT(const TValueType paValue) : mData(paValue) {
      }

      ~CIEC_UDINT() override = default;

      CIEC_UDINT &operator=(const CIEC_UDINT &paValue) {
        mData = paValue.mData;
        return *this;
      }

      template<typename T,
               std::enable_if_t<std::is_same_v<typename mpl::implicit_cast_t<T, CIEC_UDINT>, CIEC_UDINT>, int> = 0>
      CIEC_UDINT &operator=(const T &paValue) {
        mData = static_cast<TValueType>(static_cast<typename T::TValueType>(paValue));
        return *this;
      }

      /*! \brief Converts CIEC_UDINT to elementary unsigned 32 bit integer
       *
       *   Conversion operator for converting CIEC_UDINT to elementary unsigned 32 bit integer
       */
      constexpr explicit operator TForteUInt32() const {
        return mData;
      }

      EDataTypeID getDataTypeID() const override {
        return e_UDINT;
      }

    protected:
      TValueType mData = {};
  };

  constexpr inline CIEC_UDINT operator""_UDINT(unsigned long long int paValue) {
    return CIEC_UDINT(static_cast<CIEC_UDINT::TValueType>(paValue));
  }

  template<>
  struct CDataTypeTrait<CIEC_UDINT> {
      static constexpr CIEC_ANY::EDataTypeID scmDataTypeId = CIEC_ANY::e_UDINT;
      static const StringId scmDataTypeName;
  };
} // namespace forte

namespace std {
  template<>
  struct numeric_limits<forte::CIEC_UDINT> : public forte::templates::numeric_limits<forte::CIEC_UDINT> {
      static constexpr size_t bitLength = 32U;
  };
} // namespace std
