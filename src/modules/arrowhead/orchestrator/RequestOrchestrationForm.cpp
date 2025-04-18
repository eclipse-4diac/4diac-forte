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

#include "RequestOrchestrationForm.h"

USE_STRING_ID(ARRAY);
USE_STRING_ID(endpoint);
USE_STRING_ID(Event);
USE_STRING_ID(OrchestrationForm);
USE_STRING_ID(orchestrationResponse);
USE_STRING_ID(OrchestratorRequestAdp);
USE_STRING_ID(requestOrchestator);
USE_STRING_ID(requestOrchestration);
USE_STRING_ID(RequestOrchestrationForm);
USE_STRING_ID(responseReceived);
USE_STRING_ID(serviceRequestForm);
USE_STRING_ID(ServiceRequestForm);
USE_STRING_ID(WSTRING);

DEFINE_FIRMWARE_FB(FORTE_RequestOrchestrationForm, STRID(RequestOrchestrationForm))

const CStringDictionary::TStringId FORTE_RequestOrchestrationForm::scmDataInputNames[] = {STRID(serviceRequestForm),
                                                                                          STRID(endpoint)};

const CStringDictionary::TStringId FORTE_RequestOrchestrationForm::scmDataInputTypeIds[] = {STRID(ServiceRequestForm),
                                                                                            STRID(WSTRING)};

const CStringDictionary::TStringId FORTE_RequestOrchestrationForm::scmDataOutputNames[] = {
    STRID(orchestrationResponse)};

const CStringDictionary::TStringId FORTE_RequestOrchestrationForm::scmDataOutputTypeIds[] = {STRID(ARRAY), 10,
                                                                                             STRID(OrchestrationForm)};

const TForteInt16 FORTE_RequestOrchestrationForm::scmEIWithIndexes[] = {0};
const TDataIOID FORTE_RequestOrchestrationForm::scmEIWith[] = {0, 1, scmWithListDelimiter};
const CStringDictionary::TStringId FORTE_RequestOrchestrationForm::scmEventInputNames[] = {STRID(requestOrchestator)};
const CStringDictionary::TStringId FORTE_RequestOrchestrationForm::scmEventInputTypeIds[] = {STRID(Event)};

const TDataIOID FORTE_RequestOrchestrationForm::scmEOWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_RequestOrchestrationForm::scmEOWithIndexes[] = {0, -1};
const CStringDictionary::TStringId FORTE_RequestOrchestrationForm::scmEventOutputNames[] = {STRID(responseReceived)};
const CStringDictionary::TStringId FORTE_RequestOrchestrationForm::scmEventOutputTypeIds[] = {STRID(Event)};

const SAdapterInstanceDef FORTE_RequestOrchestrationForm::scmAdapterInstances[] = {
    {STRID(OrchestratorRequestAdp), STRID(requestOrchestration), true}};

const SFBInterfaceSpec FORTE_RequestOrchestrationForm::scmFBInterfaceSpec = {1,
                                                                             scmEventInputNames,
                                                                             scmEventInputTypeIds,
                                                                             scmEIWith,
                                                                             scmEIWithIndexes,
                                                                             1,
                                                                             scmEventOutputNames,
                                                                             scmEventOutputTypeIds,
                                                                             scmEOWith,
                                                                             scmEOWithIndexes,
                                                                             2,
                                                                             scmDataInputNames,
                                                                             scmDataInputTypeIds,
                                                                             1,
                                                                             scmDataOutputNames,
                                                                             scmDataOutputTypeIds,
                                                                             1,
                                                                             scmAdapterInstances};

const SCFB_FBConnectionData FORTE_RequestOrchestrationForm::scmEventConnections[] = {
    {GENERATE_CONNECTION_PORT_ID_1_ARG(STRID(requestOrchestator)), -1,
     GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(requestOrchestration), STRID(requestOrchestator)),
     CCompositeFB::scmAdapterMarker | 0},
    {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(requestOrchestration), STRID(responseReceived)),
     CCompositeFB::scmAdapterMarker | 0, GENERATE_CONNECTION_PORT_ID_1_ARG(STRID(responseReceived)), -1},
};

const SCFB_FBConnectionData FORTE_RequestOrchestrationForm::scmDataConnections[] = {
    {GENERATE_CONNECTION_PORT_ID_1_ARG(STRID(serviceRequestForm)), -1,
     GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(requestOrchestration), STRID(serviceRequestForm)),
     CCompositeFB::scmAdapterMarker | 0},
    {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(requestOrchestration), STRID(orchestrationResponse)),
     CCompositeFB::scmAdapterMarker | 0, GENERATE_CONNECTION_PORT_ID_1_ARG(STRID(orchestrationResponse)), -1},
    {GENERATE_CONNECTION_PORT_ID_1_ARG(STRID(endpoint)), -1,
     GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(requestOrchestration), STRID(endpoint)),
     CCompositeFB::scmAdapterMarker | 0},
};

const SCFB_FBNData FORTE_RequestOrchestrationForm::scmFBNData = {
    0, 0, 2, scmEventConnections, 0, 0, 3, scmDataConnections, 0, 0, 0, nullptr, 0, 0};
