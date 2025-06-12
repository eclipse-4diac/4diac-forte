/*************************************************************************
 *** Copyright (c) 2025 HR Agrartechnik GmbH
 *** This program and the accompanying materials are made available under the
 *** terms of the Eclipse Public License 2.0 which is available at
 *** http://www.eclipse.org/legal/epl-2.0.
 ***
 *** SPDX-License-Identifier: EPL-2.0
 ***
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x NG!
 ***
 *** Name: E_BLINK_TRAIN
 *** Description: Simulates a blinking signal (turning on and off for specific durations), with a limited N
 *** Version:
 ***     1.0: 2025-02-15/Franz Höpfinger - HR Agrartechnik GmbH   - initial API and implementation and/or initial
 *documentation
 *************************************************************************/

#include "E_BLINK_TRAIN_fbt.h"

USE_STRING_ID(ADD_2);
USE_STRING_ID(BOOL);
USE_STRING_ID(CNF);
USE_STRING_ID(DT);
USE_STRING_ID(E_BLINK_TRAIN);
USE_STRING_ID(EO);
USE_STRING_ID(E_PULSE);
USE_STRING_ID(E_TP);
USE_STRING_ID(E_TRAIN);
USE_STRING_ID(IN1);
USE_STRING_ID(IN2);
USE_STRING_ID(N);
USE_STRING_ID(OUT);
USE_STRING_ID(PT);
USE_STRING_ID(Q);
USE_STRING_ID(R);
USE_STRING_ID(REQ);
USE_STRING_ID(START);
USE_STRING_ID(STOP);
USE_STRING_ID(TIME);
USE_STRING_ID(TIMEHIGH);
USE_STRING_ID(TIMELOW);
USE_STRING_ID(UINT);

#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"

DEFINE_FIRMWARE_FB(FORTE_E_BLINK_TRAIN, STRID(E_BLINK_TRAIN))

namespace {
  const auto cDataInputNames = std::array{STRID(TIMELOW), STRID(TIMEHIGH), STRID(N)};
  const auto cDataOutputNames = std::array{STRID(OUT)};
  const auto cEventInputNames = std::array{STRID(START), STRID(STOP)};
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

FORTE_E_BLINK_TRAIN::FORTE_E_BLINK_TRAIN(const CStringDictionary::TStringId paInstanceNameId,
                                         forte::core::CFBContainer &paContainer) :
    CCompositeFB(paContainer, cFBInterfaceSpec, paInstanceNameId, scmFBNData),
    fb_E_TP(STRID(E_TP), *this),
    fb_E_TRAIN(STRID(E_TRAIN), *this),
    fb_ADD_2(STRID(ADD_2), "ADD_2", *this),
    conn_CNF(*this, 0),
    conn_TIMELOW(nullptr),
    conn_TIMEHIGH(nullptr),
    conn_N(nullptr),
    conn_OUT(*this, 0, 0_BOOL),
    conn_if2in_TIMELOW(*this, 0, 0_TIME),
    conn_if2in_TIMEHIGH(*this, 1, 0_TIME),
    conn_if2in_N(*this, 2, 0_UINT) {};

void FORTE_E_BLINK_TRAIN::setInitialValues() {
  conn_if2in_TIMELOW.getValue() = 0_TIME;
  conn_if2in_TIMEHIGH.getValue() = 0_TIME;
  conn_if2in_N.getValue() = 0_UINT;
  fb_E_TP->conn_Q.getValue() = 0_BOOL;
}

const SCFB_FBInstanceData FORTE_E_BLINK_TRAIN::scmInternalFBs[] = {
    {STRID(E_TP), STRID(E_PULSE)}, {STRID(E_TRAIN), STRID(E_TRAIN)}, {STRID(ADD_2), STRID(ADD_2)}};

const SCFB_FBConnectionData FORTE_E_BLINK_TRAIN::scmEventConnections[] = {
    {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_TRAIN), STRID(EO)), 1,
     GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_TP), STRID(REQ)), 0},
    {GENERATE_CONNECTION_PORT_ID_1_ARG(STRID(START)), -1, GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(ADD_2), STRID(REQ)),
     2},
    {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(ADD_2), STRID(CNF)), 2,
     GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_TRAIN), STRID(START)), 1},
    {GENERATE_CONNECTION_PORT_ID_1_ARG(STRID(STOP)), -1, GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_TRAIN), STRID(STOP)),
     1},
    {GENERATE_CONNECTION_PORT_ID_1_ARG(STRID(STOP)), -1, GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_TP), STRID(R)), 0},
    {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_TP), STRID(CNF)), 0, GENERATE_CONNECTION_PORT_ID_1_ARG(STRID(CNF)), -1},
};

