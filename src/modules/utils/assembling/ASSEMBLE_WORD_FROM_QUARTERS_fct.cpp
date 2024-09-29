/*******************************************************************************
 *** Copyright (c) 2024 HR Agrartechnik GmbH
 *** This program and the accompanying materials are made available under the
 *** terms of the Eclipse Public License 2.0 which is available at
 *** http://www.eclipse.org/legal/epl-2.0.
 ***
 *** SPDX-License-Identifier: EPL-2.0
 ***
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x NG!
 ***
 *** Name: ASSEMBLE_WORD_FROM_QUARTERS
 *** Description: this Function combines the 8 QUARTER BYTES to a WORD
 *** Version:
 ***     1.0: 2024-02-22/Franz Höpfinger - HR Agrartechnik - initial Implementation
 ***     1.1: 2024-07-28/Moritz Ortmeier - HR Agrartechnik - rename Function and change Input/Output Variables
 *******************************************************************************/

#include "ASSEMBLE_WORD_FROM_QUARTERS_fct.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "ASSEMBLE_WORD_FROM_QUARTERS_fct_gen.cpp"
#endif

#include "criticalregion.h"
#include "resource.h"
#include "forte_byte.h"
#include "forte_any_int_variant.h"
#include "forte_any_bit_variant.h"
#include "forte_word.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"
#include "quarterconst_gcf.h"
#include "ASSEMBLE_WORD_FROM_QUARTERS_fct.h"

DEFINE_FIRMWARE_FB(FORTE_ASSEMBLE_WORD_FROM_QUARTERS, g_nStringIdASSEMBLE_WORD_FROM_QUARTERS)

const CStringDictionary::TStringId FORTE_ASSEMBLE_WORD_FROM_QUARTERS::scmDataInputNames[] = {g_nStringIdQUARTER_BYTE_00, g_nStringIdQUARTER_BYTE_01, g_nStringIdQUARTER_BYTE_02, g_nStringIdQUARTER_BYTE_03, g_nStringIdQUARTER_BYTE_04, g_nStringIdQUARTER_BYTE_05, g_nStringIdQUARTER_BYTE_06, g_nStringIdQUARTER_BYTE_07};
const CStringDictionary::TStringId FORTE_ASSEMBLE_WORD_FROM_QUARTERS::scmDataInputTypeIds[] = {g_nStringIdBYTE, g_nStringIdBYTE, g_nStringIdBYTE, g_nStringIdBYTE, g_nStringIdBYTE, g_nStringIdBYTE, g_nStringIdBYTE, g_nStringIdBYTE};
const CStringDictionary::TStringId FORTE_ASSEMBLE_WORD_FROM_QUARTERS::scmDataOutputNames[] = {g_nStringId};
const CStringDictionary::TStringId FORTE_ASSEMBLE_WORD_FROM_QUARTERS::scmDataOutputTypeIds[] = {g_nStringIdWORD};
const TDataIOID FORTE_ASSEMBLE_WORD_FROM_QUARTERS::scmEIWith[] = {0, 1, 2, 3, 4, 5, 6, 7, scmWithListDelimiter};
const TForteInt16 FORTE_ASSEMBLE_WORD_FROM_QUARTERS::scmEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_ASSEMBLE_WORD_FROM_QUARTERS::scmEventInputNames[] = {g_nStringIdREQ};
const TDataIOID FORTE_ASSEMBLE_WORD_FROM_QUARTERS::scmEOWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_ASSEMBLE_WORD_FROM_QUARTERS::scmEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_ASSEMBLE_WORD_FROM_QUARTERS::scmEventOutputNames[] = {g_nStringIdCNF};
const SFBInterfaceSpec FORTE_ASSEMBLE_WORD_FROM_QUARTERS::scmFBInterfaceSpec = {
  1, scmEventInputNames, scmEIWith, scmEIWithIndexes,
  1, scmEventOutputNames, scmEOWith, scmEOWithIndexes,
  8, scmDataInputNames, scmDataInputTypeIds,
  1, scmDataOutputNames, scmDataOutputTypeIds,
  0, nullptr,
  0, nullptr
};

