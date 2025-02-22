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
 *** Name: logiBUS_2_SERVO
 *** Description: Service Interface Function Block Type
 *** Version:
 ***     1.0: 2018-12-05/Jose Cabral -  -
 ***     1.1: 2022-08-04/Franz Höpfinger - HR Agrartechnik GmbH -
 ***     1.2: 2023-01-05/Franz Höpfinger - HR Agrartechnik GmbH -
 ***     1.3: 2025-02-18/Franz Höpfinger - HR Agrartechnik GmbH - logiBUS IO created.
 *************************************************************************/

#include "logiBUS_2_SERVO_fbt.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "logiBUS_2_SERVO_fbt_gen.cpp"
#endif

#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"

DEFINE_FIRMWARE_FB(FORTE_logiBUS_2_SERVO, g_nStringIdlogiBUS_2_SERVO)

const CStringDictionary::TStringId FORTE_logiBUS_2_SERVO::scmDataInputNames[] = {g_nStringIdQI, g_nStringIdServo_S1, g_nStringIdServo_S2};
const CStringDictionary::TStringId FORTE_logiBUS_2_SERVO::scmDataInputTypeIds[] = {g_nStringIdBOOL, g_nStringIdSTRING, g_nStringIdSTRING};
const CStringDictionary::TStringId FORTE_logiBUS_2_SERVO::scmDataOutputNames[] = {g_nStringIdQO, g_nStringIdSTATUS};
const CStringDictionary::TStringId FORTE_logiBUS_2_SERVO::scmDataOutputTypeIds[] = {g_nStringIdBOOL, g_nStringIdSTRING};
const TDataIOID FORTE_logiBUS_2_SERVO::scmEIWith[] = {0, 1, 2, scmWithListDelimiter};
const TForteInt16 FORTE_logiBUS_2_SERVO::scmEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_logiBUS_2_SERVO::scmEventInputNames[] = {g_nStringIdINIT};
const CStringDictionary::TStringId FORTE_logiBUS_2_SERVO::scmEventInputTypeIds[] = {g_nStringIdEInit};
const TDataIOID FORTE_logiBUS_2_SERVO::scmEOWith[] = {0, scmWithListDelimiter, 0, 1, scmWithListDelimiter};
const TForteInt16 FORTE_logiBUS_2_SERVO::scmEOWithIndexes[] = {0, 2};
const CStringDictionary::TStringId FORTE_logiBUS_2_SERVO::scmEventOutputNames[] = {g_nStringIdINITO, g_nStringIdIND};
const CStringDictionary::TStringId FORTE_logiBUS_2_SERVO::scmEventOutputTypeIds[] = {g_nStringIdEInit, g_nStringIdEvent};
const SFBInterfaceSpec FORTE_logiBUS_2_SERVO::scmFBInterfaceSpec = {
  1, scmEventInputNames, scmEventInputTypeIds, scmEIWith, scmEIWithIndexes,
  2, scmEventOutputNames, scmEventOutputTypeIds, scmEOWith, scmEOWithIndexes,
  3, scmDataInputNames, scmDataInputTypeIds,
  2, scmDataOutputNames, scmDataOutputTypeIds,
  0, nullptr,
  0, nullptr
};

FORTE_logiBUS_2_SERVO::FORTE_logiBUS_2_SERVO(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    logiBUSIOFunctionBlock(paContainer, scmFBInterfaceSpec, paInstanceNameId),
    var_QI(0_BOOL),
    var_Servo_S1(""_STRING),
    var_Servo_S2(""_STRING),
    var_QO(0_BOOL),
    var_STATUS(""_STRING),
    var_conn_QO(var_QO),
    var_conn_STATUS(var_STATUS),
    conn_INITO(this, 0),
    conn_IND(this, 1),
    conn_QI(nullptr),
    conn_Servo_S1(nullptr),
    conn_Servo_S2(nullptr),
    conn_QO(this, 0, &var_conn_QO),
    conn_STATUS(this, 1, &var_conn_STATUS) {
};

void FORTE_logiBUS_2_SERVO::setInitialValues() {
  var_QI = 0_BOOL;
  var_Servo_S1 = ""_STRING;
  var_Servo_S2 = ""_STRING;
  var_QO = 0_BOOL;
  var_STATUS = ""_STRING;
}

void FORTE_logiBUS_2_SERVO::executeEvent(const TEventID paEIID, CEventChainExecutionThread *const paECET) {
  switch(paEIID) {
    case scmEventINITID:
      var_QO = var_QI;

      handle_S1 = std::unique_ptr<forte::core::io::IOHandle>(mapPin(var_Servo_S1.getStorage(), forte::core::io::IOMapper::Out, CIEC_ANY::e_WORD, logiBUSIODeviceController::HandleType::ESP32ServoO, logiBUSIOEnums::DigitalOutPinNumber::PinQ01));
      handle_S2 = std::unique_ptr<forte::core::io::IOHandle>(mapPin(var_Servo_S2.getStorage(), forte::core::io::IOMapper::Out, CIEC_ANY::e_WORD, logiBUSIODeviceController::HandleType::ESP32ServoO, logiBUSIOEnums::DigitalOutPinNumber::PinQ02));



      sendOutputEvent(scmEventINITOID, paECET);
      break;
  }
}

void FORTE_logiBUS_2_SERVO::readInputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventINITID: {
      readData(0, var_QI, conn_QI);
      readData(1, var_Servo_S1, conn_Servo_S1);
      readData(2, var_Servo_S2, conn_Servo_S2);
      break;
    }
    default:
      break;
  }
}

void FORTE_logiBUS_2_SERVO::writeOutputData(const TEventID paEIID) {
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

CIEC_ANY *FORTE_logiBUS_2_SERVO::getDI(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_QI;
    case 1: return &var_Servo_S1;
    case 2: return &var_Servo_S2;
  }
  return nullptr;
}

CIEC_ANY *FORTE_logiBUS_2_SERVO::getDO(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_QO;
    case 1: return &var_STATUS;
  }
  return nullptr;
}

CEventConnection *FORTE_logiBUS_2_SERVO::getEOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_INITO;
    case 1: return &conn_IND;
  }
  return nullptr;
}

CDataConnection **FORTE_logiBUS_2_SERVO::getDIConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_QI;
    case 1: return &conn_Servo_S1;
    case 2: return &conn_Servo_S2;
  }
  return nullptr;
}

CDataConnection *FORTE_logiBUS_2_SERVO::getDOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_QO;
    case 1: return &conn_STATUS;
  }
  return nullptr;
}
