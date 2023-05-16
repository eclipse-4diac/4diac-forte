/*******************************************************************************
 * Copyright (c) 2013 fortiss GmbH
 *               2019 Johannes Kepler University Linz
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl - initial API and implementation and/or initial documentation
 *   Alois Zoitl - fixed bug in down counting
 *******************************************************************************/

#include "E_CTUD.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "E_CTUD_gen.cpp"
#endif

#include "criticalregion.h"
#include "resource.h"
#include "forte_uint.h"
#include "forte_bool.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"

DEFINE_FIRMWARE_FB(FORTE_E_CTUD, g_nStringIdE_CTUD)

const CStringDictionary::TStringId FORTE_E_CTUD::scm_anDataInputNames[] = {g_nStringIdPV};
const CStringDictionary::TStringId FORTE_E_CTUD::scm_anDataInputTypeIds[] = {g_nStringIdUINT};
const CStringDictionary::TStringId FORTE_E_CTUD::scm_anDataOutputNames[] = {g_nStringIdQU, g_nStringIdQD, g_nStringIdCV};
const CStringDictionary::TStringId FORTE_E_CTUD::scm_anDataOutputTypeIds[] = {g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdUINT};
const TDataIOID FORTE_E_CTUD::scm_anEIWith[] = {0, scmWithListDelimiter, 0, scmWithListDelimiter};
const TForteInt16 FORTE_E_CTUD::scm_anEIWithIndexes[] = {0, -1, -1, 2};
const CStringDictionary::TStringId FORTE_E_CTUD::scm_anEventInputNames[] = {g_nStringIdCU, g_nStringIdCD, g_nStringIdR, g_nStringIdLD};
const TDataIOID FORTE_E_CTUD::scm_anEOWith[] = {0, 2, 1, scmWithListDelimiter, 0, 2, 1, scmWithListDelimiter, 0, 1, 2, scmWithListDelimiter};
const TForteInt16 FORTE_E_CTUD::scm_anEOWithIndexes[] = {0, 4, 8};
const CStringDictionary::TStringId FORTE_E_CTUD::scm_anEventOutputNames[] = {g_nStringIdCO, g_nStringIdRO, g_nStringIdLDO};
const SFBInterfaceSpec FORTE_E_CTUD::scm_stFBInterfaceSpec = {
  4, scm_anEventInputNames, scm_anEIWith, scm_anEIWithIndexes,
  3, scm_anEventOutputNames, scm_anEOWith, scm_anEOWithIndexes,
  1, scm_anDataInputNames, scm_anDataInputTypeIds,
  3, scm_anDataOutputNames, scm_anDataOutputTypeIds,
  0, nullptr
};

FORTE_E_CTUD::FORTE_E_CTUD(CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes) :
    CBasicFB(pa_poSrcRes, &scm_stFBInterfaceSpec, pa_nInstanceNameId, nullptr),
    var_PV(CIEC_UINT(0)),
    var_QU(CIEC_BOOL(0)),
    var_QD(CIEC_BOOL(0)),
    var_CV(CIEC_UINT(0)),
    var_conn_QU(var_QU),
    var_conn_QD(var_QD),
    var_conn_CV(var_CV),
    conn_CO(this, 0),
    conn_RO(this, 1),
    conn_LDO(this, 2),
    conn_PV(nullptr),
    conn_QU(this, 0, &var_conn_QU),
    conn_QD(this, 1, &var_conn_QD),
    conn_CV(this, 2, &var_conn_CV) {
}

void FORTE_E_CTUD::alg_CountUp(void) {
  
  var_CV = func_ADD<CIEC_UINT>(var_CV, CIEC_UINT(1));
}
void FORTE_E_CTUD::alg_Reset(void) {
  
  var_CV = CIEC_UINT(0);
}
void FORTE_E_CTUD::alg_Load(void) {
  
  var_CV = var_PV;
}
void FORTE_E_CTUD::alg_UpdateQUQD(void) {
  
  var_QU = func_GE(var_CV, var_PV);
  var_QD = func_EQ(var_CV, CIEC_UINT(0));
}
void FORTE_E_CTUD::alg_CountDown(void) {
  
  var_CV = func_SUB<CIEC_UINT>(var_CV, CIEC_UINT(1));
}


