/*******************************************************************************
 * Copyright (c) 2023, 2025 Martin Erich Jobst,
 *                          Primetals Technologies Austria GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Martin Erich Jobst - initial implementation
 *   Alois Zoitl  - migrated data type toString to std::string
 *******************************************************************************/
#pragma once

#include <variant>

#include "forte/datatypes/forte_any.h"
#include "forte/datatypes/forte_ltime.h"
#include "forte/datatypes/forte_time.h"

namespace forte {
  using TIecAnyDurationVariantType = std::variant<
      // ANY_ELEMENTARY
      //  ANY_MAGNITUDE
      //   ANY_DURATION
      CIEC_TIME,
      CIEC_LTIME
      // end
      >;

  class CIEC_ANY_DURATION_VARIANT : public CIEC_ANY_DURATION, public TIecAnyDurationVariantType {
      DECLARE_FIRMWARE_DATATYPE(ANY_DURATION_VARIANT)
    public:
      using TIecAnyDurationVariantType::variant;
      using TIecAnyDurationVariantType::operator=;
      template<class>
      static inline constexpr bool always_false_v = false;

      CIEC_ANY_DURATION_VARIANT(const CIEC_ANY_DURATION_VARIANT &paVal) : CIEC_ANY_DURATION(), variant(paVal) {
      }

      CIEC_ANY_DURATION_VARIANT(const CIEC_ANY_DURATION &paVal) {
        setValue(paVal.unwrap());
      }

      CIEC_ANY_DURATION_VARIANT &operator=(const CIEC_ANY_DURATION_VARIANT &paOther) {
        variant::operator=(paOther);
        return *this;
      }

      CIEC_ANY_DURATION_VARIANT &operator=(const CIEC_ANY_DURATION &paOther) {
        setValue(paOther.unwrap());
        return *this;
      }

      void setValue(const CIEC_ANY &paValue) override;

      void reset() override {
        unwrap().reset();
      }

      bool setDefaultValue(EDataTypeID paDataTypeId);

      [[nodiscard]] CIEC_ANY_DURATION &unwrap() override;

      [[nodiscard]] const CIEC_ANY_DURATION &unwrap() const override;

      int fromString(const char *paValue) override;

      void toString(std::string &paTargetBuf) const override;

      [[nodiscard]] bool equals(const CIEC_ANY &paOther) const override {
        return unwrap().equals(paOther.unwrap());
      }
  };

  static_assert(std::is_copy_constructible_v<CIEC_ANY_DURATION_VARIANT>);
  static_assert(std::is_move_constructible_v<CIEC_ANY_DURATION_VARIANT>);
  static_assert(std::is_constructible_v<CIEC_ANY_DURATION_VARIANT, const CIEC_ANY_DURATION &>);
  static_assert(std::is_copy_assignable_v<CIEC_ANY_DURATION_VARIANT>);
  static_assert(std::is_assignable_v<CIEC_ANY_DURATION_VARIANT, const CIEC_ANY_DURATION &>);
  static_assert(std::is_destructible_v<CIEC_ANY_DURATION_VARIANT>);

  template<>
  struct is_generic_datatype<CIEC_ANY_DURATION_VARIANT> : std::true_type {};
} // namespace forte
