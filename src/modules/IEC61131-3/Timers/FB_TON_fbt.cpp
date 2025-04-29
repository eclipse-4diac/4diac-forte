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
#include "FB_TON_fbt.h"

USE_STRING_ID(BOOL);
USE_STRING_ID(CNF);
USE_STRING_ID(ET);
USE_STRING_ID(Event);
USE_STRING_ID(FB_TON);
USE_STRING_ID(IN);
USE_STRING_ID(PT);
USE_STRING_ID(Q);
USE_STRING_ID(REQ);
USE_STRING_ID(TIME);


DEFINE_FIRMWARE_FB(FORTE_FB_TON, STRID(FB_TON))

const CStringDictionary::TStringId FORTE_FB_TON::scmDataInputNames[] = {STRID(IN), STRID(PT)};
const CStringDictionary::TStringId FORTE_FB_TON::scmDataInputTypeIds[] = {STRID(BOOL), STRID(TIME)};
const CStringDictionary::TStringId FORTE_FB_TON::scmDataOutputNames[] = {STRID(Q), STRID(ET)};
const CStringDictionary::TStringId FORTE_FB_TON::scmDataOutputTypeIds[] = {STRID(BOOL), STRID(TIME)};
const TDataIOID FORTE_FB_TON::scmEIWith[] = {0, 1, scmWithListDelimiter};
const TForteInt16 FORTE_FB_TON::scmEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_FB_TON::scmEventInputNames[] = {STRID(REQ)};
const CStringDictionary::TStringId FORTE_FB_TON::scmEventInputTypeIds[] = {STRID(Event)};
const TDataIOID FORTE_FB_TON::scmEOWith[] = {0, 1, scmWithListDelimiter};
const TForteInt16 FORTE_FB_TON::scmEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_FB_TON::scmEventOutputNames[] = {STRID(CNF)};
const CStringDictionary::TStringId FORTE_FB_TON::scmEventOutputTypeIds[] = {STRID(Event)};
const SFBInterfaceSpec FORTE_FB_TON::scmFBInterfaceSpec = {1,
                                                           scmEventInputNames,
                                                           scmEventInputTypeIds,
                                                           scmEIWith,
                                                           scmEIWithIndexes,
                                                           1,
                                                           scmEventOutputNames,
                                                           scmEventOutputTypeIds,
                                                           scmEOWith,
                                                           scmEOWithIndexes,
                                                           2,
                                                           scmDataInputNames,
                                                           scmDataInputTypeIds,
                                                           2,
                                                           scmDataOutputNames,
                                                           scmDataOutputTypeIds,
                                                           0,
                                                           nullptr,
                                                           0,
                                                           nullptr};

FORTE_FB_TON::FORTE_FB_TON(const CStringDictionary::TStringId paInstanceNameId,
                           forte::core::CFBContainer &paContainer) :
    CFunctionBlock(paContainer, scmFBInterfaceSpec, paInstanceNameId),
    conn_CNF(*this, 0),
    conn_IN(nullptr),
    conn_PT(nullptr),
    conn_Q(*this, 0, var_Q),
    conn_ET(*this, 1, var_ET) {};

void FORTE_FB_TON::setInitialValues() {
  var_IN = 0_BOOL;
  var_PT = 0_TIME;
  var_Q = 0_BOOL;
  var_ET = 0_TIME;
}

void FORTE_FB_TON::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
  switch (paEIID) {
    case scmEventREQID:
      if (!var_IN) {
        var_Q = false_BOOL;
        var_ET = 0_TIME;
        risingEdge = false;
        start = 0_TIME;
      } else {
        if (!risingEdge) {
          risingEdge = true;
          start = func_NOW_MONOTONIC();
        } else {
          count = func_SUB(func_NOW_MONOTONIC(), start);
          if (func_LE(var_PT, count)) {
            var_Q = true_BOOL;
            var_ET = var_PT;
          } else {
            var_ET = count;
          }
        }
      }
      sendOutputEvent(scmEventCNFID, paECET);
      break;
  }
}

void FORTE_FB_TON::readInputData(TEventID paEIID) {
  switch (paEIID) {
    case scmEventREQID: {
      readData(0, var_IN, conn_IN);
      readData(1, var_PT, conn_PT);
      break;
    }
    default: break;
  }
}

void FORTE_FB_TON::writeOutputData(TEventID paEIID) {
  switch (paEIID) {
    case scmEventCNFID: {
      writeData(0, var_Q, conn_Q);
      writeData(1, var_ET, conn_ET);
      break;
    }
    default: break;
  }
}

CIEC_ANY *FORTE_FB_TON::getDI(size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_IN;
    case 1: return &var_PT;
  }
  return nullptr;
}

CIEC_ANY *FORTE_FB_TON::getDO(size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_Q;
    case 1: return &var_ET;
  }
  return nullptr;
}

CEventConnection *FORTE_FB_TON::getEOConUnchecked(TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_FB_TON::getDIConUnchecked(TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_IN;
    case 1: return &conn_PT;
  }
  return nullptr;
}

CDataConnection *FORTE_FB_TON::getDOConUnchecked(TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_Q;
    case 1: return &conn_ET;
  }
  return nullptr;
}
