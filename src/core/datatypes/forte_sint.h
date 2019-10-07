/*******************************************************************************
 * Copyright (c) 2005 - 2015 Profactor GmbH, ACIN
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Thomas Strasser, Ingomar Müller, Alois Zoitl, Gerhard Ebenhofer,
 *    Ingo Hegny, Martin Melik Merkumians
 *      - initial implementation and rework communication infrastructure
 *******************************************************************************/
#ifndef _FORTE_SINT_H_
#define _FORTE_SINT_H_

#include "forte_any_int.h"
#include <limits>

/*!\ingroup COREDTS CIEC_SINT represents the SINT data type according to IEC 61131.
 */
class CIEC_SINT : public CIEC_ANY_INT{
  DECLARE_FIRMWARE_DATATYPE(SINT)

  public:
    typedef TForteInt8 TValueType;

    static const TValueType scm_nMinVal;
    static const TValueType scm_nMaxVal;

    CIEC_SINT(){
    }

    CIEC_SINT(const CIEC_SINT& paValue) :
        CIEC_ANY_INT(){
      setValueSimple(paValue);
    }

    // We don't want this constructor to be explicit as it simplifies code generation for ST algorithms
    // Maybe when we have better code generators we want to make this constructor explicit again and generate it
    // cppcheck-suppress noExplicitConstructor
    CIEC_SINT(TForteInt8 paValue){
      setTINT8(paValue);
    }

    virtual ~CIEC_SINT(){
    }

    /*! \brief Operator: CIEC_SINT data type = char data type
     *
     *   This command implements the assignment operator for the C++ data type CHAR
     *   The parameter value is represented by a C++ variable (call by value).
     *   \param paValue  Value for assignment.
     *   \return Can be the following response:
     *     - Pointer to given object.
     */
    CIEC_SINT& operator =(TForteInt8 paValue){
      setTINT8(paValue);
      return *this;
    }

    CIEC_SINT& operator =(const CIEC_SINT &paValue){
      // Simple value assignment - no self assignment check needed
      setValueSimple(paValue);
      return *this;
    }

    /*! \brief Converts CIEC_SINT to elementary 8 bit integer
     *
     *   Conversion operator for converting CIEC_SINT to elementary 8 bit integer
     */
    operator TForteInt8() const{
      return getTINT8();
    }

    virtual EDataTypeID getDataTypeID() const{
      return CIEC_ANY::e_SINT;
    }
};

#endif /*_FORTE_SINT_H_*/
