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
 *** Name: logiBUS_8_AI
 *** Description: Service Interface Function Block Type
 *** Version:
 ***     1.0: 2018-12-05/Jose Cabral -  -
 ***     1.1: 2022-08-04/Franz Höpfinger - HR Agrartechnik GmbH -
 ***     1.2: 2023-01-05/Franz Höpfinger - HR Agrartechnik GmbH -
 ***     1.3: 2025-02-18/Franz Höpfinger - HR Agrartechnik GmbH - logiBUS IO created.
 *************************************************************************/

#include "logiBUS_8_AI_fbt.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "logiBUS_8_AI_fbt_gen.cpp"
#endif

#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"
#include "IOHandleESP32AnalogIN.h"


DEFINE_FIRMWARE_FB(FORTE_logiBUS_8_AI, g_nStringIdlogiBUS_8_AI)

const CStringDictionary::TStringId FORTE_logiBUS_8_AI::scmDataInputNames[] = {g_nStringIdQI, g_nStringIdAnalogInput_I1, g_nStringIdAnalogInput_I1_hysteresis, g_nStringIdAnalogInput_I2, g_nStringIdAnalogInput_I2_hysteresis, g_nStringIdAnalogInput_I3, g_nStringIdAnalogInput_I3_hysteresis, g_nStringIdAnalogInput_I4, g_nStringIdAnalogInput_I4_hysteresis, g_nStringIdAnalogInput_I5, g_nStringIdAnalogInput_I5_hysteresis, g_nStringIdAnalogInput_I6, g_nStringIdAnalogInput_I6_hysteresis, g_nStringIdAnalogInput_I7, g_nStringIdAnalogInput_I7_hysteresis, g_nStringIdAnalogInput_I8, g_nStringIdAnalogInput_I8_hysteresis};
const CStringDictionary::TStringId FORTE_logiBUS_8_AI::scmDataInputTypeIds[] = {g_nStringIdBOOL, g_nStringIdSTRING, g_nStringIdDWORD, g_nStringIdSTRING, g_nStringIdDWORD, g_nStringIdSTRING, g_nStringIdDWORD, g_nStringIdSTRING, g_nStringIdDWORD, g_nStringIdSTRING, g_nStringIdDWORD, g_nStringIdSTRING, g_nStringIdDWORD, g_nStringIdSTRING, g_nStringIdDWORD, g_nStringIdSTRING, g_nStringIdDWORD};
const CStringDictionary::TStringId FORTE_logiBUS_8_AI::scmDataOutputNames[] = {g_nStringIdQO, g_nStringIdSTATUS};
const CStringDictionary::TStringId FORTE_logiBUS_8_AI::scmDataOutputTypeIds[] = {g_nStringIdBOOL, g_nStringIdSTRING};
const TDataIOID FORTE_logiBUS_8_AI::scmEIWith[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, scmWithListDelimiter};
const TForteInt16 FORTE_logiBUS_8_AI::scmEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_logiBUS_8_AI::scmEventInputNames[] = {g_nStringIdINIT};
const CStringDictionary::TStringId FORTE_logiBUS_8_AI::scmEventInputTypeIds[] = {g_nStringIdEInit};
const TDataIOID FORTE_logiBUS_8_AI::scmEOWith[] = {0, scmWithListDelimiter, 0, 1, scmWithListDelimiter};
const TForteInt16 FORTE_logiBUS_8_AI::scmEOWithIndexes[] = {0, 2};
const CStringDictionary::TStringId FORTE_logiBUS_8_AI::scmEventOutputNames[] = {g_nStringIdINITO, g_nStringIdIND};
const CStringDictionary::TStringId FORTE_logiBUS_8_AI::scmEventOutputTypeIds[] = {g_nStringIdEInit, g_nStringIdEvent};
const SFBInterfaceSpec FORTE_logiBUS_8_AI::scmFBInterfaceSpec = {
  1, scmEventInputNames, scmEventInputTypeIds, scmEIWith, scmEIWithIndexes,
  2, scmEventOutputNames, scmEventOutputTypeIds, scmEOWith, scmEOWithIndexes,
  17, scmDataInputNames, scmDataInputTypeIds,
  2, scmDataOutputNames, scmDataOutputTypeIds,
  0, nullptr,
  0, nullptr
};

