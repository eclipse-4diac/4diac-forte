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

#include "FB_CTD_ULINT_fbt.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "FB_CTD_ULINT_fbt_gen.cpp"
#endif

#include "criticalregion.h"
#include "resource.h"
#include "forte_bool.h"
#include "forte_ulint.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"

DEFINE_FIRMWARE_FB(FORTE_FB_CTD_ULINT, g_nStringIdFB_CTD_ULINT)

const CStringDictionary::TStringId FORTE_FB_CTD_ULINT::scmDataInputNames[] = {g_nStringIdCD, g_nStringIdLD, g_nStringIdPV};

const CStringDictionary::TStringId FORTE_FB_CTD_ULINT::scmDataInputTypeIds[] = {g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdULINT};

const CStringDictionary::TStringId FORTE_FB_CTD_ULINT::scmDataOutputNames[] = {g_nStringIdQ, g_nStringIdCV};

const CStringDictionary::TStringId FORTE_FB_CTD_ULINT::scmDataOutputTypeIds[] = {g_nStringIdBOOL, g_nStringIdULINT};

const TDataIOID FORTE_FB_CTD_ULINT::scmEIWith[] = {0, 2, 1, scmWithListDelimiter};
const TForteInt16 FORTE_FB_CTD_ULINT::scmEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_FB_CTD_ULINT::scmEventInputNames[] = {g_nStringIdREQ};

const TDataIOID FORTE_FB_CTD_ULINT::scmEOWith[] = {0, 1, scmWithListDelimiter};
const TForteInt16 FORTE_FB_CTD_ULINT::scmEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_FB_CTD_ULINT::scmEventOutputNames[] = {g_nStringIdCNF};


const SFBInterfaceSpec FORTE_FB_CTD_ULINT::scmFBInterfaceSpec = {
  1, scmEventInputNames, scmEIWith, scmEIWithIndexes,
  1, scmEventOutputNames, scmEOWith, scmEOWithIndexes,
  3, scmDataInputNames, scmDataInputTypeIds,
  2, scmDataOutputNames, scmDataOutputTypeIds,
  0, nullptr,
  0, nullptr
};


FORTE_FB_CTD_ULINT::FORTE_FB_CTD_ULINT(CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    CSimpleFB(paContainer, scmFBInterfaceSpec, paInstanceNameId, nullptr),
    var_CD(CIEC_BOOL(0)),
    var_LD(CIEC_BOOL(0)),
    var_PV(CIEC_ULINT(0)),
    var_Q(CIEC_BOOL(0)),
    var_CV(CIEC_ULINT(0)),
    var_conn_Q(var_Q),
    var_conn_CV(var_CV),
    conn_CNF(this, 0),
    conn_CD(nullptr),
    conn_LD(nullptr),
    conn_PV(nullptr),
    conn_Q(this, 0, &var_conn_Q),
    conn_CV(this, 1, &var_conn_CV) {
}

void FORTE_FB_CTD_ULINT::alg_REQ(void) {
  
  if (var_LD) {
    var_CV = var_PV;
  }
  else if (func_AND<CIEC_BOOL>(var_CD, func_GT(var_CV, CIEC_ULINT(std::numeric_limits<CIEC_ULINT::TValueType>::min())))) {
    var_CV = func_SUB<CIEC_ULINT>(var_CV, CIEC_ULINT(1));
  }
  var_Q = func_LE(var_CV, CIEC_ULINT(0));
}


void FORTE_FB_CTD_ULINT::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
  switch(paEIID) {
    case scmEventREQID:
      alg_REQ();
      break;
    default:
      break;
  }
  sendOutputEvent(scmEventCNFID, paECET);
}

void FORTE_FB_CTD_ULINT::readInputData(TEventID paEIID) {
  switch(paEIID) {
    case scmEventREQID: {
      readData(0, var_CD, conn_CD);
      readData(2, var_PV, conn_PV);
      readData(1, var_LD, conn_LD);
      break;
    }
    default:
      break;
  }
}

void FORTE_FB_CTD_ULINT::writeOutputData(TEventID paEIID) {
  switch(paEIID) {
    case scmEventCNFID: {
      writeData(0, var_Q, conn_Q);
      writeData(1, var_CV, conn_CV);
      break;
    }
    default:
      break;
  }
}

CIEC_ANY *FORTE_FB_CTD_ULINT::getDI(size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_CD;
    case 1: return &var_LD;
    case 2: return &var_PV;
  }
  return nullptr;
}

CIEC_ANY *FORTE_FB_CTD_ULINT::getDO(size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_Q;
    case 1: return &var_CV;
  }
  return nullptr;
}

CEventConnection *FORTE_FB_CTD_ULINT::getEOConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_FB_CTD_ULINT::getDIConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_CD;
    case 1: return &conn_LD;
    case 2: return &conn_PV;
  }
  return nullptr;
}

CDataConnection *FORTE_FB_CTD_ULINT::getDOConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_Q;
    case 1: return &conn_CV;
  }
  return nullptr;
}

CIEC_ANY *FORTE_FB_CTD_ULINT::getVarInternal(size_t) {
  return nullptr;
}


