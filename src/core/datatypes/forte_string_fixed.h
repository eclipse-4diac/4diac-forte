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

#include "forte_string.h"
#include "forte_char.h"
#include <string>
#include <algorithm>

template<size_t maxLength>
class CIEC_STRING_FIXED final : public CIEC_STRING {
  static_assert(maxLength > 0, "Length must be larger than 0");
  static_assert(maxLength <= CIEC_STRING::scmMaxStringLen, "Length must be smaller than CIEC_STRING::scmMaxStringLen");
  public:
    using CIEC_STRING::at;
    using CIEC_STRING::operator[];

    CIEC_STRING_FIXED() : CIEC_STRING() {}

    //Char has size 1 so it should always fit
    CIEC_STRING_FIXED(const CIEC_CHAR &paValue) : CIEC_STRING(paValue) {}

    // Copy maxLength substring from CIEC_STRING
    CIEC_STRING_FIXED(const CIEC_STRING &paValue) : CIEC_STRING(paValue.getStorage().substr(0, maxLength)) {}

    // Same size, just copy
    CIEC_STRING_FIXED(const CIEC_STRING_FIXED &paValue) : CIEC_STRING(paValue) {}
    // Same size, just move
    CIEC_STRING_FIXED(CIEC_STRING_FIXED &&paValue) : CIEC_STRING(std::move(paValue)) {}

    template <size_t otherLength>
    CIEC_STRING_FIXED(const CIEC_STRING_FIXED<otherLength> &paValue) : CIEC_STRING(paValue.getStorage().substr(0, std::min(maxLength, otherLength))) {}

    template <size_t otherLength>
    CIEC_STRING_FIXED(CIEC_STRING_FIXED<otherLength> &&paValue) : CIEC_STRING(std::move(paValue)) {
      const size_t currentLength = static_cast<size_t>(this->length());
      // currentLength should be never larger than maxLengths
      getStorageMutable().resize(std::min(currentLength, maxLength));
    }

    explicit CIEC_STRING_FIXED(const std::string paValue) : CIEC_STRING(paValue.substr(0, maxLength)) {}
    explicit CIEC_STRING_FIXED(const char *paValue, const size_t paLength) : CIEC_STRING(paValue, std::min(paLength, maxLength)) {}

    ~CIEC_STRING_FIXED() = default;

    // Also handles the case of two different fix sized strings, as only the own maxLength is relevant
    CIEC_STRING_FIXED &operator=(const CIEC_STRING &paValue) {
      const std::string_view stringView(paValue.getStorage());
      getStorageMutable() = stringView.substr(0, maxLength);
      return *this;
    }

    CIEC_STRING_FIXED &operator=(CIEC_STRING &&paValue) {
      CIEC_STRING::operator=(std::move(paValue));
      const size_t currentLength = static_cast<size_t>(this->length());
      getStorageMutable().resize(std::min(currentLength, maxLength));
      return *this;
    }

    CIEC_STRING_FIXED &operator=(const CIEC_STRING_FIXED &paValue) {
      getStorageMutable() = paValue.getStorage();
      return *this;
    }

    CIEC_STRING_FIXED &operator=(CIEC_STRING_FIXED &&paValue) {
      getStorageMutable() = std::move(paValue.getStorageMutable());
      return *this;
    }

    using CIEC_STRING::operator=;

    size_t getMaximumLength() const override {
      return maxLength;
    }

    void reserve(const TForteUInt16 paRequestedSize) override {
      if (paRequestedSize > maxLength) {
        DEVLOG_WARNING("Attempt to reserve %zu chars, which is more than the CIEC_STRING_FIXED<%zu> shall support!", paRequestedSize, maxLength);
      }
      getStorageMutable().reserve(std::min(static_cast<size_t>(paRequestedSize), maxLength));
    }

    void assign(const char *paData, const TForteUInt16 paLen) override {
      getStorageMutable().assign(paData, std::min(static_cast<size_t>(paLen), maxLength));
    }

    using CIEC_STRING::append;

    /*! Append arbitrary data (can contain '0x00')
     */
    void append(const char *paData, const TForteUInt16 paLen) override {
      const size_t currentLength = this->length();
      if (currentLength < maxLength) {
        const size_t remainingSpace = maxLength - currentLength;
        getStorageMutable().append(paData, std::min(static_cast<size_t>(paLen), remainingSpace));
      }
    }

    void append(const std::string &paValue) override {
      const size_t currentLength = length();
      if (currentLength < maxLength) {
        const size_t remainingSpace = maxLength - currentLength;
        getStorageMutable().append(paValue, 0, remainingSpace);
      }
    }
};