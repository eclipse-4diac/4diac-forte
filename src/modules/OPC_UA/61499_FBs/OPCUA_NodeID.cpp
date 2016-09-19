/*************************************************************************
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x!
 ***
 *** Name: OPCUA_NodeID
 *** Description: Basic Function Block Type
 *** Version: 
 ***     0.0: 2016-05-09/4DIAC-IDE - 4DIAC-Consortium - 
 *************************************************************************/

#include "OPCUA_NodeID.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "OPCUA_NodeID_gen.cpp"
#endif

DEFINE_FIRMWARE_FB(FORTE_OPCUA_NodeID, g_nStringIdOPCUA_NodeID)

const CStringDictionary::TStringId FORTE_OPCUA_NodeID::scm_anDataInputNames[] = {g_nStringIdnsIndex, g_nStringIdidentifierType, g_nStringIdidentifier};

const CStringDictionary::TStringId FORTE_OPCUA_NodeID::scm_anDataInputTypeIds[] = {g_nStringIdUINT, g_nStringIdUSINT, g_nStringIdANY};

const TForteInt16 FORTE_OPCUA_NodeID::scm_anEIWithIndexes[] = {0, 4};
const TDataIOID FORTE_OPCUA_NodeID::scm_anEIWith[] = {0, 1, 2, 255, 0, 1, 2, 255};
const CStringDictionary::TStringId FORTE_OPCUA_NodeID::scm_anEventInputNames[] = {g_nStringIdINIT, g_nStringIdREQ};

const TForteInt16 FORTE_OPCUA_NodeID::scm_anEOWithIndexes[] = {-1, -1, -1};
const CStringDictionary::TStringId FORTE_OPCUA_NodeID::scm_anEventOutputNames[] = {g_nStringIdINITO, g_nStringIdCNF};

const SAdapterInstanceDef FORTE_OPCUA_NodeID::scm_astAdapterInstances[] = {
{g_nStringIdAOPCUANodeID, g_nStringIdAOPCUANodeID, true }};

const SFBInterfaceSpec FORTE_OPCUA_NodeID::scm_stFBInterfaceSpec = {
  2,  scm_anEventInputNames,  scm_anEIWith,  scm_anEIWithIndexes,
  2,  scm_anEventOutputNames,  0, 0,  3,  scm_anDataInputNames, scm_anDataInputTypeIds,
  0,  0, 0,
  1,scm_astAdapterInstances};

void FORTE_OPCUA_NodeID::alg_sendNodeId(void){
AOPCUANodeID().nsIndex() = nsIndex();
AOPCUANodeID().identifierType() = identifierType();
AOPCUANodeID().identifier().clone(reinterpret_cast<TForteByte *>(&(identifier())));
}

void FORTE_OPCUA_NodeID::enterStateSTART(void){
  m_nECCState = scm_nStateSTART;
}

void FORTE_OPCUA_NodeID::enterStateState(void){
  m_nECCState = scm_nStateState;
  alg_sendNodeId();
  sendOutputEvent( scm_nEventINITOID);
}

void FORTE_OPCUA_NodeID::enterStateState_1(void){
  m_nECCState = scm_nStateState_1;
  alg_sendNodeId();
  sendOutputEvent( scm_nEventCNFID);
}

void FORTE_OPCUA_NodeID::executeEvent(int pa_nEIID){
  bool bTransitionCleared;
  do{
    bTransitionCleared = true;
    switch(m_nECCState){
      case scm_nStateSTART:
        if(scm_nEventINITID == pa_nEIID)
          enterStateState();
        else
        if(scm_nEventREQID == pa_nEIID)
          enterStateState_1();
        else
          bTransitionCleared  = false; //no transition cleared
        break;
      case scm_nStateState:
        if(1)
          enterStateSTART();
        else
          bTransitionCleared  = false; //no transition cleared
        break;
      case scm_nStateState_1:
        if(1)
          enterStateSTART();
        else
          bTransitionCleared  = false; //no transition cleared
        break;
      default:
      DEVLOG_ERROR("The state is not in the valid range! The state value is: %d. The max value can be: 2.", m_nECCState.operator TForteUInt16 ());
        m_nECCState = 0; //0 is always the initial state
        break;
    }
    pa_nEIID = cg_nInvalidEventID;  // we have to clear the event after the first check in order to ensure correct behavior
  }while(bTransitionCleared);
}


