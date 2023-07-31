/*******************************************************************************
 * Copyright (c) 2005, 2023 Profactor GmbH, ACIN, fortiss GmbH
 *                          Martin Erich Jobst
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
 *    Martin Jobst
 *      - add ANY_SIGNED based on ANY_INT
 *******************************************************************************/
#pragma once

#include "forte_any_int.h"

/*!\ingroup COREDTS CIEC_ANY_SIGNED represents all signed integer data types according to IEC 61131.

 */
class CIEC_ANY_SIGNED : public CIEC_ANY_INT {
  public:
    CIEC_ANY_SIGNED() = default;

    explicit CIEC_ANY_SIGNED(TLargestIntValueType paVal) {
      setSignedValue(paVal);
    }

    ~CIEC_ANY_SIGNED() override = default;

    CIEC_ANY *clone(TForteByte *) const override {
      DEVLOG_ERROR("Attempt to call clone on CIEC_ANY_SIGNED\n");
      return nullptr;
    }

    size_t getToStringBufferSize() const override {
      DEVLOG_ERROR("Calling CIEC_ANY_SIGNED getToStringBufferSize is invalid!\n");
      return 0;
    }
};

