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

#include "LREAL2LREAL_fbt.h"

#include "core/datatypes/forte_lreal.h"
#include "core/iec61131_functions.h"
#include "core/datatypes/forte_array_common.h"
#include "core/datatypes/forte_array.h"
#include "core/datatypes/forte_array_fixed.h"
#include "core/datatypes/forte_array_variable.h"

USE_STRING_ID(CNF);
USE_STRING_ID(Event);
USE_STRING_ID(IN);
USE_STRING_ID(LREAL);
USE_STRING_ID(LREAL2LREAL);
USE_STRING_ID(OUT);
USE_STRING_ID(REQ);

DEFINE_FIRMWARE_FB(FORTE_LREAL2LREAL, STRID(LREAL2LREAL))

const CStringDictionary::TStringId FORTE_LREAL2LREAL::scmDataInputNames[] = {STRID(IN)};
const CStringDictionary::TStringId FORTE_LREAL2LREAL::scmDataInputTypeIds[] = {STRID(LREAL)};
const CStringDictionary::TStringId FORTE_LREAL2LREAL::scmDataOutputNames[] = {STRID(OUT)};
const CStringDictionary::TStringId FORTE_LREAL2LREAL::scmDataOutputTypeIds[] = {STRID(LREAL)};
const TDataIOID FORTE_LREAL2LREAL::scmEIWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_LREAL2LREAL::scmEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_LREAL2LREAL::scmEventInputNames[] = {STRID(REQ)};
const TDataIOID FORTE_LREAL2LREAL::scmEOWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_LREAL2LREAL::scmEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_LREAL2LREAL::scmEventOutputNames[] = {STRID(CNF)};
const SFBInterfaceSpec FORTE_LREAL2LREAL::scmFBInterfaceSpec = {
  1, scmEventInputNames, nullptr, scmEIWith, scmEIWithIndexes,
  1, scmEventOutputNames, nullptr, scmEOWith, scmEOWithIndexes,
  1, scmDataInputNames, scmDataInputTypeIds,
  1, scmDataOutputNames, scmDataOutputTypeIds,
  0, nullptr,
  0, nullptr
};

FORTE_LREAL2LREAL::FORTE_LREAL2LREAL(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    CSimpleFB(paContainer, scmFBInterfaceSpec, paInstanceNameId, nullptr),
    var_IN(0_LREAL),
    var_OUT(0_LREAL),
    conn_CNF(*this, 0),
    conn_IN(nullptr),
    conn_OUT(*this, 0, var_OUT) {
}

void FORTE_LREAL2LREAL::setInitialValues() {
  var_IN = 0_LREAL;
  var_OUT = 0_LREAL;
}

void FORTE_LREAL2LREAL::executeEvent(const TEventID paEIID, CEventChainExecutionThread *const paECET) {
  switch(paEIID) {
    case scmEventREQID:
      enterStateREQ(paECET);
      break;
    default:
      break;
  }
}

void FORTE_LREAL2LREAL::enterStateREQ(CEventChainExecutionThread *const paECET) {
  alg_REQ();
  sendOutputEvent(scmEventCNFID, paECET);
}

void FORTE_LREAL2LREAL::readInputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventREQID: {
      readData(0, var_IN, conn_IN);
      break;
    }
    default:
      break;
  }
}

void FORTE_LREAL2LREAL::writeOutputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventCNFID: {
      writeData(scmFBInterfaceSpec.mNumDIs + 0, var_OUT, conn_OUT);
      break;
    }
    default:
      break;
  }
}

CIEC_ANY *FORTE_LREAL2LREAL::getDI(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_IN;
  }
  return nullptr;
}

CIEC_ANY *FORTE_LREAL2LREAL::getDO(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_OUT;
  }
  return nullptr;
}

CEventConnection *FORTE_LREAL2LREAL::getEOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_LREAL2LREAL::getDIConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_IN;
  }
  return nullptr;
}

CDataConnection *FORTE_LREAL2LREAL::getDOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_OUT;
  }
  return nullptr;
}

CIEC_ANY *FORTE_LREAL2LREAL::getVarInternal(size_t) {
  return nullptr;
}

void FORTE_LREAL2LREAL::alg_REQ(void) {

  #line 2 "LREAL2LREAL.fbt"
  var_OUT = var_IN;
}
