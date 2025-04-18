/*******************************************************************************
 * Copyright (c) 2018, 2024 fortiss GmbH, Monika Wenger, Johannes Kepler University Linz
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v2.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v20.html
 *
 * Contributors:
 *   Jose Cabral - initial implementation
 *   Monika Wenger, Alois Zoitl - migration to current runtime
 *******************************************************************************/

#include "IORevPiAIO.h"

USE_STRING_ID(AnalogInput_1);
USE_STRING_ID(AnalogInput_2);
USE_STRING_ID(AnalogInput_3);
USE_STRING_ID(AnalogInput_4);
USE_STRING_ID(AnalogOutput_1);
USE_STRING_ID(AnalogOutput_2);
USE_STRING_ID(BOOL);
USE_STRING_ID(BusAdapterIn);
USE_STRING_ID(BusAdapterOut);
USE_STRING_ID(Event);
USE_STRING_ID(IND);
USE_STRING_ID(IORevPiAIO);
USE_STRING_ID(IORevPiBusAdapter);
USE_STRING_ID(MAP);
USE_STRING_ID(MAPO);
USE_STRING_ID(QI);
USE_STRING_ID(QO);
USE_STRING_ID(RTD_1);
USE_STRING_ID(RTD_2);
USE_STRING_ID(STATUS);
USE_STRING_ID(STRING);
USE_STRING_ID(WSTRING);

#include "../RevPiController.h"

using namespace forte::core::io;

DEFINE_FIRMWARE_FB(FORTE_IORevPiAIO, STRID(IORevPiAIO))

const CStringDictionary::TStringId FORTE_IORevPiAIO::scmDataInputNames[] = {
    STRID(QI),    STRID(AnalogInput_1), STRID(AnalogInput_2),  STRID(AnalogInput_3), STRID(AnalogInput_4),
    STRID(RTD_1), STRID(RTD_2),         STRID(AnalogOutput_1), STRID(AnalogOutput_2)};
const CStringDictionary::TStringId FORTE_IORevPiAIO::scmDataInputTypeIds[] = {
    STRID(BOOL),   STRID(STRING), STRID(STRING), STRID(STRING), STRID(STRING),
    STRID(STRING), STRID(STRING), STRID(STRING), STRID(STRING)};
const CStringDictionary::TStringId FORTE_IORevPiAIO::scmDataOutputNames[] = {STRID(QO), STRID(STATUS)};
const CStringDictionary::TStringId FORTE_IORevPiAIO::scmDataOutputTypeIds[] = {STRID(BOOL), STRID(WSTRING)};
const TDataIOID FORTE_IORevPiAIO::scmEIWith[] = {1, 2, 5, 3, 4, 6, 7, 8, 0, scmWithListDelimiter};
const TForteInt16 FORTE_IORevPiAIO::scmEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_IORevPiAIO::scmEventInputNames[] = {STRID(MAP)};
const CStringDictionary::TStringId FORTE_IORevPiAIO::scmEventInputTypeIds[] = {STRID(Event)};
const TDataIOID FORTE_IORevPiAIO::scmEOWith[] = {0, scmWithListDelimiter, 0, 1, scmWithListDelimiter};
const TForteInt16 FORTE_IORevPiAIO::scmEOWithIndexes[] = {0, 2};
const CStringDictionary::TStringId FORTE_IORevPiAIO::scmEventOutputNames[] = {STRID(MAPO), STRID(IND)};
const CStringDictionary::TStringId FORTE_IORevPiAIO::scmEventOutputTypeIds[] = {STRID(Event), STRID(Event)};
const SAdapterInstanceDef FORTE_IORevPiAIO::scmAdapterInstances[] = {
    {STRID(IORevPiBusAdapter), STRID(BusAdapterOut), true}, {STRID(IORevPiBusAdapter), STRID(BusAdapterIn), false}};
const SFBInterfaceSpec FORTE_IORevPiAIO::scmFBInterfaceSpec = {1,
                                                               scmEventInputNames,
                                                               scmEventInputTypeIds,
                                                               scmEIWith,
                                                               scmEIWithIndexes,
                                                               2,
                                                               scmEventOutputNames,
                                                               scmEventOutputTypeIds,
                                                               scmEOWith,
                                                               scmEOWithIndexes,
                                                               9,
                                                               scmDataInputNames,
                                                               scmDataInputTypeIds,
                                                               2,
                                                               scmDataOutputNames,
                                                               scmDataOutputTypeIds,
                                                               0,
                                                               nullptr,
                                                               2,
                                                               scmAdapterInstances};

const TForteUInt8 FORTE_IORevPiAIO::scmSlaveConfigurationIO[] = {};
const TForteUInt8 FORTE_IORevPiAIO::scmSlaveConfigurationIONum = 0;

