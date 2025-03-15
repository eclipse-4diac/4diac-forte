/*******************************************************************************
 * Copyright (c) 2009, 2023 ACIN, fortiss GmbH
 *                          Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Monika Wenger, Alois Zoitl,  Ingo Hengy
 *   - initial API and implementation and/or initial documentation
 *   Martin Jobst - add readInputData and writeOutputData
 *******************************************************************************/
#include "FB_TP_fbt.h"

USE_STRING_ID(BOOL);
USE_STRING_ID(CNF);
USE_STRING_ID(ET);
USE_STRING_ID(Event);
USE_STRING_ID(FB_TP);
USE_STRING_ID(IN);
USE_STRING_ID(PT);
USE_STRING_ID(Q);
USE_STRING_ID(REQ);
USE_STRING_ID(TIME);


#include "criticalregion.h"
#include "resource.h"

DEFINE_FIRMWARE_FB(FORTE_FB_TP, STRID(FB_TP))

const CStringDictionary::TStringId FORTE_FB_TP::scmDataInputNames[] = {STRID(IN), STRID(PT)};
const CStringDictionary::TStringId FORTE_FB_TP::scmDataInputTypeIds[] = {STRID(BOOL), STRID(TIME)};
const CStringDictionary::TStringId FORTE_FB_TP::scmDataOutputNames[] = {STRID(Q), STRID(ET)};
const CStringDictionary::TStringId FORTE_FB_TP::scmDataOutputTypeIds[] = {STRID(BOOL), STRID(TIME)};
const TDataIOID FORTE_FB_TP::scmEIWith[] = {0, 1, scmWithListDelimiter};
const TForteInt16 FORTE_FB_TP::scmEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_FB_TP::scmEventInputNames[] = {STRID(REQ)};
const CStringDictionary::TStringId FORTE_FB_TP::scmEventInputTypeIds[] = {STRID(Event)};
const TDataIOID FORTE_FB_TP::scmEOWith[] = {0, 1, scmWithListDelimiter};
const TForteInt16 FORTE_FB_TP::scmEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_FB_TP::scmEventOutputNames[] = {STRID(CNF)};
const CStringDictionary::TStringId FORTE_FB_TP::scmEventOutputTypeIds[] = {STRID(Event)};
const SFBInterfaceSpec FORTE_FB_TP::scmFBInterfaceSpec = {
  1, scmEventInputNames, scmEventInputTypeIds, scmEIWith, scmEIWithIndexes,
  1, scmEventOutputNames, scmEventOutputTypeIds, scmEOWith, scmEOWithIndexes,
  2, scmDataInputNames, scmDataInputTypeIds,
  2, scmDataOutputNames, scmDataOutputTypeIds,
  0, nullptr,
  0, nullptr
};

FORTE_FB_TP::FORTE_FB_TP(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    CFunctionBlock(paContainer, scmFBInterfaceSpec, paInstanceNameId),
    var_conn_Q(var_Q),
    var_conn_ET(var_ET),
    conn_CNF(this, 0),
    conn_IN(nullptr),
    conn_PT(nullptr),
    conn_Q(this, 0, &var_conn_Q),
    conn_ET(this, 1, &var_conn_ET) {
};

void FORTE_FB_TP::setInitialValues() {
  var_IN = 0_BOOL;
  var_PT = 0_TIME;
  var_Q = 0_BOOL;
  var_ET = 0_TIME;
}

void FORTE_FB_TP::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
  switch(paEIID) {
    case scmEventREQID:
      if(edgeFlag) {
        if(func_GE(var_ET, var_PT)) {
          var_Q = false_BOOL;
          edgeFlag = false;
          DEVLOG_DEBUG("top\n");
        } else {
          var_ET = func_SUB(func_NOW_MONOTONIC(), start);
          DEVLOG_DEBUG("rising\n");
        }
      } else {
        if(func_AND(var_IN, func_EQ(var_ET, 0_TIME))) {
          var_Q = true_BOOL;
          edgeFlag = true;
          start = func_NOW_MONOTONIC();
          DEVLOG_DEBUG("start\n");
        } else if(func_AND(var_IN, func_GT(var_ET, 0_TIME))) {
          var_ET = 0_TIME;
          DEVLOG_DEBUG("reset\n");
        }
      }
      sendOutputEvent(scmEventCNFID, paECET);
      break;
  }
}

void FORTE_FB_TP::readInputData(TEventID paEIID) {
  switch(paEIID) {
    case scmEventREQID: {
      readData(0, var_IN, conn_IN);
      readData(1, var_PT, conn_PT);
      break;
    }
    default:
      break;
  }
}

void FORTE_FB_TP::writeOutputData(TEventID paEIID) {
  switch(paEIID) {
    case scmEventCNFID: {
      writeData(0, var_Q, conn_Q);
      writeData(1, var_ET, conn_ET);
      break;
    }
    default:
      break;
  }
}

CIEC_ANY *FORTE_FB_TP::getDI(size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_IN;
    case 1: return &var_PT;
  }
  return nullptr;
}

CIEC_ANY *FORTE_FB_TP::getDO(size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_Q;
    case 1: return &var_ET;
  }
  return nullptr;
}

CEventConnection *FORTE_FB_TP::getEOConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_FB_TP::getDIConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_IN;
    case 1: return &conn_PT;
  }
  return nullptr;
}

CDataConnection *FORTE_FB_TP::getDOConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_Q;
    case 1: return &conn_ET;
  }
  return nullptr;
}

