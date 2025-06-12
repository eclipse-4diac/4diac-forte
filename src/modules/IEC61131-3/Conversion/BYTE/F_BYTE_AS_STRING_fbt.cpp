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

#include "F_BYTE_AS_STRING_fbt.h"

#include "core/datatypes/forte_byte.h"
#include "core/datatypes/forte_string.h"
#include "core/iec61131_functions.h"
#include "core/datatypes/forte_array_common.h"
#include "core/datatypes/forte_array.h"
#include "core/datatypes/forte_array_fixed.h"
#include "core/datatypes/forte_array_variable.h"

USE_STRING_ID(BYTE);
USE_STRING_ID(CNF);
USE_STRING_ID(Event);
USE_STRING_ID(F_BYTE_AS_STRING);
USE_STRING_ID(IN);
USE_STRING_ID(OUT);
USE_STRING_ID(REQ);
USE_STRING_ID(STRING);

DEFINE_FIRMWARE_FB(FORTE_F_BYTE_AS_STRING, STRID(F_BYTE_AS_STRING))

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

FORTE_F_BYTE_AS_STRING::FORTE_F_BYTE_AS_STRING(const CStringDictionary::TStringId paInstanceNameId,
                                               forte::core::CFBContainer &paContainer) :
    CSimpleFB(paContainer, cFBInterfaceSpec, paInstanceNameId, nullptr),
    var_IN(0_BYTE),
    var_OUT(""_STRING),
    conn_CNF(*this, 0),
    conn_IN(nullptr),
    conn_OUT(*this, 0, var_OUT) {
}

void FORTE_F_BYTE_AS_STRING::setInitialValues() {
  var_IN = 0_BYTE;
  var_OUT = ""_STRING;
}

void FORTE_F_BYTE_AS_STRING::executeEvent(const TEventID paEIID, CEventChainExecutionThread *const paECET) {
  switch (paEIID) {
    case scmEventREQID: enterStateREQ(paECET); break;
    default: break;
  }
}

void FORTE_F_BYTE_AS_STRING::enterStateREQ(CEventChainExecutionThread *const paECET) {
  alg_REQ();
  sendOutputEvent(scmEventCNFID, paECET);
}

void FORTE_F_BYTE_AS_STRING::readInputData(const TEventID paEIID) {
  switch (paEIID) {
    case scmEventREQID: {
      readData(0, var_IN, conn_IN);
      break;
    }
    default: break;
  }
}

void FORTE_F_BYTE_AS_STRING::writeOutputData(const TEventID paEIID) {
  switch (paEIID) {
    case scmEventCNFID: {
      writeData(cFBInterfaceSpec.getNumDIs() + 0, var_OUT, conn_OUT);
      break;
    }
    default: break;
  }
}

CIEC_ANY *FORTE_F_BYTE_AS_STRING::getDI(const size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_IN;
  }
  return nullptr;
}

CIEC_ANY *FORTE_F_BYTE_AS_STRING::getDO(const size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_OUT;
  }
  return nullptr;
}

CEventConnection *FORTE_F_BYTE_AS_STRING::getEOConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_F_BYTE_AS_STRING::getDIConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_IN;
  }
  return nullptr;
}

CDataConnection *FORTE_F_BYTE_AS_STRING::getDOConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_OUT;
  }
  return nullptr;
}

CIEC_ANY *FORTE_F_BYTE_AS_STRING::getVarInternal(size_t) {
  return nullptr;
}

void FORTE_F_BYTE_AS_STRING::alg_REQ(void) {

#line 2 "F_BYTE_AS_STRING.fbt"
  var_OUT = func_BYTE_AS_STRING(var_IN);
}
