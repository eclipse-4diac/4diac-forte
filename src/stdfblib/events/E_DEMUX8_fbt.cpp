/*******************************************************************************
 * Copyright (c) 2024 HR Agrartechnik GmbH
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
 *** Name: E_DEMUX8
 *** Description: Event demultiplexor 8 Events
 *** Version:
 ***     1.0: 2024-07-15/Franz Höpfinger - HR Agrartechnik GmbH - copy from E_DEMUX and make a E_DEMUX8 in the same way
 *************************************************************************/

#include "E_DEMUX8_fbt.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "E_DEMUX8_fbt_gen.cpp"
#endif

#include "forte_bool.h"
#include "forte_uint.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"

DEFINE_FIRMWARE_FB(FORTE_E_DEMUX8, g_nStringIdE_DEMUX8)

const CStringDictionary::TStringId FORTE_E_DEMUX8::scmDataInputNames[] = {g_nStringIdK};
const CStringDictionary::TStringId FORTE_E_DEMUX8::scmDataInputTypeIds[] = {g_nStringIdUINT};
const TDataIOID FORTE_E_DEMUX8::scmEIWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_E_DEMUX8::scmEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_E_DEMUX8::scmEventInputNames[] = {g_nStringIdEI};
const TForteInt16 FORTE_E_DEMUX8::scmEOWithIndexes[] = {-1, -1, -1, -1, -1, -1, -1, -1};
const CStringDictionary::TStringId FORTE_E_DEMUX8::scmEventOutputNames[] = {g_nStringIdEO0, g_nStringIdEO1, g_nStringIdEO2, g_nStringIdEO3, g_nStringIdEO4, g_nStringIdEO5, g_nStringIdEO6, g_nStringIdEO7};
const SFBInterfaceSpec FORTE_E_DEMUX8::scmFBInterfaceSpec = {
  1, scmEventInputNames, nullptr, scmEIWith, scmEIWithIndexes,
  8, scmEventOutputNames, nullptr, nullptr, scmEOWithIndexes,
  1, scmDataInputNames, scmDataInputTypeIds,
  0, nullptr, nullptr,
  0, nullptr,
  0, nullptr
};

FORTE_E_DEMUX8::FORTE_E_DEMUX8(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    CBasicFB(paContainer, scmFBInterfaceSpec, paInstanceNameId, nullptr),
    var_K(0_UINT),
    conn_EO0(this, 0),
    conn_EO1(this, 1),
    conn_EO2(this, 2),
    conn_EO3(this, 3),
    conn_EO4(this, 4),
    conn_EO5(this, 5),
    conn_EO6(this, 6),
    conn_EO7(this, 7),
    conn_K(nullptr) {
}

void FORTE_E_DEMUX8::setInitialValues() {
  var_K = 0_UINT;
}

void FORTE_E_DEMUX8::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
  do {
    switch(mECCState) {
      case scmStateSTART:
        if(scmEventEIID == paEIID) enterStateState(paECET);
        else return; //no transition cleared
        break;
      case scmStateState:
        if(func_EQ(var_K, 0_UINT)) enterStateState_1(paECET);
        else
        if(func_EQ(var_K, 1_UINT)) enterStateState_2(paECET);
        else
        if(func_EQ(var_K, 2_UINT)) enterStateState_3(paECET);
        else
        if(func_EQ(var_K, 3_UINT)) enterStateState_4(paECET);
        else
        if(func_EQ(var_K, 4_UINT)) enterStateState_5(paECET);
        else
        if(func_EQ(var_K, 5_UINT)) enterStateState_6(paECET);
        else
        if(func_EQ(var_K, 6_UINT)) enterStateState_7(paECET);
        else
        if(func_EQ(var_K, 7_UINT)) enterStateState_8(paECET);
        else
        if(func_GT(var_K, 7_UINT)) enterStateSTART(paECET);
        else return; //no transition cleared
        break;
      case scmStateState_1:
        if(1) enterStateSTART(paECET);
        else return; //no transition cleared
        break;
      case scmStateState_2:
        if(1) enterStateSTART(paECET);
        else return; //no transition cleared
        break;
      case scmStateState_3:
        if(1) enterStateSTART(paECET);
        else return; //no transition cleared
        break;
      case scmStateState_4:
        if(1) enterStateSTART(paECET);
        else return; //no transition cleared
        break;
      case scmStateState_5:
        if(1) enterStateSTART(paECET);
        else return; //no transition cleared
        break;
      case scmStateState_6:
        if(1) enterStateSTART(paECET);
        else return; //no transition cleared
        break;
      case scmStateState_7:
        if(1) enterStateSTART(paECET);
        else return; //no transition cleared
        break;
      case scmStateState_8:
        if(1) enterStateSTART(paECET);
        else return; //no transition cleared
        break;
      default:
        DEVLOG_ERROR("The state is not in the valid range! The state value is: %d. The max value can be: 10.", mECCState.operator TForteUInt16 ());
        mECCState = 0; // 0 is always the initial state
        return;
    }
    paEIID = cgInvalidEventID; // we have to clear the event after the first check in order to ensure correct behavior
  } while(true);
}

