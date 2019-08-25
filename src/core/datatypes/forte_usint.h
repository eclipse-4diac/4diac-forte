/*******************************************************************************
 * Copyright (c) 2005 - 2013 Profactor GmbH, ACIN, fortiss GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Thomas Strasser, Ingomar Müller, Alois Zoitl, Gerhard Ebenhofer,
 *    Ingo Hegny, Monika Wenger,
 *      - initial implementation and rework communication infrastructure
 *******************************************************************************/
#ifndef _FORTE_USINT_H_
#define _FORTE_USINT_H_

#include "forte_any_int.h"
#include <limits>

/*!\ingroup COREDTS CIEC_USINT represents the usint data type according to IEC 61131.
 */
class CIEC_USINT : public CIEC_ANY_INT{
  DECLARE_FIRMWARE_DATATYPE(USINT)

  public:
    typedef TForteUInt8 TValueType;

    static const TValueType scm_nMinVal = 0;
    static const TValueType scm_nMaxVal;

    CIEC_USINT(){
    }

    CIEC_USINT(const CIEC_USINT& pa_roValue) :
        CIEC_ANY_INT() {
      setValueSimple(pa_roValue);
    }

    // We don't want this constructor to be explicit as it simplifies code generation for ST algorithms
    // Maybe when we have better code generators we want to make this constructur explicit again and generate it
    // cppcheck-suppress noExplicitConstructor
    CIEC_USINT(TForteUInt8 pa_nValue){
      setTUINT8(pa_nValue);
    }

    virtual ~CIEC_USINT(){
    }

    /*! \brief Operator: CIEC_USINT data type = unsigned char data type
     *
     *   This command implements the assignment operator for the C++ datatype UNSIGNED CHAR
     *   The parameter value is represented by a C++ variable (call by value).
     *   \param pa_nValue  Value for assignment.
     *   \return Can be the following response:
     *     - Pointer to given object.
     */
    CIEC_USINT& operator =(TForteUInt8 pa_nValue){
      setTUINT8(pa_nValue);
      return *this;
    }

    CIEC_USINT& operator =(const CIEC_USINT &pa_roValue){
      setValueSimple(pa_roValue);
      return *this;
    }

    /*! \brief Converts CIEC_UDINT to elementary unsigned 16 bit integer
     *
     *   Conversion operator for converting CIEC_UDINT to elementary unsigned 16 bit integer
     */
    operator TForteUInt8() const{
      return getTUINT8();
    }

    virtual EDataTypeID getDataTypeID() const{
      return CIEC_ANY::e_USINT;
    }
};

#endif /*_FORTE_USINT_H_*/
