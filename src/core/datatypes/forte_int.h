/*******************************************************************************
 * Copyright (c) 2005, 2023 Profactor GmbH, ACIN
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
 *    Thomas Strasser, Ingomar Müller, Alois Zoitl, Gerhard Ebenhofer
 *    Ingo Hegny, Monika Wenger
 *      - initial implementation and rework communication infrastructure
 *    Martin Melik Merkumians - make TForteInt16 constructor explicit,
 *        added valid cast constructors, removed built-in type operator=, added
 *        castable CIEC types operator=
 *    Martin Jobst - add user-defined literal
 *                 - add ANY_SIGNED
 *******************************************************************************/
#ifndef _FORTE_INT_H_
#define _FORTE_INT_H_

#include "iec61131_cast_helper.h"
#include "forte_any_signed.h"
#include "forte_sint.h"
#include "forte_usint.h"
#include <limits>

/*!\ingroup COREDTS CIEC_INT represents the int data type according to IEC 61131.
 */
class CIEC_INT final : public CIEC_ANY_SIGNED {
  DECLARE_FIRMWARE_DATATYPE(INT)

  public:
    typedef TForteInt16 TValueType;

    static const TValueType scmMinVal;
    static const TValueType scmMaxVal;

    CIEC_INT() = default;

    CIEC_INT(const CIEC_INT& paValue) :
            CIEC_ANY_SIGNED() {
      setValueSimple(paValue);
    }

    CIEC_INT(const CIEC_SINT& paValue) :
            CIEC_ANY_SIGNED() {
      setValueSimple(paValue);
    }

    CIEC_INT(const CIEC_USINT& paValue) :
            CIEC_ANY_SIGNED() {
      setValueSimple(paValue);
    }

    explicit CIEC_INT(const CIEC_ANY_INT& paValue) :
            CIEC_ANY_SIGNED() {
      setValueSimple(paValue);
    }

    explicit CIEC_INT(const TValueType paValue) {
      setTINT16(paValue);
    }

    ~CIEC_INT() override = default;

    CIEC_INT& operator =(const CIEC_INT &paValue){
      // Simple value assignment - no self assignment check needed
      setValueSimple(paValue);
      return *this;
    }

    template <typename T, std::enable_if_t<std::is_same_v<typename forte::core::mpl::implicit_cast_t<T, CIEC_INT>, CIEC_INT>, int> = 0>
    CIEC_INT &operator=(const T &paValue) {
      setValueSimple(paValue);
      return *this;
    }

    CIEC_INT operator-() const {
      return CIEC_INT(static_cast<CIEC_INT::TValueType>(-1 * static_cast<CIEC_INT::TValueType>(*this)));
    }

    /*! \brief Converts CIEC_INT to elementary 16 bit integer
     *
     *   Conversion operator for converting CIEC_INT to elementary 16 bit integer
     */
    explicit operator TForteInt16() const {
      return getTINT16();
    }

    /*! \brief calculates buffer size needed for toString conversion
     */
    size_t getToStringBufferSize() const override;

    EDataTypeID getDataTypeID() const override {
      return CIEC_ANY::e_INT;
    }
};

inline CIEC_INT operator ""_INT(unsigned long long int paValue) {
  return CIEC_INT(static_cast<CIEC_INT::TValueType>(paValue));
}

#endif /*_FORTE_INT_H_*/
