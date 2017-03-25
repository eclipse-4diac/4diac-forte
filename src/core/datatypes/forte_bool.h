/*******************************************************************************
  * Copyright (c) 2005 - 2013 Profactor GmbH, ACIN
  * All rights reserved. This program and the accompanying materials
  * are made available under the terms of the Eclipse Public License v1.0
  * which accompanies this distribution, and is available at
  * http://www.eclipse.org/legal/epl-v10.html
  *
  * Contributors:
  *    Thomas Strasser, Ingomar Müller, Alois Zoitl, Gerhard Ebenhofer,
  *    Ingo Hegny, Martin Melik-Merkiumians, Monika Wenger
  *      - initial implementation and rework communication infrastructure
  *******************************************************************************/
#ifndef _FORTE_BOOL_H_
#define _FORTE_BOOL_H_

#include "forte_any_bit.h"

/*!\ingroup COREDTS CIEC_BOOL represents the bool data type according to IEC 61131.
 */
class CIEC_BOOL : public CIEC_ANY_BIT{
  DECLARE_FIRMWARE_DATATYPE(BOOL)

  public:
    typedef bool TValueType;

    CIEC_BOOL(){
      setTBOOL8(false);
    }

    CIEC_BOOL(const CIEC_BOOL& pa_rbValue) : CIEC_ANY_BIT() {
      *this = pa_rbValue;
    }

    // We don't want this constructor to be explicit as it simplifies code generation for ST algorithms
    // Maybe when we have better code generators we want to make this constructur explicit again and generate it
    // cppcheck-suppress noExplicitConstructor
    CIEC_BOOL(bool pa_bValue){
      *this = pa_bValue;
    }

    virtual ~CIEC_BOOL(){
    }

    /*! \brief Operator: CIEC_BOOL data type = bool data type
     *
     *   This command implements the assignment operator for the IEC61131 datatype BOOL.
     *   The parameter value is a reference onto a given C++ variable (call by reference).
     *   \param pa_bValue  Value for assignment.
     *   \return Can be the following response:
     *     - Pointer to given object.
     */
    CIEC_BOOL& operator =(bool pa_bValue){
      setTBOOL8(pa_bValue);
      return *this;
    }

    CIEC_BOOL& operator =(const CIEC_BOOL &pa_roValue){
      setValueSimple(pa_roValue);
      return *this;
    }

    /*! \brief Converts CIEC_BOOL to elementary bool
     *
     *   Conversion operator for converting CIEC_BOOL to elementary bool
     */
    operator bool() const{
      return (0 != getLargestUInt());
    }

    virtual EDataTypeID getDataTypeID() const{
      return CIEC_ANY::e_BOOL;
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
     *   This function is necessary for communication with a proper engineering systems.
     *   \param pa_pacValue Pointer to the provided buffer
     *   \param pa_nBufferSize Size of the provided buffer
     *   \return number of bytes used in the buffer without trailing 0x00
     *           -1 on error
     */
    virtual int toString(char* pa_pacValue, unsigned int pa_nBufferSize) const;

  private:
};

#endif /*_FORTE_BOOL_H_*/
