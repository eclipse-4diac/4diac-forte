/*************************************************************************  
 *** Copyright (c) 2023 HR Agrartechnik GmbH  
 *** This program and the accompanying materials are made available under the  
 *** terms of the Eclipse Public License 2.0 which is available at  
 *** http://www.eclipse.org/legal/epl-2.0.  
 ***  
 *** SPDX-License-Identifier: EPL-2.0  
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x NG!
 ***
 *** Name: FIELDBUS_WORD_TO_PERCENT
 *** Description:
 *** Version:
 ***     1.0: 2023-10-21/franz -  -
 *************************************************************************/

#include "FIELDBUS_WORD_TO_PERCENT_fct.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "FIELDBUS_WORD_TO_PERCENT_fct_gen.cpp"
#endif

#include "criticalregion.h"
#include "resource.h"
#include "forte_real.h"
#include "forte_bool.h"
#include "forte_word.h"
#include "forte_udint.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"
#include "FIELDBUS_SIGNAL_gcf.h"

DEFINE_FIRMWARE_FB(FORTE_signalprocessing__FIELDBUS_WORD_TO_PERCENT, g_nStringIdsignalprocessing__FIELDBUS_WORD_TO_PERCENT)

const CStringDictionary::TStringId FORTE_signalprocessing__FIELDBUS_WORD_TO_PERCENT::scmDataInputNames[] = {g_nStringIdQI, g_nStringIdWI, g_nStringIdRO_E};
const CStringDictionary::TStringId FORTE_signalprocessing__FIELDBUS_WORD_TO_PERCENT::scmDataInputTypeIds[] = {g_nStringIdBOOL, g_nStringIdWORD, g_nStringIdREAL};
const CStringDictionary::TStringId FORTE_signalprocessing__FIELDBUS_WORD_TO_PERCENT::scmDataOutputNames[] = {g_nStringIdQO, g_nStringIdRO, g_nStringIdWO};
const CStringDictionary::TStringId FORTE_signalprocessing__FIELDBUS_WORD_TO_PERCENT::scmDataOutputTypeIds[] = {g_nStringIdBOOL, g_nStringIdREAL, g_nStringIdWORD};
const TDataIOID FORTE_signalprocessing__FIELDBUS_WORD_TO_PERCENT::scmEIWith[] = {0, 1, 2, scmWithListDelimiter};
const TForteInt16 FORTE_signalprocessing__FIELDBUS_WORD_TO_PERCENT::scmEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_signalprocessing__FIELDBUS_WORD_TO_PERCENT::scmEventInputNames[] = {g_nStringIdREQ};
const TDataIOID FORTE_signalprocessing__FIELDBUS_WORD_TO_PERCENT::scmEOWith[] = {0, 1, 2, scmWithListDelimiter};
const TForteInt16 FORTE_signalprocessing__FIELDBUS_WORD_TO_PERCENT::scmEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_signalprocessing__FIELDBUS_WORD_TO_PERCENT::scmEventOutputNames[] = {g_nStringIdCNF};
const SFBInterfaceSpec FORTE_signalprocessing__FIELDBUS_WORD_TO_PERCENT::scmFBInterfaceSpec = {
  1, scmEventInputNames, scmEIWith, scmEIWithIndexes,
  1, scmEventOutputNames, scmEOWith, scmEOWithIndexes,
  3, scmDataInputNames, scmDataInputTypeIds,
  3, scmDataOutputNames, scmDataOutputTypeIds,
  0, nullptr,
  0, nullptr
};

FORTE_signalprocessing__FIELDBUS_WORD_TO_PERCENT::FORTE_signalprocessing__FIELDBUS_WORD_TO_PERCENT(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    CFunctionBlock(paContainer, &scmFBInterfaceSpec, paInstanceNameId),
    var_RO_E(0.5_REAL),
    var_conn_QO(var_QO),
    var_conn_RO(var_RO),
    var_conn_WO(var_WO),
    conn_CNF(this, 0),
    conn_QI(nullptr),
    conn_WI(nullptr),
    conn_RO_E(nullptr),
    conn_QO(this, 0, &var_conn_QO),
    conn_RO(this, 1, &var_conn_RO),
    conn_WO(this, 2, &var_conn_WO) {
}

void FORTE_signalprocessing__FIELDBUS_WORD_TO_PERCENT::setInitialValues() {
  var_QI = 0_BOOL;
  var_WI = 0_WORD;
  var_RO_E = 0.5_REAL;
  var_QO = 0_BOOL;
  var_RO = 0_REAL;
  var_WO = 0_WORD;
}

void FORTE_signalprocessing__FIELDBUS_WORD_TO_PERCENT::readInputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventREQID: {
      readData(0, var_QI, conn_QI);
      readData(1, var_WI, conn_WI);
      readData(2, var_RO_E, conn_RO_E);
      break;
    }
    default:
      break;
  }
}

