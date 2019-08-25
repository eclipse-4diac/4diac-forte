/*******************************************************************************
 * Copyright (c) 2005 - 2013 Profactor GmbH, ACIN
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
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

    CIEC_BOOL(const CIEC_BOOL& paValue) :
        CIEC_ANY_BIT(){
      *this = paValue;
    }

    // We don't want this constructor to be explicit as it simplifies code generation for ST algorithms
    // Maybe when we have better code generators we want to make this constructur explicit again and generate it
    // cppcheck-suppress noExplicitConstructor
    CIEC_BOOL(bool paValue){
      *this = paValue;
    }

    virtual ~CIEC_BOOL(){
    }

    /*! \brief Operator: CIEC_BOOL data type = bool data type
     *
     *   This command implements the assignment operator for the IEC 61131 datatype BOOL.
     *   The parameter value is a reference onto a given C++ variable (call by reference).
     *   \param paValue  Value for assignment.
     *   \return Can be the following response:
     *     - Pointer to given object.
     */
    CIEC_BOOL& operator =(bool paValue){
      setTBOOL8(paValue);
      return *this;
    }

    CIEC_BOOL& operator =(const CIEC_BOOL &paValue){
      // Simple value assignment - no self assignment check needed
      setValueSimple(paValue);
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
     *   This function is necessary for communication with a proper engineering systems.
     *   \param paValue Pointer to the provided buffer
     *   \param paBufferSize Size of the provided buffer
     *   \return number of bytes used in the buffer without trailing 0x00
     *           -1 on error
     */
    virtual int toString(char* paValue, size_t paBufferSize) const;

  private:
};

#endif /*_FORTE_BOOL_H_*/
