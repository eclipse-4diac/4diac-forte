/*******************************************************************************
 * Copyright (c) 2017, 2024 fortiss GmbH, Monika Wenger, Johannes Kepler University Linz
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v2.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v20.html
 *
 * Contributors:
 *   Johannes Messmer - initial API and implementation and/or initial documentation
 *   Monika Wenger, Alois Zoitl - migration to current runtime
 *******************************************************************************/

#include "IORevPiCore.h"
#include "../RevPiController.h"

using namespace forte::literals;
using namespace forte::io;

namespace forte::eclipse4diac::io::revpi {

  DEFINE_FIRMWARE_FB(FORTE_IORevPiCore, "eclipse4diac::io::revpi::IORevPiCore"_STRID)

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

  FORTE_IORevPiCore::FORTE_IORevPiCore(const forte::StringId paInstanceNameId, CFBContainer &paContainer) :
      ::forte::io::IOConfigFBMultiMaster(paContainer, cFBInterfaceSpec, paInstanceNameId),
      var_UpdateInterval(25_UINT),
      conn_INITO(*this, 0),
      conn_IND(*this, 1),
      conn_QI(nullptr),
      conn_UpdateInterval(nullptr),
      conn_QO(*this, 0, var_QO),
      conn_STATUS(*this, 1, var_STATUS),
      var_BusAdapterOut("BusAdapterOut"_STRID, *this, 0) {
  }

  void FORTE_IORevPiCore::setInitialValues() {
    var_QI = 0_BOOL;
    var_UpdateInterval = 25_UINT;
    var_QO = 0_BOOL;
    var_STATUS = u""_WSTRING;
  }

  void FORTE_IORevPiCore::setConfig() {
    RevPiController::Config config;
    config.updateInterval = static_cast<CIEC_UINT::TValueType>(var_UpdateInterval);
    getDeviceController()->setConfig(&config);
  }

  forte::io::IODeviceController *FORTE_IORevPiCore::createDeviceController(CDeviceExecution &paDeviceExecution) {
    return new RevPiController(paDeviceExecution);
  }

  void FORTE_IORevPiCore::readInputData(const TEventID paEIID) {
    switch (paEIID) {
      case scmEventINITID: {
        readData(0, var_QI, conn_QI);
        readData(1, var_UpdateInterval, conn_UpdateInterval);
        break;
      }
      default: break;
    }
  }

  void FORTE_IORevPiCore::writeOutputData(const TEventID paEIID) {
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

  CIEC_ANY *FORTE_IORevPiCore::getDI(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_QI;
      case 1: return &var_UpdateInterval;
    }
    return nullptr;
  }

  CIEC_ANY *FORTE_IORevPiCore::getDO(const size_t paIndex) {
    switch (paIndex) {
      case 0: return &var_QO;
      case 1: return &var_STATUS;
    }
    return nullptr;
  }

  forte::IPlugPin *FORTE_IORevPiCore::getPlugPinUnchecked(size_t paIndex) {
    return (paIndex == 0) ? &var_BusAdapterOut : nullptr;
  }

  CEventConnection *FORTE_IORevPiCore::getEOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_INITO;
      case 1: return &conn_IND;
    }
    return nullptr;
  }

  CDataConnection **FORTE_IORevPiCore::getDIConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_QI;
      case 1: return &conn_UpdateInterval;
    }
    return nullptr;
  }

  CDataConnection *FORTE_IORevPiCore::getDOConUnchecked(const TPortId paIndex) {
    switch (paIndex) {
      case 0: return &conn_QO;
      case 1: return &conn_STATUS;
    }
    return nullptr;
  }
} // namespace forte::eclipse4diac::io::revpi
