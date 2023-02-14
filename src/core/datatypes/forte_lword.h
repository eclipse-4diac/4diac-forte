/*******************************************************************************
 * Copyright (c) 2010 - 2013 ACIN
 *               2020 Johannes Kepler University Linz
 *               2022 Primetals Technologies Austria GmbH
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
 *    Martin Melik Merkumians - changes multibit access index from template
 *      parameter to call argument, make TForteLWord constructor explicit, adds
 *      implicit cast constructor, removed built-in type operator=, added
 *        castable CIEC types operator=
  *******************************************************************************/
#ifndef _FORTE_LWORD_H_
#define _FORTE_LWORD_H_

#include "forte_any_bit.h"
#include "forte_bool.h"
#include "forte_byte.h"
#include "forte_word.h"
#include "forte_dword.h"
#include <limits>

#include "forte_any_int.h"

/*!\ingroup COREDTS CIEC_DWORD represents the dword data type according to IEC 61131.
 */

class CIEC_LWORD : public CIEC_ANY_BIT{
  DECLARE_FIRMWARE_DATATYPE(LWORD)

  public:
    typedef TForteLWord TValueType;

    static const TValueType scm_nMinVal = 0;
    static const TValueType scm_nMaxVal;

    CIEC_LWORD() = default;

    CIEC_LWORD(const CIEC_LWORD& paValue) :
        CIEC_ANY_BIT(){
      setValueSimple(paValue);
    }

    CIEC_LWORD(const CIEC_DWORD& paValue) :
        CIEC_ANY_BIT(){
      setValueSimple(paValue);
    }

    CIEC_LWORD(const CIEC_WORD& paValue) :
        CIEC_ANY_BIT(){
      setValueSimple(paValue);
    }

    CIEC_LWORD(const CIEC_BYTE& paValue) :
        CIEC_ANY_BIT(){
      setValueSimple(paValue);
    }

    CIEC_LWORD(const CIEC_BOOL &paValue) : CIEC_ANY_BIT() {
      setValueSimple(paValue);
    }

    explicit CIEC_LWORD(TForteLWord paValue){
      setTUINT64(paValue);
    }
    ~CIEC_LWORD() override = default;

    CIEC_LWORD& operator =(const CIEC_LWORD &paValue){
      // Simple value assignment - no self assignment check needed
      setValueSimple(paValue);
      return *this;
    }

    CIEC_LWORD& operator =(const CIEC_DWORD &paValue){
      // Simple value assignment - no self assignment check needed
      setValueSimple(paValue);
      return *this;
    }

    CIEC_LWORD& operator =(const CIEC_WORD &paValue){
      // Simple value assignment - no self assignment check needed
      setValueSimple(paValue);
      return *this;
    }

    CIEC_LWORD& operator =(const CIEC_BYTE &paValue){
      // Simple value assignment - no self assignment check needed
      setValueSimple(paValue);
      return *this;
    }

    CIEC_LWORD &operator=(const CIEC_BOOL &paValue) {
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

    EDataTypeID getDataTypeID() const override {
      return CIEC_ANY::e_LWORD;
    }

    /*! \brief Partial access within a CIEC_LWORD (e.g. [LWORD].partial<CIEC_BOOL>(1))
     *
     */
    template <class T> PARTIAL_ACCESS<T, CIEC_LWORD> partial(size_t paIndex){
      return PARTIAL_ACCESS<T,CIEC_LWORD>(*this, paIndex);
    }

    template <class T> PARTIAL_ACCESS<T, CIEC_LWORD> partial(const CIEC_ANY_INT& paIndex){
      size_t index = paIndex.getUnsignedValue();
      return PARTIAL_ACCESS<T,CIEC_LWORD>(*this, index);
    }

};

#endif /*_FORTE_LWORD_H_*/
