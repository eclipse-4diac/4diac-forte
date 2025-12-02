/*************************************************************************
 *** Copyright (c) 2013 fortiss GmbH
 *** This program and the accompanying materials are made available under the
 *** terms of the Eclipse Public License 2.0 which is available at
 *** http://www.eclipse.org/legal/epl-2.0.
 *** SPDX-License-Identifier: EPL-2.0
 ***
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter 3.0.0.202511261532!
 ***
 *** Name: ATimeOut
 *** Description: Interface for a time out service roughly based on the definitions of ROOM
 *** Version:
 ***     3.0: 2025-04-14/Patrick Aigner -  - changed package
 ***     1.0: 2017-09-22/Alois Zoitl - fortiss GmbH - initial API and implementation and/or initial documentation
 *************************************************************************/

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
    const auto cEventInputNames = std::array{"TimeOut"_STRID};
    const auto cEventOutputNames = std::array{"START"_STRID, "STOP"_STRID};
    const auto cDataOutputNames = std::array{"DT"_STRID};

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

  FORTE_ATimeOut::FORTE_ATimeOut(CFBContainer &paContainer,
                                 const SFBInterfaceSpec &paInterfaceSpec,
                                 const StringId paInstanceNameId,
                                 TForteUInt8 paParentAdapterlistID) :
      CAdapter(paContainer, paInterfaceSpec, paInstanceNameId, paParentAdapterlistID),
      var_DT(0_TIME) {
  }

  void FORTE_ATimeOut::setInitialValues() {
    forte::CAdapter::setInitialValues();
    var_DT = 0_TIME;
  }

  FORTE_ATimeOut_Plug::FORTE_ATimeOut_Plug(StringId paInstanceNameId,
                                           CFBContainer &paContainer,
                                           TForteUInt8 paParentAdapterlistID) :
      FORTE_ATimeOut(paContainer, cFBInterfaceSpecPlug, paInstanceNameId, paParentAdapterlistID),
      conn_TimeOut(*this, 0),
      conn_DT(nullptr) {
  }

  void FORTE_ATimeOut_Plug::readInputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventSTARTID: {
        readData(0, var_DT, conn_DT);
        if (auto peer = static_cast<FORTE_ATimeOut_Socket *>(getPeer()); peer) {
          peer->var_DT = var_DT;
        }
        break;
      }
      default: break;
    }
  }

  void FORTE_ATimeOut_Plug::writeOutputData(TEventID) {
    // nothing to do
  }
  CIEC_ANY *FORTE_ATimeOut_Plug::getDI(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_DT;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_ATimeOut_Plug::getDO(size_t) {
    return nullptr;
  }

  CEventConnection *FORTE_ATimeOut_Plug::getEOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_TimeOut;
    }
    return nullptr;
  }

  CDataConnection **FORTE_ATimeOut_Plug::getDIConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_DT;
    }
    return nullptr;
  }

  CDataConnection *FORTE_ATimeOut_Plug::getDOConUnchecked(TPortId) {
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
    // nothing to do
  }

  void FORTE_ATimeOut_Socket::writeOutputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventSTARTID: {
        writeData(0, var_DT, conn_DT);
        break;
      }
      default: break;
    }
  }
  CIEC_ANY *FORTE_ATimeOut_Socket::getDI(size_t) {
    return nullptr;
  }

  CIEC_ANY *FORTE_ATimeOut_Socket::getDO(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_DT;
    }
    return nullptr;
  }

  CEventConnection *FORTE_ATimeOut_Socket::getEOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_START;
      case 1: return &conn_STOP;
    }
    return nullptr;
  }

  CDataConnection **FORTE_ATimeOut_Socket::getDIConUnchecked(TPortId) {
    return nullptr;
  }

  CDataConnection *FORTE_ATimeOut_Socket::getDOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_DT;
    }
    return nullptr;
  }
} // namespace forte::iec61499::events
