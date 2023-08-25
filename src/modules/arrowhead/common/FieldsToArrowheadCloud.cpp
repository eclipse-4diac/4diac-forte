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

const CStringDictionary::TStringId FORTE_FieldsToArrowheadCloud::scmDataInputNames[] = {g_nStringIdOperator, g_nStringIdcloudName, g_nStringIdaddress, g_nStringIdport, g_nStringIdgatekeeperServiceURI, g_nStringIdauthenticationInfo, g_nStringIdsecure};

const CStringDictionary::TStringId FORTE_FieldsToArrowheadCloud::scmDataInputTypeIds[] = {g_nStringIdWSTRING, g_nStringIdWSTRING, g_nStringIdWSTRING, g_nStringIdDINT, g_nStringIdWSTRING, g_nStringIdWSTRING, g_nStringIdBOOL};

const CStringDictionary::TStringId FORTE_FieldsToArrowheadCloud::scmDataOutputNames[] = {g_nStringIdarrowheadCloud};

const CStringDictionary::TStringId FORTE_FieldsToArrowheadCloud::scmDataOutputTypeIds[] = {g_nStringIdArrowheadCloud};

const TForteInt16 FORTE_FieldsToArrowheadCloud::scmEIWithIndexes[] = {0};
const TDataIOID FORTE_FieldsToArrowheadCloud::scmEIWith[] = {0, 1, 2, 6, 5, 4, 3, scmWithListDelimiter};
const CStringDictionary::TStringId FORTE_FieldsToArrowheadCloud::scmEventInputNames[] = {g_nStringIdREQ};

const TDataIOID FORTE_FieldsToArrowheadCloud::scmEOWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_FieldsToArrowheadCloud::scmEOWithIndexes[] = {0, -1};
const CStringDictionary::TStringId FORTE_FieldsToArrowheadCloud::scmEventOutputNames[] = {g_nStringIdCNF};

const SFBInterfaceSpec FORTE_FieldsToArrowheadCloud::scmFBInterfaceSpec = {
  1,  scmEventInputNames,  scmEIWith,  scmEIWithIndexes,
  1,  scmEventOutputNames,  scmEOWith, scmEOWithIndexes,  7,  scmDataInputNames, scmDataInputTypeIds,
  1,  scmDataOutputNames, scmDataOutputTypeIds,
  0, 0
};

void FORTE_FieldsToArrowheadCloud::alg_REQ(){
arrowheadCloud().Operator() = Operator();
arrowheadCloud().cloudName() = cloudName();
arrowheadCloud().address() = address();
arrowheadCloud().port() = port();
arrowheadCloud().gatekeeperServiceURI() = gatekeeperServiceURI();
arrowheadCloud().authenticationInfo() = authenticationInfo();
arrowheadCloud().secure() = secure();





}


void FORTE_FieldsToArrowheadCloud::enterStateSTART(){
  mECCState = scmStateSTART;
}

void FORTE_FieldsToArrowheadCloud::enterStateREQ(){
  mECCState = scmStateREQ;
  alg_REQ();
  sendOutputEvent( scmEventCNFID);
}

void FORTE_FieldsToArrowheadCloud::executeEvent(TEventID paEIID){
  bool bTransitionCleared;
  do{
    bTransitionCleared = true;
    switch(mECCState){
      case scmStateSTART:
        if(scmEventREQID == paEIID)
          enterStateREQ();
        else
          bTransitionCleared  = false; //no transition cleared
        break;
      case scmStateREQ:
        if(1)
          enterStateSTART();
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


