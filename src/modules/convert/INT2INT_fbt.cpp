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

#include "INT2INT_fbt.h"

USE_STRING_ID(CNF);
USE_STRING_ID(Event);
USE_STRING_ID(IN);
USE_STRING_ID(INT);
USE_STRING_ID(INT2INT);
USE_STRING_ID(OUT);
USE_STRING_ID(REQ);

#include "forte_int.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"

DEFINE_FIRMWARE_FB(FORTE_INT2INT, STRID(INT2INT))

const CStringDictionary::TStringId FORTE_INT2INT::scmDataInputNames[] = {STRID(IN)};

const CStringDictionary::TStringId FORTE_INT2INT::scmDataInputTypeIds[] = {STRID(INT)};

const CStringDictionary::TStringId FORTE_INT2INT::scmDataOutputNames[] = {STRID(OUT)};

const CStringDictionary::TStringId FORTE_INT2INT::scmDataOutputTypeIds[] = {STRID(INT)};

const TDataIOID FORTE_INT2INT::scmEIWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_INT2INT::scmEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_INT2INT::scmEventInputNames[] = {STRID(REQ)};
const CStringDictionary::TStringId FORTE_INT2INT::scmEventInputTypeIds[] = {STRID(Event)};

const TDataIOID FORTE_INT2INT::scmEOWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_INT2INT::scmEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_INT2INT::scmEventOutputNames[] = {STRID(CNF)};
const CStringDictionary::TStringId FORTE_INT2INT::scmEventOutputTypeIds[] = {STRID(Event)};

const SFBInterfaceSpec FORTE_INT2INT::scmFBInterfaceSpec = {1,
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

FORTE_INT2INT::FORTE_INT2INT(CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    CSimpleFB(paContainer, scmFBInterfaceSpec, paInstanceNameId, nullptr),
    var_IN(CIEC_INT(0)),
    var_OUT(CIEC_INT(0)),
    conn_CNF(*this, 0),
    conn_IN(nullptr),
    conn_OUT(*this, 0, var_OUT) {
}

void FORTE_INT2INT::alg_REQ(void) {

  var_OUT = var_IN;
}

void FORTE_INT2INT::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
  switch (paEIID) {
    case scmEventREQID: alg_REQ(); break;
    default: break;
  }
  sendOutputEvent(scmEventCNFID, paECET);
}

void FORTE_INT2INT::readInputData(TEventID paEIID) {
  switch (paEIID) {
    case scmEventREQID: {
      readData(0, var_IN, conn_IN);
      break;
    }
    default: break;
  }
}

void FORTE_INT2INT::writeOutputData(TEventID paEIID) {
  switch (paEIID) {
    case scmEventCNFID: {
      writeData(0, var_OUT, conn_OUT);
      break;
    }
    default: break;
  }
}

CIEC_ANY *FORTE_INT2INT::getDI(size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_IN;
  }
  return nullptr;
}

CIEC_ANY *FORTE_INT2INT::getDO(size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_OUT;
  }
  return nullptr;
}

CEventConnection *FORTE_INT2INT::getEOConUnchecked(TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_INT2INT::getDIConUnchecked(TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_IN;
  }
  return nullptr;
}

CDataConnection *FORTE_INT2INT::getDOConUnchecked(TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_OUT;
  }
  return nullptr;
}

CIEC_ANY *FORTE_INT2INT::getVarInternal(size_t) {
  return nullptr;
}
