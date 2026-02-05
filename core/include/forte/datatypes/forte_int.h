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
 *     Ingo Hegny, Monika Wenger, Martin Melik Merkumians
 *                - initial implementation and rework communication infrastructure
 *   Martin Melik Merkumians - make TForteInt16 constructor explicit, added valid
 *                  cast constructors, removed built-in type operator=, added
 *                  castable CIEC types operator=
 *   Martin Jobst - add user-defined literal
 *                - add ANY_SIGNED
 *   Alois Zoitl  - migrated data type toString to std::string
 *   Franz Höpfinger - add constexpr
 *******************************************************************************/

#pragma once

#include "forte/iec61131_cast_helper.h"
#include "forte/datatypes/forte_any_signed.h"
#include "forte/datatypes/forte_sint.h"
#include "forte/datatypes/forte_usint.h"
#include <limits>

namespace forte {
  /*!\ingroup COREDTS CIEC_INT represents the int data type according to IEC 61131.
   */
  class CIEC_INT final : public CIEC_ANY_SIGNED {
      DECLARE_FIRMWARE_DATATYPE(INT)

    public:
      TForteByte *getDataPtr() override {
        return reinterpret_cast<TForteByte *>(&mData);
      }

      const TForteByte *getConstDataPtr() const override {
        return reinterpret_cast<const TForteByte *>(&mData);
      }
      using TValueType = TForteInt16;
      [[deprecated("Please use the corresponding numeric_limits template")]]
      constexpr static size_t scmBitLength = 16U;
      [[deprecated("Please use the corresponding numeric_limits template")]]
      static constexpr TValueType scmMinVal = std::numeric_limits<TValueType>::min();
      [[deprecated("Please use the corresponding numeric_limits template")]]
      static constexpr TValueType scmMaxVal = std::numeric_limits<TValueType>::max();

      constexpr CIEC_INT() = default;

      constexpr CIEC_INT(const CIEC_INT &paValue) : CIEC_ANY_SIGNED(), mData(paValue.mData) {
      }

      constexpr CIEC_INT(const CIEC_SINT &paValue) :
          CIEC_ANY_SIGNED(),
          mData(static_cast<TValueType>(paValue.getSignedValue())) {
      }

      constexpr CIEC_INT(const CIEC_USINT &paValue) :
          CIEC_ANY_SIGNED(),
          mData(static_cast<TValueType>(paValue.getSignedValue())) {
      }

      constexpr explicit CIEC_INT(const CIEC_ANY_INT &paValue) :
          CIEC_ANY_SIGNED(),
          mData(static_cast<TValueType>(paValue.getSignedValue())) {
      }

      constexpr explicit CIEC_INT(const TValueType paValue) : mData(paValue) {
      }

      ~CIEC_INT() override = default;

      CIEC_INT &operator=(const CIEC_INT &paValue) {
        mData = paValue.mData;
        return *this;
      }

      template<typename T,
               std::enable_if_t<std::is_same_v<typename mpl::implicit_cast_t<T, CIEC_INT>, CIEC_INT>, int> = 0>
      CIEC_INT &operator=(const T &paValue) {
        mData = static_cast<TValueType>(static_cast<typename T::TValueType>(paValue));
        return *this;
      }

      CIEC_INT operator-() const {
        return CIEC_INT(static_cast<TValueType>(-1 * static_cast<TValueType>(*this)));
      }

      /*! \brief Converts CIEC_INT to elementary 16 bit integer
       *
       *   Conversion operator for converting CIEC_INT to elementary 16 bit integer
       */
      constexpr explicit operator TForteInt16() const {
        return mData;
      }

      EDataTypeID getDataTypeID() const override {
        return e_INT;
      }

    protected:
      TValueType mData = {};
  };

  constexpr inline CIEC_INT operator""_INT(unsigned long long int paValue) {
    return CIEC_INT(static_cast<CIEC_INT::TValueType>(paValue));
  }

  template<>
  struct CDataTypeTrait<CIEC_INT> {
      static constexpr CIEC_ANY::EDataTypeID scmDataTypeId = CIEC_ANY::e_INT;
      static const StringId scmDataTypeName;
  };
} // namespace forte

namespace std {
  template<>
  struct numeric_limits<forte::CIEC_INT> : public forte::templates::numeric_limits<forte::CIEC_INT> {
      static constexpr size_t bitLength = 16U;
  };
} // namespace std