void FORTE_E_DEMUX8::enterStateSTART(CEventChainExecutionThread *const) {
  mECCState = scmStateSTART;
}

void FORTE_E_DEMUX8::enterStateState(CEventChainExecutionThread *const) {
  mECCState = scmStateState;
}

void FORTE_E_DEMUX8::enterStateState_1(CEventChainExecutionThread *const paECET) {
  mECCState = scmStateState_1;
  sendOutputEvent(scmEventEO0ID, paECET);
}

void FORTE_E_DEMUX8::enterStateState_2(CEventChainExecutionThread *const paECET) {
  mECCState = scmStateState_2;
  sendOutputEvent(scmEventEO1ID, paECET);
}

void FORTE_E_DEMUX8::enterStateState_3(CEventChainExecutionThread *const paECET) {
  mECCState = scmStateState_3;
  sendOutputEvent(scmEventEO2ID, paECET);
}

void FORTE_E_DEMUX8::enterStateState_4(CEventChainExecutionThread *const paECET) {
  mECCState = scmStateState_4;
  sendOutputEvent(scmEventEO3ID, paECET);
}

void FORTE_E_DEMUX8::enterStateState_5(CEventChainExecutionThread *const paECET) {
  mECCState = scmStateState_5;
  sendOutputEvent(scmEventEO4ID, paECET);
}

void FORTE_E_DEMUX8::enterStateState_6(CEventChainExecutionThread *const paECET) {
  mECCState = scmStateState_6;
  sendOutputEvent(scmEventEO5ID, paECET);
}

void FORTE_E_DEMUX8::enterStateState_7(CEventChainExecutionThread *const paECET) {
  mECCState = scmStateState_7;
  sendOutputEvent(scmEventEO6ID, paECET);
}

void FORTE_E_DEMUX8::enterStateState_8(CEventChainExecutionThread *const paECET) {
  mECCState = scmStateState_8;
  sendOutputEvent(scmEventEO7ID, paECET);
}

void FORTE_E_DEMUX8::readInputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventEIID: {
      readData(0, var_K, conn_K);
      break;
    }
    default:
      break;
  }
}

void FORTE_E_DEMUX8::writeOutputData(TEventID) {
  // nothing to do
}

CIEC_ANY *FORTE_E_DEMUX8::getDI(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_K;
  }
  return nullptr;
}

CIEC_ANY *FORTE_E_DEMUX8::getDO(size_t) {
  return nullptr;
}

CEventConnection *FORTE_E_DEMUX8::getEOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_EO0;
    case 1: return &conn_EO1;
    case 2: return &conn_EO2;
    case 3: return &conn_EO3;
    case 4: return &conn_EO4;
    case 5: return &conn_EO5;
    case 6: return &conn_EO6;
    case 7: return &conn_EO7;
  }
  return nullptr;
}

CDataConnection **FORTE_E_DEMUX8::getDIConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_K;
  }
  return nullptr;
}

CDataConnection *FORTE_E_DEMUX8::getDOConUnchecked(TPortId) {
  return nullptr;
}

CIEC_ANY *FORTE_E_DEMUX8::getVarInternal(size_t) {
  return nullptr;
}
