/*************************************************************************
 * Copyright (c) 2016, 2025 fortiss GmbH, Monika Wenger
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *************************************************************************/

#include "Wago459.h"

USE_STRING_ID(AnalogInput_1);
USE_STRING_ID(AnalogInput_2);
USE_STRING_ID(AnalogInput_3);
USE_STRING_ID(AnalogInput_4);
USE_STRING_ID(BOOL);
USE_STRING_ID(BusAdapterIn);
USE_STRING_ID(BusAdapterOut);
USE_STRING_ID(Event);
USE_STRING_ID(IND);
USE_STRING_ID(MAP);
USE_STRING_ID(MAPO);
USE_STRING_ID(QI);
USE_STRING_ID(QO);
USE_STRING_ID(STATUS);
USE_STRING_ID(STRING);
USE_STRING_ID(Wago459);
USE_STRING_ID(WagoBusAdapter);
USE_STRING_ID(WSTRING);

DEFINE_FIRMWARE_FB(FORTE_Wago459, STRID(Wago459))

const CStringDictionary::TStringId FORTE_Wago459::scmDataInputNames[] = {
    STRID(QI), STRID(AnalogInput_1), STRID(AnalogInput_2), STRID(AnalogInput_3), STRID(AnalogInput_4)};
const CStringDictionary::TStringId FORTE_Wago459::scmDataInputTypeIds[] = {STRID(BOOL), STRID(STRING), STRID(STRING),
                                                                           STRID(STRING), STRID(STRING)};
const CStringDictionary::TStringId FORTE_Wago459::scmDataOutputNames[] = {STRID(QO), STRID(STATUS)};
const CStringDictionary::TStringId FORTE_Wago459::scmDataOutputTypeIds[] = {STRID(BOOL), STRID(WSTRING)};
const TDataIOID FORTE_Wago459::scmEIWith[] = {1, 2, 3, 4, 0, scmWithListDelimiter};
const TForteInt16 FORTE_Wago459::scmEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_Wago459::scmEventInputNames[] = {STRID(MAP)};
const CStringDictionary::TStringId FORTE_Wago459::scmEventInputTypeIds[] = {STRID(Event)};
const TDataIOID FORTE_Wago459::scmEOWith[] = {0, scmWithListDelimiter, 0, 1, scmWithListDelimiter};
const TForteInt16 FORTE_Wago459::scmEOWithIndexes[] = {0, 2};
const CStringDictionary::TStringId FORTE_Wago459::scmEventOutputNames[] = {STRID(MAPO), STRID(IND)};
const CStringDictionary::TStringId FORTE_Wago459::scmEventOutputTypeIds[] = {STRID(Event), STRID(Event)};
const SAdapterInstanceDef FORTE_Wago459::scmAdapterInstances[] = {{STRID(WagoBusAdapter), STRID(BusAdapterOut), true},
                                                                  {STRID(WagoBusAdapter), STRID(BusAdapterIn), false}};

namespace {
  const auto cSocketNameIds = std::array{STRID(BusAdapterIn)};
  const auto cPlugNameIds = std::array{STRID(BusAdapterOut)};
} // namespace

const SFBInterfaceSpec FORTE_Wago459::scmFBInterfaceSpec = {1,
                                                            scmEventInputNames,
                                                            scmEventInputTypeIds,
                                                            scmEIWith,
                                                            scmEIWithIndexes,
                                                            2,
                                                            scmEventOutputNames,
                                                            scmEventOutputTypeIds,
                                                            scmEOWith,
                                                            scmEOWithIndexes,
                                                            5,
                                                            scmDataInputNames,
                                                            scmDataInputTypeIds,
                                                            2,
                                                            scmDataOutputNames,
                                                            scmDataOutputTypeIds,
                                                            0,
                                                            nullptr,
                                                            2,
                                                            scmAdapterInstances,
                                                            cSocketNameIds,
                                                            cPlugNameIds};

FORTE_Wago459::FORTE_Wago459(const CStringDictionary::TStringId paInstanceNameId,
                             forte::core::CFBContainer &paContainer) :
    WagoSlaveBase(459, paContainer, scmFBInterfaceSpec, paInstanceNameId),
    var_QI(0_BOOL),
    var_AnalogInput_1(""_STRING),
    var_AnalogInput_2(""_STRING),
    var_AnalogInput_3(""_STRING),
    var_AnalogInput_4(""_STRING),
    var_QO(0_BOOL),
    var_STATUS(u""_WSTRING),
    conn_MAPO(*this, 0),
    conn_IND(*this, 1),
    conn_QI(nullptr),
    conn_AnalogInput_1(nullptr),
    conn_AnalogInput_2(nullptr),
    conn_AnalogInput_3(nullptr),
    conn_AnalogInput_4(nullptr),
    conn_QO(*this, 0, var_QO),
    conn_STATUS(*this, 1, var_STATUS) {};

void FORTE_Wago459::setInitialValues() {
  var_QI = 0_BOOL;
  var_AnalogInput_1 = ""_STRING;
  var_AnalogInput_2 = ""_STRING;
  var_AnalogInput_3 = ""_STRING;
  var_AnalogInput_4 = ""_STRING;
  var_QO = 0_BOOL;
  var_STATUS = u""_WSTRING;
}

void FORTE_Wago459::readInputData(const TEventID paEIID) {
  switch (paEIID) {
    case scmEventMAPID: {
      readData(1, var_AnalogInput_1, conn_AnalogInput_1);
      readData(2, var_AnalogInput_2, conn_AnalogInput_2);
      readData(3, var_AnalogInput_3, conn_AnalogInput_3);
      readData(4, var_AnalogInput_4, conn_AnalogInput_4);
      readData(0, var_QI, conn_QI);
      break;
    }
    default: break;
  }
}

void FORTE_Wago459::writeOutputData(const TEventID paEIID) {
  switch (paEIID) {
    case scmEventMAPOID: {
      writeData(scmFBInterfaceSpec.mNumDIs + 0, var_QO, conn_QO);
      break;
    }
    case scmEventINDID: {
      writeData(scmFBInterfaceSpec.mNumDIs + 0, var_QO, conn_QO);
      writeData(scmFBInterfaceSpec.mNumDIs + 1, var_STATUS, conn_STATUS);
      break;
    }
    default: break;
  }
}

CIEC_ANY *FORTE_Wago459::getDI(const size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_QI;
    case 1: return &var_AnalogInput_1;
    case 2: return &var_AnalogInput_2;
    case 3: return &var_AnalogInput_3;
    case 4: return &var_AnalogInput_4;
  }
  return nullptr;
}

CIEC_ANY *FORTE_Wago459::getDO(const size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_QO;
    case 1: return &var_STATUS;
  }
  return nullptr;
}

CEventConnection *FORTE_Wago459::getEOConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_MAPO;
    case 1: return &conn_IND;
  }
  return nullptr;
}

CDataConnection **FORTE_Wago459::getDIConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_QI;
    case 1: return &conn_AnalogInput_1;
    case 2: return &conn_AnalogInput_2;
    case 3: return &conn_AnalogInput_3;
    case 4: return &conn_AnalogInput_4;
  }
  return nullptr;
}

CDataConnection *FORTE_Wago459::getDOConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_QO;
    case 1: return &conn_STATUS;
  }
  return nullptr;
}
