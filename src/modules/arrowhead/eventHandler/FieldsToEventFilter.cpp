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

#include "FieldsToEventFilter.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "FieldsToEventFilter_gen.cpp"
#endif

DEFINE_FIRMWARE_FB(FORTE_FieldsToEventFilter, g_nStringIdFieldsToEventFilter)

const CStringDictionary::TStringId FORTE_FieldsToEventFilter::scm_anDataInputNames[] = {g_nStringIdeventType, g_nStringIdconsumer, g_nStringIdsources, g_nStringIdstartDate, g_nStringIdendDate, g_nStringIdfilterMetadata, g_nStringIdnotifyUri, g_nStringIdmatchMetadata};

const CStringDictionary::TStringId FORTE_FieldsToEventFilter::scm_anDataInputTypeIds[] = {g_nStringIdWSTRING, g_nStringIdArrowheadSystem, g_nStringIdARRAY, 10, g_nStringIdArrowheadSystem, g_nStringIdDATE_AND_TIME, g_nStringIdDATE_AND_TIME, g_nStringIdARRAY, 10, g_nStringIdWSTRING, g_nStringIdWSTRING, g_nStringIdBOOL};

const CStringDictionary::TStringId FORTE_FieldsToEventFilter::scm_anDataOutputNames[] = {g_nStringIdeventFilter};

const CStringDictionary::TStringId FORTE_FieldsToEventFilter::scm_anDataOutputTypeIds[] = {g_nStringIdEventFilter};

const TForteInt16 FORTE_FieldsToEventFilter::scm_anEIWithIndexes[] = {0};
const TDataIOID FORTE_FieldsToEventFilter::scm_anEIWith[] = {0, 1, 3, 6, 2, 4, 5, 7, 255};
const CStringDictionary::TStringId FORTE_FieldsToEventFilter::scm_anEventInputNames[] = {g_nStringIdREQ};

const TDataIOID FORTE_FieldsToEventFilter::scm_anEOWith[] = {0, 255};
const TForteInt16 FORTE_FieldsToEventFilter::scm_anEOWithIndexes[] = {0, -1};
const CStringDictionary::TStringId FORTE_FieldsToEventFilter::scm_anEventOutputNames[] = {g_nStringIdCNF};

const SFBInterfaceSpec FORTE_FieldsToEventFilter::scm_stFBInterfaceSpec = {
  1,  scm_anEventInputNames,  scm_anEIWith,  scm_anEIWithIndexes,
  1,  scm_anEventOutputNames,  scm_anEOWith, scm_anEOWithIndexes,  8,  scm_anDataInputNames, scm_anDataInputTypeIds,
  1,  scm_anDataOutputNames, scm_anDataOutputTypeIds,
  0, 0
};

void FORTE_FieldsToEventFilter::alg_REQ(void){
CIEC_INT i;
;

i = 0;
eventFilter().eventType() = eventType();
eventFilter().consumer() = consumer();
eventFilter().startDate() = startDate();
eventFilter().endDate() = endDate();
eventFilter().notifyUri() = notifyUri();
eventFilter().matchMetadata() = matchMetadata();

while((i < 10)){
// looks like while is faster than FOR when exporting to cpp

eventFilter().sources()[i] = sources()[i];
eventFilter().filterMetadata()[i] = filterMetadata()[i];
i = i+1;
}
}


void FORTE_FieldsToEventFilter::enterStateSTART(void){
  m_nECCState = scm_nStateSTART;
}

void FORTE_FieldsToEventFilter::enterStateREQ(void){
  m_nECCState = scm_nStateREQ;
  alg_REQ();
  sendOutputEvent( scm_nEventCNFID);
}

void FORTE_FieldsToEventFilter::executeEvent(int pa_nEIID){
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


