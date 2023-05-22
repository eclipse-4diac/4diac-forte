/*******************************************************************************
 * Copyright (c) 2006 - 2014 ACIN, Profactor GmbH, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Gerhard Ebenhofer
 *    - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "RT_E_TRAIN.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "RT_E_TRAIN_gen.cpp"
#endif

#include "criticalregion.h"
#include "resource.h"

DEFINE_FIRMWARE_FB(FORTE_RT_E_TRAIN, g_nStringIdRT_E_TRAIN)

const CStringDictionary::TStringId FORTE_RT_E_TRAIN::scm_anDataInputNames[] = {g_nStringIdDT, g_nStringIdN, g_nStringIdDeadline, g_nStringIdWCET};

const CStringDictionary::TStringId FORTE_RT_E_TRAIN::scm_anDataInputTypeIds[] = {g_nStringIdTIME, g_nStringIdUINT, g_nStringIdTIME, g_nStringIdTIME};

const CStringDictionary::TStringId FORTE_RT_E_TRAIN::scm_anDataOutputNames[] = {g_nStringIdCV};

const CStringDictionary::TStringId FORTE_RT_E_TRAIN::scm_anDataOutputTypeIds[] = {g_nStringIdUINT};

const TDataIOID FORTE_RT_E_TRAIN::scm_anEIWith[] = {0, 1, 2, 3, scmWithListDelimiter};
const TForteInt16 FORTE_RT_E_TRAIN::scm_anEIWithIndexes[] = {0, -1};
const CStringDictionary::TStringId FORTE_RT_E_TRAIN::scm_anEventInputNames[] = {g_nStringIdSTART, g_nStringIdSTOP};

const TDataIOID FORTE_RT_E_TRAIN::scm_anEOWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_RT_E_TRAIN::scm_anEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_RT_E_TRAIN::scm_anEventOutputNames[] = {g_nStringIdEO};


const SFBInterfaceSpec FORTE_RT_E_TRAIN::scm_stFBInterfaceSpec = {
  2, scm_anEventInputNames, scm_anEIWith, scm_anEIWithIndexes,
  1, scm_anEventOutputNames, scm_anEOWith, scm_anEOWithIndexes,
  4, scm_anDataInputNames, scm_anDataInputTypeIds,
  1, scm_anDataOutputNames, scm_anDataOutputTypeIds,
  0, nullptr
};

FORTE_RT_E_TRAIN::FORTE_RT_E_TRAIN(const CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes) :
    CCompositeFB(pa_poSrcRes, &scm_stFBInterfaceSpec, pa_nInstanceNameId, &scm_stFBNData),
    var_DT(CIEC_TIME(0)),
    var_N(CIEC_UINT(0)),
    var_Deadline(CIEC_TIME(0)),
    var_WCET(CIEC_TIME(0)),
    var_CV(CIEC_UINT(0)),
    var_conn_CV(var_CV),
    conn_EO(this, 0),
    conn_DT(nullptr),
    conn_N(nullptr),
    conn_Deadline(nullptr),
    conn_WCET(nullptr),
    conn_CV(this, 0, &var_conn_CV) {
};

const SCFB_FBInstanceData FORTE_RT_E_TRAIN::scm_astInternalFBs[] = {
  {g_nStringIdRT_E_CYCLE, g_nStringIdRT_E_CYCLE},
  {g_nStringIdE_CTU, g_nStringIdE_CTU},
  {g_nStringIdE_SWITCH, g_nStringIdE_SWITCH}
};


const SCFB_FBConnectionData FORTE_RT_E_TRAIN::scm_astEventConnections[] = {
  {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdRT_E_CYCLE, g_nStringIdEO), 0, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdE_CTU, g_nStringIdCU), 1},
  {GENERATE_CONNECTION_PORT_ID_1_ARG(g_nStringIdSTART), -1, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdE_CTU, g_nStringIdR), 1},
  {GENERATE_CONNECTION_PORT_ID_1_ARG(g_nStringIdSTOP), -1, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdRT_E_CYCLE, g_nStringIdSTOP), 0},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdE_CTU, g_nStringIdRO), 1, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdRT_E_CYCLE, g_nStringIdSTART), 0},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdE_CTU, g_nStringIdCUO), 1, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdE_SWITCH, g_nStringIdEI), 2},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdE_SWITCH, g_nStringIdEO0), 2, GENERATE_CONNECTION_PORT_ID_1_ARG(g_nStringIdEO), -1},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdE_SWITCH, g_nStringIdEO1), 2, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdRT_E_CYCLE, g_nStringIdSTOP), 0},
};

const SCFB_FBConnectionData FORTE_RT_E_TRAIN::scm_astDataConnections[] = {
  {GENERATE_CONNECTION_PORT_ID_1_ARG(g_nStringIdDT), -1, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdRT_E_CYCLE, g_nStringIdDT), 0},
  {GENERATE_CONNECTION_PORT_ID_1_ARG(g_nStringIdDeadline), -1, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdRT_E_CYCLE, g_nStringIdDeadline), 0},
  {GENERATE_CONNECTION_PORT_ID_1_ARG(g_nStringIdWCET), -1, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdRT_E_CYCLE, g_nStringIdWCET), 0},
  {GENERATE_CONNECTION_PORT_ID_1_ARG(g_nStringIdN), -1, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdE_CTU, g_nStringIdPV), 1},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdE_CTU, g_nStringIdCV), 1, GENERATE_CONNECTION_PORT_ID_1_ARG(g_nStringIdCV), -1},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdE_CTU, g_nStringIdQ), 1, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdE_SWITCH, g_nStringIdG), 2},
};

const SCFB_FBNData FORTE_RT_E_TRAIN::scm_stFBNData = {
  3, scm_astInternalFBs,
  7, scm_astEventConnections,
  0, nullptr,
  6, scm_astDataConnections,
  0, nullptr,
  0, nullptr
};


void FORTE_RT_E_TRAIN::readInputData(TEventID pa_nEIID) {
  switch(pa_nEIID) {
    case scm_nEventSTARTID: {
      RES_DATA_CON_CRITICAL_REGION();
      readData(0, &var_DT, conn_DT);
      readData(1, &var_N, conn_N);
      readData(2, &var_Deadline, conn_Deadline);
      readData(3, &var_WCET, conn_WCET);
      break;
    }
    case scm_nEventSTOPID: {
      RES_DATA_CON_CRITICAL_REGION();
      break;
    }
    default:
      break;
  }
}

void FORTE_RT_E_TRAIN::writeOutputData(TEventID pa_nEIID) {
  switch(pa_nEIID) {
    case scm_nEventEOID: {
      RES_DATA_CON_CRITICAL_REGION();
      writeData(0, &var_CV, &conn_CV);
      break;
    }
    default:
      break;
  }
}

CIEC_ANY *FORTE_RT_E_TRAIN::getDI(size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_DT;
    case 1: return &var_N;
    case 2: return &var_Deadline;
    case 3: return &var_WCET;
  }
  return nullptr;
}

CIEC_ANY *FORTE_RT_E_TRAIN::getDO(size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_CV;
  }
  return nullptr;
}

CEventConnection *FORTE_RT_E_TRAIN::getEOConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_EO;
  }
  return nullptr;
}

CDataConnection **FORTE_RT_E_TRAIN::getDIConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_DT;
    case 1: return &conn_N;
    case 2: return &conn_Deadline;
    case 3: return &conn_WCET;
  }
  return nullptr;
}

CDataConnection *FORTE_RT_E_TRAIN::getDOConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_CV;
  }
  return nullptr;
}


