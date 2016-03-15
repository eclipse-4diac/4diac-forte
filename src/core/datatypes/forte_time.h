/*******************************************************************************
  * Copyright (c) 2005 - 2013 Profactor GmbH, ACIN
  * All rights reserved. This program and the accompanying materials
  * are made available under the terms of the Eclipse Public License v1.0
  * which accompanies this distribution, and is available at
  * http://www.eclipse.org/legal/epl-v10.html
  *
  * Contributors:
  *    Thomas Strasser, Ingomar Müller, Alois Zoitl, Gerhard Ebenhofer,
  *    Monika Wenger, Ingo Hegny, Martin Melik Merkumians,
  *      - initial implementation and rework communication infrastructure
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

    CIEC_TIME(const CIEC_TIME& pa_roValue) :
        CIEC_ANY_MAGNITUDE(){
      setValueSimple(pa_roValue);
    }

    // We don't want this constructor to be explicit as it simplifies code generation for ST algorithms
    // Maybe when we have better code generators we want to make this constructur explicit again and generate it
    // cppcheck-suppress noExplicitConstructor
    CIEC_TIME(TValueType pa_nValue){
      *this = pa_nValue;
    }

    // We don't want this constructor to be explicit as it simplifies code generation for ST algorithms
    // Maybe when we have better code generators we want to make this constructur explicit again and generate it
    // cppcheck-suppress noExplicitConstructor
    CIEC_TIME(const char *pa_pacValue){
      if(-1 == CIEC_TIME::fromString(pa_pacValue)){  //explicitly state that you like to have the fromString of this class to avoid potential virtual function clashes in the constructor
        //we had an error in string parsing set a default value
        *this = 0;
      }
    }

    virtual ~CIEC_TIME(){
    }

    CIEC_TIME& operator =(TValueType pa_nValue){
      setLargestInt(pa_nValue);
      return *this;
    }

    CIEC_TIME& operator =(const CIEC_TIME &pa_roValue){
      setValueSimple(pa_roValue);
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
     *   \param pa_nBufferSize Size of the given buffer
     *   \return number of bytes used in the buffer without trailing 0x00
     *           -1 on error
     */
    virtual int toString(char* pa_pacValue, unsigned int pa_nBufferSize) const;

    TValueType getInSeconds() const;
    TValueType getInMiliSeconds() const;
    TValueType getInMicroSeconds() const;

    void setFromSeconds(TValueType pa_nValue);
    void setFromMiliSeconds(TValueType pa_nValue);
    void setFromMicroSeconds(TValueType pa_nValue);

    static const TForteUInt16 scm_unMaxStringBufSize;
};

const CIEC_TIME TIME();

#endif /*_MTIME_H_*/
