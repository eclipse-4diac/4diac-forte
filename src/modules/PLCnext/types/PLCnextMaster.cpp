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

const CStringDictionary::TStringId FORTE_PLCnextMaster::scmDataInputNames[] = {STRID(QI), STRID(SlaveUpdateInterval)};
const CStringDictionary::TStringId FORTE_PLCnextMaster::scmDataInputTypeIds[] = {STRID(BOOL), STRID(UINT)};
const CStringDictionary::TStringId FORTE_PLCnextMaster::scmDataOutputNames[] = {STRID(QO), STRID(STATUS)};
const CStringDictionary::TStringId FORTE_PLCnextMaster::scmDataOutputTypeIds[] = {STRID(BOOL), STRID(WSTRING)};
const TDataIOID FORTE_PLCnextMaster::scmEIWith[] = {0, 1, scmWithListDelimiter};
const TForteInt16 FORTE_PLCnextMaster::scmEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_PLCnextMaster::scmEventInputNames[] = {STRID(INIT)};
const CStringDictionary::TStringId FORTE_PLCnextMaster::scmEventInputTypeIds[] = {STRID(EInit)};
const TDataIOID FORTE_PLCnextMaster::scmEOWith[] = {0, 1, scmWithListDelimiter, 0, 1, scmWithListDelimiter};
const TForteInt16 FORTE_PLCnextMaster::scmEOWithIndexes[] = {0, 3};
const CStringDictionary::TStringId FORTE_PLCnextMaster::scmEventOutputNames[] = {STRID(INITO), STRID(IND)};
const CStringDictionary::TStringId FORTE_PLCnextMaster::scmEventOutputTypeIds[] = {STRID(EInit), STRID(Event)};
const SAdapterInstanceDef FORTE_PLCnextMaster::scmAdapterInstances[] = {
    {STRID(PLCnextBusAdapter), STRID(BusAdapterOut), true}};

namespace {
  const auto cPlugNameIds = std::array{STRID(BusAdapterOut)};
} // namespace

const SFBInterfaceSpec FORTE_PLCnextMaster::scmFBInterfaceSpec = {1,
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
                                                                  scmAdapterInstances,
                                                                  {},
                                                                  cPlugNameIds};

FORTE_PLCnextMaster::FORTE_PLCnextMaster(const CStringDictionary::TStringId paInstanceNameId,
                                         forte::core::CFBContainer &paContainer) :
    forte::core::io::IOConfigFBMultiMaster(paContainer, scmFBInterfaceSpec, paInstanceNameId),
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
      writeData(scmFBInterfaceSpec.mNumDIs + 0, var_QO, conn_QO);
      writeData(scmFBInterfaceSpec.mNumDIs + 1, var_STATUS, conn_STATUS);
      break;
    }
    case scmEventINDID: {
      writeData(scmFBInterfaceSpec.mNumDIs + 0, var_QO, conn_QO);
      writeData(scmFBInterfaceSpec.mNumDIs + 1, var_STATUS, conn_STATUS);
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
