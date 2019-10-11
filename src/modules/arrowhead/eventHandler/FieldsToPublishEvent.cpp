/*************************************************************************
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x!
 ***
 *** Name: FieldsToPublishEvent
 *** Description: Template for a simple Basic Function Block Type
 *** Version: 
 ***     1.0: 2019-09-17/Cabral - fortiss GmbH - 
 *************************************************************************/

#include "FieldsToPublishEvent.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "FieldsToPublishEvent_gen.cpp"
#endif

DEFINE_FIRMWARE_FB(FORTE_FieldsToPublishEvent, g_nStringIdFieldsToPublishEvent)

const CStringDictionary::TStringId FORTE_FieldsToPublishEvent::scm_anDataInputNames[] = {g_nStringIdsource, g_nStringIdevent, g_nStringIddeliveryCompleteUri};

const CStringDictionary::TStringId FORTE_FieldsToPublishEvent::scm_anDataInputTypeIds[] = {g_nStringIdArrowheadSystem, g_nStringIdArrowheadEvent, g_nStringIdWSTRING};

const CStringDictionary::TStringId FORTE_FieldsToPublishEvent::scm_anDataOutputNames[] = {g_nStringIdpublishEvent};

const CStringDictionary::TStringId FORTE_FieldsToPublishEvent::scm_anDataOutputTypeIds[] = {g_nStringIdPublishEvent};

const TForteInt16 FORTE_FieldsToPublishEvent::scm_anEIWithIndexes[] = {0};
const TDataIOID FORTE_FieldsToPublishEvent::scm_anEIWith[] = {0, 1, 2, 255};
const CStringDictionary::TStringId FORTE_FieldsToPublishEvent::scm_anEventInputNames[] = {g_nStringIdREQ};

const TDataIOID FORTE_FieldsToPublishEvent::scm_anEOWith[] = {0, 255};
const TForteInt16 FORTE_FieldsToPublishEvent::scm_anEOWithIndexes[] = {0, -1};
const CStringDictionary::TStringId FORTE_FieldsToPublishEvent::scm_anEventOutputNames[] = {g_nStringIdCNF};

const SFBInterfaceSpec FORTE_FieldsToPublishEvent::scm_stFBInterfaceSpec = {
  1,  scm_anEventInputNames,  scm_anEIWith,  scm_anEIWithIndexes,
  1,  scm_anEventOutputNames,  scm_anEOWith, scm_anEOWithIndexes,  3,  scm_anDataInputNames, scm_anDataInputTypeIds,
  1,  scm_anDataOutputNames, scm_anDataOutputTypeIds,
  0, 0
};

void FORTE_FieldsToPublishEvent::alg_REQ(void){
publishEvent().source() = source();
publishEvent().event() = event();
publishEvent().deliveryCompleteUri() = deliveryCompleteUri();


}


void FORTE_FieldsToPublishEvent::enterStateSTART(void){
  m_nECCState = scm_nStateSTART;
}

void FORTE_FieldsToPublishEvent::enterStateREQ(void){
  m_nECCState = scm_nStateREQ;
  alg_REQ();
  sendOutputEvent( scm_nEventCNFID);
}

void FORTE_FieldsToPublishEvent::executeEvent(int pa_nEIID){
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


