/*******************************************************************************
  * Copyright (c) 2010 - 2013 ACIN
  * All rights reserved. This program and the accompanying materials
  * are made available under the terms of the Eclipse Public License v1.0
  * which accompanies this distribution, and is available at
  * http://www.eclipse.org/legal/epl-v10.html
  *
  * Contributors:
  *    Martin Melik Merkiumians, Alois Zoitl, Monika Wenger
  *      - initial implementation and rework communication infrastructure
  *******************************************************************************/
#ifndef _FORTE_LWORD_H_
#define _FORTE_LWORD_H_

#include "forte_any_bit.h"
#include <limits>

#ifdef FORTE_USE_64BIT_DATATYPES

/*!\ingroup COREDTS CIEC_DWORD represents the dword data type according to IEC 61131.
 */

class CIEC_LWORD : public CIEC_ANY_BIT{
  DECLARE_FIRMWARE_DATATYPE(LWORD)

  public:
    typedef TForteLWord TValueType;

    static const TValueType scm_nMinVal = 0;
    static const TValueType scm_nMaxVal;

    CIEC_LWORD(){
    }

    CIEC_LWORD(const CIEC_LWORD& pa_roValue) :
        CIEC_ANY_BIT(){
      setValueSimple(pa_roValue);
    }

    // We don't want this constructor to be explicit as it simplifies code generation for ST algorithms
    // Maybe when we have better code generators we want to make this constructur explicit again and generate it
    // cppcheck-suppress noExplicitConstructor
    CIEC_LWORD(TForteLWord pa_cValue){
      setTUINT64(pa_cValue);
    }
    virtual ~CIEC_LWORD(){
    }

    /*! \brief Operator: CIEC_LWORD data type = long data type
     *
     *   This command implements the assignment operator for the C++ datatype UNSIGNED LONG LONG
     *   The parameter value is represented by a C++ variable (call by value).
     *   \param pa_cValue  Value for assignment.
     *   \return Can be the following response:
     *     - Pointer to given object.
     */
    CIEC_LWORD& operator =(TForteLWord pa_cValue){
      setTUINT64(pa_cValue);
      return *this;
    }

    CIEC_LWORD& operator =(const CIEC_LWORD &pa_roValue){
      setValueSimple(pa_roValue);
      return *this;
    }

    /*! \brief Converts CIEC_LWORD to elementary word
     *
     *   Conversion operator for converting CIEC_LWORD to elementary word
     */
    operator TForteLWord() const{
      return getTUINT64();
    }

    virtual EDataTypeID getDataTypeID() const{
      return CIEC_ANY::e_LWORD;
    }
};

#endif

#endif /*_FORTE_LWORD_H_*/
