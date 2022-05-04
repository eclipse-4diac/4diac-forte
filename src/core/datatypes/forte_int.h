/*******************************************************************************
 * Copyright (c) 2005 - 2013 Profactor GmbH, ACIN
 *               2022 Primetals Technologies Austria GmbH
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
 *******************************************************************************/
#ifndef _FORTE_INT_H_
#define _FORTE_INT_H_

#include "forte_any_int.h"
#include "forte_sint.h"
#include "forte_usint.h"
#include <limits>

/*!\ingroup COREDTS CIEC_INT represents the int data type according to IEC 61131.
 */
class CIEC_INT : public CIEC_ANY_INT{
  DECLARE_FIRMWARE_DATATYPE(INT)

  public:
    typedef TForteInt16 TValueType;

    static const TValueType scm_nMinVal;
    static const TValueType scm_nMaxVal;

    CIEC_INT() = default;

    CIEC_INT(const CIEC_INT& paValue) :
        CIEC_ANY_INT() {
      setValueSimple(paValue);
    }

    CIEC_INT(const CIEC_SINT& paValue) :
        CIEC_ANY_INT() {
      setValueSimple(paValue);
    }

    CIEC_INT(const CIEC_USINT& paValue) :
        CIEC_ANY_INT() {
      setValueSimple(paValue);
    }

    explicit CIEC_INT(TForteInt16 paValue){
      setTINT16(paValue);
    }

    virtual ~CIEC_INT() = default;

    CIEC_INT& operator =(const CIEC_INT &paValue){
      // Simple value assignment - no self assignment check needed
      setValueSimple(paValue);
      return *this;
    }

    CIEC_INT& operator =(const CIEC_SINT &paValue){
      // Simple value assignment - no self assignment check needed
      setValueSimple(paValue);
      return *this;
    }

    CIEC_INT& operator =(const CIEC_USINT &paValue){
      // Simple value assignment - no self assignment check needed
      setValueSimple(paValue);
      return *this;
    }

    CIEC_INT operator-() const {
      return CIEC_INT(-1 * *this);
    }

    /*! \brief Converts CIEC_INT to elementary 16 bit integer
     *
     *   Conversion operator for converting CIEC_INT to elementary 16 bit integer
     */
    operator TForteInt16() const {
      return getTINT16();
    }

    EDataTypeID getDataTypeID() const override {
      return CIEC_ANY::e_INT;
    }
};

#endif /*_FORTE_INT_H_*/
