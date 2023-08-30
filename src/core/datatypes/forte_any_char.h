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
#ifndef _ANY_CHAR_H_
#define _ANY_CHAR_H_

#include "forte_any_chars.h"
#include <ctype.h>
#include <string_utils.h>

/*!\ingroup COREDTS CIEC_ANY_CHAR represents ANY_CHAR data types according to
 *  IEC 61131.
 */
class CIEC_ANY_CHAR : public CIEC_ANY_CHARS {
  public:
    CIEC_ANY_CHAR(const CIEC_ANY_CHAR &paVal) :
        CIEC_ANY_CHARS() {
      setLargestUInt(paVal.getLargestUInt());
    }

    ~CIEC_ANY_CHAR() override = default;

    EDataTypeID getDataTypeID() const override {
      return CIEC_ANY::e_ANY;
    }

    // int toString(char* paValue, size_t paBufferSize) const override;
    // int fromString(const char *paValue) override;

  protected:
    CIEC_ANY_CHAR() = default;

};

#endif /*_ANY_CHAR_H_*/
