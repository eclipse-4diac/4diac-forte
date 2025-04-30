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

#include "F_ULINT_TO_BCD_LWORD_fbt.h"

USE_STRING_ID(CNF);
USE_STRING_ID(Event);
USE_STRING_ID(F_ULINT_TO_BCD_LWORD);
USE_STRING_ID(IN);
USE_STRING_ID(LWORD);
USE_STRING_ID(OUT);
USE_STRING_ID(REQ);
USE_STRING_ID(ULINT);

#include "forte_lword.h"
#include "forte_ulint.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"

DEFINE_FIRMWARE_FB(FORTE_F_ULINT_TO_BCD_LWORD, STRID(F_ULINT_TO_BCD_LWORD))

const CStringDictionary::TStringId FORTE_F_ULINT_TO_BCD_LWORD::scmDataInputNames[] = {STRID(IN)};

const CStringDictionary::TStringId FORTE_F_ULINT_TO_BCD_LWORD::scmDataInputTypeIds[] = {STRID(ULINT)};

const CStringDictionary::TStringId FORTE_F_ULINT_TO_BCD_LWORD::scmDataOutputNames[] = {STRID(OUT)};

const CStringDictionary::TStringId FORTE_F_ULINT_TO_BCD_LWORD::scmDataOutputTypeIds[] = {STRID(LWORD)};

const TDataIOID FORTE_F_ULINT_TO_BCD_LWORD::scmEIWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_F_ULINT_TO_BCD_LWORD::scmEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_F_ULINT_TO_BCD_LWORD::scmEventInputNames[] = {STRID(REQ)};
const CStringDictionary::TStringId FORTE_F_ULINT_TO_BCD_LWORD::scmEventInputTypeIds[] = {STRID(Event)};

const TDataIOID FORTE_F_ULINT_TO_BCD_LWORD::scmEOWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_F_ULINT_TO_BCD_LWORD::scmEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_F_ULINT_TO_BCD_LWORD::scmEventOutputNames[] = {STRID(CNF)};
const CStringDictionary::TStringId FORTE_F_ULINT_TO_BCD_LWORD::scmEventOutputTypeIds[] = {STRID(Event)};

const SFBInterfaceSpec FORTE_F_ULINT_TO_BCD_LWORD::scmFBInterfaceSpec = {1,
                                                                         scmEventInputNames,
                                                                         scmEventInputTypeIds,
                                                                         scmEIWith,
                                                                         scmEIWithIndexes,
                                                                         1,
                                                                         scmEventOutputNames,
                                                                         scmEventOutputTypeIds,
                                                                         scmEOWith,
                                                                         scmEOWithIndexes,
                                                                         1,
                                                                         scmDataInputNames,
                                                                         scmDataInputTypeIds,
                                                                         1,
                                                                         scmDataOutputNames,
                                                                         scmDataOutputTypeIds,
                                                                         0,
                                                                         nullptr,
                                                                         0,
                                                                         nullptr};

FORTE_F_ULINT_TO_BCD_LWORD::FORTE_F_ULINT_TO_BCD_LWORD(CStringDictionary::TStringId paInstanceNameId,
                                                       forte::core::CFBContainer &paContainer) :
    CSimpleFB(paContainer, scmFBInterfaceSpec, paInstanceNameId, nullptr),
    var_IN(CIEC_ULINT(0)),
    var_OUT(CIEC_LWORD(0)),
    conn_CNF(*this, 0),
    conn_IN(nullptr),
    conn_OUT(*this, 0, var_OUT) {
}

void FORTE_F_ULINT_TO_BCD_LWORD::alg_REQ(void) {

  var_OUT = func_ULINT_TO_BCD_LWORD(var_IN);
}

void FORTE_F_ULINT_TO_BCD_LWORD::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
  switch (paEIID) {
    case scmEventREQID: alg_REQ(); break;
    default: break;
  }
  sendOutputEvent(scmEventCNFID, paECET);
}

void FORTE_F_ULINT_TO_BCD_LWORD::readInputData(TEventID paEIID) {
  switch (paEIID) {
    case scmEventREQID: {
      readData(0, var_IN, conn_IN);
      break;
    }
    default: break;
  }
}

void FORTE_F_ULINT_TO_BCD_LWORD::writeOutputData(TEventID paEIID) {
  switch (paEIID) {
    case scmEventCNFID: {
      writeData(scmFBInterfaceSpec.mNumDIs + 0, var_OUT, conn_OUT);
      break;
    }
    default: break;
  }
}

CIEC_ANY *FORTE_F_ULINT_TO_BCD_LWORD::getDI(size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_IN;
  }
  return nullptr;
}

CIEC_ANY *FORTE_F_ULINT_TO_BCD_LWORD::getDO(size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_OUT;
  }
  return nullptr;
}

CEventConnection *FORTE_F_ULINT_TO_BCD_LWORD::getEOConUnchecked(TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_F_ULINT_TO_BCD_LWORD::getDIConUnchecked(TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_IN;
  }
  return nullptr;
}

CDataConnection *FORTE_F_ULINT_TO_BCD_LWORD::getDOConUnchecked(TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_OUT;
  }
  return nullptr;
}

CIEC_ANY *FORTE_F_ULINT_TO_BCD_LWORD::getVarInternal(size_t) {
  return nullptr;
}
