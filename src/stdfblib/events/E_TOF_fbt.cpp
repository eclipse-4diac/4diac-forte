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
 *** Name: E_TOF
 *** Description: standard timer function block (off-delay timing)
 *** Version:
 ***     1.0: 2024-03-04/Franz Hoepfinger - HR Agrartechnik GmbH -
 ***     1.1: 2024-04-23/Franz Hoepfinger - HR Agrartechnik GmbH - Add a Reset to Timer FBs
 *************************************************************************/

#include "E_TOF_fbt.h"

USE_STRING_ID(BOOL);
USE_STRING_ID(CNF);
USE_STRING_ID(DT);
USE_STRING_ID(E_DELAY);
USE_STRING_ID(EI);
USE_STRING_ID(EO);
USE_STRING_ID(EO0);
USE_STRING_ID(EO1);
USE_STRING_ID(E_RS);
USE_STRING_ID(E_SWITCH);
USE_STRING_ID(E_TOF);
USE_STRING_ID(Event);
USE_STRING_ID(G);
USE_STRING_ID(IN);
USE_STRING_ID(PT);
USE_STRING_ID(Q);
USE_STRING_ID(R);
USE_STRING_ID(REQ);
USE_STRING_ID(S);
USE_STRING_ID(START);
USE_STRING_ID(STOP);
USE_STRING_ID(TIME);

DEFINE_FIRMWARE_FB(FORTE_E_TOF, STRID(E_TOF))

namespace {
  const auto cDataInputNames = std::array{STRID(IN), STRID(PT)};
  const auto cDataOutputNames = std::array{STRID(Q)};
  const auto cEventInputNames = std::array{STRID(REQ), STRID(R)};
  const auto cEventInputTypeIds = std::array{STRID(Event), STRID(Event)};
  const auto cEventOutputNames = std::array{STRID(CNF)};
  const auto cEventOutputTypeIds = std::array{STRID(Event)};
  const SFBInterfaceSpec cFBInterfaceSpec = {
      .mEINames = cEventInputNames,
      .mEITypeNames = cEventInputTypeIds,
      .mEONames = cEventOutputNames,
      .mEOTypeNames = cEventOutputTypeIds,
      .mDINames = cDataInputNames,
      .mDONames = cDataOutputNames,
      .mDIONames = {},
      .mSocketNames = {},
      .mPlugNames = {},
  };

  const auto cEventConnections = std::to_array<SCFB_FBConnectionData>({
      {CStringDictionary::scmInvalidStringId, STRID(REQ), STRID(E_SWITCH), STRID(EI)},
      {STRID(E_RS), STRID(EO), CStringDictionary::scmInvalidStringId, STRID(CNF)},
      {STRID(E_DELAY), STRID(EO), STRID(E_RS), STRID(R)},
      {STRID(E_SWITCH), STRID(EO1), STRID(E_RS), STRID(S)},
      {STRID(E_SWITCH), STRID(EO1), STRID(E_DELAY), STRID(STOP)},
      {STRID(E_SWITCH), STRID(EO0), STRID(E_DELAY), STRID(START)},
      {CStringDictionary::scmInvalidStringId, STRID(R), STRID(E_RS), STRID(R)},
      {CStringDictionary::scmInvalidStringId, STRID(R), STRID(E_DELAY), STRID(STOP)},
  });

  const auto cDataConnections = std::to_array<SCFB_FBConnectionData>({
      {CStringDictionary::scmInvalidStringId, STRID(IN), STRID(E_SWITCH), STRID(G)},
      {CStringDictionary::scmInvalidStringId, STRID(PT), STRID(E_DELAY), STRID(DT)},
      {STRID(E_RS), STRID(Q), CStringDictionary::scmInvalidStringId, STRID(Q)},
  });

  const SCFB_FBNData cFBNData = {
      .mEventConnections = cEventConnections,
      .mDataConnections = cDataConnections,
      .mAdapterConnections = {},
  };
} // namespace

FORTE_E_TOF::FORTE_E_TOF(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    CCompositeFB(paContainer, cFBInterfaceSpec, paInstanceNameId, cFBNData),
    fb_E_SWITCH(STRID(E_SWITCH), *this),
    fb_E_DELAY(STRID(E_DELAY), *this),
    fb_E_RS(STRID(E_RS), *this),
    conn_CNF(*this, 0),
    conn_IN(nullptr),
    conn_PT(nullptr),
    conn_Q(*this, 0, 0_BOOL),
    conn_if2in_IN(*this, 0, 0_BOOL),
    conn_if2in_PT(*this, 1, 0_TIME) {};

void FORTE_E_TOF::setInitialValues() {
  conn_if2in_IN.getValue() = 0_BOOL;
  conn_if2in_PT.getValue() = 0_TIME;
  fb_E_RS->conn_Q.getValue() = 0_BOOL;
}

void FORTE_E_TOF::readInputData(const TEventID paEIID) {
  switch (paEIID) {
    case scmEventREQID: {
      readData(0, conn_if2in_IN.getValue(), conn_IN);
      readData(1, conn_if2in_PT.getValue(), conn_PT);
      break;
    }
    default: break;
  }
}

void FORTE_E_TOF::writeOutputData(const TEventID paEIID) {
  switch (paEIID) {
    case scmEventCNFID: {
      writeData(cFBInterfaceSpec.getNumDIs() + 0, fb_E_RS->conn_Q.getValue(), conn_Q);
      break;
    }
    default: break;
  }
}

CIEC_ANY *FORTE_E_TOF::getDI(const size_t paIndex) {
  switch (paIndex) {
    case 0: return &conn_if2in_IN.getValue();
    case 1: return &conn_if2in_PT.getValue();
  }
  return nullptr;
}

CIEC_ANY *FORTE_E_TOF::getDO(const size_t paIndex) {
  switch (paIndex) {
    case 0: return &fb_E_RS->conn_Q.getValue();
  }
  return nullptr;
}

CEventConnection *FORTE_E_TOF::getEOConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_E_TOF::getDIConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_IN;
    case 1: return &conn_PT;
  }
  return nullptr;
}

CDataConnection *FORTE_E_TOF::getDOConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_Q;
  }
  return nullptr;
}

CDataConnection *FORTE_E_TOF::getIf2InConUnchecked(TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_if2in_IN;
    case 1: return &conn_if2in_PT;
  }
  return nullptr;
}
