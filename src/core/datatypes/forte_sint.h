/*******************************************************************************
 * Copyright (c) 2005, 2023 Profactor GmbH, ACIN
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
 *    Ingo Hegny, Martin Melik Merkumians
 *      - initial implementation and rework communication infrastructure
 *    Martin Melik Merkumians - make TForteInt8 constructor explicit,
 *      removed built-in type operator=
 *    Martin Jobst - add user-defined literal
 *                 - add ANY_SIGNED
 *******************************************************************************/
#ifndef _FORTE_SINT_H_
#define _FORTE_SINT_H_

#include "forte_any_signed.h"
#include <limits>

/*!\ingroup COREDTS CIEC_SINT represents the SINT data type according to IEC 61131.
 */
class CIEC_SINT final : public CIEC_ANY_SIGNED {
  DECLARE_FIRMWARE_DATATYPE(SINT)

  public:
    using TValueType = TForteInt8;
    [[deprecated("Please use the corresponding numeric_limits template")]]
    constexpr static size_t scmBitLength = 8U;
    [[deprecated("Please use the corresponding numeric_limits template")]]
    static constexpr TValueType scmMinVal = std::numeric_limits<TValueType>::min();
    [[deprecated("Please use the corresponding numeric_limits template")]]
    static constexpr TValueType scmMaxVal = std::numeric_limits<TValueType>::max();

    CIEC_SINT() = default;

    CIEC_SINT(const CIEC_SINT& paValue) :
            CIEC_ANY_SIGNED(){
      setValueSimple(paValue);
    }

    explicit CIEC_SINT(const CIEC_ANY_INT& paValue) :
            CIEC_ANY_SIGNED() {
      setValueSimple(paValue);
    }

    explicit CIEC_SINT(const TValueType paValue) {
      setTINT8(paValue);
    }

    ~CIEC_SINT() override = default;

    CIEC_SINT& operator =(const CIEC_SINT &paValue){
      // Simple value assignment - no self assignment check needed
      setValueSimple(paValue);
      return *this;
    }

    template <typename T, std::enable_if_t<std::is_same_v<typename forte::core::mpl::implicit_cast_t<T, CIEC_SINT>, CIEC_SINT>, int> = 0>
    CIEC_SINT &operator=(const T &paValue) {
      setValueSimple(paValue);
      return *this;
    }

    CIEC_SINT operator-() const {
      return CIEC_SINT(static_cast<CIEC_SINT::TValueType>(-1) * static_cast<CIEC_SINT::TValueType>(*this));
    }

    /*! \brief Converts CIEC_SINT to elementary 8 bit integer
     *
     *   Conversion operator for converting CIEC_SINT to elementary 8 bit integer
     */
    explicit operator TForteInt8() const{
      return getTINT8();
    }

    /*! \brief calculates buffer size needed for toString conversion
     */
    size_t getToStringBufferSize() const override {
      return sizeof("-128");
    }

    EDataTypeID getDataTypeID() const override {
      return CIEC_ANY::e_SINT;
    }
};

inline CIEC_SINT operator ""_SINT(unsigned long long int paValue) {
  return CIEC_SINT(static_cast<CIEC_SINT::TValueType>(paValue));
}

namespace std {
  template <>
  struct numeric_limits<CIEC_SINT> : public forte::templates::numeric_limits<CIEC_SINT> {
    static constexpr size_t bitLength = 8U;
  };
}

#endif /*_FORTE_SINT_H_*/
