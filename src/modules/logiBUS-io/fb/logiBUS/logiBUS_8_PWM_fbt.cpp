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
 *** Name: logiBUS_8_PWM
 *** Description: Service Interface Function Block Type
 *** Version:
 ***     1.0: 2018-12-05/Jose Cabral -  -
 ***     1.1: 2022-08-04/Franz Höpfinger - HR Agrartechnik GmbH -
 ***     1.2: 2023-01-05/Franz Höpfinger - HR Agrartechnik GmbH -
 ***     1.3: 2025-02-18/Franz Höpfinger - HR Agrartechnik GmbH - logiBUS IO created.
 *************************************************************************/

#include "logiBUS_8_PWM_fbt.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "logiBUS_8_PWM_fbt_gen.cpp"
#endif

#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"

DEFINE_FIRMWARE_FB(FORTE_logiBUS_8_PWM, g_nStringIdlogiBUS_8_PWM)

const CStringDictionary::TStringId FORTE_logiBUS_8_PWM::scmDataInputNames[] = {g_nStringIdQI, g_nStringIdPWMOutput_Q1, g_nStringIdPWMOutput_Q2, g_nStringIdPWMOutput_Q3, g_nStringIdPWMOutput_Q4, g_nStringIdPWMOutput_Q5, g_nStringIdPWMOutput_Q6, g_nStringIdPWMOutput_Q7, g_nStringIdPWMOutput_Q8};
const CStringDictionary::TStringId FORTE_logiBUS_8_PWM::scmDataInputTypeIds[] = {g_nStringIdBOOL, g_nStringIdSTRING, g_nStringIdSTRING, g_nStringIdSTRING, g_nStringIdSTRING, g_nStringIdSTRING, g_nStringIdSTRING, g_nStringIdSTRING, g_nStringIdSTRING};
const CStringDictionary::TStringId FORTE_logiBUS_8_PWM::scmDataOutputNames[] = {g_nStringIdQO, g_nStringIdSTATUS};
const CStringDictionary::TStringId FORTE_logiBUS_8_PWM::scmDataOutputTypeIds[] = {g_nStringIdBOOL, g_nStringIdSTRING};
const TDataIOID FORTE_logiBUS_8_PWM::scmEIWith[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, scmWithListDelimiter};
const TForteInt16 FORTE_logiBUS_8_PWM::scmEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_logiBUS_8_PWM::scmEventInputNames[] = {g_nStringIdINIT};
const CStringDictionary::TStringId FORTE_logiBUS_8_PWM::scmEventInputTypeIds[] = {g_nStringIdEInit};
const TDataIOID FORTE_logiBUS_8_PWM::scmEOWith[] = {0, scmWithListDelimiter, 0, 1, scmWithListDelimiter};
const TForteInt16 FORTE_logiBUS_8_PWM::scmEOWithIndexes[] = {0, 2};
const CStringDictionary::TStringId FORTE_logiBUS_8_PWM::scmEventOutputNames[] = {g_nStringIdINITO, g_nStringIdIND};
const CStringDictionary::TStringId FORTE_logiBUS_8_PWM::scmEventOutputTypeIds[] = {g_nStringIdEInit, g_nStringIdEvent};
const SFBInterfaceSpec FORTE_logiBUS_8_PWM::scmFBInterfaceSpec = {
  1, scmEventInputNames, scmEventInputTypeIds, scmEIWith, scmEIWithIndexes,
  2, scmEventOutputNames, scmEventOutputTypeIds, scmEOWith, scmEOWithIndexes,
  9, scmDataInputNames, scmDataInputTypeIds,
  2, scmDataOutputNames, scmDataOutputTypeIds,
  0, nullptr,
  0, nullptr
};

