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

#include "SINT2SINT_fbt.h"

#include "core/datatypes/forte_sint.h"
#include "core/iec61131_functions.h"
#include "core/datatypes/forte_array_common.h"
#include "core/datatypes/forte_array.h"
#include "core/datatypes/forte_array_fixed.h"
#include "core/datatypes/forte_array_variable.h"

USE_STRING_ID(CNF);
USE_STRING_ID(Event);
USE_STRING_ID(IN);
USE_STRING_ID(OUT);
USE_STRING_ID(REQ);
USE_STRING_ID(SINT);
USE_STRING_ID(SINT2SINT);

DEFINE_FIRMWARE_FB(FORTE_SINT2SINT, STRID(SINT2SINT))

const CStringDictionary::TStringId FORTE_SINT2SINT::scmDataInputNames[] = {STRID(IN)};
const CStringDictionary::TStringId FORTE_SINT2SINT::scmDataInputTypeIds[] = {STRID(SINT)};
const CStringDictionary::TStringId FORTE_SINT2SINT::scmDataOutputNames[] = {STRID(OUT)};
const CStringDictionary::TStringId FORTE_SINT2SINT::scmDataOutputTypeIds[] = {STRID(SINT)};
const TDataIOID FORTE_SINT2SINT::scmEIWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_SINT2SINT::scmEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_SINT2SINT::scmEventInputNames[] = {STRID(REQ)};
const TDataIOID FORTE_SINT2SINT::scmEOWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_SINT2SINT::scmEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_SINT2SINT::scmEventOutputNames[] = {STRID(CNF)};
const SFBInterfaceSpec FORTE_SINT2SINT::scmFBInterfaceSpec = {
  1, scmEventInputNames, nullptr, scmEIWith, scmEIWithIndexes,
  1, scmEventOutputNames, nullptr, scmEOWith, scmEOWithIndexes,
  1, scmDataInputNames, scmDataInputTypeIds,
  1, scmDataOutputNames, scmDataOutputTypeIds,
  0, nullptr,
  0, nullptr
};

FORTE_SINT2SINT::FORTE_SINT2SINT(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    CSimpleFB(paContainer, scmFBInterfaceSpec, paInstanceNameId, nullptr),
    var_IN(0_SINT),
    var_OUT(0_SINT),
    conn_CNF(*this, 0),
    conn_IN(nullptr),
    conn_OUT(*this, 0, var_OUT) {
}

void FORTE_SINT2SINT::setInitialValues() {
  var_IN = 0_SINT;
  var_OUT = 0_SINT;
}

void FORTE_SINT2SINT::executeEvent(const TEventID paEIID, CEventChainExecutionThread *const paECET) {
  switch(paEIID) {
    case scmEventREQID:
      enterStateREQ(paECET);
      break;
    default:
      break;
  }
}

void FORTE_SINT2SINT::enterStateREQ(CEventChainExecutionThread *const paECET) {
  alg_REQ();
  sendOutputEvent(scmEventCNFID, paECET);
}

void FORTE_SINT2SINT::readInputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventREQID: {
      readData(0, var_IN, conn_IN);
      break;
    }
    default:
      break;
  }
}

void FORTE_SINT2SINT::writeOutputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventCNFID: {
      writeData(scmFBInterfaceSpec.mNumDIs + 0, var_OUT, conn_OUT);
      break;
    }
    default:
      break;
  }
}

CIEC_ANY *FORTE_SINT2SINT::getDI(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_IN;
  }
  return nullptr;
}

CIEC_ANY *FORTE_SINT2SINT::getDO(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_OUT;
  }
  return nullptr;
}

CEventConnection *FORTE_SINT2SINT::getEOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_SINT2SINT::getDIConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_IN;
  }
  return nullptr;
}

CDataConnection *FORTE_SINT2SINT::getDOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_OUT;
  }
  return nullptr;
}

CIEC_ANY *FORTE_SINT2SINT::getVarInternal(size_t) {
  return nullptr;
}

void FORTE_SINT2SINT::alg_REQ(void) {

  #line 2 "SINT2SINT.fbt"
  var_OUT = var_IN;
}
