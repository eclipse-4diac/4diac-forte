/*******************************************************************************
 * Copyright (c) 2019 fortiss GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Jose Cabral - initial implementation
 *******************************************************************************/

#ifndef _FORTE_LOCALIZEDTEXT_H_
#define _FORTE_LOCALIZEDTEXT_H_

#include "forte_struct.h"
#include "forte_string.h"

class CIEC_LocalizedText : public CIEC_STRUCT {
  DECLARE_FIRMWARE_DATATYPE(LocalizedText)

  public:
    CIEC_LocalizedText();

    virtual ~CIEC_LocalizedText() {
    }

    CIEC_STRING &locale() {
      return *static_cast<CIEC_STRING*>(&getMembers()[0]);
    }

    CIEC_STRING &text() {
      return *static_cast<CIEC_STRING*>(&getMembers()[1]);
    }

  private:
    static const CStringDictionary::TStringId scmElementTypes[];
    static const CStringDictionary::TStringId scmElementNames[];
};

#endif //_FORTE_LOCALIZEDTEXT_H_
