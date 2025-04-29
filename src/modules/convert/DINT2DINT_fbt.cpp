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

#include "DINT2DINT_fbt.h"

#include "core/datatypes/forte_dint.h"
#include "core/iec61131_functions.h"
#include "core/datatypes/forte_array_common.h"
#include "core/datatypes/forte_array.h"
#include "core/datatypes/forte_array_fixed.h"
#include "core/datatypes/forte_array_variable.h"

USE_STRING_ID(CNF);
USE_STRING_ID(DINT);
USE_STRING_ID(DINT2DINT);
USE_STRING_ID(Event);
USE_STRING_ID(IN);
USE_STRING_ID(OUT);
USE_STRING_ID(REQ);

DEFINE_FIRMWARE_FB(FORTE_DINT2DINT, STRID(DINT2DINT))

const CStringDictionary::TStringId FORTE_DINT2DINT::scmDataInputNames[] = {STRID(IN)};
const CStringDictionary::TStringId FORTE_DINT2DINT::scmDataInputTypeIds[] = {STRID(DINT)};
const CStringDictionary::TStringId FORTE_DINT2DINT::scmDataOutputNames[] = {STRID(OUT)};
const CStringDictionary::TStringId FORTE_DINT2DINT::scmDataOutputTypeIds[] = {STRID(DINT)};
const TDataIOID FORTE_DINT2DINT::scmEIWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_DINT2DINT::scmEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_DINT2DINT::scmEventInputNames[] = {STRID(REQ)};
const TDataIOID FORTE_DINT2DINT::scmEOWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_DINT2DINT::scmEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_DINT2DINT::scmEventOutputNames[] = {STRID(CNF)};
const SFBInterfaceSpec FORTE_DINT2DINT::scmFBInterfaceSpec = {
  1, scmEventInputNames, nullptr, scmEIWith, scmEIWithIndexes,
  1, scmEventOutputNames, nullptr, scmEOWith, scmEOWithIndexes,
  1, scmDataInputNames, scmDataInputTypeIds,
  1, scmDataOutputNames, scmDataOutputTypeIds,
  0, nullptr,
  0, nullptr
};

FORTE_DINT2DINT::FORTE_DINT2DINT(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    CSimpleFB(paContainer, scmFBInterfaceSpec, paInstanceNameId, nullptr),
    var_IN(0_DINT),
    var_OUT(0_DINT),
    conn_CNF(*this, 0),
    conn_IN(nullptr),
    conn_OUT(*this, 0, var_OUT) {
}

void FORTE_DINT2DINT::setInitialValues() {
  var_IN = 0_DINT;
  var_OUT = 0_DINT;
}

void FORTE_DINT2DINT::executeEvent(const TEventID paEIID, CEventChainExecutionThread *const paECET) {
  switch(paEIID) {
    case scmEventREQID:
      enterStateREQ(paECET);
      break;
    default:
      break;
  }
}

void FORTE_DINT2DINT::enterStateREQ(CEventChainExecutionThread *const paECET) {
  alg_REQ();
  sendOutputEvent(scmEventCNFID, paECET);
}

void FORTE_DINT2DINT::readInputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventREQID: {
      readData(0, var_IN, conn_IN);
      break;
    }
    default:
      break;
  }
}

void FORTE_DINT2DINT::writeOutputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventCNFID: {
      writeData(scmFBInterfaceSpec.mNumDIs + 0, var_OUT, conn_OUT);
      break;
    }
    default:
      break;
  }
}

CIEC_ANY *FORTE_DINT2DINT::getDI(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_IN;
  }
  return nullptr;
}

CIEC_ANY *FORTE_DINT2DINT::getDO(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_OUT;
  }
  return nullptr;
}

CEventConnection *FORTE_DINT2DINT::getEOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_DINT2DINT::getDIConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_IN;
  }
  return nullptr;
}

CDataConnection *FORTE_DINT2DINT::getDOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_OUT;
  }
  return nullptr;
}

CIEC_ANY *FORTE_DINT2DINT::getVarInternal(size_t) {
  return nullptr;
}

void FORTE_DINT2DINT::alg_REQ(void) {

  #line 2 "DINT2DINT.fbt"
  var_OUT = var_IN;
}
