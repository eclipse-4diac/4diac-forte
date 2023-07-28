/*******************************************************************************
 * Copyright (c) 2023 Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Martin Erich Jobst - initial implementation
 *******************************************************************************/
#pragma once

#include <memory>
#include <variant>

#include "forte_any.h"
#include "forte_lreal.h"
#include "forte_real.h"

using TIecAnyRealVariantType = std::variant<
// ANY_ELEMENTARY
//  ANY_MAGNITUDE
//   ANY_NUM
//    ANY_REAL
        CIEC_REAL,
        CIEC_LREAL
// end
>;

class CIEC_ANY_REAL_VARIANT : public CIEC_ANY_REAL, public TIecAnyRealVariantType {
DECLARE_FIRMWARE_DATATYPE(ANY_REAL_VARIANT)
public:
    using TIecAnyRealVariantType::variant;
    using TIecAnyRealVariantType::operator=;
    template<class> static inline constexpr bool always_false_v = false;

    CIEC_ANY_REAL_VARIANT(const CIEC_ANY_REAL_VARIANT &paVal) : CIEC_ANY_REAL(), variant(paVal) {}

    CIEC_ANY_REAL_VARIANT(const CIEC_ANY_REAL &paVal) {
      setValue(paVal.unwrap());
    }

    CIEC_ANY_REAL_VARIANT &operator=(const CIEC_ANY_REAL_VARIANT &paOther) {
      variant::operator=(paOther);
      return *this;
    }

    CIEC_ANY_REAL_VARIANT &operator=(const CIEC_ANY_REAL &paOther) {
      setValue(paOther.unwrap());
      return *this;
    }

    void setValue(const CIEC_ANY &paValue) override;

    bool setDefaultValue(CIEC_ANY::EDataTypeID paDataTypeId);

    [[nodiscard]] CIEC_ANY_REAL &unwrap() override;

    [[nodiscard]] const CIEC_ANY_REAL &unwrap() const override;

    int fromString(const char *paValue) override;

    int toString(char* paValue, size_t paBufferSize) const override;

    size_t getToStringBufferSize() const override;

    [[nodiscard]] bool equals(const CIEC_ANY &paOther) const override {
      return unwrap().equals(paOther.unwrap());
    }
};

static_assert(std::is_copy_constructible_v<CIEC_ANY_REAL_VARIANT>);
static_assert(std::is_move_constructible_v<CIEC_ANY_REAL_VARIANT>);
static_assert(std::is_constructible_v<CIEC_ANY_REAL_VARIANT, const CIEC_ANY_REAL &>);
static_assert(std::is_copy_assignable_v<CIEC_ANY_REAL_VARIANT>);
static_assert(std::is_assignable_v<CIEC_ANY_REAL_VARIANT, const CIEC_ANY_REAL &>);
static_assert(std::is_destructible_v<CIEC_ANY_REAL_VARIANT>);

