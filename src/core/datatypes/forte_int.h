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
#ifndef _FORTE_INT_H_
#define _FORTE_INT_H_

#include "forte_any_int.h"
#include <limits>

/*!\ingroup COREDTS CIEC_INT represents the int data type according to IEC 61131.
 */
class CIEC_INT : public CIEC_ANY_INT{
  DECLARE_FIRMWARE_DATATYPE(INT)

  public:
    typedef TForteInt16 TValueType;

    static const TValueType scm_nMinVal;
    static const TValueType scm_nMaxVal;

    CIEC_INT(){
    }

    CIEC_INT(const CIEC_INT& pa_roValue) :
        CIEC_ANY_INT(){
      setValueSimple(pa_roValue);
    }

    // We don't want this constructor to be explicit as it simplifies code generation for ST algorithms
    // Maybe when we have better code generators we want to make this constructur explicit again and generate it
    // cppcheck-suppress noExplicitConstructor
    CIEC_INT(TForteInt16 pa_nValue){
      setTINT16(pa_nValue);
    }

    virtual ~CIEC_INT(){
    }

    /*! \brief Operator: CIEC_INT data type = short data type
     *
     *   This command implements the assignment operator for the C++ datatype SHORT
     *   The parameter value is represented by a C++ variable (call by value).
     *   \param pa_nValue  Value for assignment.
     *   \return Can be the following response:
     *     - Pointer to given object.
     */
    CIEC_INT& operator =(TForteInt16 pa_nValue){
      setTINT16(pa_nValue);
      return *this;
    }

    CIEC_INT& operator =(const CIEC_INT &pa_roValue){
      setValueSimple(pa_roValue);
      return *this;
    }

    /*! \brief Converts CIEC_INT to elementary 16 bit integer
     *
     *   Conversion operator for converting CIEC_INT to elementary 16 bit integer
     */
    operator TForteInt16() const{
      return getTINT16();
    }

    virtual EDataTypeID getDataTypeID() const{
      return CIEC_ANY::e_INT;
    }
};

#endif /*_FORTE_INT_H_*/
