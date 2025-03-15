/*************************************************************************
 * Copyright (c) 2025, Monika Wenger
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *************************************************************************/

#include "Wago636_fbt.h"

USE_STRING_ID(BOOL);
USE_STRING_ID(BusAdapterIn);
USE_STRING_ID(BusAdapterOut);
USE_STRING_ID(Busy);
USE_STRING_ID(CurrentPosition);
USE_STRING_ID(Event);
USE_STRING_ID(ExtendedInfoOn);
USE_STRING_ID(IND);
USE_STRING_ID(LimitSwitchN);
USE_STRING_ID(LimitSwitchP);
USE_STRING_ID(MAP);
USE_STRING_ID(MAPO);
USE_STRING_ID(MotorN);
USE_STRING_ID(MotorP);
USE_STRING_ID(OptimizeOn);
USE_STRING_ID(Positioning);
USE_STRING_ID(Preset);
USE_STRING_ID(PresetInputEnabled);
USE_STRING_ID(QI);
USE_STRING_ID(QO);
USE_STRING_ID(QuitErrors);
USE_STRING_ID(ReferenceOk);
USE_STRING_ID(STATUS);
USE_STRING_ID(STRING);
USE_STRING_ID(TargetPosition);
USE_STRING_ID(Wago636);
USE_STRING_ID(WagoBusAdapter);
USE_STRING_ID(WSTRING);

#include "WagoSlaveBase.h"
#include "../WagoDeviceController.h"

using namespace forte::core::io;

DEFINE_FIRMWARE_FB(FORTE_Wago636, STRID(Wago636))

const CStringDictionary::TStringId FORTE_Wago636::scmDataInputNames[] = { STRID(QI),
    STRID(Busy), STRID(LimitSwitchN), STRID(LimitSwitchP), STRID(PresetInputEnabled), STRID(OptimizeOn), STRID(ExtendedInfoOn), STRID(ReferenceOk), STRID(CurrentPosition), STRID(TargetPosition), STRID(MotorN), STRID(MotorP), STRID(Positioning),
    STRID(Preset), STRID(QuitErrors) };
const CStringDictionary::TStringId FORTE_Wago636::scmDataInputTypeIds[] = { STRID(BOOL), STRID(STRING), STRID(STRING), STRID(STRING), STRID(STRING), STRID(STRING), STRID(STRING), STRID(STRING), STRID(STRING), STRID(STRING), STRID(STRING), STRID(STRING), STRID(STRING), STRID(STRING), STRID(STRING) };
const CStringDictionary::TStringId FORTE_Wago636::scmDataOutputNames[] = { STRID(QO),
    STRID(STATUS) };
const CStringDictionary::TStringId FORTE_Wago636::scmDataOutputTypeIds[] = { STRID(BOOL),
    STRID(WSTRING) };
const TDataIOID FORTE_Wago636::scmEIWith[] = { 3, 2, 0, 11, 10, 8, 9, 14, 12, 13, 1, 4, 5, 6, 7, scmWithListDelimiter };
const TForteInt16 FORTE_Wago636::scmEIWithIndexes[] = { 0 };
const CStringDictionary::TStringId FORTE_Wago636::scmEventInputNames[] = { STRID(MAP) };
const CStringDictionary::TStringId FORTE_Wago636::scmEventInputTypeIds[] = {STRID(Event)};
const TDataIOID FORTE_Wago636::scmEOWith[] = { 0, scmWithListDelimiter, 0, 1, scmWithListDelimiter };
const TForteInt16 FORTE_Wago636::scmEOWithIndexes[] = { 0, 2 };
const CStringDictionary::TStringId FORTE_Wago636::scmEventOutputNames[] = { STRID(MAPO), STRID(IND) };
const CStringDictionary::TStringId FORTE_Wago636::scmEventOutputTypeIds[] = {STRID(Event), STRID(Event)};
const SAdapterInstanceDef FORTE_Wago636::scmAdapterInstances[] = {
  { STRID(WagoBusAdapter), STRID(BusAdapterOut), true },
  { STRID(WagoBusAdapter), STRID(BusAdapterIn), false }
};
const SFBInterfaceSpec FORTE_Wago636::scmFBInterfaceSpec = {
    1, scmEventInputNames, scmEventInputTypeIds, scmEIWith, scmEIWithIndexes,
    2, scmEventOutputNames, scmEventOutputTypeIds, scmEOWith, scmEOWithIndexes,
    15, scmDataInputNames, scmDataInputTypeIds,
    2, scmDataOutputNames, scmDataOutputTypeIds,
    0, nullptr,
    2, scmAdapterInstances };

