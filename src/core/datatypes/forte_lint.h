/*******************************************************************************
 * Copyright (c) 2009, 2023 ACIN
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
 *    Monika Wenger, Alois Zoitl,
 *      - initial implementation and rework communication infrastructure
 *    Martin Melik Merkumians - make TForteInt64 constructor explicit,
 *      adds casts constructors, removed built-in type operator=, added
 *      castable CIEC types operator=
 *    Martin Jobst - add user-defined literal
 *                 - add ANY_SIGNED
 *******************************************************************************/
#ifndef _FORTE_LINT_H_
#define _FORTE_LINT_H_

#include "forte_any_signed.h"
#include "forte_dint.h"
#include "forte_int.h"
#include "forte_sint.h"
#include "forte_udint.h"
#include "forte_uint.h"
#include "forte_usint.h"
#include <limits>

/*!\ingroup COREDTS CIEC_LINT represents the dint data type according to IEC 61131.
 */
class CIEC_LINT final : public CIEC_ANY_SIGNED {
  DECLARE_FIRMWARE_DATATYPE(LINT)

  public:
    typedef TForteInt64 TValueType;

    static const TValueType scmMinVal;
    static const TValueType scmMaxVal;

    CIEC_LINT() = default;

    CIEC_LINT(const CIEC_LINT& paValue) :
            CIEC_ANY_SIGNED() {
      setValueSimple(paValue);
    }

    CIEC_LINT(const CIEC_DINT& paValue) :
            CIEC_ANY_SIGNED() {
      setValueSimple(paValue);
    }

    CIEC_LINT(const CIEC_UDINT& paValue) :
            CIEC_ANY_SIGNED() {
      setValueSimple(paValue);
    }

    CIEC_LINT(const CIEC_INT& paValue) :
            CIEC_ANY_SIGNED() {
      setValueSimple(paValue);
    }

    CIEC_LINT(const CIEC_UINT& paValue) :
            CIEC_ANY_SIGNED() {
      setValueSimple(paValue);
    }

    CIEC_LINT(const CIEC_SINT& paValue) :
            CIEC_ANY_SIGNED() {
      setValueSimple(paValue);
    }

    CIEC_LINT(const CIEC_USINT& paValue) :
            CIEC_ANY_SIGNED() {
      setValueSimple(paValue);
    }

    explicit CIEC_LINT(const CIEC_ANY_INT& paValue) :
            CIEC_ANY_SIGNED() {
      setValueSimple(paValue);
    }

    explicit CIEC_LINT(const TValueType paValue) {
      setTINT64(paValue);
    }

    ~CIEC_LINT() override = default;

    CIEC_LINT& operator =(const CIEC_LINT &paValue){
      // Simple value assignment - no self assignment check needed
      setValueSimple(paValue);
      return *this;
    }

    template <typename T, std::enable_if_t<std::is_same_v<typename forte::core::mpl::implicit_cast_t<T, CIEC_LINT>, CIEC_LINT>, int> = 0>
    CIEC_LINT &operator=(const T &paValue) {
      setValueSimple(paValue);
      return *this;
    }

    CIEC_LINT operator-() const {
      return CIEC_LINT(-1 * static_cast<CIEC_LINT::TValueType>(*this));
    }

    /*! \brief Converts CIEC_LINT to elementary byte
     *
     *   Conversion operator for converting CIEC_LINT to elementary 64 bit integer
     */
    explicit operator TForteInt64() const{
      return getTINT64();
    }

    /*! \brief calculates buffer size needed for toString conversion
     */
    size_t getToStringBufferSize() const override;

    EDataTypeID getDataTypeID() const override {
      return CIEC_ANY::e_LINT;
    }
};

inline CIEC_LINT operator ""_LINT(unsigned long long int paValue) {
  return CIEC_LINT(static_cast<CIEC_LINT::TValueType>(paValue));
}

#endif /*_FORTE_LINT_H_*/
