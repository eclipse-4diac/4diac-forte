/*************************************************************************
 *** Copyright (c) 2022 Peirlberger Juergen
 ***
 *** This program and the accompanying materials are made
 *** available under the terms of the Eclipse Public License 2.0
 *** which is available at https://www.eclipse.org/legal/epl-2.0/
 ***
 *** SPDX-License-Identifier: EPL-2.0
 ***
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x NG!
 ***
 *** Name: PLCnextMaster
 *** Description: Service Interface Function Block Type
 *** Version:
 ***     1.0: 2022-04-07/Peirlberger Juergen -  - initial API and implementation and/or initial documentation
 *************************************************************************/

#include "PLCnextMaster.h"
#include "../deviceController.h"

USE_STRING_ID(BOOL);
USE_STRING_ID(BusAdapterOut);
USE_STRING_ID(EInit);
USE_STRING_ID(Event);
USE_STRING_ID(IND);
USE_STRING_ID(INIT);
USE_STRING_ID(INITO);
USE_STRING_ID(PLCnextBusAdapter);
USE_STRING_ID(PLCnextMaster);
USE_STRING_ID(QI);
USE_STRING_ID(QO);
USE_STRING_ID(SlaveUpdateInterval);
USE_STRING_ID(STATUS);
USE_STRING_ID(UINT);
USE_STRING_ID(WSTRING);

DEFINE_FIRMWARE_FB(FORTE_PLCnextMaster, STRID(PLCnextMaster))

namespace {
  const auto cDataInputNames = std::array{STRID(QI), STRID(SlaveUpdateInterval)};
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

FORTE_PLCnextMaster::FORTE_PLCnextMaster(const CStringDictionary::TStringId paInstanceNameId,
                                         forte::core::CFBContainer &paContainer) :
    forte::core::io::IOConfigFBMultiMaster(paContainer, cFBInterfaceSpec, paInstanceNameId),
    var_QI(0_BOOL),
    var_SlaveUpdateInterval(25_UINT),
    var_QO(0_BOOL),
    var_STATUS(u""_WSTRING),
    var_BusAdapterOut(STRID(BusAdapterOut), *this, true),
    conn_INITO(*this, 0),
    conn_IND(*this, 1),
    conn_QI(nullptr),
    conn_SlaveUpdateInterval(nullptr),
    conn_QO(*this, 0, var_QO),
    conn_STATUS(*this, 1, var_STATUS) {};

void FORTE_PLCnextMaster::setInitialValues() {
  var_QI = 0_BOOL;
  var_SlaveUpdateInterval = 25_UINT;
  var_QO = 0_BOOL;
  var_STATUS = u""_WSTRING;
}

void FORTE_PLCnextMaster::readInputData(const TEventID paEIID) {
  switch (paEIID) {
    case scmEventINITID: {
      readData(0, var_QI, conn_QI);
      readData(1, var_SlaveUpdateInterval, conn_SlaveUpdateInterval);
      break;
    }
    default: break;
  }
}

void FORTE_PLCnextMaster::writeOutputData(const TEventID paEIID) {
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

CIEC_ANY *FORTE_PLCnextMaster::getDI(const size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_QI;
    case 1: return &var_SlaveUpdateInterval;
  }
  return nullptr;
}

CIEC_ANY *FORTE_PLCnextMaster::getDO(const size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_QO;
    case 1: return &var_STATUS;
  }
  return nullptr;
}

forte::IPlugPin *FORTE_PLCnextMaster::getPlugPinUnchecked(size_t paIndex) {
  return (paIndex == 0) ? &var_BusAdapterOut : nullptr;
}

CEventConnection *FORTE_PLCnextMaster::getEOConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_INITO;
    case 1: return &conn_IND;
  }
  return nullptr;
}

CDataConnection **FORTE_PLCnextMaster::getDIConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_QI;
    case 1: return &conn_SlaveUpdateInterval;
  }
  return nullptr;
}

CDataConnection *FORTE_PLCnextMaster::getDOConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_QO;
    case 1: return &conn_STATUS;
  }
  return nullptr;
}

forte::core::io::IODeviceController *FORTE_PLCnextMaster::createDeviceController(CDeviceExecution &paDeviceExecution) {
  return new PLCnextDeviceController(paDeviceExecution);
}

void FORTE_PLCnextMaster::setConfig() {
  PLCnextDeviceController::PLCnextConfig config;
  config.updateInterval = var_SlaveUpdateInterval.operator TForteUInt16();
  getDeviceController()->setConfig(&config);
}
