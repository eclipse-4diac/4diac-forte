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

#ifndef _FORTE_ORCHESTRATIONFORM_H_
#define _FORTE_ORCHESTRATIONFORM_H_

#include "forte_struct.h"
#include "forte_arrowheadsystem.h"
#include "forte_arrowheadservice.h"
#include "forte_wstring.h"
#include "forte_array.h"

class CIEC_OrchestrationForm : public CIEC_STRUCT {
  DECLARE_FIRMWARE_DATATYPE(OrchestrationForm)

  public:
    CIEC_OrchestrationForm();

    virtual ~CIEC_OrchestrationForm() {
    }

    CIEC_ArrowheadService &service() {
      return *static_cast<CIEC_ArrowheadService*>(&getMembers()[0]);
    }

    CIEC_ArrowheadSystem &provider() {
      return *static_cast<CIEC_ArrowheadSystem*>(&getMembers()[1]);
    }

    CIEC_WSTRING &serviceURI() {
      return *static_cast<CIEC_WSTRING*>(&getMembers()[2]);
    }

    CIEC_WSTRING &instruction() {
      return *static_cast<CIEC_WSTRING *>(&getMembers()[3]);
    }

    CIEC_WSTRING &authorizationToken() {
      return *static_cast<CIEC_WSTRING *>(&getMembers()[4]);
    }

    CIEC_WSTRING &signature() {
      return *static_cast<CIEC_WSTRING *>(&getMembers()[5]);
    }

    CIEC_WSTRING *warnings() {
      return static_cast<CIEC_WSTRING*>((*static_cast<CIEC_ARRAY *>(&getMembers()[6]))[0]); //the first element marks the start of the array
    }

  private:
    static const CStringDictionary::TStringId scmElementTypes[];
    static const CStringDictionary::TStringId scmElementNames[];
};

#endif //_FORTE_ORCHESTRATIONFORM_H_