FORTE_logiBUS_8_AI::FORTE_logiBUS_8_AI(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    logiBUSIOFunctionBlock(paContainer, scmFBInterfaceSpec, paInstanceNameId),
    var_QI(0_BOOL),
    var_AnalogInput_I1(""_STRING),
    var_AnalogInput_I1_hysteresis(0_DWORD),
    var_AnalogInput_I2(""_STRING),
    var_AnalogInput_I2_hysteresis(0_DWORD),
    var_AnalogInput_I3(""_STRING),
    var_AnalogInput_I3_hysteresis(0_DWORD),
    var_AnalogInput_I4(""_STRING),
    var_AnalogInput_I4_hysteresis(0_DWORD),
    var_AnalogInput_I5(""_STRING),
    var_AnalogInput_I5_hysteresis(0_DWORD),
    var_AnalogInput_I6(""_STRING),
    var_AnalogInput_I6_hysteresis(0_DWORD),
    var_AnalogInput_I7(""_STRING),
    var_AnalogInput_I7_hysteresis(0_DWORD),
    var_AnalogInput_I8(""_STRING),
    var_AnalogInput_I8_hysteresis(0_DWORD),
    var_QO(0_BOOL),
    var_STATUS(""_STRING),
    var_conn_QO(var_QO),
    var_conn_STATUS(var_STATUS),
    conn_INITO(this, 0),
    conn_IND(this, 1),
    conn_QI(nullptr),
    conn_AnalogInput_I1(nullptr),
    conn_AnalogInput_I1_hysteresis(nullptr),
    conn_AnalogInput_I2(nullptr),
    conn_AnalogInput_I2_hysteresis(nullptr),
    conn_AnalogInput_I3(nullptr),
    conn_AnalogInput_I3_hysteresis(nullptr),
    conn_AnalogInput_I4(nullptr),
    conn_AnalogInput_I4_hysteresis(nullptr),
    conn_AnalogInput_I5(nullptr),
    conn_AnalogInput_I5_hysteresis(nullptr),
    conn_AnalogInput_I6(nullptr),
    conn_AnalogInput_I6_hysteresis(nullptr),
    conn_AnalogInput_I7(nullptr),
    conn_AnalogInput_I7_hysteresis(nullptr),
    conn_AnalogInput_I8(nullptr),
    conn_AnalogInput_I8_hysteresis(nullptr),
    conn_QO(this, 0, &var_conn_QO),
    conn_STATUS(this, 1, &var_conn_STATUS) {
};

void FORTE_logiBUS_8_AI::setInitialValues() {
  var_QI = 0_BOOL;
  var_AnalogInput_I1 = ""_STRING;
  var_AnalogInput_I1_hysteresis = 0_DWORD;
  var_AnalogInput_I2 = ""_STRING;
  var_AnalogInput_I2_hysteresis = 0_DWORD;
  var_AnalogInput_I3 = ""_STRING;
  var_AnalogInput_I3_hysteresis = 0_DWORD;
  var_AnalogInput_I4 = ""_STRING;
  var_AnalogInput_I4_hysteresis = 0_DWORD;
  var_AnalogInput_I5 = ""_STRING;
  var_AnalogInput_I5_hysteresis = 0_DWORD;
  var_AnalogInput_I6 = ""_STRING;
  var_AnalogInput_I6_hysteresis = 0_DWORD;
  var_AnalogInput_I7 = ""_STRING;
  var_AnalogInput_I7_hysteresis = 0_DWORD;
  var_AnalogInput_I8 = ""_STRING;
  var_AnalogInput_I8_hysteresis = 0_DWORD;
  var_QO = 0_BOOL;
  var_STATUS = ""_STRING;
}

void FORTE_logiBUS_8_AI::executeEvent(const TEventID paEIID, CEventChainExecutionThread *const paECET) {
  switch(paEIID) {
    case scmEventINITID:
      if (init()) {
        var_QO = var_QI;

        //TODO !! avoid 2 identical u16ObjId's !!!!
        handle_AI1 = std::unique_ptr<forte::core::io::IOHandle>(mapPin(var_AnalogInput_I1.getStorage(), forte::core::io::IOMapper::In, CIEC_ANY::e_DWORD, logiBUSIODeviceController::HandleType::ESP32AnalogI, logiBUSIOEnums::AnalogInPinNumber::PinAI1, (TForteUInt32)var_AnalogInput_I1_hysteresis));
        handle_AI2 = std::unique_ptr<forte::core::io::IOHandle>(mapPin(var_AnalogInput_I2.getStorage(), forte::core::io::IOMapper::In, CIEC_ANY::e_DWORD, logiBUSIODeviceController::HandleType::ESP32AnalogI, logiBUSIOEnums::AnalogInPinNumber::PinAI2, (TForteUInt32)var_AnalogInput_I2_hysteresis));
        handle_AI3 = std::unique_ptr<forte::core::io::IOHandle>(mapPin(var_AnalogInput_I3.getStorage(), forte::core::io::IOMapper::In, CIEC_ANY::e_DWORD, logiBUSIODeviceController::HandleType::ESP32AnalogI, logiBUSIOEnums::AnalogInPinNumber::PinAI3, (TForteUInt32)var_AnalogInput_I3_hysteresis));
        handle_AI4 = std::unique_ptr<forte::core::io::IOHandle>(mapPin(var_AnalogInput_I4.getStorage(), forte::core::io::IOMapper::In, CIEC_ANY::e_DWORD, logiBUSIODeviceController::HandleType::ESP32AnalogI, logiBUSIOEnums::AnalogInPinNumber::PinAI4, (TForteUInt32)var_AnalogInput_I4_hysteresis));
        handle_AI5 = std::unique_ptr<forte::core::io::IOHandle>(mapPin(var_AnalogInput_I5.getStorage(), forte::core::io::IOMapper::In, CIEC_ANY::e_DWORD, logiBUSIODeviceController::HandleType::ESP32AnalogI, logiBUSIOEnums::AnalogInPinNumber::PinAI5, (TForteUInt32)var_AnalogInput_I5_hysteresis));
        handle_AI6 = std::unique_ptr<forte::core::io::IOHandle>(mapPin(var_AnalogInput_I6.getStorage(), forte::core::io::IOMapper::In, CIEC_ANY::e_DWORD, logiBUSIODeviceController::HandleType::ESP32AnalogI, logiBUSIOEnums::AnalogInPinNumber::PinAI6, (TForteUInt32)var_AnalogInput_I6_hysteresis));
        handle_AI7 = std::unique_ptr<forte::core::io::IOHandle>(mapPin(var_AnalogInput_I7.getStorage(), forte::core::io::IOMapper::In, CIEC_ANY::e_DWORD, logiBUSIODeviceController::HandleType::ESP32AnalogI, logiBUSIOEnums::AnalogInPinNumber::PinAI7, (TForteUInt32)var_AnalogInput_I7_hysteresis));
        handle_AI8 = std::unique_ptr<forte::core::io::IOHandle>(mapPin(var_AnalogInput_I8.getStorage(), forte::core::io::IOMapper::In, CIEC_ANY::e_DWORD, logiBUSIODeviceController::HandleType::ESP32AnalogI, logiBUSIOEnums::AnalogInPinNumber::PinAI8, (TForteUInt32)var_AnalogInput_I8_hysteresis));

        //now we must start the Task:
        IOHandleESP32AnalogIN::startTask();


        sendOutputEvent(scmEventINITOID, paECET);
      }
      break;
  }
}


bool FORTE_logiBUS_8_AI::isInit = false;
bool FORTE_logiBUS_8_AI::init(){
  isInit = true;
  return isInit;
}


EMGMResponse FORTE_logiBUS_8_AI::changeExecutionState(EMGMCommandType paCommand) {
  if(EMGMCommandType::Kill == paCommand) {
    //now we must stop the Task:
    IOHandleESP32AnalogIN::stopTask();
    isInit = false;
  }
  return CFunctionBlock::changeExecutionState(paCommand);
}



void FORTE_logiBUS_8_AI::readInputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventINITID: {
      readData(0, var_QI, conn_QI);
      readData(1, var_AnalogInput_I1, conn_AnalogInput_I1);
      readData(2, var_AnalogInput_I1_hysteresis, conn_AnalogInput_I1_hysteresis);
      readData(3, var_AnalogInput_I2, conn_AnalogInput_I2);
      readData(4, var_AnalogInput_I2_hysteresis, conn_AnalogInput_I2_hysteresis);
      readData(5, var_AnalogInput_I3, conn_AnalogInput_I3);
      readData(6, var_AnalogInput_I3_hysteresis, conn_AnalogInput_I3_hysteresis);
      readData(7, var_AnalogInput_I4, conn_AnalogInput_I4);
      readData(8, var_AnalogInput_I4_hysteresis, conn_AnalogInput_I4_hysteresis);
      readData(9, var_AnalogInput_I5, conn_AnalogInput_I5);
      readData(10, var_AnalogInput_I5_hysteresis, conn_AnalogInput_I5_hysteresis);
      readData(11, var_AnalogInput_I6, conn_AnalogInput_I6);
      readData(12, var_AnalogInput_I6_hysteresis, conn_AnalogInput_I6_hysteresis);
      readData(13, var_AnalogInput_I7, conn_AnalogInput_I7);
      readData(14, var_AnalogInput_I7_hysteresis, conn_AnalogInput_I7_hysteresis);
      readData(15, var_AnalogInput_I8, conn_AnalogInput_I8);
      readData(16, var_AnalogInput_I8_hysteresis, conn_AnalogInput_I8_hysteresis);
      break;
    }
    default:
      break;
  }
}

