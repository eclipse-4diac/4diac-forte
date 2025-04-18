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
 *** Name: E_TON
 *** Description: standard timer function block (on-delay timing)
 *** Version:
 ***     1.0: 2024-03-04/Franz Hoepfinger - HR Agrartechnik GmbH -
 *************************************************************************/

#include "E_TON_fbt.h"

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
USE_STRING_ID(E_TON);
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

#include "criticalregion.h"
#include "resource.h"

DEFINE_FIRMWARE_FB(FORTE_E_TON, STRID(E_TON))

const CStringDictionary::TStringId FORTE_E_TON::scmDataInputNames[] = {STRID(IN), STRID(PT)};
const CStringDictionary::TStringId FORTE_E_TON::scmDataInputTypeIds[] = {STRID(BOOL), STRID(TIME)};
const CStringDictionary::TStringId FORTE_E_TON::scmDataOutputNames[] = {STRID(Q)};
const CStringDictionary::TStringId FORTE_E_TON::scmDataOutputTypeIds[] = {STRID(BOOL)};
const TDataIOID FORTE_E_TON::scmEIWith[] = {0, 1, scmWithListDelimiter};
const TForteInt16 FORTE_E_TON::scmEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_E_TON::scmEventInputNames[] = {STRID(REQ)};
const CStringDictionary::TStringId FORTE_E_TON::scmEventInputTypeIds[] = {STRID(Event)};
const TDataIOID FORTE_E_TON::scmEOWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_E_TON::scmEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_E_TON::scmEventOutputNames[] = {STRID(CNF)};
const CStringDictionary::TStringId FORTE_E_TON::scmEventOutputTypeIds[] = {STRID(Event)};
const SFBInterfaceSpec FORTE_E_TON::scmFBInterfaceSpec = {1,
                                                          scmEventInputNames,
                                                          scmEventInputTypeIds,
                                                          scmEIWith,
                                                          scmEIWithIndexes,
                                                          1,
                                                          scmEventOutputNames,
                                                          scmEventOutputTypeIds,
                                                          scmEOWith,
                                                          scmEOWithIndexes,
                                                          2,
                                                          scmDataInputNames,
                                                          scmDataInputTypeIds,
                                                          1,
                                                          scmDataOutputNames,
                                                          scmDataOutputTypeIds,
                                                          0,
                                                          nullptr,
                                                          0,
                                                          nullptr};

FORTE_E_TON::FORTE_E_TON(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    CCompositeFB(paContainer, scmFBInterfaceSpec, paInstanceNameId, scmFBNData),
    fb_E_SWITCH(STRID(E_SWITCH), *this),
    fb_E_DELAY(STRID(E_DELAY), *this),
    fb_E_RS(STRID(E_RS), *this),
    conn_CNF(*this, 0),
    conn_IN(nullptr),
    conn_PT(nullptr),
    conn_Q(*this, 0, 0_BOOL),
    conn_if2in_IN(*this, 0, 0_BOOL),
    conn_if2in_PT(*this, 1, 0_TIME) {};

void FORTE_E_TON::setInitialValues() {
  conn_if2in_IN.getValue() = 0_BOOL;
  conn_if2in_PT.getValue() = 0_TIME;
  fb_E_RS->conn_Q.getValue() = 0_BOOL;
}

const SCFB_FBInstanceData FORTE_E_TON::scmInternalFBs[] = {
    {STRID(E_SWITCH), STRID(E_SWITCH)}, {STRID(E_DELAY), STRID(E_DELAY)}, {STRID(E_RS), STRID(E_RS)}};

const SCFB_FBConnectionData FORTE_E_TON::scmEventConnections[] = {
    {GENERATE_CONNECTION_PORT_ID_1_ARG(STRID(REQ)), -1, GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_SWITCH), STRID(EI)),
     0},
    {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_SWITCH), STRID(EO1)), 0,
     GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_DELAY), STRID(START)), 1},
    {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_SWITCH), STRID(EO0)), 0,
     GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_DELAY), STRID(STOP)), 1},
    {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_SWITCH), STRID(EO1)), 0,
     GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_RS), STRID(R)), 2},
    {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_DELAY), STRID(EO)), 1,
     GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_RS), STRID(S)), 2},
    {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_RS), STRID(EO)), 2, GENERATE_CONNECTION_PORT_ID_1_ARG(STRID(CNF)), -1},
};

const SCFB_FBConnectionData FORTE_E_TON::scmDataConnections[] = {
    {GENERATE_CONNECTION_PORT_ID_1_ARG(STRID(IN)), -1, GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_SWITCH), STRID(G)), 0},
    {GENERATE_CONNECTION_PORT_ID_1_ARG(STRID(PT)), -1, GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_DELAY), STRID(DT)), 1},
    {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_RS), STRID(Q)), 2, GENERATE_CONNECTION_PORT_ID_1_ARG(STRID(Q)), -1},
};

const SCFB_FBNData FORTE_E_TON::scmFBNData = {
    3, scmInternalFBs, 6, scmEventConnections, 3, scmDataConnections, 0, nullptr, 0, nullptr};

void FORTE_E_TON::readInputData(const TEventID paEIID) {
  switch (paEIID) {
    case scmEventREQID: {
      readData(0, conn_if2in_IN.getValue(), conn_IN);
      readData(1, conn_if2in_PT.getValue(), conn_PT);
      break;
    }
    default: break;
  }
}

void FORTE_E_TON::writeOutputData(const TEventID paEIID) {
  switch (paEIID) {
    case scmEventCNFID: {
      writeData(0, fb_E_RS->conn_Q.getValue(), conn_Q);
      break;
    }
    default: break;
  }
}

CIEC_ANY *FORTE_E_TON::getDI(const size_t paIndex) {
  switch (paIndex) {
    case 0: return &conn_if2in_IN.getValue();
    case 1: return &conn_if2in_PT.getValue();
  }
  return nullptr;
}

CIEC_ANY *FORTE_E_TON::getDO(const size_t paIndex) {
  switch (paIndex) {
    case 0: return &fb_E_RS->conn_Q.getValue();
  }
  return nullptr;
}

CEventConnection *FORTE_E_TON::getEOConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_E_TON::getDIConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_IN;
    case 1: return &conn_PT;
  }
  return nullptr;
}

CDataConnection *FORTE_E_TON::getDOConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_Q;
  }
  return nullptr;
}

CDataConnection *FORTE_E_TON::getIf2InConUnchecked(TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_if2in_IN;
    case 1: return &conn_if2in_PT;
  }
  return nullptr;
}
