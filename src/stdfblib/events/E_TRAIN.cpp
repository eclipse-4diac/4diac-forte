/*******************************************************************************
 * Copyright (c) 2017 fortiss GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl
 *     - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "E_TRAIN.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "E_TRAIN_gen.cpp"
#endif

#include "criticalregion.h"
#include "resource.h"

DEFINE_FIRMWARE_FB(FORTE_E_TRAIN, g_nStringIdE_TRAIN)

const CStringDictionary::TStringId FORTE_E_TRAIN::scm_anDataInputNames[] = {g_nStringIdDT, g_nStringIdN};
const CStringDictionary::TStringId FORTE_E_TRAIN::scm_anDataInputTypeIds[] = {g_nStringIdTIME, g_nStringIdUINT};
const CStringDictionary::TStringId FORTE_E_TRAIN::scm_anDataOutputNames[] = {g_nStringIdCV};
const CStringDictionary::TStringId FORTE_E_TRAIN::scm_anDataOutputTypeIds[] = {g_nStringIdUINT};
const TDataIOID FORTE_E_TRAIN::scm_anEIWith[] = {0, scmWithListDelimiter, 0, scmWithListDelimiter};
const TForteInt16 FORTE_E_TRAIN::scm_anEIWithIndexes[] = {0, 2};
const CStringDictionary::TStringId FORTE_E_TRAIN::scm_anEventInputNames[] = {g_nStringIdSTART, g_nStringIdSTOP};
const TDataIOID FORTE_E_TRAIN::scm_anEOWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_E_TRAIN::scm_anEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_E_TRAIN::scm_anEventOutputNames[] = {g_nStringIdEO};
const SFBInterfaceSpec FORTE_E_TRAIN::scm_stFBInterfaceSpec = {
  2, scm_anEventInputNames, scm_anEIWith, scm_anEIWithIndexes,
  1, scm_anEventOutputNames, scm_anEOWith, scm_anEOWithIndexes,
  2, scm_anDataInputNames, scm_anDataInputTypeIds,
  1, scm_anDataOutputNames, scm_anDataOutputTypeIds,
  0, nullptr,
  0, nullptr
};

FORTE_E_TRAIN::FORTE_E_TRAIN(const CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes) :
    CCompositeFB(pa_poSrcRes, &scm_stFBInterfaceSpec, pa_nInstanceNameId, &scm_stFBNData),
    var_DT(CIEC_TIME(0)),
    var_N(CIEC_UINT(0)),
    var_CV(CIEC_UINT(0)),
    var_conn_CV(var_CV),
    conn_EO(this, 0),
    conn_DT(nullptr),
    conn_N(nullptr),
    conn_CV(this, 0, &var_conn_CV) {
};

const SCFB_FBInstanceData FORTE_E_TRAIN::scm_astInternalFBs[] = {
  {g_nStringIdCTR, g_nStringIdE_CTU},
  {g_nStringIdGATE, g_nStringIdE_SWITCH},
  {g_nStringIdDLY, g_nStringIdE_DELAY}
};


const SCFB_FBConnectionData FORTE_E_TRAIN::scm_astEventConnections[] = {
  {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdCTR, g_nStringIdCUO), 0, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdGATE, g_nStringIdEI), 1},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdCTR, g_nStringIdRO), 0, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdGATE, g_nStringIdEI), 1},
  {GENERATE_CONNECTION_PORT_ID_1_ARG(g_nStringIdSTART), -1, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdCTR, g_nStringIdR), 0},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdGATE, g_nStringIdEO0), 1, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdDLY, g_nStringIdSTART), 2},
  {GENERATE_CONNECTION_PORT_ID_1_ARG(g_nStringIdSTOP), -1, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdDLY, g_nStringIdSTOP), 2},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdDLY, g_nStringIdEO), 2, GENERATE_CONNECTION_PORT_ID_1_ARG(g_nStringIdEO), -1},
};

const SCFB_FBFannedOutConnectionData FORTE_E_TRAIN::scm_astFannedOutEventConnections[] = {
  {5, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdCTR, g_nStringIdCU), 0},
};

const SCFB_FBConnectionData FORTE_E_TRAIN::scm_astDataConnections[] = {
  {GENERATE_CONNECTION_PORT_ID_1_ARG(g_nStringIdN), -1, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdCTR, g_nStringIdPV), 0},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdCTR, g_nStringIdCV), 0, GENERATE_CONNECTION_PORT_ID_1_ARG(g_nStringIdCV), -1},
  {GENERATE_CONNECTION_PORT_ID_1_ARG(g_nStringIdDT), -1, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdDLY, g_nStringIdDT), 2},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdCTR, g_nStringIdQ), 0, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdGATE, g_nStringIdG), 1},
};

const SCFB_FBNData FORTE_E_TRAIN::scm_stFBNData = {
  3, scm_astInternalFBs,
  6, scm_astEventConnections,
  1, scm_astFannedOutEventConnections,
  4, scm_astDataConnections,
  0, nullptr,
  0, nullptr
};


void FORTE_E_TRAIN::readInputData(TEventID pa_nEIID) {
  switch(pa_nEIID) {
    case scm_nEventSTARTID: {
      RES_DATA_CON_CRITICAL_REGION();
      readData(0, var_DT, conn_DT);
      break;
    }
    case scm_nEventSTOPID: {
      RES_DATA_CON_CRITICAL_REGION();
      readData(0, var_DT, conn_DT);
      break;
    }
    default:
      break;
  }
}

void FORTE_E_TRAIN::writeOutputData(TEventID pa_nEIID) {
  switch(pa_nEIID) {
    case scm_nEventEOID: {
      RES_DATA_CON_CRITICAL_REGION();
      writeData(0, var_CV, conn_CV);
      break;
    }
    default:
      break;
  }
}

CIEC_ANY *FORTE_E_TRAIN::getDI(size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_DT;
    case 1: return &var_N;
  }
  return nullptr;
}

CIEC_ANY *FORTE_E_TRAIN::getDO(size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_CV;
  }
  return nullptr;
}

CEventConnection *FORTE_E_TRAIN::getEOConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_EO;
  }
  return nullptr;
}

CDataConnection **FORTE_E_TRAIN::getDIConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_DT;
    case 1: return &conn_N;
  }
  return nullptr;
}

CDataConnection *FORTE_E_TRAIN::getDOConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_CV;
  }
  return nullptr;
}


