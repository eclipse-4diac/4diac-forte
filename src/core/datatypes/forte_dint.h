/*******************************************************************************
 * Copyright (c) 2005 - 2013 Profactor GmbH, ACIN, fortiss GmbH
 *               2022 Primetals Technologies Austria GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Thomas Strasser, Ingomar Müller, Alois Zoitl, Gerhard Ebenhofer,
 *    Ingo Hegny, Monika Wenger
 *      - initial implementation and rework communication infrastructure
 *    Martin Melik Merkumians - make TForteInt8 constructor explicit, add
 *      implicit cast constructors, removed built-in type operator=, added
 *        castable CIEC types operator=
 *******************************************************************************/
#ifndef _FORTE_DINT_H_
#define _FORTE_DINT_H_

#include "forte_any_int.h"
#include "forte_sint.h"
#include "forte_int.h"
#include "forte_usint.h"
#include "forte_uint.h"
#include <limits>

/*!\ingroup COREDTS CIEC_DINT represents the dint data type according to IEC 61131.
 */
class CIEC_DINT : public CIEC_ANY_INT{
  DECLARE_FIRMWARE_DATATYPE(DINT)

  public:
    typedef TForteInt32 TValueType;

    static const TValueType scm_nMinVal;
    static const TValueType scm_nMaxVal;

    CIEC_DINT(){
    }

    CIEC_DINT(const CIEC_DINT& paValue) :
        CIEC_ANY_INT(){
      setValueSimple(paValue);
    }

    CIEC_DINT(const CIEC_INT& paValue) :
        CIEC_ANY_INT(){
      setValueSimple(paValue);
    }

    CIEC_DINT(const CIEC_UINT& paValue) :
        CIEC_ANY_INT(){
      setValueSimple(paValue);
    }

    CIEC_DINT(const CIEC_SINT& paValue) :
        CIEC_ANY_INT(){
      setValueSimple(paValue);
    }

    CIEC_DINT(const CIEC_USINT& paValue) :
        CIEC_ANY_INT(){
      setValueSimple(paValue);
    }

    explicit CIEC_DINT(TForteInt32 paValue){
      setTINT32(paValue);
    }

    virtual ~CIEC_DINT(){
    }

    CIEC_DINT& operator =(const CIEC_DINT &paValue){
      // Simple value assignment - no self assignment check needed
      setValueSimple(paValue);
      return *this;
    }

    CIEC_DINT& operator =(const CIEC_INT &paValue){
      // Simple value assignment - no self assignment check needed
      setValueSimple(paValue);
      return *this;
    }
    

    CIEC_DINT& operator =(const CIEC_UINT &paValue){
      // Simple value assignment - no self assignment check needed
      setValueSimple(paValue);
      return *this;
    }
    
    CIEC_DINT& operator =(const CIEC_SINT &paValue){
      // Simple value assignment - no self assignment check needed
      setValueSimple(paValue);
      return *this;
    }
    

    CIEC_DINT& operator =(const CIEC_USINT &paValue){
      // Simple value assignment - no self assignment check needed
      setValueSimple(paValue);
      return *this;
    }
    

    /*! \brief Converts CIEC_DINT to elementary byte
     *
     *   Conversion operator for converting CIEC_DINT to elementary 32 bit integer
     */
    operator TForteInt32() const{
      return getTINT32();
    }

    virtual EDataTypeID getDataTypeID() const{
      return CIEC_ANY::e_DINT;
    }
};

#endif /*_FORTE_DINT_H_*/
