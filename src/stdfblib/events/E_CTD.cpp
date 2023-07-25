/*******************************************************************************
 * Copyright (c) 2014 ACIN
 *               2019 Johannes Kepler University Linz
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Monika Wenger - initial API and implementation and/or initial documentation
 *   Alois Zoitl - fixed bug in down counting
 *******************************************************************************/

#include "E_CTD.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "E_CTD_gen.cpp"
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

DEFINE_FIRMWARE_FB(FORTE_E_CTD, g_nStringIdE_CTD)

const CStringDictionary::TStringId FORTE_E_CTD::scm_anDataInputNames[] = {g_nStringIdPV};
const CStringDictionary::TStringId FORTE_E_CTD::scm_anDataInputTypeIds[] = {g_nStringIdUINT};
const CStringDictionary::TStringId FORTE_E_CTD::scm_anDataOutputNames[] = {g_nStringIdQ, g_nStringIdCV};
const CStringDictionary::TStringId FORTE_E_CTD::scm_anDataOutputTypeIds[] = {g_nStringIdBOOL, g_nStringIdUINT};
const TDataIOID FORTE_E_CTD::scm_anEIWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_E_CTD::scm_anEIWithIndexes[] = {-1, 0};
const CStringDictionary::TStringId FORTE_E_CTD::scm_anEventInputNames[] = {g_nStringIdCD, g_nStringIdLD};
const TDataIOID FORTE_E_CTD::scm_anEOWith[] = {0, 1, scmWithListDelimiter, 0, 1, scmWithListDelimiter};
const TForteInt16 FORTE_E_CTD::scm_anEOWithIndexes[] = {0, 3};
const CStringDictionary::TStringId FORTE_E_CTD::scm_anEventOutputNames[] = {g_nStringIdCDO, g_nStringIdLDO};
const SFBInterfaceSpec FORTE_E_CTD::scm_stFBInterfaceSpec = {
  2, scm_anEventInputNames, scm_anEIWith, scm_anEIWithIndexes,
  2, scm_anEventOutputNames, scm_anEOWith, scm_anEOWithIndexes,
  1, scm_anDataInputNames, scm_anDataInputTypeIds,
  2, scm_anDataOutputNames, scm_anDataOutputTypeIds,
  0, nullptr,
  0, nullptr
};

FORTE_E_CTD::FORTE_E_CTD(CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes) :
    CBasicFB(pa_poSrcRes, &scm_stFBInterfaceSpec, pa_nInstanceNameId, nullptr),
    var_PV(CIEC_UINT(0)),
    var_Q(CIEC_BOOL(0)),
    var_CV(CIEC_UINT(0)),
    var_conn_Q(var_Q),
    var_conn_CV(var_CV),
    conn_CDO(this, 0),
    conn_LDO(this, 1),
    conn_PV(nullptr),
    conn_Q(this, 0, &var_conn_Q),
    conn_CV(this, 1, &var_conn_CV) {
}

void FORTE_E_CTD::alg_CD(void) {
  
  var_CV = func_SUB<CIEC_UINT>(var_CV, CIEC_UINT(1));
  var_Q = func_EQ(var_CV, CIEC_UINT(0));
}
void FORTE_E_CTD::alg_LOAD(void) {
  
  var_CV = var_PV;
  var_Q = func_EQ(var_CV, CIEC_UINT(0));
}


void FORTE_E_CTD::executeEvent(TEventID pa_nEIID){
  do {
    switch(m_nECCState) {
      case scm_nStateSTART:
        if((scm_nEventCDID == pa_nEIID) && (func_GT(var_CV, CIEC_UINT(0)))) enterStateCD();
        else
        if(scm_nEventLDID == pa_nEIID) enterStateLD();
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
        DEVLOG_ERROR("The state is not in the valid range! The state value is: %d. The max value can be: 3.", m_nECCState.operator TForteUInt16 ());
        m_nECCState = 0; // 0 is always the initial state
        return;
    }
    pa_nEIID = cg_nInvalidEventID; // we have to clear the event after the first check in order to ensure correct behavior
  } while(true);
}

void FORTE_E_CTD::readInputData(TEventID pa_nEIID) {
  switch(pa_nEIID) {
    case scm_nEventCDID: {
      RES_DATA_CON_CRITICAL_REGION();
      break;
    }
    case scm_nEventLDID: {
      RES_DATA_CON_CRITICAL_REGION();
      readData(0, var_PV, conn_PV);
      break;
    }
    default:
      break;
  }
}

void FORTE_E_CTD::writeOutputData(TEventID pa_nEIID) {
  switch(pa_nEIID) {
    case scm_nEventCDOID: {
      RES_DATA_CON_CRITICAL_REGION();
      writeData(0, var_Q, conn_Q);
      writeData(1, var_CV, conn_CV);
      break;
    }
    case scm_nEventLDOID: {
      RES_DATA_CON_CRITICAL_REGION();
      writeData(0, var_Q, conn_Q);
      writeData(1, var_CV, conn_CV);
      break;
    }
    default:
      break;
  }
}

CIEC_ANY *FORTE_E_CTD::getDI(size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_PV;
  }
  return nullptr;
}

CIEC_ANY *FORTE_E_CTD::getDO(size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_Q;
    case 1: return &var_CV;
  }
  return nullptr;
}

CEventConnection *FORTE_E_CTD::getEOConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_CDO;
    case 1: return &conn_LDO;
  }
  return nullptr;
}

CDataConnection **FORTE_E_CTD::getDIConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_PV;
  }
  return nullptr;
}

CDataConnection *FORTE_E_CTD::getDOConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_Q;
    case 1: return &conn_CV;
  }
  return nullptr;
}

CIEC_ANY *FORTE_E_CTD::getVarInternal(size_t) {
  return nullptr;
}


void FORTE_E_CTD::enterStateSTART(void) {
  m_nECCState = scm_nStateSTART;
}

void FORTE_E_CTD::enterStateCD(void) {
  m_nECCState = scm_nStateCD;
  alg_CD();
  sendOutputEvent(scm_nEventCDOID);
}

void FORTE_E_CTD::enterStateLD(void) {
  m_nECCState = scm_nStateLD;
  alg_LOAD();
  sendOutputEvent(scm_nEventLDOID);
}



