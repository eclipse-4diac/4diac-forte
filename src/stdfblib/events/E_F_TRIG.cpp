/*******************************************************************************
 * Copyright (c) 2007 - 2013 ACIN, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Ingo Hegny
 *     - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "E_F_TRIG.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "E_F_TRIG_gen.cpp"
#endif

#include "criticalregion.h"
#include "resource.h"

DEFINE_FIRMWARE_FB(FORTE_E_F_TRIG, g_nStringIdE_F_TRIG)

const CStringDictionary::TStringId FORTE_E_F_TRIG::scm_anDataInputNames[] = {g_nStringIdQI};
const CStringDictionary::TStringId FORTE_E_F_TRIG::scm_anDataInputTypeIds[] = {g_nStringIdBOOL};
const TDataIOID FORTE_E_F_TRIG::scm_anEIWith[] = {0, 255};
const TForteInt16 FORTE_E_F_TRIG::scm_anEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_E_F_TRIG::scm_anEventInputNames[] = {g_nStringIdEI};
const TForteInt16 FORTE_E_F_TRIG::scm_anEOWithIndexes[] = {-1};
const CStringDictionary::TStringId FORTE_E_F_TRIG::scm_anEventOutputNames[] = {g_nStringIdEO};
const SFBInterfaceSpec FORTE_E_F_TRIG::scm_stFBInterfaceSpec = {
  1, scm_anEventInputNames, scm_anEIWith, scm_anEIWithIndexes,
  1, scm_anEventOutputNames, nullptr, scm_anEOWithIndexes,
  1, scm_anDataInputNames, scm_anDataInputTypeIds,
  0, nullptr, nullptr,
  0, nullptr
};

FORTE_E_F_TRIG::FORTE_E_F_TRIG(const CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes) :
    CCompositeFB(pa_poSrcRes, &scm_stFBInterfaceSpec, pa_nInstanceNameId, &scm_stFBNData),
    var_QI(CIEC_BOOL(0)),
    conn_EO(this, 0),
    conn_QI(nullptr) {
};

const SCFB_FBInstanceData FORTE_E_F_TRIG::scm_astInternalFBs[] = {
  {g_nStringIdE_D_FF, g_nStringIdE_D_FF},
  {g_nStringIdE_SWITCH, g_nStringIdE_SWITCH}
};


const SCFB_FBConnectionData FORTE_E_F_TRIG::scm_astEventConnections[] = {
  {GENERATE_CONNECTION_PORT_ID_1_ARG(g_nStringIdEI), -1, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdE_D_FF, g_nStringIdCLK), 0},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdE_D_FF, g_nStringIdEO), 0, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdE_SWITCH, g_nStringIdEI), 1},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdE_SWITCH, g_nStringIdEO0), 1, GENERATE_CONNECTION_PORT_ID_1_ARG(g_nStringIdEO), -1},
};

const SCFB_FBConnectionData FORTE_E_F_TRIG::scm_astDataConnections[] = {
  {GENERATE_CONNECTION_PORT_ID_1_ARG(g_nStringIdQI), -1, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdE_D_FF, g_nStringIdD), 0},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdE_D_FF, g_nStringIdQ), 0, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdE_SWITCH, g_nStringIdG), 1},
};

const SCFB_FBNData FORTE_E_F_TRIG::scm_stFBNData = {
  2, scm_astInternalFBs,
  3, scm_astEventConnections,
  0, nullptr,
  2, scm_astDataConnections,
  0, nullptr,
  0, nullptr
};


void FORTE_E_F_TRIG::readInputData(size_t pa_nEIID) {
  switch(pa_nEIID) {
    case scm_nEventEIID: {
      CCriticalRegion criticalRegion(getResource().m_oResDataConSync);
      readData(0, &var_QI, conn_QI);
      break;
    }
    default:
      break;
  }
}

void FORTE_E_F_TRIG::writeOutputData(size_t pa_nEIID) {
}

CIEC_ANY *FORTE_E_F_TRIG::getDI(size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_QI;
  }
  return nullptr;
}

CIEC_ANY *FORTE_E_F_TRIG::getDO(size_t) {
  return nullptr;
}

CEventConnection *FORTE_E_F_TRIG::getEOConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_EO;
  }
  return nullptr;
}

CDataConnection **FORTE_E_F_TRIG::getDIConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_QI;
  }
  return nullptr;
}

CDataConnection *FORTE_E_F_TRIG::getDOConUnchecked(TPortId) {
  return nullptr;
}


