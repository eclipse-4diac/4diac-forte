/*******************************************************************************
 * Copyright (c) 2005 - 2013 Profactor GmbH, ACIN
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
#ifndef _FORTE_UDINT_H_
#define _FORTE_UDINT_H_

#include "forte_any_int.h"
#include <limits>

/*!\ingroup COREDTS CIEC_UDINT represents the udint data type according to IEC 61131.
 */
class CIEC_UDINT : public CIEC_ANY_INT{
  DECLARE_FIRMWARE_DATATYPE(UDINT)

  public:
    typedef TForteUInt32 TValueType;

    static const TValueType scm_nMinVal = 0;
    static const TValueType scm_nMaxVal;

    CIEC_UDINT(){
    }

    CIEC_UDINT(const CIEC_UDINT& paValue) :
        CIEC_ANY_INT(){
      setValueSimple(paValue);
    }

    // We don't want this constructor to be explicit as it simplifies code generation for ST algorithms
    // Maybe when we have better code generators we want to make this constructor explicit again and generate it
    // cppcheck-suppress noExplicitConstructor
    CIEC_UDINT(TForteUInt32 paValue){
      setTUINT32(paValue);
    }

    virtual ~CIEC_UDINT(){
    }

    /*! \brief Operator: CIEC_UDINT data type = unsigned long data type
     *
     *   This command implements the assignment operator for the C++ data type UNSIGNED LONG
     *   The parameter value is represented by a C++ variable (call by value).
     *   \param paValue  Value for assignment.
     *   \return Can be the following response:
     *     - Pointer to given object.
     */
    CIEC_UDINT& operator =(TForteUInt32 paValue){
      setTUINT32(paValue);
      return *this;
    }

    CIEC_UDINT& operator =(const CIEC_UDINT &paValue){
      // Simple value assignment - no self assignment check needed
      setValueSimple(paValue);
      return *this;
    }

    /*! \brief Converts CIEC_UDINT to elementary unsigned 32 bit integer
     *
     *   Conversion operator for converting CIEC_UDINT to elementary unsigned 32 bit integer
     */
    operator TForteUInt32() const{
      return getTUINT32();
    }

    virtual EDataTypeID getDataTypeID() const{
      return CIEC_ANY::e_UDINT;
    }
};

#endif /*_FORTE_UDINT_H_*/
