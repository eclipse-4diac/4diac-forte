/*******************************************************************************
 * Copyright (c) 2005 - 2013 Profactor GmbH, ACIN, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Thomas Strasser, Ingomar Müller, Alois Zoitl, Gerhard Ebenhofer,
 *    Ingo Hegny, Monika Wenger
 *      - initial implementation and rework communication infrastructure
 *******************************************************************************/
#ifndef _FORTE_UINT_H_
#define _FORTE_UINT_H_

#include "forte_any_int.h"
#include <limits>

/*!\ingroup COREDTS CIEC_UINT represents the uint data type according to IEC 61131.
 */
class CIEC_UINT : public CIEC_ANY_INT{
  DECLARE_FIRMWARE_DATATYPE(UINT)

  public:
    typedef TForteUInt16 TValueType;

    static const TValueType scm_nMinVal = 0;
    static const TValueType scm_nMaxVal;

    CIEC_UINT(){
    }

    CIEC_UINT(const CIEC_UINT& paValue) :
        CIEC_ANY_INT(){
      setValueSimple(paValue);
    }

    // We don't want this constructor to be explicit as it simplifies code generation for ST algorithms
    // Maybe when we have better code generators we want to make this constructor explicit again and generate it
    // cppcheck-suppress noExplicitConstructor
    CIEC_UINT(TForteUInt16 paValue){
      setTUINT16(paValue);
    }

    virtual ~CIEC_UINT(){
    }

    /*! \brief Operator: CIEC_UINT data type = unsigned short data type
     *
     *   This command implements the assignment operator for the C++ data type UNSIGNED SHORT
     *   The parameter value is represented by a C++ variable (call by value).
     *   \param pa_nValue  Value for assignment.
     *   \return Can be the following response:
     *     - Pointer to given object.
     */
    CIEC_UINT& operator=(TForteUInt16 paValue){
      setTUINT16(paValue);
      return *this;
    }

    CIEC_UINT& operator =(const CIEC_UINT &paValue){
      // Simple value assignment - no self assignment check needed
      setValueSimple(paValue);
      return *this;
    }

    /*! \brief Converts CIEC_UDINT to elementary unsigned 16 bit integer
     *
     *   Conversion operator for converting CIEC_UDINT to elementary unsigned 16 bit integer
     */
    operator TForteUInt16() const{
      return getTUINT16();
    }

    virtual EDataTypeID getDataTypeID() const{
      return CIEC_ANY::e_UINT;
    }
};

#endif /*_FORTE_UINT_H_*/
