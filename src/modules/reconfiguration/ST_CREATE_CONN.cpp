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
#include "ST_CREATE_CONN.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "ST_CREATE_CONN_gen.cpp"
#endif
#include "../../stdfblib/ita/DEV_MGR.h"
#include <device.h>

DEFINE_FIRMWARE_FB(FORTE_ST_CREATE_CONN, g_nStringIdST_CREATE_CONN)

const CStringDictionary::TStringId FORTE_ST_CREATE_CONN::scm_anDataInputNames[] = {g_nStringIdQI, g_nStringIdSRC_FB, g_nStringIdSRC_FB_OUT, g_nStringIdDST_FB, g_nStringIdDST_FB_IN, g_nStringIdDST};

const CStringDictionary::TStringId FORTE_ST_CREATE_CONN::scm_anDataInputTypeIds[] = {g_nStringIdBOOL, g_nStringIdWSTRING, g_nStringIdWSTRING, g_nStringIdWSTRING, g_nStringIdWSTRING, g_nStringIdWSTRING};

const CStringDictionary::TStringId FORTE_ST_CREATE_CONN::scm_anDataOutputNames[] = {g_nStringIdQO, g_nStringIdSTATUS};

const CStringDictionary::TStringId FORTE_ST_CREATE_CONN::scm_anDataOutputTypeIds[] = {g_nStringIdBOOL, g_nStringIdWSTRING};

const TForteInt16 FORTE_ST_CREATE_CONN::scm_anEIWithIndexes[] = {0};
const TDataIOID FORTE_ST_CREATE_CONN::scm_anEIWith[] = {1, 2, 3, 4, 5, 0, 255};
const CStringDictionary::TStringId FORTE_ST_CREATE_CONN::scm_anEventInputNames[] = {g_nStringIdREQ};

const TDataIOID FORTE_ST_CREATE_CONN::scm_anEOWith[] = {1, 0, 255};
const TForteInt16 FORTE_ST_CREATE_CONN::scm_anEOWithIndexes[] = {0, -1};
const CStringDictionary::TStringId FORTE_ST_CREATE_CONN::scm_anEventOutputNames[] = {g_nStringIdCNF};

const SFBInterfaceSpec FORTE_ST_CREATE_CONN::scm_stFBInterfaceSpec = {
  1,  scm_anEventInputNames,  scm_anEIWith,  scm_anEIWithIndexes,
  1,  scm_anEventOutputNames,  scm_anEOWith, scm_anEOWithIndexes,  6,  scm_anDataInputNames, scm_anDataInputTypeIds,
  2,  scm_anDataOutputNames, scm_anDataOutputTypeIds,
  0, 0
};


void FORTE_ST_CREATE_CONN::executeEvent(int pa_nEIID){
  switch(pa_nEIID){
    case scm_nEventREQID:
          QO() = QI();
      if(true == QI()) {
        executeRQST();
      } else {
      STATUS() = "Not Ready";
      }
      sendOutputEvent(scm_nEventCNFID);
      break;
  }
}


void FORTE_ST_CREATE_CONN::executeRQST(void){
  forte::core::SManagementCMD theCommand;
  
  theCommand.mDestination = CStringDictionary::getInstance().getId(DST().getValue());
  theCommand.mFirstParam.pushBack(CStringDictionary::getInstance().getId(SRC_FB().getValue()));
  theCommand.mFirstParam.pushBack(CStringDictionary::getInstance().getId(SRC_FB_OUT().getValue()));
  theCommand.mSecondParam.pushBack(CStringDictionary::getInstance().getId(DST_FB().getValue()));
  theCommand.mSecondParam.pushBack(CStringDictionary::getInstance().getId(DST_FB_IN().getValue()));
  theCommand.mCMD = cg_nMGM_CMD_Create_Connection;
  
  EMGMResponse resp = m_poDevice.executeMGMCommand(theCommand);

  //calculate return value
  CIEC_STRING retVal(DEV_MGR::scm_sMGMResponseTexts[resp]);
  CIEC_STRING compareVal(DEV_MGR::scm_sMGMResponseTexts[e_RDY]);
  QO() = retVal == compareVal;

  DEVLOG_DEBUG("%s\n", DEV_MGR::scm_sMGMResponseTexts[resp]);
  STATUS() = (DEV_MGR::scm_sMGMResponseTexts[resp]);
}
