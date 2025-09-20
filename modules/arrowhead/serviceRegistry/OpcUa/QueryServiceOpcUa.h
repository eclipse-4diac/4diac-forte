/*******************************************************************************
 * Copyright (c) 2019, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Jose Cabral - initial implementation
 *******************************************************************************/

#ifndef _QUERYSERVICEOPCUA_H_
#define _QUERYSERVICEOPCUA_H_

#include "forte/cfb.h"
#include "forte/typelib.h"
#include "QueryServicesAdp_adp.h"

class FORTE_QueryServiceOpcUa : public CCompositeFB {
    DECLARE_FIRMWARE_FB(FORTE_QueryServiceOpcUa)

  private:
    FORTE_QueryServicesAdp &queryServices() {
      return (*static_cast<FORTE_QueryServicesAdp *>(getAdapterUnchecked(0)));
    };
    static const int scm_nqueryServicesAdpNum = 0;

  public:
    FORTE_QueryServiceOpcUa(const forte::StringId paInstanceNameId, forte::CFBContainer &paContainer) :

        ~FORTE_QueryServiceOpcUa() override = default;
};

#endif // close the ifdef sequence from the beginning of the file
