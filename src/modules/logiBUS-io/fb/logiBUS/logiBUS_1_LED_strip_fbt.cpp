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
 *** Name: logiBUS_1_LED_strip
 *** Description: Service Interface Function Block Type
 *** Version:
 ***     1.0: 2018-12-05/Jose Cabral -  -
 ***     1.1: 2022-08-04/Franz Höpfinger - HR Agrartechnik GmbH -
 ***     1.2: 2023-01-05/Franz Höpfinger - HR Agrartechnik GmbH -
 ***     1.3: 2025-02-18/Franz Höpfinger - HR Agrartechnik GmbH - logiBUS IO created.
 *************************************************************************/

#include "logiBUS_1_LED_strip_fbt.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "logiBUS_1_LED_strip_fbt_gen.cpp"
#endif

#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"
#include "LED_COLOURS_gcf.h"
#include "LED_strip_gcf.h"

DEFINE_FIRMWARE_FB(FORTE_logiBUS_1_LED_strip, g_nStringIdlogiBUS_1_LED_strip)

const CStringDictionary::TStringId FORTE_logiBUS_1_LED_strip::scmDataInputNames[] = {g_nStringIdQI, g_nStringIdOutput, g_nStringIdColour, g_nStringIdLED_5HZ, g_nStringIdLED_2HZ, g_nStringIdLED_1HZ, g_nStringIdLED_05HZ, g_nStringIdLED_ON};
const CStringDictionary::TStringId FORTE_logiBUS_1_LED_strip::scmDataInputTypeIds[] = {g_nStringIdBOOL, g_nStringIdUSINT, g_nStringIdUINT, g_nStringIdSTRING, g_nStringIdSTRING, g_nStringIdSTRING, g_nStringIdSTRING, g_nStringIdSTRING};
const CStringDictionary::TStringId FORTE_logiBUS_1_LED_strip::scmDataOutputNames[] = {g_nStringIdQO, g_nStringIdSTATUS};
const CStringDictionary::TStringId FORTE_logiBUS_1_LED_strip::scmDataOutputTypeIds[] = {g_nStringIdBOOL, g_nStringIdSTRING};
const TDataIOID FORTE_logiBUS_1_LED_strip::scmEIWith[] = {0, 1, 2, 3, 4, 5, 6, 7, scmWithListDelimiter};
const TForteInt16 FORTE_logiBUS_1_LED_strip::scmEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_logiBUS_1_LED_strip::scmEventInputNames[] = {g_nStringIdINIT};
const CStringDictionary::TStringId FORTE_logiBUS_1_LED_strip::scmEventInputTypeIds[] = {g_nStringIdEInit};
const TDataIOID FORTE_logiBUS_1_LED_strip::scmEOWith[] = {0, scmWithListDelimiter, 0, 1, scmWithListDelimiter};
const TForteInt16 FORTE_logiBUS_1_LED_strip::scmEOWithIndexes[] = {0, 2};
const CStringDictionary::TStringId FORTE_logiBUS_1_LED_strip::scmEventOutputNames[] = {g_nStringIdINITO, g_nStringIdIND};
const CStringDictionary::TStringId FORTE_logiBUS_1_LED_strip::scmEventOutputTypeIds[] = {g_nStringIdEInit, g_nStringIdEvent};
const SFBInterfaceSpec FORTE_logiBUS_1_LED_strip::scmFBInterfaceSpec = {
  1, scmEventInputNames, scmEventInputTypeIds, scmEIWith, scmEIWithIndexes,
  2, scmEventOutputNames, scmEventOutputTypeIds, scmEOWith, scmEOWithIndexes,
  8, scmDataInputNames, scmDataInputTypeIds,
  2, scmDataOutputNames, scmDataOutputTypeIds,
  0, nullptr,
  0, nullptr
};