FORTE_Wago636::FORTE_Wago636 (const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) : WagoSlaveBase(636, paContainer, scmFBInterfaceSpec, paInstanceNameId),
    var_QI(0_BOOL),
    var_Busy(""_STRING),
    var_LimitSwitchN(""_STRING),
    var_LimitSwitchP(""_STRING),
    var_PresetInputEnabled(""_STRING),
    var_OptimizeOn(""_STRING),
    var_ExtendedInfoOn(""_STRING),
    var_ReferenceOk(""_STRING),
    var_CurrentPosition(""_STRING),
    var_TargetPosition(""_STRING),
    var_MotorN(""_STRING),
    var_MotorP(""_STRING),
    var_Positioning(""_STRING),
    var_Preset(""_STRING),
    var_QuitErrors(""_STRING),
    var_QO(0_BOOL),
    var_STATUS(u""_WSTRING),
    var_conn_QO(var_QO),
    var_conn_STATUS(var_STATUS),
    conn_MAPO(this, 0),
    conn_IND(this, 1),
    conn_QI(nullptr),
    conn_Busy(nullptr),
    conn_LimitSwitchN(nullptr),
    conn_LimitSwitchP(nullptr),
    conn_PresetInputEnabled(nullptr),
    conn_OptimizeOn(nullptr),
    conn_ExtendedInfoOn(nullptr),
    conn_ReferenceOk(nullptr),
    conn_CurrentPosition(nullptr),
    conn_TargetPosition(nullptr),
    conn_MotorN(nullptr),
    conn_MotorP(nullptr),
    conn_Positioning(nullptr),
    conn_Preset(nullptr),
    conn_QuitErrors(nullptr),
    conn_QO(this, 0, &var_conn_QO),
    conn_STATUS(this, 1, &var_conn_STATUS) {
};

void FORTE_Wago636::setInitialValues () {
  var_QI = 0_BOOL;
  var_Busy = ""_STRING;
  var_LimitSwitchN = ""_STRING;
  var_LimitSwitchP = ""_STRING;
  var_PresetInputEnabled = ""_STRING;
  var_OptimizeOn = ""_STRING;
  var_ExtendedInfoOn = ""_STRING;
  var_ReferenceOk = ""_STRING;
  var_CurrentPosition = ""_STRING;
  var_TargetPosition = ""_STRING;
  var_MotorN = ""_STRING;
  var_MotorP = ""_STRING;
  var_Positioning = ""_STRING;
  var_Preset = ""_STRING;
  var_QuitErrors = ""_STRING;
  var_QO = 0_BOOL;
  var_STATUS = u""_WSTRING;
}

void FORTE_Wago636::readInputData (const TEventID paEIID) {
  switch (paEIID) {
    case scmEventMAPID: {
      readData(0, var_QI, conn_QI);
      readData(1, var_Busy, conn_Busy);
      readData(2, var_LimitSwitchN, conn_LimitSwitchN);
      readData(3, var_LimitSwitchP, conn_LimitSwitchP);
      readData(4, var_PresetInputEnabled, conn_PresetInputEnabled);
      readData(5, var_OptimizeOn, conn_OptimizeOn);
      readData(6, var_ExtendedInfoOn, conn_ExtendedInfoOn);
      readData(7, var_ReferenceOk, conn_ReferenceOk);
      readData(8, var_CurrentPosition, conn_CurrentPosition);
      readData(9, var_TargetPosition, conn_TargetPosition);
      readData(10, var_MotorN, conn_MotorN);
      readData(11, var_MotorP, conn_MotorP);
      readData(12, var_Positioning, conn_Positioning);
      readData(13, var_Preset, conn_Preset);
      readData(14, var_QuitErrors, conn_QuitErrors);
      break;
    }
    default:
      break;
  }
}

void FORTE_Wago636::writeOutputData (const TEventID paEIID) {
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
    default:
      break;
  }
}

CIEC_ANY* FORTE_Wago636::getDI (const size_t paIndex) {
  switch (paIndex) {
    case 0:
      return &var_QI;
    case 1:
      return &var_Busy;
    case 2:
      return &var_LimitSwitchN;
    case 3:
      return &var_LimitSwitchP;
    case 4:
      return &var_PresetInputEnabled;
    case 5:
      return &var_OptimizeOn;
    case 6:
      return &var_ExtendedInfoOn;
    case 7:
      return &var_ReferenceOk;
    case 8:
      return &var_CurrentPosition;
    case 9:
      return &var_TargetPosition;
    case 10:
      return &var_MotorN;
    case 11:
      return &var_MotorP;
    case 12:
      return &var_Positioning;
    case 13:
      return &var_Preset;
    case 14:
      return &var_QuitErrors;
  }
  return nullptr;
}

CIEC_ANY* FORTE_Wago636::getDO (const size_t paIndex) {
  switch (paIndex) {
    case 0:
      return &var_QO;
    case 1:
      return &var_STATUS;
  }
  return nullptr;
}

