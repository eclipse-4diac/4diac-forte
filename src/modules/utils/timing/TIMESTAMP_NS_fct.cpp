/*************************************************************************
 * Copyright (c) 2024
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x NG!
 ***
 *** Name: TIMESTAMP_NS
 *** Description: creates a Unix-Epoch-Timestamp in nanoseconds that is accepted by InfluxDB 2.0
 *** Version:
 ***     1.0: 2024-10-14/Monika Wenger -  -
 *************************************************************************/

#include "TIMESTAMP_NS_fct.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "TIMESTAMP_NS_fct_gen.cpp"
#endif

#include "forte_any_duration_variant.h"
#include "forte_date_and_time.h"
#include "forte_ulint.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"
#include "TIMESTAMP_NS_fct.h"

DEFINE_FIRMWARE_FB(FORTE_TIMESTAMP_NS, g_nStringIdTIMESTAMP_NS)

const CStringDictionary::TStringId FORTE_TIMESTAMP_NS::scmDataOutputNames[] = {g_nStringId};
const CStringDictionary::TStringId FORTE_TIMESTAMP_NS::scmDataOutputTypeIds[] = {g_nStringIdULINT};
const TForteInt16 FORTE_TIMESTAMP_NS::scmEIWithIndexes[] = {-1};
const CStringDictionary::TStringId FORTE_TIMESTAMP_NS::scmEventInputNames[] = {g_nStringIdREQ};
const TDataIOID FORTE_TIMESTAMP_NS::scmEOWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_TIMESTAMP_NS::scmEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_TIMESTAMP_NS::scmEventOutputNames[] = {g_nStringIdCNF};
const SFBInterfaceSpec FORTE_TIMESTAMP_NS::scmFBInterfaceSpec = {
  1, scmEventInputNames, nullptr, scmEIWithIndexes,
  1, scmEventOutputNames, scmEOWith, scmEOWithIndexes,
  0, nullptr, nullptr,
  1, scmDataOutputNames, scmDataOutputTypeIds,
  0, nullptr,
  0, nullptr
};

FORTE_TIMESTAMP_NS::FORTE_TIMESTAMP_NS(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    CFunctionBlock(paContainer, scmFBInterfaceSpec, paInstanceNameId),
    var_conn_(var_),
    conn_CNF(this, 0),
    conn_(this, 0, &var_conn_) {
}

void FORTE_TIMESTAMP_NS::setInitialValues() {
  var_ = 0_ULINT;
}

void FORTE_TIMESTAMP_NS::readInputData(TEventID) {
  // nothing to do
}

void FORTE_TIMESTAMP_NS::writeOutputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventCNFID: {
      writeData(0, var_, conn_);
      break;
    }
    default:
      break;
  }
}

CIEC_ANY *FORTE_TIMESTAMP_NS::getDI(size_t) {
  return nullptr;
}

CIEC_ANY *FORTE_TIMESTAMP_NS::getDO(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_;
  }
  return nullptr;
}

CEventConnection *FORTE_TIMESTAMP_NS::getEOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_TIMESTAMP_NS::getDIConUnchecked(TPortId) {
  return nullptr;
}

CDataConnection *FORTE_TIMESTAMP_NS::getDOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_;
  }
  return nullptr;
}

void FORTE_TIMESTAMP_NS::executeEvent(const TEventID, CEventChainExecutionThread *const paECET) {
  var_ = func_TIMESTAMP_NS();
  sendOutputEvent(scmEventCNFID, paECET);
}

CIEC_ULINT func_TIMESTAMP_NS() {
  CIEC_ULINT st_ret_val = 0_ULINT;

  #line 2 "TIMESTAMP_NS.fct"
  st_ret_val = func_TIME_IN_NS_TO_ULINT(func_SUB<CIEC_TIME>(func_NOW(), 0_DATE_AND_TIME));

  return st_ret_val;
}
