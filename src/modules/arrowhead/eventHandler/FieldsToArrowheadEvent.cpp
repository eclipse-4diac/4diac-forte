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

USE_STRING_ID(ARRAY);
USE_STRING_ID(arrowheadEvent);
USE_STRING_ID(ArrowheadEvent);
USE_STRING_ID(CNF);
USE_STRING_ID(DATE_AND_TIME);
USE_STRING_ID(Event);
USE_STRING_ID(eventMetadata);
USE_STRING_ID(FieldsToArrowheadEvent);
USE_STRING_ID(payload);
USE_STRING_ID(REQ);
USE_STRING_ID(timestamp);
USE_STRING_ID(type);
USE_STRING_ID(WSTRING);


DEFINE_FIRMWARE_FB(FORTE_FieldsToArrowheadEvent, STRID(FieldsToArrowheadEvent))

const CStringDictionary::TStringId FORTE_FieldsToArrowheadEvent::scmDataInputNames[] = {STRID(type), STRID(payload), STRID(timestamp), STRID(eventMetadata)};

const CStringDictionary::TStringId FORTE_FieldsToArrowheadEvent::scmDataInputTypeIds[] = {STRID(WSTRING), STRID(WSTRING), STRID(DATE_AND_TIME), STRID(ARRAY), 10, STRID(WSTRING)};

const CStringDictionary::TStringId FORTE_FieldsToArrowheadEvent::scmDataOutputNames[] = {STRID(arrowheadEvent)};

const CStringDictionary::TStringId FORTE_FieldsToArrowheadEvent::scmDataOutputTypeIds[] = {STRID(ArrowheadEvent)};

const TForteInt16 FORTE_FieldsToArrowheadEvent::scmEIWithIndexes[] = {0};
const TDataIOID FORTE_FieldsToArrowheadEvent::scmEIWith[] = {0, 1, 2, 3, scmWithListDelimiter};
const CStringDictionary::TStringId FORTE_FieldsToArrowheadEvent::scmEventInputNames[] = {STRID(REQ)};
const CStringDictionary::TStringId FORTE_FieldsToArrowheadEvent::scmEventInputTypeIds[] = {STRID(Event)};

const TDataIOID FORTE_FieldsToArrowheadEvent::scmEOWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_FieldsToArrowheadEvent::scmEOWithIndexes[] = {0, -1};
const CStringDictionary::TStringId FORTE_FieldsToArrowheadEvent::scmEventOutputNames[] = {STRID(CNF)};
const CStringDictionary::TStringId FORTE_FieldsToArrowheadEvent::scmEventOutputTypeIds[] = {STRID(Event)};

const SFBInterfaceSpec FORTE_FieldsToArrowheadEvent::scmFBInterfaceSpec = {
  1,  scmEventInputNames, scmEventInputTypeIds,  scmEIWith,  scmEIWithIndexes,
  1,  scmEventOutputNames, scmEventOutputTypeIds,  scmEOWith, scmEOWithIndexes,  4,  scmDataInputNames, scmDataInputTypeIds,
  1,  scmDataOutputNames, scmDataOutputTypeIds,
  0, 0
};

void FORTE_FieldsToArrowheadEvent::alg_REQ(){
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


void FORTE_FieldsToArrowheadEvent::enterStateSTART(CEventChainExecutionThread *const paECET){
  mECCState = scmStateSTART;
}

void FORTE_FieldsToArrowheadEvent::enterStateREQ(CEventChainExecutionThread *const paECET){
  mECCState = scmStateREQ;
  alg_REQ();
  sendOutputEvent(scmEventCNFID, paECET);
}

void FORTE_FieldsToArrowheadEvent::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
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
        if((1))
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


