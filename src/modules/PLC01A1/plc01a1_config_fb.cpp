/*******************************************************************************
 * Copyright (c) 2019 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Jose Cabral - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "plc01a1_config_fb.h"

USE_STRING_ID(BOOL);
USE_STRING_ID(EInit);
USE_STRING_ID(Event);
USE_STRING_ID(IN1);
USE_STRING_ID(IN2);
USE_STRING_ID(IN3);
USE_STRING_ID(IN4);
USE_STRING_ID(IN5);
USE_STRING_ID(IN6);
USE_STRING_ID(IN7);
USE_STRING_ID(IN8);
USE_STRING_ID(IND);
USE_STRING_ID(INIT);
USE_STRING_ID(INITO);
USE_STRING_ID(OUT1);
USE_STRING_ID(OUT2);
USE_STRING_ID(OUT3);
USE_STRING_ID(OUT4);
USE_STRING_ID(OUT5);
USE_STRING_ID(OUT6);
USE_STRING_ID(OUT7);
USE_STRING_ID(OUT8);
USE_STRING_ID(PLC01A1);
USE_STRING_ID(QI);
USE_STRING_ID(QO);
USE_STRING_ID(STATUS);
USE_STRING_ID(UINT);
USE_STRING_ID(UpdateInterval);
USE_STRING_ID(WSTRING);


#include "forte_uint.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"
#include "plc01a1_controller.h"

DEFINE_FIRMWARE_FB(PLC01A1ConfigFB, STRID(PLC01A1))


const CStringDictionary::TStringId PLC01A1ConfigFB::scmDataInputNames[] = { STRID(QI), STRID(IN1), STRID(IN2), STRID(IN3), STRID(IN4),
  STRID(IN5), STRID(IN6), STRID(IN7), STRID(IN8), STRID(OUT1), STRID(OUT2), STRID(OUT3), STRID(OUT4), STRID(OUT5),
  STRID(OUT6), STRID(OUT7), STRID(OUT8), STRID(UpdateInterval) };

const CStringDictionary::TStringId PLC01A1ConfigFB::scmDataInputTypeIds[] = { STRID(BOOL), STRID(WSTRING), STRID(WSTRING), STRID(WSTRING),
  STRID(WSTRING), STRID(WSTRING), STRID(WSTRING), STRID(WSTRING), STRID(WSTRING), STRID(WSTRING), STRID(WSTRING),
  STRID(WSTRING), STRID(WSTRING), STRID(WSTRING), STRID(WSTRING), STRID(WSTRING), STRID(WSTRING), STRID(UINT) };

const CStringDictionary::TStringId PLC01A1ConfigFB::scmDataOutputNames[] = { STRID(QO), STRID(STATUS) };
const CStringDictionary::TStringId PLC01A1ConfigFB::scmDataOutputTypeIds[] = { STRID(BOOL), STRID(WSTRING) };
const TForteInt16 PLC01A1ConfigFB::scmEIWithIndexes[] = { 0 };
const TDataIOID PLC01A1ConfigFB::scmEIWith[] = { 0, 17, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, scmWithListDelimiter };
const TForteInt16 PLC01A1ConfigFB::scmEIWithIndexes[] = {0};
const CStringDictionary::TStringId PLC01A1ConfigFB::scmEventInputNames[] = { STRID(INIT) };
const CStringDictionary::TStringId PLC01A1ConfigFB::scmEventInputTypeIds[] = {STRID(EInit)};
const TDataIOID PLC01A1ConfigFB::scmEOWith[] = { 0, 1, scmWithListDelimiter, 0, 1, scmWithListDelimiter };
const TForteInt16 PLC01A1ConfigFB::scmEOWithIndexes[] = { 0, 3, -1 };
const CStringDictionary::TStringId PLC01A1ConfigFB::scmEventOutputNames[] = { STRID(INITO), STRID(IND) };
const CStringDictionary::TStringId PLC01A1ConfigFB::scmEventOutputTypeIds[] = {STRID(Event), STRID(Event)};
const SFBInterfaceSpec PLC01A1ConfigFB::scmFBInterfaceSpec = {
  1,  scmEventInputNames, scmEventInputTypeIds,  scmEIWith,  scmEIWithIndexes,
  2,  scmEventOutputNames, scmEventOutputTypeIds,  scmEOWith, scmEOWithIndexes,  18,  scmDataInputNames, scmDataInputTypeIds,
  18, scmDataInputNames, scmDataInputTypeIds,
  2, scmDataOutputNames, scmDataOutputTypeIds,
  0, nullptr,
  0, nullptr
};

PLC01A1ConfigFB::PLC01A1ConfigFB(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    CFunctionBlock(paContainer, scmFBInterfaceSpec, paInstanceNameId),
    var_QI(0_BOOL),
    var_IN1(u""_WSTRING),
    var_IN2(u""_WSTRING),
    var_IN3(u""_WSTRING),
    var_IN4(u""_WSTRING),
    var_IN5(u""_WSTRING),
    var_IN6(u""_WSTRING),
    var_IN7(u""_WSTRING),
    var_IN8(u""_WSTRING),
    var_OUT1(u""_WSTRING),
    var_OUT2(u""_WSTRING),
    var_OUT3(u""_WSTRING),
    var_OUT4(u""_WSTRING),
    var_OUT5(u""_WSTRING),
    var_OUT6(u""_WSTRING),
    var_OUT7(u""_WSTRING),
    var_OUT8(u""_WSTRING),
    var_UpdateInterval(25_UINT),
    var_QO(0_BOOL),
    var_STATUS(u""_WSTRING),
    var_conn_QO(var_QO),
    var_conn_STATUS(var_STATUS),
    conn_INITO(this, 0),
    conn_IND(this, 1),
    conn_QI(nullptr),
    conn_IN1(nullptr),
    conn_IN2(nullptr),
    conn_IN3(nullptr),
    conn_IN4(nullptr),
    conn_IN5(nullptr),
    conn_IN6(nullptr),
    conn_IN7(nullptr),
    conn_IN8(nullptr),
    conn_OUT1(nullptr),
    conn_OUT2(nullptr),
    conn_OUT3(nullptr),
    conn_OUT4(nullptr),
    conn_OUT5(nullptr),
    conn_OUT6(nullptr),
    conn_OUT7(nullptr),
    conn_OUT8(nullptr),
    conn_UpdateInterval(nullptr),
    conn_QO(this, 0, &var_conn_QO),
    conn_STATUS(this, 1, &var_conn_STATUS) {
};

void PLC01A1ConfigFB::setInitialValues() {
  var_QI = 0_BOOL;
  var_IN1 = u""_WSTRING;
  var_IN2 = u""_WSTRING;
  var_IN3 = u""_WSTRING;
  var_IN4 = u""_WSTRING;
  var_IN5 = u""_WSTRING;
  var_IN6 = u""_WSTRING;
  var_IN7 = u""_WSTRING;
  var_IN8 = u""_WSTRING;
  var_OUT1 = u""_WSTRING;
  var_OUT2 = u""_WSTRING;
  var_OUT3 = u""_WSTRING;
  var_OUT4 = u""_WSTRING;
  var_OUT5 = u""_WSTRING;
  var_OUT6 = u""_WSTRING;
  var_OUT7 = u""_WSTRING;
  var_OUT8 = u""_WSTRING;
  var_UpdateInterval = 25_UINT;
  var_QO = 0_BOOL;
  var_STATUS = u""_WSTRING;
}

void PLC01A1ConfigFB::executeEvent(const TEventID paEIID, CEventChainExecutionThread *const paECET) {
  switch(paEIID) {
    case scmEventINITID:
      #error add code for INIT event!
          sendOutputEvent(scmEventCNFID, paECET);
      break;
  }
}

forte::core::io::IODeviceController* PLC01A1ConfigFB::createDeviceController(CDeviceExecution &paDeviceExecution) {
  return new PLC01A1Controller(paDeviceExecution);
}

void PLC01A1ConfigFB::setConfig() {
  PLC01A1Controller::Config config;
  config.mUpdateInterval = UpdateInterval();
  getDeviceController()->setConfig(&config);
}

void PLC01A1ConfigFB::onStartup() {
  // Initialize handles
  size_t initialDIOffset = 1;
  size_t numberOfInputs = 8;
  size_t numberOfOutputs = 8;

  for(size_t i = 0; i < numberOfInputs; i++) {
    PLC01A1Controller::HandleDescriptor desc = PLC01A1Controller::HandleDescriptor(*static_cast<CIEC_WSTRING*>(getDI(initialDIOffset + i)),
      forte::core::io::IOMapper::In,
      0 /*offset is always 0 */,
      static_cast<uint8_t>(i));
    initHandle(desc);
  }

  for(size_t i = 0; i < numberOfOutputs; i++) {
    PLC01A1Controller::HandleDescriptor
    desc = PLC01A1Controller::HandleDescriptor(*static_cast<CIEC_WSTRING*>(getDI(initialDIOffset + numberOfInputs + i)),
      forte::core::io::IOMapper::Out, 0 /*offset is always 0 */, static_cast<uint8_t>(numberOfOutputs - i - 1));
    initHandle(desc);
  }

  started();
}

