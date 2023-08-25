/*******************************************************************************
 * Copyright (c) 2022, 2023 Primetals Technologies Austria GmbH
 *                          Martin Erich Jobst
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
 *    Martin Jobst - add user-defined literal
 *******************************************************************************/
#ifndef _FORTE_WCHAR_H_
#define _FORTE_WCHAR_H_

#include "forte_any_char.h"

/*!\ingroup COREDTS CIEC_BYTE represents the byte data type according to IEC 61131.
 */
class CIEC_WCHAR : public CIEC_ANY_CHAR{
  DECLARE_FIRMWARE_DATATYPE(WCHAR)

  public:
    typedef TForteWChar TValueType;

    static const TValueType scmMinVal = 0;
    static const TValueType scmMaxVal;

    CIEC_WCHAR() = default;

    CIEC_WCHAR(const CIEC_WCHAR& paValue) :
        CIEC_ANY_CHAR(){
      setValueSimple(paValue);
    }

    explicit CIEC_WCHAR(const TValueType paValue) {
      setChar16(paValue);
    }

    ~CIEC_WCHAR() override = default;

    CIEC_WCHAR& operator =(const CIEC_WCHAR &paValue){
      // Simple value assignment - no self assignment check needed
      setValueSimple(paValue);
      return *this;
    }

    /*! \brief Converts CIEC_WCHAR to elementary byte
     *
     *   Conversion operator for converting CIEC_WCHAR to elementary byte
     */
    explicit operator TForteWChar() const {
      return getChar16();
    }

    int toString(char *paValue, size_t paBufferSize) const override;

    int fromString(const char *paValue) override;

    /*! \brief calculates buffer size needed for toString conversion
     */
    size_t getToStringBufferSize() const override;

    EDataTypeID getDataTypeID() const override {
      return CIEC_ANY::e_WCHAR;
    }
};

inline CIEC_WCHAR operator ""_WCHAR(char16_t paValue) {
  return CIEC_WCHAR(static_cast<CIEC_WCHAR::TValueType>(paValue));
}

inline CIEC_WCHAR operator ""_WCHAR(unsigned long long int paValue) {
  return CIEC_WCHAR(static_cast<CIEC_WCHAR::TValueType>(paValue));
}

#endif /*_FORTE_WCHAR_H_*/
