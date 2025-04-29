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

#include "F_DT_TO_DATE_fbt.h"

#include "core/datatypes/forte_date.h"
#include "core/datatypes/forte_date_and_time.h"
#include "core/iec61131_functions.h"
#include "core/datatypes/forte_array_common.h"
#include "core/datatypes/forte_array.h"
#include "core/datatypes/forte_array_fixed.h"
#include "core/datatypes/forte_array_variable.h"

USE_STRING_ID(CNF);
USE_STRING_ID(DATE);
USE_STRING_ID(DATE_AND_TIME);
USE_STRING_ID(Event);
USE_STRING_ID(F_DT_TO_DATE);
USE_STRING_ID(IN);
USE_STRING_ID(OUT);
USE_STRING_ID(REQ);

DEFINE_FIRMWARE_FB(FORTE_F_DT_TO_DATE, STRID(F_DT_TO_DATE))

const CStringDictionary::TStringId FORTE_F_DT_TO_DATE::scmDataInputNames[] = {STRID(IN)};
const CStringDictionary::TStringId FORTE_F_DT_TO_DATE::scmDataInputTypeIds[] = {STRID(DATE_AND_TIME)};
const CStringDictionary::TStringId FORTE_F_DT_TO_DATE::scmDataOutputNames[] = {STRID(OUT)};
const CStringDictionary::TStringId FORTE_F_DT_TO_DATE::scmDataOutputTypeIds[] = {STRID(DATE)};
const TDataIOID FORTE_F_DT_TO_DATE::scmEIWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_F_DT_TO_DATE::scmEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_F_DT_TO_DATE::scmEventInputNames[] = {STRID(REQ)};
const TDataIOID FORTE_F_DT_TO_DATE::scmEOWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_F_DT_TO_DATE::scmEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_F_DT_TO_DATE::scmEventOutputNames[] = {STRID(CNF)};
const SFBInterfaceSpec FORTE_F_DT_TO_DATE::scmFBInterfaceSpec = {
  1, scmEventInputNames, nullptr, scmEIWith, scmEIWithIndexes,
  1, scmEventOutputNames, nullptr, scmEOWith, scmEOWithIndexes,
  1, scmDataInputNames, scmDataInputTypeIds,
  1, scmDataOutputNames, scmDataOutputTypeIds,
  0, nullptr,
  0, nullptr
};

FORTE_F_DT_TO_DATE::FORTE_F_DT_TO_DATE(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    CSimpleFB(paContainer, scmFBInterfaceSpec, paInstanceNameId, nullptr),
    var_IN(0_DATE_AND_TIME),
    var_OUT(0_DATE),
    conn_CNF(*this, 0),
    conn_IN(nullptr),
    conn_OUT(*this, 0, var_OUT) {
}

void FORTE_F_DT_TO_DATE::setInitialValues() {
  var_IN = 0_DATE_AND_TIME;
  var_OUT = 0_DATE;
}

void FORTE_F_DT_TO_DATE::executeEvent(const TEventID paEIID, CEventChainExecutionThread *const paECET) {
  switch(paEIID) {
    case scmEventREQID:
      enterStateREQ(paECET);
      break;
    default:
      break;
  }
}

void FORTE_F_DT_TO_DATE::enterStateREQ(CEventChainExecutionThread *const paECET) {
  alg_REQ();
  sendOutputEvent(scmEventCNFID, paECET);
}

void FORTE_F_DT_TO_DATE::readInputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventREQID: {
      readData(0, var_IN, conn_IN);
      break;
    }
    default:
      break;
  }
}

void FORTE_F_DT_TO_DATE::writeOutputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventCNFID: {
      writeData(scmFBInterfaceSpec.mNumDIs + 0, var_OUT, conn_OUT);
      break;
    }
    default:
      break;
  }
}

CIEC_ANY *FORTE_F_DT_TO_DATE::getDI(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_IN;
  }
  return nullptr;
}

CIEC_ANY *FORTE_F_DT_TO_DATE::getDO(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_OUT;
  }
  return nullptr;
}

CEventConnection *FORTE_F_DT_TO_DATE::getEOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_F_DT_TO_DATE::getDIConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_IN;
  }
  return nullptr;
}

CDataConnection *FORTE_F_DT_TO_DATE::getDOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_OUT;
  }
  return nullptr;
}

CIEC_ANY *FORTE_F_DT_TO_DATE::getVarInternal(size_t) {
  return nullptr;
}

void FORTE_F_DT_TO_DATE::alg_REQ(void) {

  #line 2 "F_DT_TO_DATE.fbt"
  var_OUT = func_DT_TO_DATE(var_IN);
}