FORTE_ASSEMBLE_WORD_FROM_QUARTERS::FORTE_ASSEMBLE_WORD_FROM_QUARTERS(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    CFunctionBlock(paContainer, scmFBInterfaceSpec, paInstanceNameId),
    var_conn_(var_),
    conn_CNF(this, 0),
    conn_QUARTER_BYTE_00(nullptr),
    conn_QUARTER_BYTE_01(nullptr),
    conn_QUARTER_BYTE_02(nullptr),
    conn_QUARTER_BYTE_03(nullptr),
    conn_QUARTER_BYTE_04(nullptr),
    conn_QUARTER_BYTE_05(nullptr),
    conn_QUARTER_BYTE_06(nullptr),
    conn_QUARTER_BYTE_07(nullptr),
    conn_(this, 0, &var_conn_) {
}

void FORTE_ASSEMBLE_WORD_FROM_QUARTERS::setInitialValues() {
  var_QUARTER_BYTE_00 = 0_BYTE;
  var_QUARTER_BYTE_01 = 0_BYTE;
  var_QUARTER_BYTE_02 = 0_BYTE;
  var_QUARTER_BYTE_03 = 0_BYTE;
  var_QUARTER_BYTE_04 = 0_BYTE;
  var_QUARTER_BYTE_05 = 0_BYTE;
  var_QUARTER_BYTE_06 = 0_BYTE;
  var_QUARTER_BYTE_07 = 0_BYTE;
  var_ = 0_WORD;
}

void FORTE_ASSEMBLE_WORD_FROM_QUARTERS::readInputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventREQID: {
      readData(0, var_QUARTER_BYTE_00, conn_QUARTER_BYTE_00);
      readData(1, var_QUARTER_BYTE_01, conn_QUARTER_BYTE_01);
      readData(2, var_QUARTER_BYTE_02, conn_QUARTER_BYTE_02);
      readData(3, var_QUARTER_BYTE_03, conn_QUARTER_BYTE_03);
      readData(4, var_QUARTER_BYTE_04, conn_QUARTER_BYTE_04);
      readData(5, var_QUARTER_BYTE_05, conn_QUARTER_BYTE_05);
      readData(6, var_QUARTER_BYTE_06, conn_QUARTER_BYTE_06);
      readData(7, var_QUARTER_BYTE_07, conn_QUARTER_BYTE_07);
      break;
    }
    default:
      break;
  }
}

void FORTE_ASSEMBLE_WORD_FROM_QUARTERS::writeOutputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventCNFID: {
      writeData(0, var_, conn_);
      break;
    }
    default:
      break;
  }
}

CIEC_ANY *FORTE_ASSEMBLE_WORD_FROM_QUARTERS::getDI(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_QUARTER_BYTE_00;
    case 1: return &var_QUARTER_BYTE_01;
    case 2: return &var_QUARTER_BYTE_02;
    case 3: return &var_QUARTER_BYTE_03;
    case 4: return &var_QUARTER_BYTE_04;
    case 5: return &var_QUARTER_BYTE_05;
    case 6: return &var_QUARTER_BYTE_06;
    case 7: return &var_QUARTER_BYTE_07;
  }
  return nullptr;
}

CIEC_ANY *FORTE_ASSEMBLE_WORD_FROM_QUARTERS::getDO(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_;
  }
  return nullptr;
}

CEventConnection *FORTE_ASSEMBLE_WORD_FROM_QUARTERS::getEOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_ASSEMBLE_WORD_FROM_QUARTERS::getDIConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_QUARTER_BYTE_00;
    case 1: return &conn_QUARTER_BYTE_01;
    case 2: return &conn_QUARTER_BYTE_02;
    case 3: return &conn_QUARTER_BYTE_03;
    case 4: return &conn_QUARTER_BYTE_04;
    case 5: return &conn_QUARTER_BYTE_05;
    case 6: return &conn_QUARTER_BYTE_06;
    case 7: return &conn_QUARTER_BYTE_07;
  }
  return nullptr;
}

