/*******************************************************************************
 * Copyright (c) 2022 Primetals Technologies Austria GmbH
 *               2023 Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Martin Melik Merkumians
 *      - initial implementation and rework communication infrastructure
 *    Martin Jobst
 *      - refactored for common assignment operators
 *      - refactored array type structure
 *******************************************************************************/
#pragma once

#include <stddef.h>
#include <inttypes.h>
#include <initializer_list>

#include "forte_any_derived.h"
#include "forte_any_int.h"

/** \brief A common supertype for all CIEC_ARRAY variants, providing the minimal interface an array must provide
 */
class CIEC_ARRAY : public CIEC_ANY_DERIVED {
public:
    using value_type = CIEC_ANY;
    using pointer = value_type *;
    using const_pointer = const value_type *;
    using reference = value_type &;
    using const_reference = const value_type &;

    [[nodiscard]] virtual intmax_t getLowerBound() const = 0;

    [[nodiscard]] virtual intmax_t getUpperBound() const = 0;

    [[nodiscard]] virtual size_t size() const = 0;

    [[nodiscard]] virtual CIEC_ANY::EDataTypeID getElementDataTypeID() const = 0;

    [[nodiscard]] virtual CStringDictionary::TStringId getElementTypeNameID() const = 0;

    [[nodiscard]] virtual reference at(intmax_t index) = 0;

    [[nodiscard]] virtual reference operator[](intmax_t index) = 0;

    [[nodiscard]] virtual const_reference at(intmax_t index) const = 0;

    [[nodiscard]] virtual const_reference operator[](intmax_t index) const = 0;

    CIEC_ARRAY &operator=(const CIEC_ARRAY &paSource) {
      assignDynamic(paSource, paSource.getLowerBound(), paSource.getUpperBound());
      return *this;
    }

    [[nodiscard]] virtual reference at(const CIEC_ANY_INT &index) {
      const intmax_t indexValue = index.getSignedValue();
      return at(indexValue);
    }

    [[nodiscard]] virtual reference operator[](const CIEC_ANY_INT &index) {
      const intmax_t indexValue = index.getSignedValue();
      return operator[](indexValue);
    }

    [[nodiscard]] virtual const_reference at(const CIEC_ANY_INT &index) const {
      const intmax_t indexValue = index.getSignedValue();
      return at(indexValue);
    }

    [[nodiscard]] virtual const_reference operator[](const CIEC_ANY_INT &index) const {
      const intmax_t indexValue = index.getSignedValue();
      return operator[](indexValue);
    }

    [[nodiscard]] EDataTypeID getDataTypeID() const final {
      return CIEC_ANY::e_ARRAY;
    }

    CStringDictionary::TStringId getTypeNameID() const override;

    void setValue(const CIEC_ANY &paValue) override;

    [[nodiscard]] bool equals(const CIEC_ANY &paOther) const override;

    [[nodiscard]] int toString(char *paValue, size_t paBufferSize) const override;

    [[nodiscard]] size_t getToStringBufferSize() const override;

    ~CIEC_ARRAY() override = default;

protected:
    CIEC_ARRAY() = default;

    static void findNextNonBlankSpace(const char **paRunner) {
      while (' ' == **paRunner) {
        (*paRunner)++;
      }
    }

    static const intmax_t cmCollapseMaxSize = 100;

private:
    [[nodiscard]] int toCollapsedString(char *paValue, size_t paBufferSize) const;

    [[nodiscard]] int toCollapsedElementString(const CIEC_ANY &paElement, size_t paCount, bool paComma, char *paValue,
                                               size_t paBufferSize) const;

    template<typename U>
    inline void assignDynamic(const U &paArray, intmax_t sourceLowerBound, intmax_t sourceUpperBound) {
      if(size() && paArray.size()) { // check if initialized
        intmax_t begin = std::max(getLowerBound(), sourceLowerBound);
        intmax_t end = std::min(getUpperBound(), sourceUpperBound);
        for (intmax_t i = begin; i <= end; ++i) {
          (*this)[i].setValue(paArray[i]);
        }
      }
    }
};

static_assert(std::is_copy_assignable_v<CIEC_ARRAY>);
static_assert(std::is_destructible_v<CIEC_ARRAY>);

