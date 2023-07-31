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
#include "forte_any_signed.h"
#include "forte_dint.h"
#include "forte_int.h"
#include "forte_lint.h"
#include "forte_sint.h"

using TIecAnySignedVariantType = std::variant<
// ANY_ELEMENTARY
//  ANY_MAGNITUDE
//   ANY_NUM
//    ANY_INTEGER
//     ANY_SIGNED
        CIEC_SINT,
        CIEC_INT,
        CIEC_DINT,
        CIEC_LINT
// end
>;

class CIEC_ANY_SIGNED_VARIANT : public CIEC_ANY_SIGNED, public TIecAnySignedVariantType {
DECLARE_FIRMWARE_DATATYPE(ANY_SIGNED_VARIANT)
public:
    using TIecAnySignedVariantType::variant;
    using TIecAnySignedVariantType::operator=;
    template<class> static inline constexpr bool always_false_v = false;

    CIEC_ANY_SIGNED_VARIANT(const CIEC_ANY_SIGNED_VARIANT &paVal) : CIEC_ANY_SIGNED(), variant(paVal) {}

    CIEC_ANY_SIGNED_VARIANT(const CIEC_ANY_SIGNED &paVal) {
      setValue(paVal.unwrap());
    }

    CIEC_ANY_SIGNED_VARIANT &operator=(const CIEC_ANY_SIGNED_VARIANT &paOther) {
      variant::operator=(paOther);
      return *this;
    }

    CIEC_ANY_SIGNED_VARIANT &operator=(const CIEC_ANY_SIGNED &paOther) {
      setValue(paOther.unwrap());
      return *this;
    }

    void setValue(const CIEC_ANY &paValue) override;

    bool setDefaultValue(CIEC_ANY::EDataTypeID paDataTypeId);

    [[nodiscard]] CIEC_ANY_SIGNED &unwrap() override;

    [[nodiscard]] const CIEC_ANY_SIGNED &unwrap() const override;

    int fromString(const char *paValue) override;

    int toString(char* paValue, size_t paBufferSize) const override;

    size_t getToStringBufferSize() const override;

    [[nodiscard]] bool equals(const CIEC_ANY &paOther) const override {
      return unwrap().equals(paOther.unwrap());
    }
};

static_assert(std::is_copy_constructible_v<CIEC_ANY_SIGNED_VARIANT>);
static_assert(std::is_move_constructible_v<CIEC_ANY_SIGNED_VARIANT>);
static_assert(std::is_constructible_v<CIEC_ANY_SIGNED_VARIANT, const CIEC_ANY_SIGNED &>);
static_assert(std::is_copy_assignable_v<CIEC_ANY_SIGNED_VARIANT>);
static_assert(std::is_assignable_v<CIEC_ANY_SIGNED_VARIANT, const CIEC_ANY_SIGNED &>);
static_assert(std::is_destructible_v<CIEC_ANY_SIGNED_VARIANT>);

