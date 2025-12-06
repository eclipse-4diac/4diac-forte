/*******************************************************************************
 * Copyright (c) 2013 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "forte/iec61499/events/ATimeOut_adp.h"
#include "forte/eventconn.h"
#include "forte/iec61499/events/E_N_TABLE_fbt.h"

using namespace forte::literals;

namespace forte::iec61499::events {
  namespace {
    const auto cDataOutputNames = std::array{"DT"_STRID};
    const auto cEventInputNames = std::array{"TimeOut"_STRID};
    const auto cEventOutputNames = std::array{"START"_STRID, "STOP"_STRID};

    const SFBInterfaceSpec cFBInterfaceSpecSocket = {
        .mEINames = cEventInputNames,
        .mEITypeNames = {},
        .mEONames = cEventOutputNames,
        .mEOTypeNames = {},
        .mDINames = {},
        .mDONames = cDataOutputNames,
        .mDIONames = {},
        .mSocketNames = {},
        .mPlugNames = {},
    };

    const SFBInterfaceSpec cFBInterfaceSpecPlug = {
        .mEINames = cEventOutputNames,
        .mEITypeNames = {},
        .mEONames = cEventInputNames,
        .mEOTypeNames = {},
        .mDINames = cDataOutputNames,
        .mDONames = {},
        .mDIONames = {},
        .mSocketNames = {},
        .mPlugNames = {},
    };
  } // namespace

  DEFINE_ADAPTER_TYPE(FORTE_ATimeOut, "iec61499::events::ATimeOut"_STRID)

  void FORTE_ATimeOut::setInitialValues() {
    forte::CAdapter::setInitialValues();
    var_DT = 0_TIME;
  }

  FORTE_ATimeOut::FORTE_ATimeOut(CFBContainer &paContainer,
                                 const SFBInterfaceSpec &paInterfaceSpec,
                                 const StringId paInstanceNameId,
                                 TForteUInt8 paParentAdapterlistID) :
      CAdapter(paContainer, paInterfaceSpec, paInstanceNameId, paParentAdapterlistID) {
  }

  FORTE_ATimeOut_Plug::FORTE_ATimeOut_Plug(StringId paInstanceNameId,
                                           CFBContainer &paContainer,
                                           TForteUInt8 paParentAdapterlistID) :
      FORTE_ATimeOut(paContainer, cFBInterfaceSpecPlug, paInstanceNameId, paParentAdapterlistID),
      conn_TimeOUT(*this, 0),
      conn_DT(nullptr) {
  }

  void FORTE_ATimeOut_Plug::readInputData(TEventID paEIID) {
    if (paEIID == scmEventSTARTID) {
      readData(0, var_DT, conn_DT);
      if (auto peer = static_cast<FORTE_ATimeOut_Socket *>(getPeer()); peer) {
        peer->var_DT = var_DT;
      }
    }
  }
  void FORTE_ATimeOut_Plug::writeOutputData(TEventID) {
    // nothing to do for this plug
  }

  CEventConnection *FORTE_ATimeOut_Plug::getEOConUnchecked(TPortId paEONum) {
    return (paEONum == 0) ? &conn_TimeOUT : nullptr;
  }

  CIEC_ANY *FORTE_ATimeOut_Plug::getDI(TPortId paDINum) {
    return (paDINum == 0) ? &var_DT : nullptr;
  }

  CDataConnection **FORTE_ATimeOut_Plug::getDIConUnchecked(TPortId paDINum) {
    return (paDINum == 0) ? &conn_DT : nullptr;
  }

  CDataConnection *FORTE_ATimeOut_Plug::getDOConUnchecked(TPortId) {
    return nullptr;
  }

  CIEC_ANY *FORTE_ATimeOut_Plug::getDO(TPortId) {
    return nullptr;
  }

  FORTE_ATimeOut_Socket::FORTE_ATimeOut_Socket(StringId paInstanceNameId,
                                               CFBContainer &paContainer,
                                               TForteUInt8 paParentAdapterlistID) :
      FORTE_ATimeOut(paContainer, cFBInterfaceSpecSocket, paInstanceNameId, paParentAdapterlistID),
      conn_START(*this, 0),
      conn_STOP(*this, 1),
      conn_DT(*this, 0, var_DT) {
  }

  void FORTE_ATimeOut_Socket::readInputData(TEventID) {
    // nothing to be done for socket
  }

  void FORTE_ATimeOut_Socket::writeOutputData(TEventID paEIID) {
    if (paEIID == scmEventSTARTID) {
      writeData(0, var_DT, conn_DT);
    }
  }

  CEventConnection *FORTE_ATimeOut_Socket::getEOConUnchecked(TPortId paEONum) {
    switch (paEONum) {
      case 0: return &conn_START; break;
      case 1: return &conn_STOP; break;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_ATimeOut_Socket::getDI(TPortId) {
    return nullptr;
  }

  CDataConnection **FORTE_ATimeOut_Socket::getDIConUnchecked(TPortId) {
    return nullptr;
  }

  CDataConnection *FORTE_ATimeOut_Socket::getDOConUnchecked(TPortId paDONum) {
    return (paDONum == 0) ? &conn_DT : nullptr;
  }

  CIEC_ANY *FORTE_ATimeOut_Socket::getDO(TPortId paDONum) {
    return (paDONum == 0) ? &var_DT : nullptr;
  }
} // namespace forte::iec61499::events
