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

#include "EBSlave2181.h"

USE_STRING_ID(BOOL);
USE_STRING_ID(BusAdapterIn);
USE_STRING_ID(BusAdapterOut);
USE_STRING_ID(DigitalInput_1);
USE_STRING_ID(DigitalInput_2);
USE_STRING_ID(DigitalInput_3);
USE_STRING_ID(DigitalInput_4);
USE_STRING_ID(DigitalInput_5);
USE_STRING_ID(DigitalInput_6);
USE_STRING_ID(DigitalInput_7);
USE_STRING_ID(DigitalInput_8);
USE_STRING_ID(DigitalOutput_1);
USE_STRING_ID(DigitalOutput_2);
USE_STRING_ID(DigitalOutput_3);
USE_STRING_ID(DigitalOutput_4);
USE_STRING_ID(DigitalOutput_5);
USE_STRING_ID(DigitalOutput_6);
USE_STRING_ID(DigitalOutput_7);
USE_STRING_ID(DigitalOutput_8);
USE_STRING_ID(EBBusAdapter);
USE_STRING_ID(EBSlave2181);
USE_STRING_ID(IND);
USE_STRING_ID(MAP);
USE_STRING_ID(MAPO);
USE_STRING_ID(QI);
USE_STRING_ID(QO);
USE_STRING_ID(STATUS);
USE_STRING_ID(UINT);
USE_STRING_ID(UpdateInterval);
USE_STRING_ID(WSTRING);

#include "../handler/bus.h"

DEFINE_FIRMWARE_FB(FORTE_EBSlave2181, STRID(EBSlave2181))

const CStringDictionary::TStringId FORTE_EBSlave2181::scmDataInputNames[] = {STRID(QI),
                                                                             STRID(DigitalInput_1),
                                                                             STRID(DigitalInput_2),
                                                                             STRID(DigitalInput_3),
                                                                             STRID(DigitalInput_4),
                                                                             STRID(DigitalInput_5),
                                                                             STRID(DigitalInput_6),
                                                                             STRID(DigitalInput_7),
                                                                             STRID(DigitalInput_8),
                                                                             STRID(DigitalOutput_1),
                                                                             STRID(DigitalOutput_2),
                                                                             STRID(DigitalOutput_3),
                                                                             STRID(DigitalOutput_4),
                                                                             STRID(DigitalOutput_5),
                                                                             STRID(DigitalOutput_6),
                                                                             STRID(DigitalOutput_7),
                                                                             STRID(DigitalOutput_8),
                                                                             STRID(UpdateInterval)};
const CStringDictionary::TStringId FORTE_EBSlave2181::scmDataInputTypeIds[] = {
    STRID(BOOL),    STRID(WSTRING), STRID(WSTRING), STRID(WSTRING), STRID(WSTRING), STRID(WSTRING),
    STRID(WSTRING), STRID(WSTRING), STRID(WSTRING), STRID(WSTRING), STRID(WSTRING), STRID(WSTRING),
    STRID(WSTRING), STRID(WSTRING), STRID(WSTRING), STRID(WSTRING), STRID(WSTRING), STRID(UINT)};
const CStringDictionary::TStringId FORTE_EBSlave2181::scmDataOutputNames[] = {STRID(QO), STRID(STATUS)};
const CStringDictionary::TStringId FORTE_EBSlave2181::scmDataOutputTypeIds[] = {STRID(BOOL), STRID(WSTRING)};
const TDataIOID FORTE_EBSlave2181::scmEIWith[] = {1,  2,  5,  3,  4,  6,  7,  8, 9,
                                                  10, 11, 12, 13, 14, 15, 16, 0, scmWithListDelimiter};
const TForteInt16 FORTE_EBSlave2181::scmEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_EBSlave2181::scmEventInputNames[] = {STRID(MAP)};
const TDataIOID FORTE_EBSlave2181::scmEOWith[] = {0, scmWithListDelimiter, 0, 1, scmWithListDelimiter};
const TForteInt16 FORTE_EBSlave2181::scmEOWithIndexes[] = {0, 2};
const CStringDictionary::TStringId FORTE_EBSlave2181::scmEventOutputNames[] = {STRID(MAPO), STRID(IND)};
const SAdapterInstanceDef FORTE_EBSlave2181::scmAdapterInstances[] = {
    {STRID(EBBusAdapter), STRID(BusAdapterIn), false}, {STRID(EBBusAdapter), STRID(BusAdapterOut), true}};
