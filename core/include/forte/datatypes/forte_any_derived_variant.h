/*******************************************************************************
 * Copyright (c) 2026 Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Martin Erich Jobst - initial implementation
 *******************************************************************************/
#pragma once

#include <variant>

#include "forte/datatypes/forte_any.h"
#include "forte/datatypes/forte_any_struct.h"
#include "forte/datatypes/forte_any_unique_ptr.h"
#include "forte/datatypes/forte_array_common.h"
#include "forte/datatypes/forte_struct.h"

namespace forte {
  using TIecAnyDerivedVariantType = std::variant<
      // ANY_DERIVED
      CIEC_ANY_UNIQUE_PTR<CIEC_ARRAY>,
      CIEC_ANY_UNIQUE_PTR<CIEC_STRUCT>
      // end
      >;

  class CIEC_ANY_DERIVED_VARIANT : public CIEC_ANY, public TIecAnyDerivedVariantType {
      DECLARE_FIRMWARE_DATATYPE(ANY_DERIVED_VARIANT)
    public:
      using TIecAnyDerivedVariantType::variant;
      using TIecAnyDerivedVariantType::operator=;
      template<class>
      static inline constexpr bool always_false_v = false;

      CIEC_ANY_DERIVED_VARIANT() : variant(std::make_unique<CIEC_ANY_STRUCT>()) {
      }

      CIEC_ANY_DERIVED_VARIANT(const CIEC_ANY_DERIVED_VARIANT &paVal) : variant(paVal) {
      }

      CIEC_ANY_DERIVED_VARIANT(const CIEC_ANY &paVal) {
        setValue(paVal.unwrap());
      }

      CIEC_ANY_DERIVED_VARIANT &operator=(const CIEC_ANY_DERIVED_VARIANT &paOther) {
        variant::operator=(paOther);
        return *this;
      }

      CIEC_ANY_DERIVED_VARIANT &operator=(const CIEC_ANY &paOther) {
        setValue(paOther.unwrap());
        return *this;
      }

      void setValue(const CIEC_ANY &paValue) override;

      void reset() override {
        unwrap().reset();
      }

      [[nodiscard]] CIEC_ANY &unwrap() override;

      [[nodiscard]] const CIEC_ANY &unwrap() const override;

      int fromString(const char *paValue) override;
      void toString(std::string &paTargetBuf) const override;

      [[nodiscard]] bool equals(const CIEC_ANY &paOther) const override {
        return unwrap().equals(paOther.unwrap());
      }
  };

  static_assert(std::is_copy_constructible_v<CIEC_ANY_DERIVED_VARIANT>);
  static_assert(std::is_move_constructible_v<CIEC_ANY_DERIVED_VARIANT>);
  static_assert(std::is_constructible_v<CIEC_ANY_DERIVED_VARIANT, const CIEC_ANY &>);
  static_assert(std::is_copy_assignable_v<CIEC_ANY_DERIVED_VARIANT>);
  static_assert(std::is_assignable_v<CIEC_ANY_DERIVED_VARIANT, const CIEC_ANY &>);
  static_assert(std::is_destructible_v<CIEC_ANY_DERIVED_VARIANT>);

  template<>
  struct is_generic_datatype<CIEC_ANY_DERIVED_VARIANT> : std::true_type {};
} // namespace forte
