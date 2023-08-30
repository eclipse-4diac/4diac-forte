/*******************************************************************************
 * Copyright (c) 2005, 2023 Profactor GmbH, ACIN, fortiss GmbH
 *                          Primetals Technologies Austria GmbH
 *                          Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Thomas Strasser, Ingomar Müller, Alois Zoitl, Gerhard Ebenhofer,
 *    Ingo Hegny, Monika Wenger,
 *      - initial implementation and rework communication infrastructure
 *    Martin Melik Merkumians - make TForteUInt8 constructor explicit,
 *        removed built-in type operator=
 *    Martin Jobst - add user-defined literal
 *******************************************************************************/
#ifndef _FORTE_USINT_H_
#define _FORTE_USINT_H_

#include "forte_any_unsigned.h"
#include <limits>

/*!\ingroup COREDTS CIEC_USINT represents the usint data type according to IEC 61131.
 */
class CIEC_USINT final : public CIEC_ANY_UNSIGNED {
  DECLARE_FIRMWARE_DATATYPE(USINT)

  public:
    typedef TForteUInt8 TValueType;

    static const TValueType scmMinVal = 0;
    static const TValueType scmMaxVal;

    CIEC_USINT() = default;

    CIEC_USINT(const CIEC_USINT& paValue) :
            CIEC_ANY_UNSIGNED() {
      setValueSimple(paValue);
    }

    explicit CIEC_USINT(const CIEC_ANY_INT& paValue) :
            CIEC_ANY_UNSIGNED() {
      setValueSimple(paValue);
    }

    explicit CIEC_USINT(const TValueType paValue) {
      setTUINT8(paValue);
    }

    ~CIEC_USINT() override = default;

    CIEC_USINT& operator =(const CIEC_USINT &paValue){
      setValueSimple(paValue);
      return *this;
    }

    template <typename T, std::enable_if_t<std::is_same_v<typename forte::core::mpl::implicit_cast_t<T, CIEC_USINT>, CIEC_USINT>, int> = 0>
    CIEC_USINT &operator=(const T &paValue) {
      setValueSimple(paValue);
      return *this;
    }

    /*! \brief Converts CIEC_UDINT to elementary unsigned 16 bit integer
     *
     *   Conversion operator for converting CIEC_UDINT to elementary unsigned 16 bit integer
     */
    explicit operator TForteUInt8() const{
      return getTUINT8();
    }

    /*! \brief calculates buffer size needed for toString conversion
     */
    size_t getToStringBufferSize() const override;

    EDataTypeID getDataTypeID() const override {
      return CIEC_ANY::e_USINT;
    }
};

inline CIEC_USINT operator ""_USINT(unsigned long long int paValue) {
  return CIEC_USINT(static_cast<CIEC_USINT::TValueType>(paValue));
}

#endif /*_FORTE_USINT_H_*/
