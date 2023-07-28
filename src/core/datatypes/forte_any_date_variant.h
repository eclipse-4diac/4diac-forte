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
#include "forte_date.h"
#include "forte_date_and_time.h"
#include "forte_ldate.h"
#include "forte_ldate_and_time.h"
#include "forte_ltime_of_day.h"
#include "forte_time_of_day.h"

using TIecAnyDateVariantType = std::variant<
// ANY_ELEMENTARY
//  ANY_DATE
        CIEC_DATE,
        CIEC_LDATE,
        CIEC_DATE_AND_TIME,
        CIEC_LDATE_AND_TIME,
        CIEC_TIME_OF_DAY,
        CIEC_LTIME_OF_DAY
// end
>;

class CIEC_ANY_DATE_VARIANT : public CIEC_ANY_DATE, public TIecAnyDateVariantType {
DECLARE_FIRMWARE_DATATYPE(ANY_DATE_VARIANT)
public:
    using TIecAnyDateVariantType::variant;
    using TIecAnyDateVariantType::operator=;
    template<class> static inline constexpr bool always_false_v = false;

    CIEC_ANY_DATE_VARIANT(const CIEC_ANY_DATE_VARIANT &paVal) : CIEC_ANY_DATE(), variant(paVal) {}

    CIEC_ANY_DATE_VARIANT(const CIEC_ANY_DATE &paVal) {
      setValue(paVal.unwrap());
    }

    CIEC_ANY_DATE_VARIANT &operator=(const CIEC_ANY_DATE_VARIANT &paOther) {
      variant::operator=(paOther);
      return *this;
    }

    CIEC_ANY_DATE_VARIANT &operator=(const CIEC_ANY_DATE &paOther) {
      setValue(paOther.unwrap());
      return *this;
    }

    void setValue(const CIEC_ANY &paValue) override;

    bool setDefaultValue(CIEC_ANY::EDataTypeID paDataTypeId);

    [[nodiscard]] CIEC_ANY_DATE &unwrap() override;

    [[nodiscard]] const CIEC_ANY_DATE &unwrap() const override;

    int fromString(const char *paValue) override;

    int toString(char* paValue, size_t paBufferSize) const override;

    size_t getToStringBufferSize() const override;

    [[nodiscard]] bool equals(const CIEC_ANY &paOther) const override {
      return unwrap().equals(paOther.unwrap());
    }
};

static_assert(std::is_copy_constructible_v<CIEC_ANY_DATE_VARIANT>);
static_assert(std::is_move_constructible_v<CIEC_ANY_DATE_VARIANT>);
static_assert(std::is_constructible_v<CIEC_ANY_DATE_VARIANT, const CIEC_ANY_DATE &>);
static_assert(std::is_copy_assignable_v<CIEC_ANY_DATE_VARIANT>);
static_assert(std::is_assignable_v<CIEC_ANY_DATE_VARIANT, const CIEC_ANY_DATE &>);
static_assert(std::is_destructible_v<CIEC_ANY_DATE_VARIANT>);

