/*******************************************************************************
  * Copyright (c) 2005 - 2013 Profactor GmbH, ACIN
  * All rights reserved. This program and the accompanying materials
  * are made available under the terms of the Eclipse Public License v1.0
  * which accompanies this distribution, and is available at
  * http://www.eclipse.org/legal/epl-v10.html
  *
  * Contributors:
  *    Thomas Strasser, Ingomar Müller, Alois Zoitl,
  *    Ingo Hegny, Monika Wenger, Martin Melik Merkumians
  *      - initial implementation and rework communication infrastructure
  *******************************************************************************/
#ifndef _FORTE_LREAL_H_
#define _FORTE_LREAL_H_

#include "forte_any_real.h"

/*!\ingroup COREDTS CIEC_LREAL represents the lreal data type according to IEC 61131.
 */
class CIEC_LREAL : public CIEC_ANY_REAL{
  DECLARE_FIRMWARE_DATATYPE(LREAL)

  public:
    typedef TForteDFloat TValueType;

    CIEC_LREAL(){
    }

    CIEC_LREAL(const CIEC_LREAL& pa_roValue) :
        CIEC_ANY_REAL(){
      setValueSimple(pa_roValue);
    }

    // We don't want this constructor to be explicit as it simplifies code generation for ST algorithms
    // Maybe when we have better code generators we want to make this constructur explicit again and generate it
    // cppcheck-suppress noExplicitConstructor
    CIEC_LREAL(TForteDFloat pa_fValue){
      setTDFLOAT(pa_fValue);
    }

    virtual ~CIEC_LREAL(){
    }

    /*! \brief Operator: CIEC_LREAL data type = CIEC_LREAL data type
     *
     *   This command implements the assignment operator for the C++ datatype DFLOAT.
     *   The parameter value is represented by a C++ variable (call by value).
     *   \param pa_fValue  Value for assignment.
     *   \return Can be the following response:
     *     - Pointer to given object.
     */
    CIEC_LREAL& operator =(TForteDFloat pa_fValue){
      setTDFLOAT(pa_fValue);
      return *this;
    }

    CIEC_LREAL& operator =(const CIEC_LREAL &pa_roValue){
      setValueSimple(pa_roValue);
      return *this;
    }

    /*! \brief Converts CIEC_LREAL to elementary 64 bit float
     *
     *   Conversion operator for converting CIEC_LREAL to elementary 64 bit float
     */

    operator TForteDFloat() const{
      return getTDFLOAT();
    }

    virtual EDataTypeID getDataTypeID() const{
      return CIEC_ANY::e_LREAL;
    }

    virtual void setValue(const CIEC_ANY& pa_roValue);

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

    /*! \brief Casts TForteDFloat values to CIEC data types
     *  \param pa_roSrcValue Source value for the cast
     *  \param pa_roDestValue Destination CIEC object of the cast
     */
    static void castLRealData(const CIEC_LREAL &pa_roSrcValue, CIEC_ANY &pa_roDestValue);

  protected:

  private:
    /*! \brief bit distortion extension for serialization function - 64 bit values.
     *   \param pa_pcBytes  Pointer to first part of original object.
     *   \param pa_nValue   Pointer to the modified object
     *   \return Can be the following response:
     *     - No return value.
     */

    //TODO: Can we delete this function?
    /*static void serializedouble(TForteByte* pa_pcBytes, TForteUInt64 pa_nValue){
      pa_pcBytes[0] = (TForteByte) ((pa_nValue >> 56) & 0x00FF);
      pa_pcBytes[1] = (TForteByte) ((pa_nValue >> 48) & 0x00FF);
      pa_pcBytes[2] = (TForteByte) ((pa_nValue >> 40) & 0x00FF);
      pa_pcBytes[3] = (TForteByte) ((pa_nValue >> 32) & 0x00FF);
      pa_pcBytes[4] = (TForteByte) ((pa_nValue >> 24) & 0x00FF);
      pa_pcBytes[5] = (TForteByte) ((pa_nValue >> 16) & 0x00FF);
      pa_pcBytes[6] = (TForteByte) ((pa_nValue >> 8) & 0x00FF);
      pa_pcBytes[7] = (TForteByte) (pa_nValue & 0x00FF);
    }*/
    ;
    /*! \brief bit distortion extension for deserialization function - 64 bit values.
     *   \param pa_pcBytes  Pointer to first part of final object.
     *   \param pa_nValue   Pointer to the modified object
     *   \return Can be the following response:
     *     - No return value.
     */
    //TODO: Can we delete this function?
    /*static void deserializedouble(const TForteByte* pa_pcBytes, TForteUInt64* pa_nValue){
      *pa_nValue = ((((TForteUInt64) pa_pcBytes[0]) << 56) & 0xFF00000000000000ULL) + ((((TForteUInt64) pa_pcBytes[1]) << 48) & 0x00FF000000000000ULL) + ((((TForteUInt64) pa_pcBytes[2]) << 40) & 0x0000FF0000000000ULL) + ((((TForteUInt64) pa_pcBytes[3]) << 32) & 0x000000FF00000000ULL) + ((((TForteUInt64) pa_pcBytes[4]) << 24) & 0x00000000FF000000ULL) + ((((TForteUInt64) pa_pcBytes[5]) << 16) & 0x0000000000FF0000ULL) + ((((TForteUInt64) pa_pcBytes[6]) << 8) & 0x000000000000FF00ULL) + ((TForteUInt64) pa_pcBytes[7] & 0x00000000000000FFULL);
    }
    ;*/

};

#endif /*_FORTE_LREAL_H_*/