FORTE_logiBUS_1_LED_strip::FORTE_logiBUS_1_LED_strip(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    logiBUSIOFunctionBlock(paContainer, scmFBInterfaceSpec, paInstanceNameId),
    var_QI(0_BOOL),
    var_Output(st_global_Output_strip),
    var_Colour(st_global_LED_GREEN),
    var_LED_5HZ(""_STRING),
    var_LED_2HZ(""_STRING),
    var_LED_1HZ(""_STRING),
    var_LED_05HZ(""_STRING),
    var_LED_ON(""_STRING),
    var_QO(0_BOOL),
    var_STATUS(""_STRING),
    var_conn_QO(var_QO),
    var_conn_STATUS(var_STATUS),
    conn_INITO(this, 0),
    conn_IND(this, 1),
    conn_QI(nullptr),
    conn_Output(nullptr),
    conn_Colour(nullptr),
    conn_LED_5HZ(nullptr),
    conn_LED_2HZ(nullptr),
    conn_LED_1HZ(nullptr),
    conn_LED_05HZ(nullptr),
    conn_LED_ON(nullptr),
    conn_QO(this, 0, &var_conn_QO),
    conn_STATUS(this, 1, &var_conn_STATUS) {
};

void FORTE_logiBUS_1_LED_strip::setInitialValues() {
  var_QI = 0_BOOL;
  var_Output = st_global_Output_strip;
  var_Colour = st_global_LED_GREEN;
  var_LED_5HZ = ""_STRING;
  var_LED_2HZ = ""_STRING;
  var_LED_1HZ = ""_STRING;
  var_LED_05HZ = ""_STRING;
  var_LED_ON = ""_STRING;
  var_QO = 0_BOOL;
  var_STATUS = ""_STRING;
}

void FORTE_logiBUS_1_LED_strip::executeEvent(const TEventID paEIID, CEventChainExecutionThread *const paECET) {
  switch(paEIID) {
    case scmEventINITID:
      if((true == var_QI) && (false == var_QO)){
        //this is the first time init is called
        //TODO !! avoid 2 identical u16ObjId's !!!!
        handle_LED_5HZ            = std::unique_ptr<forte::core::io::IOHandle>(mapPin(var_LED_5HZ.getStorage(),  forte::core::io::IOMapper::Out, CIEC_ANY::e_BOOL, logiBUSIODeviceController::HandleType::ESP32LED_STRIP_5HZ,            static_cast<logiBUSIOEnums::LEDStripsPinNumber>((TForteUInt8)var_Output), (TForteUInt16)var_Colour));
        handle_LED_2HZ            = std::unique_ptr<forte::core::io::IOHandle>(mapPin(var_LED_2HZ.getStorage(),  forte::core::io::IOMapper::Out, CIEC_ANY::e_BOOL, logiBUSIODeviceController::HandleType::ESP32LED_STRIP_2HZ,            static_cast<logiBUSIOEnums::LEDStripsPinNumber>((TForteUInt8)var_Output), (TForteUInt16)var_Colour));
        handle_LED_1HZ            = std::unique_ptr<forte::core::io::IOHandle>(mapPin(var_LED_1HZ.getStorage(),  forte::core::io::IOMapper::Out, CIEC_ANY::e_BOOL, logiBUSIODeviceController::HandleType::ESP32LED_STRIP_1HZ,            static_cast<logiBUSIOEnums::LEDStripsPinNumber>((TForteUInt8)var_Output), (TForteUInt16)var_Colour));
        handle_LED_05HZ           = std::unique_ptr<forte::core::io::IOHandle>(mapPin(var_LED_05HZ.getStorage(), forte::core::io::IOMapper::Out, CIEC_ANY::e_BOOL, logiBUSIODeviceController::HandleType::ESP32LED_STRIP_05HZ,           static_cast<logiBUSIOEnums::LEDStripsPinNumber>((TForteUInt8)var_Output), (TForteUInt16)var_Colour));
        handle_LED_ON             = std::unique_ptr<forte::core::io::IOHandle>(mapPin(var_LED_ON.getStorage(),   forte::core::io::IOMapper::Out, CIEC_ANY::e_BOOL, logiBUSIODeviceController::HandleType::ESP32LED_STRIP_ON,             static_cast<logiBUSIOEnums::LEDStripsPinNumber>((TForteUInt8)var_Output), (TForteUInt16)var_Colour));
        var_STATUS = scmOK;
        var_QO = true_BOOL;
        sendOutputEvent(scmEventINITOID, paECET);
      }
      break;
  }
}

void FORTE_logiBUS_1_LED_strip::readInputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventINITID: {
      readData(0, var_QI, conn_QI);
      readData(1, var_Output, conn_Output);
      readData(2, var_Colour, conn_Colour);
      readData(3, var_LED_5HZ, conn_LED_5HZ);
      readData(4, var_LED_2HZ, conn_LED_2HZ);
      readData(5, var_LED_1HZ, conn_LED_1HZ);
      readData(6, var_LED_05HZ, conn_LED_05HZ);
      readData(7, var_LED_ON, conn_LED_ON);
      break;
    }
    default:
      break;
  }
}

void FORTE_logiBUS_1_LED_strip::writeOutputData(const TEventID paEIID) {
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

CIEC_ANY *FORTE_logiBUS_1_LED_strip::getDI(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_QI;
    case 1: return &var_Output;
    case 2: return &var_Colour;
    case 3: return &var_LED_5HZ;
    case 4: return &var_LED_2HZ;
    case 5: return &var_LED_1HZ;
    case 6: return &var_LED_05HZ;
    case 7: return &var_LED_ON;
  }
  return nullptr;
}

CIEC_ANY *FORTE_logiBUS_1_LED_strip::getDO(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_QO;
    case 1: return &var_STATUS;
  }
  return nullptr;
}

CEventConnection *FORTE_logiBUS_1_LED_strip::getEOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_INITO;
    case 1: return &conn_IND;
  }
  return nullptr;
}

CDataConnection **FORTE_logiBUS_1_LED_strip::getDIConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_QI;
    case 1: return &conn_Output;
    case 2: return &conn_Colour;
    case 3: return &conn_LED_5HZ;
    case 4: return &conn_LED_2HZ;
    case 5: return &conn_LED_1HZ;
    case 6: return &conn_LED_05HZ;
    case 7: return &conn_LED_ON;
  }
  return nullptr;
}

CDataConnection *FORTE_logiBUS_1_LED_strip::getDOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_QO;
    case 1: return &conn_STATUS;
  }
  return nullptr;
}
