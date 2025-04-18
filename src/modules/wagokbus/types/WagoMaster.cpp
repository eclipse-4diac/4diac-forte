/*************************************************************************
 * Copyright (c) 2018, 2025 fortiss GmbH, Monika Wenger
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *************************************************************************/

#include "WagoMaster.h"

USE_STRING_ID(BOOL);
USE_STRING_ID(BusAdapterOut);
USE_STRING_ID(EInit);
USE_STRING_ID(Event);
USE_STRING_ID(IND);
USE_STRING_ID(INIT);
USE_STRING_ID(INITO);
USE_STRING_ID(QI);
USE_STRING_ID(QO);
USE_STRING_ID(STATUS);
USE_STRING_ID(UINT);
USE_STRING_ID(UpdateInterval);
USE_STRING_ID(WagoBusAdapter);
USE_STRING_ID(WagoMaster);
USE_STRING_ID(WSTRING);

#include "../WagoDeviceController.h"

using namespace forte::core::io;

DEFINE_FIRMWARE_FB(FORTE_WagoMaster, STRID(WagoMaster))

const CStringDictionary::TStringId FORTE_WagoMaster::scmDataInputNames[] = {STRID(QI), STRID(UpdateInterval)};
const CStringDictionary::TStringId FORTE_WagoMaster::scmDataInputTypeIds[] = {STRID(BOOL), STRID(UINT)};
const CStringDictionary::TStringId FORTE_WagoMaster::scmDataOutputNames[] = {STRID(QO), STRID(STATUS)};
const CStringDictionary::TStringId FORTE_WagoMaster::scmDataOutputTypeIds[] = {STRID(BOOL), STRID(WSTRING)};
const TDataIOID FORTE_WagoMaster::scmEIWith[] = {0, 1, scmWithListDelimiter};
const TForteInt16 FORTE_WagoMaster::scmEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_WagoMaster::scmEventInputNames[] = {STRID(INIT)};
const CStringDictionary::TStringId FORTE_WagoMaster::scmEventInputTypeIds[] = {STRID(EInit)};
const TDataIOID FORTE_WagoMaster::scmEOWith[] = {0, 1, scmWithListDelimiter, 1, 0, scmWithListDelimiter};
const TForteInt16 FORTE_WagoMaster::scmEOWithIndexes[] = {0, 3};
const CStringDictionary::TStringId FORTE_WagoMaster::scmEventOutputNames[] = {STRID(INITO), STRID(IND)};
const CStringDictionary::TStringId FORTE_WagoMaster::scmEventOutputTypeIds[] = {STRID(EInit), STRID(Event)};
const SAdapterInstanceDef FORTE_WagoMaster::scmAdapterInstances[] = {
    {STRID(WagoBusAdapter), STRID(BusAdapterOut), true}};
const SFBInterfaceSpec FORTE_WagoMaster::scmFBInterfaceSpec = {1,
                                                               scmEventInputNames,
                                                               scmEventInputTypeIds,
                                                               scmEIWith,
                                                               scmEIWithIndexes,
                                                               2,
                                                               scmEventOutputNames,
                                                               scmEventOutputTypeIds,
                                                               scmEOWith,
                                                               scmEOWithIndexes,
                                                               2,
                                                               scmDataInputNames,
                                                               scmDataInputTypeIds,
                                                               2,
                                                               scmDataOutputNames,
                                                               scmDataOutputTypeIds,
                                                               0,
                                                               nullptr,
                                                               1,
                                                               scmAdapterInstances};

FORTE_WagoMaster::FORTE_WagoMaster(const CStringDictionary::TStringId paInstanceNameId,
                                   forte::core::CFBContainer &paContainer) :
    IOConfigFBMultiMaster(paContainer, scmFBInterfaceSpec, paInstanceNameId),
    var_QI(0_BOOL),
    var_UpdateInterval(25_UINT),
    var_QO(0_BOOL),
    var_STATUS(u""_WSTRING),
    conn_INITO(*this, 0),
    conn_IND(*this, 1),
    conn_QI(nullptr),
    conn_UpdateInterval(nullptr),
    conn_QO(*this, 0, var_QO),
    conn_STATUS(*this, 1, var_STATUS) {};

void FORTE_WagoMaster::setInitialValues() {
  var_QI = 0_BOOL;
  var_UpdateInterval = 25_UINT;
  var_QO = 0_BOOL;
  var_STATUS = u""_WSTRING;
}

void FORTE_WagoMaster::setConfig() {
  WagoDeviceController::WagoConfig config;
  config.updateInterval = static_cast<CIEC_UINT::TValueType>(var_UpdateInterval);
  getDeviceController()->setConfig(&config);
}

forte::core::io::IODeviceController *FORTE_WagoMaster::createDeviceController(CDeviceExecution &paDeviceExecution) {
  return new WagoDeviceController(paDeviceExecution);
}

void FORTE_WagoMaster::readInputData(const TEventID paEIID) {
  switch (paEIID) {
    case scmEventINITID: {
      readData(0, var_QI, conn_QI);
      readData(1, var_UpdateInterval, conn_UpdateInterval);
      break;
    }
    default: break;
  }
}

void FORTE_WagoMaster::writeOutputData(const TEventID paEIID) {
  switch (paEIID) {
    case scmEventINITOID: {
      writeData(0, var_QO, conn_QO);
      writeData(1, var_STATUS, conn_STATUS);
      break;
    }
    case scmEventINDID: {
      writeData(1, var_STATUS, conn_STATUS);
      writeData(0, var_QO, conn_QO);
      break;
    }
    default: break;
  }
}

CIEC_ANY *FORTE_WagoMaster::getDI(const size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_QI;
    case 1: return &var_UpdateInterval;
  }
  return nullptr;
}

CIEC_ANY *FORTE_WagoMaster::getDO(const size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_QO;
    case 1: return &var_STATUS;
  }
  return nullptr;
}

CEventConnection *FORTE_WagoMaster::getEOConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_INITO;
    case 1: return &conn_IND;
  }
  return nullptr;
}

CDataConnection **FORTE_WagoMaster::getDIConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_QI;
    case 1: return &conn_UpdateInterval;
  }
  return nullptr;
}

CDataConnection *FORTE_WagoMaster::getDOConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_QO;
    case 1: return &conn_STATUS;
  }
  return nullptr;
}