CEventConnection* FORTE_Wago636::getEOConUnchecked (const TPortId paIndex) {
  switch (paIndex) {
    case 0:
      return &conn_MAPO;
    case 1:
      return &conn_IND;
  }
  return nullptr;
}

CDataConnection** FORTE_Wago636::getDIConUnchecked (const TPortId paIndex) {
  switch (paIndex) {
    case 0:
      return &conn_QI;
    case 1:
      return &conn_Busy;
    case 2:
      return &conn_LimitSwitchN;
    case 3:
      return &conn_LimitSwitchP;
    case 4:
      return &conn_PresetInputEnabled;
    case 5:
      return &conn_OptimizeOn;
    case 6:
      return &conn_ExtendedInfoOn;
    case 7:
      return &conn_ReferenceOk;
    case 8:
      return &conn_CurrentPosition;
    case 9:
      return &conn_TargetPosition;
    case 10:
      return &conn_MotorN;
    case 11:
      return &conn_MotorP;
    case 12:
      return &conn_Positioning;
    case 13:
      return &conn_Preset;
    case 14:
      return &conn_QuitErrors;
  }
  return nullptr;
}

CDataConnection* FORTE_Wago636::getDOConUnchecked (const TPortId paIndex) {
  switch (paIndex) {
    case 0:
      return &conn_QO;
    case 1:
      return &conn_STATUS;
  }
  return nullptr;
}

void FORTE_Wago636::initHandlesBase (size_t paNumberOfBoolInputs, size_t paNumberOfBoolOutputs, size_t paNumberOfAnalogInputs, size_t paNumberOfAnalogOutputs) {
  size_t offset = 1; //skip QI

  if (paNumberOfBoolInputs == 7) {
    initWagoHandle(offset, 3, CIEC_ANY::e_BOOL, IOMapper::In); // busy - Status-Byte 0 Bit 3
    initWagoHandle(offset + 1, 14, CIEC_ANY::e_BOOL, IOMapper::In); // switch negative - Status-Byte 1 Bit 6
    initWagoHandle(offset + 2, 15, CIEC_ANY::e_BOOL, IOMapper::In); // switch positive - Status-Byte 1 Bit 7
    initWagoHandle(offset + 3, 8, CIEC_ANY::e_BOOL, IOMapper::In); // preset input enabled - Status-Byte 1 Bit 0
    initWagoHandle(offset + 4, 9, CIEC_ANY::e_BOOL, IOMapper::In); // optimize on - Status-Byte 1 Bit 1
    initWagoHandle(offset + 5, 10, CIEC_ANY::e_BOOL, IOMapper::In); // extended info on - Status-Byte 1 Bit 2
    initWagoHandle(offset + 6, 5, CIEC_ANY::e_BOOL, IOMapper::In); // reference ok - Status-Byte 0 Bit 5
  }
  else {
    DEVLOG_ERROR("[Wago636] only supports 7 BOOL inputs, but got %d.\n", paNumberOfBoolInputs);
  }

  offset += paNumberOfBoolInputs;

  if (paNumberOfAnalogInputs == 1) {
    initWagoHandle(offset, 16, CIEC_ANY::e_DWORD, IOMapper::In); // current position - Status-Byte 2-5
  }
  else {
    DEVLOG_ERROR("[Wago636] only supports 1 DWORD input, but got %d.\n", paNumberOfAnalogInputs);
  }

  offset += paNumberOfAnalogInputs;

  if (paNumberOfAnalogOutputs == 1) {
    initWagoHandle(offset, 16, CIEC_ANY::e_DWORD, IOMapper::Out); // target position - Status-Byte 2-5
  }
  else {
    DEVLOG_ERROR("[Wago636] only supports 1 DWORD output, but got %d.\n", paNumberOfAnalogOutputs);
  }

  offset += paNumberOfAnalogOutputs;

  if (paNumberOfBoolOutputs == 5) {
    initWagoHandle(offset, 0, CIEC_ANY::e_BOOL, IOMapper::Out); // motor negative - Control-Byte 0 Bit 0
    initWagoHandle(offset + 1, 1, CIEC_ANY::e_BOOL, IOMapper::Out); // motor positive - Control-Byte 0 Bit 1
    initWagoHandle(offset + 2, 2, CIEC_ANY::e_BOOL, IOMapper::Out); // positioning - Control-Byte 0 Bit 2
    initWagoHandle(offset + 3, 3, CIEC_ANY::e_BOOL, IOMapper::Out); // preset - Control-Byte 0 Bit 3
    initWagoHandle(offset + 4, 15, CIEC_ANY::e_BOOL, IOMapper::Out); // quit error Control-Byte 1 Bit 7
  }
  else {
    DEVLOG_ERROR("[Wago636] only supports 5 BOOL outputs, but got %d.\n", paNumberOfBoolOutputs);
  }
}
