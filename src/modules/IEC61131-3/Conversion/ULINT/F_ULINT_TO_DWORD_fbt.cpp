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

#include "F_ULINT_TO_DWORD_fbt.h"

#include "core/datatypes/forte_dword.h"
#include "core/datatypes/forte_ulint.h"
#include "core/iec61131_functions.h"
#include "core/datatypes/forte_array_common.h"
#include "core/datatypes/forte_array.h"
#include "core/datatypes/forte_array_fixed.h"
#include "core/datatypes/forte_array_variable.h"

USE_STRING_ID(CNF);
USE_STRING_ID(DWORD);
USE_STRING_ID(Event);
USE_STRING_ID(F_ULINT_TO_DWORD);
USE_STRING_ID(IN);
USE_STRING_ID(OUT);
USE_STRING_ID(REQ);
USE_STRING_ID(ULINT);

DEFINE_FIRMWARE_FB(FORTE_F_ULINT_TO_DWORD, STRID(F_ULINT_TO_DWORD))

const CStringDictionary::TStringId FORTE_F_ULINT_TO_DWORD::scmDataInputNames[] = {STRID(IN)};
const CStringDictionary::TStringId FORTE_F_ULINT_TO_DWORD::scmDataInputTypeIds[] = {STRID(ULINT)};
const CStringDictionary::TStringId FORTE_F_ULINT_TO_DWORD::scmDataOutputNames[] = {STRID(OUT)};
const CStringDictionary::TStringId FORTE_F_ULINT_TO_DWORD::scmDataOutputTypeIds[] = {STRID(DWORD)};
const TDataIOID FORTE_F_ULINT_TO_DWORD::scmEIWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_F_ULINT_TO_DWORD::scmEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_F_ULINT_TO_DWORD::scmEventInputNames[] = {STRID(REQ)};
const TDataIOID FORTE_F_ULINT_TO_DWORD::scmEOWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_F_ULINT_TO_DWORD::scmEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_F_ULINT_TO_DWORD::scmEventOutputNames[] = {STRID(CNF)};
const SFBInterfaceSpec FORTE_F_ULINT_TO_DWORD::scmFBInterfaceSpec = {
  1, scmEventInputNames, nullptr, scmEIWith, scmEIWithIndexes,
  1, scmEventOutputNames, nullptr, scmEOWith, scmEOWithIndexes,
  1, scmDataInputNames, scmDataInputTypeIds,
  1, scmDataOutputNames, scmDataOutputTypeIds,
  0, nullptr,
  0, nullptr
};

FORTE_F_ULINT_TO_DWORD::FORTE_F_ULINT_TO_DWORD(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    CSimpleFB(paContainer, scmFBInterfaceSpec, paInstanceNameId, nullptr),
    var_IN(0_ULINT),
    var_OUT(0_DWORD),
    conn_CNF(*this, 0),
    conn_IN(nullptr),
    conn_OUT(*this, 0, var_OUT) {
}

void FORTE_F_ULINT_TO_DWORD::setInitialValues() {
  var_IN = 0_ULINT;
  var_OUT = 0_DWORD;
}

void FORTE_F_ULINT_TO_DWORD::executeEvent(const TEventID paEIID, CEventChainExecutionThread *const paECET) {
  switch(paEIID) {
    case scmEventREQID:
      enterStateREQ(paECET);
      break;
    default:
      break;
  }
}

void FORTE_F_ULINT_TO_DWORD::enterStateREQ(CEventChainExecutionThread *const paECET) {
  alg_REQ();
  sendOutputEvent(scmEventCNFID, paECET);
}

void FORTE_F_ULINT_TO_DWORD::readInputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventREQID: {
      readData(0, var_IN, conn_IN);
      break;
    }
    default:
      break;
  }
}

void FORTE_F_ULINT_TO_DWORD::writeOutputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventCNFID: {
      writeData(scmFBInterfaceSpec.mNumDIs + 0, var_OUT, conn_OUT);
      break;
    }
    default:
      break;
  }
}

CIEC_ANY *FORTE_F_ULINT_TO_DWORD::getDI(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_IN;
  }
  return nullptr;
}

CIEC_ANY *FORTE_F_ULINT_TO_DWORD::getDO(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_OUT;
  }
  return nullptr;
}

CEventConnection *FORTE_F_ULINT_TO_DWORD::getEOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_F_ULINT_TO_DWORD::getDIConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_IN;
  }
  return nullptr;
}

CDataConnection *FORTE_F_ULINT_TO_DWORD::getDOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_OUT;
  }
  return nullptr;
}

CIEC_ANY *FORTE_F_ULINT_TO_DWORD::getVarInternal(size_t) {
  return nullptr;
}

void FORTE_F_ULINT_TO_DWORD::alg_REQ(void) {

  #line 2 "F_ULINT_TO_DWORD.fbt"
  var_OUT = func_ULINT_TO_DWORD(var_IN);
}
