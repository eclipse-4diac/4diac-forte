/*******************************************************************************
 * Copyright (c) 2009 - 2013 ACIN, fortiss GmbH
 *               2022 Primetals Technologies Austria GmbH
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
 *******************************************************************************/
#ifndef _FORTE_ULINT_H_
#define _FORTE_ULINT_H_

#include "forte_any_int.h"
#include "forte_udint.h"
#include "forte_uint.h"
#include "forte_usint.h"
#include <limits>


/*!\ingroup COREDTS CIEC_ULINT represents the ulint data type according to IEC 61131.
 *  @author mw & az
 */
class CIEC_ULINT : public CIEC_ANY_INT{
  DECLARE_FIRMWARE_DATATYPE(ULINT)

  public:
    typedef TForteUInt64 TValueType;

    static const TValueType scm_nMinVal = 0;
    static const TValueType scm_nMaxVal;

    CIEC_ULINT() = default;

    CIEC_ULINT(const CIEC_ULINT& paValue) : CIEC_ANY_INT() {
      setValueSimple(paValue);
    }

    CIEC_ULINT(const CIEC_UDINT& paValue) : CIEC_ANY_INT() {
      setValueSimple(paValue);
    }

    CIEC_ULINT(const CIEC_UINT& paValue) : CIEC_ANY_INT() {
      setValueSimple(paValue);
    }

    CIEC_ULINT(const CIEC_USINT& paValue) : CIEC_ANY_INT() {
      setValueSimple(paValue);
    }

    explicit CIEC_ULINT(const CIEC_ANY_INT& paValue) :
        CIEC_ANY_INT() {
      setValueSimple(paValue);
    }

   explicit CIEC_ULINT(TForteUInt64 paValue){
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

    EDataTypeID getDataTypeID() const override {
      return CIEC_ANY::e_ULINT;
    }
};

#endif /*_FORTE_ULINT_H_*/
