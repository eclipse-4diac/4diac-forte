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

#ifndef _REQUESTORCHESTRATIONHTTP_H_
#define _REQUESTORCHESTRATIONHTTP_H_

#include "forte/cfb.h"
#include "forte/typelib.h"
#include "OrchestratorRequestAdp_adp.h"

class FORTE_RequestOrchestrationHTTP : public CCompositeFB {
    DECLARE_FIRMWARE_FB(FORTE_RequestOrchestrationHTTP)

  private:
    FORTE_OrchestratorRequestAdp &requestOrchestration() {
      return (*static_cast<FORTE_OrchestratorRequestAdp *>(getAdapterUnchecked(0)));
    };
    static const int scm_nrequestOrchestrationAdpNum = 0;

  public:
    FORTE_RequestOrchestrationHTTP(const forte::StringId paInstanceNameId, forte::CFBContainer &paContainer) :

        ~FORTE_RequestOrchestrationHTTP() override = default;
};

#endif // close the ifdef sequence from the beginning of the file
