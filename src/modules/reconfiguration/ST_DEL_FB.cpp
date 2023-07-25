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
#include "ST_DEL_FB.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "ST_DEL_FB_gen.cpp"
#endif
#include "../../stdfblib/ita/DEV_MGR.h"
#include <device.h>

DEFINE_FIRMWARE_FB(FORTE_ST_DEL_FB, g_nStringIdST_DEL_FB)

const CStringDictionary::TStringId FORTE_ST_DEL_FB::scm_anDataInputNames[] = {g_nStringIdQI, g_nStringIdFB_NAME, g_nStringIdDST};

const CStringDictionary::TStringId FORTE_ST_DEL_FB::scm_anDataInputTypeIds[] = {g_nStringIdBOOL, g_nStringIdWSTRING, g_nStringIdWSTRING};

const CStringDictionary::TStringId FORTE_ST_DEL_FB::scm_anDataOutputNames[] = {g_nStringIdQO, g_nStringIdSTATUS};

const CStringDictionary::TStringId FORTE_ST_DEL_FB::scm_anDataOutputTypeIds[] = {g_nStringIdBOOL, g_nStringIdWSTRING};

const TForteInt16 FORTE_ST_DEL_FB::scm_anEIWithIndexes[] = {0};
const TDataIOID FORTE_ST_DEL_FB::scm_anEIWith[] = {1, 2, 0, scmWithListDelimiter};
const CStringDictionary::TStringId FORTE_ST_DEL_FB::scm_anEventInputNames[] = {g_nStringIdREQ};

const TDataIOID FORTE_ST_DEL_FB::scm_anEOWith[] = {1, 0, scmWithListDelimiter};
const TForteInt16 FORTE_ST_DEL_FB::scm_anEOWithIndexes[] = {0, -1};
const CStringDictionary::TStringId FORTE_ST_DEL_FB::scm_anEventOutputNames[] = {g_nStringIdCNF};

const SFBInterfaceSpec FORTE_ST_DEL_FB::scm_stFBInterfaceSpec = {
  1,  scm_anEventInputNames,  scm_anEIWith,  scm_anEIWithIndexes,
  1,  scm_anEventOutputNames,  scm_anEOWith, scm_anEOWithIndexes,  3,  scm_anDataInputNames, scm_anDataInputTypeIds,
  2,  scm_anDataOutputNames, scm_anDataOutputTypeIds,
  0, nullptr,
  0, nullptr
};


void FORTE_ST_DEL_FB::executeEvent(TEventID pa_nEIID){
  switch(pa_nEIID){
    case scm_nEventREQID:
      QO() = QI();
      if(true == QI()) {
        executeRQST();
      } else {
      STATUS() = CIEC_WSTRING("Not Ready");
      }
      sendOutputEvent(scm_nEventCNFID);
      break;
  }
}

void FORTE_ST_DEL_FB::executeRQST(){
  forte::core::SManagementCMD theCommand;
  
  theCommand.mDestination = CStringDictionary::getInstance().getId(DST().getValue());
  theCommand.mFirstParam.pushBack(CStringDictionary::getInstance().getId(FB_NAME().getValue()));
  theCommand.mCMD = EMGMCommandType::DeleteFBInstance;
  
  EMGMResponse resp = m_poDevice.executeMGMCommand(theCommand);
  
  //calculate return value
  QO() = CIEC_BOOL(resp == EMGMResponse::Ready);
  const std::string retVal(DEV_MGR::getResponseText(resp));
  DEVLOG_DEBUG("%s\n", retVal.c_str());
  STATUS() = CIEC_WSTRING(retVal.c_str());
}

