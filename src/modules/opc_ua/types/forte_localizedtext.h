/*******************************************************************************
 * Copyright (c) 2019 fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
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

    CIEC_STRING &text() {
      return *static_cast<CIEC_STRING*>(&getMembers()[0]);
    }

    CIEC_STRING &locale() {
      return *static_cast<CIEC_STRING*>(&getMembers()[1]);
    }

  private:
    static const CStringDictionary::TStringId scmElementTypes[];
    static const CStringDictionary::TStringId scmElementNames[];
};

#endif //_FORTE_LOCALIZEDTEXT_H_
