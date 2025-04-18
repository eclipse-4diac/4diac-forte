/*******************************************************************************
 * Copyright (c) 2016 - 2018 Johannes Messmer (admin@jomess.com), fortiss GmbH
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

#include "EBSlave2301.h"

USE_STRING_ID(BOOL);
USE_STRING_ID(BusAdapterIn);
USE_STRING_ID(BusAdapterOut);
USE_STRING_ID(EBBusAdapter);
USE_STRING_ID(EBSlave2301);
USE_STRING_ID(IND);
USE_STRING_ID(MAP);
USE_STRING_ID(MAPO);
USE_STRING_ID(QI);
USE_STRING_ID(QO);
USE_STRING_ID(Relay_1);
USE_STRING_ID(Relay_2);
USE_STRING_ID(Relay_3);
USE_STRING_ID(Relay_4);
USE_STRING_ID(Relay_5);
USE_STRING_ID(Relay_6);
USE_STRING_ID(STATUS);
USE_STRING_ID(UINT);
USE_STRING_ID(UpdateInterval);
USE_STRING_ID(WSTRING);

#include "criticalregion.h"
#include "resource.h"
#include "../handler/bus.h"

DEFINE_FIRMWARE_FB(FORTE_EBSlave2301, STRID(EBSlave2301))

const CStringDictionary::TStringId FORTE_EBSlave2301::scmDataInputNames[] = {
    STRID(QI),      STRID(Relay_1), STRID(Relay_2), STRID(Relay_3),
    STRID(Relay_4), STRID(Relay_5), STRID(Relay_6), STRID(UpdateInterval)};
const CStringDictionary::TStringId FORTE_EBSlave2301::scmDataInputTypeIds[] = {
    STRID(BOOL),    STRID(WSTRING), STRID(WSTRING), STRID(WSTRING),
    STRID(WSTRING), STRID(WSTRING), STRID(WSTRING), STRID(UINT)};
const CStringDictionary::TStringId FORTE_EBSlave2301::scmDataOutputNames[] = {STRID(QO), STRID(STATUS)};
const CStringDictionary::TStringId FORTE_EBSlave2301::scmDataOutputTypeIds[] = {STRID(BOOL), STRID(WSTRING)};
const TDataIOID FORTE_EBSlave2301::scmEIWith[] = {1, 2, 3, 4, 5, 6, 0, scmWithListDelimiter};
const TForteInt16 FORTE_EBSlave2301::scmEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_EBSlave2301::scmEventInputNames[] = {STRID(MAP)};
const TDataIOID FORTE_EBSlave2301::scmEOWith[] = {0, scmWithListDelimiter, 0, 1, scmWithListDelimiter};
const TForteInt16 FORTE_EBSlave2301::scmEOWithIndexes[] = {0, 2};
const CStringDictionary::TStringId FORTE_EBSlave2301::scmEventOutputNames[] = {STRID(MAPO), STRID(IND)};
const SAdapterInstanceDef FORTE_EBSlave2301::scmAdapterInstances[] = {
    {STRID(EBBusAdapter), STRID(BusAdapterIn), false}, {STRID(EBBusAdapter), STRID(BusAdapterOut), true}};
const SFBInterfaceSpec FORTE_EBSlave2301::scmFBInterfaceSpec = {1,
                                                                scmEventInputNames,
                                                                nullptr,
                                                                scmEIWith,
                                                                scmEIWithIndexes,
                                                                2,
                                                                scmEventOutputNames,
                                                                nullptr,
                                                                scmEOWith,
                                                                scmEOWithIndexes,
                                                                8,
                                                                scmDataInputNames,
                                                                scmDataInputTypeIds,
                                                                2,
                                                                scmDataOutputNames,
                                                                scmDataOutputTypeIds,
                                                                0,
                                                                nullptr,
                                                                2,
                                                                scmAdapterInstances};

FORTE_EBSlave2301::FORTE_EBSlave2301(const CStringDictionary::TStringId paInstanceNameId,
                                     forte::core::CFBContainer &paContainer) :
    EmbrickSlave(scmSlaveConfigurationIO,
                 scmSlaveConfigurationIONum,
                 EmbrickSlaveHandler::G_2RelNo4RelCo,
                 paContainer,
                 scmFBInterfaceSpec,
                 paInstanceNameId),
    conn_MAPO(*this, 0),
    conn_IND(*this, 1),
    conn_QI(nullptr),
    conn_Relay_1(nullptr),
    conn_Relay_2(nullptr),
    conn_Relay_3(nullptr),
    conn_Relay_4(nullptr),
    conn_Relay_5(nullptr),
    conn_Relay_6(nullptr),
    conn_UpdateInterval(nullptr),
    conn_QO(*this, 0, var_QO),
    conn_STATUS(*this, 1, var_STATUS) {};

void FORTE_EBSlave2301::setInitialValues() {
  var_QI = 0_BOOL;
  var_Relay_1 = u""_WSTRING;
  var_Relay_2 = u""_WSTRING;
  var_Relay_3 = u""_WSTRING;
  var_Relay_4 = u""_WSTRING;
  var_Relay_5 = u""_WSTRING;
  var_Relay_6 = u""_WSTRING;
  var_UpdateInterval = 0_UINT;
  var_QO = 0_BOOL;
  var_STATUS = u""_WSTRING;
}

void FORTE_EBSlave2301::readInputData(const TEventID paEIID) {
  switch (paEIID) {
    case scmEventMAPID: {
      readData(1, var_Relay_1, conn_Relay_1);
      readData(2, var_Relay_2, conn_Relay_2);
      readData(3, var_Relay_3, conn_Relay_3);
      readData(4, var_Relay_4, conn_Relay_4);
      readData(5, var_Relay_5, conn_Relay_5);
      readData(6, var_Relay_6, conn_Relay_6);
      readData(0, var_QI, conn_QI);
      break;
    }
    default: break;
  }
}

void FORTE_EBSlave2301::writeOutputData(const TEventID paEIID) {
  switch (paEIID) {
    case scmEventMAPOID: {
      writeData(0, var_QO, conn_QO);
      break;
    }
    case scmEventINDID: {
      writeData(0, var_QO, conn_QO);
      writeData(1, var_STATUS, conn_STATUS);
      break;
    }
    default: break;
  }
}

CIEC_ANY *FORTE_EBSlave2301::getDI(const size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_QI;
    case 1: return &var_Relay_1;
    case 2: return &var_Relay_2;
    case 3: return &var_Relay_3;
    case 4: return &var_Relay_4;
    case 5: return &var_Relay_5;
    case 6: return &var_Relay_6;
    case 7: return &var_UpdateInterval;
  }
  return nullptr;
}

CIEC_ANY *FORTE_EBSlave2301::getDO(const size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_QO;
    case 1: return &var_STATUS;
  }
  return nullptr;
}

CEventConnection *FORTE_EBSlave2301::getEOConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_MAPO;
    case 1: return &conn_IND;
  }
  return nullptr;
}

CDataConnection **FORTE_EBSlave2301::getDIConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_QI;
    case 1: return &conn_Relay_1;
    case 2: return &conn_Relay_2;
    case 3: return &conn_Relay_3;
    case 4: return &conn_Relay_4;
    case 5: return &conn_Relay_5;
    case 6: return &conn_Relay_6;
    case 7: return &conn_UpdateInterval;
  }
  return nullptr;
}

CDataConnection *FORTE_EBSlave2301::getDOConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_QO;
    case 1: return &conn_STATUS;
  }
  return nullptr;
}

const TForteUInt8 FORTE_EBSlave2301::scmSlaveConfigurationIO[] = {7};
const TForteUInt8 FORTE_EBSlave2301::scmSlaveConfigurationIONum = 1;

void FORTE_EBSlave2301::initHandles() {
  // Initialize handles
  int iCount = 0;
  int oCount = 6;
  int iOffset = 1;
  int oOffset = iOffset + iCount;

  for (int i = 0; i < oCount; i++) {
    EmbrickBusHandler::HandleDescriptor desc = EmbrickBusHandler::HandleDescriptor(
        static_cast<CIEC_WSTRING *>(getDI(oOffset + i))->getValue(), forte::core::io::IOMapper::Out, mIndex,
        EmbrickBusHandler::Bit, (uint8_t) (i / 8), (uint8_t) (i % 8));
    initHandle(desc);
  }
}
