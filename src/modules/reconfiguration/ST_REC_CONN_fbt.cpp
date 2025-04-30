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
#include "ST_REC_CONN_fbt.h"

USE_STRING_ID(BOOL);
USE_STRING_ID(CNF);
USE_STRING_ID(DST);
USE_STRING_ID(Event);
USE_STRING_ID(NEW_DST_FB);
USE_STRING_ID(NEW_DST_FB_IN);
USE_STRING_ID(NEW_SRC_FB);
USE_STRING_ID(NEW_SRC_FB_OUT);
USE_STRING_ID(OLD_DST_FB);
USE_STRING_ID(OLD_DST_FB_IN);
USE_STRING_ID(OLD_SRC_FB);
USE_STRING_ID(OLD_SRC_FB_OUT);
USE_STRING_ID(QI);
USE_STRING_ID(QO);
USE_STRING_ID(REQ);
USE_STRING_ID(STATUS);
USE_STRING_ID(ST_REC_CONN);
USE_STRING_ID(WSTRING);


#include "device.h"

DEFINE_FIRMWARE_FB(FORTE_ST_REC_CONN, STRID(ST_REC_CONN))

const CStringDictionary::TStringId FORTE_ST_REC_CONN::scmDataInputNames[] = {
    STRID(QI),         STRID(OLD_SRC_FB),     STRID(OLD_SRC_FB_OUT), STRID(OLD_DST_FB),    STRID(OLD_DST_FB_IN),
    STRID(NEW_SRC_FB), STRID(NEW_SRC_FB_OUT), STRID(NEW_DST_FB),     STRID(NEW_DST_FB_IN), STRID(DST)};
const CStringDictionary::TStringId FORTE_ST_REC_CONN::scmDataInputTypeIds[] = {
    STRID(BOOL),    STRID(WSTRING), STRID(WSTRING), STRID(WSTRING), STRID(WSTRING),
    STRID(WSTRING), STRID(WSTRING), STRID(WSTRING), STRID(WSTRING), STRID(WSTRING)};
const CStringDictionary::TStringId FORTE_ST_REC_CONN::scmDataOutputNames[] = {STRID(QO), STRID(STATUS)};
const CStringDictionary::TStringId FORTE_ST_REC_CONN::scmDataOutputTypeIds[] = {STRID(BOOL), STRID(WSTRING)};
const TDataIOID FORTE_ST_REC_CONN::scmEIWith[] = {1, 2, 3, 4, 9, 5, 6, 7, 8, 0, scmWithListDelimiter};
const TForteInt16 FORTE_ST_REC_CONN::scmEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_ST_REC_CONN::scmEventInputNames[] = {STRID(REQ)};
const CStringDictionary::TStringId FORTE_ST_REC_CONN::scmEventInputTypeIds[] = {STRID(Event)};
const TDataIOID FORTE_ST_REC_CONN::scmEOWith[] = {1, 0, scmWithListDelimiter};
const TForteInt16 FORTE_ST_REC_CONN::scmEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_ST_REC_CONN::scmEventOutputNames[] = {STRID(CNF)};
const CStringDictionary::TStringId FORTE_ST_REC_CONN::scmEventOutputTypeIds[] = {STRID(Event)};
const SFBInterfaceSpec FORTE_ST_REC_CONN::scmFBInterfaceSpec = {1,
                                                                scmEventInputNames,
                                                                scmEventInputTypeIds,
                                                                scmEIWith,
                                                                scmEIWithIndexes,
                                                                1,
                                                                scmEventOutputNames,
                                                                scmEventOutputTypeIds,
                                                                scmEOWith,
                                                                scmEOWithIndexes,
                                                                10,
                                                                scmDataInputNames,
                                                                scmDataInputTypeIds,
                                                                2,
                                                                scmDataOutputNames,
                                                                scmDataOutputTypeIds,
                                                                0,
                                                                nullptr,
                                                                0,
                                                                nullptr};

FORTE_ST_REC_CONN::FORTE_ST_REC_CONN(const CStringDictionary::TStringId paInstanceNameId,
                                     forte::core::CFBContainer &paContainer) :
    CFunctionBlock(paContainer, scmFBInterfaceSpec, paInstanceNameId),
    conn_CNF(*this, 0),
    conn_QI(nullptr),
    conn_OLD_SRC_FB(nullptr),
    conn_OLD_SRC_FB_OUT(nullptr),
    conn_OLD_DST_FB(nullptr),
    conn_OLD_DST_FB_IN(nullptr),
    conn_NEW_SRC_FB(nullptr),
    conn_NEW_SRC_FB_OUT(nullptr),
    conn_NEW_DST_FB(nullptr),
    conn_NEW_DST_FB_IN(nullptr),
    conn_DST(nullptr),
    conn_QO(*this, 0, var_QO),
    conn_STATUS(*this, 1, var_STATUS) {};

void FORTE_ST_REC_CONN::setInitialValues() {
  var_QI = 0_BOOL;
  var_OLD_SRC_FB = u""_WSTRING;
  var_OLD_SRC_FB_OUT = u""_WSTRING;
  var_OLD_DST_FB = u""_WSTRING;
  var_OLD_DST_FB_IN = u""_WSTRING;
  var_NEW_SRC_FB = u""_WSTRING;
  var_NEW_SRC_FB_OUT = u""_WSTRING;
  var_NEW_DST_FB = u""_WSTRING;
  var_NEW_DST_FB_IN = u""_WSTRING;
  var_DST = u""_WSTRING;
  var_QO = 0_BOOL;
  var_STATUS = u""_WSTRING;
}

