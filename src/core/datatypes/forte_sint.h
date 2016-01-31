/*******************************************************************************
  * Copyright (c) 2005 - 2015 Profactor GmbH, ACIN
  * All rights reserved. This program and the accompanying materials
  * are made available under the terms of the Eclipse Public License v1.0
  * which accompanies this distribution, and is available at
  * http://www.eclipse.org/legal/epl-v10.html
  *
  * Contributors:
  *    Thomas Strasser, Ingomar Müller, Alois Zoitl, Gerhard Ebenhofer,
  *    Ingo Hegny, Martin Melik Merkumians
  *      - initial implementation and rework communication infrastructure
  *******************************************************************************/
#ifndef _FORTE_SINT_H_
#define _FORTE_SINT_H_

#include "forte_any_int.h"
#include <limits>

/*!\ingroup COREDTS CIEC_SINT represents the sint data type according to IEC 61131.
 */
class CIEC_SINT : public CIEC_ANY_INT{
  DECLARE_FIRMWARE_DATATYPE(SINT)

  public:
    typedef TForteInt8 TValueType;

    static const TValueType scm_nMinVal;
    static const TValueType scm_nMaxVal;

    CIEC_SINT(){
    }

    CIEC_SINT(const CIEC_SINT& pa_roValue) :
        CIEC_ANY_INT(){
      setValueSimple(pa_roValue);
    }

    CIEC_SINT(TForteInt8 pa_nValue){
      setTINT8(pa_nValue);
    }

    virtual ~CIEC_SINT(){
    }

    /*! \brief Operator: CIEC_SINT data type = char data type
     *
     *   This command implements the assignment operator for the C++ datatype CHAR
     *   The parameter value is represented by a C++ variable (call by value).
     *   \param pa_nValue  Value for assignment.
     *   \return Can be the following response:
     *     - Pointer to given object.
     */
    CIEC_SINT& operator =(TForteInt8 pa_nValue){
      setTINT8(pa_nValue);
      return *this;
    }

    CIEC_SINT& operator =(const CIEC_SINT &pa_roValue){
      setValueSimple(pa_roValue);
      return *this;
    }

    /*! \brief Converts CIEC_SINT to elementary 8 bit integer
     *
     *   Conversion operator for converting CIEC_SINT to elementary 8 bit integer
     */
    operator TForteInt8() const{
      return getTINT8();
    }

    virtual EDataTypeID getDataTypeID() const{
      return CIEC_ANY::e_SINT;
    }

    static const TForteUInt16 scm_unMaxStringBufSize;
};

#endif /*_FORTE_SINT_H_*/
