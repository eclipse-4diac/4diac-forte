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

#ifndef _REGISTERSERVICE_H_
#define _REGISTERSERVICE_H_

#include "forte/cfb.h"
#include "forte/typelib.h"
#include <forte_serviceregistryentry.h>
#include "forte/datatypes/forte_wstring.h"
#include "RegisterServiceAdp_adp.h"

class FORTE_RegisterService : public CCompositeFB {
    DECLARE_FIRMWARE_FB(FORTE_RegisterService)

  private:
    CIEC_ServiceRegistryEntry &serviceRegistryEntry() {
      return *static_cast<CIEC_ServiceRegistryEntry *>(getDI(0));
    };

    CIEC_WSTRING &endpoint() {
      return *static_cast<CIEC_WSTRING *>(getDI(1));
    };

    static const TEventID scmEventregisterID = 0;
    static const TEventID scmEventunregisterID = 1;

    static const TEventID scmEventdoneRegisterID = 0;
    static const TEventID scmEventdoneUnregisterID = 1;

    FORTE_RegisterServiceAdp &registerService() {
      return (*static_cast<FORTE_RegisterServiceAdp *>(getAdapterUnchecked(0)));
    };
    static const int scm_nregisterServiceAdpNum = 0;

    static const SCFB_FBNData scmFBNData;

  public:
    FORTE_RegisterService(const forte::StringId paInstanceNameId, forte::CFBContainer &paContainer) :

        ~FORTE_RegisterService() override = default;
};

#endif // close the ifdef sequence from the beginning of the file
