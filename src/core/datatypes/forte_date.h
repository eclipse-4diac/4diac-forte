/*******************************************************************************
 * Copyright (c) 2008 - 2013 nxtControl GmbH, ACIN, fortiss GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Stanislav Meduna, Alois Zoitl, Martin Melik Merkumians, Monika Wenger
 *      - initial implementation and rework communication infrastructure
 *******************************************************************************/
#ifndef _FORTE_DATE_H_
#define _FORTE_DATE_H_

#include "forte_any_date.h"

#ifdef FORTE_USE_64BIT_DATATYPES

/*!\ingroup COREDTS CIEC_DATE represents the time data types according to IEC 61131.
 */

class CIEC_DATE : public CIEC_ANY_DATE{
  DECLARE_FIRMWARE_DATATYPE(DATE)
  public:
    typedef TForteUInt64 TValueType;

    CIEC_DATE(){
    }

    CIEC_DATE(const CIEC_DATE& paValue) :
        CIEC_ANY_DATE(){
      setValueSimple(paValue);
    }

    // We don't want this constructor to be explicit as it simplifies code generation for ST algorithms
    // Maybe when we have better code generators we want to make this constructor explicit again and generate it
    // cppcheck-suppress noExplicitConstructor
    CIEC_DATE(TForteUInt64 paValue){
      setTUINT64(paValue);
    }

    virtual ~CIEC_DATE(){
    }

    /*! \brief Operator: CIEC_DATE data type = long data type
     *
     *   This command implements the assignment operator for the C++ data type long long.
     *   The parameter value is represented by a C++ variable (call by value).
     *   \param paValue  Value for assignment.
     *   \return Can be the following response:
     *     - Pointer to given object.
     */
    CIEC_DATE& operator =(TForteUInt64 paValue){
      setTUINT64(paValue);
      return *this;
    }

    CIEC_DATE& operator =(const CIEC_DATE &paValue){
      // Simple value assignment - no self assignment check needed
      setValueSimple(paValue);
      return *this;
    }

    /*! \brief Operator: CIEC_TIME data type operator++
     *
     *   This command implements the increment operator for the IEC 61131 data type TIME.
     *   The parameter value is represented by a C++ variable (call by value).
     *   \return Can be the following response:
     *     - Pointer to given object.
     */
    CIEC_DATE& operator ++(void){
      setTUINT64(getTUINT64() + 1);
      return *this;
    }
    /*! \brief Converts CIEC_SINT to elementary 32 bit integer
     *
     *   Conversion operator for converting CIEC_SINT to elementary 32 bit integer
     */
    operator TForteUInt64() const{
      return getTUINT64();
    }

    virtual EDataTypeID getDataTypeID() const{
      return CIEC_ANY::e_DATE;
    }

    /*! \brief Converts string value to data type value
     *
     *   This command implements a conversion function from IEC 61131
     *   data type (string format) to a C++ conform type.
     *   This function is necessary for communication with a proper engineering system.
     *   \param paValue Pointer to the given String
     *   \return Can be the following response:
     *   \return number of bytes taken used from the buffer
     *        -1 on on error
     */
    virtual int fromString(const char *paValue);
    /*! \brief Converts data type value to string
     *
     *   This command implements a conversion function from C++ data type
     *   to IEC 61131 conform data type (string format).
     *   This function is necessary for communication with a proper engineering system.
     *   \param paValue Pointer to the provided buffer
     *   \param paBufferSize Size of the provided buffer
     *   \return number of bytes used in the buffer without trailing 0x00
     *           -1 on error
     */
    virtual int toString(char* paValue, size_t paBufferSize) const;
};

#endif /* #ifdef FORTE_USE_64BIT_DATATYPES */

#endif /*_FORTE_DATE_H_*/
