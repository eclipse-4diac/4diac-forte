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

#include "F_BOOL_TO_DWORD_fbt.h"

#include "core/datatypes/forte_bool.h"
#include "core/datatypes/forte_dword.h"
#include "core/iec61131_functions.h"
#include "core/datatypes/forte_array_common.h"
#include "core/datatypes/forte_array.h"
#include "core/datatypes/forte_array_fixed.h"
#include "core/datatypes/forte_array_variable.h"

USE_STRING_ID(BOOL);
USE_STRING_ID(CNF);
USE_STRING_ID(DWORD);
USE_STRING_ID(Event);
USE_STRING_ID(F_BOOL_TO_DWORD);
USE_STRING_ID(IN);
USE_STRING_ID(OUT);
USE_STRING_ID(REQ);

DEFINE_FIRMWARE_FB(FORTE_F_BOOL_TO_DWORD, STRID(F_BOOL_TO_DWORD))

namespace {
  const auto cDataInputNames = std::array{STRID(IN)};
  const auto cDataOutputNames = std::array{STRID(OUT)};
  const auto cEventInputNames = std::array{STRID(REQ)};
  const auto cEventOutputNames = std::array{STRID(CNF)};
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

FORTE_F_BOOL_TO_DWORD::FORTE_F_BOOL_TO_DWORD(const CStringDictionary::TStringId paInstanceNameId,
                                             forte::core::CFBContainer &paContainer) :
    CSimpleFB(paContainer, cFBInterfaceSpec, paInstanceNameId, nullptr),
    var_IN(false_BOOL),
    var_OUT(0_DWORD),
    conn_CNF(*this, 0),
    conn_IN(nullptr),
    conn_OUT(*this, 0, var_OUT) {
}

void FORTE_F_BOOL_TO_DWORD::setInitialValues() {
  var_IN = 0_BOOL;
  var_OUT = 0_DWORD;
}

void FORTE_F_BOOL_TO_DWORD::executeEvent(const TEventID paEIID, CEventChainExecutionThread *const paECET) {
  switch (paEIID) {
    case scmEventREQID: enterStateREQ(paECET); break;
    default: break;
  }
}

void FORTE_F_BOOL_TO_DWORD::enterStateREQ(CEventChainExecutionThread *const paECET) {
  alg_REQ();
  sendOutputEvent(scmEventCNFID, paECET);
}

void FORTE_F_BOOL_TO_DWORD::readInputData(const TEventID paEIID) {
  switch (paEIID) {
    case scmEventREQID: {
      readData(0, var_IN, conn_IN);
      break;
    }
    default: break;
  }
}

void FORTE_F_BOOL_TO_DWORD::writeOutputData(const TEventID paEIID) {
  switch (paEIID) {
    case scmEventCNFID: {
      writeData(cFBInterfaceSpec.getNumDIs() + 0, var_OUT, conn_OUT);
      break;
    }
    default: break;
  }
}

CIEC_ANY *FORTE_F_BOOL_TO_DWORD::getDI(const size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_IN;
  }
  return nullptr;
}

CIEC_ANY *FORTE_F_BOOL_TO_DWORD::getDO(const size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_OUT;
  }
  return nullptr;
}

CEventConnection *FORTE_F_BOOL_TO_DWORD::getEOConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_F_BOOL_TO_DWORD::getDIConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_IN;
  }
  return nullptr;
}

CDataConnection *FORTE_F_BOOL_TO_DWORD::getDOConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_OUT;
  }
  return nullptr;
}

CIEC_ANY *FORTE_F_BOOL_TO_DWORD::getVarInternal(size_t) {
  return nullptr;
}

void FORTE_F_BOOL_TO_DWORD::alg_REQ(void) {

#line 2 "F_BOOL_TO_DWORD.fbt"
  var_OUT = func_BOOL_TO_DWORD(var_IN);
}
