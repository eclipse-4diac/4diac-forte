/*******************************************************************************
  * Copyright (c) 2005 - 2013 Profactor GmbH, ACIN
  * All rights reserved. This program and the accompanying materials
  * are made available under the terms of the Eclipse Public License v1.0
  * which accompanies this distribution, and is available at
  * http://www.eclipse.org/legal/epl-v10.html
  *
  * Contributors:
  *    Thomas Strasser, Ingomar Müller, Alois Zoitl, Gerhard Ebenhofer
  *    Ingo Hegny, Monika Wenger
  *      - initial implementation and rework communication infrastructure
  *******************************************************************************/
#ifndef _FORTE_DWORD_H_
#define _FORTE_DWORD_H_

#include "forte_any_bit.h"
#include <limits>

/*!\ingroup COREDTS CIEC_DWORD represents the dword data type according to IEC 61131.
 */
class CIEC_DWORD : public CIEC_ANY_BIT{
  DECLARE_FIRMWARE_DATATYPE(DWORD)

  public:
    typedef TForteDWord TValueType;

    static const TValueType scm_nMinVal = 0;
    static const TValueType scm_nMaxVal;

    CIEC_DWORD(){
    }

    CIEC_DWORD(const CIEC_DWORD& pa_roValue) : CIEC_ANY_BIT(){
      setValueSimple(pa_roValue);
    }

    CIEC_DWORD(TForteDWord pa_cValue){
      setTUINT32(pa_cValue);
    }

    virtual ~CIEC_DWORD(){
    }

    /*! \brief Operator: CIEC_DWORD data type = long data type
     *
     *   This command implements the assignment operator for the C++ datatype UNSIGNED LONG
     *   The parameter value is represented by a C++ variable (call by value).
     *   \param pa_cValue  Value for assignment.
     *   \return Can be the following response:
     *     - Pointer to given object.
     */
    CIEC_DWORD& operator =(TForteDWord pa_cValue){
      setTUINT32(pa_cValue);
      return *this;
    }

    CIEC_DWORD& operator =(const CIEC_DWORD &pa_roValue){
      setValueSimple(pa_roValue);
      return *this;
    }

    /*! \brief Converts CIEC_DWORD to elementary word
     *
     *   Conversion operator for converting CIEC_DWORD to elementary word
     */
    operator TForteDWord() const{
      return getTUINT32();
    }

    virtual EDataTypeID getDataTypeID() const{
      return CIEC_ANY::e_DWORD;
    }

    static const TForteUInt16 scm_unMaxStringBufSize;
};

#endif /*_FORTE_DWORD_H_*/
