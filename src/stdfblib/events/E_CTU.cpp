/*******************************************************************************
 * Copyright (c) 2006 - 2013 ACIN, Profactor GmbH, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Gerhard Ebenhofer, Martin Melik Merkumians, Ingo Hegny
 *     - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "E_CTU.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "E_CTU_gen.cpp"
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

DEFINE_FIRMWARE_FB(FORTE_E_CTU, g_nStringIdE_CTU)

const CStringDictionary::TStringId FORTE_E_CTU::scm_anDataInputNames[] = {g_nStringIdPV};
const CStringDictionary::TStringId FORTE_E_CTU::scm_anDataInputTypeIds[] = {g_nStringIdUINT};
const CStringDictionary::TStringId FORTE_E_CTU::scm_anDataOutputNames[] = {g_nStringIdQ, g_nStringIdCV};
const CStringDictionary::TStringId FORTE_E_CTU::scm_anDataOutputTypeIds[] = {g_nStringIdBOOL, g_nStringIdUINT};
const TDataIOID FORTE_E_CTU::scm_anEIWith[] = {0, 255};
const TForteInt16 FORTE_E_CTU::scm_anEIWithIndexes[] = {0, -1};
const CStringDictionary::TStringId FORTE_E_CTU::scm_anEventInputNames[] = {g_nStringIdCU, g_nStringIdR};
const TDataIOID FORTE_E_CTU::scm_anEOWith[] = {0, 1, 255, 0, 1, 255};
const TForteInt16 FORTE_E_CTU::scm_anEOWithIndexes[] = {0, 3};
const CStringDictionary::TStringId FORTE_E_CTU::scm_anEventOutputNames[] = {g_nStringIdCUO, g_nStringIdRO};
const SFBInterfaceSpec FORTE_E_CTU::scm_stFBInterfaceSpec = {
  2, scm_anEventInputNames, scm_anEIWith, scm_anEIWithIndexes,
  2, scm_anEventOutputNames, scm_anEOWith, scm_anEOWithIndexes,
  1, scm_anDataInputNames, scm_anDataInputTypeIds,
  2, scm_anDataOutputNames, scm_anDataOutputTypeIds,
  0, nullptr
};

FORTE_E_CTU::FORTE_E_CTU(CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes) :
    CBasicFB(pa_poSrcRes, &scm_stFBInterfaceSpec, pa_nInstanceNameId, nullptr),
    var_PV(CIEC_UINT(0)),
    var_Q(CIEC_BOOL(0)),
    var_CV(CIEC_UINT(0)),
    var_conn_Q(var_Q),
    var_conn_CV(var_CV),
    conn_CUO(this, 0),
    conn_RO(this, 1),
    conn_PV(nullptr),
    conn_Q(this, 0, &var_conn_Q),
    conn_CV(this, 1, &var_conn_CV) {
}

void FORTE_E_CTU::alg_R(void) {
  
  var_CV = CIEC_UINT(0);
  var_Q = CIEC_BOOL(false);
}
void FORTE_E_CTU::alg_CU(void) {
  
  var_CV = func_ADD<CIEC_UINT>(var_CV, CIEC_UINT(1));
  var_Q = func_GE(var_CV, var_PV);
}


void FORTE_E_CTU::executeEvent(int pa_nEIID){
  do {
    switch(m_nECCState) {
      case scm_nStateSTART:
        if((scm_nEventCUID == pa_nEIID) && (func_LT(var_CV, CIEC_UINT(65535)))) enterStateCU();
        else
        if(scm_nEventRID == pa_nEIID) enterStateR();
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
      default:
        DEVLOG_ERROR("The state is not in the valid range! The state value is: %d. The max value can be: 3.", m_nECCState.operator TForteUInt16 ());
        m_nECCState = 0; // 0 is always the initial state
        return;
    }
    pa_nEIID = cg_nInvalidEventID; // we have to clear the event after the first check in order to ensure correct behavior
  } while(true);
}

void FORTE_E_CTU::readInputData(size_t pa_nEIID) {
  switch(pa_nEIID) {
    case scm_nEventCUID: {
      CCriticalRegion criticalRegion(getResource().m_oResDataConSync);
      readData(0, &var_PV, conn_PV);
      break;
    }
    case scm_nEventRID: {
      CCriticalRegion criticalRegion(getResource().m_oResDataConSync);
      break;
    }
    default:
      break;
  }
}

void FORTE_E_CTU::writeOutputData(size_t pa_nEIID) {
  switch(pa_nEIID) {
    case scm_nEventCUOID: {
      CCriticalRegion criticalRegion(getResource().m_oResDataConSync);
      writeData(0, &var_Q, &conn_Q);
      writeData(1, &var_CV, &conn_CV);
      break;
    }
    case scm_nEventROID: {
      CCriticalRegion criticalRegion(getResource().m_oResDataConSync);
      writeData(0, &var_Q, &conn_Q);
      writeData(1, &var_CV, &conn_CV);
      break;
    }
    default:
      break;
  }
}

CIEC_ANY *FORTE_E_CTU::getDI(size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_PV;
  }
  return nullptr;
}

CIEC_ANY *FORTE_E_CTU::getDO(size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_Q;
    case 1: return &var_CV;
  }
  return nullptr;
}

CEventConnection *FORTE_E_CTU::getEOConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_CUO;
    case 1: return &conn_RO;
  }
  return nullptr;
}

CDataConnection **FORTE_E_CTU::getDIConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_PV;
  }
  return nullptr;
}

CDataConnection *FORTE_E_CTU::getDOConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_Q;
    case 1: return &conn_CV;
  }
  return nullptr;
}

CIEC_ANY *FORTE_E_CTU::getVarInternal(size_t) {
  return nullptr;
}


void FORTE_E_CTU::enterStateSTART(void) {
  m_nECCState = scm_nStateSTART;
}

void FORTE_E_CTU::enterStateCU(void) {
  m_nECCState = scm_nStateCU;
  alg_CU();
  sendOutputEvent(scm_nEventCUOID);
}

void FORTE_E_CTU::enterStateR(void) {
  m_nECCState = scm_nStateR;
  alg_R();
  sendOutputEvent(scm_nEventROID);
}