void PLC01A1ConfigFB::readInputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventINITID: {
      readData(0, var_QI, conn_QI);
      readData(17, var_UpdateInterval, conn_UpdateInterval);
      readData(1, var_IN1, conn_IN1);
      readData(2, var_IN2, conn_IN2);
      readData(3, var_IN3, conn_IN3);
      readData(4, var_IN4, conn_IN4);
      readData(5, var_IN5, conn_IN5);
      readData(6, var_IN6, conn_IN6);
      readData(7, var_IN7, conn_IN7);
      readData(8, var_IN8, conn_IN8);
      readData(9, var_OUT1, conn_OUT1);
      readData(10, var_OUT2, conn_OUT2);
      readData(11, var_OUT3, conn_OUT3);
      readData(12, var_OUT4, conn_OUT4);
      readData(13, var_OUT5, conn_OUT5);
      readData(14, var_OUT6, conn_OUT6);
      readData(15, var_OUT7, conn_OUT7);
      readData(16, var_OUT8, conn_OUT8);
      break;
    }
    default:
      break;
  }
}

void PLC01A1ConfigFB::writeOutputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventINITOID: {
      writeData(0, var_QO, conn_QO);
      writeData(1, var_STATUS, conn_STATUS);
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

CIEC_ANY *PLC01A1ConfigFB::getDI(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_QI;
    case 1: return &var_IN1;
    case 2: return &var_IN2;
    case 3: return &var_IN3;
    case 4: return &var_IN4;
    case 5: return &var_IN5;
    case 6: return &var_IN6;
    case 7: return &var_IN7;
    case 8: return &var_IN8;
    case 9: return &var_OUT1;
    case 10: return &var_OUT2;
    case 11: return &var_OUT3;
    case 12: return &var_OUT4;
    case 13: return &var_OUT5;
    case 14: return &var_OUT6;
    case 15: return &var_OUT7;
    case 16: return &var_OUT8;
    case 17: return &var_UpdateInterval;
  }
  return nullptr;
}

CIEC_ANY *PLC01A1ConfigFB::getDO(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_QO;
    case 1: return &var_STATUS;
  }
  return nullptr;
}

CEventConnection *PLC01A1ConfigFB::getEOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_INITO;
    case 1: return &conn_IND;
  }
  return nullptr;
}

CDataConnection **PLC01A1ConfigFB::getDIConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_QI;
    case 1: return &conn_IN1;
    case 2: return &conn_IN2;
    case 3: return &conn_IN3;
    case 4: return &conn_IN4;
    case 5: return &conn_IN5;
    case 6: return &conn_IN6;
    case 7: return &conn_IN7;
    case 8: return &conn_IN8;
    case 9: return &conn_OUT1;
    case 10: return &conn_OUT2;
    case 11: return &conn_OUT3;
    case 12: return &conn_OUT4;
    case 13: return &conn_OUT5;
    case 14: return &conn_OUT6;
    case 15: return &conn_OUT7;
    case 16: return &conn_OUT8;
    case 17: return &conn_UpdateInterval;
  }
  return nullptr;
}

CDataConnection *PLC01A1ConfigFB::getDOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_QO;
    case 1: return &conn_STATUS;
  }
  return nullptr;
}
