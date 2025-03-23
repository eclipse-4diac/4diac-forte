/*******************************************************************************
 * Copyright (c) 2017, 2025 fortiss GmbH, HR Agrartechnik GmbH
 *
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
 *** Name: E_TRAIN
 *** Description: Generate of a finite train of events seperated by the time DT
 *** Version:
 ***     1.0: 2017-09-22/Alois Zoitl - fortiss GmbH - initial API and implementation and/or initial documentation
 ***     1.1: 2025-02-15/Franz Höpfinger -  HR Agrartechnik GmbH   - Bugfix
 *************************************************************************/

#include "E_TRAIN_fbt.h"

USE_STRING_ID(CTR);
USE_STRING_ID(CU);
USE_STRING_ID(CUO);
USE_STRING_ID(CV);
USE_STRING_ID(DLY);
USE_STRING_ID(DT);
USE_STRING_ID(E_CTU);
USE_STRING_ID(E_DELAY);
USE_STRING_ID(EI);
USE_STRING_ID(EO);
USE_STRING_ID(EO0);
USE_STRING_ID(E_SWITCH);
USE_STRING_ID(E_TRAIN);
USE_STRING_ID(G);
USE_STRING_ID(GATE);
USE_STRING_ID(N);
USE_STRING_ID(PV);
USE_STRING_ID(Q);
USE_STRING_ID(R);
USE_STRING_ID(RO);
USE_STRING_ID(START);
USE_STRING_ID(STOP);
USE_STRING_ID(TIME);
USE_STRING_ID(UINT);


#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"

DEFINE_FIRMWARE_FB(FORTE_E_TRAIN, STRID(E_TRAIN))

const CStringDictionary::TStringId FORTE_E_TRAIN::scmDataInputNames[] = {STRID(DT), STRID(N)};
const CStringDictionary::TStringId FORTE_E_TRAIN::scmDataInputTypeIds[] = {STRID(TIME), STRID(UINT)};
const CStringDictionary::TStringId FORTE_E_TRAIN::scmDataOutputNames[] = {STRID(CV)};
const CStringDictionary::TStringId FORTE_E_TRAIN::scmDataOutputTypeIds[] = {STRID(UINT)};
const TDataIOID FORTE_E_TRAIN::scmEIWith[] = {0, 1, scmWithListDelimiter};
const TForteInt16 FORTE_E_TRAIN::scmEIWithIndexes[] = {0, -1};
const CStringDictionary::TStringId FORTE_E_TRAIN::scmEventInputNames[] = {STRID(START), STRID(STOP)};
const TDataIOID FORTE_E_TRAIN::scmEOWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_E_TRAIN::scmEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_E_TRAIN::scmEventOutputNames[] = {STRID(EO)};
const SFBInterfaceSpec FORTE_E_TRAIN::scmFBInterfaceSpec = {
  2, scmEventInputNames, nullptr, scmEIWith, scmEIWithIndexes,
  1, scmEventOutputNames, nullptr, scmEOWith, scmEOWithIndexes,
  2, scmDataInputNames, scmDataInputTypeIds,
  1, scmDataOutputNames, scmDataOutputTypeIds,
  0, nullptr,
  0, nullptr
};

FORTE_E_TRAIN::FORTE_E_TRAIN(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    CCompositeFB(paContainer, scmFBInterfaceSpec, paInstanceNameId, scmFBNData),
    fb_CTR(STRID(CTR), *this),
    fb_GATE(STRID(GATE), *this),
    fb_DLY(STRID(DLY), *this),
    conn_EO(this, 0),
    conn_DT(nullptr),
    conn_N(nullptr),
    conn_CV(this, 0, 0_UINT),
    conn_if2in_DT(this, 0, 0_TIME),
    conn_if2in_N(this, 1, 0_UINT) {
};

void FORTE_E_TRAIN::setInitialValues() {
  conn_if2in_DT.getValue() = 0_TIME;
  conn_if2in_N.getValue() = 0_UINT;
  fb_CTR->conn_CV.getValue() = 0_UINT;
}

const SCFB_FBInstanceData FORTE_E_TRAIN::scmInternalFBs[] = {
  {STRID(CTR), STRID(E_CTU)},
  {STRID(GATE), STRID(E_SWITCH)},
  {STRID(DLY), STRID(E_DELAY)}
};

const SCFB_FBConnectionData FORTE_E_TRAIN::scmEventConnections[] = {
  {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(CTR), STRID(CUO)), 0, GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(GATE), STRID(EI)), 1},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(CTR), STRID(RO)), 0, GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(GATE), STRID(EI)), 1},
  {GENERATE_CONNECTION_PORT_ID_1_ARG(STRID(START)), -1, GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(CTR), STRID(R)), 0},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(GATE), STRID(EO0)), 1, GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(DLY), STRID(START)), 2},
  {GENERATE_CONNECTION_PORT_ID_1_ARG(STRID(STOP)), -1, GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(DLY), STRID(STOP)), 2},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(DLY), STRID(EO)), 2, GENERATE_CONNECTION_PORT_ID_1_ARG(STRID(EO)), -1},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(DLY), STRID(EO)), 2, GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(CTR), STRID(CU)), 0}
};

const SCFB_FBConnectionData FORTE_E_TRAIN::scmDataConnections[] = {
  {GENERATE_CONNECTION_PORT_ID_1_ARG(STRID(N)), -1, GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(CTR), STRID(PV)), 0},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(CTR), STRID(CV)), 0, GENERATE_CONNECTION_PORT_ID_1_ARG(STRID(CV)), -1},
  {GENERATE_CONNECTION_PORT_ID_1_ARG(STRID(DT)), -1, GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(DLY), STRID(DT)), 2},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(CTR), STRID(Q)), 0, GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(GATE), STRID(G)), 1},
};

const SCFB_FBNData FORTE_E_TRAIN::scmFBNData = {
  3, scmInternalFBs,
  7, scmEventConnections,
  4, scmDataConnections,
  0, nullptr,
  0, nullptr,
};

void FORTE_E_TRAIN::readInputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventSTARTID: {
      readData(0, conn_if2in_DT.getValue(), conn_DT);
      readData(1, conn_if2in_N.getValue(), conn_N);
      break;
    }
    default:
      break;
  }
}

void FORTE_E_TRAIN::writeOutputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventEOID: {
      writeData(0, fb_CTR->conn_CV.getValue(), conn_CV);
      break;
    }
    default:
      break;
  }
}

CIEC_ANY *FORTE_E_TRAIN::getDI(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &conn_if2in_DT.getValue();
    case 1: return &conn_if2in_N.getValue();
  }
  return nullptr;
}

CIEC_ANY *FORTE_E_TRAIN::getDO(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &fb_CTR->conn_CV.getValue();
  }
  return nullptr;
}

CEventConnection *FORTE_E_TRAIN::getEOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_EO;
  }
  return nullptr;
}

CDataConnection **FORTE_E_TRAIN::getDIConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_DT;
    case 1: return &conn_N;
  }
  return nullptr;
}

CDataConnection *FORTE_E_TRAIN::getDOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_CV;
  }
  return nullptr;
}

CDataConnection *FORTE_E_TRAIN::getIf2InConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_if2in_DT;
    case 1: return &conn_if2in_N;
  }
  return nullptr;
}