void FORTE_signalprocessing__FIELDBUS_WORD_TO_PERCENT::writeOutputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventCNFID: {
      writeData(0, var_QO, conn_QO);
      writeData(1, var_RO, conn_RO);
      writeData(2, var_WO, conn_WO);
      break;
    }
    default:
      break;
  }
}

CIEC_ANY *FORTE_signalprocessing__FIELDBUS_WORD_TO_PERCENT::getDI(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_QI;
    case 1: return &var_WI;
    case 2: return &var_RO_E;
  }
  return nullptr;
}

CIEC_ANY *FORTE_signalprocessing__FIELDBUS_WORD_TO_PERCENT::getDO(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_QO;
    case 1: return &var_RO;
    case 2: return &var_WO;
  }
  return nullptr;
}

CEventConnection *FORTE_signalprocessing__FIELDBUS_WORD_TO_PERCENT::getEOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_signalprocessing__FIELDBUS_WORD_TO_PERCENT::getDIConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_QI;
    case 1: return &conn_WI;
    case 2: return &conn_RO_E;
  }
  return nullptr;
}

CDataConnection *FORTE_signalprocessing__FIELDBUS_WORD_TO_PERCENT::getDOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_QO;
    case 1: return &conn_RO;
    case 2: return &conn_WO;
  }
  return nullptr;
}

void FORTE_signalprocessing__FIELDBUS_WORD_TO_PERCENT::executeEvent(const TEventID, CEventChainExecutionThread *const paECET) {
  func_FIELDBUS_WORD_TO_PERCENT(var_QI, var_WI, var_RO_E, var_QO, var_RO, var_WO);
  sendOutputEvent(scmEventCNFID, paECET);
}

void func_FIELDBUS_WORD_TO_PERCENT(CIEC_BOOL st_lv_QI, CIEC_WORD st_lv_WI, CIEC_REAL st_lv_RO_E, CIEC_BOOL &st_lv_QO, CIEC_REAL &st_lv_RO, CIEC_WORD &st_lv_WO) {
  st_lv_QO = 0_BOOL;
  st_lv_RO = 0_REAL;
  st_lv_WO = 0_WORD;

  #line 16 "FIELDBUS_WORD_TO_PERCENT.fct"
  st_lv_QO = st_lv_QI;
  #line 17 "FIELDBUS_WORD_TO_PERCENT.fct"
  if (func_AND<CIEC_BOOL>(func_EQ(true_BOOL, st_lv_QI), func_LE(st_lv_WI, st_global_FIELDBUS_VALID_SIGNAL_W))) {
    #line 18 "FIELDBUS_WORD_TO_PERCENT.fct"
    st_lv_WO = st_lv_WI;
    #line 19 "FIELDBUS_WORD_TO_PERCENT.fct"
    st_lv_RO = func_DIV<CIEC_REAL>(func_UDINT_TO_REAL(func_WORD_TO_UDINT(st_lv_WO)), func_UDINT_TO_REAL(func_WORD_TO_UDINT(st_global_FIELDBUS_VALID_SIGNAL_W)));
  }
  else {
    #line 21 "FIELDBUS_WORD_TO_PERCENT.fct"
    st_lv_RO = st_lv_RO_E;
    #line 22 "FIELDBUS_WORD_TO_PERCENT.fct"
    st_lv_WO = func_UDINT_TO_WORD(func_REAL_TO_UDINT(func_MUL<CIEC_REAL>(st_lv_RO_E, func_UDINT_TO_REAL(func_WORD_TO_UDINT(st_global_FIELDBUS_VALID_SIGNAL_W)))));
  }

}

