/*******************************************************************************
 * Copyright (c) 2009 - 2013 ACIN
 *               2022 Primetals Technologies Austria GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Monika Wenger, Alois Zoitl,
 *      - initial implementation and rework communication infrastructure
 *    Martin Melik Merkumians - make TForteInt64 constructor explicit,
 *      adds casts constructors, removed built-in type operator=, added
 *      castable CIEC types operator=
  *******************************************************************************/
#ifndef _FORTE_LINT_H_
#define _FORTE_LINT_H_

#include "forte_any_int.h"
#include "forte_dint.h"
#include "forte_int.h"
#include "forte_sint.h"
#include "forte_udint.h"
#include "forte_uint.h"
#include "forte_usint.h"
#include <limits>

/*!\ingroup COREDTS CIEC_LINT represents the dint data type according to IEC 61131.
 */
class CIEC_LINT : public CIEC_ANY_INT{
  DECLARE_FIRMWARE_DATATYPE(LINT)

  public:
    typedef TForteInt64 TValueType;

    static const TValueType scm_nMinVal;
    static const TValueType scm_nMaxVal;

    CIEC_LINT() = default;

    CIEC_LINT(const CIEC_LINT& paValue) :
        CIEC_ANY_INT() {
      setValueSimple(paValue);
    }

    CIEC_LINT(const CIEC_DINT& paValue) :
        CIEC_ANY_INT() {
      setValueSimple(paValue);
    }

    CIEC_LINT(const CIEC_UDINT& paValue) :
        CIEC_ANY_INT() {
      setValueSimple(paValue);
    }

    CIEC_LINT(const CIEC_INT& paValue) :
        CIEC_ANY_INT() {
      setValueSimple(paValue);
    }

    CIEC_LINT(const CIEC_UINT& paValue) :
        CIEC_ANY_INT() {
      setValueSimple(paValue);
    }

    CIEC_LINT(const CIEC_SINT& paValue) :
        CIEC_ANY_INT() {
      setValueSimple(paValue);
    }

    CIEC_LINT(const CIEC_USINT& paValue) :
        CIEC_ANY_INT() {
      setValueSimple(paValue);
    }

    explicit CIEC_LINT(TForteInt64 paValue){
      setTINT64(paValue);
    }

    ~CIEC_LINT() override = default;

    CIEC_LINT& operator =(const CIEC_LINT &paValue){
      // Simple value assignment - no self assignment check needed
      setValueSimple(paValue);
      return *this;
    }

    CIEC_LINT& operator =(const CIEC_DINT &paValue){
      // Simple value assignment - no self assignment check needed
      setValueSimple(paValue);
      return *this;
    }

    CIEC_LINT& operator =(const CIEC_UDINT &paValue){
      // Simple value assignment - no self assignment check needed
      setValueSimple(paValue);
      return *this;
    }

    CIEC_LINT& operator =(const CIEC_INT &paValue){
      // Simple value assignment - no self assignment check needed
      setValueSimple(paValue);
      return *this;
    }

    CIEC_LINT& operator =(const CIEC_UINT &paValue){
      // Simple value assignment - no self assignment check needed
      setValueSimple(paValue);
      return *this;
    }

    CIEC_LINT& operator =(const CIEC_SINT &paValue){
      // Simple value assignment - no self assignment check needed
      setValueSimple(paValue);
      return *this;
    }

    CIEC_LINT& operator =(const CIEC_USINT &paValue){
      // Simple value assignment - no self assignment check needed
      setValueSimple(paValue);
      return *this;
    }

    CIEC_LINT operator-() const {
      return CIEC_LINT(-1 * *this);
    }

    /*! \brief Converts CIEC_LINT to elementary byte
     *
     *   Conversion operator for converting CIEC_LINT to elementary 64 bit integer
     */
    operator TForteInt64() const{
      return getTINT64();
    }

    EDataTypeID getDataTypeID() const override {
      return CIEC_ANY::e_LINT;
    }
};

#endif /*_FORTE_LINT_H_*/
