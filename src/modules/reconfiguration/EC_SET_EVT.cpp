/*******************************************************************************
 * Copyright (c) 2014, 2015 Profactor GmbH, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Matthias Plasch, Gerhard Ebenhofer, Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "EC_SET_EVT.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "EC_SET_EVT_gen.cpp"
#endif
#include "../../stdfblib/ita/DEV_MGR.h"
#include <mgmcmdstruct.h>
#include <device.h>

DEFINE_FIRMWARE_FB(FORTE_EC_SET_EVT, g_nStringIdEC_SET_EVT)

const CStringDictionary::TStringId FORTE_EC_SET_EVT::scm_anDataInputNames[] = {g_nStringIdQI, g_nStringIdFB_NAME, g_nStringIdFB_EVENT_IO, g_nStringIdDST};

const CStringDictionary::TStringId FORTE_EC_SET_EVT::scm_anDataInputTypeIds[] = {g_nStringIdBOOL, g_nStringIdWSTRING, g_nStringIdWSTRING, g_nStringIdWSTRING};

const CStringDictionary::TStringId FORTE_EC_SET_EVT::scm_anDataOutputNames[] = {g_nStringIdQO, g_nStringIdSTATUS};

const CStringDictionary::TStringId FORTE_EC_SET_EVT::scm_anDataOutputTypeIds[] = {g_nStringIdBOOL, g_nStringIdWSTRING};

const TForteInt16 FORTE_EC_SET_EVT::scm_anEIWithIndexes[] = {0};
const TDataIOID FORTE_EC_SET_EVT::scm_anEIWith[] = {0, 3, 1, 2, scmWithListDelimiter};
const CStringDictionary::TStringId FORTE_EC_SET_EVT::scm_anEventInputNames[] = {g_nStringIdREQ};

const TDataIOID FORTE_EC_SET_EVT::scm_anEOWith[] = {0, 1, scmWithListDelimiter};
const TForteInt16 FORTE_EC_SET_EVT::scm_anEOWithIndexes[] = {0, -1};
const CStringDictionary::TStringId FORTE_EC_SET_EVT::scm_anEventOutputNames[] = {g_nStringIdCNF};

const SFBInterfaceSpec FORTE_EC_SET_EVT::scm_stFBInterfaceSpec = {
  1,  scm_anEventInputNames,  scm_anEIWith,  scm_anEIWithIndexes,
  1,  scm_anEventOutputNames,  scm_anEOWith, scm_anEOWithIndexes,  4,  scm_anDataInputNames, scm_anDataInputTypeIds,
  2,  scm_anDataOutputNames, scm_anDataOutputTypeIds,
  0, nullptr
};


void FORTE_EC_SET_EVT::executeEvent(TEventID pa_nEIID){
  switch(pa_nEIID){
    case scm_nEventREQID:
      QO() = QI();
      if(true == QI()){
        executeRQST();
      }else{
        STATUS() = CIEC_WSTRING("NOT Ready");
      }
      sendOutputEvent(scm_nEventCNFID);
      break;
  }
}

void FORTE_EC_SET_EVT::executeRQST(){
  forte::core::SManagementCMD theCommand;
  theCommand.mDestination = CStringDictionary::getInstance().getId(DST().getValue());
  theCommand.mFirstParam.pushBack(CStringDictionary::getInstance().getId(FB_NAME().getValue()));
  theCommand.mFirstParam.pushBack(CStringDictionary::getInstance().getId(FB_EVENT_IO().getValue()));
  theCommand.mCMD = EMGMCommandType::MonitoringTriggerEvent;
  
  EMGMResponse resp =  m_poDevice.executeMGMCommand(theCommand);

  //calculate return value
  QO() = CIEC_BOOL(resp == EMGMResponse::Ready);
  const std::string retVal(DEV_MGR::getResponseText(resp));
  DEVLOG_DEBUG("%s\n", retVal.c_str());
  STATUS() = CIEC_WSTRING(retVal.c_str());
}



