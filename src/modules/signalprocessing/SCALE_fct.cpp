/*************************************************************************  
 *** Copyright (c) 2024 HR Agrartechnik GmbH
 *** This program and the accompanying materials are made available under the  
 *** terms of the Eclipse Public License 2.0 which is available at  
 *** http://www.eclipse.org/legal/epl-2.0.  
 ***  
 *** SPDX-License-Identifier: EPL-2.0  
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x NG!
 ***
 *** Name: SCALE
 *** Description: Scaling Function Block
 *** Version:
 ***     1.0: 2024-09-19/Franz Höpfinger - HR Agrartechnik GmbH -
 *************************************************************************/

#include "SCALE_fct.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "SCALE_fct_gen.cpp"
#endif

#include "criticalregion.h"
#include "resource.h"
#include "forte_real.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"
#include "SCALE_fct.h"

DEFINE_FIRMWARE_FB(FORTE_signalprocessing__SCALE, g_nStringIdsignalprocessing__SCALE)

const CStringDictionary::TStringId FORTE_signalprocessing__SCALE::scmDataInputNames[] = {g_nStringIdIN, g_nStringIdMAX1, g_nStringIdMIN1, g_nStringIdMAX2, g_nStringIdMIN2};
const CStringDictionary::TStringId FORTE_signalprocessing__SCALE::scmDataInputTypeIds[] = {g_nStringIdREAL, g_nStringIdREAL, g_nStringIdREAL, g_nStringIdREAL, g_nStringIdREAL};
const CStringDictionary::TStringId FORTE_signalprocessing__SCALE::scmDataOutputNames[] = {g_nStringId};
const CStringDictionary::TStringId FORTE_signalprocessing__SCALE::scmDataOutputTypeIds[] = {g_nStringIdREAL};
const TDataIOID FORTE_signalprocessing__SCALE::scmEIWith[] = {0, 1, 2, 3, 4, scmWithListDelimiter};
const TForteInt16 FORTE_signalprocessing__SCALE::scmEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_signalprocessing__SCALE::scmEventInputNames[] = {g_nStringIdREQ};
const TDataIOID FORTE_signalprocessing__SCALE::scmEOWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_signalprocessing__SCALE::scmEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_signalprocessing__SCALE::scmEventOutputNames[] = {g_nStringIdCNF};
const SFBInterfaceSpec FORTE_signalprocessing__SCALE::scmFBInterfaceSpec = {
  1, scmEventInputNames, scmEIWith, scmEIWithIndexes,
  1, scmEventOutputNames, scmEOWith, scmEOWithIndexes,
  5, scmDataInputNames, scmDataInputTypeIds,
  1, scmDataOutputNames, scmDataOutputTypeIds,
  0, nullptr,
  0, nullptr
};

FORTE_signalprocessing__SCALE::FORTE_signalprocessing__SCALE(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    CFunctionBlock(paContainer, scmFBInterfaceSpec, paInstanceNameId),
    var_conn_(var_),
    conn_CNF(this, 0),
    conn_IN(nullptr),
    conn_MAX1(nullptr),
    conn_MIN1(nullptr),
    conn_MAX2(nullptr),
    conn_MIN2(nullptr),
    conn_(this, 0, &var_conn_) {
}

void FORTE_signalprocessing__SCALE::setInitialValues() {
  var_IN = 0_REAL;
  var_MAX1 = 0_REAL;
  var_MIN1 = 0_REAL;
  var_MAX2 = 0_REAL;
  var_MIN2 = 0_REAL;
  var_ = 0_REAL;
}

void FORTE_signalprocessing__SCALE::readInputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventREQID: {
      readData(0, var_IN, conn_IN);
      readData(1, var_MAX1, conn_MAX1);
      readData(2, var_MIN1, conn_MIN1);
      readData(3, var_MAX2, conn_MAX2);
      readData(4, var_MIN2, conn_MIN2);
      break;
    }
    default:
      break;
  }
}

void FORTE_signalprocessing__SCALE::writeOutputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventCNFID: {
      writeData(0, var_, conn_);
      break;
    }
    default:
      break;
  }
}

CIEC_ANY *FORTE_signalprocessing__SCALE::getDI(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_IN;
    case 1: return &var_MAX1;
    case 2: return &var_MIN1;
    case 3: return &var_MAX2;
    case 4: return &var_MIN2;
  }
  return nullptr;
}

CIEC_ANY *FORTE_signalprocessing__SCALE::getDO(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_;
  }
  return nullptr;
}

CEventConnection *FORTE_signalprocessing__SCALE::getEOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_signalprocessing__SCALE::getDIConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_IN;
    case 1: return &conn_MAX1;
    case 2: return &conn_MIN1;
    case 3: return &conn_MAX2;
    case 4: return &conn_MIN2;
  }
  return nullptr;
}

CDataConnection *FORTE_signalprocessing__SCALE::getDOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_;
  }
  return nullptr;
}

void FORTE_signalprocessing__SCALE::executeEvent(const TEventID, CEventChainExecutionThread *const paECET) {
  var_ = func_SCALE(var_IN, var_MAX1, var_MIN1, var_MAX2, var_MIN2);
  sendOutputEvent(scmEventCNFID, paECET);
}

CIEC_REAL func_SCALE(CIEC_REAL st_lv_IN, CIEC_REAL st_lv_MAX1, CIEC_REAL st_lv_MIN1, CIEC_REAL st_lv_MAX2, CIEC_REAL st_lv_MIN2) {
  CIEC_REAL st_ret_val = 0_REAL;

  #line 13 "SCALE.fct"
  st_ret_val = func_ADD<CIEC_REAL>(func_DIV<CIEC_REAL>(func_MUL<CIEC_REAL>(func_SUB<CIEC_REAL>(st_lv_IN, st_lv_MIN1), func_SUB<CIEC_REAL>(st_lv_MAX2, st_lv_MIN2)), func_SUB<CIEC_REAL>(st_lv_MAX1, st_lv_MIN1)), st_lv_MIN2);

  return st_ret_val;
}
