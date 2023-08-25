/*******************************************************************************
 * Copyright (c) 2010, 2023 ACIN
 *                          Johannes Kepler University Linz
 *                          Primetals Technologies Austria GmbH
 *                          Martin Erich Jobst
 *
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
 *    Martin Jobst - add user-defined literal
 *******************************************************************************/
#ifndef _FORTE_LWORD_H_
#define _FORTE_LWORD_H_

#include "forte_any_bit.h"
#include "forte_bool.h"
#include "forte_byte.h"
#include "forte_word.h"
#include "forte_dword.h"

#include "forte_any_int.h"
#include "forte_any_bit_partial.h"

#include <limits>

/*!\ingroup COREDTS CIEC_DWORD represents the dword data type according to IEC 61131.
 */

class CIEC_LWORD : public CIEC_ANY_BIT{
  DECLARE_FIRMWARE_DATATYPE(LWORD)

  public:
    using TValueType = TForteLWord;

    static constexpr TValueType scmMinVal = std::numeric_limits<CIEC_LWORD::TValueType>::min();
    static constexpr TValueType scmMaxVal = std::numeric_limits<CIEC_LWORD::TValueType>::max();

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

    explicit CIEC_LWORD(const TValueType paValue) {
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

    /*! \brief calculates buffer size needed for toString conversion
     */
    size_t getToStringBufferSize() const override;

    EDataTypeID getDataTypeID() const override {
      return CIEC_ANY::e_LWORD;
    }

    /*! \brief Partial access within a CIEC_LWORD (e.g. [LWORD].partial<CIEC_BOOL>(1))
     *
     */
    template <class T> CIEC_ANY_BIT_PARTIAL<T, CIEC_LWORD> partial(size_t paIndex){
      return CIEC_ANY_BIT_PARTIAL<T, CIEC_LWORD>(*this, paIndex);
    }

    template <class T> CIEC_ANY_BIT_PARTIAL<T, CIEC_LWORD> partial(const CIEC_ANY_INT& paIndex){
      size_t index = paIndex.getUnsignedValue();
      return CIEC_ANY_BIT_PARTIAL<T, CIEC_LWORD>(*this, index);
    }

};

inline CIEC_LWORD operator ""_LWORD(unsigned long long int paValue) {
  return CIEC_LWORD(static_cast<CIEC_LWORD::TValueType>(paValue));
}

#endif /*_FORTE_LWORD_H_*/
