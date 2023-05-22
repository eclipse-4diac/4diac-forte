/*******************************************************************************
 * Copyright (c) 2014 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "E_RTimeOut.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "E_RTimeOut_gen.cpp"
#endif

#include "criticalregion.h"
#include "resource.h"

DEFINE_FIRMWARE_FB(FORTE_E_RTimeOut, g_nStringIdE_RTimeOut)

const SAdapterInstanceDef FORTE_E_RTimeOut::scm_astAdapterInstances[] = {
  {g_nStringIdARTimeOut, g_nStringIdTimeOutSocket, false}
};
const SFBInterfaceSpec FORTE_E_RTimeOut::scm_stFBInterfaceSpec = {
  0, nullptr, nullptr, nullptr,
  0, nullptr, nullptr, nullptr,
  0, nullptr, nullptr,
  0, nullptr, nullptr,
  1, scm_astAdapterInstances
};

FORTE_E_RTimeOut::FORTE_E_RTimeOut(const CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes) :
    CCompositeFB(pa_poSrcRes, &scm_stFBInterfaceSpec, pa_nInstanceNameId, &scm_stFBNData) {
};

const SCFB_FBInstanceData FORTE_E_RTimeOut::scm_astInternalFBs[] = {
  {g_nStringIdDLY, g_nStringIdE_RDELAY}
};


const SCFB_FBConnectionData FORTE_E_RTimeOut::scm_astEventConnections[] = {
  {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdTimeOutSocket, g_nStringIdSTART), CCompositeFB::scm_nAdapterMarker | 0, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdDLY, g_nStringIdSTART), 0},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdTimeOutSocket, g_nStringIdSTOP), CCompositeFB::scm_nAdapterMarker | 0, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdDLY, g_nStringIdSTOP), 0},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdDLY, g_nStringIdEO), 0, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdTimeOutSocket, g_nStringIdTimeOut), CCompositeFB::scm_nAdapterMarker | 0},
};

const SCFB_FBConnectionData FORTE_E_RTimeOut::scm_astDataConnections[] = {
  {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdTimeOutSocket, g_nStringIdDT), CCompositeFB::scm_nAdapterMarker | 0, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdDLY, g_nStringIdDT), 0},
};

const SCFB_FBNData FORTE_E_RTimeOut::scm_stFBNData = {
  1, scm_astInternalFBs,
  3, scm_astEventConnections,
  0, nullptr,
  1, scm_astDataConnections,
  0, nullptr,
  0, nullptr
};


void FORTE_E_RTimeOut::readInputData(TEventID pa_nEIID) {
}

void FORTE_E_RTimeOut::writeOutputData(TEventID pa_nEIID) {
}

CIEC_ANY *FORTE_E_RTimeOut::getDI(size_t) {
  return nullptr;
}

CIEC_ANY *FORTE_E_RTimeOut::getDO(size_t) {
  return nullptr;
}

CEventConnection *FORTE_E_RTimeOut::getEOConUnchecked(TPortId) {
  return nullptr;
}

CDataConnection **FORTE_E_RTimeOut::getDIConUnchecked(TPortId) {
  return nullptr;
}

CDataConnection *FORTE_E_RTimeOut::getDOConUnchecked(TPortId) {
  return nullptr;
}


