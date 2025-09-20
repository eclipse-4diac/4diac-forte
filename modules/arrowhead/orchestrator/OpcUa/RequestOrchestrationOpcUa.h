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

#ifndef _REQUESTORCHESTRATIONOPCUA_H_
#define _REQUESTORCHESTRATIONOPCUA_H_

#include "forte/cfb.h"
#include "forte/typelib.h"
#include "OrchestratorRequestAdp_adp.h"

class FORTE_RequestOrchestrationOpcUa : public CCompositeFB {
    DECLARE_FIRMWARE_FB(FORTE_RequestOrchestrationOpcUa)

  private:
    FORTE_OrchestratorRequestAdp &requestOrchestration() {
      return (*static_cast<FORTE_OrchestratorRequestAdp *>(getAdapterUnchecked(0)));
    };
    static const int scm_nrequestOrchestrationAdpNum = 0;

  public:
    FORTE_RequestOrchestrationOpcUa(const forte::StringId paInstanceNameId, forte::CFBContainer &paContainer) :

        ~FORTE_RequestOrchestrationOpcUa() override = default;
};

#endif // close the ifdef sequence from the beginning of the file
