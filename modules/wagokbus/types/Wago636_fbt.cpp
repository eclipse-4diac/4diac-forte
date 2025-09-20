/*************************************************************************
 * Copyright (c) 2025, Monika Wenger
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *************************************************************************/

#include "Wago636_fbt.h"

using namespace forte::literals;

#include "WagoSlaveBase.h"
#include "../WagoDeviceController.h"

using namespace forte::io;

DEFINE_FIRMWARE_FB(FORTE_Wago636, "Wago636"_STRID)

namespace {
  const auto cDataInputNames = std::array{"QI"_STRID,
                                          "Busy"_STRID,
                                          "LimitSwitchN"_STRID,
                                          "LimitSwitchP"_STRID,
                                          "PresetInput"_STRID,
                                          "OptimizeOnZInput"_STRID,
                                          "OnTarget"_STRID,
                                          "ReferenceOk"_STRID,
                                          "CurrentPosition"_STRID,
                                          "TargetPosition"_STRID,
                                          "MotorN"_STRID,
                                          "MotorP"_STRID,
                                          "Positioning"_STRID,
                                          "OptimizeOn"_STRID,
                                          "Preset"_STRID,
                                          "PresetInputEnable"_STRID,
                                          "QuitErrors"_STRID};
  const auto cDataOutputNames = std::array{"QO"_STRID, "STATUS"_STRID};
  const auto cEventInputNames = std::array{"MAP"_STRID};
  const auto cEventOutputNames = std::array{"MAPO"_STRID, "IND"_STRID};
  const auto cSocketNameIds = std::array{"BusAdapterIn"_STRID};
  const auto cPlugNameIds = std::array{"BusAdapterOut"_STRID};

  const SFBInterfaceSpec cFBInterfaceSpec = {
      .mEINames = cEventInputNames,
      .mEITypeNames = {},
      .mEONames = cEventOutputNames,
      .mEOTypeNames = {},
      .mDINames = cDataInputNames,
      .mDONames = cDataOutputNames,
      .mDIONames = {},
      .mSocketNames = cSocketNameIds,
      .mPlugNames = cPlugNameIds,
  };
} // namespace

FORTE_Wago636::FORTE_Wago636(const forte::StringId paInstanceNameId, CFBContainer &paContainer) :
    WagoSlaveBase(636, paContainer, cFBInterfaceSpec, paInstanceNameId),
    var_QI(0_BOOL),
    var_Busy(""_STRING),
    var_LimitSwitchN(""_STRING),
    var_LimitSwitchP(""_STRING),
    var_PresetInput(""_STRING),
    var_OptimizeOnZInput(""_STRING),
    var_OnTarget(""_STRING),
    var_ReferenceOk(""_STRING),
    var_CurrentPosition(""_STRING),
    var_TargetPosition(""_STRING),
    var_MotorN(""_STRING),
    var_MotorP(""_STRING),
    var_Positioning(""_STRING),
    var_Preset(""_STRING),
    var_PresetInputEnable(""_STRING),
    var_QuitErrors(""_STRING),
    var_QO(0_BOOL),
    var_STATUS(u""_WSTRING),
    conn_MAPO(*this, 0),
    conn_IND(*this, 1),
    conn_QI(nullptr),
    conn_Busy(nullptr),
    conn_LimitSwitchN(nullptr),
    conn_LimitSwitchP(nullptr),
    conn_PresetInput(nullptr),
    conn_OptimizeOnZInput(nullptr),
    conn_OnTarget(nullptr),
    conn_ReferenceOk(nullptr),
    conn_CurrentPosition(nullptr),
    conn_TargetPosition(nullptr),
    conn_MotorN(nullptr),
    conn_MotorP(nullptr),
    conn_Positioning(nullptr),
    conn_OptimizeOn(nullptr),
    conn_Preset(nullptr),
    conn_PresetInputEnable(nullptr),
    conn_QuitErrors(nullptr),
    conn_QO(*this, 0, var_QO),
    conn_STATUS(*this, 1, var_STATUS) {};

