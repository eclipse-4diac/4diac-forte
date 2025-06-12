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

USE_STRING_ID(BOOL);
USE_STRING_ID(BusAdapterOut);
USE_STRING_ID(EInit);
USE_STRING_ID(Event);
USE_STRING_ID(IND);
USE_STRING_ID(INIT);
USE_STRING_ID(INITO);
USE_STRING_ID(IORevPiBusAdapter);
USE_STRING_ID(IORevPiCore);
USE_STRING_ID(QI);
USE_STRING_ID(QO);
USE_STRING_ID(STATUS);
USE_STRING_ID(UINT);
USE_STRING_ID(UpdateInterval);
USE_STRING_ID(WSTRING);

#include "../RevPiController.h"

DEFINE_FIRMWARE_FB(FORTE_IORevPiCore, STRID(IORevPiCore))

namespace {
  const auto cDataInputNames = std::array{STRID(QI), STRID(UpdateInterval)};
  const auto cDataOutputNames = std::array{STRID(QO), STRID(STATUS)};
  const auto cEventInputNames = std::array{STRID(INIT)};
  const auto cEventInputTypeIds = std::array{STRID(EInit)};
  const auto cEventOutputNames = std::array{STRID(INITO), STRID(IND)};
  const auto cEventOutputTypeIds = std::array{STRID(EInit), STRID(Event)};
  const auto cPlugNameIds = std::array{STRID(BusAdapterOut)};
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

FORTE_IORevPiCore::FORTE_IORevPiCore(const CStringDictionary::TStringId paInstanceNameId,
                                     forte::core::CFBContainer &paContainer) :
    forte::core::io::IOConfigFBMultiMaster(paContainer, cFBInterfaceSpec, paInstanceNameId),
    var_UpdateInterval(25_UINT),
    conn_INITO(*this, 0),
    conn_IND(*this, 1),
    conn_QI(nullptr),
    conn_UpdateInterval(nullptr),
    conn_QO(*this, 0, var_QO),
    conn_STATUS(*this, 1, var_STATUS),
    var_BusAdapterOut(STRID(BusAdapterOut), *this, 0) {
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

forte::core::io::IODeviceController *FORTE_IORevPiCore::createDeviceController(CDeviceExecution &paDeviceExecution) {
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