const SCFB_FBConnectionData FORTE_E_BLINK_TRAIN::scmDataConnections[] = {
    {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(ADD_2), STRID(OUT)), 2,
     GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_TRAIN), STRID(DT)), 1},
    {GENERATE_CONNECTION_PORT_ID_1_ARG(STRID(TIMELOW)), -1, GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(ADD_2), STRID(IN1)),
     2},
    {GENERATE_CONNECTION_PORT_ID_1_ARG(STRID(TIMEHIGH)), -1,
     GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(ADD_2), STRID(IN2)), 2},
    {GENERATE_CONNECTION_PORT_ID_1_ARG(STRID(TIMEHIGH)), -1, GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_TP), STRID(PT)),
     0},
    {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_TP), STRID(Q)), 0, GENERATE_CONNECTION_PORT_ID_1_ARG(STRID(OUT)), -1},
    {GENERATE_CONNECTION_PORT_ID_1_ARG(STRID(N)), -1, GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_TRAIN), STRID(N)), 1},
};

const SCFB_FBNData FORTE_E_BLINK_TRAIN::scmFBNData = {
    3, scmInternalFBs, 6, scmEventConnections, 5, scmDataConnections, 0, nullptr, 0, nullptr};

void FORTE_E_BLINK_TRAIN::readInputData(const TEventID paEIID) {
  switch (paEIID) {
    case scmEventSTARTID: {
      readData(0, conn_if2in_TIMELOW.getValue(), conn_TIMELOW);
      readData(1, conn_if2in_TIMEHIGH.getValue(), conn_TIMEHIGH);
      readData(2, conn_if2in_N.getValue(), conn_N);
      break;
    }
    default: break;
  }
}

void FORTE_E_BLINK_TRAIN::writeOutputData(const TEventID paEIID) {
  switch (paEIID) {
    case scmEventCNFID: {
      writeData(cFBInterfaceSpec.getNumDIs() + 0, fb_E_TP->conn_Q.getValue(), conn_OUT);
      break;
    }
    default: break;
  }
}

CIEC_ANY *FORTE_E_BLINK_TRAIN::getDI(const size_t paIndex) {
  switch (paIndex) {
    case 0: return &conn_if2in_TIMELOW.getValue();
    case 1: return &conn_if2in_TIMEHIGH.getValue();
    case 2: return &conn_if2in_N.getValue();
  }
  return nullptr;
}

CIEC_ANY *FORTE_E_BLINK_TRAIN::getDO(const size_t paIndex) {
  switch (paIndex) {
    case 0: return &fb_E_TP->conn_Q.getValue();
  }
  return nullptr;
}

CEventConnection *FORTE_E_BLINK_TRAIN::getEOConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_E_BLINK_TRAIN::getDIConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_TIMELOW;
    case 1: return &conn_TIMEHIGH;
    case 2: return &conn_N;
  }
  return nullptr;
}

CDataConnection *FORTE_E_BLINK_TRAIN::getDOConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_OUT;
  }
  return nullptr;
}

CDataConnection *FORTE_E_BLINK_TRAIN::getIf2InConUnchecked(TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_if2in_TIMELOW;
    case 1: return &conn_if2in_TIMEHIGH;
    case 2: return &conn_if2in_N;
  }
  return nullptr;
}
