/*******************************************************************************
 * Copyright (c) 2010 - 2013 ACIN
 *               2020 Johannes Kepler University Linz
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Martin Melik Merkiumians, Alois Zoitl, Monika Wenger
 *      - initial implementation and rework communication infrastructure
 *    Ernst Blecha - add multibit partial access
  *******************************************************************************/
#ifndef _FORTE_LWORD_H_
#define _FORTE_LWORD_H_

#include "forte_any_bit.h"
#include "forte_bool.h"
#include "forte_byte.h"
#include "forte_word.h"
#include "forte_dword.h"
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

    CIEC_LWORD(const CIEC_LWORD& paValue) :
        CIEC_ANY_BIT(){
      setValueSimple(paValue);
    }

    // We don't want this constructor to be explicit as it simplifies code generation for ST algorithms
    // Maybe when we have better code generators we want to make this constructor explicit again and generate it
    // cppcheck-suppress noExplicitConstructor
    CIEC_LWORD(TForteLWord paValue){
      setTUINT64(paValue);
    }
    virtual ~CIEC_LWORD(){
    }

    /*! \brief Operator: CIEC_LWORD data type = long data type
     *
     *   This command implements the assignment operator for the C++ data type UNSIGNED LONG LONG
     *   The parameter value is represented by a C++ variable (call by value).
     *   \param paValue  Value for assignment.
     *   \return Can be the following response:
     *     - Pointer to given object.
     */
    CIEC_LWORD& operator =(TForteLWord paValue){
      setTUINT64(paValue);
      return *this;
    }

    CIEC_LWORD& operator =(const CIEC_LWORD &paValue){
      // Simple value assignment - no self assignment check needed
      setValueSimple(paValue);
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

    /*! \brief Partial access within a CIEC_LWORD (e.g. [LWORD].partial<CIEC_BOOL,1>())
     *
     */
    template <class T, size_t paIndex> PARTIAL_ACCESS<T, CIEC_LWORD, paIndex> partial(){
      return PARTIAL_ACCESS<T,CIEC_LWORD, paIndex>(*this);
    }

};

#endif

#endif /*_FORTE_LWORD_H_*/
