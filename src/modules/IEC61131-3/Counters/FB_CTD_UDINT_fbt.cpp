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

#include "FB_CTD_UDINT_fbt.h"

USE_STRING_ID(BOOL);
USE_STRING_ID(CD);
USE_STRING_ID(CNF);
USE_STRING_ID(CV);
USE_STRING_ID(Event);
USE_STRING_ID(FB_CTD_UDINT);
USE_STRING_ID(LD);
USE_STRING_ID(PV);
USE_STRING_ID(Q);
USE_STRING_ID(REQ);
USE_STRING_ID(UDINT);


#include "criticalregion.h"
#include "resource.h"
#include "forte_bool.h"
#include "forte_udint.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"

DEFINE_FIRMWARE_FB(FORTE_FB_CTD_UDINT, STRID(FB_CTD_UDINT))

const CStringDictionary::TStringId FORTE_FB_CTD_UDINT::scmDataInputNames[] = {STRID(CD), STRID(LD), STRID(PV)};

const CStringDictionary::TStringId FORTE_FB_CTD_UDINT::scmDataInputTypeIds[] = {STRID(BOOL), STRID(BOOL), STRID(UDINT)};

const CStringDictionary::TStringId FORTE_FB_CTD_UDINT::scmDataOutputNames[] = {STRID(Q), STRID(CV)};

const CStringDictionary::TStringId FORTE_FB_CTD_UDINT::scmDataOutputTypeIds[] = {STRID(BOOL), STRID(UDINT)};

const TDataIOID FORTE_FB_CTD_UDINT::scmEIWith[] = {0, 2, 1, scmWithListDelimiter};
const TForteInt16 FORTE_FB_CTD_UDINT::scmEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_FB_CTD_UDINT::scmEventInputNames[] = {STRID(REQ)};
const CStringDictionary::TStringId FORTE_FB_CTD_UDINT::scmEventInputTypeIds[] = {STRID(Event)};

const TDataIOID FORTE_FB_CTD_UDINT::scmEOWith[] = {0, 1, scmWithListDelimiter};
const TForteInt16 FORTE_FB_CTD_UDINT::scmEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_FB_CTD_UDINT::scmEventOutputNames[] = {STRID(CNF)};
const CStringDictionary::TStringId FORTE_FB_CTD_UDINT::scmEventOutputTypeIds[] = {STRID(Event)};


const SFBInterfaceSpec FORTE_FB_CTD_UDINT::scmFBInterfaceSpec = {
  1, scmEventInputNames, scmEventInputTypeIds, scmEIWith, scmEIWithIndexes,
  1, scmEventOutputNames, scmEventOutputTypeIds, scmEOWith, scmEOWithIndexes,
  3, scmDataInputNames, scmDataInputTypeIds,
  2, scmDataOutputNames, scmDataOutputTypeIds,
  0, nullptr,
  0, nullptr
};


FORTE_FB_CTD_UDINT::FORTE_FB_CTD_UDINT(CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    CSimpleFB(paContainer, scmFBInterfaceSpec, paInstanceNameId, nullptr),
    var_CD(CIEC_BOOL(0)),
    var_LD(CIEC_BOOL(0)),
    var_PV(CIEC_UDINT(0)),
    var_Q(CIEC_BOOL(0)),
    var_CV(CIEC_UDINT(0)),
    conn_CNF(*this, 0),
    conn_CD(nullptr),
    conn_LD(nullptr),
    conn_PV(nullptr),
    conn_Q(*this, 0, var_Q),
    conn_CV(*this, 1, var_CV) {
}

void FORTE_FB_CTD_UDINT::alg_REQ(void) {
  
  if (var_LD) {
    var_CV = var_PV;
  }
  else if (func_AND<CIEC_BOOL>(var_CD, func_GT(var_CV, CIEC_UDINT(std::numeric_limits<CIEC_UDINT::TValueType>::min())))) {
    var_CV = func_SUB<CIEC_UDINT>(var_CV, CIEC_UDINT(1));
  }
  var_Q = func_LE(var_CV, CIEC_UDINT(0));
}


void FORTE_FB_CTD_UDINT::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
  switch(paEIID) {
    case scmEventREQID:
      alg_REQ();
      break;
    default:
      break;
  }
  sendOutputEvent(scmEventCNFID, paECET);
}

void FORTE_FB_CTD_UDINT::readInputData(TEventID paEIID) {
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

void FORTE_FB_CTD_UDINT::writeOutputData(TEventID paEIID) {
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

CIEC_ANY *FORTE_FB_CTD_UDINT::getDI(size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_CD;
    case 1: return &var_LD;
    case 2: return &var_PV;
  }
  return nullptr;
}

CIEC_ANY *FORTE_FB_CTD_UDINT::getDO(size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_Q;
    case 1: return &var_CV;
  }
  return nullptr;
}

CEventConnection *FORTE_FB_CTD_UDINT::getEOConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_FB_CTD_UDINT::getDIConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_CD;
    case 1: return &conn_LD;
    case 2: return &conn_PV;
  }
  return nullptr;
}

CDataConnection *FORTE_FB_CTD_UDINT::getDOConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_Q;
    case 1: return &conn_CV;
  }
  return nullptr;
}

CIEC_ANY *FORTE_FB_CTD_UDINT::getVarInternal(size_t) {
  return nullptr;
}


