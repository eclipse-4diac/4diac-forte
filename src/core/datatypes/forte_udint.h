/*******************************************************************************
 * Copyright (c) 2005 - 2013 Profactor GmbH, ACIN
 *               2022 Primetals Technologies Austria GmbH
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
 *    Martin Melik Merkumians - make TForteUInt32 constructor explicit,
 *      removed built-in type operator=, added castable CIEC types operator=
 *******************************************************************************/
#ifndef _FORTE_UDINT_H_
#define _FORTE_UDINT_H_

#include "forte_any_int.h"
#include "forte_uint.h"
#include "forte_usint.h"
#include <limits>

/*!\ingroup COREDTS CIEC_UDINT represents the udint data type according to IEC 61131.
 */
class CIEC_UDINT : public CIEC_ANY_INT{
  DECLARE_FIRMWARE_DATATYPE(UDINT)

  public:
    typedef TForteUInt32 TValueType;

    static const TValueType scm_nMinVal = 0;
    static const TValueType scm_nMaxVal;

    CIEC_UDINT() = default;

    CIEC_UDINT(const CIEC_UDINT& paValue) :
        CIEC_ANY_INT() {
      setValueSimple(paValue);
    }

    CIEC_UDINT(const CIEC_UINT& paValue) :
        CIEC_ANY_INT() {
      setValueSimple(paValue);
    }

    CIEC_UDINT(const CIEC_USINT& paValue) :
        CIEC_ANY_INT() {
      setValueSimple(paValue);
    }

    explicit CIEC_UDINT(const CIEC_ANY_INT& paValue) :
        CIEC_ANY_INT() {
      setValueSimple(paValue);
    }

    explicit CIEC_UDINT(TForteUInt32 paValue){
      setTUINT32(paValue);
    }

    ~CIEC_UDINT() override = default;

    CIEC_UDINT& operator =(const CIEC_UDINT &paValue){
      // Simple value assignment - no self assignment check needed
      setValueSimple(paValue);
      return *this;
    }

    template <typename T, std::enable_if_t<std::is_same_v<typename forte::core::mpl::implicit_cast_t<T, CIEC_UDINT>, CIEC_UDINT>, int> = 0>
    CIEC_UDINT &operator=(const T &paValue) {
      setValueSimple(paValue);
      return *this;
    }

    /*! \brief Converts CIEC_UDINT to elementary unsigned 32 bit integer
     *
     *   Conversion operator for converting CIEC_UDINT to elementary unsigned 32 bit integer
     */
    explicit operator TForteUInt32() const{
      return getTUINT32();
    }

    EDataTypeID getDataTypeID() const override {
      return CIEC_ANY::e_UDINT;
    }
};

#endif /*_FORTE_UDINT_H_*/
