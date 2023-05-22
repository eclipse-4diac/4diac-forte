/*******************************************************************************
 * Copyright (c) 2005 - 2013 Profactor GmbH, ACIN, fortiss GmbH
 *               2022 Primetals Technologies Austria GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Thomas Strasser, Ingomar Müller, Alois Zoitl, Gerhard Ebenhofer,
 *    Ingo Hegny, Monika Wenger,
 *      - initial implementation and rework communication infrastructure
 *    Martin Melik Merkumians - make TForteUInt8 constructor explicit,
 *        removed built-in type operator=
 *******************************************************************************/
#ifndef _FORTE_USINT_H_
#define _FORTE_USINT_H_

#include "forte_any_int.h"
#include <limits>

/*!\ingroup COREDTS CIEC_USINT represents the usint data type according to IEC 61131.
 */
class CIEC_USINT final : public CIEC_ANY_INT{
  DECLARE_FIRMWARE_DATATYPE(USINT)

  public:
    typedef TForteUInt8 TValueType;

    static const TValueType scm_nMinVal = 0;
    static const TValueType scm_nMaxVal;

    CIEC_USINT() = default;

    CIEC_USINT(const CIEC_USINT& pa_roValue) :
        CIEC_ANY_INT() {
      setValueSimple(pa_roValue);
    }

    explicit CIEC_USINT(const CIEC_ANY_INT& paValue) :
        CIEC_ANY_INT() {
      setValueSimple(paValue);
    }

    explicit CIEC_USINT(TForteUInt8 pa_nValue){
      setTUINT8(pa_nValue);
    }

    ~CIEC_USINT() override = default;

    CIEC_USINT& operator =(const CIEC_USINT &pa_roValue){
      setValueSimple(pa_roValue);
      return *this;
    }

    template <typename T, std::enable_if_t<std::is_same_v<typename forte::core::mpl::implicit_cast_t<T, CIEC_USINT>, CIEC_USINT>, int> = 0>
    CIEC_USINT &operator=(const T &paValue) {
      setValueSimple(paValue);
      return *this;
    }

    /*! \brief Converts CIEC_UDINT to elementary unsigned 16 bit integer
     *
     *   Conversion operator for converting CIEC_UDINT to elementary unsigned 16 bit integer
     */
    explicit operator TForteUInt8() const{
      return getTUINT8();
    }

    /*! \brief calculates buffer size needed for toString conversion
     */
    size_t getToStringBufferSize() const override;

    EDataTypeID getDataTypeID() const override {
      return CIEC_ANY::e_USINT;
    }
};

#endif /*_FORTE_USINT_H_*/
