/*******************************************************************************
 * Copyright (c) 2019, fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Jose Cabral - initial implementation
 *******************************************************************************/

#include "OrchestrationForm2OrchestrationForm.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "OrchestrationForm2OrchestrationForm_gen.cpp"
#endif

DEFINE_FIRMWARE_FB(FORTE_OrchestrationForm2OrchestrationForm, g_nStringIdOrchestrationForm2OrchestrationForm)

const CStringDictionary::TStringId FORTE_OrchestrationForm2OrchestrationForm::scm_anDataInputNames[] = {g_nStringIdIN};

const CStringDictionary::TStringId FORTE_OrchestrationForm2OrchestrationForm::scm_anDataInputTypeIds[] = {g_nStringIdOrchestrationForm};

const CStringDictionary::TStringId FORTE_OrchestrationForm2OrchestrationForm::scm_anDataOutputNames[] = {g_nStringIdOUT};

const CStringDictionary::TStringId FORTE_OrchestrationForm2OrchestrationForm::scm_anDataOutputTypeIds[] = {g_nStringIdOrchestrationForm};

const TForteInt16 FORTE_OrchestrationForm2OrchestrationForm::scm_anEIWithIndexes[] = {0};
const TDataIOID FORTE_OrchestrationForm2OrchestrationForm::scm_anEIWith[] = {0, 255};
const CStringDictionary::TStringId FORTE_OrchestrationForm2OrchestrationForm::scm_anEventInputNames[] = {g_nStringIdREQ};

const TDataIOID FORTE_OrchestrationForm2OrchestrationForm::scm_anEOWith[] = {0, 255};
const TForteInt16 FORTE_OrchestrationForm2OrchestrationForm::scm_anEOWithIndexes[] = {0, -1};
const CStringDictionary::TStringId FORTE_OrchestrationForm2OrchestrationForm::scm_anEventOutputNames[] = {g_nStringIdCNF};

const SFBInterfaceSpec FORTE_OrchestrationForm2OrchestrationForm::scm_stFBInterfaceSpec = {
  1,  scm_anEventInputNames,  scm_anEIWith,  scm_anEIWithIndexes,
  1,  scm_anEventOutputNames,  scm_anEOWith, scm_anEOWithIndexes,  1,  scm_anDataInputNames, scm_anDataInputTypeIds,
  1,  scm_anDataOutputNames, scm_anDataOutputTypeIds,
  0, 0
};


void FORTE_OrchestrationForm2OrchestrationForm::executeEvent(int pa_nEIID){
  if(scm_nEventREQID == pa_nEIID) {
    OUT() = IN();
    sendOutputEvent(scm_nEventCNFID);
  }
}



