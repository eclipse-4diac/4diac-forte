/*******************************************************************************
  * Copyright (c) 2005 - 2013 Profactor GmbH, ACIN
  * All rights reserved. This program and the accompanying materials
  * are made available under the terms of the Eclipse Public License v1.0
  * which accompanies this distribution, and is available at
  * http://www.eclipse.org/legal/epl-v10.html
  *
  * Contributors:
  *    Thomas Strasser, Ingomar Müller, Alois Zoitl, Gerhard Ebenhofer, Ingo Hegny,
  *    Monika Wenger
  *      - initial implementation and rework communication infrastructure
  *******************************************************************************/
#ifndef _FORTE_WORD_H_
#define _FORTE_WORD_H_

#include "forte_any_bit.h"
#include <limits>

/*!\ingroup COREDTS CIEC_WORD represents the word data type according to IEC 61131.
 */
class CIEC_WORD : public CIEC_ANY_BIT{
  DECLARE_FIRMWARE_DATATYPE(WORD)

  public:
    typedef TForteWord TValueType;

    static const TValueType scm_nMinVal = 0;
    static const TValueType scm_nMaxVal;

    CIEC_WORD(){
    }

    CIEC_WORD(const CIEC_WORD& pa_roValue) :
        CIEC_ANY_BIT(){
      setValueSimple(pa_roValue);
    }

    CIEC_WORD(TForteWord pa_cValue){
      setTUINT16(pa_cValue);
    }

    virtual ~CIEC_WORD(){
    }

    /*! \brief Operator: CIEC_WORD data type = unsigned short data type
     *
     *   This command implements the assignment operator for the C++ datatype UNSIGNED SHORT
     *   The parameter value is represented by a C++ variable (call by value).
     *   \param pa_cValue  Value for assignment.
     *   \return Can be the following response:
     *     - Pointer to given object.
     */
    CIEC_WORD& operator =(TForteWord pa_cValue){
      setTUINT16(pa_cValue);
      return *this;
    }

    CIEC_WORD& operator =(const CIEC_WORD &pa_roValue){
      setValueSimple(pa_roValue);
      return *this;
    }

    /*! \brief Converts CIEC_WORD to elementary word
     *
     *   Conversion operator for converting CIEC_WORD to elementary word
     */
    operator TForteWord() const{
      return getTUINT16();
    }

    virtual EDataTypeID getDataTypeID() const{
      return CIEC_ANY::e_WORD;
    }

    static const TForteUInt16 scm_unMaxStringBufSize;
};

#endif /*_FORTE_WORD_H_*/
