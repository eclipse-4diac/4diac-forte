/*************************************************************************
 *** Copyright (c) 2013 fortiss GmbH
 *** This program and the accompanying materials are made available under the
 *** terms of the Eclipse Public License 2.0 which is available at
 *** http://www.eclipse.org/legal/epl-2.0.
 ***
 *** SPDX-License-Identifier: EPL-2.0
 ***
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter 3.0.0.202511261532!
 ***
 *** Name: E_TimeOut
 *** Description: Simple implementation of the timeout services
 *** Version:
 ***     3.0: 2025-04-14/Patrick Aigner -  - changed package
 ***     1.0: 2017-09-22/Alois Zoitl - fortiss GmbH - initial API and implementation and/or initial documentation
 *************************************************************************/

#include "forte/iec61499/events/E_TimeOut_fbt.h"

#include "forte/iec61499/events/ATimeOut_adp.h"
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

  DEFINE_FIRMWARE_FB(FORTE_E_TimeOut, "iec61499::events::E_TimeOut"_STRID)

  FORTE_E_TimeOut::FORTE_E_TimeOut(const StringId paInstanceNameId, CFBContainer &paContainer) :
      CCompositeFB(paContainer, cFBInterfaceSpec, paInstanceNameId, cFBNData),
      fb_DLY("DLY"_STRID, *this),
      var_TimeOutSocket("TimeOutSocket"_STRID, *this, forte::cgCFBParentAdapterlistIDMarker) {};

  void FORTE_E_TimeOut::setInitialValues() {
    CCompositeFB::setInitialValues();
  }

  void FORTE_E_TimeOut::readInputData(TEventID) {
    // nothing to do
  }

  void FORTE_E_TimeOut::writeOutputData(TEventID) {
    // nothing to do
  }

  CIEC_ANY *FORTE_E_TimeOut::getDI(size_t) {
    return nullptr;
  }

  CIEC_ANY *FORTE_E_TimeOut::getDO(size_t) {
    return nullptr;
  }

  forte::ISocketPin *FORTE_E_TimeOut::getSocketPinUnchecked(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_TimeOutSocket;
    }
    return nullptr;
  }

  CEventConnection *FORTE_E_TimeOut::getEOConUnchecked(TPortId) {
    return nullptr;
  }

  CDataConnection **FORTE_E_TimeOut::getDIConUnchecked(TPortId) {
    return nullptr;
  }

  CDataConnection *FORTE_E_TimeOut::getDOConUnchecked(TPortId) {
    return nullptr;
  }

  CDataConnection *FORTE_E_TimeOut::getIf2InConUnchecked(TPortId) {
    return nullptr;
  }

} // namespace forte::iec61499::events
