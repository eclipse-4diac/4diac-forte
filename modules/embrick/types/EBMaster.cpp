/*******************************************************************************
 * Copyright (c) 2016, 2025 Johannes Messmer (admin@jomess.com), fortiss GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Johannes Messmer - initial API and implementation and/or initial documentation
 *   Jose Cabral - Cleaning of namespaces
 *******************************************************************************/

#include "EBMaster.h"
#include "../handler/bus.h"

using namespace forte::literals;

namespace forte::eclipse4diac::io::embrick {

  DEFINE_FIRMWARE_FB(FORTE_EBMaster, "eclipse4diac::io::embrick::EBMaster"_STRID)

  namespace {
    const auto cDataInputNames = std::array{"QI"_STRID,           "BusInterface"_STRID, "BusSelectPin"_STRID,
                                            "BusInitSpeed"_STRID, "BusLoopSpeed"_STRID, "SlaveUpdateInterval"_STRID};
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

  FORTE_EBMaster::FORTE_EBMaster(const forte::StringId paInstanceNameId, CFBContainer &paContainer) :
      IOConfigFBMultiMaster(paContainer, cFBInterfaceSpec, paInstanceNameId),
      var_BusInterface(1_UINT),
      var_BusSelectPin(49_UINT),
      var_BusInitSpeed(300000_UDINT),
      var_BusLoopSpeed(700000_UDINT),
      var_SlaveUpdateInterval(25_UINT),
      conn_INITO(*this, 0),
      conn_IND(*this, 1),
      conn_QI(nullptr),
      conn_BusInterface(nullptr),
      conn_BusSelectPin(nullptr),
      conn_BusInitSpeed(nullptr),
      conn_BusLoopSpeed(nullptr),
      conn_SlaveUpdateInterval(nullptr),
      conn_QO(*this, 0, var_QO),
      conn_STATUS(*this, 1, var_STATUS),
      var_BusAdapterOut("BusAdapterOut"_STRID, *this, 0) {};

  void FORTE_EBMaster::setInitialValues() {
    var_QI = 0_BOOL;
    var_BusInterface = 1_UINT;
    var_BusSelectPin = 49_UINT;
    var_BusInitSpeed = 300000_UDINT;
    var_BusLoopSpeed = 700000_UDINT;
    var_SlaveUpdateInterval = 25_UINT;
    var_QO = 0_BOOL;
    var_STATUS = u""_WSTRING;
  }

  void FORTE_EBMaster::readInputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventINITID: {
        readData(0, var_QI, conn_QI);
        readData(3, var_BusInitSpeed, conn_BusInitSpeed);
        readData(5, var_SlaveUpdateInterval, conn_SlaveUpdateInterval);
        readData(4, var_BusLoopSpeed, conn_BusLoopSpeed);
        readData(1, var_BusInterface, conn_BusInterface);
        readData(2, var_BusSelectPin, conn_BusSelectPin);
        break;
      }
      default: break;
    }
  }

  void FORTE_EBMaster::writeOutputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventINITOID: {
        writeData(cFBInterfaceSpec.getNumDIs() + 0, var_QO, conn_QO);
        writeData(cFBInterfaceSpec.getNumDIs() + 1, var_STATUS, conn_STATUS);
        break;
      }
      case scmEventINDID: {
        writeData(cFBInterfaceSpec.getNumDIs() + 0, var_QO, conn_QO);
        writeData(cFBInterfaceSpec.getNumDIs() + 1, var_STATUS, conn_STATUS);
        break;
      }
      default: break;
    }
  }

  CIEC_ANY *FORTE_EBMaster::getDI(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_QI;
      case 1: return &var_BusInterface;
      case 2: return &var_BusSelectPin;
      case 3: return &var_BusInitSpeed;
      case 4: return &var_BusLoopSpeed;
      case 5: return &var_SlaveUpdateInterval;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_EBMaster::getDO(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_QO;
      case 1: return &var_STATUS;
    }
    return nullptr;
  }

  CEventConnection *FORTE_EBMaster::getEOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_INITO;
      case 1: return &conn_IND;
    }
    return nullptr;
  }

  CDataConnection **FORTE_EBMaster::getDIConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_QI;
      case 1: return &conn_BusInterface;
      case 2: return &conn_BusSelectPin;
      case 3: return &conn_BusInitSpeed;
      case 4: return &conn_BusLoopSpeed;
      case 5: return &conn_SlaveUpdateInterval;
    }
    return nullptr;
  }

  CDataConnection *FORTE_EBMaster::getDOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_QO;
      case 1: return &conn_STATUS;
    }
    return nullptr;
  }

  forte::IPlugPin *FORTE_EBMaster::getPlugPinUnchecked(size_t paIndex) {
    return (paIndex == 0) ? &var_BusAdapterOut : nullptr;
  }

  forte::io::IODeviceController *FORTE_EBMaster::createDeviceController(CDeviceExecution &paDeviceExecution) {
    return new EmbrickBusHandler(paDeviceExecution);
  }

  void FORTE_EBMaster::setConfig() {
    EmbrickBusHandler::Config config;
    config.mBusInterface = var_BusInterface.operator TForteUInt16();
    config.mBusSelectPin = var_BusSelectPin.operator TForteUInt16();
    config.mBusInitSpeed = var_BusInitSpeed.operator TForteUInt32();
    config.mBusLoopSpeed = var_BusLoopSpeed.operator TForteUInt32();
    getDeviceController()->setConfig(&config);
  }

  void FORTE_EBMaster::onStartup(CEventChainExecutionThread *const paECET) {
    var_BusAdapterOut->var_UpdateInterval = var_SlaveUpdateInterval;

    IOConfigFBMultiMaster::onStartup(paECET);
  }

} // namespace forte::eclipse4diac::io::embrick
