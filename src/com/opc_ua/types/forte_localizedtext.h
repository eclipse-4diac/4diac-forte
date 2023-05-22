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

    CIEC_STRING var_locale;
    CIEC_STRING var_text;

    size_t getStructSize() const override {
      return 2;
    }

    const CStringDictionary::TStringId* elementNames() const override {
      return scmElementNames;
    }

    CStringDictionary::TStringId getStructTypeNameID() const override;

    CIEC_ANY *getMember(size_t paMemberIndex) override;
    const CIEC_ANY *getMember(size_t paMemberIndex) const override;

  private:
    static const CStringDictionary::TStringId scmElementNames[];
};

#endif //_FORTE_LOCALIZEDTEXT_H_
