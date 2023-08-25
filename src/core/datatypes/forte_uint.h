/*******************************************************************************
 * Copyright (c) 2005, 2023 Profactor GmbH, ACIN, fortiss GmbH
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
 *    Thomas Strasser, Ingomar Müller, Alois Zoitl, Gerhard Ebenhofer,
 *    Ingo Hegny, Monika Wenger
 *      - initial implementation and rework communication infrastructure,
 *    Martin Melik Merkumains - make TForteUInt16 constructor explicit,
 *        removed built-in type operator=, added castable CIEC types operator=
 *    Martin Jobst - add user-defined literal
 *******************************************************************************/
#ifndef _FORTE_UINT_H_
#define _FORTE_UINT_H_

#include "forte_any_unsigned.h"
#include "forte_usint.h"
#include <limits>

/*!\ingroup COREDTS CIEC_UINT represents the uint data type according to IEC 61131.
 */
class CIEC_UINT final : public CIEC_ANY_UNSIGNED {
  DECLARE_FIRMWARE_DATATYPE(UINT)

  public:
    typedef TForteUInt16 TValueType;

    static const TValueType scmMinVal = 0;
    static const TValueType scmMaxVal;

    CIEC_UINT() = default;

    CIEC_UINT(const CIEC_UINT& paValue) :
            CIEC_ANY_UNSIGNED(){
      setValueSimple(paValue);
    }

    CIEC_UINT(const CIEC_USINT& paValue) :
            CIEC_ANY_UNSIGNED(){
      setValueSimple(paValue);
    }

    template <typename T, std::enable_if_t<std::is_same_v<typename forte::core::mpl::implicit_cast_t<T, CIEC_UINT>, CIEC_UINT>, int> = 0>
    explicit CIEC_UINT(const T& paValue) :
            CIEC_ANY_UNSIGNED(){
      setValueSimple(paValue);
    }

    explicit CIEC_UINT(const TValueType paValue) {
      setTUINT16(paValue);
    }

    ~CIEC_UINT() override = default;

    CIEC_UINT& operator =(const CIEC_UINT &paValue){
      // Simple value assignment - no self assignment check needed
      setValueSimple(paValue);
      return *this;
    }

    template <typename T, std::enable_if_t<std::is_same_v<typename forte::core::mpl::implicit_cast_t<T, CIEC_UINT>, CIEC_UINT>, int> = 0>
    CIEC_UINT &operator=(const T &paValue) {
      setValueSimple(paValue);
      return *this;
    }

    /*! \brief Converts CIEC_UDINT to elementary unsigned 16 bit integer
     *
     *   Conversion operator for converting CIEC_UDINT to elementary unsigned 16 bit integer
     */
    explicit operator TForteUInt16() const{
      return getTUINT16();
    }

    /*! \brief calculates buffer size needed for toString conversion
     */
    size_t getToStringBufferSize() const override;

    EDataTypeID getDataTypeID() const override {
      return CIEC_ANY::e_UINT;
    }
};

inline CIEC_UINT operator ""_UINT(unsigned long long int paValue) {
  return CIEC_UINT(static_cast<CIEC_UINT::TValueType>(paValue));
}

#endif /*_FORTE_UINT_H_*/
