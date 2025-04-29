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

#include "F_UINT_TO_WORD_fbt.h"

#include "core/datatypes/forte_uint.h"
#include "core/datatypes/forte_word.h"
#include "core/iec61131_functions.h"
#include "core/datatypes/forte_array_common.h"
#include "core/datatypes/forte_array.h"
#include "core/datatypes/forte_array_fixed.h"
#include "core/datatypes/forte_array_variable.h"

USE_STRING_ID(CNF);
USE_STRING_ID(Event);
USE_STRING_ID(F_UINT_TO_WORD);
USE_STRING_ID(IN);
USE_STRING_ID(OUT);
USE_STRING_ID(REQ);
USE_STRING_ID(UINT);
USE_STRING_ID(WORD);

DEFINE_FIRMWARE_FB(FORTE_F_UINT_TO_WORD, STRID(F_UINT_TO_WORD))

const CStringDictionary::TStringId FORTE_F_UINT_TO_WORD::scmDataInputNames[] = {STRID(IN)};
const CStringDictionary::TStringId FORTE_F_UINT_TO_WORD::scmDataInputTypeIds[] = {STRID(UINT)};
const CStringDictionary::TStringId FORTE_F_UINT_TO_WORD::scmDataOutputNames[] = {STRID(OUT)};
const CStringDictionary::TStringId FORTE_F_UINT_TO_WORD::scmDataOutputTypeIds[] = {STRID(WORD)};
const TDataIOID FORTE_F_UINT_TO_WORD::scmEIWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_F_UINT_TO_WORD::scmEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_F_UINT_TO_WORD::scmEventInputNames[] = {STRID(REQ)};
const TDataIOID FORTE_F_UINT_TO_WORD::scmEOWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_F_UINT_TO_WORD::scmEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_F_UINT_TO_WORD::scmEventOutputNames[] = {STRID(CNF)};
const SFBInterfaceSpec FORTE_F_UINT_TO_WORD::scmFBInterfaceSpec = {
  1, scmEventInputNames, nullptr, scmEIWith, scmEIWithIndexes,
  1, scmEventOutputNames, nullptr, scmEOWith, scmEOWithIndexes,
  1, scmDataInputNames, scmDataInputTypeIds,
  1, scmDataOutputNames, scmDataOutputTypeIds,
  0, nullptr,
  0, nullptr
};

FORTE_F_UINT_TO_WORD::FORTE_F_UINT_TO_WORD(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    CSimpleFB(paContainer, scmFBInterfaceSpec, paInstanceNameId, nullptr),
    var_IN(0_UINT),
    var_OUT(0_WORD),
    conn_CNF(*this, 0),
    conn_IN(nullptr),
    conn_OUT(*this, 0, var_OUT) {
}

void FORTE_F_UINT_TO_WORD::setInitialValues() {
  var_IN = 0_UINT;
  var_OUT = 0_WORD;
}

void FORTE_F_UINT_TO_WORD::executeEvent(const TEventID paEIID, CEventChainExecutionThread *const paECET) {
  switch(paEIID) {
    case scmEventREQID:
      enterStateREQ(paECET);
      break;
    default:
      break;
  }
}

void FORTE_F_UINT_TO_WORD::enterStateREQ(CEventChainExecutionThread *const paECET) {
  alg_REQ();
  sendOutputEvent(scmEventCNFID, paECET);
}

void FORTE_F_UINT_TO_WORD::readInputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventREQID: {
      readData(0, var_IN, conn_IN);
      break;
    }
    default:
      break;
  }
}

void FORTE_F_UINT_TO_WORD::writeOutputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventCNFID: {
      writeData(scmFBInterfaceSpec.mNumDIs + 0, var_OUT, conn_OUT);
      break;
    }
    default:
      break;
  }
}

CIEC_ANY *FORTE_F_UINT_TO_WORD::getDI(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_IN;
  }
  return nullptr;
}

CIEC_ANY *FORTE_F_UINT_TO_WORD::getDO(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_OUT;
  }
  return nullptr;
}

CEventConnection *FORTE_F_UINT_TO_WORD::getEOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_F_UINT_TO_WORD::getDIConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_IN;
  }
  return nullptr;
}

CDataConnection *FORTE_F_UINT_TO_WORD::getDOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_OUT;
  }
  return nullptr;
}

CIEC_ANY *FORTE_F_UINT_TO_WORD::getVarInternal(size_t) {
  return nullptr;
}

void FORTE_F_UINT_TO_WORD::alg_REQ(void) {

  #line 2 "F_UINT_TO_WORD.fbt"
  var_OUT = func_UINT_TO_WORD(var_IN);
}