const SFBInterfaceSpec FORTE_EBSlave2181::scmFBInterfaceSpec = {1,
                                                                scmEventInputNames,
                                                                nullptr,
                                                                scmEIWith,
                                                                scmEIWithIndexes,
                                                                2,
                                                                scmEventOutputNames,
                                                                nullptr,
                                                                scmEOWith,
                                                                scmEOWithIndexes,
                                                                18,
                                                                scmDataInputNames,
                                                                scmDataInputTypeIds,
                                                                2,
                                                                scmDataOutputNames,
                                                                scmDataOutputTypeIds,
                                                                0,
                                                                nullptr,
                                                                2,
                                                                scmAdapterInstances};

FORTE_EBSlave2181::FORTE_EBSlave2181(const CStringDictionary::TStringId paInstanceNameId,
                                     forte::core::CFBContainer &paContainer) :
    EmbrickSlave(scmSlaveConfigurationIO,
                 scmSlaveConfigurationIONum,
                 EmbrickSlaveHandler::G_8Di8Do,
                 paContainer,
                 scmFBInterfaceSpec,
                 paInstanceNameId),
    conn_MAPO(*this, 0),
    conn_IND(*this, 1),
    conn_QI(nullptr),
    conn_DigitalInput_1(nullptr),
    conn_DigitalInput_2(nullptr),
    conn_DigitalInput_3(nullptr),
    conn_DigitalInput_4(nullptr),
    conn_DigitalInput_5(nullptr),
    conn_DigitalInput_6(nullptr),
    conn_DigitalInput_7(nullptr),
    conn_DigitalInput_8(nullptr),
    conn_DigitalOutput_1(nullptr),
    conn_DigitalOutput_2(nullptr),
    conn_DigitalOutput_3(nullptr),
    conn_DigitalOutput_4(nullptr),
    conn_DigitalOutput_5(nullptr),
    conn_DigitalOutput_6(nullptr),
    conn_DigitalOutput_7(nullptr),
    conn_DigitalOutput_8(nullptr),
    conn_UpdateInterval(nullptr),
    conn_QO(*this, 0, var_QO),
    conn_STATUS(*this, 1, var_STATUS) {};

void FORTE_EBSlave2181::setInitialValues() {
  var_QI = 0_BOOL;
  var_DigitalInput_1 = u""_WSTRING;
  var_DigitalInput_2 = u""_WSTRING;
  var_DigitalInput_3 = u""_WSTRING;
  var_DigitalInput_4 = u""_WSTRING;
  var_DigitalInput_5 = u""_WSTRING;
  var_DigitalInput_6 = u""_WSTRING;
  var_DigitalInput_7 = u""_WSTRING;
  var_DigitalInput_8 = u""_WSTRING;
  var_DigitalOutput_1 = u""_WSTRING;
  var_DigitalOutput_2 = u""_WSTRING;
  var_DigitalOutput_3 = u""_WSTRING;
  var_DigitalOutput_4 = u""_WSTRING;
  var_DigitalOutput_5 = u""_WSTRING;
  var_DigitalOutput_6 = u""_WSTRING;
  var_DigitalOutput_7 = u""_WSTRING;
  var_DigitalOutput_8 = u""_WSTRING;
  var_UpdateInterval = 0_UINT;
  var_QO = 0_BOOL;
  var_STATUS = u""_WSTRING;
}

void FORTE_EBSlave2181::readInputData(const TEventID paEIID) {
  switch (paEIID) {
    case scmEventMAPID: {
      readData(1, var_DigitalInput_1, conn_DigitalInput_1);
      readData(2, var_DigitalInput_2, conn_DigitalInput_2);
      readData(5, var_DigitalInput_5, conn_DigitalInput_5);
      readData(3, var_DigitalInput_3, conn_DigitalInput_3);
      readData(4, var_DigitalInput_4, conn_DigitalInput_4);
      readData(6, var_DigitalInput_6, conn_DigitalInput_6);
      readData(7, var_DigitalInput_7, conn_DigitalInput_7);
      readData(8, var_DigitalInput_8, conn_DigitalInput_8);
      readData(9, var_DigitalOutput_1, conn_DigitalOutput_1);
      readData(10, var_DigitalOutput_2, conn_DigitalOutput_2);
      readData(11, var_DigitalOutput_3, conn_DigitalOutput_3);
      readData(12, var_DigitalOutput_4, conn_DigitalOutput_4);
      readData(13, var_DigitalOutput_5, conn_DigitalOutput_5);
      readData(14, var_DigitalOutput_6, conn_DigitalOutput_6);
      readData(15, var_DigitalOutput_7, conn_DigitalOutput_7);
      readData(16, var_DigitalOutput_8, conn_DigitalOutput_8);
      readData(0, var_QI, conn_QI);
      break;
    }
    default: break;
  }
}

