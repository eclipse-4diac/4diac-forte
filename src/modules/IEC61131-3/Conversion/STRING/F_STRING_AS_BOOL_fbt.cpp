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

#include "F_STRING_AS_BOOL_fbt.h"

USE_STRING_ID(BOOL);
USE_STRING_ID(CNF);
USE_STRING_ID(Event);
USE_STRING_ID(F_STRING_AS_BOOL);
USE_STRING_ID(IN);
USE_STRING_ID(OUT);
USE_STRING_ID(REQ);
USE_STRING_ID(STRING);

#include "forte_bool.h"
#include "forte_string.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"

DEFINE_FIRMWARE_FB(FORTE_F_STRING_AS_BOOL, STRID(F_STRING_AS_BOOL))

const CStringDictionary::TStringId FORTE_F_STRING_AS_BOOL::scmDataInputNames[] = {STRID(IN)};

const CStringDictionary::TStringId FORTE_F_STRING_AS_BOOL::scmDataInputTypeIds[] = {STRID(STRING)};

const CStringDictionary::TStringId FORTE_F_STRING_AS_BOOL::scmDataOutputNames[] = {STRID(OUT)};

const CStringDictionary::TStringId FORTE_F_STRING_AS_BOOL::scmDataOutputTypeIds[] = {STRID(BOOL)};

const TDataIOID FORTE_F_STRING_AS_BOOL::scmEIWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_F_STRING_AS_BOOL::scmEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_F_STRING_AS_BOOL::scmEventInputNames[] = {STRID(REQ)};
const CStringDictionary::TStringId FORTE_F_STRING_AS_BOOL::scmEventInputTypeIds[] = {STRID(Event)};

const TDataIOID FORTE_F_STRING_AS_BOOL::scmEOWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_F_STRING_AS_BOOL::scmEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_F_STRING_AS_BOOL::scmEventOutputNames[] = {STRID(CNF)};
const CStringDictionary::TStringId FORTE_F_STRING_AS_BOOL::scmEventOutputTypeIds[] = {STRID(Event)};

const SFBInterfaceSpec FORTE_F_STRING_AS_BOOL::scmFBInterfaceSpec = {1,
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

FORTE_F_STRING_AS_BOOL::FORTE_F_STRING_AS_BOOL(CStringDictionary::TStringId paInstanceNameId,
                                               forte::core::CFBContainer &paContainer) :
    CSimpleFB(paContainer, scmFBInterfaceSpec, paInstanceNameId, nullptr),
    var_IN(CIEC_STRING("", 0)),
    var_OUT(false_BOOL),
    conn_CNF(*this, 0),
    conn_IN(nullptr),
    conn_OUT(*this, 0, var_OUT) {
}

void FORTE_F_STRING_AS_BOOL::alg_REQ(void) {

  var_OUT = func_STRING_AS_BOOL(var_IN);
}

void FORTE_F_STRING_AS_BOOL::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
  switch (paEIID) {
    case scmEventREQID: alg_REQ(); break;
    default: break;
  }
  sendOutputEvent(scmEventCNFID, paECET);
}

void FORTE_F_STRING_AS_BOOL::readInputData(TEventID paEIID) {
  switch (paEIID) {
    case scmEventREQID: {
      readData(0, var_IN, conn_IN);
      break;
    }
    default: break;
  }
}

void FORTE_F_STRING_AS_BOOL::writeOutputData(TEventID paEIID) {
  switch (paEIID) {
    case scmEventCNFID: {
      writeData(0, var_OUT, conn_OUT);
      break;
    }
    default: break;
  }
}

CIEC_ANY *FORTE_F_STRING_AS_BOOL::getDI(size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_IN;
  }
  return nullptr;
}

CIEC_ANY *FORTE_F_STRING_AS_BOOL::getDO(size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_OUT;
  }
  return nullptr;
}

CEventConnection *FORTE_F_STRING_AS_BOOL::getEOConUnchecked(TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_F_STRING_AS_BOOL::getDIConUnchecked(TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_IN;
  }
  return nullptr;
}

CDataConnection *FORTE_F_STRING_AS_BOOL::getDOConUnchecked(TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_OUT;
  }
  return nullptr;
}

CIEC_ANY *FORTE_F_STRING_AS_BOOL::getVarInternal(size_t) {
  return nullptr;
}
