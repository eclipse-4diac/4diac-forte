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

USE_STRING_ID(address);
USE_STRING_ID(arrowheadSystem);
USE_STRING_ID(ArrowheadSystem);
USE_STRING_ID(authenticationInfo);
USE_STRING_ID(CNF);
USE_STRING_ID(DINT);
USE_STRING_ID(Event);
USE_STRING_ID(FieldsToArrowheadSystem);
USE_STRING_ID(port);
USE_STRING_ID(REQ);
USE_STRING_ID(systemName);
USE_STRING_ID(WSTRING);


DEFINE_FIRMWARE_FB(FORTE_FieldsToArrowheadSystem, STRID(FieldsToArrowheadSystem))

const CStringDictionary::TStringId FORTE_FieldsToArrowheadSystem::scmDataInputNames[] = {STRID(systemName), STRID(address), STRID(port), STRID(authenticationInfo)};

const CStringDictionary::TStringId FORTE_FieldsToArrowheadSystem::scmDataInputTypeIds[] = {STRID(WSTRING), STRID(WSTRING), STRID(DINT), STRID(WSTRING)};

const CStringDictionary::TStringId FORTE_FieldsToArrowheadSystem::scmDataOutputNames[] = {STRID(arrowheadSystem)};

const CStringDictionary::TStringId FORTE_FieldsToArrowheadSystem::scmDataOutputTypeIds[] = {STRID(ArrowheadSystem)};

const TForteInt16 FORTE_FieldsToArrowheadSystem::scmEIWithIndexes[] = {0};
const TDataIOID FORTE_FieldsToArrowheadSystem::scmEIWith[] = {3, 2, 1, 0, scmWithListDelimiter};
const CStringDictionary::TStringId FORTE_FieldsToArrowheadSystem::scmEventInputNames[] = {STRID(REQ)};
const CStringDictionary::TStringId FORTE_FieldsToArrowheadSystem::scmEventInputTypeIds[] = {STRID(Event)};

const TDataIOID FORTE_FieldsToArrowheadSystem::scmEOWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_FieldsToArrowheadSystem::scmEOWithIndexes[] = {0, -1};
const CStringDictionary::TStringId FORTE_FieldsToArrowheadSystem::scmEventOutputNames[] = {STRID(CNF)};
const CStringDictionary::TStringId FORTE_FieldsToArrowheadSystem::scmEventOutputTypeIds[] = {STRID(Event)};

const SFBInterfaceSpec FORTE_FieldsToArrowheadSystem::scmFBInterfaceSpec = {
  1,  scmEventInputNames, scmEventInputTypeIds,  scmEIWith,  scmEIWithIndexes,
  1,  scmEventOutputNames, scmEventOutputTypeIds,  scmEOWith, scmEOWithIndexes,  4,  scmDataInputNames, scmDataInputTypeIds,
  1,  scmDataOutputNames, scmDataOutputTypeIds,
  0, 0
};

void FORTE_FieldsToArrowheadSystem::alg_REQ(){
arrowheadSystem().systemName() = systemName();
arrowheadSystem().address() = address();
arrowheadSystem().port() = port();
arrowheadSystem().authenticationInfo() = authenticationInfo();

}


void FORTE_FieldsToArrowheadSystem::enterStateSTART(CEventChainExecutionThread *const paECET){
  mECCState = scmStateSTART;
}

void FORTE_FieldsToArrowheadSystem::enterStateREQ(CEventChainExecutionThread *const paECET){
  mECCState = scmStateREQ;
  alg_REQ();
  sendOutputEvent(scmEventCNFID, paECET);
}

void FORTE_FieldsToArrowheadSystem::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
  bool bTransitionCleared;
  do{
    bTransitionCleared = true;
    switch(mECCState){
      case scmStateSTART:
        if(scmEventREQID == paEIID)
          enterStateREQ(paECET);
        else
          bTransitionCleared  = false; //no transition cleared
        break;
      case scmStateREQ:
        if(1)
          enterStateSTART(paECET);
        else
          bTransitionCleared  = false; //no transition cleared
        break;
      default:
      DEVLOG_ERROR("The state is not in the valid range! The state value is: %d. The max value can be: 1.", mECCState.operator TForteUInt16 ());
        mECCState = 0; //0 is always the initial state
        break;
    }
    paEIID = cgInvalidEventID;  // we have to clear the event after the first check in order to ensure correct behavior
  }while(bTransitionCleared);
}


