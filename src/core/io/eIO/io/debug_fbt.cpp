/*******************************************************************************
 * Copyright (c) 2025 Maximilian Scharf
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Maximilian Scharf - Initial implementation.
 *******************************************************************************/

#include "debug_fbt.h"

#include "core/iec61131_functions.h"
#include "core/datatypes/forte_array_common.h"
#include "core/datatypes/forte_array.h"
#include "core/datatypes/forte_array_fixed.h"
#include "core/datatypes/forte_array_variable.h"

USE_STRING_ID(CNF);
USE_STRING_ID(Event);
USE_STRING_ID(INIT);
USE_STRING_ID(PARAM);
USE_STRING_ID(RD);
USE_STRING_ID(SD);
USE_STRING_ID(STRING);
USE_STRING_ID(WORD);
USE_STRING_ID(debug);

DEFINE_FIRMWARE_FB(FORTE_debug, STRID(debug))

const CStringDictionary::TStringId FORTE_debug::scmDataInputNames[] = {STRID(SD), STRID(PARAM)};
const CStringDictionary::TStringId FORTE_debug::scmDataInputTypeIds[] = {STRID(WORD), STRID(STRING)};
const CStringDictionary::TStringId FORTE_debug::scmDataOutputNames[] = {STRID(RD)};
const CStringDictionary::TStringId FORTE_debug::scmDataOutputTypeIds[] = {STRID(WORD)};
const TDataIOID FORTE_debug::scmEIWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_debug::scmEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_debug::scmEventInputNames[] = {STRID(INIT)};
const TDataIOID FORTE_debug::scmEOWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_debug::scmEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_debug::scmEventOutputNames[] = {STRID(CNF)};
const SFBInterfaceSpec FORTE_debug::scmFBInterfaceSpec = {
  1, scmEventInputNames, nullptr, scmEIWith, scmEIWithIndexes,
  1, scmEventOutputNames, nullptr, scmEOWith, scmEOWithIndexes,
  2, scmDataInputNames, scmDataInputTypeIds,
  1, scmDataOutputNames, scmDataOutputTypeIds,
  0, nullptr,
  0, nullptr
};

FORTE_debug::FORTE_debug(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    CFunctionBlock(paContainer, scmFBInterfaceSpec, paInstanceNameId),
    var_SD(0_WORD),
    var_PARAM(""_STRING),
    var_RD(0_WORD),
    conn_CNF(*this, 0),
    conn_SD(nullptr),
    conn_PARAM(nullptr),
    conn_RD(*this, 0, var_RD) {
};

void FORTE_debug::setInitialValues() {
  var_SD = 0_WORD;
  var_PARAM = ""_STRING;
  var_RD = 0_WORD;
}

void FORTE_debug::executeEvent(const TEventID paEIID, CEventChainExecutionThread *const paECET) {
  switch(paEIID) {
    case scmEventINITID:
      size_t paBufferSize = 30;
      char paValue[paBufferSize];
      var_SD.toString(paValue, paBufferSize);
      DEVLOG_DEBUG("[DEBUG::%s] value: %s\r\n", var_PARAM.c_str(), paValue);
      var_RD = var_SD;
      sendOutputEvent(scmEventINITID, paECET);
      break;
  }
}

void FORTE_debug::readInputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventINITID: {
      readData(0, var_SD, conn_SD);
      break;
    }
    default:
      break;
  }
}

void FORTE_debug::writeOutputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventCNFID: {
      writeData(0, var_RD, conn_RD);
      break;
    }
    default:
      break;
  }
}

CIEC_ANY *FORTE_debug::getDI(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_SD;
    case 1: return &var_PARAM;
  }
  return nullptr;
}

CIEC_ANY *FORTE_debug::getDO(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_RD;
  }
  return nullptr;
}

CEventConnection *FORTE_debug::getEOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_debug::getDIConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_SD;
    case 1: return &conn_PARAM;
  }
  return nullptr;
}

CDataConnection *FORTE_debug::getDOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_RD;
  }
  return nullptr;
}
