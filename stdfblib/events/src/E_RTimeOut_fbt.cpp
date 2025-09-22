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

#include "forte/iec61499/events/E_RTimeOut_fbt.h"

#include "forte/iec61499/events/ARTimeOut_adp.h"
#include "forte/iec61131_functions.h"
#include "forte/datatypes/forte_array_common.h"
#include "forte/datatypes/forte_array.h"
#include "forte/datatypes/forte_array_fixed.h"
#include "forte/datatypes/forte_array_variable.h"

using namespace std::literals;

using namespace forte::literals;

namespace forte::iec61499::events {
  namespace {
    const auto cSocketNameIds = std::array{"TimeOutSocket"_STRID};

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
        {"TimeOutSocket"_STRID, "START"_STRID, "DLY"_STRID, "START"_STRID},
        {"TimeOutSocket"_STRID, "STOP"_STRID, "DLY"_STRID, "STOP"_STRID},
        {"DLY"_STRID, "EO"_STRID, "TimeOutSocket"_STRID, "TimeOut"_STRID},
    });

    const auto cDataConnections = std::to_array<SCFB_FBConnectionData>({
        {"TimeOutSocket"_STRID, "DT"_STRID, "DLY"_STRID, "DT"_STRID},
    });

    const SCFB_FBNData cFBNData = {
        .mEventConnections = cEventConnections,
        .mDataConnections = cDataConnections,
        .mAdapterConnections = {},
    };
  } // namespace

  DEFINE_FIRMWARE_FB(FORTE_E_RTimeOut, "iec61499::events::E_RTimeOut"_STRID)

  FORTE_E_RTimeOut::FORTE_E_RTimeOut(const StringId paInstanceNameId, CFBContainer &paContainer) :
      CCompositeFB(paContainer, cFBInterfaceSpec, paInstanceNameId, cFBNData),
      fb_DLY("DLY"_STRID, *this),
      var_TimeOutSocket("TimeOutSocket"_STRID, *this, cgCFBParentAdapterlistIDMarker) {};

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

} // namespace forte::iec61499::events
