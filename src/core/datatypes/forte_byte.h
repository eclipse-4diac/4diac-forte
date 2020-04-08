/*******************************************************************************
 * Copyright (c) 2005 - 2013 Profactor GmbH, ACIN
 *               2020 Johannes Kepler University Linz
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Thomas Strasser, Ingomar Müller, Alois Zoitl, Gerhard Ebenhofer,
 *    Ingo Hegny, Martin Melik-Merkumians, Monika Wenger
 *      - initial implementation and rework communication infrastructure
 *    Ernst Blecha - add multibit partial access
 *******************************************************************************/
#ifndef _FORTE_BYTE_H_
#define _FORTE_BYTE_H_

#include "forte_any_bit.h"
#include "forte_bool.h"
#include <limits>

/*!\ingroup COREDTS CIEC_BYTE represents the byte data type according to IEC 61131.
 */
class CIEC_BYTE : public CIEC_ANY_BIT{
  DECLARE_FIRMWARE_DATATYPE(BYTE)

  public:
    typedef TForteByte TValueType;

    static const TValueType scm_nMinVal = 0;
    static const TValueType scm_nMaxVal;

    CIEC_BYTE(){
    }

    CIEC_BYTE(const CIEC_BYTE& paValue) :
        CIEC_ANY_BIT(){
      setValueSimple(paValue);
    }

    explicit CIEC_BYTE(TForteByte paValue){
      setTUINT8(paValue);
    }

    virtual ~CIEC_BYTE(){
    }

    /*! \brief Operator: CIEC_BYTE data type = CIEC_BYTE data type
     *
     *   This command implements the assignment operator for the IEC 61131 data type BYTE.
     *   The parameter value is a reference onto a given C++ variable (call by reference).
     *   \param paValue  Reference for assignment.
     *   \return Can be the following response:
     *     - Pointer to given object.
     */
    CIEC_BYTE& operator =(TForteByte paValue){
      setTUINT8(paValue);
      return *this;
    }

    CIEC_BYTE& operator =(const CIEC_BYTE &paValue){
      // Simple value assignment - no self assignment check needed
      setValueSimple(paValue);
      return *this;
    }

    /*! \brief Converts CIEC_BYTE to elementary byte
     *
     *   Conversion operator for converting CIEC_BYTE to elementary byte
     */
    operator TForteByte() const{
      return getTUINT8();
    }

    virtual EDataTypeID getDataTypeID() const{
      return CIEC_ANY::e_BYTE;
    }

    /*! \brief Partial access within a CIEC_BYTE (e.g. [BYTE].partial<CIEC_BOOL,1>())
     *
     */
    template <class T, size_t paIndex> PARTIAL_ACCESS<T, CIEC_BYTE, paIndex> partial(){
      return PARTIAL_ACCESS<T,CIEC_BYTE, paIndex>(*this);
    }

};

#endif /*_FORTE_BYTE_H_*/
