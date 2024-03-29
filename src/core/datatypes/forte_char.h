/*******************************************************************************
 * Copyright (c) 2022 Primetals Technologies Austria GmbH
 *               
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Martin Melik-Merkumians
 *      - initial implementation and rework communication infrastructure
 *******************************************************************************/
#ifndef _FORTE_CHAR_H_
#define _FORTE_CHAR_H_

#include "forte_any_char.h"

/*!\ingroup COREDTS CIEC_BYTE represents the byte data type according to IEC 61131.
 */
class CIEC_CHAR : public CIEC_ANY_CHAR{
  DECLARE_FIRMWARE_DATATYPE(CHAR)

  public:
    using TValueType = TForteChar;
    [[deprecated("Please use the corresponding numeric_limits template")]]
    constexpr static size_t scmBitLength = 8U;

    [[deprecated("Please use the corresponding numeric_limits template")]]
    static constexpr TValueType scmMinVal = std::numeric_limits<TValueType>::min();
    [[deprecated("Please use the corresponding numeric_limits template")]]
    static constexpr TValueType scmMaxVal = std::numeric_limits<TValueType>::max();

    CIEC_CHAR() = default;

    CIEC_CHAR(const CIEC_CHAR& paValue) :
        CIEC_ANY_CHAR(){
      setValueSimple(paValue);
    }

    explicit CIEC_CHAR(const TValueType paValue) {
      setChar(paValue);
    }

    ~CIEC_CHAR() override = default;

    CIEC_CHAR& operator =(const CIEC_CHAR &paValue){
      // Simple value assignment - no self assignment check needed
      setValueSimple(paValue);
      return *this;
    }

    /*! \brief Converts CIEC_CHAR to elementary byte
     *
     *   Conversion operator for converting CIEC_CHAR to elementary byte
     */
    explicit operator TForteChar() const {
      return getChar8();
    }

    int toString(char* paValue, size_t paBufferSize) const override;

    int fromString(const char *paValue) override;

    /*! \brief calculates buffer size needed for toString conversion
     */
    size_t getToStringBufferSize() const override final {
      return sizeof("CHAR#'$00'");
    }

    EDataTypeID getDataTypeID() const override final {
      return CIEC_ANY::e_CHAR;
    }
};

inline bool operator==(const CIEC_CHAR &lhs, const CIEC_CHAR &rhs) {
  return static_cast<CIEC_CHAR::TValueType>(lhs) == static_cast<CIEC_CHAR::TValueType>(rhs);
}

inline CIEC_CHAR operator ""_CHAR(char paValue) {
  return CIEC_CHAR(static_cast<CIEC_CHAR::TValueType>(paValue));
}

inline CIEC_CHAR operator ""_CHAR(unsigned long long int paValue) {
  return CIEC_CHAR(static_cast<CIEC_CHAR::TValueType>(paValue));
}

namespace std {
  template <>
  struct numeric_limits<CIEC_CHAR> : public forte::templates::numeric_limits<CIEC_CHAR> {
    static constexpr size_t bitLength = 8U;
  };
}

#endif /*_FORTE_CHAR_H_*/
