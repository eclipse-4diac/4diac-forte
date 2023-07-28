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
#include "forte_any_unique_ptr.h"
#include "forte_array_common.h"
#include "forte_bool.h"
#include "forte_byte.h"
#include "forte_char.h"
#include "forte_date.h"
#include "forte_date_and_time.h"
#include "forte_dint.h"
#include "forte_dword.h"
#include "forte_int.h"
#include "forte_ldate.h"
#include "forte_ldate_and_time.h"
#include "forte_lint.h"
#include "forte_lreal.h"
#include "forte_ltime.h"
#include "forte_ltime_of_day.h"
#include "forte_lword.h"
#include "forte_real.h"
#include "forte_sint.h"
#include "forte_string.h"
#include "forte_struct.h"
#include "forte_time.h"
#include "forte_time_of_day.h"
#include "forte_udint.h"
#include "forte_uint.h"
#include "forte_ulint.h"
#include "forte_usint.h"
#include "forte_wchar.h"
#include "forte_word.h"
#include "forte_wstring.h"

using TIecAnyElementaryVariantType = std::variant<
// ANY_ELEMENTARY
//  ANY_MAGNITUDE
//   ANY_NUM
//    ANY_INTEGER
//     ANY_SIGNED
        CIEC_SINT,
        CIEC_INT,
        CIEC_DINT,
        CIEC_LINT,
//     ANY_UNSINED
        CIEC_USINT,
        CIEC_UINT,
        CIEC_UDINT,
        CIEC_ULINT,
//    ANY_REAL
        CIEC_REAL,
        CIEC_LREAL,
//   ANY_DURATION
        CIEC_TIME,
        CIEC_LTIME,
//  ANY_BIT
        CIEC_BOOL,
        CIEC_BYTE,
        CIEC_WORD,
        CIEC_DWORD,
        CIEC_LWORD,
//  ANY_CHARS
//   ANY_CHAR
        CIEC_CHAR,
        CIEC_WCHAR,
//   ANY_STRING
        CIEC_STRING,
        CIEC_WSTRING,
//  ANY_DATE
        CIEC_DATE,
        CIEC_LDATE,
        CIEC_DATE_AND_TIME,
        CIEC_LDATE_AND_TIME,
        CIEC_TIME_OF_DAY,
        CIEC_LTIME_OF_DAY
// end
>;

class CIEC_ANY_ELEMENTARY_VARIANT : public CIEC_ANY_ELEMENTARY, public TIecAnyElementaryVariantType {
DECLARE_FIRMWARE_DATATYPE(ANY_ELEMENTARY_VARIANT)
public:
    using TIecAnyElementaryVariantType::variant;
    using TIecAnyElementaryVariantType::operator=;
    template<class...> static inline constexpr bool always_false_v = false;

    CIEC_ANY_ELEMENTARY_VARIANT(const CIEC_ANY_ELEMENTARY_VARIANT &paVal) : CIEC_ANY_ELEMENTARY(), variant(paVal) {}

    CIEC_ANY_ELEMENTARY_VARIANT(const CIEC_ANY_ELEMENTARY &paVal) {
      setValue(paVal.unwrap());
    }

    CIEC_ANY_ELEMENTARY_VARIANT &operator=(const CIEC_ANY_ELEMENTARY_VARIANT &paOther) {
      variant::operator=(paOther);
      return *this;
    }

    CIEC_ANY_ELEMENTARY_VARIANT &operator=(const CIEC_ANY_ELEMENTARY &paOther) {
      setValue(paOther.unwrap());
      return *this;
    }

    void setValue(const CIEC_ANY &paValue) override;

    bool setDefaultValue(CIEC_ANY::EDataTypeID paDataTypeId);

    [[nodiscard]] CIEC_ANY_ELEMENTARY &unwrap() override;

    [[nodiscard]] const CIEC_ANY_ELEMENTARY &unwrap() const override;

    int fromString(const char *paValue) override;

    int toString(char* paValue, size_t paBufferSize) const override;

    size_t getToStringBufferSize() const override;

    [[nodiscard]] bool equals(const CIEC_ANY &paOther) const override {
      return unwrap().equals(paOther.unwrap());
    }

    [[nodiscard]] static int compare(const CIEC_ANY_ELEMENTARY_VARIANT &paValue,
                                     const CIEC_ANY_ELEMENTARY_VARIANT &paOther);
};

inline bool operator==(const CIEC_ANY_ELEMENTARY_VARIANT &paValue, const CIEC_ANY_ELEMENTARY_VARIANT &paOther) {
  return CIEC_ANY_ELEMENTARY_VARIANT::compare(paValue, paOther) == 0;
}

inline bool operator!=(const CIEC_ANY_ELEMENTARY_VARIANT &paValue, const CIEC_ANY_ELEMENTARY_VARIANT &paOther) {
  return CIEC_ANY_ELEMENTARY_VARIANT::compare(paValue, paOther) != 0;
}

inline bool operator<(const CIEC_ANY_ELEMENTARY_VARIANT &paValue, const CIEC_ANY_ELEMENTARY_VARIANT &paOther) {
  return CIEC_ANY_ELEMENTARY_VARIANT::compare(paValue, paOther) < 0;
}

inline bool operator<=(const CIEC_ANY_ELEMENTARY_VARIANT &paValue, const CIEC_ANY_ELEMENTARY_VARIANT &paOther) {
  return CIEC_ANY_ELEMENTARY_VARIANT::compare(paValue, paOther) <= 0;
}

inline bool operator>(const CIEC_ANY_ELEMENTARY_VARIANT &paValue, const CIEC_ANY_ELEMENTARY_VARIANT &paOther) {
  return CIEC_ANY_ELEMENTARY_VARIANT::compare(paValue, paOther) > 0;
}

inline bool operator>=(const CIEC_ANY_ELEMENTARY_VARIANT &paValue, const CIEC_ANY_ELEMENTARY_VARIANT &paOther) {
  return CIEC_ANY_ELEMENTARY_VARIANT::compare(paValue, paOther) >= 0;
}

static_assert(std::is_copy_constructible_v<CIEC_ANY_ELEMENTARY_VARIANT>);
static_assert(std::is_move_constructible_v<CIEC_ANY_ELEMENTARY_VARIANT>);
static_assert(std::is_constructible_v<CIEC_ANY_ELEMENTARY_VARIANT, const CIEC_ANY_ELEMENTARY &>);
static_assert(std::is_copy_assignable_v<CIEC_ANY_ELEMENTARY_VARIANT>);
static_assert(std::is_assignable_v<CIEC_ANY_ELEMENTARY_VARIANT, const CIEC_ANY_ELEMENTARY &>);
static_assert(std::is_destructible_v<CIEC_ANY_ELEMENTARY_VARIANT>);

