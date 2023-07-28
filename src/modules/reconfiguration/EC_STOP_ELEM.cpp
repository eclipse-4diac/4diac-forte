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
#include "EC_STOP_ELEM.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "EC_STOP_ELEM_gen.cpp"
#endif
#include "../../stdfblib/ita/DEV_MGR.h"
#include <device.h>

DEFINE_FIRMWARE_FB(FORTE_EC_STOP_ELEM, g_nStringIdEC_STOP_ELEM)

const CStringDictionary::TStringId FORTE_EC_STOP_ELEM::scm_anDataInputNames[] = {g_nStringIdQI, g_nStringIdELEM_NAME, g_nStringIdDST};

const CStringDictionary::TStringId FORTE_EC_STOP_ELEM::scm_anDataInputTypeIds[] = {g_nStringIdBOOL, g_nStringIdWSTRING, g_nStringIdWSTRING};

const CStringDictionary::TStringId FORTE_EC_STOP_ELEM::scm_anDataOutputNames[] = {g_nStringIdQO, g_nStringIdSTATUS};

const CStringDictionary::TStringId FORTE_EC_STOP_ELEM::scm_anDataOutputTypeIds[] = {g_nStringIdBOOL, g_nStringIdWSTRING};

const TForteInt16 FORTE_EC_STOP_ELEM::scm_anEIWithIndexes[] = {0};
const TDataIOID FORTE_EC_STOP_ELEM::scm_anEIWith[] = {1, 2, 0, scmWithListDelimiter};
const CStringDictionary::TStringId FORTE_EC_STOP_ELEM::scm_anEventInputNames[] = {g_nStringIdREQ};

const TDataIOID FORTE_EC_STOP_ELEM::scm_anEOWith[] = {1, 0, scmWithListDelimiter};
const TForteInt16 FORTE_EC_STOP_ELEM::scm_anEOWithIndexes[] = {0, -1};
const CStringDictionary::TStringId FORTE_EC_STOP_ELEM::scm_anEventOutputNames[] = {g_nStringIdCNF};

const SFBInterfaceSpec FORTE_EC_STOP_ELEM::scm_stFBInterfaceSpec = {
  1,  scm_anEventInputNames,  scm_anEIWith,  scm_anEIWithIndexes,
  1,  scm_anEventOutputNames,  scm_anEOWith, scm_anEOWithIndexes,  3,  scm_anDataInputNames, scm_anDataInputTypeIds,
  2,  scm_anDataOutputNames, scm_anDataOutputTypeIds,
  0, nullptr
};


void FORTE_EC_STOP_ELEM::executeEvent(TEventID pa_nEIID){
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


void FORTE_EC_STOP_ELEM::executeRQST(){
  forte::core::SManagementCMD theCommand;
  
  theCommand.mDestination = CStringDictionary::getInstance().getId(DST().getValue());
  theCommand.mFirstParam.pushBack(CStringDictionary::getInstance().getId(ELEM_NAME().getValue()));
  theCommand.mCMD = EMGMCommandType::Stop;
  
  EMGMResponse resp = m_poDevice.executeMGMCommand(theCommand);

  //calculate return value
  QO() = CIEC_BOOL(resp == EMGMResponse::Ready);
  const std::string retVal(DEV_MGR::getResponseText(resp));
  DEVLOG_DEBUG("%s\n", retVal.c_str());
  STATUS() = CIEC_WSTRING(retVal.c_str());
}
