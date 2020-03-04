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
 *    Thomas Strasser, Ingomar Müller, Alois Zoitl, Gerhard Ebenhofer, Ingo Hegny,
 *    Monika Wenger
 *      - initial implementation and rework communication infrastructure
 *    Ernst Blecha - add multibit partial access
 *******************************************************************************/
#ifndef _FORTE_WORD_H_
#define _FORTE_WORD_H_

#include "forte_any_bit.h"
#include "forte_bool.h"
#include "forte_byte.h"
#include <limits>

/*!\ingroup COREDTS CIEC_WORD represents the word data type according to IEC 61131.
 */
class CIEC_WORD : public CIEC_ANY_BIT{
  DECLARE_FIRMWARE_DATATYPE(WORD)

  public:
    typedef TForteWord TValueType;

    static const TValueType scm_nMinVal = 0;
    static const TValueType scm_nMaxVal;

    CIEC_WORD(){
    }

    CIEC_WORD(const CIEC_WORD& paValue) :
        CIEC_ANY_BIT(){
      setValueSimple(paValue);
    }

    // We don't want this constructor to be explicit as it simplifies code generation for ST algorithms
    // Maybe when we have better code generators we want to make this constructor explicit again and generate it
    // cppcheck-suppress noExplicitConstructor
    CIEC_WORD(TForteWord pa_cValue){
      setTUINT16(pa_cValue);
    }

    virtual ~CIEC_WORD(){
    }

    /*! \brief Operator: CIEC_WORD data type = unsigned short data type
     *
     *   This command implements the assignment operator for the C++ data type UNSIGNED SHORT
     *   The parameter value is represented by a C++ variable (call by value).
     *   \param paValue  Value for assignment.
     *   \return Can be the following response:
     *     - Pointer to given object.
     */
    CIEC_WORD& operator =(TForteWord paValue){
      setTUINT16(paValue);
      return *this;
    }

    CIEC_WORD& operator =(const CIEC_WORD &paValue){
      // Simple value assignment - no self assignment check needed
      setValueSimple(paValue);
      return *this;
    }

    /*! \brief Converts CIEC_WORD to elementary word
     *
     *   Conversion operator for converting CIEC_WORD to elementary word
     */
    operator TForteWord() const{
      return getTUINT16();
    }

    virtual EDataTypeID getDataTypeID() const{
      return CIEC_ANY::e_WORD;
    }

    /*! \brief Partial access within a CIEC_WORD (e.g. [WORD].partial<CIEC_BOOL,1>())
     *
     */
    template <class T, size_t paIndex> PARTIAL_ACCESS<T, CIEC_WORD, paIndex> partial(){
      return PARTIAL_ACCESS<T,CIEC_WORD, paIndex>(*this);
    }

};

#endif /*_FORTE_WORD_H_*/
