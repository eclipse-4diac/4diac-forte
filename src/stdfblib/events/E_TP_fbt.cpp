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
 *** Name: E_TP
 *** Description: standard timer function block (pulse)
 *** Version:
 ***     1.0: 2024-03-04/Franz Hoepfinger - HR Agrartechnik GmbH -
 ***     1.1: 2024-04-23/Franz Hoepfinger - HR Agrartechnik GmbH - Add a Reset to Timer FBs
 *************************************************************************/

#include "E_TP_fbt.h"

USE_STRING_ID(BOOL);
USE_STRING_ID(CNF);
USE_STRING_ID(DT);
USE_STRING_ID(E_DELAY);
USE_STRING_ID(EI);
USE_STRING_ID(EO);
USE_STRING_ID(E_PERMIT);
USE_STRING_ID(E_RS);
USE_STRING_ID(E_TP);
USE_STRING_ID(Event);
USE_STRING_ID(IN);
USE_STRING_ID(PERMIT);
USE_STRING_ID(PT);
USE_STRING_ID(Q);
USE_STRING_ID(R);
USE_STRING_ID(REQ);
USE_STRING_ID(S);
USE_STRING_ID(START);
USE_STRING_ID(STOP);
USE_STRING_ID(TIME);


DEFINE_FIRMWARE_FB(FORTE_E_TP, STRID(E_TP))

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
}


FORTE_E_TP::FORTE_E_TP(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    CCompositeFB(paContainer, cFBInterfaceSpec, paInstanceNameId, scmFBNData),
    fb_E_DELAY(STRID(E_DELAY), *this),
    fb_E_RS(STRID(E_RS), *this),
    fb_E_PERMIT(STRID(E_PERMIT), *this),
    conn_CNF(*this, 0),
    conn_IN(nullptr),
    conn_PT(nullptr),
    conn_Q(*this, 0, 0_BOOL),
    conn_if2in_IN(*this, 0, 0_BOOL),
    conn_if2in_PT(*this, 1, 0_TIME) {};

void FORTE_E_TP::setInitialValues() {
  conn_if2in_IN.getValue() = 0_BOOL;
  conn_if2in_PT.getValue() = 0_TIME;
  fb_E_RS->conn_Q.getValue() = 0_BOOL;
}

const SCFB_FBInstanceData FORTE_E_TP::scmInternalFBs[] = {
    {STRID(E_DELAY), STRID(E_DELAY)}, {STRID(E_RS), STRID(E_RS)}, {STRID(E_PERMIT), STRID(E_PERMIT)}};

const SCFB_FBConnectionData FORTE_E_TP::scmEventConnections[] = {
    {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_RS), STRID(EO)), 1, GENERATE_CONNECTION_PORT_ID_1_ARG(STRID(CNF)), -1},
    {GENERATE_CONNECTION_PORT_ID_1_ARG(STRID(R)), -1, GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_DELAY), STRID(STOP)),
     0},
    {GENERATE_CONNECTION_PORT_ID_1_ARG(STRID(R)), -1, GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_RS), STRID(R)), 1},
    {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_DELAY), STRID(EO)), 0,
     GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_RS), STRID(R)), 1},
    {GENERATE_CONNECTION_PORT_ID_1_ARG(STRID(REQ)), -1, GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_PERMIT), STRID(EI)),
     2},
    {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_PERMIT), STRID(EO)), 2,
     GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_RS), STRID(S)), 1},
    {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_PERMIT), STRID(EO)), 2,
     GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_DELAY), STRID(START)), 0},
};

const SCFB_FBConnectionData FORTE_E_TP::scmDataConnections[] = {
    {GENERATE_CONNECTION_PORT_ID_1_ARG(STRID(PT)), -1, GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_DELAY), STRID(DT)), 0},
    {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_RS), STRID(Q)), 1, GENERATE_CONNECTION_PORT_ID_1_ARG(STRID(Q)), -1},
    {GENERATE_CONNECTION_PORT_ID_1_ARG(STRID(IN)), -1,
     GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_PERMIT), STRID(PERMIT)), 2},
};

const SCFB_FBNData FORTE_E_TP::scmFBNData = {
    3, scmInternalFBs, 7, scmEventConnections, 3, scmDataConnections, 0, nullptr, 0, nullptr};

void FORTE_E_TP::readInputData(const TEventID paEIID) {
  switch (paEIID) {
    case scmEventREQID: {
      readData(0, conn_if2in_IN.getValue(), conn_IN);
      readData(1, conn_if2in_PT.getValue(), conn_PT);
      break;
    }
    case scmEventRID: {
      readData(0, conn_if2in_IN.getValue(), conn_IN);
      break;
    }
    default: break;
  }
}

void FORTE_E_TP::writeOutputData(const TEventID paEIID) {
  switch (paEIID) {
    case scmEventCNFID: {
      writeData(cFBInterfaceSpec.getNumDIs() + 0, fb_E_RS->conn_Q.getValue(), conn_Q);
      break;
    }
    default: break;
  }
}

CIEC_ANY *FORTE_E_TP::getDI(const size_t paIndex) {
  switch (paIndex) {
    case 0: return &conn_if2in_IN.getValue();
    case 1: return &conn_if2in_PT.getValue();
  }
  return nullptr;
}

CIEC_ANY *FORTE_E_TP::getDO(const size_t paIndex) {
  switch (paIndex) {
    case 0: return &fb_E_RS->conn_Q.getValue();
  }
  return nullptr;
}

CEventConnection *FORTE_E_TP::getEOConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_E_TP::getDIConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_IN;
    case 1: return &conn_PT;
  }
  return nullptr;
}

CDataConnection *FORTE_E_TP::getDOConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_Q;
  }
  return nullptr;
}

CDataConnection *FORTE_E_TP::getIf2InConUnchecked(TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_if2in_IN;
    case 1: return &conn_if2in_PT;
  }
  return nullptr;
}
