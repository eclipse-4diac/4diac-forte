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

#include "UINT2UINT_fbt.h"

#include "core/datatypes/forte_uint.h"
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
USE_STRING_ID(UINT);
USE_STRING_ID(UINT2UINT);

DEFINE_FIRMWARE_FB(FORTE_UINT2UINT, STRID(UINT2UINT))

const CStringDictionary::TStringId FORTE_UINT2UINT::scmDataInputNames[] = {STRID(IN)};
const CStringDictionary::TStringId FORTE_UINT2UINT::scmDataInputTypeIds[] = {STRID(UINT)};
const CStringDictionary::TStringId FORTE_UINT2UINT::scmDataOutputNames[] = {STRID(OUT)};
const CStringDictionary::TStringId FORTE_UINT2UINT::scmDataOutputTypeIds[] = {STRID(UINT)};
const TDataIOID FORTE_UINT2UINT::scmEIWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_UINT2UINT::scmEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_UINT2UINT::scmEventInputNames[] = {STRID(REQ)};
const TDataIOID FORTE_UINT2UINT::scmEOWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_UINT2UINT::scmEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_UINT2UINT::scmEventOutputNames[] = {STRID(CNF)};
const SFBInterfaceSpec FORTE_UINT2UINT::scmFBInterfaceSpec = {
  1, scmEventInputNames, nullptr, scmEIWith, scmEIWithIndexes,
  1, scmEventOutputNames, nullptr, scmEOWith, scmEOWithIndexes,
  1, scmDataInputNames, scmDataInputTypeIds,
  1, scmDataOutputNames, scmDataOutputTypeIds,
  0, nullptr,
  0, nullptr
};

FORTE_UINT2UINT::FORTE_UINT2UINT(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    CSimpleFB(paContainer, scmFBInterfaceSpec, paInstanceNameId, nullptr),
    var_IN(0_UINT),
    var_OUT(0_UINT),
    conn_CNF(*this, 0),
    conn_IN(nullptr),
    conn_OUT(*this, 0, var_OUT) {
}

void FORTE_UINT2UINT::setInitialValues() {
  var_IN = 0_UINT;
  var_OUT = 0_UINT;
}

void FORTE_UINT2UINT::executeEvent(const TEventID paEIID, CEventChainExecutionThread *const paECET) {
  switch(paEIID) {
    case scmEventREQID:
      enterStateREQ(paECET);
      break;
    default:
      break;
  }
}

void FORTE_UINT2UINT::enterStateREQ(CEventChainExecutionThread *const paECET) {
  alg_REQ();
  sendOutputEvent(scmEventCNFID, paECET);
}

void FORTE_UINT2UINT::readInputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventREQID: {
      readData(0, var_IN, conn_IN);
      break;
    }
    default:
      break;
  }
}

void FORTE_UINT2UINT::writeOutputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventCNFID: {
      writeData(scmFBInterfaceSpec.mNumDIs + 0, var_OUT, conn_OUT);
      break;
    }
    default:
      break;
  }
}

CIEC_ANY *FORTE_UINT2UINT::getDI(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_IN;
  }
  return nullptr;
}

CIEC_ANY *FORTE_UINT2UINT::getDO(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_OUT;
  }
  return nullptr;
}

CEventConnection *FORTE_UINT2UINT::getEOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_UINT2UINT::getDIConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_IN;
  }
  return nullptr;
}

CDataConnection *FORTE_UINT2UINT::getDOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_OUT;
  }
  return nullptr;
}

CIEC_ANY *FORTE_UINT2UINT::getVarInternal(size_t) {
  return nullptr;
}

void FORTE_UINT2UINT::alg_REQ(void) {

  #line 2 "UINT2UINT.fbt"
  var_OUT = var_IN;
}
