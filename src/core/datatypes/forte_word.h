/*******************************************************************************
 * Copyright (c) 2005, 2023 Profactor GmbH, ACIN
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
 *    Thomas Strasser, Ingomar Müller, Alois Zoitl, Gerhard Ebenhofer, Ingo Hegny,
 *    Monika Wenger
 *      - initial implementation and rework communication infrastructure
 *    Ernst Blecha - add multibit partial access
 *    Martin Melik Merkumians - changes multibit access index from template
 *      parameter to call argument, make TForteWord constructor explicit, adds
 *      implicit cast constructor, removed built-in type operator=, added
 *        castable CIEC types operator=
 *    Martin Jobst - add user-defined literal
 *******************************************************************************/
#ifndef _FORTE_WORD_H_
#define _FORTE_WORD_H_

#include "forte_any_bit.h"
#include "forte_bool.h"
#include "forte_byte.h"

#include "forte_any_int.h"
#include "forte_any_bit_partial.h"

#include <limits>

/*!\ingroup COREDTS CIEC_WORD represents the word data type according to IEC 61131.
 */
class CIEC_WORD : public CIEC_ANY_BIT{
  DECLARE_FIRMWARE_DATATYPE(WORD)

  public:
    typedef TForteWord TValueType;

    static constexpr TValueType scmMinVal = std::numeric_limits<CIEC_WORD::TValueType>::min();
    static constexpr TValueType scmMaxVal = std::numeric_limits<CIEC_WORD::TValueType>::max();

    CIEC_WORD() = default;

    CIEC_WORD(const CIEC_WORD& paValue) :
        CIEC_ANY_BIT(){
      setValueSimple(paValue);
    }

    CIEC_WORD(const CIEC_BYTE& paValue) :
        CIEC_ANY_BIT(){
      setValueSimple(paValue);
    }

    CIEC_WORD(const CIEC_BOOL &paValue) : CIEC_ANY_BIT() {
      setValueSimple(paValue);
    }

    explicit CIEC_WORD(const TValueType paValue) {
      setTUINT16(paValue);
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

    CIEC_WORD &operator=(const CIEC_BOOL &paValue) {
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

    /*! \brief calculates buffer size needed for toString conversion
     */
    size_t getToStringBufferSize() const override;

    EDataTypeID getDataTypeID() const override {
      return CIEC_ANY::e_WORD;
    }

    /*! \brief Partial access within a CIEC_WORD (e.g. [WORD].partial<CIEC_BOOL>(1))
     *
     */
    template <class T> CIEC_ANY_BIT_PARTIAL<T, CIEC_WORD> partial(size_t paIndex) {
      return CIEC_ANY_BIT_PARTIAL<T, CIEC_WORD>(*this, paIndex);
    }

    template <class T> CIEC_ANY_BIT_PARTIAL<T, CIEC_WORD> partial(const CIEC_ANY_INT& paIndex){
      size_t index = paIndex.getUnsignedValue();
      return CIEC_ANY_BIT_PARTIAL<T, CIEC_WORD>(*this, index);
    }
};

inline CIEC_WORD operator ""_WORD(unsigned long long int paValue) {
  return CIEC_WORD(static_cast<CIEC_WORD::TValueType>(paValue));
}

#endif /*_FORTE_WORD_H_*/
