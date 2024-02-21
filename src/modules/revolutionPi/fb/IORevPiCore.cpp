/*******************************************************************************
 * Copyright (c) 2017 fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Johannes Messmer - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "IORevPiCore.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "IORevPiCore_gen.cpp"
#endif

#include "../RevPiController.h"

DEFINE_FIRMWARE_FB(FORTE_IORevPiCore, g_nStringIdIORevPiCore)

const CStringDictionary::TStringId FORTE_IORevPiCore::scmDataInputNames[] = {g_nStringIdQI, g_nStringIdUpdateInterval};
const CStringDictionary::TStringId FORTE_IORevPiCore::scmDataInputTypeIds[] = {g_nStringIdBOOL, g_nStringIdUINT};
const CStringDictionary::TStringId FORTE_IORevPiCore::scmDataOutputNames[] = {g_nStringIdQO, g_nStringIdSTATUS};
const CStringDictionary::TStringId FORTE_IORevPiCore::scmDataOutputTypeIds[] = {g_nStringIdBOOL, g_nStringIdWSTRING};
const TDataIOID FORTE_IORevPiCore::scmEIWith[] = {0, 1, scmWithListDelimiter};
const TForteInt16 FORTE_IORevPiCore::scmEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_IORevPiCore::scmEventInputNames[] = {g_nStringIdINIT};
const TDataIOID FORTE_IORevPiCore::scmEOWith[] = {0, 1, scmWithListDelimiter, 0, 1, scmWithListDelimiter};
const TForteInt16 FORTE_IORevPiCore::scmEOWithIndexes[] = {0, 3};
const CStringDictionary::TStringId FORTE_IORevPiCore::scmEventOutputNames[] = {g_nStringIdINITO, g_nStringIdIND};
const SAdapterInstanceDef FORTE_IORevPiCore::scmAdapterInstances[] = {
  {g_nStringIdIORevPiBusAdapter, g_nStringIdBusAdapterOut, true}
};
const SFBInterfaceSpec FORTE_IORevPiCore::scmFBInterfaceSpec = {
  1, scmEventInputNames, scmEIWith, scmEIWithIndexes,
  2, scmEventOutputNames, scmEOWith, scmEOWithIndexes,
  2, scmDataInputNames, scmDataInputTypeIds,
  2, scmDataOutputNames, scmDataOutputTypeIds,
  0, nullptr,
  1, scmAdapterInstances
};

void FORTE_IORevPiCore::setInitialValues() {
  var_QI = 0_BOOL;
  var_UpdateInterval = 25_UINT;
  var_QO = 0_BOOL;
  var_STATUS = u""_WSTRING;
}

void FORTE_IORevPiCore::setConfig() {
  RevPiController::Config config;
  config.updateInterval = var_UpdateInterval.getUnsignedValue();
  getDeviceController()->setConfig(&config);
}

forte::core::io::IODeviceController* FORTE_IORevPiCore::createDeviceController(CDeviceExecution& paDeviceExecution) {
  return new RevPiController(paDeviceExecution);
}

void FORTE_IORevPiCore::readInputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventINITID: {
      readData(0, var_QI, conn_QI);
      readData(1, var_UpdateInterval, conn_UpdateInterval);
      break;
    }
    default:
      break;
  }
}

void FORTE_IORevPiCore::writeOutputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventINITOID: {
      writeData(0, var_QO, conn_QO);
      writeData(1, var_STATUS, conn_STATUS);
      break;
    }
    case scmEventINDID: {
      writeData(0, var_QO, conn_QO);
      writeData(1, var_STATUS, conn_STATUS);
      break;
    }
    default:
      break;
  }
}

CIEC_ANY *FORTE_IORevPiCore::getDI(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_QI;
    case 1: return &var_UpdateInterval;
  }
  return nullptr;
}

CIEC_ANY *FORTE_IORevPiCore::getDO(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_QO;
    case 1: return &var_STATUS;
  }
  return nullptr;
}

CEventConnection *FORTE_IORevPiCore::getEOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_INITO;
    case 1: return &conn_IND;
  }
  return nullptr;
}

CDataConnection **FORTE_IORevPiCore::getDIConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_QI;
    case 1: return &conn_UpdateInterval;
  }
  return nullptr;
}

CDataConnection *FORTE_IORevPiCore::getDOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_QO;
    case 1: return &conn_STATUS;
  }
  return nullptr;
}