void FORTE_Wago636::setInitialValues() {
  var_QI = 0_BOOL;
  var_Busy = ""_STRING;
  var_LimitSwitchN = ""_STRING;
  var_LimitSwitchP = ""_STRING;
  var_PresetInput = ""_STRING;
  var_OptimizeOnZInput = ""_STRING;
  var_OnTarget = ""_STRING;
  var_ReferenceOk = ""_STRING;
  var_CurrentPosition = ""_STRING;
  var_TargetPosition = ""_STRING;
  var_MotorN = ""_STRING;
  var_MotorP = ""_STRING;
  var_Positioning = ""_STRING;
  var_OptimizeOn = ""_STRING;
  var_Preset = ""_STRING;
  var_PresetInputEnable = ""_STRING;
  var_QuitErrors = ""_STRING;
  var_QO = 0_BOOL;
  var_STATUS = u""_WSTRING;
}

void FORTE_Wago636::readInputData(const TEventID paEIID) {
  switch (paEIID) {
    case scmEventMAPID: {
      readData(0, var_QI, conn_QI);
      readData(1, var_Busy, conn_Busy);
      readData(2, var_LimitSwitchN, conn_LimitSwitchN);
      readData(3, var_LimitSwitchP, conn_LimitSwitchP);
      readData(4, var_PresetInput, conn_PresetInput);
      readData(5, var_OptimizeOnZInput, conn_OptimizeOnZInput);
      readData(6, var_OnTarget, conn_OnTarget);
      readData(7, var_ReferenceOk, conn_ReferenceOk);
      readData(8, var_CurrentPosition, conn_CurrentPosition);
      readData(9, var_TargetPosition, conn_TargetPosition);
      readData(10, var_MotorN, conn_MotorN);
      readData(11, var_MotorP, conn_MotorP);
      readData(12, var_Positioning, conn_Positioning);
      readData(13, var_OptimizeOn, conn_OptimizeOn);
      readData(14, var_Preset, conn_Preset);
      readData(15, var_PresetInputEnable, conn_PresetInputEnable);
      readData(16, var_QuitErrors, conn_QuitErrors);
      break;
    }
    default: break;
  }
}

void FORTE_Wago636::writeOutputData(const TEventID paEIID) {
  switch (paEIID) {
    case scmEventMAPOID: {
      writeData(cFBInterfaceSpec.getNumDIs() + 0, var_QO, conn_QO);
      break;
    }
    case scmEventINDID: {
      writeData(cFBInterfaceSpec.getNumDIs() + 0, var_QO, conn_QO);
      writeData(cFBInterfaceSpec.getNumDIs() + 1, var_STATUS, conn_STATUS);
      break;
    }
    default: break;
  }
}

CIEC_ANY *FORTE_Wago636::getDI(const size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_QI;
    case 1: return &var_Busy;
    case 2: return &var_LimitSwitchN;
    case 3: return &var_LimitSwitchP;
    case 4: return &var_PresetInput;
    case 5: return &var_OptimizeOnZInput;
    case 6: return &var_OnTarget;
    case 7: return &var_ReferenceOk;
    case 8: return &var_CurrentPosition;
    case 9: return &var_TargetPosition;
    case 10: return &var_MotorN;
    case 11: return &var_MotorP;
    case 12: return &var_Positioning;
    case 13: return &var_OptimizeOn;
    case 14: return &var_Preset;
    case 15: return &var_PresetInputEnable;
    case 16: return &var_QuitErrors;
  }
  return nullptr;
}

CIEC_ANY *FORTE_Wago636::getDO(const size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_QO;
    case 1: return &var_STATUS;
  }
  return nullptr;
}

CEventConnection *FORTE_Wago636::getEOConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_MAPO;
    case 1: return &conn_IND;
  }
  return nullptr;
}

CDataConnection **FORTE_Wago636::getDIConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_QI;
    case 1: return &conn_Busy;
    case 2: return &conn_LimitSwitchN;
    case 3: return &conn_LimitSwitchP;
    case 4: return &conn_PresetInput;
    case 5: return &conn_OptimizeOnZInput;
    case 6: return &conn_OnTarget;
    case 7: return &conn_ReferenceOk;
    case 8: return &conn_CurrentPosition;
    case 9: return &conn_TargetPosition;
    case 10: return &conn_MotorN;
    case 11: return &conn_MotorP;
    case 12: return &conn_Positioning;
    case 13: return &conn_OptimizeOn;
    case 14: return &conn_Preset;
    case 15: return &conn_PresetInputEnable;
    case 16: return &conn_QuitErrors;
  }
  return nullptr;
}

