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

#include "E_RTimeOut_fbt.h"

USE_STRING_ID(ARTimeOut);
USE_STRING_ID(DLY);
USE_STRING_ID(DT);
USE_STRING_ID(EO);
USE_STRING_ID(E_RDELAY);
USE_STRING_ID(E_RTimeOut);
USE_STRING_ID(START);
USE_STRING_ID(STOP);
USE_STRING_ID(TimeOut);
USE_STRING_ID(TimeOutSocket);


DEFINE_FIRMWARE_FB(FORTE_E_RTimeOut, STRID(E_RTimeOut))

const SAdapterInstanceDef FORTE_E_RTimeOut::scmAdapterInstances[] = {{STRID(ARTimeOut), STRID(TimeOutSocket), false}};
const SFBInterfaceSpec FORTE_E_RTimeOut::scmFBInterfaceSpec = {
    0, nullptr, nullptr, nullptr, nullptr, 0,       nullptr, nullptr, nullptr, nullptr,
    0, nullptr, nullptr, 0,       nullptr, nullptr, 0,       nullptr, 1,       scmAdapterInstances};

FORTE_E_RTimeOut::FORTE_E_RTimeOut(const CStringDictionary::TStringId paInstanceNameId,
                                   forte::core::CFBContainer &paContainer) :
    CCompositeFB(paContainer, scmFBInterfaceSpec, paInstanceNameId, scmFBNData),
    fb_DLY(STRID(DLY), *this),
    var_TimeOutSocket(STRID(TimeOutSocket), *this, false) {};

bool FORTE_E_RTimeOut::initialize() {
  if (!var_TimeOutSocket.initialize()) {
    return false;
  }
  var_TimeOutSocket.setParentFB(this, 0);
  return CCompositeFB::initialize();
}

const SCFB_FBInstanceData FORTE_E_RTimeOut::scmInternalFBs[] = {{STRID(DLY), STRID(E_RDELAY)}};

const SCFB_FBConnectionData FORTE_E_RTimeOut::scmEventConnections[] = {
    {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(TimeOutSocket), STRID(START)), CCompositeFB::scmAdapterMarker | 0,
     GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(DLY), STRID(START)), 0},
    {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(TimeOutSocket), STRID(STOP)), CCompositeFB::scmAdapterMarker | 0,
     GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(DLY), STRID(STOP)), 0},
    {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(DLY), STRID(EO)), 0,
     GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(TimeOutSocket), STRID(TimeOut)), CCompositeFB::scmAdapterMarker | 0},
};

const SCFB_FBConnectionData FORTE_E_RTimeOut::scmDataConnections[] = {
    {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(TimeOutSocket), STRID(DT)), CCompositeFB::scmAdapterMarker | 0,
     GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(DLY), STRID(DT)), 0},
};

const SCFB_FBNData FORTE_E_RTimeOut::scmFBNData = {
    1, scmInternalFBs, 3, scmEventConnections, 1, scmDataConnections, 0, nullptr, 0, nullptr};

void FORTE_E_RTimeOut::readInputData(TEventID) {
  // nothing to do
}

void FORTE_E_RTimeOut::writeOutputData(TEventID) {
  // nothing to do
}

CIEC_ANY *FORTE_E_RTimeOut::getDI(size_t) {
  return nullptr;
}

CIEC_ANY *FORTE_E_RTimeOut::getDO(size_t) {
  return nullptr;
}

CAdapter *FORTE_E_RTimeOut::getAdapterUnchecked(const size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_TimeOutSocket;
  }
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

CDataConnection *FORTE_E_RTimeOut::getIf2InConUnchecked(TPortId) {
  return nullptr;
}

void FORTE_E_RTimeOut::setInitialValues() {
  // not data pins so nothing todo
}
