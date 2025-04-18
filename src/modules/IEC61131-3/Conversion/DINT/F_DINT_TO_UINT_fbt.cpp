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

#include "F_DINT_TO_UINT_fbt.h"

USE_STRING_ID(CNF);
USE_STRING_ID(DINT);
USE_STRING_ID(Event);
USE_STRING_ID(F_DINT_TO_UINT);
USE_STRING_ID(IN);
USE_STRING_ID(OUT);
USE_STRING_ID(REQ);
USE_STRING_ID(UINT);

#include "criticalregion.h"
#include "resource.h"
#include "forte_uint.h"
#include "forte_dint.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"

DEFINE_FIRMWARE_FB(FORTE_F_DINT_TO_UINT, STRID(F_DINT_TO_UINT))

const CStringDictionary::TStringId FORTE_F_DINT_TO_UINT::scmDataInputNames[] = {STRID(IN)};

const CStringDictionary::TStringId FORTE_F_DINT_TO_UINT::scmDataInputTypeIds[] = {STRID(DINT)};

const CStringDictionary::TStringId FORTE_F_DINT_TO_UINT::scmDataOutputNames[] = {STRID(OUT)};

const CStringDictionary::TStringId FORTE_F_DINT_TO_UINT::scmDataOutputTypeIds[] = {STRID(UINT)};

const TDataIOID FORTE_F_DINT_TO_UINT::scmEIWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_F_DINT_TO_UINT::scmEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_F_DINT_TO_UINT::scmEventInputNames[] = {STRID(REQ)};
const CStringDictionary::TStringId FORTE_F_DINT_TO_UINT::scmEventInputTypeIds[] = {STRID(Event)};

const TDataIOID FORTE_F_DINT_TO_UINT::scmEOWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_F_DINT_TO_UINT::scmEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_F_DINT_TO_UINT::scmEventOutputNames[] = {STRID(CNF)};
const CStringDictionary::TStringId FORTE_F_DINT_TO_UINT::scmEventOutputTypeIds[] = {STRID(Event)};

const SFBInterfaceSpec FORTE_F_DINT_TO_UINT::scmFBInterfaceSpec = {1,
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

FORTE_F_DINT_TO_UINT::FORTE_F_DINT_TO_UINT(CStringDictionary::TStringId paInstanceNameId,
                                           forte::core::CFBContainer &paContainer) :
    CSimpleFB(paContainer, scmFBInterfaceSpec, paInstanceNameId, nullptr),
    var_IN(CIEC_DINT(0)),
    var_OUT(CIEC_UINT(0)),
    conn_CNF(*this, 0),
    conn_IN(nullptr),
    conn_OUT(*this, 0, var_OUT) {
}

void FORTE_F_DINT_TO_UINT::alg_REQ(void) {

  var_OUT = func_DINT_TO_UINT(var_IN);
}

void FORTE_F_DINT_TO_UINT::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
  switch (paEIID) {
    case scmEventREQID: alg_REQ(); break;
    default: break;
  }
  sendOutputEvent(scmEventCNFID, paECET);
}

void FORTE_F_DINT_TO_UINT::readInputData(TEventID paEIID) {
  switch (paEIID) {
    case scmEventREQID: {
      readData(0, var_IN, conn_IN);
      break;
    }
    default: break;
  }
}

void FORTE_F_DINT_TO_UINT::writeOutputData(TEventID paEIID) {
  switch (paEIID) {
    case scmEventCNFID: {
      writeData(0, var_OUT, conn_OUT);
      break;
    }
    default: break;
  }
}

CIEC_ANY *FORTE_F_DINT_TO_UINT::getDI(size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_IN;
  }
  return nullptr;
}

CIEC_ANY *FORTE_F_DINT_TO_UINT::getDO(size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_OUT;
  }
  return nullptr;
}

CEventConnection *FORTE_F_DINT_TO_UINT::getEOConUnchecked(TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_F_DINT_TO_UINT::getDIConUnchecked(TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_IN;
  }
  return nullptr;
}

CDataConnection *FORTE_F_DINT_TO_UINT::getDOConUnchecked(TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_OUT;
  }
  return nullptr;
}

CIEC_ANY *FORTE_F_DINT_TO_UINT::getVarInternal(size_t) {
  return nullptr;
}
