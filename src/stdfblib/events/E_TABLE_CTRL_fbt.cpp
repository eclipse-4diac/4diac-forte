/*******************************************************************************
 * Copyright (c) 2017 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x NG!
 ***
 *** Name: E_TABLE_CTRL
 *** Description: Support function block for E_TABLE
 *** Version:
 ***     1.0: 2017-09-22/Alois Zoitl - fortiss GmbH - initial API and implementation and/or initial documentation
 *************************************************************************/

#include "E_TABLE_CTRL_fbt.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "E_TABLE_CTRL_fbt_gen.cpp"
#endif

#include "forte_any_elementary_variant.h"
#include "forte_bool.h"
#include "forte_sint.h"
#include "forte_time.h"
#include "forte_uint.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"

DEFINE_FIRMWARE_FB(FORTE_E_TABLE_CTRL, g_nStringIdE_TABLE_CTRL)

const CStringDictionary::TStringId FORTE_E_TABLE_CTRL::scmDataInputNames[] = {g_nStringIdDT, g_nStringIdN};
const CStringDictionary::TStringId FORTE_E_TABLE_CTRL::scmDataInputTypeIds[] = {g_nStringIdARRAY, static_cast<CStringDictionary::TStringId>(0), static_cast<CStringDictionary::TStringId>(3), g_nStringIdTIME, g_nStringIdUINT};
const CStringDictionary::TStringId FORTE_E_TABLE_CTRL::scmDataOutputNames[] = {g_nStringIdDTO, g_nStringIdCV};
const CStringDictionary::TStringId FORTE_E_TABLE_CTRL::scmDataOutputTypeIds[] = {g_nStringIdTIME, g_nStringIdUINT};
const TDataIOID FORTE_E_TABLE_CTRL::scmEIWith[] = {0, 1, scmWithListDelimiter};
const TForteInt16 FORTE_E_TABLE_CTRL::scmEIWithIndexes[] = {0, -1};
const CStringDictionary::TStringId FORTE_E_TABLE_CTRL::scmEventInputNames[] = {g_nStringIdSTART, g_nStringIdCLK};
const TDataIOID FORTE_E_TABLE_CTRL::scmEOWith[] = {0, 1, scmWithListDelimiter};
const TForteInt16 FORTE_E_TABLE_CTRL::scmEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_E_TABLE_CTRL::scmEventOutputNames[] = {g_nStringIdCLKO};
const SFBInterfaceSpec FORTE_E_TABLE_CTRL::scmFBInterfaceSpec = {
  2, scmEventInputNames, nullptr, scmEIWith, scmEIWithIndexes,
  1, scmEventOutputNames, nullptr, scmEOWith, scmEOWithIndexes,
  2, scmDataInputNames, scmDataInputTypeIds,
  2, scmDataOutputNames, scmDataOutputTypeIds,
  0, nullptr,
  0, nullptr
};

