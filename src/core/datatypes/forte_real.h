/*******************************************************************************
 * Copyright (c) 2005 - 2015 Pr3factor GmbH, ACIN, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Thomas Strasser, Ingomar Müller, Alois Zoitl, Gerhard Ebenhofer,
 *    Ingo Hegny, Martin Melik Merkumians, Monika Wenger
 *      - initial implementation and rework communication infrastructure
 *******************************************************************************/
#ifndef _FORTE_REAL_H_
#define _FORTE_REAL_H_

#include "forte_any_real.h"

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

    CIEC_REAL(const CIEC_REAL& paValue) :
        CIEC_ANY_REAL(){
      setValueSimple(paValue);
    }

    // We don't want this constructor to be explicit as it simplifies code generation for ST algorithms
    // Maybe when we have better code generators we want to make this constructor explicit again and generate it
    // cppcheck-suppress noExplicitConstructor
    CIEC_REAL(TForteFloat paValue){
      setTFLOAT(paValue);
    }

    virtual ~CIEC_REAL(){
    }

    /*! \brief Operator: CIEC_REAL data type = float data type
     *
     *   This command implements the assignment operator for the C++ data type FLOAT.
     *   The parameter value is represented by a C++ variable (call by value).
     *   \param paValue  Value for assignment.
     *   \return Can be the following response:
     *     - Pointer to given object.
     */
    CIEC_REAL& operator =(TForteFloat paValue){
      setTFLOAT(paValue);
      return *this;
    }
    ;

    CIEC_REAL& operator =(const CIEC_REAL &paValue){
      // Simple value assignment - no self assignment check needed
      setValueSimple(paValue);
      return *this;
    }

    /*! \brief Converts CIEC_REAL to elementary 32 bit float
     *
     *   Conversion operator for converting CIEC_REAL to elementary 32 bit float
     */
    operator TForteFloat() const{
      return getTFLOAT();
    }

    virtual void setValue(const CIEC_ANY& paValue);

    virtual EDataTypeID getDataTypeID() const{
      return CIEC_ANY::e_REAL;
    }

    /*! \brief Converts string value to data type value
     *
     *   This command implements a conversion function from IEC 61131
     *   data type (string format) to a C++ conform type.
     *   This function is necessary for communication with a proper engineering system.
     *   \param paValue string buffer
     *   \return number of bytes taken used from the buffer
     *        -1 on on error
     */
    virtual int fromString(const char *paValue);
    /*! \brief Converts data type value to string
     *
     *   This command implements a conversion function from C++ data type
     *   to IEC 61131 conform data type (string format).
     *   This function is necessary for communication with a proper engineering system.
     *   \param paValue Pointer to the buffer String
     *   \param paBufferSize size of the buffer thats available for this function
     *   \return number of bytes used in the buffer without trailing 0x00
     *           -1 on error
     */
    virtual int toString(char* paValue, size_t paBufferSize) const;

    /*! \brief Casts TForteFloat values to CIEC data types
     *  \param paSrcValue Source value for the cast
     *  \param paDestValue Destination CIEC object of the cast
     */
    static void castRealData(const CIEC_REAL &paSrcValue, CIEC_ANY &paDestValue);
};

#endif /*_FORTE_REAL_H_*/
