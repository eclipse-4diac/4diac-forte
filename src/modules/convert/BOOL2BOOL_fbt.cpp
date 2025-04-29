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

#include "BOOL2BOOL_fbt.h"

#include "core/datatypes/forte_bool.h"
#include "core/iec61131_functions.h"
#include "core/datatypes/forte_array_common.h"
#include "core/datatypes/forte_array.h"
#include "core/datatypes/forte_array_fixed.h"
#include "core/datatypes/forte_array_variable.h"

USE_STRING_ID(BOOL);
USE_STRING_ID(BOOL2BOOL);
USE_STRING_ID(CNF);
USE_STRING_ID(Event);
USE_STRING_ID(IN);
USE_STRING_ID(OUT);
USE_STRING_ID(REQ);

DEFINE_FIRMWARE_FB(FORTE_BOOL2BOOL, STRID(BOOL2BOOL))

const CStringDictionary::TStringId FORTE_BOOL2BOOL::scmDataInputNames[] = {STRID(IN)};
const CStringDictionary::TStringId FORTE_BOOL2BOOL::scmDataInputTypeIds[] = {STRID(BOOL)};
const CStringDictionary::TStringId FORTE_BOOL2BOOL::scmDataOutputNames[] = {STRID(OUT)};
const CStringDictionary::TStringId FORTE_BOOL2BOOL::scmDataOutputTypeIds[] = {STRID(BOOL)};
const TDataIOID FORTE_BOOL2BOOL::scmEIWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_BOOL2BOOL::scmEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_BOOL2BOOL::scmEventInputNames[] = {STRID(REQ)};
const TDataIOID FORTE_BOOL2BOOL::scmEOWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_BOOL2BOOL::scmEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_BOOL2BOOL::scmEventOutputNames[] = {STRID(CNF)};
const SFBInterfaceSpec FORTE_BOOL2BOOL::scmFBInterfaceSpec = {1,
                                                              scmEventInputNames,
                                                              nullptr,
                                                              scmEIWith,
                                                              scmEIWithIndexes,
                                                              1,
                                                              scmEventOutputNames,
                                                              nullptr,
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

FORTE_BOOL2BOOL::FORTE_BOOL2BOOL(const CStringDictionary::TStringId paInstanceNameId,
                                 forte::core::CFBContainer &paContainer) :
    CSimpleFB(paContainer, scmFBInterfaceSpec, paInstanceNameId, nullptr),
    var_IN(false_BOOL),
    var_OUT(false_BOOL),
    conn_CNF(*this, 0),
    conn_IN(nullptr),
    conn_OUT(*this, 0, var_OUT) {
}

void FORTE_BOOL2BOOL::setInitialValues() {
  var_IN = 0_BOOL;
  var_OUT = 0_BOOL;
}

void FORTE_BOOL2BOOL::executeEvent(const TEventID paEIID, CEventChainExecutionThread *const paECET) {
  switch (paEIID) {
    case scmEventREQID: enterStateREQ(paECET); break;
    default: break;
  }
}

void FORTE_BOOL2BOOL::enterStateREQ(CEventChainExecutionThread *const paECET) {
  alg_REQ();
  sendOutputEvent(scmEventCNFID, paECET);
}

void FORTE_BOOL2BOOL::readInputData(const TEventID paEIID) {
  switch (paEIID) {
    case scmEventREQID: {
      readData(0, var_IN, conn_IN);
      break;
    }
    default: break;
  }
}

void FORTE_BOOL2BOOL::writeOutputData(const TEventID paEIID) {
  switch (paEIID) {
    case scmEventCNFID: {
      writeData(scmFBInterfaceSpec.mNumDIs + 0, var_OUT, conn_OUT);
      break;
    }
    default: break;
  }
}

CIEC_ANY *FORTE_BOOL2BOOL::getDI(const size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_IN;
  }
  return nullptr;
}

CIEC_ANY *FORTE_BOOL2BOOL::getDO(const size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_OUT;
  }
  return nullptr;
}

CEventConnection *FORTE_BOOL2BOOL::getEOConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_BOOL2BOOL::getDIConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_IN;
  }
  return nullptr;
}

CDataConnection *FORTE_BOOL2BOOL::getDOConUnchecked(const TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_OUT;
  }
  return nullptr;
}

CIEC_ANY *FORTE_BOOL2BOOL::getVarInternal(size_t) {
  return nullptr;
}

void FORTE_BOOL2BOOL::alg_REQ(void) {

#line 2 "BOOL2BOOL.fbt"
  var_OUT = var_IN;
}
