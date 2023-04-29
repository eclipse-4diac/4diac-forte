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

#include "FB_CTD_DINT.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "FB_CTD_DINT_gen.cpp"
#endif

#include "criticalregion.h"
#include "resource.h"
#include "forte_bool.h"
#include "forte_dint.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"

DEFINE_FIRMWARE_FB(FORTE_FB_CTD_DINT, g_nStringIdFB_CTD_DINT)

const CStringDictionary::TStringId FORTE_FB_CTD_DINT::scm_anDataInputNames[] = {g_nStringIdCD, g_nStringIdLD, g_nStringIdPV};

const CStringDictionary::TStringId FORTE_FB_CTD_DINT::scm_anDataInputTypeIds[] = {g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdDINT};

const CStringDictionary::TStringId FORTE_FB_CTD_DINT::scm_anDataOutputNames[] = {g_nStringIdQ, g_nStringIdCV};

const CStringDictionary::TStringId FORTE_FB_CTD_DINT::scm_anDataOutputTypeIds[] = {g_nStringIdBOOL, g_nStringIdDINT};

const TDataIOID FORTE_FB_CTD_DINT::scm_anEIWith[] = {0, 2, 1, 255};
const TForteInt16 FORTE_FB_CTD_DINT::scm_anEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_FB_CTD_DINT::scm_anEventInputNames[] = {g_nStringIdREQ};

const TDataIOID FORTE_FB_CTD_DINT::scm_anEOWith[] = {0, 1, 255};
const TForteInt16 FORTE_FB_CTD_DINT::scm_anEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_FB_CTD_DINT::scm_anEventOutputNames[] = {g_nStringIdCNF};


const SFBInterfaceSpec FORTE_FB_CTD_DINT::scm_stFBInterfaceSpec = {
  1, scm_anEventInputNames, scm_anEIWith, scm_anEIWithIndexes,
  1, scm_anEventOutputNames, scm_anEOWith, scm_anEOWithIndexes,
  3, scm_anDataInputNames, scm_anDataInputTypeIds,
  2, scm_anDataOutputNames, scm_anDataOutputTypeIds,
  0, nullptr
};


FORTE_FB_CTD_DINT::FORTE_FB_CTD_DINT(CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes) :
    CSimpleFB(pa_poSrcRes, &scm_stFBInterfaceSpec, pa_nInstanceNameId, nullptr),
    var_CD(CIEC_BOOL(0)),
    var_LD(CIEC_BOOL(0)),
    var_PV(CIEC_DINT(0)),
    var_Q(CIEC_BOOL(0)),
    var_CV(CIEC_DINT(0)),
    var_conn_Q(var_Q),
    var_conn_CV(var_CV),
    conn_CNF(this, 0),
    conn_CD(nullptr),
    conn_LD(nullptr),
    conn_PV(nullptr),
    conn_Q(this, 0, &var_conn_Q),
    conn_CV(this, 1, &var_conn_CV) {
}

void FORTE_FB_CTD_DINT::alg_REQ(void) {
  
  if (var_LD) {
    var_CV = var_PV;
  }
  else if (func_AND<CIEC_BOOL>(var_CD, func_GT(var_CV, CIEC_DINT(-2147483648)))) {
    var_CV = func_SUB<CIEC_DINT>(var_CV, CIEC_DINT(1));
  }
  var_Q = func_LE(var_CV, CIEC_DINT(0));
}


void FORTE_FB_CTD_DINT::executeEvent(int pa_nEIID){
  switch(pa_nEIID) {
    case scm_nEventREQID:
      alg_REQ();
      break;
    default:
      break;
  }
  sendOutputEvent(scm_nEventCNFID);
}

void FORTE_FB_CTD_DINT::readInputData(size_t pa_nEIID) {
  switch(pa_nEIID) {
    case scm_nEventREQID: {
      CCriticalRegion criticalRegion(getResource().m_oResDataConSync);
      readData(0, &var_CD, conn_CD);
      readData(2, &var_PV, conn_PV);
      readData(1, &var_LD, conn_LD);
      break;
    }
    default:
      break;
  }
}

void FORTE_FB_CTD_DINT::writeOutputData(size_t pa_nEIID) {
  switch(pa_nEIID) {
    case scm_nEventCNFID: {
      CCriticalRegion criticalRegion(getResource().m_oResDataConSync);
      writeData(0, &var_Q, &conn_Q);
      writeData(1, &var_CV, &conn_CV);
      break;
    }
    default:
      break;
  }
}

CIEC_ANY *FORTE_FB_CTD_DINT::getDI(size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_CD;
    case 1: return &var_LD;
    case 2: return &var_PV;
  }
  return nullptr;
}

CIEC_ANY *FORTE_FB_CTD_DINT::getDO(size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_Q;
    case 1: return &var_CV;
  }
  return nullptr;
}

CEventConnection *FORTE_FB_CTD_DINT::getEOConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_FB_CTD_DINT::getDIConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_CD;
    case 1: return &conn_LD;
    case 2: return &conn_PV;
  }
  return nullptr;
}

CDataConnection *FORTE_FB_CTD_DINT::getDOConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_Q;
    case 1: return &conn_CV;
  }
  return nullptr;
}

CIEC_ANY *FORTE_FB_CTD_DINT::getVarInternal(size_t) {
  return nullptr;
}


