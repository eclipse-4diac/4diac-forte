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

#ifndef _FORTE_ARROWHEADSYSTEM_H_
#define _FORTE_ARROWHEADSYSTEM_H_

#include "forte_struct.h"
#include "forte_wstring.h"
#include "forte_dint.h"

class CIEC_ArrowheadSystem : public CIEC_STRUCT {
  DECLARE_FIRMWARE_DATATYPE(ArrowheadSystem)

  public:
    CIEC_ArrowheadSystem();

    virtual ~CIEC_ArrowheadSystem() {
    }

    CIEC_WSTRING &systemName() {
      return *static_cast<CIEC_WSTRING*>(&getMembers()[0]);
    }

    CIEC_WSTRING &address() {
      return *static_cast<CIEC_WSTRING*>(&getMembers()[1]);
    }

    CIEC_DINT &port() {
      return *static_cast<CIEC_DINT*>(&getMembers()[2]);
    }

    CIEC_WSTRING &authenticationInfo() {
      return *static_cast<CIEC_WSTRING*>(&getMembers()[3]);
    }

  private:
    static const CStringDictionary::TStringId scmElementTypes[];
    static const CStringDictionary::TStringId scmElementNames[];
};

#endif //_FORTE_ARROWHEADSYSTEM_H_
