/*************************************************************************
 *** Copyright (c) 2018, 2025 HR Agrartechnik GmbH, Jose Cabral
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
 *** Name: logiBUS_8_DI_UP
 *** Description: Button released --> IE
 *** Version:
 ***     1.0: 2018-12-05/Jose Cabral -  -
 ***     1.1: 2022-08-04/Franz Höpfinger - HR Agrartechnik GmbH -
 ***     1.2: 2023-01-05/Franz Höpfinger - HR Agrartechnik GmbH -
 ***     1.3: 2025-02-18/Franz Höpfinger - HR Agrartechnik GmbH - logiBUS IO created.
 *************************************************************************/

#include "logiBUS_8_DI_UP_fbt.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "logiBUS_8_DI_UP_fbt_gen.cpp"
#endif

#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"

DEFINE_FIRMWARE_FB(FORTE_logiBUS_8_DI_UP, g_nStringIdlogiBUS_8_DI_UP)

const CStringDictionary::TStringId FORTE_logiBUS_8_DI_UP::scmDataInputNames[] = {g_nStringIdQI, g_nStringIdDigitalInput_I1, g_nStringIdDigitalInput_I2, g_nStringIdDigitalInput_I3, g_nStringIdDigitalInput_I4, g_nStringIdDigitalInput_I5, g_nStringIdDigitalInput_I6, g_nStringIdDigitalInput_I7, g_nStringIdDigitalInput_I8};
const CStringDictionary::TStringId FORTE_logiBUS_8_DI_UP::scmDataInputTypeIds[] = {g_nStringIdBOOL, g_nStringIdSTRING, g_nStringIdSTRING, g_nStringIdSTRING, g_nStringIdSTRING, g_nStringIdSTRING, g_nStringIdSTRING, g_nStringIdSTRING, g_nStringIdSTRING};
const CStringDictionary::TStringId FORTE_logiBUS_8_DI_UP::scmDataOutputNames[] = {g_nStringIdQO, g_nStringIdSTATUS};
const CStringDictionary::TStringId FORTE_logiBUS_8_DI_UP::scmDataOutputTypeIds[] = {g_nStringIdBOOL, g_nStringIdSTRING};
const TDataIOID FORTE_logiBUS_8_DI_UP::scmEIWith[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, scmWithListDelimiter};
const TForteInt16 FORTE_logiBUS_8_DI_UP::scmEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_logiBUS_8_DI_UP::scmEventInputNames[] = {g_nStringIdINIT};
const CStringDictionary::TStringId FORTE_logiBUS_8_DI_UP::scmEventInputTypeIds[] = {g_nStringIdEInit};
const TDataIOID FORTE_logiBUS_8_DI_UP::scmEOWith[] = {0, scmWithListDelimiter, 0, 1, scmWithListDelimiter};
const TForteInt16 FORTE_logiBUS_8_DI_UP::scmEOWithIndexes[] = {0, 2};
const CStringDictionary::TStringId FORTE_logiBUS_8_DI_UP::scmEventOutputNames[] = {g_nStringIdINITO, g_nStringIdIND};
const CStringDictionary::TStringId FORTE_logiBUS_8_DI_UP::scmEventOutputTypeIds[] = {g_nStringIdEInit, g_nStringIdEvent};
const SFBInterfaceSpec FORTE_logiBUS_8_DI_UP::scmFBInterfaceSpec = {
  1, scmEventInputNames, scmEventInputTypeIds, scmEIWith, scmEIWithIndexes,
  2, scmEventOutputNames, scmEventOutputTypeIds, scmEOWith, scmEOWithIndexes,
  9, scmDataInputNames, scmDataInputTypeIds,
  2, scmDataOutputNames, scmDataOutputTypeIds,
  0, nullptr,
  0, nullptr
};

