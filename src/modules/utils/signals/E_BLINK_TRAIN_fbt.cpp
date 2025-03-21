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
 ***     1.0: 2025-02-15/Franz Höpfinger - HR Agrartechnik GmbH   - initial API and implementation and/or initial documentation
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

const CStringDictionary::TStringId FORTE_E_BLINK_TRAIN::scmDataInputNames[] = {STRID(TIMELOW), STRID(TIMEHIGH), STRID(N)};
const CStringDictionary::TStringId FORTE_E_BLINK_TRAIN::scmDataInputTypeIds[] = {STRID(TIME), STRID(TIME), STRID(UINT)};
const CStringDictionary::TStringId FORTE_E_BLINK_TRAIN::scmDataOutputNames[] = {STRID(OUT)};
const CStringDictionary::TStringId FORTE_E_BLINK_TRAIN::scmDataOutputTypeIds[] = {STRID(BOOL)};
const TDataIOID FORTE_E_BLINK_TRAIN::scmEIWith[] = {0, 1, 2, scmWithListDelimiter};
const TForteInt16 FORTE_E_BLINK_TRAIN::scmEIWithIndexes[] = {0, -1};
const CStringDictionary::TStringId FORTE_E_BLINK_TRAIN::scmEventInputNames[] = {STRID(START), STRID(STOP)};
const TDataIOID FORTE_E_BLINK_TRAIN::scmEOWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_E_BLINK_TRAIN::scmEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_E_BLINK_TRAIN::scmEventOutputNames[] = {STRID(CNF)};
const SFBInterfaceSpec FORTE_E_BLINK_TRAIN::scmFBInterfaceSpec = {
  2, scmEventInputNames, nullptr, scmEIWith, scmEIWithIndexes,
  1, scmEventOutputNames, nullptr, scmEOWith, scmEOWithIndexes,
  3, scmDataInputNames, scmDataInputTypeIds,
  1, scmDataOutputNames, scmDataOutputTypeIds,
  0, nullptr,
  0, nullptr
};

FORTE_E_BLINK_TRAIN::FORTE_E_BLINK_TRAIN(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    CCompositeFB(paContainer, scmFBInterfaceSpec, paInstanceNameId, scmFBNData),
    fb_E_TP(STRID(E_TP), *this),
    fb_E_TRAIN(STRID(E_TRAIN), *this),
    fb_ADD_2(STRID(ADD_2), "ADD_2", *this),
    var_TIMELOW(0_TIME),
    var_TIMEHIGH(0_TIME),
    var_N(0_UINT),
    var_OUT(0_BOOL),
    conn_CNF(this, 0),
    conn_TIMELOW(nullptr),
    conn_TIMEHIGH(nullptr),
    conn_N(nullptr),
    conn_OUT(this, 0, var_OUT) {
};

void FORTE_E_BLINK_TRAIN::setInitialValues() {
  var_TIMELOW = 0_TIME;
  var_TIMEHIGH = 0_TIME;
  var_N = 0_UINT;
  var_OUT = 0_BOOL;
}

const SCFB_FBInstanceData FORTE_E_BLINK_TRAIN::scmInternalFBs[] = {
  {STRID(E_TP), STRID(E_PULSE)},
  {STRID(E_TRAIN), STRID(E_TRAIN)},
  {STRID(ADD_2), STRID(ADD_2)}
};

const SCFB_FBConnectionData FORTE_E_BLINK_TRAIN::scmEventConnections[] = {
  {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_TRAIN), STRID(EO)), 1, GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_TP), STRID(REQ)), 0},
  {GENERATE_CONNECTION_PORT_ID_1_ARG(STRID(START)), -1, GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(ADD_2), STRID(REQ)), 2},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(ADD_2), STRID(CNF)), 2, GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_TRAIN), STRID(START)), 1},
  {GENERATE_CONNECTION_PORT_ID_1_ARG(STRID(STOP)), -1, GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_TRAIN), STRID(STOP)), 1},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_TP), STRID(CNF)), 0, GENERATE_CONNECTION_PORT_ID_1_ARG(STRID(CNF)), -1},
};

const SCFB_FBFannedOutConnectionData FORTE_E_BLINK_TRAIN::scmFannedOutEventConnections[] = {
  {3, GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_TP), STRID(R)), 0},
};

const SCFB_FBConnectionData FORTE_E_BLINK_TRAIN::scmDataConnections[] = {
  {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(ADD_2), STRID(OUT)), 2, GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_TRAIN), STRID(DT)), 1},
  {GENERATE_CONNECTION_PORT_ID_1_ARG(STRID(TIMELOW)), -1, GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(ADD_2), STRID(IN1)), 2},
  {GENERATE_CONNECTION_PORT_ID_1_ARG(STRID(TIMEHIGH)), -1, GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(ADD_2), STRID(IN2)), 2},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_TP), STRID(Q)), 0, GENERATE_CONNECTION_PORT_ID_1_ARG(STRID(OUT)), -1},
  {GENERATE_CONNECTION_PORT_ID_1_ARG(STRID(N)), -1, GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_TRAIN), STRID(N)), 1},
};

const SCFB_FBFannedOutConnectionData FORTE_E_BLINK_TRAIN::scmFannedOutDataConnections[] = {
  {2, GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_TP), STRID(PT)), 0},
};

const SCFB_FBNData FORTE_E_BLINK_TRAIN::scmFBNData = {
  3, scmInternalFBs,
  5, scmEventConnections,
  1, scmFannedOutEventConnections,
  5, scmDataConnections,
  1, scmFannedOutDataConnections,
  0, nullptr
};

void FORTE_E_BLINK_TRAIN::readInternal2InterfaceOutputData(const TEventID paEOID) {
  switch(paEOID) {
    case scmEventCNFID: {
      if(CDataConnection *conn = getIn2IfConUnchecked(0); conn) { conn->readData(var_OUT); }
      break;
    }
    default:
      break;
  }
}
void FORTE_E_BLINK_TRAIN::readInputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventSTARTID: {
      readData(0, var_TIMELOW, conn_TIMELOW);
      readData(1, var_TIMEHIGH, conn_TIMEHIGH);
      readData(2, var_N, conn_N);
      break;
    }
    default:
      break;
  }
}

void FORTE_E_BLINK_TRAIN::writeOutputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventCNFID: {
      writeData(0, var_OUT, conn_OUT);
      break;
    }
    default:
      break;
  }
}

CIEC_ANY *FORTE_E_BLINK_TRAIN::getDI(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_TIMELOW;
    case 1: return &var_TIMEHIGH;
    case 2: return &var_N;
  }
  return nullptr;
}

CIEC_ANY *FORTE_E_BLINK_TRAIN::getDO(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_OUT;
  }
  return nullptr;
}

CEventConnection *FORTE_E_BLINK_TRAIN::getEOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_E_BLINK_TRAIN::getDIConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_TIMELOW;
    case 1: return &conn_TIMEHIGH;
    case 2: return &conn_N;
  }
  return nullptr;
}

CDataConnection *FORTE_E_BLINK_TRAIN::getDOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_OUT;
  }
  return nullptr;
}
