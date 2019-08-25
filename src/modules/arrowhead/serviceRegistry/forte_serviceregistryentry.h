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

#ifndef _FORTE_SERVICEREGISTRYENTRY_H_
#define _FORTE_SERVICEREGISTRYENTRY_H_

#include "forte_struct.h"
#include "forte_arrowheadservice.h"
#include "forte_arrowheadsystem.h"
#include "forte_dint.h"
#include "forte_wstring.h"
#include "forte_bool.h"

class CIEC_ServiceRegistryEntry : public CIEC_STRUCT {
  DECLARE_FIRMWARE_DATATYPE(ServiceRegistryEntry)

  public:
    CIEC_ServiceRegistryEntry();

    virtual ~CIEC_ServiceRegistryEntry() {
    }

    CIEC_ArrowheadService &providedService() {
      return *static_cast<CIEC_ArrowheadService*>(&getMembers()[0]);
    }

    CIEC_ArrowheadSystem &provider() {
      return *static_cast<CIEC_ArrowheadSystem*>(&getMembers()[1]);
    }

    CIEC_WSTRING &serviceURI() {
      return *static_cast<CIEC_WSTRING*>(&getMembers()[2]);
    }

    CIEC_DINT &version() {
      return *static_cast<CIEC_DINT*>(&getMembers()[3]);
    }

    CIEC_BOOL &udp() {
      return *static_cast<CIEC_BOOL*>(&getMembers()[4]);
    }
    CIEC_DINT &ttl() {
      return *static_cast<CIEC_DINT*>(&getMembers()[5]);
    }

    CIEC_WSTRING &metadata() {
      return *static_cast<CIEC_WSTRING*>(&getMembers()[6]);
    }

  private:
    static const CStringDictionary::TStringId scmElementTypes[];
    static const CStringDictionary::TStringId scmElementNames[];
};

#endif //_FORTE_SERVICEREGISTRYENTRY_H_
