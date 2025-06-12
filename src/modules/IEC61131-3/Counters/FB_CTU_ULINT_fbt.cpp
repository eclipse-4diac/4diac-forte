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

#include "FB_CTU_ULINT_fbt.h"

#include "core/datatypes/forte_bool.h"
#include "core/datatypes/forte_ulint.h"
#include "core/iec61131_functions.h"
#include "core/datatypes/forte_array_common.h"
#include "core/datatypes/forte_array.h"
#include "core/datatypes/forte_array_fixed.h"
#include "core/datatypes/forte_array_variable.h"

using namespace std::literals;

USE_STRING_ID(BOOL);
USE_STRING_ID(CNF);
USE_STRING_ID(CU);
USE_STRING_ID(CV);
USE_STRING_ID(Event);
USE_STRING_ID(FB_CTU_ULINT);
USE_STRING_ID(PV);
USE_STRING_ID(Q);
USE_STRING_ID(R);
USE_STRING_ID(REQ);
USE_STRING_ID(ULINT);

namespace {
  const auto cEventInputNames = std::array{STRID(REQ)};
  const auto cEventOutputNames = std::array{STRID(CNF)};
  const auto cDataInputNames = std::array{STRID(CU), STRID(R), STRID(PV)};
  const auto cDataOutputNames = std::array{STRID(Q), STRID(CV)};
  const SFBInterfaceSpec cFBInterfaceSpec = {.mEINames = cEventInputNames,
                                                 .mEITypeNames = {},
                                                 .mEONames = cEventOutputNames,
                                                 .mEOTypeNames = {},
                                                 .mDINames = cDataInputNames,
                                                 .mDONames = cDataOutputNames,
                                                 .mDIONames = {},
                                                 .mSocketNames = {},
                                                 .mPlugNames = {}};
}

DEFINE_FIRMWARE_FB(FORTE_FB_CTU_ULINT, STRID(FB_CTU_ULINT))


FORTE_FB_CTU_ULINT::FORTE_FB_CTU_ULINT(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    CSimpleFB(paContainer, cFBInterfaceSpec, paInstanceNameId, nullptr),
    var_CU(0_BOOL),
    var_R(0_BOOL),
    var_PV(0_ULINT),
    var_Q(0_BOOL),
    var_CV(0_ULINT),
    conn_CNF(*this, 0),
    conn_CU(nullptr),
    conn_R(nullptr),
    conn_PV(nullptr),
    conn_Q(*this, 0, var_Q),
    conn_CV(*this, 1, var_CV) {
}

void FORTE_FB_CTU_ULINT::setInitialValues() {
  CSimpleFB::setInitialValues();
  var_CU = 0_BOOL;
  var_R = 0_BOOL;
  var_PV = 0_ULINT;
  var_Q = 0_BOOL;
  var_CV = 0_ULINT;
}

void FORTE_FB_CTU_ULINT::executeEvent(const TEventID paEIID, CEventChainExecutionThread *const paECET) {
  switch(paEIID) {
    case scmEventREQID:
      enterStateREQ(paECET);
      break;
    default:
      break;
  }
}

void FORTE_FB_CTU_ULINT::enterStateREQ(CEventChainExecutionThread *const paECET) {
  alg_REQ();
  sendOutputEvent(scmEventCNFID, paECET);
}

void FORTE_FB_CTU_ULINT::readInputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventREQID: {
      readData(0, var_CU, conn_CU);
      readData(1, var_R, conn_R);
      break;
    }
    default:
      break;
  }
}

void FORTE_FB_CTU_ULINT::writeOutputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventCNFID: {
      writeData(3, var_Q, conn_Q);
      writeData(4, var_CV, conn_CV);
      break;
    }
    default:
      break;
  }
}

CIEC_ANY *FORTE_FB_CTU_ULINT::getDI(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_CU;
    case 1: return &var_R;
    case 2: return &var_PV;
  }
  return nullptr;
}

CIEC_ANY *FORTE_FB_CTU_ULINT::getDO(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_Q;
    case 1: return &var_CV;
  }
  return nullptr;
}

CEventConnection *FORTE_FB_CTU_ULINT::getEOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_FB_CTU_ULINT::getDIConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_CU;
    case 1: return &conn_R;
    case 2: return &conn_PV;
  }
  return nullptr;
}

CDataConnection *FORTE_FB_CTU_ULINT::getDOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_Q;
    case 1: return &conn_CV;
  }
  return nullptr;
}

CIEC_ANY *FORTE_FB_CTU_ULINT::getVarInternal(size_t) {
  return nullptr;
}

void FORTE_FB_CTU_ULINT::alg_REQ(void) {

  #line 2 "FB_CTU_ULINT.fbt"
  if (var_R) {
    #line 3 "FB_CTU_ULINT.fbt"
    var_CV = 0_ULINT;
  }
  else if (func_AND<CIEC_BOOL>(var_CU, func_LT(var_CV, 18446744073709551615_ULINT))) {
    #line 5 "FB_CTU_ULINT.fbt"
    var_CV = func_ADD<CIEC_ULINT>(var_CV, 1_ULINT);
  }
  #line 7 "FB_CTU_ULINT.fbt"
  var_Q = func_GE(var_CV, var_PV);
}
