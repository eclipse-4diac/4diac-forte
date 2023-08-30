/*******************************************************************************
 * Copyright (c) 2005 - 2014 Profactor GmbH, ACIN, fortiss GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Thomas Strasser, Ingomar Müller, Alois Zoitl, Monika Wenger,
 *    Martin Melik Markumians
 *      - initial implementation and rework communication infrastructure
 *******************************************************************************/
#ifndef _ANY_INT_H_
#define _ANY_INT_H_

#include "forte_any_num.h"

/*!\ingroup COREDTS CIEC_ANY_INT represents all integer data types according to IEC 61131.

 */
class CIEC_ANY_INT : public CIEC_ANY_NUM{
  public:
    explicit CIEC_ANY_INT(TLargestIntValueType paVal) {
      setSignedValue(paVal);
    }

    ~CIEC_ANY_INT() override = default;

    CIEC_ANY *clone(TForteByte *) const override {
      DEVLOG_ERROR("Attempt to call clone on CIEC_ANY_INT\n");
      return nullptr;
    }

    bool isSigned() const {
      return e_LINT >= getDataTypeID();
    }

    TLargestUIntValueType getUnsignedValue() const {
      return getLargestUInt();
    }

    TLargestIntValueType getSignedValue() const {
      return getLargestInt();
    }

     void setSignedValue(TLargestIntValueType mVal) {
       setLargestInt(mVal);
     }

     void setUnsignedValue(TLargestIntValueType mVal) {
       setLargestInt(mVal);
     }

    CIEC_ANY_INT() = default;

    size_t getToStringBufferSize() const {
      DEVLOG_ERROR("Calling CIEC_ANY_INT getToStringBufferSize is invalid!\n");
      return 0;
    }
};

#endif /*_MANY_INT_H_*/