CDataConnection *FORTE_Wago636::getDOConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_QO;
    case 1: return &conn_STATUS;
  }
  return nullptr;
}

void FORTE_Wago636::initHandlesBase(size_t paNumberOfBoolInputs,
                                    size_t paNumberOfBoolOutputs,
                                    size_t paNumberOfAnalogInputs,
                                    size_t paNumberOfAnalogOutputs) {
  size_t offset = 1; // skip QI

  if (paNumberOfBoolInputs == 7) {
    initWagoHandle(offset, 3, CIEC_ANY::e_BOOL, IOMapper::In); // busy - Status-Byte 0 Bit 3
    initWagoHandle(1 + offset, 14, CIEC_ANY::e_BOOL,
                   IOMapper::In); // limit switch negative (Hardware pin) - Status-Byte 1 Bit 6
    initWagoHandle(2 + offset, 15, CIEC_ANY::e_BOOL,
                   IOMapper::In); // limit switch positive (Hardware pin) - Status-Byte 1 Bit 7
    initWagoHandle(3 + offset, 13, CIEC_ANY::e_BOOL, IOMapper::In); // preset input (Hardware pin) - Status-Byte 1 Bit 0
    initWagoHandle(4 + offset, 9, CIEC_ANY::e_BOOL,
                   IOMapper::In); // optimize on / z input (Hardware pin) - Status-Byte 1 Bit 1
    initWagoHandle(5 + offset, 2, CIEC_ANY::e_BOOL, IOMapper::In); // on target - Status-Byte 0 Bit 2
    initWagoHandle(6 + offset, 5, CIEC_ANY::e_BOOL, IOMapper::In); // reference ok - Status-Byte 0 Bit 5
  } else {
    DEVLOG_ERROR("[Wago636] only supports 7 BOOL inputs, but got %d.\n", paNumberOfBoolInputs);
  }

  offset += paNumberOfBoolInputs;

  if (paNumberOfAnalogInputs == 1) {
    initWagoHandle(offset, 16, CIEC_ANY::e_DWORD, IOMapper::In); // current position - Status-Byte 2-5
  } else {
    DEVLOG_ERROR("[Wago636] only supports 1 DWORD input, but got %d.\n", paNumberOfAnalogInputs);
  }

  offset += paNumberOfAnalogInputs;

  if (paNumberOfAnalogOutputs == 1) {
    initWagoHandle(offset, 16, CIEC_ANY::e_DWORD, IOMapper::Out); // target position - Status-Byte 2-5
  } else {
    DEVLOG_ERROR("[Wago636] only supports 1 DWORD output, but got %d.\n", paNumberOfAnalogOutputs);
  }

  offset += paNumberOfAnalogOutputs;

  if (paNumberOfBoolOutputs == 7) {
    initWagoHandle(offset, 0, CIEC_ANY::e_BOOL, IOMapper::Out); // motor negative - Control-Byte 0 Bit 0
    initWagoHandle(1 + offset, 1, CIEC_ANY::e_BOOL, IOMapper::Out); // motor positive - Control-Byte 0 Bit 1
    initWagoHandle(2 + offset, 2, CIEC_ANY::e_BOOL, IOMapper::Out); // positioning - Control-Byte 0 Bit 2
    initWagoHandle(3 + offset, 9, CIEC_ANY::e_BOOL, IOMapper::Out); // optimize on - Control-Byte 1 Bit 1
    initWagoHandle(4 + offset, 3, CIEC_ANY::e_BOOL, IOMapper::Out); // preset - Control-Byte 0 Bit 3
    initWagoHandle(5 + offset, 8, CIEC_ANY::e_BOOL, IOMapper::Out); // preset input enable - Control-Byte 1 Bit 0
    initWagoHandle(6 + offset, 15, CIEC_ANY::e_BOOL, IOMapper::Out); // quit error Control-Byte 1 Bit 7
  } else {
    DEVLOG_ERROR("[Wago636] only supports 7 BOOL outputs, but got %d.\n", paNumberOfBoolOutputs);
  }
}
