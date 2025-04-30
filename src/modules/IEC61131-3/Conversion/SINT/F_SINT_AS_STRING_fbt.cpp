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

#include "F_SINT_AS_STRING_fbt.h"

USE_STRING_ID(CNF);
USE_STRING_ID(Event);
USE_STRING_ID(F_SINT_AS_STRING);
USE_STRING_ID(IN);
USE_STRING_ID(OUT);
USE_STRING_ID(REQ);
USE_STRING_ID(SINT);
USE_STRING_ID(STRING);

#include "forte_string.h"
#include "forte_sint.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"

DEFINE_FIRMWARE_FB(FORTE_F_SINT_AS_STRING, STRID(F_SINT_AS_STRING))

const CStringDictionary::TStringId FORTE_F_SINT_AS_STRING::scmDataInputNames[] = {STRID(IN)};

const CStringDictionary::TStringId FORTE_F_SINT_AS_STRING::scmDataInputTypeIds[] = {STRID(SINT)};

const CStringDictionary::TStringId FORTE_F_SINT_AS_STRING::scmDataOutputNames[] = {STRID(OUT)};

const CStringDictionary::TStringId FORTE_F_SINT_AS_STRING::scmDataOutputTypeIds[] = {STRID(STRING)};

const TDataIOID FORTE_F_SINT_AS_STRING::scmEIWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_F_SINT_AS_STRING::scmEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_F_SINT_AS_STRING::scmEventInputNames[] = {STRID(REQ)};
const CStringDictionary::TStringId FORTE_F_SINT_AS_STRING::scmEventInputTypeIds[] = {STRID(Event)};

const TDataIOID FORTE_F_SINT_AS_STRING::scmEOWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_F_SINT_AS_STRING::scmEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_F_SINT_AS_STRING::scmEventOutputNames[] = {STRID(CNF)};
const CStringDictionary::TStringId FORTE_F_SINT_AS_STRING::scmEventOutputTypeIds[] = {STRID(Event)};

const SFBInterfaceSpec FORTE_F_SINT_AS_STRING::scmFBInterfaceSpec = {1,
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

FORTE_F_SINT_AS_STRING::FORTE_F_SINT_AS_STRING(CStringDictionary::TStringId paInstanceNameId,
                                               forte::core::CFBContainer &paContainer) :
    CSimpleFB(paContainer, scmFBInterfaceSpec, paInstanceNameId, nullptr),
    var_IN(CIEC_SINT(0)),
    var_OUT(CIEC_STRING("", 0)),
    conn_CNF(*this, 0),
    conn_IN(nullptr),
    conn_OUT(*this, 0, var_OUT) {
}

void FORTE_F_SINT_AS_STRING::alg_REQ(void) {

  var_OUT = func_SINT_AS_STRING(var_IN);
}

void FORTE_F_SINT_AS_STRING::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
  switch (paEIID) {
    case scmEventREQID: alg_REQ(); break;
    default: break;
  }
  sendOutputEvent(scmEventCNFID, paECET);
}

void FORTE_F_SINT_AS_STRING::readInputData(TEventID paEIID) {
  switch (paEIID) {
    case scmEventREQID: {
      readData(0, var_IN, conn_IN);
      break;
    }
    default: break;
  }
}

void FORTE_F_SINT_AS_STRING::writeOutputData(TEventID paEIID) {
  switch (paEIID) {
    case scmEventCNFID: {
      writeData(scmFBInterfaceSpec.mNumDIs + 0, var_OUT, conn_OUT);
      break;
    }
    default: break;
  }
}

CIEC_ANY *FORTE_F_SINT_AS_STRING::getDI(size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_IN;
  }
  return nullptr;
}

CIEC_ANY *FORTE_F_SINT_AS_STRING::getDO(size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_OUT;
  }
  return nullptr;
}

CEventConnection *FORTE_F_SINT_AS_STRING::getEOConUnchecked(TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_F_SINT_AS_STRING::getDIConUnchecked(TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_IN;
  }
  return nullptr;
}

CDataConnection *FORTE_F_SINT_AS_STRING::getDOConUnchecked(TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_OUT;
  }
  return nullptr;
}

CIEC_ANY *FORTE_F_SINT_AS_STRING::getVarInternal(size_t) {
  return nullptr;
}
