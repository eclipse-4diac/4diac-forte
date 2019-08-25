/*******************************************************************************
 * Copyright (c) 2009 - 2013 ACIN
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Monika Wenger, Alois Zoitl,
 *      - initial implementation and rework communication infrastructure
  *******************************************************************************/
#ifndef _FORTE_LINT_H_
#define _FORTE_LINT_H_

#include "forte_any_int.h"
#include <limits>

#ifdef FORTE_USE_64BIT_DATATYPES

/*!\ingroup COREDTS CIEC_LINT represents the dint data type according to IEC 61131.
 */
class CIEC_LINT : public CIEC_ANY_INT{
  DECLARE_FIRMWARE_DATATYPE(LINT)

  public:
    typedef TForteInt64 TValueType;

    static const TValueType scm_nMinVal;
    static const TValueType scm_nMaxVal;

    CIEC_LINT(){
    }

    CIEC_LINT(const CIEC_LINT& paValue) :
        CIEC_ANY_INT(){
      setValueSimple(paValue);
    }

    // We don't want this constructor to be explicit as it simplifies code generation for ST algorithms
    // Maybe when we have better code generators we want to make this constructor explicit again and generate it
    // cppcheck-suppress noExplicitConstructor
    CIEC_LINT(TForteInt64 paValue){
      setTINT64(paValue);
    }

    virtual ~CIEC_LINT(){
    }

    /*! \brief Operator: CIEC_LINT data type = long data type
     *
     *   This command implements the assignment operator for the C++ data type LONG.
     *   The parameter value is represented by a C++ variable (call by value).
     *   \param paValue  Value for assignment.
     *   \return Can be the following response:
     *     - Pointer to given object.
     */
    CIEC_LINT& operator =(TForteInt64 paValue){
      setTINT64(paValue);
      return *this;
    }

    CIEC_LINT& operator =(const CIEC_LINT &paValue){
      // Simple value assignment - no self assignment check needed
      setValueSimple(paValue);
      return *this;
    }

    /*! \brief Converts CIEC_LINT to elementary byte
     *
     *   Conversion operator for converting CIEC_LINT to elementary 64 bit integer
     */
    operator TForteInt64() const{
      return getTINT64();
    }

    virtual EDataTypeID getDataTypeID() const{
      return CIEC_ANY::e_LINT;
    }
};

#endif
#endif /*_FORTE_LINT_H_*/