FORTE_E_TABLE_CTRL::FORTE_E_TABLE_CTRL(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    CBasicFB(paContainer, scmFBInterfaceSpec, paInstanceNameId, nullptr),
    var_DT(CIEC_ARRAY_FIXED<CIEC_TIME, 0, 3>{}),
    var_N(0_UINT),
    var_DTO(0_TIME),
    var_CV(0_UINT),
    var_conn_DTO(var_DTO),
    var_conn_CV(var_CV),
    conn_CLKO(this, 0),
    conn_DT(nullptr),
    conn_N(nullptr),
    conn_DTO(this, 0, &var_conn_DTO),
    conn_CV(this, 1, &var_conn_CV) {
}

void FORTE_E_TABLE_CTRL::setInitialValues() {
  var_DT = CIEC_ARRAY_FIXED<CIEC_TIME, 0, 3>{};
  var_N = 0_UINT;
  var_DTO = 0_TIME;
  var_CV = 0_UINT;
}

void FORTE_E_TABLE_CTRL::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
  do {
    switch(mECCState) {
      case scmStateSTART:
        if(scmEventSTARTID == paEIID) enterStateINIT(paECET);
        else
        if((scmEventCLKID == paEIID) && (func_LT(var_CV, func_MIN(3_UINT, func_SUB<CIEC_UINT>(var_N, 1_UINT))))) enterStateNEXT_STEP(paECET);
        else return; //no transition cleared
        break;
      case scmStateINIT:
        if(func_EQ(var_N, 0_UINT)) enterStateSTART(paECET);
        else
        if(func_GT(var_N, 0_UINT)) enterStateINIT1(paECET);
        else return; //no transition cleared
        break;
      case scmStateINIT1:
        if(1) enterStateSTART(paECET);
        else return; //no transition cleared
        break;
      case scmStateNEXT_STEP:
        if(1) enterStateSTART(paECET);
        else return; //no transition cleared
        break;
      default:
        DEVLOG_ERROR("The state is not in the valid range! The state value is: %d. The max value can be: 4.", mECCState.operator TForteUInt16 ());
        mECCState = 0; // 0 is always the initial state
        return;
    }
    paEIID = cgInvalidEventID; // we have to clear the event after the first check in order to ensure correct behavior
  } while(true);
}

void FORTE_E_TABLE_CTRL::enterStateSTART(CEventChainExecutionThread *const) {
  mECCState = scmStateSTART;
}

void FORTE_E_TABLE_CTRL::enterStateINIT(CEventChainExecutionThread *const) {
  mECCState = scmStateINIT;
  alg_INIT();
}

void FORTE_E_TABLE_CTRL::enterStateINIT1(CEventChainExecutionThread *const paECET) {
  mECCState = scmStateINIT1;
  sendOutputEvent(scmEventCLKOID, paECET);
}

void FORTE_E_TABLE_CTRL::enterStateNEXT_STEP(CEventChainExecutionThread *const paECET) {
  mECCState = scmStateNEXT_STEP;
  alg_NEXT_STEP();
  sendOutputEvent(scmEventCLKOID, paECET);
}

void FORTE_E_TABLE_CTRL::readInputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventSTARTID: {
      readData(0, var_DT, conn_DT);
      readData(1, var_N, conn_N);
      break;
    }
    default:
      break;
  }
}

void FORTE_E_TABLE_CTRL::writeOutputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventCLKOID: {
      writeData(0, var_DTO, conn_DTO);
      writeData(1, var_CV, conn_CV);
      break;
    }
    default:
      break;
  }
}

CIEC_ANY *FORTE_E_TABLE_CTRL::getDI(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_DT;
    case 1: return &var_N;
  }
  return nullptr;
}

CIEC_ANY *FORTE_E_TABLE_CTRL::getDO(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_DTO;
    case 1: return &var_CV;
  }
  return nullptr;
}

CEventConnection *FORTE_E_TABLE_CTRL::getEOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_CLKO;
  }
  return nullptr;
}

CDataConnection **FORTE_E_TABLE_CTRL::getDIConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_DT;
    case 1: return &conn_N;
  }
  return nullptr;
}

CDataConnection *FORTE_E_TABLE_CTRL::getDOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_DTO;
    case 1: return &conn_CV;
  }
  return nullptr;
}

CIEC_ANY *FORTE_E_TABLE_CTRL::getVarInternal(size_t) {
  return nullptr;
}

void FORTE_E_TABLE_CTRL::alg_INIT(void) {

  #line 2 "E_TABLE_CTRL.fbt"
  var_CV = 0_UINT;
  #line 3 "E_TABLE_CTRL.fbt"
  var_DTO = var_DT[0_SINT];
}

void FORTE_E_TABLE_CTRL::alg_NEXT_STEP(void) {

  #line 7 "E_TABLE_CTRL.fbt"
  var_CV = func_ADD<CIEC_UINT>(var_CV, 1_UINT);
  #line 8 "E_TABLE_CTRL.fbt"
  var_DTO = var_DT[var_CV];
}
