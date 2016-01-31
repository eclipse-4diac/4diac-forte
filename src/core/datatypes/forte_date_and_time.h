/*******************************************************************************
  * Copyright (c) 2008 - 2013 nxtControl GmbH, ACIN, fortiss GmbH
  * All rights reserved. This program and the accompanying materials
  * are made available under the terms of the Eclipse Public License v1.0
  * which accompanies this distribution, and is available at
  * http://www.eclipse.org/legal/epl-v10.html
  *
  * Contributors:
  *    Stanislav Meduna, Alois Zoitl, Martin Melik Merkumians, Monika Wenger
  *      - initial implementation and rework communication infrastructure
  *******************************************************************************/
#ifndef _FORTE_DATE_AND_TIME_H_
#define _FORTE_DATE_AND_TIME_H_

#include "forte_any_date.h"
#include <time.h>
#if defined(WINCE)
#include <wce_time.h>
#endif

#ifdef FORTE_USE_64BIT_DATATYPES

/*!\ingroup COREDTS CIEC_DATE_AND_TIME represents the time data types according to IEC 61131.
 */

class CIEC_DATE_AND_TIME : public CIEC_ANY_DATE {
    DECLARE_FIRMWARE_DATATYPE(DATE_AND_TIME)
  public:
    typedef TForteUInt64 TValueType;

    CIEC_DATE_AND_TIME()  { }

    CIEC_DATE_AND_TIME(const CIEC_DATE_AND_TIME& pa_roValue) : CIEC_ANY_DATE() {
      setValueSimple(pa_roValue);
    }

    CIEC_DATE_AND_TIME(TForteUInt64 pa_nValue) {
      setTUINT64(pa_nValue);
    }

    virtual ~CIEC_DATE_AND_TIME() {}

  /*! \brief Operator: CIEC_DATE_AND_TIME data type = long data type
   *
   *   This command implements the assignment operator for the C++ datatype long long.
   *   The parameter value is represented by a C++ variable (call by value).
   *   \param pa_nValue  Value for assignment.
   *   \return Can be the following response:
   *     - Pointer to given object.
   */
    CIEC_DATE_AND_TIME& operator = (TForteUInt64 pa_nValue) {
      setTUINT64(pa_nValue);
      return *this;
    }

    CIEC_DATE_AND_TIME& operator = (const CIEC_DATE_AND_TIME &pa_roValue){
      setValueSimple(pa_roValue);
      return *this;
    }

  /*! \brief Operator: CIEC_TIME data type operator++
   *
   *   This command implements the increment operator for the IEC61131 data type TIME.
   *   The parameter value is represented by a C++ variable (call by value).
   *   \return Can be the following response:
   *     - Pointer to given object.
   */
    CIEC_DATE_AND_TIME& operator ++ (void) {
      setTUINT64(getTUINT64()+1);
      return *this;
    }
  /*! \brief Converts CIEC_SINT to elementary 32 bit integer
   *
   *   Conversion operator for converting CIEC_SINT to elementary 32 bit integer
   */
    operator TForteUInt64 () const { return getTUINT64(); }

    virtual EDataTypeID getDataTypeID() const{
          return CIEC_ANY::e_DATE_AND_TIME;
        }

  /*! \brief Converts string value to data type value
   *
   *   This command implements a conversion function from IEC61131
   *   data type (string format) to a C++ conform type.
   *   This function is necessary for communication with a proper engineering system.
   *   \param pa_pacValue  Pointer to the given String
   *   \return Can be the following response:
   *   \return number of bytes taken used from the buffer
   *        -1 on on error
   */
    virtual int fromString(const char *pa_pacValue);
      /*! \brief Converts data type value to string - use local time
       *
       *   This command implements a conversion function from C++ data type
       *   to IEC61131 conform data type (string format).
       *   This function is necessary for communication with a proper engineering system.
       *   \param pa_pacValue Pointer to the provided buffer
       *   \param pa_nBufferSize Size of the provided buffer
       *   \return number of bytes used in the buffer without trailing 0x00
       *           -1 on error
       */
    virtual int toString(char* pa_pacValue, unsigned int pa_nBufferSize) const ;

      /*! \brief Converts data type value to string - use GMT time
       *   \param pa_pacValue Pointer to the provided buffer
       *   \param pa_nBufferSize Size of the provided buffer
       *   \return number of bytes used in the buffer without trailing 0x00
       *           -1 on error
       */
    virtual int toGMTString(char* pa_pacValue, unsigned int pa_nBufferSize) const ;

    /*! \brief Set the variable to current time
    */
    void setCurrentTime(){
    #if ! defined(WINCE)
      setTUINT64(time(0) * 1000ULL);
    #else
      setTUINT64(wceex_time(0) * 1000ULL);
    #endif
    };
};

#endif /* #ifdef FORTE_USE_64BIT_DATATYPES */

#endif /*_MTIME_H_*/
