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

    static const TValueType scmMinVal = 0;
    static const TValueType scmMaxVal;

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
    size_t getToStringBufferSize() const override;

    EDataTypeID getDataTypeID() const override {
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

#endif /*_FORTE_CHAR_H_*/
