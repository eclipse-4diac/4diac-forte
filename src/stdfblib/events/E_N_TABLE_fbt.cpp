/*******************************************************************************
 * Copyright (c) 2017 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x NG!
 ***
 *** Name: E_N_TABLE
 *** Description: Generation of a finite train of sperate events
 *** Version:
 ***     1.0: 2017-09-22/Alois Zoitl - fortiss GmbH - initial API and implementation and/or initial documentation
 *************************************************************************/

#include "E_N_TABLE_fbt.h"

USE_STRING_ID(ARRAY);
USE_STRING_ID(CNF);
USE_STRING_ID(CV);
USE_STRING_ID(DT);
USE_STRING_ID(E_DEMUX);
USE_STRING_ID(EI);
USE_STRING_ID(E_N_TABLE);
USE_STRING_ID(EO);
USE_STRING_ID(EO0);
USE_STRING_ID(EO1);
USE_STRING_ID(EO2);
USE_STRING_ID(EO3);
USE_STRING_ID(E_TABLE);
USE_STRING_ID(F_SUB);
USE_STRING_ID(IN1);
USE_STRING_ID(K);
USE_STRING_ID(N);
USE_STRING_ID(OUT);
USE_STRING_ID(REQ);
USE_STRING_ID(START);
USE_STRING_ID(STOP);
USE_STRING_ID(TIME);
USE_STRING_ID(UINT);

#include "forte_uint.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"

DEFINE_FIRMWARE_FB(FORTE_E_N_TABLE, STRID(E_N_TABLE))

namespace {
  const auto cDataInputNames = std::array{STRID(DT), STRID(N)};
  const auto cEventInputNames = std::array{STRID(START), STRID(STOP)};
  const auto cEventOutputNames = std::array{STRID(EO0), STRID(EO1), STRID(EO2), STRID(EO3)};
  const SFBInterfaceSpec cFBInterfaceSpec = {
      .mEINames = cEventInputNames,
      .mEITypeNames = {},
      .mEONames = cEventOutputNames,
      .mEOTypeNames = {},
      .mDINames = cDataInputNames,
      .mDONames = {},
      .mDIONames = {},
      .mSocketNames = {},
      .mPlugNames = {},
  };

  const auto cEventConnections = std::to_array<SCFB_FBConnectionData>({
      {CStringDictionary::scmInvalidStringId, STRID(START), STRID(E_TABLE), STRID(START)},
      {CStringDictionary::scmInvalidStringId, STRID(STOP), STRID(E_TABLE), STRID(STOP)},
      {STRID(E_TABLE), STRID(EO), STRID(F_SUB), STRID(REQ)},
      {STRID(F_SUB), STRID(CNF), STRID(E_DEMUX), STRID(EI)},
      {STRID(E_DEMUX), STRID(EO0), CStringDictionary::scmInvalidStringId, STRID(EO0)},
      {STRID(E_DEMUX), STRID(EO1), CStringDictionary::scmInvalidStringId, STRID(EO1)},
      {STRID(E_DEMUX), STRID(EO2), CStringDictionary::scmInvalidStringId, STRID(EO2)},
      {STRID(E_DEMUX), STRID(EO3), CStringDictionary::scmInvalidStringId, STRID(EO3)},
  });

  const auto cDataConnections = std::to_array<SCFB_FBConnectionData>({
      {CStringDictionary::scmInvalidStringId, STRID(DT), STRID(E_TABLE), STRID(DT)},
      {CStringDictionary::scmInvalidStringId, STRID(N), STRID(E_TABLE), STRID(N)},
      {STRID(E_TABLE), STRID(CV), STRID(F_SUB), STRID(IN1)},
      {STRID(F_SUB), STRID(OUT), STRID(E_DEMUX), STRID(K)},
  });

  const SCFB_FBNData cFBNData = {
      .mEventConnections = cEventConnections,
      .mDataConnections = cDataConnections,
      .mAdapterConnections = {},
  };
} // namespace

FORTE_E_N_TABLE::FORTE_E_N_TABLE(const CStringDictionary::TStringId paInstanceNameId,
                                 forte::core::CFBContainer &paContainer) :
    CCompositeFB(paContainer, cFBInterfaceSpec, paInstanceNameId, cFBNData),
    fb_E_TABLE(STRID(E_TABLE), *this),
    fb_E_DEMUX(STRID(E_DEMUX), *this),
    fb_F_SUB(STRID(F_SUB), *this),
    conn_EO0(*this, 0),
    conn_EO1(*this, 1),
    conn_EO2(*this, 2),
    conn_EO3(*this, 3),
    conn_DT(nullptr),
    conn_N(nullptr),
    conn_if2in_DT(*this, 0, CIEC_ARRAY_FIXED<CIEC_TIME, 0, 3>{}),
    conn_if2in_N(*this, 0, 0_UINT) {};

void FORTE_E_N_TABLE::setInitialValues() {
  conn_if2in_DT.getValue() = CIEC_ARRAY_FIXED<CIEC_TIME, 0, 3>{};
  conn_if2in_N.getValue() = 0_UINT;
}

void FORTE_E_N_TABLE::setFBNetworkInitialValues() {
  fb_F_SUB->var_IN2 = 1_UINT;
}

void FORTE_E_N_TABLE::readInputData(const TEventID paEIID) {
  switch (paEIID) {
    case scmEventSTARTID: {
      readData(0, conn_if2in_DT.getValue(), conn_DT);
      readData(1, conn_if2in_N.getValue(), conn_N);
      break;
    }
    default: break;
  }
}

void FORTE_E_N_TABLE::writeOutputData(TEventID) {
  // nothing to do
}

CIEC_ANY *FORTE_E_N_TABLE::getDI(const size_t paIndex) {
  switch (paIndex) {
    case 0: return &conn_if2in_DT.getValue();
    case 1: return &conn_if2in_N.getValue();
  }
  return nullptr;
}

CIEC_ANY *FORTE_E_N_TABLE::getDO(size_t) {
  return nullptr;
}

CEventConnection *FORTE_E_N_TABLE::getEOConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_EO0;
    case 1: return &conn_EO1;
    case 2: return &conn_EO2;
    case 3: return &conn_EO3;
  }
  return nullptr;
}

CDataConnection **FORTE_E_N_TABLE::getDIConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_DT;
    case 1: return &conn_N;
  }
  return nullptr;
}

CDataConnection *FORTE_E_N_TABLE::getDOConUnchecked(TPortId) {
  return nullptr;
}

CDataConnection *FORTE_E_N_TABLE::getIf2InConUnchecked(TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_if2in_DT;
    case 1: return &conn_if2in_N;
  }
  return nullptr;
}
