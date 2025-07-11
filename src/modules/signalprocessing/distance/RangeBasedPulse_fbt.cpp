/*************************************************************************
 *** Copyright (c) 2024 HR Agrartechnik GmbH
 *** This program and the accompanying materials are made available under the
 *** terms of the Eclipse Public License 2.0 which is available at
 *** http://www.eclipse.org/legal/epl-2.0.
 ***
 *** SPDX-License-Identifier: EPL-2.0
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x NG!
 ***
 *** Name: RangeBasedPulse
 *** Description: Distance based Impulse Generator
 *** Version:
 ***     1.0: 2024-10-18/Franz Höpfinger - HR Agrartechnik GmbH - initial Version
 ***     1.1: 2024-10-18/Franz Höpfinger - HR Agrartechnik GmbH - added Offset
 *************************************************************************/

#include "RangeBasedPulse_fbt.h"

USE_STRING_ID(BOOL);
USE_STRING_ID(CNF);
USE_STRING_ID(DIST_HIGH);
USE_STRING_ID(DIST_IN);
USE_STRING_ID(DIST_LOW);
USE_STRING_ID(DIST_OFF);
USE_STRING_ID(DIST_REMAINDER);
USE_STRING_ID(Q);
USE_STRING_ID(REQ);
USE_STRING_ID(signalprocessing__distance__RangeBasedPulse);
USE_STRING_ID(UDINT);

#include "core/datatypes/forte_bool.h"
#include "core/datatypes/forte_udint.h"
#include "core/iec61131_functions.h"
#include "core/datatypes/forte_array_common.h"
#include "core/datatypes/forte_array.h"
#include "core/datatypes/forte_array_fixed.h"
#include "core/datatypes/forte_array_variable.h"

DEFINE_FIRMWARE_FB(FORTE_signalprocessing__distance__RangeBasedPulse,
                   STRID(signalprocessing__distance__RangeBasedPulse))

namespace {
  const auto cDataInputNames = std::array{STRID(DIST_IN), STRID(DIST_OFF), STRID(DIST_HIGH), STRID(DIST_LOW)};
  const auto cDataOutputNames = std::array{STRID(Q)};
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

  const auto cInternalsNames = std::array{STRID(DIST_REMAINDER)};
} // namespace

FORTE_signalprocessing__distance__RangeBasedPulse::FORTE_signalprocessing__distance__RangeBasedPulse(
    const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    CSimpleFB(paContainer, cFBInterfaceSpec, paInstanceNameId, cInternalsNames),
    conn_CNF(*this, 0),
    conn_DIST_IN(nullptr),
    conn_DIST_OFF(nullptr),
    conn_DIST_HIGH(nullptr),
    conn_DIST_LOW(nullptr),
    conn_Q(*this, 0, var_Q) {
}

void FORTE_signalprocessing__distance__RangeBasedPulse::setInitialValues() {
  var_DIST_REMAINDER = 0_UDINT;
  var_DIST_IN = 0_UDINT;
  var_DIST_OFF = 0_UDINT;
  var_DIST_HIGH = 0_UDINT;
  var_DIST_LOW = 0_UDINT;
  var_Q = 0_BOOL;
}

void FORTE_signalprocessing__distance__RangeBasedPulse::executeEvent(const TEventID paEIID,
                                                                     CEventChainExecutionThread *const paECET) {
  switch (paEIID) {
    case scmEventREQID: enterStateREQ(paECET); break;
    default: break;
  }
}

void FORTE_signalprocessing__distance__RangeBasedPulse::enterStateREQ(CEventChainExecutionThread *const paECET) {
  alg_REQ();
  sendOutputEvent(scmEventCNFID, paECET);
}

void FORTE_signalprocessing__distance__RangeBasedPulse::readInputData(const TEventID paEIID) {
  switch (paEIID) {
    case scmEventREQID: {
      readData(0, var_DIST_IN, conn_DIST_IN);
      readData(1, var_DIST_OFF, conn_DIST_OFF);
      readData(2, var_DIST_HIGH, conn_DIST_HIGH);
      readData(3, var_DIST_LOW, conn_DIST_LOW);
      break;
    }
    default: break;
  }
}

void FORTE_signalprocessing__distance__RangeBasedPulse::writeOutputData(const TEventID paEIID) {
  switch (paEIID) {
    case scmEventCNFID: {
      writeData(cFBInterfaceSpec.getNumDIs() + 0, var_Q, conn_Q);
      break;
    }
    default: break;
  }
}

CIEC_ANY *FORTE_signalprocessing__distance__RangeBasedPulse::getDI(const size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_DIST_IN;
    case 1: return &var_DIST_OFF;
    case 2: return &var_DIST_HIGH;
    case 3: return &var_DIST_LOW;
  }
  return nullptr;
}

CIEC_ANY *FORTE_signalprocessing__distance__RangeBasedPulse::getDO(const size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_Q;
  }
  return nullptr;
}

CEventConnection *FORTE_signalprocessing__distance__RangeBasedPulse::getEOConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_signalprocessing__distance__RangeBasedPulse::getDIConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_DIST_IN;
    case 1: return &conn_DIST_OFF;
    case 2: return &conn_DIST_HIGH;
    case 3: return &conn_DIST_LOW;
  }
  return nullptr;
}

CDataConnection *FORTE_signalprocessing__distance__RangeBasedPulse::getDOConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_Q;
  }
  return nullptr;
}

CIEC_ANY *FORTE_signalprocessing__distance__RangeBasedPulse::getVarInternal(const size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_DIST_REMAINDER;
  }
  return nullptr;
}

void FORTE_signalprocessing__distance__RangeBasedPulse::alg_REQ(void) {

#line 3 "RangeBasedPulse.fbt"
  var_DIST_REMAINDER = func_MOD<CIEC_UDINT>(func_ADD<CIEC_UDINT>(var_DIST_IN, var_DIST_OFF),
                                            func_ADD<CIEC_UDINT>(var_DIST_HIGH, var_DIST_LOW));
#line 5 "RangeBasedPulse.fbt"
  var_Q = func_GE(var_DIST_REMAINDER, var_DIST_HIGH);
}
