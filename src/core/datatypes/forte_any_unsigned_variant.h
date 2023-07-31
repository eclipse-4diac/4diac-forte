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
#include "forte_any_unsigned.h"
#include "forte_dint.h"
#include "forte_int.h"
#include "forte_lint.h"
#include "forte_sint.h"
#include "forte_udint.h"
#include "forte_uint.h"
#include "forte_ulint.h"
#include "forte_usint.h"

using TIecAnyUnsignedVariantType = std::variant<
// ANY_ELEMENTARY
//  ANY_MAGNITUDE
//   ANY_NUM
//    ANY_INTEGER
//     ANY_UNSIGNED
        CIEC_USINT,
        CIEC_UINT,
        CIEC_UDINT,
        CIEC_ULINT
// end
>;

class CIEC_ANY_UNSIGNED_VARIANT : public CIEC_ANY_UNSIGNED, public TIecAnyUnsignedVariantType {
DECLARE_FIRMWARE_DATATYPE(ANY_UNSIGNED_VARIANT)
public:
    using TIecAnyUnsignedVariantType::variant;
    using TIecAnyUnsignedVariantType::operator=;
    template<class> static inline constexpr bool always_false_v = false;

    CIEC_ANY_UNSIGNED_VARIANT(const CIEC_ANY_UNSIGNED_VARIANT &paVal) : CIEC_ANY_UNSIGNED(), variant(paVal) {}

    CIEC_ANY_UNSIGNED_VARIANT(const CIEC_ANY_UNSIGNED &paVal) {
      setValue(paVal.unwrap());
    }

    CIEC_ANY_UNSIGNED_VARIANT &operator=(const CIEC_ANY_UNSIGNED_VARIANT &paOther) {
      variant::operator=(paOther);
      return *this;
    }

    CIEC_ANY_UNSIGNED_VARIANT &operator=(const CIEC_ANY_UNSIGNED &paOther) {
      setValue(paOther.unwrap());
      return *this;
    }

    void setValue(const CIEC_ANY &paValue) override;

    bool setDefaultValue(CIEC_ANY::EDataTypeID paDataTypeId);

    [[nodiscard]] CIEC_ANY_UNSIGNED &unwrap() override;

    [[nodiscard]] const CIEC_ANY_UNSIGNED &unwrap() const override;

    int fromString(const char *paValue) override;

    int toString(char* paValue, size_t paBufferSize) const override;

    size_t getToStringBufferSize() const override;

    [[nodiscard]] bool equals(const CIEC_ANY &paOther) const override {
      return unwrap().equals(paOther.unwrap());
    }
};

static_assert(std::is_copy_constructible_v<CIEC_ANY_UNSIGNED_VARIANT>);
static_assert(std::is_move_constructible_v<CIEC_ANY_UNSIGNED_VARIANT>);
static_assert(std::is_constructible_v<CIEC_ANY_UNSIGNED_VARIANT, const CIEC_ANY_UNSIGNED &>);
static_assert(std::is_copy_assignable_v<CIEC_ANY_UNSIGNED_VARIANT>);
static_assert(std::is_assignable_v<CIEC_ANY_UNSIGNED_VARIANT, const CIEC_ANY_UNSIGNED &>);
static_assert(std::is_destructible_v<CIEC_ANY_UNSIGNED_VARIANT>);

