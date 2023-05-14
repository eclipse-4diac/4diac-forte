/*******************************************************************************
 * Copyright (c) 2023 Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Martin Jobst
 *     - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "FB_R_TRIG.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "FB_R_TRIG_gen.cpp"
#endif

#include "criticalregion.h"
#include "resource.h"
#include "forte_bool.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"

DEFINE_FIRMWARE_FB(FORTE_FB_R_TRIG, g_nStringIdFB_R_TRIG)

const CStringDictionary::TStringId FORTE_FB_R_TRIG::scm_anDataInputNames[] = {g_nStringIdCLK};

const CStringDictionary::TStringId FORTE_FB_R_TRIG::scm_anDataInputTypeIds[] = {g_nStringIdBOOL};

const CStringDictionary::TStringId FORTE_FB_R_TRIG::scm_anDataOutputNames[] = {g_nStringIdQ};

const CStringDictionary::TStringId FORTE_FB_R_TRIG::scm_anDataOutputTypeIds[] = {g_nStringIdBOOL};

const TDataIOID FORTE_FB_R_TRIG::scm_anEIWith[] = {0, 255};
const TForteInt16 FORTE_FB_R_TRIG::scm_anEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_FB_R_TRIG::scm_anEventInputNames[] = {g_nStringIdREQ};

const TDataIOID FORTE_FB_R_TRIG::scm_anEOWith[] = {0, 255};
const TForteInt16 FORTE_FB_R_TRIG::scm_anEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_FB_R_TRIG::scm_anEventOutputNames[] = {g_nStringIdCNF};


const SFBInterfaceSpec FORTE_FB_R_TRIG::scm_stFBInterfaceSpec = {
  1, scm_anEventInputNames, scm_anEIWith, scm_anEIWithIndexes,
  1, scm_anEventOutputNames, scm_anEOWith, scm_anEOWithIndexes,
  1, scm_anDataInputNames, scm_anDataInputTypeIds,
  1, scm_anDataOutputNames, scm_anDataOutputTypeIds,
  0, nullptr
};

const CStringDictionary::TStringId FORTE_FB_R_TRIG::scm_anInternalsNames[] = {g_nStringIdMEM};
const CStringDictionary::TStringId FORTE_FB_R_TRIG::scm_anInternalsTypeIds[] = {g_nStringIdBOOL};
const SInternalVarsInformation FORTE_FB_R_TRIG::scm_stInternalVars = {1, scm_anInternalsNames, scm_anInternalsTypeIds};


FORTE_FB_R_TRIG::FORTE_FB_R_TRIG(CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes) :
    CSimpleFB(pa_poSrcRes, &scm_stFBInterfaceSpec, pa_nInstanceNameId, &scm_stInternalVars),
    var_MEM(CIEC_BOOL(0)),
    var_CLK(CIEC_BOOL(0)),
    var_Q(CIEC_BOOL(0)),
    var_conn_Q(var_Q),
    conn_CNF(this, 0),
    conn_CLK(nullptr),
    conn_Q(this, 0, &var_conn_Q) {
}

void FORTE_FB_R_TRIG::alg_REQ(void) {
  
  var_Q = func_AND<CIEC_BOOL>(var_CLK, func_NOT<CIEC_BOOL>(var_MEM));
  var_MEM = var_CLK;
}


void FORTE_FB_R_TRIG::executeEvent(TEventID pa_nEIID){
  switch(pa_nEIID) {
    case scm_nEventREQID:
      alg_REQ();
      break;
    default:
      break;
  }
  sendOutputEvent(scm_nEventCNFID);
}

void FORTE_FB_R_TRIG::readInputData(TEventID pa_nEIID) {
  switch(pa_nEIID) {
    case scm_nEventREQID: {
      CCriticalRegion criticalRegion(getResource().m_oResDataConSync);
      readData(0, &var_CLK, conn_CLK);
      break;
    }
    default:
      break;
  }
}

void FORTE_FB_R_TRIG::writeOutputData(TEventID pa_nEIID) {
  switch(pa_nEIID) {
    case scm_nEventCNFID: {
      CCriticalRegion criticalRegion(getResource().m_oResDataConSync);
      writeData(0, &var_Q, &conn_Q);
      break;
    }
    default:
      break;
  }
}

CIEC_ANY *FORTE_FB_R_TRIG::getDI(size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_CLK;
  }
  return nullptr;
}

CIEC_ANY *FORTE_FB_R_TRIG::getDO(size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_Q;
  }
  return nullptr;
}

CEventConnection *FORTE_FB_R_TRIG::getEOConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_FB_R_TRIG::getDIConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_CLK;
  }
  return nullptr;
}

CDataConnection *FORTE_FB_R_TRIG::getDOConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_Q;
  }
  return nullptr;
}

CIEC_ANY *FORTE_FB_R_TRIG::getVarInternal(size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_MEM;
  }
  return nullptr;
}


