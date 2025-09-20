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

#ifndef _REQUESTORCHESTRATIONFORM_H_
#define _REQUESTORCHESTRATIONFORM_H_

#include "forte/cfb.h"
#include "forte/typelib.h"
#include <forte_orchestrationform.h>
#include <forte_servicerequestform.h>
#include "forte/datatypes/forte_wstring.h"
#include "forte/datatypes/forte_array.h"
#include "OrchestratorRequestAdp_adp.h"

class FORTE_RequestOrchestrationForm : public CCompositeFB {
    DECLARE_FIRMWARE_FB(FORTE_RequestOrchestrationForm)

  private:
    CIEC_ServiceRequestForm &serviceRequestForm() {
      return *static_cast<CIEC_ServiceRequestForm *>(getDI(0));
    };

    CIEC_WSTRING &endpoint() {
      return *static_cast<CIEC_WSTRING *>(getDI(1));
    };

    CIEC_OrchestrationForm *orchestrationResponse() {
      return (CIEC_OrchestrationForm *) (*static_cast<CIEC_ARRAY<> *>(
          getDO(0)))[0]; // the first element marks the start of the array
    };

    CIEC_ARRAY<> &orchestrationResponse_Array() {
      return *static_cast<CIEC_ARRAY<> *>(getDO(0));
    };

    static const TEventID scmEventrequestOrchestatorID = 0;

    static const TEventID scmEventresponseReceivedID = 0;

    FORTE_OrchestratorRequestAdp &requestOrchestration() {
      return (*static_cast<FORTE_OrchestratorRequestAdp *>(getAdapterUnchecked(0)));
    };
    static const int scm_nrequestOrchestrationAdpNum = 0;

  public:
    FORTE_RequestOrchestrationForm(const forte::StringId paInstanceNameId, forte::CFBContainer &paContainer) :

        ~FORTE_RequestOrchestrationForm() override = default;
};

#endif // close the ifdef sequence from the beginning of the file
