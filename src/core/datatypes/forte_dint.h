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
 *      - initial implementation and rework communication infrastructure
 *    Martin Melik Merkumians - make TForteInt8 constructor explicit, add
 *      implicit cast constructors, removed built-in type operator=, added
 *        castable CIEC types operator=
 *    Martin Jobst - add user-defined literal
 *                 - add ANY_SIGNED
 *******************************************************************************/
#ifndef _FORTE_DINT_H_
#define _FORTE_DINT_H_

#include "forte_any_signed.h"
#include "forte_sint.h"
#include "forte_int.h"
#include "forte_usint.h"
#include "forte_uint.h"
#include <limits>

/*!\ingroup COREDTS CIEC_DINT represents the dint data type according to IEC 61131.
 */
class CIEC_DINT final : public CIEC_ANY_SIGNED {
  DECLARE_FIRMWARE_DATATYPE(DINT)

  public:
    typedef TForteInt32 TValueType;

    static const TValueType scmMinVal;
    static const TValueType scmMaxVal;

    CIEC_DINT() = default;

    CIEC_DINT(const CIEC_DINT& paValue) :
            CIEC_ANY_SIGNED() {
      setValueSimple(paValue);
    }

    CIEC_DINT(const CIEC_INT& paValue) :
            CIEC_ANY_SIGNED() {
      setValueSimple(paValue);
    }

    CIEC_DINT(const CIEC_UINT& paValue) :
            CIEC_ANY_SIGNED() {
      setValueSimple(paValue);
    }

    CIEC_DINT(const CIEC_SINT& paValue) :
            CIEC_ANY_SIGNED() {
      setValueSimple(paValue);
    }

    CIEC_DINT(const CIEC_USINT& paValue) :
            CIEC_ANY_SIGNED() {
      setValueSimple(paValue);
    }

    explicit CIEC_DINT(const CIEC_ANY_INT& paValue) :
            CIEC_ANY_SIGNED() {
      setValueSimple(paValue);
    }

    explicit CIEC_DINT(const TValueType paValue) {
      setTINT32(paValue);
    }

    ~CIEC_DINT() override = default;

    CIEC_DINT& operator =(const CIEC_DINT &paValue){
      // Simple value assignment - no self assignment check needed
      setValueSimple(paValue);
      return *this;
    }

    template <typename T, std::enable_if_t<std::is_same_v<typename forte::core::mpl::implicit_cast_t<T, CIEC_DINT>, CIEC_DINT>, int> = 0>
    CIEC_DINT &operator=(const T &paValue) {
      setValueSimple(paValue);
      return *this;
    }

    CIEC_DINT operator-() const {
      return CIEC_DINT(-1 * static_cast<CIEC_DINT::TValueType>(*this));
    }
    
    /*! \brief Converts CIEC_DINT to elementary byte
     *
     *   Conversion operator for converting CIEC_DINT to elementary 32 bit integer
     */
    explicit operator TForteInt32() const{
      return getTINT32();
    }

    /*! \brief calculates buffer size needed for toString conversion
     */
    size_t getToStringBufferSize() const override;

    EDataTypeID getDataTypeID() const override {
      return CIEC_ANY::e_DINT;
    }
};

inline CIEC_DINT operator ""_DINT(unsigned long long int paValue) {
  return CIEC_DINT(static_cast<CIEC_DINT::TValueType>(paValue));
}

#endif /*_FORTE_DINT_H_*/