CDataConnection *FORTE_ASSEMBLE_WORD_FROM_QUARTERS::getDOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_;
  }
  return nullptr;
}

void FORTE_ASSEMBLE_WORD_FROM_QUARTERS::executeEvent(const TEventID, CEventChainExecutionThread *const paECET) {
  var_ = func_ASSEMBLE_WORD_FROM_QUARTERS(var_QUARTER_BYTE_00, var_QUARTER_BYTE_01, var_QUARTER_BYTE_02, var_QUARTER_BYTE_03, var_QUARTER_BYTE_04, var_QUARTER_BYTE_05, var_QUARTER_BYTE_06, var_QUARTER_BYTE_07);
  sendOutputEvent(scmEventCNFID, paECET);
}

CIEC_WORD func_ASSEMBLE_WORD_FROM_QUARTERS(CIEC_BYTE st_lv_QUARTER_BYTE_00, CIEC_BYTE st_lv_QUARTER_BYTE_01, CIEC_BYTE st_lv_QUARTER_BYTE_02, CIEC_BYTE st_lv_QUARTER_BYTE_03, CIEC_BYTE st_lv_QUARTER_BYTE_04, CIEC_BYTE st_lv_QUARTER_BYTE_05, CIEC_BYTE st_lv_QUARTER_BYTE_06, CIEC_BYTE st_lv_QUARTER_BYTE_07) {
  CIEC_WORD st_ret_val = 0_WORD;

  #line 15 "ASSEMBLE_WORD_FROM_QUARTERS.fct"
  st_ret_val = st_global_WORD_QUARTER_ZZ;
  #line 16 "ASSEMBLE_WORD_FROM_QUARTERS.fct"
  st_ret_val = func_OR<CIEC_WORD>(st_ret_val, func_SHL(func_BYTE_TO_WORD(st_lv_QUARTER_BYTE_07), st_global_SHIFT_QUARTER_07));
  #line 17 "ASSEMBLE_WORD_FROM_QUARTERS.fct"
  st_ret_val = func_OR<CIEC_WORD>(st_ret_val, func_SHL(func_BYTE_TO_WORD(st_lv_QUARTER_BYTE_06), st_global_SHIFT_QUARTER_06));
  #line 18 "ASSEMBLE_WORD_FROM_QUARTERS.fct"
  st_ret_val = func_OR<CIEC_WORD>(st_ret_val, func_SHL(func_BYTE_TO_WORD(st_lv_QUARTER_BYTE_05), st_global_SHIFT_QUARTER_05));
  #line 19 "ASSEMBLE_WORD_FROM_QUARTERS.fct"
  st_ret_val = func_OR<CIEC_WORD>(st_ret_val, func_SHL(func_BYTE_TO_WORD(st_lv_QUARTER_BYTE_04), st_global_SHIFT_QUARTER_04));
  #line 20 "ASSEMBLE_WORD_FROM_QUARTERS.fct"
  st_ret_val = func_OR<CIEC_WORD>(st_ret_val, func_SHL(func_BYTE_TO_WORD(st_lv_QUARTER_BYTE_03), st_global_SHIFT_QUARTER_03));
  #line 21 "ASSEMBLE_WORD_FROM_QUARTERS.fct"
  st_ret_val = func_OR<CIEC_WORD>(st_ret_val, func_SHL(func_BYTE_TO_WORD(st_lv_QUARTER_BYTE_02), st_global_SHIFT_QUARTER_02));
  #line 22 "ASSEMBLE_WORD_FROM_QUARTERS.fct"
  st_ret_val = func_OR<CIEC_WORD>(st_ret_val, func_SHL(func_BYTE_TO_WORD(st_lv_QUARTER_BYTE_01), st_global_SHIFT_QUARTER_01));
  #line 23 "ASSEMBLE_WORD_FROM_QUARTERS.fct"
  st_ret_val = func_OR<CIEC_WORD>(st_ret_val, func_SHL(func_BYTE_TO_WORD(st_lv_QUARTER_BYTE_00), st_global_SHIFT_QUARTER_00));

  return st_ret_val;
}

