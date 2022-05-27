/*******************************************************************************
 * Copyright (c) 2005 - 2015 Profactor GmbH, ACIN
 *               2022 Primetals Technologies Austria GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Thomas Strasser, Ingomar Müller, Alois Zoitl, Gerhard Ebenhofer,
 *    Ingo Hegny, Martin Melik Merkumians
 *      - initial implementation and rework communication infrastructure
 *    Martin Melik Merkumians - make TForteInt8 constructor explicit,
 *      removed built-in type operator=
 *******************************************************************************/
#ifndef _FORTE_SINT_H_
#define _FORTE_SINT_H_

#include "forte_any_int.h"
#include <limits>

/*!\ingroup COREDTS CIEC_SINT represents the SINT data type according to IEC 61131.
 */
class CIEC_SINT : public CIEC_ANY_INT{
  DECLARE_FIRMWARE_DATATYPE(SINT)

  public:
    typedef TForteInt8 TValueType;

    static const TValueType scm_nMinVal;
    static const TValueType scm_nMaxVal;

    CIEC_SINT() = default;

    CIEC_SINT(const CIEC_SINT& paValue) :
        CIEC_ANY_INT(){
      setValueSimple(paValue);
    }

    explicit CIEC_SINT(TForteInt8 paValue){
      setTINT8(paValue);
    }

    virtual ~CIEC_SINT() = default;

    CIEC_SINT& operator =(const CIEC_SINT &paValue){
      // Simple value assignment - no self assignment check needed
      setValueSimple(paValue);
      return *this;
    }

    CIEC_SINT operator-() const {
      return CIEC_SINT(static_cast<TForteInt8>(-1 * *this));
    }

    /*! \brief Converts CIEC_SINT to elementary 8 bit integer
     *
     *   Conversion operator for converting CIEC_SINT to elementary 8 bit integer
     */
    operator TForteInt8() const{
      return getTINT8();
    }

    EDataTypeID getDataTypeID() const override {
      return CIEC_ANY::e_SINT;
    }
};

#endif /*_FORTE_SINT_H_*/
