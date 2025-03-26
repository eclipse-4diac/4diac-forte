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
#include "ST_SET_PARM_fbt.h"

USE_STRING_ID(BOOL);
USE_STRING_ID(CNF);
USE_STRING_ID(DST);
USE_STRING_ID(ELEM_DATA_IN);
USE_STRING_ID(ELEM_NAME);
USE_STRING_ID(Event);
USE_STRING_ID(PARM_VAL);
USE_STRING_ID(QI);
USE_STRING_ID(QO);
USE_STRING_ID(REQ);
USE_STRING_ID(STATUS);
USE_STRING_ID(ST_SET_PARM);
USE_STRING_ID(WSTRING);


#include "criticalregion.h"
#include "resource.h"

#include "device.h"

DEFINE_FIRMWARE_FB(FORTE_ST_SET_PARM, STRID(ST_SET_PARM))

const CStringDictionary::TStringId FORTE_ST_SET_PARM::scmDataInputNames[] = {STRID(QI), STRID(ELEM_NAME), STRID(ELEM_DATA_IN), STRID(PARM_VAL), STRID(DST)};
const CStringDictionary::TStringId FORTE_ST_SET_PARM::scmDataInputTypeIds[] = {STRID(BOOL), STRID(WSTRING), STRID(WSTRING), STRID(WSTRING), STRID(WSTRING)};
const CStringDictionary::TStringId FORTE_ST_SET_PARM::scmDataOutputNames[] = {STRID(QO), STRID(STATUS)};
const CStringDictionary::TStringId FORTE_ST_SET_PARM::scmDataOutputTypeIds[] = {STRID(BOOL), STRID(WSTRING)};
const TDataIOID FORTE_ST_SET_PARM::scmEIWith[] = {1, 2, 3, 4, 0, scmWithListDelimiter};
const TForteInt16 FORTE_ST_SET_PARM::scmEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_ST_SET_PARM::scmEventInputNames[] = {STRID(REQ)};
const CStringDictionary::TStringId FORTE_ST_SET_PARM::scmEventInputTypeIds[] = {STRID(Event)};
const TDataIOID FORTE_ST_SET_PARM::scmEOWith[] = {1, 0, scmWithListDelimiter};
const TForteInt16 FORTE_ST_SET_PARM::scmEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_ST_SET_PARM::scmEventOutputNames[] = {STRID(CNF)};
const CStringDictionary::TStringId FORTE_ST_SET_PARM::scmEventOutputTypeIds[] = {STRID(Event)};
const SFBInterfaceSpec FORTE_ST_SET_PARM::scmFBInterfaceSpec = {
  1, scmEventInputNames, scmEventInputTypeIds, scmEIWith, scmEIWithIndexes,
  1, scmEventOutputNames, scmEventOutputTypeIds, scmEOWith, scmEOWithIndexes,
  5, scmDataInputNames, scmDataInputTypeIds,
  2, scmDataOutputNames, scmDataOutputTypeIds,
  0, nullptr,
  0, nullptr
};

FORTE_ST_SET_PARM::FORTE_ST_SET_PARM(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    CFunctionBlock(paContainer, scmFBInterfaceSpec, paInstanceNameId),
    conn_CNF(*this, 0),
    conn_QI(nullptr),
    conn_ELEM_NAME(nullptr),
    conn_ELEM_DATA_IN(nullptr),
    conn_PARM_VAL(nullptr),
    conn_DST(nullptr),
    conn_QO(*this, 0, var_QO),
    conn_STATUS(*this, 1, var_STATUS) {
};

void FORTE_ST_SET_PARM::setInitialValues() {
  var_QI = 0_BOOL;
  var_ELEM_NAME = u""_WSTRING;
  var_ELEM_DATA_IN = u""_WSTRING;
  var_PARM_VAL = u""_WSTRING;
  var_DST = u""_WSTRING;
  var_QO = 0_BOOL;
  var_STATUS = u""_WSTRING;
}

void FORTE_ST_SET_PARM::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
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

void FORTE_ST_SET_PARM::executeRQST() {
  forte::core::SManagementCMD theCommand;
  theCommand.mDestination = CStringDictionary::getId(var_DST.getValue());
  theCommand.mFirstParam.push_back(CStringDictionary::getId(var_ELEM_NAME.getValue()));
  theCommand.mFirstParam.push_back(CStringDictionary::getId(var_ELEM_DATA_IN.getValue()));
  theCommand.mAdditionalParams = func_WSTRING_TO_STRING(var_PARM_VAL).getStorage();
  theCommand.mCMD = EMGMCommandType::Write;

  EMGMResponse resp = getDevice()->executeMGMCommand(theCommand);

  //calculate return value
  var_QO = CIEC_BOOL(resp == EMGMResponse::Ready);
  const std::string retVal(forte::mgm_cmd::getResponseText(resp));
  DEVLOG_DEBUG("%s\n", retVal.c_str());
  var_STATUS = CIEC_WSTRING(retVal.c_str());
}

void FORTE_ST_SET_PARM::readInputData(TEventID paEIID) {
  switch(paEIID) {
    case scmEventREQID: {
      readData(1, var_ELEM_NAME, conn_ELEM_NAME);
      readData(2, var_ELEM_DATA_IN, conn_ELEM_DATA_IN);
      readData(3, var_PARM_VAL, conn_PARM_VAL);
      readData(4, var_DST, conn_DST);
      readData(0, var_QI, conn_QI);
      break;
    }
    default:
      break;
  }
}

void FORTE_ST_SET_PARM::writeOutputData(TEventID paEIID) {
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

CIEC_ANY *FORTE_ST_SET_PARM::getDI(size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_QI;
    case 1: return &var_ELEM_NAME;
    case 2: return &var_ELEM_DATA_IN;
    case 3: return &var_PARM_VAL;
    case 4: return &var_DST;
  }
  return nullptr;
}

CIEC_ANY *FORTE_ST_SET_PARM::getDO(size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_QO;
    case 1: return &var_STATUS;
  }
  return nullptr;
}

CEventConnection *FORTE_ST_SET_PARM::getEOConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_ST_SET_PARM::getDIConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_QI;
    case 1: return &conn_ELEM_NAME;
    case 2: return &conn_ELEM_DATA_IN;
    case 3: return &conn_PARM_VAL;
    case 4: return &conn_DST;
  }
  return nullptr;
}

CDataConnection *FORTE_ST_SET_PARM::getDOConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_QO;
    case 1: return &conn_STATUS;
  }
  return nullptr;
}


