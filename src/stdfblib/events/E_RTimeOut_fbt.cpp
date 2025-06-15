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

#include "ARTimeOut_adp.h"
#include "core/iec61131_functions.h"
#include "core/datatypes/forte_array_common.h"
#include "core/datatypes/forte_array.h"
#include "core/datatypes/forte_array_fixed.h"
#include "core/datatypes/forte_array_variable.h"

using namespace std::literals;

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

namespace {
  const auto cSocketNameIds = std::array{STRID(TimeOutSocket)};

  const SFBInterfaceSpec cFBInterfaceSpec = {
      .mEINames = {},
      .mEITypeNames = {},
      .mEONames = {},
      .mEOTypeNames = {},
      .mDINames = {},
      .mDONames = {},
      .mDIONames = {},
      .mSocketNames = cSocketNameIds,
      .mPlugNames = {},
  };

  const auto cEventConnections = std::to_array<SCFB_FBConnectionData>({
      {STRID(TimeOutSocket), STRID(START), STRID(DLY), STRID(START)},
      {STRID(TimeOutSocket), STRID(STOP), STRID(DLY), STRID(STOP)},
      {STRID(DLY), STRID(EO), STRID(TimeOutSocket), STRID(TimeOut)},
  });

  const auto cDataConnections = std::to_array<SCFB_FBConnectionData>({
      {STRID(TimeOutSocket), STRID(DT), STRID(DLY), STRID(DT)},
  });

  const SCFB_FBNData cFBNData = {
      .mEventConnections = cEventConnections,
      .mDataConnections = cDataConnections,
      .mAdapterConnections = {},
  };
} // namespace

FORTE_E_RTimeOut::FORTE_E_RTimeOut(const CStringDictionary::TStringId paInstanceNameId,
                                   forte::core::CFBContainer &paContainer) :
    CCompositeFB(paContainer, cFBInterfaceSpec, paInstanceNameId, cFBNData),
    fb_DLY(STRID(DLY), *this),
    var_TimeOutSocket(STRID(TimeOutSocket), *this, forte::cgCFBParentAdapterlistIDMarker) {};

void FORTE_E_RTimeOut::setInitialValues() {
  CCompositeFB::setInitialValues();
}

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

forte::ISocketPin *FORTE_E_RTimeOut::getSocketPinUnchecked(const size_t paIndex) {
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