FORTE_logiBUS_8_PWM::FORTE_logiBUS_8_PWM(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    logiBUSIOFunctionBlock(paContainer, scmFBInterfaceSpec, paInstanceNameId),
    var_QI(0_BOOL),
    var_PWMOutput_Q1(""_STRING),
    var_PWMOutput_Q2(""_STRING),
    var_PWMOutput_Q3(""_STRING),
    var_PWMOutput_Q4(""_STRING),
    var_PWMOutput_Q5(""_STRING),
    var_PWMOutput_Q6(""_STRING),
    var_PWMOutput_Q7(""_STRING),
    var_PWMOutput_Q8(""_STRING),
    var_QO(0_BOOL),
    var_STATUS(""_STRING),
    var_conn_QO(var_QO),
    var_conn_STATUS(var_STATUS),
    conn_INITO(this, 0),
    conn_IND(this, 1),
    conn_QI(nullptr),
    conn_PWMOutput_Q1(nullptr),
    conn_PWMOutput_Q2(nullptr),
    conn_PWMOutput_Q3(nullptr),
    conn_PWMOutput_Q4(nullptr),
    conn_PWMOutput_Q5(nullptr),
    conn_PWMOutput_Q6(nullptr),
    conn_PWMOutput_Q7(nullptr),
    conn_PWMOutput_Q8(nullptr),
    conn_QO(this, 0, &var_conn_QO),
    conn_STATUS(this, 1, &var_conn_STATUS) {
};

void FORTE_logiBUS_8_PWM::setInitialValues() {
  var_QI = 0_BOOL;
  var_PWMOutput_Q1 = ""_STRING;
  var_PWMOutput_Q2 = ""_STRING;
  var_PWMOutput_Q3 = ""_STRING;
  var_PWMOutput_Q4 = ""_STRING;
  var_PWMOutput_Q5 = ""_STRING;
  var_PWMOutput_Q6 = ""_STRING;
  var_PWMOutput_Q7 = ""_STRING;
  var_PWMOutput_Q8 = ""_STRING;
  var_QO = 0_BOOL;
  var_STATUS = ""_STRING;
}

void FORTE_logiBUS_8_PWM::executeEvent(const TEventID paEIID, CEventChainExecutionThread *const paECET) {
  switch(paEIID) {
    case scmEventINITID:
      var_QO = var_QI;

      handle_Q1 = std::unique_ptr<forte::core::io::IOHandle>(mapPin(var_PWMOutput_Q1.getStorage(), forte::core::io::IOMapper::Out, CIEC_ANY::e_DWORD, logiBUSIODeviceController::HandleType::ESP32PWMO, logiBUSIOEnums::DigitalOutPinNumber::PinQ01));
      handle_Q2 = std::unique_ptr<forte::core::io::IOHandle>(mapPin(var_PWMOutput_Q2.getStorage(), forte::core::io::IOMapper::Out, CIEC_ANY::e_DWORD, logiBUSIODeviceController::HandleType::ESP32PWMO, logiBUSIOEnums::DigitalOutPinNumber::PinQ02));
      handle_Q3 = std::unique_ptr<forte::core::io::IOHandle>(mapPin(var_PWMOutput_Q3.getStorage(), forte::core::io::IOMapper::Out, CIEC_ANY::e_DWORD, logiBUSIODeviceController::HandleType::ESP32PWMO, logiBUSIOEnums::DigitalOutPinNumber::PinQ03));
      handle_Q4 = std::unique_ptr<forte::core::io::IOHandle>(mapPin(var_PWMOutput_Q4.getStorage(), forte::core::io::IOMapper::Out, CIEC_ANY::e_DWORD, logiBUSIODeviceController::HandleType::ESP32PWMO, logiBUSIOEnums::DigitalOutPinNumber::PinQ04));
      handle_Q5 = std::unique_ptr<forte::core::io::IOHandle>(mapPin(var_PWMOutput_Q5.getStorage(), forte::core::io::IOMapper::Out, CIEC_ANY::e_DWORD, logiBUSIODeviceController::HandleType::ESP32PWMO, logiBUSIOEnums::DigitalOutPinNumber::PinQ05));
      handle_Q6 = std::unique_ptr<forte::core::io::IOHandle>(mapPin(var_PWMOutput_Q6.getStorage(), forte::core::io::IOMapper::Out, CIEC_ANY::e_DWORD, logiBUSIODeviceController::HandleType::ESP32PWMO, logiBUSIOEnums::DigitalOutPinNumber::PinQ06));
      handle_Q7 = std::unique_ptr<forte::core::io::IOHandle>(mapPin(var_PWMOutput_Q7.getStorage(), forte::core::io::IOMapper::Out, CIEC_ANY::e_DWORD, logiBUSIODeviceController::HandleType::ESP32PWMO, logiBUSIOEnums::DigitalOutPinNumber::PinQ07));
      handle_Q8 = std::unique_ptr<forte::core::io::IOHandle>(mapPin(var_PWMOutput_Q8.getStorage(), forte::core::io::IOMapper::Out, CIEC_ANY::e_DWORD, logiBUSIODeviceController::HandleType::ESP32PWMO, logiBUSIOEnums::DigitalOutPinNumber::PinQ08));



      sendOutputEvent(scmEventINITOID, paECET);
      break;
  }
}

void FORTE_logiBUS_8_PWM::readInputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventINITID: {
      readData(0, var_QI, conn_QI);
      readData(1, var_PWMOutput_Q1, conn_PWMOutput_Q1);
      readData(2, var_PWMOutput_Q2, conn_PWMOutput_Q2);
      readData(3, var_PWMOutput_Q3, conn_PWMOutput_Q3);
      readData(4, var_PWMOutput_Q4, conn_PWMOutput_Q4);
      readData(5, var_PWMOutput_Q5, conn_PWMOutput_Q5);
      readData(6, var_PWMOutput_Q6, conn_PWMOutput_Q6);
      readData(7, var_PWMOutput_Q7, conn_PWMOutput_Q7);
      readData(8, var_PWMOutput_Q8, conn_PWMOutput_Q8);
      break;
    }
    default:
      break;
  }
}

void FORTE_logiBUS_8_PWM::writeOutputData(const TEventID paEIID) {
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

CIEC_ANY *FORTE_logiBUS_8_PWM::getDI(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_QI;
    case 1: return &var_PWMOutput_Q1;
    case 2: return &var_PWMOutput_Q2;
    case 3: return &var_PWMOutput_Q3;
    case 4: return &var_PWMOutput_Q4;
    case 5: return &var_PWMOutput_Q5;
    case 6: return &var_PWMOutput_Q6;
    case 7: return &var_PWMOutput_Q7;
    case 8: return &var_PWMOutput_Q8;
  }
  return nullptr;
}

CIEC_ANY *FORTE_logiBUS_8_PWM::getDO(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_QO;
    case 1: return &var_STATUS;
  }
  return nullptr;
}

CEventConnection *FORTE_logiBUS_8_PWM::getEOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_INITO;
    case 1: return &conn_IND;
  }
  return nullptr;
}

CDataConnection **FORTE_logiBUS_8_PWM::getDIConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_QI;
    case 1: return &conn_PWMOutput_Q1;
    case 2: return &conn_PWMOutput_Q2;
    case 3: return &conn_PWMOutput_Q3;
    case 4: return &conn_PWMOutput_Q4;
    case 5: return &conn_PWMOutput_Q5;
    case 6: return &conn_PWMOutput_Q6;
    case 7: return &conn_PWMOutput_Q7;
    case 8: return &conn_PWMOutput_Q8;
  }
  return nullptr;
}

CDataConnection *FORTE_logiBUS_8_PWM::getDOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_QO;
    case 1: return &conn_STATUS;
  }
  return nullptr;
}
