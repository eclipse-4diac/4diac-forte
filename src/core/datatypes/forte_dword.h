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
 *    Thomas Strasser, Ingomar Müller, Alois Zoitl, Gerhard Ebenhofer
 *    Ingo Hegny, Monika Wenger
 *      - initial implementation and rework communication infrastructure
 *    Ernst Blecha - add multibit partial access
 *******************************************************************************/
#ifndef _FORTE_DWORD_H_
#define _FORTE_DWORD_H_

#include "forte_any_bit.h"
#include "forte_bool.h"
#include "forte_byte.h"
#include "forte_word.h"
#include <limits>

/*!\ingroup COREDTS CIEC_DWORD represents the dword data type according to IEC 61131.
 */
class CIEC_DWORD : public CIEC_ANY_BIT{
  DECLARE_FIRMWARE_DATATYPE(DWORD)

  public:
    typedef TForteDWord TValueType;

    static const TValueType scm_nMinVal = 0;
    static const TValueType scm_nMaxVal;

    CIEC_DWORD(){
    }

    CIEC_DWORD(const CIEC_DWORD& paValue) :
        CIEC_ANY_BIT(){
      setValueSimple(paValue);
    }

    // We don't want this constructor to be explicit as it simplifies code generation for ST algorithms
    // Maybe when we have better code generators we want to make this constructor explicit again and generate it
    // cppcheck-suppress noExplicitConstructor
    CIEC_DWORD(TForteDWord paValue){
      setTUINT32(paValue);
    }

    virtual ~CIEC_DWORD(){
    }

    /*! \brief Operator: CIEC_DWORD data type = long data type
     *
     *   This command implements the assignment operator for the C++ data type UNSIGNED LONG
     *   The parameter value is represented by a C++ variable (call by value).
     *   \param paValue  Value for assignment.
     *   \return Can be the following response:
     *     - Pointer to given object.
     */
    CIEC_DWORD& operator =(TForteDWord paValue){
      setTUINT32(paValue);
      return *this;
    }

    CIEC_DWORD& operator =(const CIEC_DWORD &paValue){
      // Simple value assignment - no self assignment check needed
      setValueSimple(paValue);
      return *this;
    }

    /*! \brief Converts CIEC_DWORD to elementary word
     *
     *   Conversion operator for converting CIEC_DWORD to elementary word
     */
    operator TForteDWord() const{
      return getTUINT32();
    }

    virtual EDataTypeID getDataTypeID() const{
      return CIEC_ANY::e_DWORD;
    }

    /*! \brief Partial access within a CIEC_DWORD (e.g. [DWORD].partial<CIEC_BOOL,1>())
     *
     */
    template <class T, size_t paIndex> PARTIAL_ACCESS<T, CIEC_DWORD, paIndex> partial(){
      return PARTIAL_ACCESS<T,CIEC_DWORD, paIndex>(*this);
    }

};

#endif /*_FORTE_DWORD_H_*/
