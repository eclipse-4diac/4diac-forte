/*************************************************************************  
 *** Copyright (c) 2012, 2022 TU Wien ACIN, HR Agrartechnik GmbH  
 *** This program and the accompanying materials are made available under the  
 *** terms of the Eclipse Public License 2.0 which is available at  
 *** http://www.eclipse.org/legal/epl-2.0.  
 ***  
 *** SPDX-License-Identifier: EPL-2.0  
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x NG!
 ***
 *** Name: F_SEL_E_3
 *** Description: selection event-based selection)
 *** Version:
 ***     1.0: 2012-03-25/Monika Wenger - TU Wien ACIN -
 ***     1.1: 2022-08-04/Franz Höpfinger - HR Agrartechnik GmbH -
 ***     1.2: 2024-07-13/Franz Höpfinger - HR Agrartechnik GmbH - changed to Basic FB
 *************************************************************************/

#include "F_SEL_E_3_fbt.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "F_SEL_E_3_fbt_gen.cpp"
#endif

#include "criticalregion.h"
#include "resource.h"
#include "forte_any_variant.h"
#include "forte_bool.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"

DEFINE_FIRMWARE_FB(FORTE_F_SEL_E_3, g_nStringIdF_SEL_E_3)

const CStringDictionary::TStringId FORTE_F_SEL_E_3::scmDataInputNames[] = {g_nStringIdIN0, g_nStringIdIN1, g_nStringIdIN2};
const CStringDictionary::TStringId FORTE_F_SEL_E_3::scmDataInputTypeIds[] = {g_nStringIdANY, g_nStringIdANY, g_nStringIdANY};
const CStringDictionary::TStringId FORTE_F_SEL_E_3::scmDataOutputNames[] = {g_nStringIdOUT};
const CStringDictionary::TStringId FORTE_F_SEL_E_3::scmDataOutputTypeIds[] = {g_nStringIdANY};
const TDataIOID FORTE_F_SEL_E_3::scmEIWith[] = {0, scmWithListDelimiter, 1, scmWithListDelimiter, 2, scmWithListDelimiter};
const TForteInt16 FORTE_F_SEL_E_3::scmEIWithIndexes[] = {0, 2, 4};
const CStringDictionary::TStringId FORTE_F_SEL_E_3::scmEventInputNames[] = {g_nStringIdREQ0, g_nStringIdREQ1, g_nStringIdREQ2};
const TDataIOID FORTE_F_SEL_E_3::scmEOWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_F_SEL_E_3::scmEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_F_SEL_E_3::scmEventOutputNames[] = {g_nStringIdCNF};
const SFBInterfaceSpec FORTE_F_SEL_E_3::scmFBInterfaceSpec = {
  3, scmEventInputNames, scmEIWith, scmEIWithIndexes,
  1, scmEventOutputNames, scmEOWith, scmEOWithIndexes,
  3, scmDataInputNames, scmDataInputTypeIds,
  1, scmDataOutputNames, scmDataOutputTypeIds,
  0, nullptr,
  0, nullptr
};

FORTE_F_SEL_E_3::FORTE_F_SEL_E_3(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    CBasicFB(paContainer, &scmFBInterfaceSpec, paInstanceNameId, nullptr),
    var_conn_OUT(var_OUT),
    conn_CNF(this, 0),
    conn_IN0(nullptr),
    conn_IN1(nullptr),
    conn_IN2(nullptr),
    conn_OUT(this, 0, &var_conn_OUT) {
}

void FORTE_F_SEL_E_3::setInitialValues() {
    var_IN0 = CIEC_ANY_VARIANT();
    var_IN1 = CIEC_ANY_VARIANT();
    var_IN2 = CIEC_ANY_VARIANT();
    var_OUT = CIEC_ANY_VARIANT();
}

void FORTE_F_SEL_E_3::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
  do {
    switch(mECCState) {
      case scmStateSTART:
        if(scmEventREQ0ID == paEIID) enterStateCOPY0(paECET);
        else
        if(scmEventREQ1ID == paEIID) enterStateCOPY1(paECET);
        else
        if(scmEventREQ2ID == paEIID) enterStateCOPY2(paECET);
        else return; //no transition cleared
        break;
      case scmStateCOPY0:
        if(1) enterStateSTART(paECET);
        else return; //no transition cleared
        break;
      case scmStateCOPY1:
        if(1) enterStateSTART(paECET);
        else return; //no transition cleared
        break;
      case scmStateCOPY2:
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

void FORTE_F_SEL_E_3::enterStateSTART(CEventChainExecutionThread *const) {
  mECCState = scmStateSTART;
}

void FORTE_F_SEL_E_3::enterStateCOPY0(CEventChainExecutionThread *const paECET) {
  mECCState = scmStateCOPY0;
  alg_COPY0();
  sendOutputEvent(scmEventCNFID, paECET);
}

void FORTE_F_SEL_E_3::enterStateCOPY1(CEventChainExecutionThread *const paECET) {
  mECCState = scmStateCOPY1;
  alg_COPY1();
  sendOutputEvent(scmEventCNFID, paECET);
}

void FORTE_F_SEL_E_3::enterStateCOPY2(CEventChainExecutionThread *const paECET) {
  mECCState = scmStateCOPY2;
  alg_COPY2();
  sendOutputEvent(scmEventCNFID, paECET);
}

void FORTE_F_SEL_E_3::readInputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventREQ0ID: {
      readData(0, var_IN0, conn_IN0);
      break;
    }
    case scmEventREQ1ID: {
      readData(1, var_IN1, conn_IN1);
      break;
    }
    case scmEventREQ2ID: {
      readData(2, var_IN2, conn_IN2);
      break;
    }
    default:
      break;
  }
}

void FORTE_F_SEL_E_3::writeOutputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventCNFID: {
      writeData(0, var_OUT, conn_OUT);
      break;
    }
    default:
      break;
  }
}

CIEC_ANY *FORTE_F_SEL_E_3::getDI(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_IN0;
    case 1: return &var_IN1;
    case 2: return &var_IN2;
  }
  return nullptr;
}

CIEC_ANY *FORTE_F_SEL_E_3::getDO(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_OUT;
  }
  return nullptr;
}

CEventConnection *FORTE_F_SEL_E_3::getEOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_F_SEL_E_3::getDIConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_IN0;
    case 1: return &conn_IN1;
    case 2: return &conn_IN2;
  }
  return nullptr;
}

CDataConnection *FORTE_F_SEL_E_3::getDOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_OUT;
  }
  return nullptr;
}

CIEC_ANY *FORTE_F_SEL_E_3::getVarInternal(size_t) {
  return nullptr;
}

void FORTE_F_SEL_E_3::alg_COPY0(void) {

  #line 2 "F_SEL_E_3.fbt"
  var_OUT.setValue(var_IN0.unwrap());
}

void FORTE_F_SEL_E_3::alg_COPY1(void) {

  #line 6 "F_SEL_E_3.fbt"
  var_OUT.setValue(var_IN1.unwrap());
}

void FORTE_F_SEL_E_3::alg_COPY2(void) {

  #line 10 "F_SEL_E_3.fbt"
  var_OUT.setValue(var_IN2.unwrap());
}

