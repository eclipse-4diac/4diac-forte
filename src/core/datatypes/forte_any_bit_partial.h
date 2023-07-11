/*******************************************************************************
 * Copyright (c) 2023 Primetals Technologies Austria GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Martin Melik Merkumians
 *      - initial implementation
 *******************************************************************************/

#pragma once

#include <limits>
#include "iec61131_cast_helper.h"
#include "devlog.h"

template<typename PartialType, typename SourceType>
class CIEC_ANY_BIT_PARTIAL final : public PartialType {
  private:
    using SourceValueType = typename SourceType::TValueType;
    using PartialValueType = typename PartialType::TValueType;

    static constexpr auto scmSourceDigits = std::numeric_limits<SourceValueType>::digits;
    static constexpr auto scmPartialDigits = std::numeric_limits<PartialValueType>::digits;
    static constexpr auto scmMaxIndex = scmSourceDigits / scmPartialDigits - 1;

  public:
    using PartialType::operator=;

    CIEC_ANY_BIT_PARTIAL(SourceType &paValue, const size_t paIndex) : mOriginalValue(paValue), mIndex(paIndex)  {
      PartialValueType partialValue = 0;
      if(paIndex <= scmMaxIndex) {
        const auto shiftIndex = mIndex * scmPartialDigits;
        const SourceValueType sourceValue = static_cast<SourceValueType>(paValue);
        if constexpr(std::is_same_v<CIEC_BOOL, PartialType>) {
          partialValue = static_cast<PartialValueType>((sourceValue >> shiftIndex) & 0x1);
        } else {
          partialValue = static_cast<PartialValueType>(sourceValue >> shiftIndex);
        }
      } else {
        DEVLOG_ERROR("Attempted partial index %d outside the range of allowed indices %d\n", paIndex, scmMaxIndex);
      }
      *this = PartialType(partialValue);
    }

    CIEC_ANY_BIT_PARTIAL& operator=(const CIEC_ANY_BIT_PARTIAL &paValue) {
      *this = PartialType(paValue);
      return *this;
    }

    ~CIEC_ANY_BIT_PARTIAL() override {
      if(mIndex <= scmMaxIndex) {
        constexpr SourceValueType maskTemplate = std::numeric_limits<PartialValueType>::max();
        const auto shiftIndex = mIndex * scmPartialDigits;
        const SourceValueType mask = (maskTemplate << shiftIndex);
        const SourceValueType partialPartValue = static_cast<SourceValueType>(static_cast<PartialValueType>(*this)) << shiftIndex;
        const SourceValueType sourceValue = static_cast<SourceValueType>(mOriginalValue);
        SourceValueType mergedValue = sourceValue ^ ((sourceValue ^ partialPartValue) & mask);
        mOriginalValue = SourceType(mergedValue);
      }
    }

  private:
    SourceType &mOriginalValue;
    const size_t mIndex;

    static_assert(std::is_base_of_v<CIEC_ANY_BIT, PartialType>);
    static_assert(std::is_base_of_v<CIEC_ANY_BIT, SourceType>);
    static_assert(forte::core::mpl::is_implicitly_castable_v<PartialType, SourceType>);
};
