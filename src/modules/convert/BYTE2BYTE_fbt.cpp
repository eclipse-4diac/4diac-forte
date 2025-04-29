/*******************************************************************************
 * Copyright (c) 2023 Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Martin Jobst
 *     - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "BYTE2BYTE_fbt.h"

#include "core/datatypes/forte_byte.h"
#include "core/iec61131_functions.h"
#include "core/datatypes/forte_array_common.h"
#include "core/datatypes/forte_array.h"
#include "core/datatypes/forte_array_fixed.h"
#include "core/datatypes/forte_array_variable.h"

USE_STRING_ID(BYTE);
USE_STRING_ID(BYTE2BYTE);
USE_STRING_ID(CNF);
USE_STRING_ID(Event);
USE_STRING_ID(IN);
USE_STRING_ID(OUT);
USE_STRING_ID(REQ);

DEFINE_FIRMWARE_FB(FORTE_BYTE2BYTE, STRID(BYTE2BYTE))

const CStringDictionary::TStringId FORTE_BYTE2BYTE::scmDataInputNames[] = {STRID(IN)};
const CStringDictionary::TStringId FORTE_BYTE2BYTE::scmDataInputTypeIds[] = {STRID(BYTE)};
const CStringDictionary::TStringId FORTE_BYTE2BYTE::scmDataOutputNames[] = {STRID(OUT)};
const CStringDictionary::TStringId FORTE_BYTE2BYTE::scmDataOutputTypeIds[] = {STRID(BYTE)};
const TDataIOID FORTE_BYTE2BYTE::scmEIWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_BYTE2BYTE::scmEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_BYTE2BYTE::scmEventInputNames[] = {STRID(REQ)};
const TDataIOID FORTE_BYTE2BYTE::scmEOWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_BYTE2BYTE::scmEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_BYTE2BYTE::scmEventOutputNames[] = {STRID(CNF)};
const SFBInterfaceSpec FORTE_BYTE2BYTE::scmFBInterfaceSpec = {
  1, scmEventInputNames, nullptr, scmEIWith, scmEIWithIndexes,
  1, scmEventOutputNames, nullptr, scmEOWith, scmEOWithIndexes,
  1, scmDataInputNames, scmDataInputTypeIds,
  1, scmDataOutputNames, scmDataOutputTypeIds,
  0, nullptr,
  0, nullptr
};

FORTE_BYTE2BYTE::FORTE_BYTE2BYTE(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    CSimpleFB(paContainer, scmFBInterfaceSpec, paInstanceNameId, nullptr),
    var_IN(0_BYTE),
    var_OUT(0_BYTE),
    conn_CNF(*this, 0),
    conn_IN(nullptr),
    conn_OUT(*this, 0, var_OUT) {
}

void FORTE_BYTE2BYTE::setInitialValues() {
  var_IN = 0_BYTE;
  var_OUT = 0_BYTE;
}

void FORTE_BYTE2BYTE::executeEvent(const TEventID paEIID, CEventChainExecutionThread *const paECET) {
  switch(paEIID) {
    case scmEventREQID:
      enterStateREQ(paECET);
      break;
    default:
      break;
  }
}

void FORTE_BYTE2BYTE::enterStateREQ(CEventChainExecutionThread *const paECET) {
  alg_REQ();
  sendOutputEvent(scmEventCNFID, paECET);
}

void FORTE_BYTE2BYTE::readInputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventREQID: {
      readData(0, var_IN, conn_IN);
      break;
    }
    default:
      break;
  }
}

void FORTE_BYTE2BYTE::writeOutputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventCNFID: {
      writeData(scmFBInterfaceSpec.mNumDIs + 0, var_OUT, conn_OUT);
      break;
    }
    default:
      break;
  }
}

CIEC_ANY *FORTE_BYTE2BYTE::getDI(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_IN;
  }
  return nullptr;
}

CIEC_ANY *FORTE_BYTE2BYTE::getDO(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_OUT;
  }
  return nullptr;
}

CEventConnection *FORTE_BYTE2BYTE::getEOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_BYTE2BYTE::getDIConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_IN;
  }
  return nullptr;
}

CDataConnection *FORTE_BYTE2BYTE::getDOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_OUT;
  }
  return nullptr;
}

CIEC_ANY *FORTE_BYTE2BYTE::getVarInternal(size_t) {
  return nullptr;
}

void FORTE_BYTE2BYTE::alg_REQ(void) {

  #line 2 "BYTE2BYTE.fbt"
  var_OUT = var_IN;
}