FORTE_logiBUS_8_DI_UP::FORTE_logiBUS_8_DI_UP(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    logiBUSIOFunctionBlock(paContainer, scmFBInterfaceSpec, paInstanceNameId),
    var_QI(0_BOOL),
    var_DigitalInput_I1(""_STRING),
    var_DigitalInput_I2(""_STRING),
    var_DigitalInput_I3(""_STRING),
    var_DigitalInput_I4(""_STRING),
    var_DigitalInput_I5(""_STRING),
    var_DigitalInput_I6(""_STRING),
    var_DigitalInput_I7(""_STRING),
    var_DigitalInput_I8(""_STRING),
    var_QO(0_BOOL),
    var_STATUS(""_STRING),
    var_conn_QO(var_QO),
    var_conn_STATUS(var_STATUS),
    conn_INITO(this, 0),
    conn_IND(this, 1),
    conn_QI(nullptr),
    conn_DigitalInput_I1(nullptr),
    conn_DigitalInput_I2(nullptr),
    conn_DigitalInput_I3(nullptr),
    conn_DigitalInput_I4(nullptr),
    conn_DigitalInput_I5(nullptr),
    conn_DigitalInput_I6(nullptr),
    conn_DigitalInput_I7(nullptr),
    conn_DigitalInput_I8(nullptr),
    conn_QO(this, 0, &var_conn_QO),
    conn_STATUS(this, 1, &var_conn_STATUS) {
};

void FORTE_logiBUS_8_DI_UP::setInitialValues() {
  var_QI = 0_BOOL;
  var_DigitalInput_I1 = ""_STRING;
  var_DigitalInput_I2 = ""_STRING;
  var_DigitalInput_I3 = ""_STRING;
  var_DigitalInput_I4 = ""_STRING;
  var_DigitalInput_I5 = ""_STRING;
  var_DigitalInput_I6 = ""_STRING;
  var_DigitalInput_I7 = ""_STRING;
  var_DigitalInput_I8 = ""_STRING;
  var_QO = 0_BOOL;
  var_STATUS = ""_STRING;
}

void FORTE_logiBUS_8_DI_UP::executeEvent(const TEventID paEIID, CEventChainExecutionThread *const paECET) {
  switch(paEIID) {
    case scmEventINITID:
      var_QO = var_QI;
      //TODO !! avoid 2 identical u16ObjId's !!!!
      handle_I1 = std::unique_ptr<forte::core::io::IOHandle>(mapPin(var_DigitalInput_I1.getStorage(), forte::core::io::IOMapper::In, CIEC_ANY::e_Max, logiBUSIODeviceController::HandleType::ESP32Button_UP, logiBUSIOEnums::DigitalInPinNumber::PinI1));
      handle_I2 = std::unique_ptr<forte::core::io::IOHandle>(mapPin(var_DigitalInput_I2.getStorage(), forte::core::io::IOMapper::In, CIEC_ANY::e_Max, logiBUSIODeviceController::HandleType::ESP32Button_UP, logiBUSIOEnums::DigitalInPinNumber::PinI2));
      handle_I3 = std::unique_ptr<forte::core::io::IOHandle>(mapPin(var_DigitalInput_I3.getStorage(), forte::core::io::IOMapper::In, CIEC_ANY::e_Max, logiBUSIODeviceController::HandleType::ESP32Button_UP, logiBUSIOEnums::DigitalInPinNumber::PinI3));
      handle_I4 = std::unique_ptr<forte::core::io::IOHandle>(mapPin(var_DigitalInput_I4.getStorage(), forte::core::io::IOMapper::In, CIEC_ANY::e_Max, logiBUSIODeviceController::HandleType::ESP32Button_UP, logiBUSIOEnums::DigitalInPinNumber::PinI4));
      handle_I5 = std::unique_ptr<forte::core::io::IOHandle>(mapPin(var_DigitalInput_I5.getStorage(), forte::core::io::IOMapper::In, CIEC_ANY::e_Max, logiBUSIODeviceController::HandleType::ESP32Button_UP, logiBUSIOEnums::DigitalInPinNumber::PinI5));
      handle_I6 = std::unique_ptr<forte::core::io::IOHandle>(mapPin(var_DigitalInput_I6.getStorage(), forte::core::io::IOMapper::In, CIEC_ANY::e_Max, logiBUSIODeviceController::HandleType::ESP32Button_UP, logiBUSIOEnums::DigitalInPinNumber::PinI6));
      handle_I7 = std::unique_ptr<forte::core::io::IOHandle>(mapPin(var_DigitalInput_I7.getStorage(), forte::core::io::IOMapper::In, CIEC_ANY::e_Max, logiBUSIODeviceController::HandleType::ESP32Button_UP, logiBUSIOEnums::DigitalInPinNumber::PinI7));
      handle_I8 = std::unique_ptr<forte::core::io::IOHandle>(mapPin(var_DigitalInput_I8.getStorage(), forte::core::io::IOMapper::In, CIEC_ANY::e_Max, logiBUSIODeviceController::HandleType::ESP32Button_UP, logiBUSIOEnums::DigitalInPinNumber::PinI8));

      sendOutputEvent(scmEventINITOID, paECET);
      break;
  }
}

void FORTE_logiBUS_8_DI_UP::readInputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventINITID: {
      readData(0, var_QI, conn_QI);
      readData(1, var_DigitalInput_I1, conn_DigitalInput_I1);
      readData(2, var_DigitalInput_I2, conn_DigitalInput_I2);
      readData(3, var_DigitalInput_I3, conn_DigitalInput_I3);
      readData(4, var_DigitalInput_I4, conn_DigitalInput_I4);
      readData(5, var_DigitalInput_I5, conn_DigitalInput_I5);
      readData(6, var_DigitalInput_I6, conn_DigitalInput_I6);
      readData(7, var_DigitalInput_I7, conn_DigitalInput_I7);
      readData(8, var_DigitalInput_I8, conn_DigitalInput_I8);
      break;
    }
    default:
      break;
  }
}

void FORTE_logiBUS_8_DI_UP::writeOutputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventINITOID: {
      writeData(0, var_QO, conn_QO);
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

CIEC_ANY *FORTE_logiBUS_8_DI_UP::getDI(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_QI;
    case 1: return &var_DigitalInput_I1;
    case 2: return &var_DigitalInput_I2;
    case 3: return &var_DigitalInput_I3;
    case 4: return &var_DigitalInput_I4;
    case 5: return &var_DigitalInput_I5;
    case 6: return &var_DigitalInput_I6;
    case 7: return &var_DigitalInput_I7;
    case 8: return &var_DigitalInput_I8;
  }
  return nullptr;
}

CIEC_ANY *FORTE_logiBUS_8_DI_UP::getDO(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_QO;
    case 1: return &var_STATUS;
  }
  return nullptr;
}

CEventConnection *FORTE_logiBUS_8_DI_UP::getEOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_INITO;
    case 1: return &conn_IND;
  }
  return nullptr;
}

CDataConnection **FORTE_logiBUS_8_DI_UP::getDIConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_QI;
    case 1: return &conn_DigitalInput_I1;
    case 2: return &conn_DigitalInput_I2;
    case 3: return &conn_DigitalInput_I3;
    case 4: return &conn_DigitalInput_I4;
    case 5: return &conn_DigitalInput_I5;
    case 6: return &conn_DigitalInput_I6;
    case 7: return &conn_DigitalInput_I7;
    case 8: return &conn_DigitalInput_I8;
  }
  return nullptr;
}

CDataConnection *FORTE_logiBUS_8_DI_UP::getDOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_QO;
    case 1: return &conn_STATUS;
  }
  return nullptr;
}