void FORTE_logiBUS_8_AI::writeOutputData(const TEventID paEIID) {
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

CIEC_ANY *FORTE_logiBUS_8_AI::getDI(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_QI;
    case 1: return &var_AnalogInput_I1;
    case 2: return &var_AnalogInput_I1_hysteresis;
    case 3: return &var_AnalogInput_I2;
    case 4: return &var_AnalogInput_I2_hysteresis;
    case 5: return &var_AnalogInput_I3;
    case 6: return &var_AnalogInput_I3_hysteresis;
    case 7: return &var_AnalogInput_I4;
    case 8: return &var_AnalogInput_I4_hysteresis;
    case 9: return &var_AnalogInput_I5;
    case 10: return &var_AnalogInput_I5_hysteresis;
    case 11: return &var_AnalogInput_I6;
    case 12: return &var_AnalogInput_I6_hysteresis;
    case 13: return &var_AnalogInput_I7;
    case 14: return &var_AnalogInput_I7_hysteresis;
    case 15: return &var_AnalogInput_I8;
    case 16: return &var_AnalogInput_I8_hysteresis;
  }
  return nullptr;
}

CIEC_ANY *FORTE_logiBUS_8_AI::getDO(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_QO;
    case 1: return &var_STATUS;
  }
  return nullptr;
}

