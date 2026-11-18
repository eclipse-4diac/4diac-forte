/*******************************************************************************
 * Copyright (c) 2026 Thomas Oellinger
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Thomas Oellinger - initial implementation
 *******************************************************************************/

#include "MCCIOController.h"

#include "../MCCDaqHatController.h"

using namespace forte::literals;

namespace forte::eclipse4diac::io::mccdaqhat {

  DEFINE_FIRMWARE_FB(FORTE_MCCIOController, "eclipse4diac::io::mccdaqhat::MCCIOController"_STRID)

  namespace {
    const auto cDataInputNames = std::array{"QI"_STRID, "UpdateInterval"_STRID};
    const auto cDataOutputNames = std::array{"QO"_STRID, "STATUS"_STRID};
    const auto cEventInputNames = std::array{"INIT"_STRID};
    const auto cEventInputTypeIds = std::array{"EInit"_STRID};
    const auto cEventOutputNames = std::array{"INITO"_STRID, "IND"_STRID};
    const auto cEventOutputTypeIds = std::array{"EInit"_STRID, "Event"_STRID};
    const auto cPlugNameIds = std::array{"BusAdapterOut"_STRID};
    const SFBInterfaceSpec cFBInterfaceSpec = {
        .mEINames = cEventInputNames,
        .mEITypeNames = cEventInputTypeIds,
        .mEONames = cEventOutputNames,
        .mEOTypeNames = cEventOutputTypeIds,
        .mDINames = cDataInputNames,
        .mDONames = cDataOutputNames,
        .mDIONames = {},
        .mSocketNames = {},
        .mPlugNames = cPlugNameIds,
    };
  } // namespace

  FORTE_MCCIOController::FORTE_MCCIOController(const forte::StringId paInstanceNameId, CFBContainer &paContainer) :
      IOConfigFBMultiMaster(paContainer, cFBInterfaceSpec, paInstanceNameId),
      var_UpdateInterval(25_UINT),
      conn_INITO(*this, 0),
      conn_IND(*this, 1),
      conn_QI(nullptr),
      conn_UpdateInterval(nullptr),
      conn_QO(*this, 0, var_QO),
      conn_STATUS(*this, 1, var_STATUS),
      var_BusAdapterOut("BusAdapterOut"_STRID, *this, 0) {
  }

  FORTE_MCCIOController::~FORTE_MCCIOController() {
    // var_STATUS is destroyed before ~IOConfigFBController runs. Stop the controller here while
    // STATUS()/getDO() are still valid; the base destructor then finds mController == nullptr.
    deInit(nullptr, true);
  }

  void FORTE_MCCIOController::setInitialValues() {
    var_QI = 0_BOOL;
    var_UpdateInterval = 25_UINT;
    var_QO = 0_BOOL;
    var_STATUS = u""_WSTRING;
  }

  void FORTE_MCCIOController::readInputData(const TEventID paEIID) {
    if (paEIID == scmEventINITID) {
      readData(0, var_QI, conn_QI);
      readData(1, var_UpdateInterval, conn_UpdateInterval);
    }
  }

  void FORTE_MCCIOController::writeOutputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventINITOID:
      case scmEventINDID:
        writeData(cFBInterfaceSpec.getNumDIs() + 0, var_QO, conn_QO);
        writeData(cFBInterfaceSpec.getNumDIs() + 1, var_STATUS, conn_STATUS);
        break;
      default: break;
    }
  }

  CIEC_ANY *FORTE_MCCIOController::getDI(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_QI;
      case 1: return &var_UpdateInterval;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_MCCIOController::getDO(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_QO;
      case 1: return &var_STATUS;
    }
    return nullptr;
  }

  CEventConnection *FORTE_MCCIOController::getEOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_INITO;
      case 1: return &conn_IND;
    }
    return nullptr;
  }

  CDataConnection **FORTE_MCCIOController::getDIConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_QI;
      case 1: return &conn_UpdateInterval;
    }
    return nullptr;
  }

  CDataConnection *FORTE_MCCIOController::getDOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_QO;
      case 1: return &conn_STATUS;
    }
    return nullptr;
  }

  forte::IPlugPin *FORTE_MCCIOController::getPlugPinUnchecked(size_t paIndex) {
    return (paIndex == 0) ? &var_BusAdapterOut : nullptr;
  }

  forte::io::IODeviceController *FORTE_MCCIOController::createDeviceController(CDeviceExecution &paDeviceExecution) {
    return new MCCDaqHatController(paDeviceExecution);
  }

  void FORTE_MCCIOController::setConfig() {
    MCCDaqHatController::Config config;
    config.mUpdateInterval = static_cast<CIEC_UINT::TValueType>(var_UpdateInterval);
    getDeviceController()->setConfig(&config);
  }

} // namespace forte::eclipse4diac::io::mccdaqhat
