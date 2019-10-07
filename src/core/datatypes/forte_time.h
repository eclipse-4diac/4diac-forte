/*******************************************************************************
 * Copyright (c) 2005 - 2013 Profactor GmbH, ACIN; 2019 TU Wien/ACIN
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Thomas Strasser, Ingomar Müller, Alois Zoitl, Gerhard Ebenhofer,
 *    Monika Wenger, Ingo Hegny, Martin Melik Merkumians,
 *      - initial implementation and rework communication infrastructure
 *    Martin Melik-Merkumians - added getInNanoSeconds, setFromNanoSeconds,
 *      changed defines to constants, added literal parsing for micro and
 *      nanoseconds
 *******************************************************************************/
#ifndef _FORTE_TIME_H_
#define _FORTE_TIME_H_

#include "forte_any_magnitude.h"

/*!\ingroup COREDTS CIEC_TIME represents the time data types according to IEC 61131.
 */

class CIEC_TIME : public CIEC_ANY_MAGNITUDE{
  DECLARE_FIRMWARE_DATATYPE(TIME)
  public:
    typedef TLargestIntValueType TValueType;

    CIEC_TIME(){
    }

    CIEC_TIME(const CIEC_TIME& paValue) :
        CIEC_ANY_MAGNITUDE(){
      setValueSimple(paValue);
    }

    // We don't want this constructor to be explicit as it simplifies code generation for ST algorithms
    // Maybe when we have better code generators we want to make this constructor explicit again and generate it
    // cppcheck-suppress noExplicitConstructor
    CIEC_TIME(TValueType paValue){
      *this = paValue;
    }

    // We don't want this constructor to be explicit as it simplifies code generation for ST algorithms
    // Maybe when we have better code generators we want to make this constructor explicit again and generate it
    // cppcheck-suppress noExplicitConstructor
    CIEC_TIME(const char *paValue){
      if(-1 == CIEC_TIME::fromString(paValue)){ //explicitly state that you like to have the fromString of this class to avoid potential virtual function clashes in the constructor
      //we had an error in string parsing set a default value
        *this = 0;
      }
    }

    virtual ~CIEC_TIME(){
    }

    CIEC_TIME& operator =(TValueType paValue){
      setLargestInt(paValue);
      return *this;
    }

    CIEC_TIME& operator =(const CIEC_TIME &paValue){
      // Simple value assignment - no self assignment check needed
      setValueSimple(paValue);
      return *this;
    }

    CIEC_TIME& operator ++(void){
      *this = 1 + (*this);
      return *this;
    }

    operator TValueType() const{
      return getLargestInt();
    }

    virtual EDataTypeID getDataTypeID() const{
      return CIEC_ANY::e_TIME;
    }

    /*! \brief Converts string value to data type value
     *
     *   This command implements a conversion function from IEC61131
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
     *   \param paBufferSize Size of the given buffer
     *   \return number of bytes used in the buffer without trailing 0x00
     *           -1 on error
     */
    virtual int toString(char* paValue, size_t paBufferSize) const;

    TValueType getInSeconds() const;
    TValueType getInMilliSeconds() const;
    TValueType getInMicroSeconds() const;
    TValueType getInNanoSeconds() const;

    void setFromSeconds(TValueType paValue);
    void setFromMilliSeconds(TValueType paValue);
    void setFromMicroSeconds(TValueType paValue);
    void setFromNanoSeconds(TValueType paValue);
};

#endif /*_MTIME_H_*/