CEventConnection *FORTE_logiBUS_8_AI::getEOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_INITO;
    case 1: return &conn_IND;
  }
  return nullptr;
}

CDataConnection **FORTE_logiBUS_8_AI::getDIConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_QI;
    case 1: return &conn_AnalogInput_I1;
    case 2: return &conn_AnalogInput_I1_hysteresis;
    case 3: return &conn_AnalogInput_I2;
    case 4: return &conn_AnalogInput_I2_hysteresis;
    case 5: return &conn_AnalogInput_I3;
    case 6: return &conn_AnalogInput_I3_hysteresis;
    case 7: return &conn_AnalogInput_I4;
    case 8: return &conn_AnalogInput_I4_hysteresis;
    case 9: return &conn_AnalogInput_I5;
    case 10: return &conn_AnalogInput_I5_hysteresis;
    case 11: return &conn_AnalogInput_I6;
    case 12: return &conn_AnalogInput_I6_hysteresis;
    case 13: return &conn_AnalogInput_I7;
    case 14: return &conn_AnalogInput_I7_hysteresis;
    case 15: return &conn_AnalogInput_I8;
    case 16: return &conn_AnalogInput_I8_hysteresis;
  }
  return nullptr;
}

CDataConnection *FORTE_logiBUS_8_AI::getDOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_QO;
    case 1: return &conn_STATUS;
  }
  return nullptr;
}
