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

#include "FieldsToServiceRequestForm.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "FieldsToServiceRequestForm_gen.cpp"
#endif

DEFINE_FIRMWARE_FB(FORTE_FieldsToServiceRequestForm, g_nStringIdFieldsToServiceRequestForm)

const CStringDictionary::TStringId FORTE_FieldsToServiceRequestForm::scm_anDataInputNames[] = {g_nStringIdrequesterSystem, g_nStringIdrequesterCloud, g_nStringIdrequestedService, g_nStringIdorchestrationFlags, g_nStringIdpreferredProviders, g_nStringIdrequestedQoS};

const CStringDictionary::TStringId FORTE_FieldsToServiceRequestForm::scm_anDataInputTypeIds[] = {g_nStringIdArrowheadSystem, g_nStringIdArrowheadCloud, g_nStringIdArrowheadService, g_nStringIdARRAY, 10, g_nStringIdWSTRING, g_nStringIdARRAY, 10, g_nStringIdPreferredProvider, g_nStringIdARRAY, 10, g_nStringIdWSTRING};

const CStringDictionary::TStringId FORTE_FieldsToServiceRequestForm::scm_anDataOutputNames[] = {g_nStringIdserviceRequestForm};

const CStringDictionary::TStringId FORTE_FieldsToServiceRequestForm::scm_anDataOutputTypeIds[] = {g_nStringIdServiceRequestForm};

const TForteInt16 FORTE_FieldsToServiceRequestForm::scm_anEIWithIndexes[] = {0};
const TDataIOID FORTE_FieldsToServiceRequestForm::scm_anEIWith[] = {0, 1, 5, 4, 3, 2, 255};
const CStringDictionary::TStringId FORTE_FieldsToServiceRequestForm::scm_anEventInputNames[] = {g_nStringIdREQ};

const TDataIOID FORTE_FieldsToServiceRequestForm::scm_anEOWith[] = {0, 255};
const TForteInt16 FORTE_FieldsToServiceRequestForm::scm_anEOWithIndexes[] = {0, -1};
const CStringDictionary::TStringId FORTE_FieldsToServiceRequestForm::scm_anEventOutputNames[] = {g_nStringIdCNF};

const SFBInterfaceSpec FORTE_FieldsToServiceRequestForm::scm_stFBInterfaceSpec = {
  1,  scm_anEventInputNames,  scm_anEIWith,  scm_anEIWithIndexes,
  1,  scm_anEventOutputNames,  scm_anEOWith, scm_anEOWithIndexes,  6,  scm_anDataInputNames, scm_anDataInputTypeIds,
  1,  scm_anDataOutputNames, scm_anDataOutputTypeIds,
  0, 0
};

void FORTE_FieldsToServiceRequestForm::alg_REQ(void){
CIEC_INT i;
;

serviceRequestForm().requesterSystem() = requesterSystem();
serviceRequestForm().requesterCloud() = requesterCloud();
serviceRequestForm().requestedService() = requestedService();


i = 0;
while((i < 10)){
// looks like while is faster than FOR when exporting to cpp

serviceRequestForm().orchestrationFlags()[i] = orchestrationFlags()[i];
serviceRequestForm().preferredProviders()[i] = preferredProviders()[i];
serviceRequestForm().requestedQoS()[i] = requestedQoS()[i];
i = i+1;
}






}


void FORTE_FieldsToServiceRequestForm::enterStateSTART(void){
  m_nECCState = scm_nStateSTART;
}

void FORTE_FieldsToServiceRequestForm::enterStateREQ(void){
  m_nECCState = scm_nStateREQ;
  alg_REQ();
  sendOutputEvent( scm_nEventCNFID);
}

void FORTE_FieldsToServiceRequestForm::executeEvent(int pa_nEIID){
  bool bTransitionCleared;
  do{
    bTransitionCleared = true;
    switch(m_nECCState){
      case scm_nStateSTART:
        if(scm_nEventREQID == pa_nEIID)
          enterStateREQ();
        else
          bTransitionCleared  = false; //no transition cleared
        break;
      case scm_nStateREQ:
        if(1)
          enterStateSTART();
        else
          bTransitionCleared  = false; //no transition cleared
        break;
      default:
      DEVLOG_ERROR("The state is not in the valid range! The state value is: %d. The max value can be: 1.", m_nECCState.operator TForteUInt16 ());
        m_nECCState = 0; //0 is always the initial state
        break;
    }
    pa_nEIID = cg_nInvalidEventID;  // we have to clear the event after the first check in order to ensure correct behavior
  }while(bTransitionCleared);
}


