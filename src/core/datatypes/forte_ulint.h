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

#ifdef FORTE_USE_64BIT_DATATYPES

/*!\ingroup COREDTS CIEC_ULINT represents the ulint data type according to IEC 61131.
 *  @author mw & az
 */
class CIEC_ULINT : public CIEC_ANY_INT{
  DECLARE_FIRMWARE_DATATYPE(ULINT)

  public:
    typedef TForteUInt64 TValueType;

    static const TValueType scm_nMinVal = 0;
    static const TValueType scm_nMaxVal;

    CIEC_ULINT(){
    }

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

   explicit CIEC_ULINT(TForteUInt64 paValue){
      setTUINT64(paValue);
    }

    virtual ~CIEC_ULINT(){
    }

    CIEC_ULINT& operator =(const CIEC_ULINT &paValue) {
      // Simple value assignment - no self assignment check needed
      setValueSimple(paValue);
      return *this;
    }

    CIEC_ULINT& operator =(const CIEC_UDINT &paValue) {
      // Simple value assignment - no self assignment check needed
      setValueSimple(paValue);
      return *this;
    }

    CIEC_ULINT& operator =(const CIEC_UINT &paValue) {
      // Simple value assignment - no self assignment check needed
      setValueSimple(paValue);
      return *this;
    }

    CIEC_ULINT& operator =(const CIEC_USINT &paValue) {
      // Simple value assignment - no self assignment check needed
      setValueSimple(paValue);
      return *this;
    }

    /*! \brief Converts CIEC_ULINT to elementary unsigned 64 bit integer
     *
     *   Conversion operator for converting CIEC_ULINT to elementary unsigned 64 bit integer
     */
    operator TForteUInt64() const {
      return getTUINT64();
    }

    virtual EDataTypeID getDataTypeID() const {
      return CIEC_ANY::e_ULINT;
    }
};

#endif
#endif /*_FORTE_ULINT_H_*/
