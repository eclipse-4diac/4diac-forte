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

#include "FieldsToArrowheadSystem.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "FieldsToArrowheadSystem_gen.cpp"
#endif

DEFINE_FIRMWARE_FB(FORTE_FieldsToArrowheadSystem, g_nStringIdFieldsToArrowheadSystem)

const CStringDictionary::TStringId FORTE_FieldsToArrowheadSystem::scm_anDataInputNames[] = {g_nStringIdsystemName, g_nStringIdaddress, g_nStringIdport, g_nStringIdauthenticationInfo};

const CStringDictionary::TStringId FORTE_FieldsToArrowheadSystem::scm_anDataInputTypeIds[] = {g_nStringIdWSTRING, g_nStringIdWSTRING, g_nStringIdDINT, g_nStringIdWSTRING};

const CStringDictionary::TStringId FORTE_FieldsToArrowheadSystem::scm_anDataOutputNames[] = {g_nStringIdarrowheadSystem};

const CStringDictionary::TStringId FORTE_FieldsToArrowheadSystem::scm_anDataOutputTypeIds[] = {g_nStringIdArrowheadSystem};

const TForteInt16 FORTE_FieldsToArrowheadSystem::scm_anEIWithIndexes[] = {0};
const TDataIOID FORTE_FieldsToArrowheadSystem::scm_anEIWith[] = {3, 2, 1, 0, 255};
const CStringDictionary::TStringId FORTE_FieldsToArrowheadSystem::scm_anEventInputNames[] = {g_nStringIdREQ};

const TDataIOID FORTE_FieldsToArrowheadSystem::scm_anEOWith[] = {0, 255};
const TForteInt16 FORTE_FieldsToArrowheadSystem::scm_anEOWithIndexes[] = {0, -1};
const CStringDictionary::TStringId FORTE_FieldsToArrowheadSystem::scm_anEventOutputNames[] = {g_nStringIdCNF};

const SFBInterfaceSpec FORTE_FieldsToArrowheadSystem::scm_stFBInterfaceSpec = {
  1,  scm_anEventInputNames,  scm_anEIWith,  scm_anEIWithIndexes,
  1,  scm_anEventOutputNames,  scm_anEOWith, scm_anEOWithIndexes,  4,  scm_anDataInputNames, scm_anDataInputTypeIds,
  1,  scm_anDataOutputNames, scm_anDataOutputTypeIds,
  0, 0
};

void FORTE_FieldsToArrowheadSystem::alg_REQ(void){
arrowheadSystem().systemName() = systemName();
arrowheadSystem().address() = address();
arrowheadSystem().port() = port();
arrowheadSystem().authenticationInfo() = authenticationInfo();

}


void FORTE_FieldsToArrowheadSystem::enterStateSTART(void){
  m_nECCState = scm_nStateSTART;
}

void FORTE_FieldsToArrowheadSystem::enterStateREQ(void){
  m_nECCState = scm_nStateREQ;
  alg_REQ();
  sendOutputEvent( scm_nEventCNFID);
}

void FORTE_FieldsToArrowheadSystem::executeEvent(int pa_nEIID){
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


