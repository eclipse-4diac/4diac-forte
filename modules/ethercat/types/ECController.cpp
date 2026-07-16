/*******************************************************************************
 * Copyright (c) 2026 Sichuan Qunyuan Technology Co., Ltd.
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Zijun Tang - initial API and implementation
 *******************************************************************************/

#include "ECController.h"
#include "../handler/bus.h"

using namespace forte::literals;

namespace forte::eclipse4diac::io::ethercat {

  DEFINE_FIRMWARE_FB(FORTE_ECController, "eclipse4diac::io::ethercat::ECController"_STRID,
                     "v2:SHA3-512:zdg7DeWvo5613AFG13T0n485ZdrDL9FBrnCW3_pjeBIh7Imn522te-wa85a-yODs2Hb5ClODMYi-r23BwH_ZiA==")

  namespace {
    const auto cDataInputNames = std::array{"QI"_STRID, "Enable"_STRID, "ControllerId"_STRID, "UpdateInterval"_STRID};
    const auto cDataOutputNames = std::array{"QO"_STRID, "STATUS"_STRID};
    const auto cEventInputNames = std::array{"INIT"_STRID, "REQ"_STRID};
    const auto cEventInputTypeIds = std::array{"Event"_STRID, "Event"_STRID};
    const auto cEventOutputNames = std::array{"INITO"_STRID, "CNF"_STRID, "IND"_STRID};
    const auto cEventOutputTypeIds = std::array{"Event"_STRID, "Event"_STRID, "EVENT"_STRID};
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
  }

  FORTE_ECController::FORTE_ECController(const forte::StringId paInstanceNameId, CFBContainer &paContainer) :
      IOConfigFBMultiMaster(paContainer, cFBInterfaceSpec, paInstanceNameId),
      var_QI(0_BOOL),
      var_Enable(0_BOOL),
      var_ControllerId(0_UINT),
      var_UpdateInterval(0_TIME),
      conn_INITO(*this, 0),
      conn_CNF(*this, 1),
      conn_IND(*this, 2),
      conn_QI(nullptr),
      conn_Enable(nullptr),
      conn_ControllerId(nullptr),
      conn_UpdateInterval(nullptr),
      conn_QO(*this, 0, var_QO),
      conn_STATUS(*this, 1, var_STATUS),
      var_BusAdapterOut("BusAdapterOut"_STRID, *this, 0) {
  };

  FORTE_ECController::~FORTE_ECController() {
    // var_STATUS is destroyed before ~IOConfigFBController runs. Shut down the bus here while
    // STATUS()/getDO() are still valid; base destructor deInit() then finds mController == nullptr.
    deInit(nullptr, true);
  }

  void FORTE_ECController::setInitialValues() {
    var_QI = 0_BOOL;
    var_Enable = 0_BOOL;
    var_ControllerId = 0_UINT;
    var_UpdateInterval.setFromMilliSeconds(1);
    var_QO = 0_BOOL;
    var_STATUS = u""_WSTRING;
  }

  void FORTE_ECController::readInputData(const TEventID paEIED) {
    switch (paEIED){
      case scmEventINITID:
      case scmEventREQID:
        readData(0, var_QI, conn_QI);
        readData(1, var_Enable, conn_Enable);
        readData(2, var_ControllerId, conn_ControllerId);
        readData(3, var_UpdateInterval, conn_UpdateInterval);
        break;
      default:break;
    }
  }

  void FORTE_ECController::writeOutputData(const TEventID paEIID) {
    switch(paEIID) {
      case scmEventIND:
      case scmEventCNF:
      case scmEventINITOID: {
        writeData(cFBInterfaceSpec.getNumDIs() + 0, var_QO, conn_QO);
        writeData(cFBInterfaceSpec.getNumDIs() + 1, var_STATUS, conn_STATUS);
        break;
      }
      default: break;
    }
  }

  CIEC_ANY *FORTE_ECController::getDI(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_QI;
      case 1: return &var_Enable;
      case 2: return &var_ControllerId;
      case 3: return &var_UpdateInterval;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_ECController::getDO(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_QO;
      case 1: return &var_STATUS;
    }
    return nullptr;
  }

  CEventConnection *FORTE_ECController::getEOConUnchecked(const TPortId paIndex) {
    switch(paIndex) {
      case 0: return &conn_INITO;
      case 1: return &conn_CNF;
      case 2: return &conn_IND;
    }
    return nullptr;
  }

  CDataConnection **FORTE_ECController::getDIConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_QI;
      case 1: return &conn_Enable;
      case 2: return &conn_ControllerId;
      case 3: return &conn_UpdateInterval;
    }
    return nullptr;
  }

  CDataConnection *FORTE_ECController::getDOConUnchecked(const TPortId paIndex) {
    switch(paIndex) {
      case 0: return &conn_QO;
      case 1: return &conn_STATUS;
    }
    return nullptr;
  }

  forte::IPlugPin *FORTE_ECController::getPlugPinUnchecked(size_t paIndex) {
    return (paIndex == 0) ? &var_BusAdapterOut : nullptr;
  }

  forte::io::IODeviceController *FORTE_ECController::createDeviceController(CDeviceExecution &paDeviceExecution) {
    return new ECBusHandler(paDeviceExecution);
  }

  void FORTE_ECController::setConfig() {
    ECBusHandler::Config config;
    config.mECControllerId = var_ControllerId.operator TForteUInt16();
    config.mUpdateInterval = static_cast<unsigned int>(var_UpdateInterval.getInMicroSeconds());
    DEVLOG_INFO("ECControllerId is:%d, and UpdateInterval is: %d\n", config.mECControllerId, config.mUpdateInterval);
    getDeviceController()->setConfig(&config);
  }

  void FORTE_ECController::onStartup(CEventChainExecutionThread *const paECET) {
    var_BusAdapterOut->var_Index = 0_UINT;
    IOConfigFBMultiMaster::onStartup(paECET);
  }

  EMGMResponse FORTE_ECController::changeExecutionState(EMGMCommandType paCommand) {
    if (paCommand == EMGMCommandType::Kill || paCommand == EMGMCommandType::Stop) {
      if (auto *bus = static_cast<ECBusHandler *>(getDeviceController()); bus != nullptr) {
        bus->enableECCycle(false);
      }
    }

    return IOConfigFBMultiMaster::changeExecutionState(paCommand);
  }

  void FORTE_ECController::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
    if (paEIID == scmEventREQID) {
      // Align with legacy EtherCAT behavior: REQ toggles cyclic bus execution.
      readInputData(paEIID);
      auto *bus = static_cast<ECBusHandler *>(getDeviceController());
      if (bus != nullptr) {
        bus->enableECCycle(var_Enable);
      }
      sendOutputEvent(scmEventCNF, paECET);
    }
    IOConfigFBMultiMaster::executeEvent(paEIID, paECET);
  }
      
}