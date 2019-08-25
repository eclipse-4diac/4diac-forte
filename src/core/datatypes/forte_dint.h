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
#ifndef _FORTE_DINT_H_
#define _FORTE_DINT_H_

#include "forte_any_int.h"
#include <limits>

/*!\ingroup COREDTS CIEC_DINT represents the dint data type according to IEC 61131.
 */
class CIEC_DINT : public CIEC_ANY_INT{
  DECLARE_FIRMWARE_DATATYPE(DINT)

  public:
    typedef TForteInt32 TValueType;

    static const TValueType scm_nMinVal;
    static const TValueType scm_nMaxVal;

    CIEC_DINT(){
    }

    CIEC_DINT(const CIEC_DINT& paValue) :
        CIEC_ANY_INT(){
      setValueSimple(paValue);
    }

    // We don't want this constructor to be explicit as it simplifies code generation for ST algorithms
    // Maybe when we have better code generators we want to make this constructur explicit again and generate it
    // cppcheck-suppress noExplicitConstructor
    CIEC_DINT(TForteInt32 paValue){
      setTINT32(paValue);
    }

    virtual ~CIEC_DINT(){
    }

    /*! \brief Operator: CIEC_DINT data type = long data type
     *
     *   This command implements the assignment operator for the C++ data type LONG.
     *   The parameter value is represented by a C++ variable (call by value).
     *   \param paValue  Value for assignment.
     *   \return Can be the following response:
     *     - Pointer to given object.
     */
    CIEC_DINT& operator =(TForteInt32 paValue){
      setTINT32(paValue);
      return *this;
    }

    CIEC_DINT& operator =(const CIEC_DINT &paValue){
      // Simple value assignment - no self assignment check needed
      setValueSimple(paValue);
      return *this;
    }

    /*! \brief Converts CIEC_DINT to elementary byte
     *
     *   Conversion operator for converting CIEC_DINT to elementary 32 bit integer
     */
    operator TForteInt32() const{
      return getTINT32();
    }

    virtual EDataTypeID getDataTypeID() const{
      return CIEC_ANY::e_DINT;
    }
};

#endif /*_FORTE_DINT_H_*/
