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

#include "FieldsToArrowheadEvent.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "FieldsToArrowheadEvent_gen.cpp"
#endif

DEFINE_FIRMWARE_FB(FORTE_FieldsToArrowheadEvent, g_nStringIdFieldsToArrowheadEvent)

const CStringDictionary::TStringId FORTE_FieldsToArrowheadEvent::scm_anDataInputNames[] = {g_nStringIdtype, g_nStringIdpayload, g_nStringIdtimestamp, g_nStringIdeventMetadata};

const CStringDictionary::TStringId FORTE_FieldsToArrowheadEvent::scm_anDataInputTypeIds[] = {g_nStringIdWSTRING, g_nStringIdWSTRING, g_nStringIdDATE_AND_TIME, g_nStringIdARRAY, 10, g_nStringIdWSTRING};

const CStringDictionary::TStringId FORTE_FieldsToArrowheadEvent::scm_anDataOutputNames[] = {g_nStringIdarrowheadEvent};

const CStringDictionary::TStringId FORTE_FieldsToArrowheadEvent::scm_anDataOutputTypeIds[] = {g_nStringIdArrowheadEvent};

const TForteInt16 FORTE_FieldsToArrowheadEvent::scm_anEIWithIndexes[] = {0};
const TDataIOID FORTE_FieldsToArrowheadEvent::scm_anEIWith[] = {0, 1, 2, 3, 255};
const CStringDictionary::TStringId FORTE_FieldsToArrowheadEvent::scm_anEventInputNames[] = {g_nStringIdREQ};

const TDataIOID FORTE_FieldsToArrowheadEvent::scm_anEOWith[] = {0, 255};
const TForteInt16 FORTE_FieldsToArrowheadEvent::scm_anEOWithIndexes[] = {0, -1};
const CStringDictionary::TStringId FORTE_FieldsToArrowheadEvent::scm_anEventOutputNames[] = {g_nStringIdCNF};

const SFBInterfaceSpec FORTE_FieldsToArrowheadEvent::scm_stFBInterfaceSpec = {
  1,  scm_anEventInputNames,  scm_anEIWith,  scm_anEIWithIndexes,
  1,  scm_anEventOutputNames,  scm_anEOWith, scm_anEOWithIndexes,  4,  scm_anDataInputNames, scm_anDataInputTypeIds,
  1,  scm_anDataOutputNames, scm_anDataOutputTypeIds,
  0, 0
};

void FORTE_FieldsToArrowheadEvent::alg_REQ(void){
CIEC_INT i;
;

i = 0;
arrowheadEvent().type() = type();
arrowheadEvent().payload() = payload();
arrowheadEvent().timestamp() = timestamp();

while((i < 10)){
// looks like while is faster than FOR when exporting to cpp

arrowheadEvent().eventMetadata()[i] = eventMetadata()[i];
i = i+1;
}
}


void FORTE_FieldsToArrowheadEvent::enterStateSTART(void){
  m_nECCState = scm_nStateSTART;
}

void FORTE_FieldsToArrowheadEvent::enterStateREQ(void){
  m_nECCState = scm_nStateREQ;
  alg_REQ();
  sendOutputEvent( scm_nEventCNFID);
}

void FORTE_FieldsToArrowheadEvent::executeEvent(int pa_nEIID){
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
        if((1))
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


