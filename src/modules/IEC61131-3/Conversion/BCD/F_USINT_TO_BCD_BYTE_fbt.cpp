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

#include "F_USINT_TO_BCD_BYTE_fbt.h"

#include "core/datatypes/forte_byte.h"
#include "core/datatypes/forte_usint.h"
#include "core/iec61131_functions.h"
#include "core/datatypes/forte_array_common.h"
#include "core/datatypes/forte_array.h"
#include "core/datatypes/forte_array_fixed.h"
#include "core/datatypes/forte_array_variable.h"

USE_STRING_ID(BYTE);
USE_STRING_ID(CNF);
USE_STRING_ID(Event);
USE_STRING_ID(F_USINT_TO_BCD_BYTE);
USE_STRING_ID(IN);
USE_STRING_ID(OUT);
USE_STRING_ID(REQ);
USE_STRING_ID(USINT);

DEFINE_FIRMWARE_FB(FORTE_F_USINT_TO_BCD_BYTE, STRID(F_USINT_TO_BCD_BYTE))

const CStringDictionary::TStringId FORTE_F_USINT_TO_BCD_BYTE::scmDataInputNames[] = {STRID(IN)};
const CStringDictionary::TStringId FORTE_F_USINT_TO_BCD_BYTE::scmDataInputTypeIds[] = {STRID(USINT)};
const CStringDictionary::TStringId FORTE_F_USINT_TO_BCD_BYTE::scmDataOutputNames[] = {STRID(OUT)};
const CStringDictionary::TStringId FORTE_F_USINT_TO_BCD_BYTE::scmDataOutputTypeIds[] = {STRID(BYTE)};
const TDataIOID FORTE_F_USINT_TO_BCD_BYTE::scmEIWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_F_USINT_TO_BCD_BYTE::scmEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_F_USINT_TO_BCD_BYTE::scmEventInputNames[] = {STRID(REQ)};
const TDataIOID FORTE_F_USINT_TO_BCD_BYTE::scmEOWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_F_USINT_TO_BCD_BYTE::scmEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_F_USINT_TO_BCD_BYTE::scmEventOutputNames[] = {STRID(CNF)};
const SFBInterfaceSpec FORTE_F_USINT_TO_BCD_BYTE::scmFBInterfaceSpec = {
  1, scmEventInputNames, nullptr, scmEIWith, scmEIWithIndexes,
  1, scmEventOutputNames, nullptr, scmEOWith, scmEOWithIndexes,
  1, scmDataInputNames, scmDataInputTypeIds,
  1, scmDataOutputNames, scmDataOutputTypeIds,
  0, nullptr,
  0, nullptr
};

FORTE_F_USINT_TO_BCD_BYTE::FORTE_F_USINT_TO_BCD_BYTE(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    CSimpleFB(paContainer, scmFBInterfaceSpec, paInstanceNameId, nullptr),
    var_IN(0_USINT),
    var_OUT(0_BYTE),
    conn_CNF(*this, 0),
    conn_IN(nullptr),
    conn_OUT(*this, 0, var_OUT) {
}

void FORTE_F_USINT_TO_BCD_BYTE::setInitialValues() {
  var_IN = 0_USINT;
  var_OUT = 0_BYTE;
}

void FORTE_F_USINT_TO_BCD_BYTE::executeEvent(const TEventID paEIID, CEventChainExecutionThread *const paECET) {
  switch(paEIID) {
    case scmEventREQID:
      enterStateREQ(paECET);
      break;
    default:
      break;
  }
}

void FORTE_F_USINT_TO_BCD_BYTE::enterStateREQ(CEventChainExecutionThread *const paECET) {
  alg_REQ();
  sendOutputEvent(scmEventCNFID, paECET);
}

void FORTE_F_USINT_TO_BCD_BYTE::readInputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventREQID: {
      readData(0, var_IN, conn_IN);
      break;
    }
    default:
      break;
  }
}

void FORTE_F_USINT_TO_BCD_BYTE::writeOutputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventCNFID: {
      writeData(scmFBInterfaceSpec.mNumDIs + 0, var_OUT, conn_OUT);
      break;
    }
    default:
      break;
  }
}

CIEC_ANY *FORTE_F_USINT_TO_BCD_BYTE::getDI(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_IN;
  }
  return nullptr;
}

CIEC_ANY *FORTE_F_USINT_TO_BCD_BYTE::getDO(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_OUT;
  }
  return nullptr;
}

CEventConnection *FORTE_F_USINT_TO_BCD_BYTE::getEOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_F_USINT_TO_BCD_BYTE::getDIConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_IN;
  }
  return nullptr;
}

CDataConnection *FORTE_F_USINT_TO_BCD_BYTE::getDOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_OUT;
  }
  return nullptr;
}

CIEC_ANY *FORTE_F_USINT_TO_BCD_BYTE::getVarInternal(size_t) {
  return nullptr;
}

void FORTE_F_USINT_TO_BCD_BYTE::alg_REQ(void) {

  #line 2 "F_USINT_TO_BCD_BYTE.fbt"
  var_OUT = func_USINT_TO_BCD_BYTE(var_IN);
}
