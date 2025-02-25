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
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "E_TRAIN_fbt_gen.cpp"
#endif

#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"

DEFINE_FIRMWARE_FB(FORTE_E_TRAIN, g_nStringIdE_TRAIN)

const CStringDictionary::TStringId FORTE_E_TRAIN::scmDataInputNames[] = {g_nStringIdDT, g_nStringIdN};
const CStringDictionary::TStringId FORTE_E_TRAIN::scmDataInputTypeIds[] = {g_nStringIdTIME, g_nStringIdUINT};
const CStringDictionary::TStringId FORTE_E_TRAIN::scmDataOutputNames[] = {g_nStringIdCV};
const CStringDictionary::TStringId FORTE_E_TRAIN::scmDataOutputTypeIds[] = {g_nStringIdUINT};
const TDataIOID FORTE_E_TRAIN::scmEIWith[] = {0, 1, scmWithListDelimiter};
const TForteInt16 FORTE_E_TRAIN::scmEIWithIndexes[] = {0, -1};
const CStringDictionary::TStringId FORTE_E_TRAIN::scmEventInputNames[] = {g_nStringIdSTART, g_nStringIdSTOP};
const TDataIOID FORTE_E_TRAIN::scmEOWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_E_TRAIN::scmEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_E_TRAIN::scmEventOutputNames[] = {g_nStringIdEO};
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
    fb_CTR(g_nStringIdCTR, *this),
    fb_GATE(g_nStringIdGATE, *this),
    fb_DLY(g_nStringIdDLY, *this),
    var_DT(0_TIME),
    var_N(0_UINT),
    var_CV(0_UINT),
    var_conn_CV(var_CV),
    conn_EO(this, 0),
    conn_DT(nullptr),
    conn_N(nullptr),
    conn_CV(this, 0, &var_conn_CV) {
};

void FORTE_E_TRAIN::setInitialValues() {
  var_DT = 0_TIME;
  var_N = 0_UINT;
  var_CV = 0_UINT;
}

const SCFB_FBInstanceData FORTE_E_TRAIN::scmInternalFBs[] = {
  {g_nStringIdCTR, g_nStringIdE_CTU},
  {g_nStringIdGATE, g_nStringIdE_SWITCH},
  {g_nStringIdDLY, g_nStringIdE_DELAY}
};

const SCFB_FBConnectionData FORTE_E_TRAIN::scmEventConnections[] = {
  {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdCTR, g_nStringIdCUO), 0, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdGATE, g_nStringIdEI), 1},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdCTR, g_nStringIdRO), 0, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdGATE, g_nStringIdEI), 1},
  {GENERATE_CONNECTION_PORT_ID_1_ARG(g_nStringIdSTART), -1, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdCTR, g_nStringIdR), 0},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdGATE, g_nStringIdEO0), 1, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdDLY, g_nStringIdSTART), 2},
  {GENERATE_CONNECTION_PORT_ID_1_ARG(g_nStringIdSTOP), -1, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdDLY, g_nStringIdSTOP), 2},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdDLY, g_nStringIdEO), 2, GENERATE_CONNECTION_PORT_ID_1_ARG(g_nStringIdEO), -1},
};

const SCFB_FBFannedOutConnectionData FORTE_E_TRAIN::scmFannedOutEventConnections[] = {
  {5, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdCTR, g_nStringIdCU), 0},
};

const SCFB_FBConnectionData FORTE_E_TRAIN::scmDataConnections[] = {
  {GENERATE_CONNECTION_PORT_ID_1_ARG(g_nStringIdN), -1, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdCTR, g_nStringIdPV), 0},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdCTR, g_nStringIdCV), 0, GENERATE_CONNECTION_PORT_ID_1_ARG(g_nStringIdCV), -1},
  {GENERATE_CONNECTION_PORT_ID_1_ARG(g_nStringIdDT), -1, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdDLY, g_nStringIdDT), 2},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdCTR, g_nStringIdQ), 0, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdGATE, g_nStringIdG), 1},
};

const SCFB_FBNData FORTE_E_TRAIN::scmFBNData = {
  3, scmInternalFBs,
  6, scmEventConnections,
  1, scmFannedOutEventConnections,
  4, scmDataConnections,
  0, nullptr,
  0, nullptr,
  0, nullptr,
};

void FORTE_E_TRAIN::readInternal2InterfaceOutputData(const TEventID paEOID) {
  switch(paEOID) {
    case scmEventEOID: {
      if(CDataConnection *conn = getIn2IfConUnchecked(0); conn) { conn->readData(var_CV); }
      break;
    }
    default:
      break;
  }
}
void FORTE_E_TRAIN::readInputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventSTARTID: {
      readData(0, var_DT, conn_DT);
      readData(1, var_N, conn_N);
      break;
    }
    default:
      break;
  }
}

void FORTE_E_TRAIN::writeOutputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventEOID: {
      writeData(0, var_CV, conn_CV);
      break;
    }
    default:
      break;
  }
}

CIEC_ANY *FORTE_E_TRAIN::getDI(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_DT;
    case 1: return &var_N;
  }
  return nullptr;
}

CIEC_ANY *FORTE_E_TRAIN::getDO(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_CV;
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
