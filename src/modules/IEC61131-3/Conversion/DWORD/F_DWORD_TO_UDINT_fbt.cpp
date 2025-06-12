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

#include "F_DWORD_TO_UDINT_fbt.h"

#include "core/datatypes/forte_dword.h"
#include "core/datatypes/forte_udint.h"
#include "core/iec61131_functions.h"
#include "core/datatypes/forte_array_common.h"
#include "core/datatypes/forte_array.h"
#include "core/datatypes/forte_array_fixed.h"
#include "core/datatypes/forte_array_variable.h"

USE_STRING_ID(CNF);
USE_STRING_ID(DWORD);
USE_STRING_ID(Event);
USE_STRING_ID(F_DWORD_TO_UDINT);
USE_STRING_ID(IN);
USE_STRING_ID(OUT);
USE_STRING_ID(REQ);
USE_STRING_ID(UDINT);

DEFINE_FIRMWARE_FB(FORTE_F_DWORD_TO_UDINT, STRID(F_DWORD_TO_UDINT))

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

FORTE_F_DWORD_TO_UDINT::FORTE_F_DWORD_TO_UDINT(const CStringDictionary::TStringId paInstanceNameId,
                                               forte::core::CFBContainer &paContainer) :
    CSimpleFB(paContainer, cFBInterfaceSpec, paInstanceNameId, nullptr),
    var_IN(0_DWORD),
    var_OUT(0_UDINT),
    conn_CNF(*this, 0),
    conn_IN(nullptr),
    conn_OUT(*this, 0, var_OUT) {
}

void FORTE_F_DWORD_TO_UDINT::setInitialValues() {
  var_IN = 0_DWORD;
  var_OUT = 0_UDINT;
}

void FORTE_F_DWORD_TO_UDINT::executeEvent(const TEventID paEIID, CEventChainExecutionThread *const paECET) {
  switch (paEIID) {
    case scmEventREQID: enterStateREQ(paECET); break;
    default: break;
  }
}

void FORTE_F_DWORD_TO_UDINT::enterStateREQ(CEventChainExecutionThread *const paECET) {
  alg_REQ();
  sendOutputEvent(scmEventCNFID, paECET);
}

void FORTE_F_DWORD_TO_UDINT::readInputData(const TEventID paEIID) {
  switch (paEIID) {
    case scmEventREQID: {
      readData(0, var_IN, conn_IN);
      break;
    }
    default: break;
  }
}

void FORTE_F_DWORD_TO_UDINT::writeOutputData(const TEventID paEIID) {
  switch (paEIID) {
    case scmEventCNFID: {
      writeData(cFBInterfaceSpec.getNumDIs() + 0, var_OUT, conn_OUT);
      break;
    }
    default: break;
  }
}

CIEC_ANY *FORTE_F_DWORD_TO_UDINT::getDI(const size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_IN;
  }
  return nullptr;
}

CIEC_ANY *FORTE_F_DWORD_TO_UDINT::getDO(const size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_OUT;
  }
  return nullptr;
}

CEventConnection *FORTE_F_DWORD_TO_UDINT::getEOConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_F_DWORD_TO_UDINT::getDIConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_IN;
  }
  return nullptr;
}

CDataConnection *FORTE_F_DWORD_TO_UDINT::getDOConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_OUT;
  }
  return nullptr;
}

CIEC_ANY *FORTE_F_DWORD_TO_UDINT::getVarInternal(size_t) {
  return nullptr;
}

void FORTE_F_DWORD_TO_UDINT::alg_REQ(void) {

#line 2 "F_DWORD_TO_UDINT.fbt"
  var_OUT = func_DWORD_TO_UDINT(var_IN);
}
