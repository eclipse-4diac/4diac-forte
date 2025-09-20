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

#ifndef _QUERYSERVICE_H_
#define _QUERYSERVICE_H_

#include "forte/cfb.h"
#include "forte/typelib.h"
#include <forte_servicequeryform.h>
#include <forte_serviceregistryentry.h>
#include "forte/datatypes/forte_wstring.h"
#include "forte/datatypes/forte_array.h"
#include "QueryServicesAdp_adp.h"

class FORTE_QueryService : public CCompositeFB {
    DECLARE_FIRMWARE_FB(FORTE_QueryService)

  private:
    CIEC_ServiceQueryForm &serviceQueryForm() {
      return *static_cast<CIEC_ServiceQueryForm *>(getDI(0));
    };

    CIEC_WSTRING &address() {
      return *static_cast<CIEC_WSTRING *>(getDI(1));
    };

    CIEC_ServiceRegistryEntry *serviceEntries() {
      return (CIEC_ServiceRegistryEntry *) (*static_cast<CIEC_ARRAY<> *>(
          getDO(0)))[0]; // the first element marks the start of the array
    };

    CIEC_ARRAY<> &serviceEntries_Array() {
      return *static_cast<CIEC_ARRAY<> *>(getDO(0));
    };

    static const TEventID scmEventqueryID = 0;

    static const TEventID scmEventqueriedID = 0;

    FORTE_QueryServicesAdp &queryServices() {
      return (*static_cast<FORTE_QueryServicesAdp *>(getAdapterUnchecked(0)));
    };
    static const int scm_nqueryServicesAdpNum = 0;

    static const SCFB_FBNData scmFBNData;

  public:
    FORTE_QueryService(const forte::StringId paInstanceNameId, forte::CFBContainer &paContainer) :

        ~FORTE_QueryService() override = default;
};

#endif // close the ifdef sequence from the beginning of the file
