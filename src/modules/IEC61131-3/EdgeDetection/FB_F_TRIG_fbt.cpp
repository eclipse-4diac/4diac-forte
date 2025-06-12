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

#include "FB_F_TRIG_fbt.h"

#include "core/datatypes/forte_bool.h"
#include "core/iec61131_functions.h"
#include "core/datatypes/forte_array_common.h"
#include "core/datatypes/forte_array.h"
#include "core/datatypes/forte_array_fixed.h"
#include "core/datatypes/forte_array_variable.h"

using namespace std::literals;

USE_STRING_ID(BOOL);
USE_STRING_ID(CLK);
USE_STRING_ID(CNF);
USE_STRING_ID(Event);
USE_STRING_ID(FB_F_TRIG);
USE_STRING_ID(MEM);
USE_STRING_ID(Q);
USE_STRING_ID(REQ);

namespace {
  const auto cEventInputNames = std::array{STRID(REQ)};
  const auto cEventOutputNames = std::array{STRID(CNF)};
  const auto cDataInputNames = std::array{STRID(CLK)};
  const auto cDataOutputNames = std::array{STRID(Q)};
  const SFBInterfaceSpec cFBInterfaceSpec = {
      .mEINames = cEventInputNames,
      .mEITypeNames = {},
      .mEONames = cEventOutputNames,
      .mEOTypeNames = {},
      .mDINames = cDataInputNames,
      .mDONames = cDataOutputNames,
      .mDIONames = {},
      .mSocketNames = {},
      .mPlugNames = {},
  };
} // namespace

DEFINE_FIRMWARE_FB(FORTE_FB_F_TRIG, STRID(FB_F_TRIG))

const CStringDictionary::TStringId FORTE_FB_F_TRIG::scmInternalsNames[] = {STRID(MEM)};
const CStringDictionary::TStringId FORTE_FB_F_TRIG::scmInternalsTypeIds[] = {STRID(BOOL)};
const SInternalVarsInformation FORTE_FB_F_TRIG::scmInternalVars = {1, scmInternalsNames, scmInternalsTypeIds};

FORTE_FB_F_TRIG::FORTE_FB_F_TRIG(const CStringDictionary::TStringId paInstanceNameId,
                                 forte::core::CFBContainer &paContainer) :
    CSimpleFB(paContainer, cFBInterfaceSpec, paInstanceNameId, &scmInternalVars),
    var_MEM(true_BOOL),
    var_CLK(0_BOOL),
    var_Q(0_BOOL),
    conn_CNF(*this, 0),
    conn_CLK(nullptr),
    conn_Q(*this, 0, var_Q) {
}

void FORTE_FB_F_TRIG::setInitialValues() {
  CSimpleFB::setInitialValues();
  var_MEM = true_BOOL;
  var_CLK = 0_BOOL;
  var_Q = 0_BOOL;
}

void FORTE_FB_F_TRIG::executeEvent(const TEventID paEIID, CEventChainExecutionThread *const paECET) {
  switch (paEIID) {
    case scmEventREQID: enterStateREQ(paECET); break;
    default: break;
  }
}

void FORTE_FB_F_TRIG::enterStateREQ(CEventChainExecutionThread *const paECET) {
  alg_REQ();
  sendOutputEvent(scmEventCNFID, paECET);
}

void FORTE_FB_F_TRIG::readInputData(const TEventID paEIID) {
  switch (paEIID) {
    case scmEventREQID: {
      readData(0, var_CLK, conn_CLK);
      break;
    }
    default: break;
  }
}

void FORTE_FB_F_TRIG::writeOutputData(const TEventID paEIID) {
  switch (paEIID) {
    case scmEventCNFID: {
      writeData(1, var_Q, conn_Q);
      break;
    }
    default: break;
  }
}

CIEC_ANY *FORTE_FB_F_TRIG::getDI(const size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_CLK;
  }
  return nullptr;
}

CIEC_ANY *FORTE_FB_F_TRIG::getDO(const size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_Q;
  }
  return nullptr;
}

CEventConnection *FORTE_FB_F_TRIG::getEOConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_FB_F_TRIG::getDIConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_CLK;
  }
  return nullptr;
}

CDataConnection *FORTE_FB_F_TRIG::getDOConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_Q;
  }
  return nullptr;
}

CIEC_ANY *FORTE_FB_F_TRIG::getVarInternal(const size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_MEM;
  }
  return nullptr;
}

void FORTE_FB_F_TRIG::alg_REQ(void) {

#line 2 "FB_F_TRIG.fbt"
  var_Q = func_AND<CIEC_BOOL>(func_NOT<CIEC_BOOL>(var_CLK), func_NOT<CIEC_BOOL>(var_MEM));
#line 3 "FB_F_TRIG.fbt"
  var_MEM = func_NOT<CIEC_BOOL>(var_CLK);
}