FORTE_IORevPiAIO::FORTE_IORevPiAIO(const CStringDictionary::TStringId paInstanceNameId,
                                   forte::core::CFBContainer &paContainer) :
    IOConfigFBMultiSlave(
        scmSlaveConfigurationIO, scmSlaveConfigurationIONum, 103, paContainer, scmFBInterfaceSpec, paInstanceNameId),
    conn_MAPO(*this, 0),
    conn_IND(*this, 1),
    conn_QI(nullptr),
    conn_AnalogInput_1(nullptr),
    conn_AnalogInput_2(nullptr),
    conn_AnalogInput_3(nullptr),
    conn_AnalogInput_4(nullptr),
    conn_RTD_1(nullptr),
    conn_RTD_2(nullptr),
    conn_AnalogOutput_1(nullptr),
    conn_AnalogOutput_2(nullptr),
    conn_QO(*this, 0, var_QO),
    conn_STATUS(*this, 1, var_STATUS) {

    };

void FORTE_IORevPiAIO::initHandles() {
  uint8_t inputOffset = 0;
  uint8_t outputOffset = 0;

  for (int i = 1; i < 9; i++) {
    uint8_t *currentOffset = (i < 7) ? &inputOffset : &outputOffset;
    RevPiController::HandleDescriptor desc(static_cast<CIEC_STRING *>(getDI(i))->getStorage(),
                                           (i < 7) ? IOMapper::In : IOMapper::Out, mIndex, CIEC_ANY::e_WORD,
                                           *currentOffset, 0);
    initHandle(desc);
    *currentOffset = static_cast<uint8_t>(*currentOffset + 2);
  }
}

void FORTE_IORevPiAIO::setInitialValues() {
  var_QI = 0_BOOL;
  var_AnalogInput_1 = ""_STRING;
  var_AnalogInput_2 = ""_STRING;
  var_AnalogInput_3 = ""_STRING;
  var_AnalogInput_4 = ""_STRING;
  var_RTD_1 = ""_STRING;
  var_RTD_2 = ""_STRING;
  var_AnalogOutput_1 = ""_STRING;
  var_AnalogOutput_2 = ""_STRING;
  var_QO = 0_BOOL;
  var_STATUS = u""_WSTRING;
}

void FORTE_IORevPiAIO::readInputData(const TEventID paEIID) {
  switch (paEIID) {
    case scmEventMAPID: {
      readData(1, var_AnalogInput_1, conn_AnalogInput_1);
      readData(2, var_AnalogInput_2, conn_AnalogInput_2);
      readData(5, var_RTD_1, conn_RTD_1);
      readData(3, var_AnalogInput_3, conn_AnalogInput_3);
      readData(4, var_AnalogInput_4, conn_AnalogInput_4);
      readData(6, var_RTD_2, conn_RTD_2);
      readData(7, var_AnalogOutput_1, conn_AnalogOutput_1);
      readData(8, var_AnalogOutput_2, conn_AnalogOutput_2);
      readData(0, var_QI, conn_QI);
      break;
    }
    default: break;
  }
}

void FORTE_IORevPiAIO::writeOutputData(const TEventID paEIID) {
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

CIEC_ANY *FORTE_IORevPiAIO::getDI(const size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_QI;
    case 1: return &var_AnalogInput_1;
    case 2: return &var_AnalogInput_2;
    case 3: return &var_AnalogInput_3;
    case 4: return &var_AnalogInput_4;
    case 5: return &var_RTD_1;
    case 6: return &var_RTD_2;
    case 7: return &var_AnalogOutput_1;
    case 8: return &var_AnalogOutput_2;
  }
  return nullptr;
}

CIEC_ANY *FORTE_IORevPiAIO::getDO(const size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_QO;
    case 1: return &var_STATUS;
  }
  return nullptr;
}

CEventConnection *FORTE_IORevPiAIO::getEOConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_MAPO;
    case 1: return &conn_IND;
  }
  return nullptr;
}

CDataConnection **FORTE_IORevPiAIO::getDIConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_QI;
    case 1: return &conn_AnalogInput_1;
    case 2: return &conn_AnalogInput_2;
    case 3: return &conn_AnalogInput_3;
    case 4: return &conn_AnalogInput_4;
    case 5: return &conn_RTD_1;
    case 6: return &conn_RTD_2;
    case 7: return &conn_AnalogOutput_1;
    case 8: return &conn_AnalogOutput_2;
  }
  return nullptr;
}

CDataConnection *FORTE_IORevPiAIO::getDOConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_QO;
    case 1: return &conn_STATUS;
  }
  return nullptr;
}