void FORTE_E_CTUD::executeEvent(TEventID pa_nEIID){
  do {
    switch(m_nECCState) {
      case scm_nStateSTART:
        if((scm_nEventCUID == pa_nEIID) && (func_LT(var_CV, CIEC_UINT(65535)))) enterStateCU();
        else
        if(scm_nEventRID == pa_nEIID) enterStateR();
        else
        if((scm_nEventCDID == pa_nEIID) && (func_GT(var_CV, CIEC_UINT(0)))) enterStateCD();
        else
        if(scm_nEventLDID == pa_nEIID) enterStateLD();
        else return; //no transition cleared
        break;
      case scm_nStateCU:
        if(1) enterStateSTART();
        else return; //no transition cleared
        break;
      case scm_nStateR:
        if(1) enterStateSTART();
        else return; //no transition cleared
        break;
      case scm_nStateCD:
        if(1) enterStateSTART();
        else return; //no transition cleared
        break;
      case scm_nStateLD:
        if(1) enterStateSTART();
        else return; //no transition cleared
        break;
      default:
        DEVLOG_ERROR("The state is not in the valid range! The state value is: %d. The max value can be: 5.", m_nECCState.operator TForteUInt16 ());
        m_nECCState = 0; // 0 is always the initial state
        return;
    }
    pa_nEIID = cg_nInvalidEventID; // we have to clear the event after the first check in order to ensure correct behavior
  } while(true);
}

void FORTE_E_CTUD::readInputData(TEventID pa_nEIID) {
  switch(pa_nEIID) {
    case scm_nEventCUID: {
      RES_DATA_CON_CRITICAL_REGION();
      readData(0, &var_PV, conn_PV);
      break;
    }
    case scm_nEventCDID: {
      RES_DATA_CON_CRITICAL_REGION();
      break;
    }
    case scm_nEventRID: {
      RES_DATA_CON_CRITICAL_REGION();
      break;
    }
    case scm_nEventLDID: {
      RES_DATA_CON_CRITICAL_REGION();
      readData(0, &var_PV, conn_PV);
      break;
    }
    default:
      break;
  }
}

void FORTE_E_CTUD::writeOutputData(TEventID pa_nEIID) {
  switch(pa_nEIID) {
    case scm_nEventCOID: {
      RES_DATA_CON_CRITICAL_REGION();
      writeData(0, &var_QU, &conn_QU);
      writeData(2, &var_CV, &conn_CV);
      writeData(1, &var_QD, &conn_QD);
      break;
    }
    case scm_nEventROID: {
      RES_DATA_CON_CRITICAL_REGION();
      writeData(0, &var_QU, &conn_QU);
      writeData(2, &var_CV, &conn_CV);
      writeData(1, &var_QD, &conn_QD);
      break;
    }
    case scm_nEventLDOID: {
      RES_DATA_CON_CRITICAL_REGION();
      writeData(0, &var_QU, &conn_QU);
      writeData(1, &var_QD, &conn_QD);
      writeData(2, &var_CV, &conn_CV);
      break;
    }
    default:
      break;
  }
}

CIEC_ANY *FORTE_E_CTUD::getDI(size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_PV;
  }
  return nullptr;
}

CIEC_ANY *FORTE_E_CTUD::getDO(size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_QU;
    case 1: return &var_QD;
    case 2: return &var_CV;
  }
  return nullptr;
}

CEventConnection *FORTE_E_CTUD::getEOConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_CO;
    case 1: return &conn_RO;
    case 2: return &conn_LDO;
  }
  return nullptr;
}

CDataConnection **FORTE_E_CTUD::getDIConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_PV;
  }
  return nullptr;
}

CDataConnection *FORTE_E_CTUD::getDOConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_QU;
    case 1: return &conn_QD;
    case 2: return &conn_CV;
  }
  return nullptr;
}

CIEC_ANY *FORTE_E_CTUD::getVarInternal(size_t) {
  return nullptr;
}


void FORTE_E_CTUD::enterStateSTART(void) {
  m_nECCState = scm_nStateSTART;
}

void FORTE_E_CTUD::enterStateCU(void) {
  m_nECCState = scm_nStateCU;
  alg_CountUp();
  alg_UpdateQUQD();
  sendOutputEvent(scm_nEventCOID);
}

void FORTE_E_CTUD::enterStateR(void) {
  m_nECCState = scm_nStateR;
  alg_Reset();
  alg_UpdateQUQD();
  sendOutputEvent(scm_nEventROID);
}

void FORTE_E_CTUD::enterStateCD(void) {
  m_nECCState = scm_nStateCD;
  alg_CountDown();
  alg_UpdateQUQD();
  sendOutputEvent(scm_nEventCOID);
}

void FORTE_E_CTUD::enterStateLD(void) {
  m_nECCState = scm_nStateLD;
  alg_Load();
  alg_UpdateQUQD();
  sendOutputEvent(scm_nEventLDOID);
}



