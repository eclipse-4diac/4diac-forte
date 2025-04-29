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

#include "USINT2USINT_fbt.h"

USE_STRING_ID(CNF);
USE_STRING_ID(Event);
USE_STRING_ID(IN);
USE_STRING_ID(OUT);
USE_STRING_ID(REQ);
USE_STRING_ID(USINT);
USE_STRING_ID(USINT2USINT);

#include "forte_usint.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"

DEFINE_FIRMWARE_FB(FORTE_USINT2USINT, STRID(USINT2USINT))

const CStringDictionary::TStringId FORTE_USINT2USINT::scmDataInputNames[] = {STRID(IN)};

const CStringDictionary::TStringId FORTE_USINT2USINT::scmDataInputTypeIds[] = {STRID(USINT)};

const CStringDictionary::TStringId FORTE_USINT2USINT::scmDataOutputNames[] = {STRID(OUT)};

const CStringDictionary::TStringId FORTE_USINT2USINT::scmDataOutputTypeIds[] = {STRID(USINT)};

const TDataIOID FORTE_USINT2USINT::scmEIWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_USINT2USINT::scmEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_USINT2USINT::scmEventInputNames[] = {STRID(REQ)};
const CStringDictionary::TStringId FORTE_USINT2USINT::scmEventInputTypeIds[] = {STRID(Event)};

const TDataIOID FORTE_USINT2USINT::scmEOWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_USINT2USINT::scmEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_USINT2USINT::scmEventOutputNames[] = {STRID(CNF)};
const CStringDictionary::TStringId FORTE_USINT2USINT::scmEventOutputTypeIds[] = {STRID(Event)};

const SFBInterfaceSpec FORTE_USINT2USINT::scmFBInterfaceSpec = {1,
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

FORTE_USINT2USINT::FORTE_USINT2USINT(CStringDictionary::TStringId paInstanceNameId,
                                     forte::core::CFBContainer &paContainer) :
    CSimpleFB(paContainer, scmFBInterfaceSpec, paInstanceNameId, nullptr),
    var_IN(CIEC_USINT(0)),
    var_OUT(CIEC_USINT(0)),
    conn_CNF(*this, 0),
    conn_IN(nullptr),
    conn_OUT(*this, 0, var_OUT) {
}

void FORTE_USINT2USINT::alg_REQ(void) {

  var_OUT = var_IN;
}

void FORTE_USINT2USINT::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
  switch (paEIID) {
    case scmEventREQID: alg_REQ(); break;
    default: break;
  }
  sendOutputEvent(scmEventCNFID, paECET);
}

void FORTE_USINT2USINT::readInputData(TEventID paEIID) {
  switch (paEIID) {
    case scmEventREQID: {
      readData(0, var_IN, conn_IN);
      break;
    }
    default: break;
  }
}

void FORTE_USINT2USINT::writeOutputData(TEventID paEIID) {
  switch (paEIID) {
    case scmEventCNFID: {
      writeData(0, var_OUT, conn_OUT);
      break;
    }
    default: break;
  }
}

CIEC_ANY *FORTE_USINT2USINT::getDI(size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_IN;
  }
  return nullptr;
}

CIEC_ANY *FORTE_USINT2USINT::getDO(size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_OUT;
  }
  return nullptr;
}

CEventConnection *FORTE_USINT2USINT::getEOConUnchecked(TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_USINT2USINT::getDIConUnchecked(TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_IN;
  }
  return nullptr;
}

CDataConnection *FORTE_USINT2USINT::getDOConUnchecked(TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_OUT;
  }
  return nullptr;
}

CIEC_ANY *FORTE_USINT2USINT::getVarInternal(size_t) {
  return nullptr;
}
