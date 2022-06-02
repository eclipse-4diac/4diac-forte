/*******************************************************************************
 * Copyright (c) 2005 - 2013 Profactor GmbH, ACIN
 *               2020 Johannes Kepler University Linz
 *               2022 Primetals Technologies Austria GmbH
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
 *    Martin Melik Merkumians - changes multibit access index from template
 *      parameter to call argument, make TForteWord constructor explicit, adds
 *      implicit cast constructor, removed built-in type operator=, added
 *        castable CIEC types operator=
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

    CIEC_WORD() = default;

    CIEC_WORD(const CIEC_WORD& paValue) :
        CIEC_ANY_BIT(){
      setValueSimple(paValue);
    }

    CIEC_WORD(const CIEC_BYTE& paValue) :
        CIEC_ANY_BIT(){
      setValueSimple(paValue);
    }

    explicit CIEC_WORD(TForteWord pa_cValue) {
      setTUINT16(pa_cValue);
    }

    ~CIEC_WORD() override = default;

    CIEC_WORD& operator =(const CIEC_WORD &paValue) {
      // Simple value assignment - no self assignment check needed
      setValueSimple(paValue);
      return *this;
    }

    CIEC_WORD& operator =(const CIEC_BYTE &paValue) {
      // Simple value assignment - no self assignment check needed
      setValueSimple(paValue);
      return *this;
    }

    /*! \brief Converts CIEC_WORD to elementary word
     *
     *   Conversion operator for converting CIEC_WORD to elementary word
     */
    operator TForteWord() const {
      return getTUINT16();
    }

    EDataTypeID getDataTypeID() const override {
      return CIEC_ANY::e_WORD;
    }

    /*! \brief Partial access within a CIEC_WORD (e.g. [WORD].partial<CIEC_BOOL>(1))
     *
     */
    template <class T> PARTIAL_ACCESS<T, CIEC_WORD> partial(size_t paIndex) {
      return PARTIAL_ACCESS<T,CIEC_WORD>(*this, paIndex);
    }

};

#endif /*_FORTE_WORD_H_*/
