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

#include "forte/io/eIO/eIWconfig_fbt.h"
#include "forte/io/eIO/eIW_fbt.h"

using namespace forte::literals;

namespace forte::io {
  namespace {
    const auto cEventInputNames = std::array{"CONF"_STRID};
    const auto cDataInputNames = std::array{"ST"_STRID, "BT"_STRID, "GRAD"_STRID};
    const auto cDataOutputNames = std::array{"STATUS"_STRID};
    const auto cEventOutputNames = std::array{"CNF"_STRID};
    const auto cPlugNames = std::array{"eIW"_STRID};

    const SFBInterfaceSpec cFBInterfaceSpec = {
        .mEINames = cEventInputNames,
        .mEITypeNames = {},
        .mEONames = cEventOutputNames,
        .mEOTypeNames = {},
        .mDINames = cDataInputNames,
        .mDONames = cDataOutputNames,
        .mDIONames = {},
        .mSocketNames = {},
        .mPlugNames = cPlugNames,
    };
  } // namespace

  DEFINE_FIRMWARE_FB(FORTE_eIWconfig, "eIWconfig"_STRID)

  FORTE_eIWconfig::FORTE_eIWconfig(const StringId paInstanceNameId, CFBContainer &paContainer) :
      CeConfigFB(paInstanceNameId, cFBInterfaceSpec, paContainer),
      var_ST(0_WORD),
      var_BT(0_WORD),
      var_GRAD(0_WORD),
      var_STATUS(u""_WSTRING),
      var_eIW("eIW"_STRID, *this, 0),
      conn_CNF(*this, 0),
      conn_ST(nullptr),
      conn_BT(nullptr),
      conn_GRAD(nullptr),
      conn_STATUS(*this, 0, var_STATUS) {};

  void FORTE_eIWconfig::setInitialValues() {
    CFunctionBlock::setInitialValues();
    var_ST = 0_WORD;
    var_BT = 0_WORD;
    var_GRAD = 0_WORD;
    var_STATUS = u""_WSTRING;
  }

  void FORTE_eIWconfig::executeEvent(const TEventID paEIID, CEventChainExecutionThread *const paECET) {
    switch (paEIID) {
      case scmEventCONFID:
        eventGen();
        sendOutputEvent(scmEventCNFID, paECET);
        break;
      default: break;
    }
  }

  void FORTE_eIWconfig::readInputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventCONFID: {
        readData(0, var_ST, conn_ST);
        readData(1, var_BT, conn_BT);
        readData(2, var_GRAD, conn_GRAD);
        break;
      }
      default: break;
    }
  }

  void FORTE_eIWconfig::writeOutputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventCNFID: {
        writeData(3, var_STATUS, conn_STATUS);
        break;
      }
      default: break;
    }
  }

  CIEC_ANY *FORTE_eIWconfig::getDI(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_ST;
      case 1: return &var_BT;
      case 2: return &var_GRAD;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_eIWconfig::getDO(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_STATUS;
    }
    return nullptr;
  }

  ISocketPin *FORTE_eIWconfig::getSocketPinUnchecked(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_eIW;
    }
    return nullptr;
  }

  CEventConnection *FORTE_eIWconfig::getEOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_CNF;
    }
    return nullptr;
  }

  CDataConnection **FORTE_eIWconfig::getDIConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_ST;
      case 1: return &conn_BT;
      case 2: return &conn_GRAD;
    }
    return nullptr;
  }

  CDataConnection *FORTE_eIWconfig::getDOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_STATUS;
    }
    return nullptr;
  }

  bool FORTE_eIWconfig::eventGen() {
    DEVLOG_DEBUG("[eIWconfig] eventGen\r\n");

    // deregister every already registered evert-trigger condition of this FB
    deregisterFBsEventTrigger();

    /* connecting to adapter peer */
    FORTE_eIW *eIW = nullptr;

    /* CONNECTING TO PEER =================================================================== BEGIN === */
    if (auto &plg = var_eIW.getAdapterCon()->getPlug(); plg.getAdapterBlock() != nullptr) {
      eIW = &static_cast<FORTE_eIW &>(plg.getAdapterBlock()->getParent());
    } else {
      DEVLOG_ERROR("[eIWconfig] Could not reach eIW FB via adapter!\r\n");
      return false;
    }

    if (eIW == nullptr) {
      DEVLOG_ERROR("[eIWconfig] eIW is nullptr\r\n");
      return false;
    }
    /* CONNECTING TO PEER ===================================================================  END  === */

    /* REGISTER eTRIGGERS =================================================================== BEGIN === */
    // deregister every already registered evert-trigger condition of this FB
    deregisterFBsEventTrigger();

    CeSpecBase *newEvent;

    // register bounded area event-trigger
    if (var_ST != 0 && var_BT != 0) {
      DEVLOG_DEBUG("[eventGen] register eBA event\r\n");
      newEvent = new CeIO_BoundedArea(eIW, eIW->getHandle(), var_BT, var_ST);
      registerEventTrigger(newEvent);
    } else {
      // register upper-threshold event-trigger
      if (var_ST != 0) {
        DEVLOG_DEBUG("[eventGen] register eUT event\r\n");
        newEvent = new CeIO_LowerThreshold(eIW, eIW->getHandle(), var_ST);
        registerEventTrigger(newEvent);
      }
      // register bottum/lower-threshold event-trigger
      if (var_BT != 0) {
        DEVLOG_DEBUG("[eventGen] register eGR event\r\n");
        newEvent = new CeIO_UpperThreshold(eIW, eIW->getHandle(), var_BT);
        registerEventTrigger(newEvent);
      }
    }
    // register gradient event-trigger
    if (var_GRAD != 0) {
      DEVLOG_DEBUG("[eventGen] register eGR event\r\n");
      newEvent = new CeIO_Gradient(eIW, eIW->getHandle(), var_GRAD);
      registerEventTrigger(newEvent);
    }
    /* REGISTER eTRIGGERS ===================================================================  END  === */

    return true;
  }
} // namespace forte::io
