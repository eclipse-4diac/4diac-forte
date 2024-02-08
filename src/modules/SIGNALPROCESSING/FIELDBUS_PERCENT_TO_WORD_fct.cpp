/*************************************************************************  
 *** Copyright (c) 2012, 2023 TU Wien ACIN, HR Agrartechnik GmbH  
 *** This program and the accompanying materials are made available under the  
 *** terms of the Eclipse Public License 2.0 which is available at  
 *** http://www.eclipse.org/legal/epl-2.0.  
 ***  
 *** SPDX-License-Identifier: EPL-2.0  
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x NG!
 ***
 *** Name: FIELDBUS_PERCENT_TO_WORD
 *** Description:
 *** Version:
 ***     1.0: 2023-10-21/franz -  -
 *************************************************************************/

#include "FIELDBUS_PERCENT_TO_WORD_fct.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "FIELDBUS_PERCENT_TO_WORD_fct_gen.cpp"
#endif

#include "criticalregion.h"
#include "resource.h"
#include "forte_bool.h"
#include "forte_real.h"
#include "forte_word.h"
#include "forte_udint.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"
#include "FIELDBUS_SIGNAL_gcf.h"

DEFINE_FIRMWARE_FB(FORTE_signalprocessing__FIELDBUS_PERCENT_TO_WORD, g_nStringIdsignalprocessing__FIELDBUS_PERCENT_TO_WORD)

const CStringDictionary::TStringId FORTE_signalprocessing__FIELDBUS_PERCENT_TO_WORD::scmDataInputNames[] = {g_nStringIdQI, g_nStringIdRI};
const CStringDictionary::TStringId FORTE_signalprocessing__FIELDBUS_PERCENT_TO_WORD::scmDataInputTypeIds[] = {g_nStringIdBOOL, g_nStringIdREAL};
const CStringDictionary::TStringId FORTE_signalprocessing__FIELDBUS_PERCENT_TO_WORD::scmDataOutputNames[] = {g_nStringIdQO, g_nStringIdWO};
const CStringDictionary::TStringId FORTE_signalprocessing__FIELDBUS_PERCENT_TO_WORD::scmDataOutputTypeIds[] = {g_nStringIdBOOL, g_nStringIdWORD};
const TDataIOID FORTE_signalprocessing__FIELDBUS_PERCENT_TO_WORD::scmEIWith[] = {0, 1, scmWithListDelimiter};
const TForteInt16 FORTE_signalprocessing__FIELDBUS_PERCENT_TO_WORD::scmEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_signalprocessing__FIELDBUS_PERCENT_TO_WORD::scmEventInputNames[] = {g_nStringIdREQ};
const TDataIOID FORTE_signalprocessing__FIELDBUS_PERCENT_TO_WORD::scmEOWith[] = {0, 1, scmWithListDelimiter};
const TForteInt16 FORTE_signalprocessing__FIELDBUS_PERCENT_TO_WORD::scmEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_signalprocessing__FIELDBUS_PERCENT_TO_WORD::scmEventOutputNames[] = {g_nStringIdCNF};
const SFBInterfaceSpec FORTE_signalprocessing__FIELDBUS_PERCENT_TO_WORD::scmFBInterfaceSpec = {
  1, scmEventInputNames, scmEIWith, scmEIWithIndexes,
  1, scmEventOutputNames, scmEOWith, scmEOWithIndexes,
  2, scmDataInputNames, scmDataInputTypeIds,
  2, scmDataOutputNames, scmDataOutputTypeIds,
  0, nullptr,
  0, nullptr
};

FORTE_signalprocessing__FIELDBUS_PERCENT_TO_WORD::FORTE_signalprocessing__FIELDBUS_PERCENT_TO_WORD(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    CFunctionBlock(paContainer, &scmFBInterfaceSpec, paInstanceNameId),
    var_conn_QO(var_QO),
    var_conn_WO(var_WO),
    conn_CNF(this, 0),
    conn_QI(nullptr),
    conn_RI(nullptr),
    conn_QO(this, 0, &var_conn_QO),
    conn_WO(this, 1, &var_conn_WO) {
}

void FORTE_signalprocessing__FIELDBUS_PERCENT_TO_WORD::setInitialValues() {
  var_QI = 0_BOOL;
  var_RI = 0_REAL;
  var_QO = 0_BOOL;
  var_WO = 0_WORD;
}

void FORTE_signalprocessing__FIELDBUS_PERCENT_TO_WORD::readInputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventREQID: {
      readData(0, var_QI, conn_QI);
      readData(1, var_RI, conn_RI);
      break;
    }
    default:
      break;
  }
}

void FORTE_signalprocessing__FIELDBUS_PERCENT_TO_WORD::writeOutputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventCNFID: {
      writeData(0, var_QO, conn_QO);
      writeData(1, var_WO, conn_WO);
      break;
    }
    default:
      break;
  }
}

CIEC_ANY *FORTE_signalprocessing__FIELDBUS_PERCENT_TO_WORD::getDI(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_QI;
    case 1: return &var_RI;
  }
  return nullptr;
}

CIEC_ANY *FORTE_signalprocessing__FIELDBUS_PERCENT_TO_WORD::getDO(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_QO;
    case 1: return &var_WO;
  }
  return nullptr;
}

CEventConnection *FORTE_signalprocessing__FIELDBUS_PERCENT_TO_WORD::getEOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_signalprocessing__FIELDBUS_PERCENT_TO_WORD::getDIConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_QI;
    case 1: return &conn_RI;
  }
  return nullptr;
}

CDataConnection *FORTE_signalprocessing__FIELDBUS_PERCENT_TO_WORD::getDOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_QO;
    case 1: return &conn_WO;
  }
  return nullptr;
}

void FORTE_signalprocessing__FIELDBUS_PERCENT_TO_WORD::executeEvent(const TEventID paEIID, CEventChainExecutionThread *const paECET) {
  func_FIELDBUS_PERCENT_TO_WORD(var_QI, var_RI, var_QO, var_WO);
  sendOutputEvent(scmEventCNFID, paECET);
}

void func_FIELDBUS_PERCENT_TO_WORD(CIEC_BOOL st_lv_QI, CIEC_REAL st_lv_RI, CIEC_BOOL &st_lv_QO, CIEC_WORD &st_lv_WO) {
  st_lv_QO = 0_BOOL;
  st_lv_WO = 0_WORD;

  #line 14 "FIELDBUS_PERCENT_TO_WORD.fct"
  st_lv_QO = st_lv_QI;
  #line 15 "FIELDBUS_PERCENT_TO_WORD.fct"
  if (func_EQ(true_BOOL, st_lv_QI)) {
    #line 16 "FIELDBUS_PERCENT_TO_WORD.fct"
    st_lv_WO = func_UDINT_TO_WORD(func_REAL_TO_UDINT(func_MUL<CIEC_REAL>(st_lv_RI, func_UDINT_TO_REAL(func_WORD_TO_UDINT(st_global_FIELDBUS_VALID_SIGNAL_W)))));
  }

}

