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

#include "TIME2TIME_fbt.h"

#include "core/datatypes/forte_time.h"
#include "core/iec61131_functions.h"
#include "core/datatypes/forte_array_common.h"
#include "core/datatypes/forte_array.h"
#include "core/datatypes/forte_array_fixed.h"
#include "core/datatypes/forte_array_variable.h"

USE_STRING_ID(CNF);
USE_STRING_ID(Event);
USE_STRING_ID(IN);
USE_STRING_ID(OUT);
USE_STRING_ID(REQ);
USE_STRING_ID(TIME);
USE_STRING_ID(TIME2TIME);

DEFINE_FIRMWARE_FB(FORTE_TIME2TIME, STRID(TIME2TIME))

const CStringDictionary::TStringId FORTE_TIME2TIME::scmDataInputNames[] = {STRID(IN)};
const CStringDictionary::TStringId FORTE_TIME2TIME::scmDataInputTypeIds[] = {STRID(TIME)};
const CStringDictionary::TStringId FORTE_TIME2TIME::scmDataOutputNames[] = {STRID(OUT)};
const CStringDictionary::TStringId FORTE_TIME2TIME::scmDataOutputTypeIds[] = {STRID(TIME)};
const TDataIOID FORTE_TIME2TIME::scmEIWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_TIME2TIME::scmEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_TIME2TIME::scmEventInputNames[] = {STRID(REQ)};
const TDataIOID FORTE_TIME2TIME::scmEOWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_TIME2TIME::scmEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_TIME2TIME::scmEventOutputNames[] = {STRID(CNF)};
const SFBInterfaceSpec FORTE_TIME2TIME::scmFBInterfaceSpec = {
  1, scmEventInputNames, nullptr, scmEIWith, scmEIWithIndexes,
  1, scmEventOutputNames, nullptr, scmEOWith, scmEOWithIndexes,
  1, scmDataInputNames, scmDataInputTypeIds,
  1, scmDataOutputNames, scmDataOutputTypeIds,
  0, nullptr,
  0, nullptr
};

FORTE_TIME2TIME::FORTE_TIME2TIME(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    CSimpleFB(paContainer, scmFBInterfaceSpec, paInstanceNameId, nullptr),
    var_IN(0_TIME),
    var_OUT(0_TIME),
    conn_CNF(*this, 0),
    conn_IN(nullptr),
    conn_OUT(*this, 0, var_OUT) {
}

void FORTE_TIME2TIME::setInitialValues() {
  var_IN = 0_TIME;
  var_OUT = 0_TIME;
}

void FORTE_TIME2TIME::executeEvent(const TEventID paEIID, CEventChainExecutionThread *const paECET) {
  switch(paEIID) {
    case scmEventREQID:
      enterStateREQ(paECET);
      break;
    default:
      break;
  }
}

void FORTE_TIME2TIME::enterStateREQ(CEventChainExecutionThread *const paECET) {
  alg_REQ();
  sendOutputEvent(scmEventCNFID, paECET);
}

void FORTE_TIME2TIME::readInputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventREQID: {
      readData(0, var_IN, conn_IN);
      break;
    }
    default:
      break;
  }
}

void FORTE_TIME2TIME::writeOutputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventCNFID: {
      writeData(scmFBInterfaceSpec.mNumDIs + 0, var_OUT, conn_OUT);
      break;
    }
    default:
      break;
  }
}

CIEC_ANY *FORTE_TIME2TIME::getDI(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_IN;
  }
  return nullptr;
}

CIEC_ANY *FORTE_TIME2TIME::getDO(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_OUT;
  }
  return nullptr;
}

CEventConnection *FORTE_TIME2TIME::getEOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_TIME2TIME::getDIConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_IN;
  }
  return nullptr;
}

CDataConnection *FORTE_TIME2TIME::getDOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_OUT;
  }
  return nullptr;
}

CIEC_ANY *FORTE_TIME2TIME::getVarInternal(size_t) {
  return nullptr;
}

void FORTE_TIME2TIME::alg_REQ(void) {

  #line 2 "TIME2TIME.fbt"
  var_OUT = var_IN;
}
