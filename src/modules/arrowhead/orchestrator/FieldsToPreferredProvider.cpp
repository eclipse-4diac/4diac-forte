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

#include "FieldsToPreferredProvider.h"

USE_STRING_ID(ArrowheadCloud);
USE_STRING_ID(ArrowheadSystem);
USE_STRING_ID(CNF);
USE_STRING_ID(Event);
USE_STRING_ID(FieldsToPreferredProvider);
USE_STRING_ID(preferredProvider);
USE_STRING_ID(PreferredProvider);
USE_STRING_ID(providerCloud);
USE_STRING_ID(providerSystem);
USE_STRING_ID(REQ);


DEFINE_FIRMWARE_FB(FORTE_FieldsToPreferredProvider, STRID(FieldsToPreferredProvider))

const CStringDictionary::TStringId FORTE_FieldsToPreferredProvider::scmDataInputNames[] = {STRID(providerSystem), STRID(providerCloud)};

const CStringDictionary::TStringId FORTE_FieldsToPreferredProvider::scmDataInputTypeIds[] = {STRID(ArrowheadSystem), STRID(ArrowheadCloud)};

const CStringDictionary::TStringId FORTE_FieldsToPreferredProvider::scmDataOutputNames[] = {STRID(preferredProvider)};

const CStringDictionary::TStringId FORTE_FieldsToPreferredProvider::scmDataOutputTypeIds[] = {STRID(PreferredProvider)};

const TForteInt16 FORTE_FieldsToPreferredProvider::scmEIWithIndexes[] = {0};
const TDataIOID FORTE_FieldsToPreferredProvider::scmEIWith[] = {0, 1, scmWithListDelimiter};
const CStringDictionary::TStringId FORTE_FieldsToPreferredProvider::scmEventInputNames[] = {STRID(REQ)};
const CStringDictionary::TStringId FORTE_FieldsToPreferredProvider::scmEventInputTypeIds[] = {STRID(Event)};

const TDataIOID FORTE_FieldsToPreferredProvider::scmEOWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_FieldsToPreferredProvider::scmEOWithIndexes[] = {0, -1};
const CStringDictionary::TStringId FORTE_FieldsToPreferredProvider::scmEventOutputNames[] = {STRID(CNF)};
const CStringDictionary::TStringId FORTE_FieldsToPreferredProvider::scmEventOutputTypeIds[] = {STRID(Event)};

const SFBInterfaceSpec FORTE_FieldsToPreferredProvider::scmFBInterfaceSpec = {
  1,  scmEventInputNames, scmEventInputTypeIds,  scmEIWith,  scmEIWithIndexes,
  1,  scmEventOutputNames, scmEventOutputTypeIds,  scmEOWith, scmEOWithIndexes,  2,  scmDataInputNames, scmDataInputTypeIds,
  1,  scmDataOutputNames, scmDataOutputTypeIds,
  0, 0
};

void FORTE_FieldsToPreferredProvider::alg_REQ(){
preferredProvider().providerSystem() = providerSystem();
preferredProvider().providerCloud() = providerCloud();





}


void FORTE_FieldsToPreferredProvider::enterStateSTART(CEventChainExecutionThread *const paECET){
  mECCState = scmStateSTART;
}

void FORTE_FieldsToPreferredProvider::enterStateREQ(CEventChainExecutionThread * const paECET){
  mECCState = scmStateREQ;
  alg_REQ();
  sendOutputEvent(scmEventCNFID, paECET);
}

void FORTE_FieldsToPreferredProvider::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
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


