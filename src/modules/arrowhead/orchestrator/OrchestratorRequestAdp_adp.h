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

#ifndef _ORCHESTRATORREQUESTADP_H_
#define _ORCHESTRATORREQUESTADP_H_

#include "forte/adapter.h"
#include "forte/typelib.h"
#include <forte_orchestrationform.h>
#include <forte_servicerequestform.h>
#include "forte/datatypes/forte_wstring.h"
#include "forte/datatypes/forte_array.h"

class FORTE_OrchestratorRequestAdp : public CAdapter {
    DECLARE_ADAPTER_TYPE(FORTE_OrchestratorRequestAdp)

  private:
  private:
  public:
    CIEC_OrchestrationForm *orchestrationResponse() {
      return (CIEC_OrchestrationForm *) (*static_cast<CIEC_ARRAY<> *>(
          (isSocket()) ? getDI(0) : getDO(0)))[0]; // the first element marks the start of the array
    };

    CIEC_ARRAY<> &orchestrationResponse_Array() {
      return *static_cast<CIEC_ARRAY<> *>((isSocket()) ? getDI(0) : getDO(0));
    };

  private:
  public:
    CIEC_ServiceRequestForm &serviceRequestForm() {
      return *static_cast<CIEC_ServiceRequestForm *>((isSocket()) ? getDO(0) : getDI(0));
    };

    CIEC_WSTRING &endpoint() {
      return *static_cast<CIEC_WSTRING *>((isSocket()) ? getDO(1) : getDI(1));
    };

  public:
    static const TEventID scmEventresponseReceivedID = 0;
    int responseReceived() {
      return mParentAdapterListEventID + scmEventresponseReceivedID;
    }

  private:
  public:
    static const TEventID scmEventrequestOrchestatorID = 0;
    int requestOrchestator() {
      return mParentAdapterListEventID + scmEventrequestOrchestatorID;
    }

  private:
  public:
    ADAPTER_CTOR(FORTE_OrchestratorRequestAdp) {};

    ~FORTE_OrchestratorRequestAdp() override = default;
};

#endif // close the ifdef sequence from the beginning of the file
