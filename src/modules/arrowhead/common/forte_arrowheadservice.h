/*******************************************************************************
 * Copyright (c) 2018, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Jose Cabral - initial implementation
 *******************************************************************************/

#ifndef _FORTE_ARROWHEADSERVICE_H_
#define _FORTE_ARROWHEADSERVICE_H_

#include "forte_struct.h"
#include "forte_wstring.h"
#include "forte_dint.h"
#include "forte_array.h"

class CIEC_ArrowheadService : public CIEC_STRUCT {
  DECLARE_FIRMWARE_DATATYPE(ArrowheadService)

  public:
    CIEC_ArrowheadService();

    virtual ~CIEC_ArrowheadService() {
    }

    CIEC_WSTRING &serviceDefinition() {
      return *static_cast<CIEC_WSTRING*>(&getMembers()[0]);
    }

    CIEC_ARRAY *interfaces_array() {
      return static_cast<CIEC_ARRAY *>(&getMembers()[1]); //the first element marks the start of the array
    }

    CIEC_WSTRING *interfaces() {
      return static_cast<CIEC_WSTRING*>((*static_cast<CIEC_ARRAY *>(&getMembers()[1]))[0]); //the first element marks the start of the array
    }

    CIEC_WSTRING *serviceMetadata() {
      return static_cast<CIEC_WSTRING*>((*static_cast<CIEC_ARRAY *>(&getMembers()[2]))[0]); //the first element marks the start of the array
    }

  private:
    static const CStringDictionary::TStringId scmElementTypes[];
    static const CStringDictionary::TStringId scmElementNames[];
};

#endif //_FORTE_ARROWHEADSERVICE_H_
