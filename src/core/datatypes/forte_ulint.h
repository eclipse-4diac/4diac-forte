/*******************************************************************************
 * Copyright (c) 2009 - 2013 ACIN, fortiss GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Monika Wenger, Alois Zoitl
 *      - initial implementation and rework communication infrastructure
 *******************************************************************************/
#ifndef _FORTE_ULINT_H_
#define _FORTE_ULINT_H_

#include "forte_any_int.h"
#include <limits>

#ifdef FORTE_USE_64BIT_DATATYPES

/*!\ingroup COREDTS CIEC_ULINT represents the ulint data type according to IEC 61131.
 *  @author mw & az
 */
class CIEC_ULINT : public CIEC_ANY_INT{
  DECLARE_FIRMWARE_DATATYPE(ULINT)

  public:
    typedef TForteUInt64 TValueType;

    static const TValueType scm_nMinVal = 0;
    static const TValueType scm_nMaxVal;

    CIEC_ULINT(){
    }

    CIEC_ULINT(const CIEC_ULINT& paValue) : CIEC_ANY_INT() {
      setValueSimple(paValue);
    }

    // We don't want this constructor to be explicit as it simplifies code generation for ST algorithms
    // Maybe when we have better code generators we want to make this constructor explicit again and generate it
    // cppcheck-suppress noExplicitConstructor
   CIEC_ULINT(TForteUInt64 paValue){
      setTUINT64(paValue);
    }

    virtual ~CIEC_ULINT(){
    }

    /*! \brief Operator: CIEC_ULINT data type = unsigned long data type
     *
     *   This command implements the assignment operator for the C++ data type UNSIGNED LONG
     *   The parameter value is represented by a C++ variable (call by value).
     *   \param paValue  Value for assignment.
     *   \return Can be the following response:
     *     - Pointer to given object.
     */
    CIEC_ULINT& operator =(TForteUInt64 paValue){
      setTUINT64(paValue);
      return *this;
    }

    CIEC_ULINT& operator =(const CIEC_ULINT &paValue){
      // Simple value assignment - no self assignment check needed
      setValueSimple(paValue);
      return *this;
    }

    /*! \brief Converts CIEC_ULINT to elementary unsigned 64 bit integer
     *
     *   Conversion operator for converting CIEC_ULINT to elementary unsigned 64 bit integer
     */
    operator TForteUInt64() const{
      return getTUINT64();
    }

    virtual EDataTypeID getDataTypeID() const{
      return CIEC_ANY::e_ULINT;
    }
};

#endif
#endif /*_FORTE_ULINT_H_*/
