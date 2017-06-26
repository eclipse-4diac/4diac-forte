/*******************************************************************************
  * Copyright (c) 2005 - 2015 Pr3factor GmbH, ACIN, fortiss GmbH
  * All rights reserved. This program and the accompanying materials
  * are made available under the terms of the Eclipse Public License v1.0
  * which accompanies this distribution, and is available at
  * http://www.eclipse.org/legal/epl-v10.html
  *
  * Contributors:
  *    Thomas Strasser, Ingomar Müller, Alois Zoitl, Gerhard Ebenhofer,
  *    Ingo Hegny, Martin Melik Merkumians, Monika Wenger
  *      - initial implementation and rework communication infrastructure
  *******************************************************************************/
#ifndef _FORTE_REAL_H_
#define _FORTE_REAL_H_

#include "forte_any_real.h"

#ifdef FORTE_USE_REAL_DATATYPE

#include "forte_string.h"
#include "forte_wstring.h"

/*!\ingroup COREDTS CIEC_REAL represents the real data type according to IEC 61131.
 */
class CIEC_REAL : public CIEC_ANY_REAL{
  DECLARE_FIRMWARE_DATATYPE(REAL)

  public:
    typedef TForteFloat TValueType;

    CIEC_REAL(){
    }

    CIEC_REAL(const CIEC_REAL& pa_roValue) :
        CIEC_ANY_REAL(){
      setValueSimple(pa_roValue);
    }

    CIEC_REAL(TForteFloat pa_fValue){
      setTFLOAT(pa_fValue);
    }

    virtual ~CIEC_REAL(){
    }

    /*! \brief Operator: CIEC_REAL data type = float data type
     *
     *   This command implements the assignment operator for the C++ datatype FLOAT.
     *   The parameter value is represented by a C++ variable (call by value).
     *   \param pa_fValue  Value for assignment.
     *   \return Can be the following response:
     *     - Pointer to given object.
     */
    CIEC_REAL& operator =(TForteFloat pa_fValue){
      setTFLOAT(pa_fValue);
      return *this;
    }
    ;

    CIEC_REAL& operator =(const CIEC_REAL &pa_roValue){
      setValueSimple(pa_roValue);
      return *this;
    }

    /*! \brief Converts CIEC_REAL to elementary 32 bit float
     *
     *   Conversion operator for converting CIEC_REAL to elementary 32 bit float
     */
    operator TForteFloat() const{
      return getTFLOAT();
    }

    virtual void setValue(const CIEC_ANY& pa_roValue);

    virtual EDataTypeID getDataTypeID() const{
      return CIEC_ANY::e_REAL;
    }

    /*! \brief Converts string value to data type value
     *
     *   This command implements a conversion function from IEC61131
     *   data type (string format) to a C++ conform type.
     *   This function is necessary for communication with a proper engineering system.
     *   \param pa_pacValue string buffer
     *   \return number of bytes taken used from the buffer
     *        -1 on on error
     */
    virtual int fromString(const char *pa_pacValue);
    /*! \brief Converts data type value to string
     *
     *   This command implements a conversion function from C++ data type
     *   to IEC61131 conform data type (string format).
     *   This function is necessary for communication with a proper engineering system.
     *   \param pa_pacValue Pointer to the buffer String
     *   \param pa_nBufferSize size of the buffer thats available for this function
     *   \return number of bytes used in the buffer without trailing 0x00
     *           -1 on error
     */
    virtual int toString(char* pa_pacValue, unsigned int pa_nBufferSize) const;

    /*! \brief Casts TForteFloat values to CIEC data types
     *  \param pa_roSrcValue Source value for the cast
     *  \param pa_roDestValue Destination CIEC object of the cast
     */
    static void castRealData(const CIEC_REAL &pa_roSrcValue, CIEC_ANY &pa_roDestValue);
};

#endif /* #ifdef FORTE_USE_REAL_DATATYPE */

#endif /*_FORTE_REAL_H_*/
