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

#include "FB_CTU_UDINT_fbt.h"

USE_STRING_ID(BOOL);
USE_STRING_ID(CNF);
USE_STRING_ID(CU);
USE_STRING_ID(CV);
USE_STRING_ID(Event);
USE_STRING_ID(FB_CTU_UDINT);
USE_STRING_ID(PV);
USE_STRING_ID(Q);
USE_STRING_ID(R);
USE_STRING_ID(REQ);
USE_STRING_ID(UDINT);

#include "forte_bool.h"
#include "forte_udint.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"

DEFINE_FIRMWARE_FB(FORTE_FB_CTU_UDINT, STRID(FB_CTU_UDINT))

const CStringDictionary::TStringId FORTE_FB_CTU_UDINT::scmDataInputNames[] = {STRID(CU), STRID(R), STRID(PV)};

const CStringDictionary::TStringId FORTE_FB_CTU_UDINT::scmDataInputTypeIds[] = {STRID(BOOL), STRID(BOOL), STRID(UDINT)};

const CStringDictionary::TStringId FORTE_FB_CTU_UDINT::scmDataOutputNames[] = {STRID(Q), STRID(CV)};

const CStringDictionary::TStringId FORTE_FB_CTU_UDINT::scmDataOutputTypeIds[] = {STRID(BOOL), STRID(UDINT)};

const TDataIOID FORTE_FB_CTU_UDINT::scmEIWith[] = {0, 1, scmWithListDelimiter};
const TForteInt16 FORTE_FB_CTU_UDINT::scmEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_FB_CTU_UDINT::scmEventInputNames[] = {STRID(REQ)};
const CStringDictionary::TStringId FORTE_FB_CTU_UDINT::scmEventInputTypeIds[] = {STRID(Event)};

const TDataIOID FORTE_FB_CTU_UDINT::scmEOWith[] = {0, 1, scmWithListDelimiter};
const TForteInt16 FORTE_FB_CTU_UDINT::scmEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_FB_CTU_UDINT::scmEventOutputNames[] = {STRID(CNF)};
const CStringDictionary::TStringId FORTE_FB_CTU_UDINT::scmEventOutputTypeIds[] = {STRID(Event)};

const SFBInterfaceSpec FORTE_FB_CTU_UDINT::scmFBInterfaceSpec = {1,
                                                                 scmEventInputNames,
                                                                 scmEventInputTypeIds,
                                                                 scmEIWith,
                                                                 scmEIWithIndexes,
                                                                 1,
                                                                 scmEventOutputNames,
                                                                 scmEventOutputTypeIds,
                                                                 scmEOWith,
                                                                 scmEOWithIndexes,
                                                                 3,
                                                                 scmDataInputNames,
                                                                 scmDataInputTypeIds,
                                                                 2,
                                                                 scmDataOutputNames,
                                                                 scmDataOutputTypeIds,
                                                                 0,
                                                                 nullptr,
                                                                 0,
                                                                 nullptr};

FORTE_FB_CTU_UDINT::FORTE_FB_CTU_UDINT(CStringDictionary::TStringId paInstanceNameId,
                                       forte::core::CFBContainer &paContainer) :
    CSimpleFB(paContainer, scmFBInterfaceSpec, paInstanceNameId, nullptr),
    var_CU(false_BOOL),
    var_R(false_BOOL),
    var_PV(CIEC_UDINT(0)),
    var_Q(false_BOOL),
    var_CV(CIEC_UDINT(0)),
    conn_CNF(*this, 0),
    conn_CU(nullptr),
    conn_R(nullptr),
    conn_PV(nullptr),
    conn_Q(*this, 0, var_Q),
    conn_CV(*this, 1, var_CV) {
}

void FORTE_FB_CTU_UDINT::alg_REQ(void) {

  if (var_R) {
    var_CV = CIEC_UDINT(0);
  } else if (func_AND<CIEC_BOOL>(var_CU,
                                 func_LT(var_CV, CIEC_UDINT(std::numeric_limits<CIEC_UDINT::TValueType>::max())))) {
    var_CV = func_ADD<CIEC_UDINT>(var_CV, CIEC_UDINT(1));
  }
  var_Q = func_GE(var_CV, var_PV);
}

void FORTE_FB_CTU_UDINT::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
  switch (paEIID) {
    case scmEventREQID: alg_REQ(); break;
    default: break;
  }
  sendOutputEvent(scmEventCNFID, paECET);
}

void FORTE_FB_CTU_UDINT::readInputData(TEventID paEIID) {
  switch (paEIID) {
    case scmEventREQID: {
      readData(0, var_CU, conn_CU);
      readData(1, var_R, conn_R);
      break;
    }
    default: break;
  }
}

void FORTE_FB_CTU_UDINT::writeOutputData(TEventID paEIID) {
  switch (paEIID) {
    case scmEventCNFID: {
      writeData(scmFBInterfaceSpec.mNumDIs + 0, var_Q, conn_Q);
      writeData(scmFBInterfaceSpec.mNumDIs + 1, var_CV, conn_CV);
      break;
    }
    default: break;
  }
}

CIEC_ANY *FORTE_FB_CTU_UDINT::getDI(size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_CU;
    case 1: return &var_R;
    case 2: return &var_PV;
  }
  return nullptr;
}

CIEC_ANY *FORTE_FB_CTU_UDINT::getDO(size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_Q;
    case 1: return &var_CV;
  }
  return nullptr;
}

CEventConnection *FORTE_FB_CTU_UDINT::getEOConUnchecked(TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_FB_CTU_UDINT::getDIConUnchecked(TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_CU;
    case 1: return &conn_R;
    case 2: return &conn_PV;
  }
  return nullptr;
}

CDataConnection *FORTE_FB_CTU_UDINT::getDOConUnchecked(TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_Q;
    case 1: return &conn_CV;
  }
  return nullptr;
}

CIEC_ANY *FORTE_FB_CTU_UDINT::getVarInternal(size_t) {
  return nullptr;
}

void FORTE_FB_CTU_UDINT::setInitialValues() {
  var_CU = false_BOOL;
  var_R = false_BOOL;
  var_PV = 0_UDINT;
  var_Q = false_BOOL;
  var_CV = 0_UDINT;
}
