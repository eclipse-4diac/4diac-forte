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

#include <cfb.h>
#include <typelib.h>
#include "OrchestratorRequestAdp.h"

class FORTE_RequestOrchestrationOpcUa: public CCompositeFB{
  DECLARE_FIRMWARE_FB(FORTE_RequestOrchestrationOpcUa)

private:
  static const TForteInt16 scm_anEOWithIndexes[];
  static const SAdapterInstanceDef scm_astAdapterInstances[];

  FORTE_OrchestratorRequestAdp& requestOrchestration() {
    return (*static_cast<FORTE_OrchestratorRequestAdp*>(m_apoAdapters[0]));
  };
  static const int scm_nrequestOrchestrationAdpNum = 0;
  static const SFBInterfaceSpec scm_stFBInterfaceSpec;

   FORTE_FB_DATA_ARRAY(0, 0, 0, 1);

  static const SCFB_FBInstanceData scm_astInternalFBs[];

  static const SCFB_FBParameter scm_astParamters[];

  static const SCFB_FBConnectionData scm_astEventConnections[];

  static const SCFB_FBFannedOutConnectionData scm_astFannedOutEventConnections[];

  static const SCFB_FBConnectionData scm_astDataConnections[];

  static const SCFB_FBFannedOutConnectionData scm_astFannedOutDataConnections[];
  static const SCFB_FBNData scm_stFBNData;

public:
  COMPOSITE_FUNCTION_BLOCK_CTOR(FORTE_RequestOrchestrationOpcUa){
  };

  virtual ~FORTE_RequestOrchestrationOpcUa(){};

};

#endif //close the ifdef sequence from the beginning of the file