void FORTE_ST_REC_CONN::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
  switch (paEIID) {
    case scmEventREQID:
      var_QO = var_QI;
      if (var_QI) {
        executeRQST();
      } else {
        var_STATUS = u"Not Ready"_WSTRING;
      }
      sendOutputEvent(scmEventCNFID, paECET);
      break;
  }
}

void FORTE_ST_REC_CONN::executeRQST() {
  forte::core::SManagementCMD theCommand;
  // delete old connection
  theCommand.mDestination = CStringDictionary::getId(var_DST.getValue());
  theCommand.mFirstParam.push_back(CStringDictionary::getId(var_OLD_SRC_FB.getValue()));
  theCommand.mFirstParam.push_back(CStringDictionary::getId(var_OLD_SRC_FB_OUT.getValue()));
  theCommand.mSecondParam.push_back(CStringDictionary::getId(var_OLD_DST_FB.getValue()));
  theCommand.mSecondParam.push_back(CStringDictionary::getId(var_OLD_DST_FB_IN.getValue()));
  theCommand.mCMD = EMGMCommandType::DeleteConnection;

  EMGMResponse resp = getDevice()->executeMGMCommand(theCommand);

  if (resp == EMGMResponse::Ready) {
    // create new connection
    theCommand.mDestination = CStringDictionary::getId(var_DST.getValue());
    theCommand.mFirstParam.clear();
    theCommand.mFirstParam.push_back(CStringDictionary::getId(var_NEW_SRC_FB.getValue()));
    theCommand.mFirstParam.push_back(CStringDictionary::getId(var_NEW_SRC_FB_OUT.getValue()));
    theCommand.mSecondParam.push_back(CStringDictionary::getId(var_NEW_DST_FB.getValue()));
    theCommand.mSecondParam.push_back(CStringDictionary::getId(var_NEW_DST_FB_IN.getValue()));
    theCommand.mCMD = EMGMCommandType::CreateConnection;
    resp = getDevice()->executeMGMCommand(theCommand);
  }

  // calculate return value
  var_QO = CIEC_BOOL(resp == EMGMResponse::Ready);
  const std::string retVal(forte::mgm_cmd::getResponseText(resp));
  DEVLOG_DEBUG("%s\n", retVal.c_str());
  var_STATUS = CIEC_WSTRING(retVal.c_str());
}

void FORTE_ST_REC_CONN::readInputData(TEventID paEIID) {
  switch (paEIID) {
    case scmEventREQID: {
      readData(1, var_OLD_SRC_FB, conn_OLD_SRC_FB);
      readData(2, var_OLD_SRC_FB_OUT, conn_OLD_SRC_FB_OUT);
      readData(3, var_OLD_DST_FB, conn_OLD_DST_FB);
      readData(4, var_OLD_DST_FB_IN, conn_OLD_DST_FB_IN);
      readData(9, var_DST, conn_DST);
      readData(5, var_NEW_SRC_FB, conn_NEW_SRC_FB);
      readData(6, var_NEW_SRC_FB_OUT, conn_NEW_SRC_FB_OUT);
      readData(7, var_NEW_DST_FB, conn_NEW_DST_FB);
      readData(8, var_NEW_DST_FB_IN, conn_NEW_DST_FB_IN);
      readData(0, var_QI, conn_QI);
      break;
    }
    default: break;
  }
}

void FORTE_ST_REC_CONN::writeOutputData(TEventID paEIID) {
  switch (paEIID) {
    case scmEventCNFID: {
      writeData(scmFBInterfaceSpec.mNumDIs + 1, var_STATUS, conn_STATUS);
      writeData(scmFBInterfaceSpec.mNumDIs + 0, var_QO, conn_QO);
      break;
    }
    default: break;
  }
}

CIEC_ANY *FORTE_ST_REC_CONN::getDI(size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_QI;
    case 1: return &var_OLD_SRC_FB;
    case 2: return &var_OLD_SRC_FB_OUT;
    case 3: return &var_OLD_DST_FB;
    case 4: return &var_OLD_DST_FB_IN;
    case 5: return &var_NEW_SRC_FB;
    case 6: return &var_NEW_SRC_FB_OUT;
    case 7: return &var_NEW_DST_FB;
    case 8: return &var_NEW_DST_FB_IN;
    case 9: return &var_DST;
  }
  return nullptr;
}

CIEC_ANY *FORTE_ST_REC_CONN::getDO(size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_QO;
    case 1: return &var_STATUS;
  }
  return nullptr;
}

CEventConnection *FORTE_ST_REC_CONN::getEOConUnchecked(TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_ST_REC_CONN::getDIConUnchecked(TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_QI;
    case 1: return &conn_OLD_SRC_FB;
    case 2: return &conn_OLD_SRC_FB_OUT;
    case 3: return &conn_OLD_DST_FB;
    case 4: return &conn_OLD_DST_FB_IN;
    case 5: return &conn_NEW_SRC_FB;
    case 6: return &conn_NEW_SRC_FB_OUT;
    case 7: return &conn_NEW_DST_FB;
    case 8: return &conn_NEW_DST_FB_IN;
    case 9: return &conn_DST;
  }
  return nullptr;
}

CDataConnection *FORTE_ST_REC_CONN::getDOConUnchecked(TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_QO;
    case 1: return &conn_STATUS;
  }
  return nullptr;
}
