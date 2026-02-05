/*******************************************************************************
 * Copyright (c) 2005, 2026 Profactor GmbH, ACIN, fortiss GmbH
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
 *     Ingo Hegny, Martin Melik Merkumians
 *                - initial implementation and rework communication infrastructure
 *   Martin Melik Merkumians - make TForteInt8 constructor explicit,
 *                  removed built-in type operator=
 *   Martin Jobst - add user-defined literal
 *                - add ANY_SIGNED
 *   Alois Zoitl  - migrated data type toString to std::string
 *   Franz Höpfinger - add constexpr
 *******************************************************************************/

#pragma once

#include "forte/datatypes/forte_any_signed.h"
#include <limits>

namespace forte {
  /*!\ingroup COREDTS CIEC_SINT represents the SINT data type according to IEC 61131.
   */
  class CIEC_SINT final : public CIEC_ANY_SIGNED {
      DECLARE_FIRMWARE_DATATYPE(SINT)

    public:
      TForteByte *getDataPtr() override {
        return reinterpret_cast<TForteByte *>(&mData);
      }

      const TForteByte *getConstDataPtr() const override {
        return reinterpret_cast<const TForteByte *>(&mData);
      }
      using TValueType = TForteInt8;
      [[deprecated("Please use the corresponding numeric_limits template")]]
      constexpr static size_t scmBitLength = 8U;
      [[deprecated("Please use the corresponding numeric_limits template")]]
      static constexpr TValueType scmMinVal = std::numeric_limits<TValueType>::min();
      [[deprecated("Please use the corresponding numeric_limits template")]]
      static constexpr TValueType scmMaxVal = std::numeric_limits<TValueType>::max();

      constexpr CIEC_SINT() = default;

      constexpr CIEC_SINT(const CIEC_SINT &paValue) : CIEC_ANY_SIGNED(), mData(paValue.mData) {
      }

      constexpr explicit CIEC_SINT(const CIEC_ANY_INT &paValue) :
          CIEC_ANY_SIGNED(),
          mData(static_cast<TValueType>(paValue.getSignedValue())) {
      }

      constexpr explicit CIEC_SINT(const TValueType paValue) : mData(paValue) {
      }

      ~CIEC_SINT() override = default;

      CIEC_SINT &operator=(const CIEC_SINT &paValue) {
        mData = paValue.mData;
        return *this;
      }

      template<typename T,
               std::enable_if_t<std::is_same_v<typename mpl::implicit_cast_t<T, CIEC_SINT>, CIEC_SINT>, int> = 0>
      CIEC_SINT &operator=(const T &paValue) {
        mData = static_cast<TValueType>(static_cast<typename T::TValueType>(paValue));
        return *this;
      }

      CIEC_SINT operator-() const {
        return CIEC_SINT(static_cast<TValueType>(-1) * static_cast<TValueType>(*this));
      }

      /*! \brief Converts CIEC_SINT to elementary 8 bit integer
       *
       *   Conversion operator for converting CIEC_SINT to elementary 8 bit integer
       */
      constexpr explicit operator TForteInt8() const {
        return mData;
      }

      EDataTypeID getDataTypeID() const override {
        return e_SINT;
      }

    protected:
      TValueType mData = {};
  };

  constexpr inline CIEC_SINT operator""_SINT(unsigned long long int paValue) {
    return CIEC_SINT(static_cast<CIEC_SINT::TValueType>(paValue));
  }

  template<>
  struct CDataTypeTrait<CIEC_SINT> {
      static constexpr CIEC_ANY::EDataTypeID scmDataTypeId = CIEC_ANY::e_SINT;
      static const StringId scmDataTypeName;
  };
} // namespace forte

namespace std {
  template<>
  struct numeric_limits<forte::CIEC_SINT> : public forte::templates::numeric_limits<forte::CIEC_SINT> {
      static constexpr size_t bitLength = 8U;
  };
} // namespace std
