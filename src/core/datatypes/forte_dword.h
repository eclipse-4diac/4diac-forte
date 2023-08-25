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
 *    Thomas Strasser, Ingomar Müller, Alois Zoitl, Gerhard Ebenhofer
 *    Ingo Hegny, Monika Wenger
 *      - initial implementation and rework communication infrastructure
 *    Ernst Blecha - add multibit partial access
 *    Martin Melik Merkumians - changes multibit access index from template
 *      parameter to call argument, make TForteDWord constructor explicit, adds
 *      implicit cast constructor, removed built-in type operator=, added
 *      castable CIEC types operator=
 *    Martin Jobst - add user-defined literal
 *******************************************************************************/
#ifndef _FORTE_DWORD_H_
#define _FORTE_DWORD_H_

#include "forte_any_bit.h"
#include "forte_bool.h"
#include "forte_byte.h"
#include "forte_word.h"

#include "forte_any_int.h"
#include "forte_any_bit_partial.h"

#include <limits>

/*!\ingroup COREDTS CIEC_DWORD represents the dword data type according to IEC 61131.
 */
class CIEC_DWORD : public CIEC_ANY_BIT {
  DECLARE_FIRMWARE_DATATYPE(DWORD)

  public:
    typedef TForteDWord TValueType;

    static constexpr TValueType scmMinVal = std::numeric_limits<CIEC_DWORD::TValueType>::min();
    static constexpr TValueType scmMaxVal = std::numeric_limits<CIEC_DWORD::TValueType>::max();

    CIEC_DWORD() = default;

    CIEC_DWORD(const CIEC_DWORD& paValue) :
        CIEC_ANY_BIT() {
      setValueSimple(paValue);
    }

    CIEC_DWORD(const CIEC_WORD& paValue) :
        CIEC_ANY_BIT() {
      setValueSimple(paValue);
    }

    CIEC_DWORD(const CIEC_BYTE& paValue) :
        CIEC_ANY_BIT() {
      setValueSimple(paValue);
    }

    CIEC_DWORD(const CIEC_BOOL &paValue) : CIEC_ANY_BIT() {
      setValueSimple(paValue);
    }

    explicit CIEC_DWORD(const TValueType paValue) {
      setTUINT32(paValue);
    }

    ~CIEC_DWORD() override = default;

    CIEC_DWORD& operator =(const CIEC_DWORD &paValue) {
      // Simple value assignment - no self assignment check needed
      setValueSimple(paValue);
      return *this;
    }

    CIEC_DWORD& operator =(const CIEC_WORD &paValue) {
      // Simple value assignment - no self assignment check needed
      setValueSimple(paValue);
      return *this;
    }

    CIEC_DWORD& operator =(const CIEC_BYTE &paValue) {
      // Simple value assignment - no self assignment check needed
      setValueSimple(paValue);
      return *this;
    }

    CIEC_DWORD &operator=(const CIEC_BOOL &paValue) {
      // Simple value assignment - no self assignment check needed
      setValueSimple(paValue);
      return *this;
    }

    /*! \brief Converts CIEC_DWORD to elementary word
     *
     *   Conversion operator for converting CIEC_DWORD to elementary word
     */
    operator TForteDWord() const {
      return getTUINT32();
    }

    /*! \brief calculates buffer size needed for toString conversion
     */
    size_t getToStringBufferSize() const override;

    EDataTypeID getDataTypeID() const override {
      return CIEC_ANY::e_DWORD;
    }

    /*! \brief Partial access within a CIEC_DWORD (e.g. [DWORD].partial<CIEC_BOOL>(1))
     *
     */
    template <class T> CIEC_ANY_BIT_PARTIAL<T, CIEC_DWORD> partial(size_t paIndex) {
      return CIEC_ANY_BIT_PARTIAL<T, CIEC_DWORD>(*this, paIndex);
    }

    template <class T> CIEC_ANY_BIT_PARTIAL<T, CIEC_DWORD> partial(const CIEC_ANY_INT& paIndex){
      size_t index = paIndex.getUnsignedValue();
      return CIEC_ANY_BIT_PARTIAL<T, CIEC_DWORD>(*this, index);
    }
};

inline CIEC_DWORD operator ""_DWORD(unsigned long long int paValue) {
  return CIEC_DWORD(static_cast<CIEC_DWORD::TValueType>(paValue));
}

#endif /*_FORTE_DWORD_H_*/
