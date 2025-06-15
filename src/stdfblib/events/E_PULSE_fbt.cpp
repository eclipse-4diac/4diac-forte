/*************************************************************************
 *** Copyright (c) 2023, 2024 HR Agrartechnik GmbH
 *** This program and the accompanying materials are made available under the
 *** terms of the Eclipse Public License 2.0 which is available at
 *** http://www.eclipse.org/legal/epl-2.0.
 ***
 *** SPDX-License-Identifier: EPL-2.0
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x NG!
 ***
 *** Name: E_PULSE
 *** Description: standard timer function block (pulse)
 *** Version:
 ***     1.0: 2023-08-21/Franz Hoepfinger - HR Agrartechnik GmbH - initial implementation as E_IMPULSE
 ***     1.0: 2024-03-05/Franz Hoepfinger - HR Agrartechnik GmbH - renamed to E_PULSE
 ***     1.1: 2024-04-23/Franz Hoepfinger - HR Agrartechnik GmbH - Add a Reset to Timer FBs
 *************************************************************************/

#include "E_PULSE_fbt.h"

USE_STRING_ID(BOOL);
USE_STRING_ID(CNF);
USE_STRING_ID(DT);
USE_STRING_ID(E_DELAY);
USE_STRING_ID(EO);
USE_STRING_ID(E_PULSE);
USE_STRING_ID(E_SR);
USE_STRING_ID(Event);
USE_STRING_ID(PT);
USE_STRING_ID(Q);
USE_STRING_ID(R);
USE_STRING_ID(REQ);
USE_STRING_ID(S);
USE_STRING_ID(START);
USE_STRING_ID(STOP);
USE_STRING_ID(TIME);

DEFINE_FIRMWARE_FB(FORTE_E_PULSE, STRID(E_PULSE))

namespace {
  const auto cDataInputNames = std::array{STRID(PT)};
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
      {CStringDictionary::scmInvalidStringId, STRID(REQ), STRID(E_DELAY), STRID(START)},
      {CStringDictionary::scmInvalidStringId, STRID(REQ), STRID(E_SR), STRID(S)},
      {STRID(E_DELAY), STRID(EO), STRID(E_SR), STRID(R)},
      {STRID(E_SR), STRID(EO), CStringDictionary::scmInvalidStringId, STRID(CNF)},
      {CStringDictionary::scmInvalidStringId, STRID(R), STRID(E_DELAY), STRID(STOP)},
      {CStringDictionary::scmInvalidStringId, STRID(R), STRID(E_SR), STRID(R)},
  });

  const auto cDataConnections = std::to_array<SCFB_FBConnectionData>({
      {CStringDictionary::scmInvalidStringId, STRID(PT), STRID(E_DELAY), STRID(DT)},
      {STRID(E_SR), STRID(Q), CStringDictionary::scmInvalidStringId, STRID(Q)},
  });

  const SCFB_FBNData cFBNData = {
      .mEventConnections = cEventConnections,
      .mDataConnections = cDataConnections,
      .mAdapterConnections = {},
  };
} // namespace

FORTE_E_PULSE::FORTE_E_PULSE(const CStringDictionary::TStringId paInstanceNameId,
                             forte::core::CFBContainer &paContainer) :
    CCompositeFB(paContainer, cFBInterfaceSpec, paInstanceNameId, cFBNData),
    fb_E_DELAY(STRID(E_DELAY), *this),
    fb_E_SR(STRID(E_SR), *this),
    conn_CNF(*this, 0),
    conn_PT(nullptr),
    conn_Q(*this, 0, 0_BOOL),
    conn_if2in_PT(*this, 0, 0_TIME) {};

void FORTE_E_PULSE::setInitialValues() {
  conn_if2in_PT.getValue() = 0_TIME;
  fb_E_SR->conn_Q.getValue() = 0_BOOL;
}

void FORTE_E_PULSE::readInputData(const TEventID paEIID) {
  switch (paEIID) {
    case scmEventREQID: {
      readData(0, conn_if2in_PT.getValue(), conn_PT);
      break;
    }
    default: break;
  }
}

void FORTE_E_PULSE::writeOutputData(const TEventID paEIID) {
  switch (paEIID) {
    case scmEventCNFID: {
      writeData(cFBInterfaceSpec.getNumDIs() + 0, fb_E_SR->conn_Q.getValue(), conn_Q);
      break;
    }
    default: break;
  }
}

CIEC_ANY *FORTE_E_PULSE::getDI(const size_t paIndex) {
  switch (paIndex) {
    case 0: return &conn_if2in_PT.getValue();
  }
  return nullptr;
}

CIEC_ANY *FORTE_E_PULSE::getDO(const size_t paIndex) {
  switch (paIndex) {
    case 0: return &fb_E_SR->conn_Q.getValue();
  }
  return nullptr;
}

CEventConnection *FORTE_E_PULSE::getEOConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_E_PULSE::getDIConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_PT;
  }
  return nullptr;
}

CDataConnection *FORTE_E_PULSE::getDOConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_Q;
  }
  return nullptr;
}

CDataConnection *FORTE_E_PULSE::getIf2InConUnchecked(TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_if2in_PT;
  }
  return nullptr;
}
