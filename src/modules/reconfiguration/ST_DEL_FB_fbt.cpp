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
#include "ST_DEL_FB_fbt.h"

USE_STRING_ID(BOOL);
USE_STRING_ID(CNF);
USE_STRING_ID(DST);
USE_STRING_ID(Event);
USE_STRING_ID(FB_NAME);
USE_STRING_ID(QI);
USE_STRING_ID(QO);
USE_STRING_ID(REQ);
USE_STRING_ID(STATUS);
USE_STRING_ID(ST_DEL_FB);
USE_STRING_ID(WSTRING);


#include "criticalregion.h"
#include "resource.h"

#include "device.h"

DEFINE_FIRMWARE_FB(FORTE_ST_DEL_FB, STRID(ST_DEL_FB))

const CStringDictionary::TStringId FORTE_ST_DEL_FB::scmDataInputNames[] = {STRID(QI), STRID(FB_NAME), STRID(DST)};
const CStringDictionary::TStringId FORTE_ST_DEL_FB::scmDataInputTypeIds[] = {STRID(BOOL), STRID(WSTRING), STRID(WSTRING)};
const CStringDictionary::TStringId FORTE_ST_DEL_FB::scmDataOutputNames[] = {STRID(QO), STRID(STATUS)};
const CStringDictionary::TStringId FORTE_ST_DEL_FB::scmDataOutputTypeIds[] = {STRID(BOOL), STRID(WSTRING)};
const TDataIOID FORTE_ST_DEL_FB::scmEIWith[] = {1, 2, 0, scmWithListDelimiter};
const TForteInt16 FORTE_ST_DEL_FB::scmEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_ST_DEL_FB::scmEventInputNames[] = {STRID(REQ)};
const CStringDictionary::TStringId FORTE_ST_DEL_FB::scmEventInputTypeIds[] = {STRID(Event)};
const TDataIOID FORTE_ST_DEL_FB::scmEOWith[] = {1, 0, scmWithListDelimiter};
const TForteInt16 FORTE_ST_DEL_FB::scmEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_ST_DEL_FB::scmEventOutputNames[] = {STRID(CNF)};
const CStringDictionary::TStringId FORTE_ST_DEL_FB::scmEventOutputTypeIds[] = {STRID(Event)};
const SFBInterfaceSpec FORTE_ST_DEL_FB::scmFBInterfaceSpec = {
  1, scmEventInputNames, scmEventInputTypeIds, scmEIWith, scmEIWithIndexes,
  1, scmEventOutputNames, scmEventOutputTypeIds, scmEOWith, scmEOWithIndexes,
  3, scmDataInputNames, scmDataInputTypeIds,
  2, scmDataOutputNames, scmDataOutputTypeIds,
  0, nullptr,
  0, nullptr
};

FORTE_ST_DEL_FB::FORTE_ST_DEL_FB(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    CFunctionBlock(paContainer, scmFBInterfaceSpec, paInstanceNameId),
    conn_CNF(this, 0),
    conn_QI(nullptr),
    conn_FB_NAME(nullptr),
    conn_DST(nullptr),
    conn_QO(this, 0, var_QO),
    conn_STATUS(this, 1, var_STATUS) {
};

void FORTE_ST_DEL_FB::setInitialValues() {
  var_QI = 0_BOOL;
  var_FB_NAME = u""_WSTRING;
  var_DST = u""_WSTRING;
  var_QO = 0_BOOL;
  var_STATUS = u""_WSTRING;
}

void FORTE_ST_DEL_FB::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
  switch(paEIID) {
    case scmEventREQID:
      var_QO = var_QI;
      if(var_QI) {
        executeRQST();
      } else {
        var_STATUS = u"Not Ready"_WSTRING;
      }
      sendOutputEvent(scmEventCNFID, paECET);
      break;
  }
}

void FORTE_ST_DEL_FB::executeRQST(){
  forte::core::SManagementCMD theCommand;

  theCommand.mDestination = CStringDictionary::getId(var_DST.getValue());
  theCommand.mFirstParam.push_back(CStringDictionary::getId(var_FB_NAME.getValue()));
  theCommand.mCMD = EMGMCommandType::DeleteFBInstance;

  EMGMResponse resp = getDevice()->executeMGMCommand(theCommand);

  //calculate return value
  var_QO = CIEC_BOOL(resp == EMGMResponse::Ready);
  const std::string retVal(forte::mgm_cmd::getResponseText(resp));
  DEVLOG_DEBUG("%s\n", retVal.c_str());
  var_STATUS = CIEC_WSTRING(retVal.c_str());
}

void FORTE_ST_DEL_FB::readInputData(TEventID paEIID) {
  switch(paEIID) {
    case scmEventREQID: {
      readData(1, var_FB_NAME, conn_FB_NAME);
      readData(2, var_DST, conn_DST);
      readData(0, var_QI, conn_QI);
      break;
    }
    default:
      break;
  }
}

void FORTE_ST_DEL_FB::writeOutputData(TEventID paEIID) {
  switch(paEIID) {
    case scmEventCNFID: {
      writeData(1, var_STATUS, conn_STATUS);
      writeData(0, var_QO, conn_QO);
      break;
    }
    default:
      break;
  }
}

CIEC_ANY *FORTE_ST_DEL_FB::getDI(size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_QI;
    case 1: return &var_FB_NAME;
    case 2: return &var_DST;
  }
  return nullptr;
}

CIEC_ANY *FORTE_ST_DEL_FB::getDO(size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_QO;
    case 1: return &var_STATUS;
  }
  return nullptr;
}

CEventConnection *FORTE_ST_DEL_FB::getEOConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_ST_DEL_FB::getDIConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_QI;
    case 1: return &conn_FB_NAME;
    case 2: return &conn_DST;
  }
  return nullptr;
}

CDataConnection *FORTE_ST_DEL_FB::getDOConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_QO;
    case 1: return &conn_STATUS;
  }
  return nullptr;
}


