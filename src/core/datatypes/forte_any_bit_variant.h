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
#include "forte_bool.h"
#include "forte_byte.h"
#include "forte_dword.h"
#include "forte_lword.h"
#include "forte_word.h"

using TIecAnyBitVariantType = std::variant<
// ANY_ELEMENTARY
//  ANY_BIT
        CIEC_BOOL,
        CIEC_BYTE,
        CIEC_WORD,
        CIEC_DWORD,
        CIEC_LWORD
// end
>;

class CIEC_ANY_BIT_VARIANT : public CIEC_ANY_BIT, public TIecAnyBitVariantType {
DECLARE_FIRMWARE_DATATYPE(ANY_BIT_VARIANT)
public:
    using TIecAnyBitVariantType::variant;
    using TIecAnyBitVariantType::operator=;
    template<class> static inline constexpr bool always_false_v = false;

    CIEC_ANY_BIT_VARIANT(const CIEC_ANY_BIT_VARIANT &paVal) : CIEC_ANY_BIT(), variant(paVal) {}

    CIEC_ANY_BIT_VARIANT(const CIEC_ANY_BIT &paVal) {
      setValue(paVal.unwrap());
    }

    CIEC_ANY_BIT_VARIANT &operator=(const CIEC_ANY_BIT_VARIANT &paOther) {
      variant::operator=(paOther);
      return *this;
    }

    CIEC_ANY_BIT_VARIANT &operator=(const CIEC_ANY_BIT &paOther) {
      setValue(paOther.unwrap());
      return *this;
    }

    void setValue(const CIEC_ANY &paValue) override;

    bool setDefaultValue(CIEC_ANY::EDataTypeID paDataTypeId);

    [[nodiscard]] CIEC_ANY_BIT &unwrap() override;

    [[nodiscard]] const CIEC_ANY_BIT &unwrap() const override;

    int fromString(const char *paValue) override;

    int toString(char* paValue, size_t paBufferSize) const override;

    size_t getToStringBufferSize() const override;

    [[nodiscard]] bool equals(const CIEC_ANY &paOther) const override {
      return unwrap().equals(paOther.unwrap());
    }
};

static_assert(std::is_copy_constructible_v<CIEC_ANY_BIT_VARIANT>);
static_assert(std::is_move_constructible_v<CIEC_ANY_BIT_VARIANT>);
static_assert(std::is_constructible_v<CIEC_ANY_BIT_VARIANT, const CIEC_ANY_BIT &>);
static_assert(std::is_copy_assignable_v<CIEC_ANY_BIT_VARIANT>);
static_assert(std::is_assignable_v<CIEC_ANY_BIT_VARIANT, const CIEC_ANY_BIT &>);
static_assert(std::is_destructible_v<CIEC_ANY_BIT_VARIANT>);

