/*******************************************************************************
 * Copyright (c) 2025 Maximilian Scharf
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Maximilian Scharf - Initial implementation.
 *******************************************************************************/

#include "forte/io/eIO/eIXconfig_fbt.h"
#include "forte/io/eIO/eIX_fbt.h"

using namespace forte::literals;

namespace forte::io {
  namespace {
    const auto cEventInputNames = std::array{"CONF"_STRID};
    const auto cEventOutputNames = std::array{"CNF"_STRID};
    const auto cDataInputNames = std::array{"FE"_STRID, "RE"_STRID};
    const auto cDataOutputNames = std::array{"STATUS"_STRID};
    const auto cSocketNames = std::array{"eIX"_STRID};

    const SFBInterfaceSpec cFBInterfaceSpec = {
        .mEINames = cEventInputNames,
        .mEITypeNames = {},
        .mEONames = cEventOutputNames,
        .mEOTypeNames = {},
        .mDINames = cDataInputNames,
        .mDONames = cDataOutputNames,
        .mDIONames = {},
        .mSocketNames = cSocketNames,
        .mPlugNames = {},
    };
  } // namespace

  DEFINE_FIRMWARE_FB(FORTE_eIXconfig, "eIXconfig"_STRID)

  FORTE_eIXconfig::FORTE_eIXconfig(const StringId paInstanceNameId, CFBContainer &paContainer) :
      CeConfigFB(paInstanceNameId, cFBInterfaceSpec, paContainer),
      var_FE(0_BOOL),
      var_RE(0_BOOL),
      var_STATUS(u""_WSTRING),
      var_eIX("eIX"_STRID, *this, false),
      conn_CNF(*this, 0),
      conn_FE(nullptr),
      conn_RE(nullptr),
      conn_STATUS(*this, 0, var_STATUS) {};

  void FORTE_eIXconfig::setInitialValues() {
    CFunctionBlock::setInitialValues();
    var_FE = 0_BOOL;
    var_RE = 0_BOOL;
    var_STATUS = u""_WSTRING;
  }

  void FORTE_eIXconfig::executeEvent(const TEventID paEIID, CEventChainExecutionThread *const paECET) {
    switch (paEIID) {
      case scmEventCONFID:
        eventGen();
        sendOutputEvent(scmEventCNFID, paECET);
        break;
      default: break;
    }
  }

  void FORTE_eIXconfig::readInputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventCONFID: {
        readData(0, var_FE, conn_FE);
        readData(1, var_RE, conn_RE);
        break;
      }
      default: break;
    }
  }

  void FORTE_eIXconfig::writeOutputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventCNFID: {
        writeData(2, var_STATUS, conn_STATUS);
        break;
      }
      default: break;
    }
  }

  CIEC_ANY *FORTE_eIXconfig::getDI(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_FE;
      case 1: return &var_RE;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_eIXconfig::getDO(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_STATUS;
    }
    return nullptr;
  }

  ISocketPin *FORTE_eIXconfig::getSocketPinUnchecked(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_eIX;
    }
    return nullptr;
  }

  CEventConnection *FORTE_eIXconfig::getEOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_CNF;
    }
    return nullptr;
  }

  CDataConnection **FORTE_eIXconfig::getDIConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_FE;
      case 1: return &conn_RE;
    }
    return nullptr;
  }

  CDataConnection *FORTE_eIXconfig::getDOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_STATUS;
    }
    return nullptr;
  }

  bool FORTE_eIXconfig::eventGen() {
    DEVLOG_DEBUG("[eIXconfig] eventGen\r\n");
    // deregister every already registered evert-trigger condition of this FB
    deregisterFBsEventTrigger();

    auto *eIX = static_cast<FORTE_eIX *>(getIOPeer(var_eIX.getAdapterBlock()));
    if (eIX == nullptr) {
      DEVLOG_ERROR("[eIXconfig::evenGen] IO instance is nullptr.\r\n");
      return false;
    }

    /* REGISTER eTRIGGERS =================================================================== BEGIN === */
    // register rising edge event-trigger
    if (var_RE) {
      DEVLOG_DEBUG("[eventGen] register eRE event\r\n");
      auto *newEvent = new CeIO_RisingEdge(eIX, eIX->getHandle());
      registerEventTrigger(newEvent);
    }

    // register falling edge event-trigger
    if (var_FE) {
      DEVLOG_DEBUG("[eventGen] register eFE event\r\n");
      auto *newEvent = new CeIO_FallingEdge(eIX, eIX->getHandle());
      registerEventTrigger(newEvent);
    }
    /* REGISTER eTRIGGERS ===================================================================  END  === */

    return true;
  }
} // namespace forte::io
