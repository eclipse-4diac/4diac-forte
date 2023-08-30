/*******************************************************************************
 * Copyright (c) 2009, 2023 ACIN, fortiss GmbH
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
 *    Monika Wenger, Alois Zoitl
 *      - initial implementation and rework communication infrastructure
 *    Martin Melik Merkumians - make TForteUInt64 constructor explicit,
 *        removed built-in type operator=, added castable CIEC types operator=
 *    Martin Jobst - add user-defined literal
 *******************************************************************************/
#ifndef _FORTE_ULINT_H_
#define _FORTE_ULINT_H_

#include "forte_any_unsigned.h"
#include "forte_udint.h"
#include "forte_uint.h"
#include "forte_usint.h"
#include <limits>


/*!\ingroup COREDTS CIEC_ULINT represents the ulint data type according to IEC 61131.
 *  @author mw & az
 */
class CIEC_ULINT final : public CIEC_ANY_UNSIGNED {
  DECLARE_FIRMWARE_DATATYPE(ULINT)

  public:
    typedef TForteUInt64 TValueType;

    static const TValueType scmMinVal = 0;
    static const TValueType scmMaxVal;

    CIEC_ULINT() = default;

    CIEC_ULINT(const CIEC_ULINT& paValue) : CIEC_ANY_UNSIGNED() {
      setValueSimple(paValue);
    }

    CIEC_ULINT(const CIEC_UDINT& paValue) : CIEC_ANY_UNSIGNED() {
      setValueSimple(paValue);
    }

    CIEC_ULINT(const CIEC_UINT& paValue) : CIEC_ANY_UNSIGNED() {
      setValueSimple(paValue);
    }

    CIEC_ULINT(const CIEC_USINT& paValue) : CIEC_ANY_UNSIGNED() {
      setValueSimple(paValue);
    }

    explicit CIEC_ULINT(const CIEC_ANY_INT& paValue) :
            CIEC_ANY_UNSIGNED() {
      setValueSimple(paValue);
    }

   explicit CIEC_ULINT(const TValueType paValue) {
      setTUINT64(paValue);
    }

    ~CIEC_ULINT() override = default;

    CIEC_ULINT& operator =(const CIEC_ULINT &paValue) {
      // Simple value assignment - no self assignment check needed
      setValueSimple(paValue);
      return *this;
    }

    template <typename T, std::enable_if_t<std::is_same_v<typename forte::core::mpl::implicit_cast_t<T, CIEC_ULINT>, CIEC_ULINT>, int> = 0>
    CIEC_ULINT &operator=(const T &paValue) {
      setValueSimple(paValue);
      return *this;
    }

    /*! \brief Converts CIEC_ULINT to elementary unsigned 64 bit integer
     *
     *   Conversion operator for converting CIEC_ULINT to elementary unsigned 64 bit integer
     */
    explicit operator TForteUInt64() const {
      return getTUINT64();
    }

    /*! \brief calculates buffer size needed for toString conversion
     */
    size_t getToStringBufferSize() const override;

    EDataTypeID getDataTypeID() const override {
      return CIEC_ANY::e_ULINT;
    }
};

inline CIEC_ULINT operator ""_ULINT(unsigned long long int paValue) {
  return CIEC_ULINT(static_cast<CIEC_ULINT::TValueType>(paValue));
}

#endif /*_FORTE_ULINT_H_*/