void FORTE_EBSlave2181::writeOutputData(const TEventID paEIID) {
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

CIEC_ANY *FORTE_EBSlave2181::getDI(const size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_QI;
    case 1: return &var_DigitalInput_1;
    case 2: return &var_DigitalInput_2;
    case 3: return &var_DigitalInput_3;
    case 4: return &var_DigitalInput_4;
    case 5: return &var_DigitalInput_5;
    case 6: return &var_DigitalInput_6;
    case 7: return &var_DigitalInput_7;
    case 8: return &var_DigitalInput_8;
    case 9: return &var_DigitalOutput_1;
    case 10: return &var_DigitalOutput_2;
    case 11: return &var_DigitalOutput_3;
    case 12: return &var_DigitalOutput_4;
    case 13: return &var_DigitalOutput_5;
    case 14: return &var_DigitalOutput_6;
    case 15: return &var_DigitalOutput_7;
    case 16: return &var_DigitalOutput_8;
    case 17: return &var_UpdateInterval;
  }
  return nullptr;
}

CIEC_ANY *FORTE_EBSlave2181::getDO(const size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_QO;
    case 1: return &var_STATUS;
  }
  return nullptr;
}

CEventConnection *FORTE_EBSlave2181::getEOConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_MAPO;
    case 1: return &conn_IND;
  }
  return nullptr;
}

CDataConnection **FORTE_EBSlave2181::getDIConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_QI;
    case 1: return &conn_DigitalInput_1;
    case 2: return &conn_DigitalInput_2;
    case 3: return &conn_DigitalInput_3;
    case 4: return &conn_DigitalInput_4;
    case 5: return &conn_DigitalInput_5;
    case 6: return &conn_DigitalInput_6;
    case 7: return &conn_DigitalInput_7;
    case 8: return &conn_DigitalInput_8;
    case 9: return &conn_DigitalOutput_1;
    case 10: return &conn_DigitalOutput_2;
    case 11: return &conn_DigitalOutput_3;
    case 12: return &conn_DigitalOutput_4;
    case 13: return &conn_DigitalOutput_5;
    case 14: return &conn_DigitalOutput_6;
    case 15: return &conn_DigitalOutput_7;
    case 16: return &conn_DigitalOutput_8;
    case 17: return &conn_UpdateInterval;
  }
  return nullptr;
}

CDataConnection *FORTE_EBSlave2181::getDOConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_QO;
    case 1: return &conn_STATUS;
  }
  return nullptr;
}

const TForteUInt8 FORTE_EBSlave2181::scmSlaveConfigurationIO[] = {17};
const TForteUInt8 FORTE_EBSlave2181::scmSlaveConfigurationIONum = 1;

void FORTE_EBSlave2181::initHandles() {
  // Initialize handles
  int iCount = 8;
  int oCount = 8;
  int iOffset = 1;
  int oOffset = iOffset + iCount;

  for (int i = 0; i < iCount; i++) {
    EmbrickBusHandler::HandleDescriptor desc = EmbrickBusHandler::HandleDescriptor(
        static_cast<CIEC_WSTRING *>(getDI(iOffset + i))->getValue(), forte::core::io::IOMapper::In, mIndex,
        EmbrickBusHandler::Bit, (uint8_t) (i / 8), (uint8_t) (i % 8));
    initHandle(desc);
  }

  for (int i = 0; i < oCount; i++) {
    EmbrickBusHandler::HandleDescriptor desc = EmbrickBusHandler::HandleDescriptor(
        static_cast<CIEC_WSTRING *>(getDI(oOffset + i))->getValue(), forte::core::io::IOMapper::Out, mIndex,
        EmbrickBusHandler::Bit, (uint8_t) (i / 8), (uint8_t) (i % 8));
    initHandle(desc);
  }
}
