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
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "E_N_TABLE_fbt_gen.cpp"
#endif

#include "forte_uint.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"

DEFINE_FIRMWARE_FB(FORTE_E_N_TABLE, g_nStringIdE_N_TABLE)

const CStringDictionary::TStringId FORTE_E_N_TABLE::scmDataInputNames[] = {g_nStringIdDT, g_nStringIdN};
const CStringDictionary::TStringId FORTE_E_N_TABLE::scmDataInputTypeIds[] = {g_nStringIdARRAY, static_cast<CStringDictionary::TStringId>(0), static_cast<CStringDictionary::TStringId>(3), g_nStringIdTIME, g_nStringIdUINT};
const TDataIOID FORTE_E_N_TABLE::scmEIWith[] = {0, 1, scmWithListDelimiter};
const TForteInt16 FORTE_E_N_TABLE::scmEIWithIndexes[] = {0, -1};
const CStringDictionary::TStringId FORTE_E_N_TABLE::scmEventInputNames[] = {g_nStringIdSTART, g_nStringIdSTOP};
const TForteInt16 FORTE_E_N_TABLE::scmEOWithIndexes[] = {-1, -1, -1, -1};
const CStringDictionary::TStringId FORTE_E_N_TABLE::scmEventOutputNames[] = {g_nStringIdEO0, g_nStringIdEO1, g_nStringIdEO2, g_nStringIdEO3};
const SFBInterfaceSpec FORTE_E_N_TABLE::scmFBInterfaceSpec = {
  2, scmEventInputNames, nullptr, scmEIWith, scmEIWithIndexes,
  4, scmEventOutputNames, nullptr, nullptr, scmEOWithIndexes,
  2, scmDataInputNames, scmDataInputTypeIds,
  0, nullptr, nullptr,
  0, nullptr,
  0, nullptr
};

FORTE_E_N_TABLE::FORTE_E_N_TABLE(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    CCompositeFB(paContainer, scmFBInterfaceSpec, paInstanceNameId, scmFBNData),
    fb_E_TABLE(g_nStringIdE_TABLE, *this),
    fb_E_DEMUX(g_nStringIdE_DEMUX, *this),
    fb_F_SUB(g_nStringIdF_SUB, *this),
    var_DT(CIEC_ARRAY_FIXED<CIEC_TIME, 0, 3>{}),
    var_N(0_UINT),
    conn_EO0(this, 0),
    conn_EO1(this, 1),
    conn_EO2(this, 2),
    conn_EO3(this, 3),
    conn_DT(nullptr),
    conn_N(nullptr) {
};

void FORTE_E_N_TABLE::setInitialValues() {
  var_DT = CIEC_ARRAY_FIXED<CIEC_TIME, 0, 3>{};
  var_N = 0_UINT;
}

void FORTE_E_N_TABLE::setFBNetworkInitialValues() {
  fb_F_SUB->var_IN2 = 1_UINT;
}

const SCFB_FBInstanceData FORTE_E_N_TABLE::scmInternalFBs[] = {
  {g_nStringIdE_TABLE, g_nStringIdE_TABLE},
  {g_nStringIdE_DEMUX, g_nStringIdE_DEMUX},
  {g_nStringIdF_SUB, g_nStringIdF_SUB}
};

const SCFB_FBConnectionData FORTE_E_N_TABLE::scmEventConnections[] = {
  {GENERATE_CONNECTION_PORT_ID_1_ARG(g_nStringIdSTART), -1, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdE_TABLE, g_nStringIdSTART), 0},
  {GENERATE_CONNECTION_PORT_ID_1_ARG(g_nStringIdSTOP), -1, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdE_TABLE, g_nStringIdSTOP), 0},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdE_TABLE, g_nStringIdEO), 0, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdF_SUB, g_nStringIdREQ), 2},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdF_SUB, g_nStringIdCNF), 2, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdE_DEMUX, g_nStringIdEI), 1},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdE_DEMUX, g_nStringIdEO0), 1, GENERATE_CONNECTION_PORT_ID_1_ARG(g_nStringIdEO0), -1},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdE_DEMUX, g_nStringIdEO1), 1, GENERATE_CONNECTION_PORT_ID_1_ARG(g_nStringIdEO1), -1},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdE_DEMUX, g_nStringIdEO2), 1, GENERATE_CONNECTION_PORT_ID_1_ARG(g_nStringIdEO2), -1},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdE_DEMUX, g_nStringIdEO3), 1, GENERATE_CONNECTION_PORT_ID_1_ARG(g_nStringIdEO3), -1},
};

const SCFB_FBConnectionData FORTE_E_N_TABLE::scmDataConnections[] = {
  {GENERATE_CONNECTION_PORT_ID_1_ARG(g_nStringIdDT), -1, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdE_TABLE, g_nStringIdDT), 0},
  {GENERATE_CONNECTION_PORT_ID_1_ARG(g_nStringIdN), -1, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdE_TABLE, g_nStringIdN), 0},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdE_TABLE, g_nStringIdCV), 0, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdF_SUB, g_nStringIdIN1), 2},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdF_SUB, g_nStringIdOUT), 2, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdE_DEMUX, g_nStringIdK), 1},
};

const SCFB_FBNData FORTE_E_N_TABLE::scmFBNData = {
  3, scmInternalFBs,
  8, scmEventConnections,
  0, nullptr,
  4, scmDataConnections,
  0, nullptr,
  0, nullptr
};

void FORTE_E_N_TABLE::readInternal2InterfaceOutputData(TEventID) {
  // nothing to do
}
void FORTE_E_N_TABLE::readInputData(const TEventID paEIID) {
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

void FORTE_E_N_TABLE::writeOutputData(TEventID) {
  // nothing to do
}

CIEC_ANY *FORTE_E_N_TABLE::getDI(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_DT;
    case 1: return &var_N;
  }
  return nullptr;
}

CIEC_ANY *FORTE_E_N_TABLE::getDO(size_t) {
  return nullptr;
}

CEventConnection *FORTE_E_N_TABLE::getEOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_EO0;
    case 1: return &conn_EO1;
    case 2: return &conn_EO2;
    case 3: return &conn_EO3;
  }
  return nullptr;
}

CDataConnection **FORTE_E_N_TABLE::getDIConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_DT;
    case 1: return &conn_N;
  }
  return nullptr;
}

CDataConnection *FORTE_E_N_TABLE::getDOConUnchecked(TPortId) {
  return nullptr;
}
