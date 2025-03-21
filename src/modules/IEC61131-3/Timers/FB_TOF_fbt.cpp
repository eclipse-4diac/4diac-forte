/*******************************************************************************
 * Copyright (c) 2009, 2023 ACIN, fortiss GmbH
 *                          Martin Erich Jobst
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Monika Wenger, Alois Zoitl, Ingo Hengy
 *   - initial API and implementation and/or initial documentation
 *   Martin Jobst - add readInputData and writeOutputData
 *******************************************************************************/
#include "FB_TOF_fbt.h"

USE_STRING_ID(BOOL);
USE_STRING_ID(CNF);
USE_STRING_ID(ET);
USE_STRING_ID(Event);
USE_STRING_ID(FB_TOF);
USE_STRING_ID(IN);
USE_STRING_ID(PT);
USE_STRING_ID(Q);
USE_STRING_ID(REQ);
USE_STRING_ID(TIME);


#include "criticalregion.h"
#include "resource.h"

DEFINE_FIRMWARE_FB(FORTE_FB_TOF, STRID(FB_TOF))

const CStringDictionary::TStringId FORTE_FB_TOF::scmDataInputNames[] = {STRID(IN), STRID(PT)};
const CStringDictionary::TStringId FORTE_FB_TOF::scmDataInputTypeIds[] = {STRID(BOOL), STRID(TIME)};
const CStringDictionary::TStringId FORTE_FB_TOF::scmDataOutputNames[] = {STRID(Q), STRID(ET)};
const CStringDictionary::TStringId FORTE_FB_TOF::scmDataOutputTypeIds[] = {STRID(BOOL), STRID(TIME)};
const TDataIOID FORTE_FB_TOF::scmEIWith[] = {0, 1, scmWithListDelimiter};
const TForteInt16 FORTE_FB_TOF::scmEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_FB_TOF::scmEventInputNames[] = {STRID(REQ)};
const CStringDictionary::TStringId FORTE_FB_TOF::scmEventInputTypeIds[] = {STRID(Event)};
const TDataIOID FORTE_FB_TOF::scmEOWith[] = {0, 1, scmWithListDelimiter};
const TForteInt16 FORTE_FB_TOF::scmEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_FB_TOF::scmEventOutputNames[] = {STRID(CNF)};
const CStringDictionary::TStringId FORTE_FB_TOF::scmEventOutputTypeIds[] = {STRID(Event)};
const SFBInterfaceSpec FORTE_FB_TOF::scmFBInterfaceSpec = {
  1, scmEventInputNames, scmEventInputTypeIds, scmEIWith, scmEIWithIndexes,
  1, scmEventOutputNames, scmEventOutputTypeIds, scmEOWith, scmEOWithIndexes,
  2, scmDataInputNames, scmDataInputTypeIds,
  2, scmDataOutputNames, scmDataOutputTypeIds,
  0, nullptr,
  0, nullptr
};

FORTE_FB_TOF::FORTE_FB_TOF(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    CFunctionBlock(paContainer, scmFBInterfaceSpec, paInstanceNameId),
    conn_CNF(this, 0),
    conn_IN(nullptr),
    conn_PT(nullptr),
    conn_Q(this, 0, var_Q),
    conn_ET(this, 1, var_ET) {
};

void FORTE_FB_TOF::setInitialValues() {
  var_IN = 0_BOOL;
  var_PT = 0_TIME;
  var_Q = 0_BOOL;
  var_ET = 0_TIME;
}

void FORTE_FB_TOF::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
  switch(paEIID) {
    case scmEventREQID:
      if(var_IN) {
        var_Q = true_BOOL;
        var_ET = 0_TIME;
        fallingEdge = false;
        notFirstRisingEdge = true;
        start = 0_TIME;
      } else {
        if(notFirstRisingEdge) {
          if(!fallingEdge) {
            fallingEdge = true;
            start = func_NOW_MONOTONIC();
          } else {
            count = func_SUB(func_NOW_MONOTONIC(), start);
            if(func_LE(var_PT, count)) {
              var_Q = false_BOOL;
              var_ET = var_PT;
            } else {
              var_ET = count;
            }
          }
        }
      }
      sendOutputEvent(scmEventCNFID, paECET);
      break;
  }
}

void FORTE_FB_TOF::readInputData(TEventID paEIID) {
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

void FORTE_FB_TOF::writeOutputData(TEventID paEIID) {
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

CIEC_ANY *FORTE_FB_TOF::getDI(size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_IN;
    case 1: return &var_PT;
  }
  return nullptr;
}

CIEC_ANY *FORTE_FB_TOF::getDO(size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_Q;
    case 1: return &var_ET;
  }
  return nullptr;
}

CEventConnection *FORTE_FB_TOF::getEOConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_FB_TOF::getDIConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_IN;
    case 1: return &conn_PT;
  }
  return nullptr;
}

CDataConnection *FORTE_FB_TOF::getDOConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_Q;
    case 1: return &conn_ET;
  }
  return nullptr;
}
