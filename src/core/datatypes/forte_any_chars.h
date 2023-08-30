/*******************************************************************************
 * Copyright (c) 2022 Primetals Technologies Austria GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Martin Melik Merkumians
 *      - initial implementation and rework communication infrastructure
  *******************************************************************************/
#ifndef _ANY_CHARS_H_
#define _ANY_CHARS_H_

#include "forte_any_elementary.h"
#include <ctype.h>
#include <string_utils.h>

/*!\ingroup COREDTS CIEC_ANY_CHARS represents ANY_CHARS data types according to
 *  IEC 61131.
 */
class CIEC_ANY_CHARS : public CIEC_ANY_ELEMENTARY {
  public:
    CIEC_ANY_CHARS(const CIEC_ANY_CHARS &paVal) :
        CIEC_ANY_ELEMENTARY() {
      setLargestUInt(paVal.getLargestUInt());
    }

    ~CIEC_ANY_CHARS() override = default;

    EDataTypeID getDataTypeID() const override {
      return CIEC_ANY::e_ANY;
    }

    // int toString(char* paValue, size_t paBufferSize) const override;
    // int fromString(const char *paValue) override;

  protected:
    CIEC_ANY_CHARS() = default;
};

#endif /*_ANY_CHARS_H_*/
