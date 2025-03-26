/*******************************************************************************
 * Copyright (c) 2024 HR Agrartechnik GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0

 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x NG!
 ***
 *** Name: F_NOW_MONOTONIC
 *** Description: return a clear monotonic time
 *** Version:
 ***     1.0: 2024-10-01/Franz Höpfinger - HR Agrartechnik GmbH    -
 *************************************************************************/

#include "F_NOW_MONOTONIC_fct.h"

USE_STRING_ID();
USE_STRING_ID(CNF);
USE_STRING_ID(Event);
USE_STRING_ID(REQ);
USE_STRING_ID(TIME);
USE_STRING_ID(utils__timing__F_NOW_MONOTONIC);


#include "forte_time.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"
#include "F_NOW_MONOTONIC_fct.h"

DEFINE_FIRMWARE_FB(FORTE_utils__timing__F_NOW_MONOTONIC, STRID(utils__timing__F_NOW_MONOTONIC))

const CStringDictionary::TStringId FORTE_utils__timing__F_NOW_MONOTONIC::scmDataOutputNames[] = {STRID()};
const CStringDictionary::TStringId FORTE_utils__timing__F_NOW_MONOTONIC::scmDataOutputTypeIds[] = {STRID(TIME)};
const TForteInt16 FORTE_utils__timing__F_NOW_MONOTONIC::scmEIWithIndexes[] = {-1};
const CStringDictionary::TStringId FORTE_utils__timing__F_NOW_MONOTONIC::scmEventInputNames[] = {STRID(REQ)};
const CStringDictionary::TStringId FORTE_utils__timing__F_NOW_MONOTONIC::scmEventInputTypeIds[] = {STRID(Event)};
const TDataIOID FORTE_utils__timing__F_NOW_MONOTONIC::scmEOWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_utils__timing__F_NOW_MONOTONIC::scmEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_utils__timing__F_NOW_MONOTONIC::scmEventOutputNames[] = {STRID(CNF)};
const CStringDictionary::TStringId FORTE_utils__timing__F_NOW_MONOTONIC::scmEventOutputTypeIds[] = {STRID(Event)};
const SFBInterfaceSpec FORTE_utils__timing__F_NOW_MONOTONIC::scmFBInterfaceSpec = {
  1, scmEventInputNames, scmEventInputTypeIds, nullptr, scmEIWithIndexes,
  1, scmEventOutputNames, scmEventOutputTypeIds, scmEOWith, scmEOWithIndexes,
  0, nullptr, nullptr,
  1, scmDataOutputNames, scmDataOutputTypeIds,
  0, nullptr,
  0, nullptr
};

FORTE_utils__timing__F_NOW_MONOTONIC::FORTE_utils__timing__F_NOW_MONOTONIC(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    CFunctionBlock(paContainer, scmFBInterfaceSpec, paInstanceNameId),
    conn_CNF(*this, 0),
    conn_(*this, 0, var_) {
}

void FORTE_utils__timing__F_NOW_MONOTONIC::setInitialValues() {
  var_ = 0_TIME;
}

void FORTE_utils__timing__F_NOW_MONOTONIC::readInputData(TEventID) {
  // nothing to do
}

void FORTE_utils__timing__F_NOW_MONOTONIC::writeOutputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventCNFID: {
      writeData(0, var_, conn_);
      break;
    }
    default:
      break;
  }
}

CIEC_ANY *FORTE_utils__timing__F_NOW_MONOTONIC::getDI(size_t) {
  return nullptr;
}

CIEC_ANY *FORTE_utils__timing__F_NOW_MONOTONIC::getDO(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_;
  }
  return nullptr;
}

CEventConnection *FORTE_utils__timing__F_NOW_MONOTONIC::getEOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_utils__timing__F_NOW_MONOTONIC::getDIConUnchecked(TPortId) {
  return nullptr;
}

CDataConnection *FORTE_utils__timing__F_NOW_MONOTONIC::getDOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_;
  }
  return nullptr;
}

void FORTE_utils__timing__F_NOW_MONOTONIC::executeEvent(const TEventID, CEventChainExecutionThread *const paECET) {
  var_ = func_F_NOW_MONOTONIC();
  sendOutputEvent(scmEventCNFID, paECET);
}

CIEC_TIME func_F_NOW_MONOTONIC() {
  CIEC_TIME st_ret_val = 0_TIME;

  #line 5 "F_NOW_MONOTONIC.fct"
  st_ret_val = func_NOW_MONOTONIC();

  return st_ret_val;
}
