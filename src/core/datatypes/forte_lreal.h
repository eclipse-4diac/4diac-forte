/*******************************************************************************
 * Copyright (c) 2005 - 2013 Profactor GmbH, ACIN
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
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

    CIEC_LREAL(const CIEC_LREAL& paValue) :
        CIEC_ANY_REAL(){
      setValueSimple(paValue);
    }

    // We don't want this constructor to be explicit as it simplifies code generation for ST algorithms
    // Maybe when we have better code generators we want to make this constructor explicit again and generate it
    // cppcheck-suppress noExplicitConstructor
    CIEC_LREAL(TForteDFloat paValue){
      setTDFLOAT(paValue);
    }

    virtual ~CIEC_LREAL(){
    }

    /*! \brief Operator: CIEC_LREAL data type = CIEC_LREAL data type
     *
     *   This command implements the assignment operator for the C++ data type DFLOAT.
     *   The parameter value is represented by a C++ variable (call by value).
     *   \param paValue  Value for assignment.
     *   \return Can be the following response:
     *     - Pointer to given object.
     */
    CIEC_LREAL& operator =(TForteDFloat pa_fValue){
      setTDFLOAT(pa_fValue);
      return *this;
    }

    CIEC_LREAL& operator =(const CIEC_LREAL &paValue){
      // Simple value assignment - no self assignment check needed
      setValueSimple(paValue);
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

    virtual void setValue(const CIEC_ANY& paValue);

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
     *   to IEC61131 conform data type (string format).
     *   This function is necessary for communication with a proper engineering system.
     *   \param paValue Pointer to the buffer String
     *   \param paBufferSize size of the buffer thats available for this function
     *   \return number of bytes used in the buffer without trailing 0x00
     *           -1 on error
     */
    virtual int toString(char* paValue, size_t paBufferSize) const;

    /*! \brief Casts TForteDFloat values to CIEC data types
     *  \param paSrcValue Source value for the cast
     *  \param paDestValue Destination CIEC object of the cast
     */
    static void castLRealData(const CIEC_LREAL &paSrcValue, CIEC_ANY &paDestValue);
};

#endif /*_FORTE_LREAL_H_*/
