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

#include "FieldsToArrowheadCloud.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "FieldsToArrowheadCloud_gen.cpp"
#endif

DEFINE_FIRMWARE_FB(FORTE_FieldsToArrowheadCloud, g_nStringIdFieldsToArrowheadCloud)

const CStringDictionary::TStringId FORTE_FieldsToArrowheadCloud::scm_anDataInputNames[] = {g_nStringIdOperator, g_nStringIdcloudName, g_nStringIdaddress, g_nStringIdport, g_nStringIdgatekeeperServiceURI, g_nStringIdauthenticationInfo, g_nStringIdsecure};

const CStringDictionary::TStringId FORTE_FieldsToArrowheadCloud::scm_anDataInputTypeIds[] = {g_nStringIdWSTRING, g_nStringIdWSTRING, g_nStringIdWSTRING, g_nStringIdDINT, g_nStringIdWSTRING, g_nStringIdWSTRING, g_nStringIdBOOL};

const CStringDictionary::TStringId FORTE_FieldsToArrowheadCloud::scm_anDataOutputNames[] = {g_nStringIdarrowheadCloud};

const CStringDictionary::TStringId FORTE_FieldsToArrowheadCloud::scm_anDataOutputTypeIds[] = {g_nStringIdArrowheadCloud};

const TForteInt16 FORTE_FieldsToArrowheadCloud::scm_anEIWithIndexes[] = {0};
const TDataIOID FORTE_FieldsToArrowheadCloud::scm_anEIWith[] = {0, 1, 2, 6, 5, 4, 3, 255};
const CStringDictionary::TStringId FORTE_FieldsToArrowheadCloud::scm_anEventInputNames[] = {g_nStringIdREQ};

const TDataIOID FORTE_FieldsToArrowheadCloud::scm_anEOWith[] = {0, 255};
const TForteInt16 FORTE_FieldsToArrowheadCloud::scm_anEOWithIndexes[] = {0, -1};
const CStringDictionary::TStringId FORTE_FieldsToArrowheadCloud::scm_anEventOutputNames[] = {g_nStringIdCNF};

const SFBInterfaceSpec FORTE_FieldsToArrowheadCloud::scm_stFBInterfaceSpec = {
  1,  scm_anEventInputNames,  scm_anEIWith,  scm_anEIWithIndexes,
  1,  scm_anEventOutputNames,  scm_anEOWith, scm_anEOWithIndexes,  7,  scm_anDataInputNames, scm_anDataInputTypeIds,
  1,  scm_anDataOutputNames, scm_anDataOutputTypeIds,
  0, 0
};

void FORTE_FieldsToArrowheadCloud::alg_REQ(void){
arrowheadCloud().Operator() = Operator();
arrowheadCloud().cloudName() = cloudName();
arrowheadCloud().address() = address();
arrowheadCloud().port() = port();
arrowheadCloud().gatekeeperServiceURI() = gatekeeperServiceURI();
arrowheadCloud().authenticationInfo() = authenticationInfo();
arrowheadCloud().secure() = secure();





}


void FORTE_FieldsToArrowheadCloud::enterStateSTART(void){
  m_nECCState = scm_nStateSTART;
}

void FORTE_FieldsToArrowheadCloud::enterStateREQ(void){
  m_nECCState = scm_nStateREQ;
  alg_REQ();
  sendOutputEvent( scm_nEventCNFID);
}

void FORTE_FieldsToArrowheadCloud::executeEvent(int pa_nEIID){
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


