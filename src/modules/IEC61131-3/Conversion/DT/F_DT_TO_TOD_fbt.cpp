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

#include "F_DT_TO_TOD_fbt.h"

#include "core/datatypes/forte_date_and_time.h"
#include "core/datatypes/forte_time_of_day.h"
#include "core/iec61131_functions.h"
#include "core/datatypes/forte_array_common.h"
#include "core/datatypes/forte_array.h"
#include "core/datatypes/forte_array_fixed.h"
#include "core/datatypes/forte_array_variable.h"

USE_STRING_ID(CNF);
USE_STRING_ID(DATE_AND_TIME);
USE_STRING_ID(Event);
USE_STRING_ID(F_DT_TO_TOD);
USE_STRING_ID(IN);
USE_STRING_ID(OUT);
USE_STRING_ID(REQ);
USE_STRING_ID(TIME_OF_DAY);

DEFINE_FIRMWARE_FB(FORTE_F_DT_TO_TOD, STRID(F_DT_TO_TOD))

const CStringDictionary::TStringId FORTE_F_DT_TO_TOD::scmDataInputNames[] = {STRID(IN)};
const CStringDictionary::TStringId FORTE_F_DT_TO_TOD::scmDataInputTypeIds[] = {STRID(DATE_AND_TIME)};
const CStringDictionary::TStringId FORTE_F_DT_TO_TOD::scmDataOutputNames[] = {STRID(OUT)};
const CStringDictionary::TStringId FORTE_F_DT_TO_TOD::scmDataOutputTypeIds[] = {STRID(TIME_OF_DAY)};
const TDataIOID FORTE_F_DT_TO_TOD::scmEIWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_F_DT_TO_TOD::scmEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_F_DT_TO_TOD::scmEventInputNames[] = {STRID(REQ)};
const TDataIOID FORTE_F_DT_TO_TOD::scmEOWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_F_DT_TO_TOD::scmEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_F_DT_TO_TOD::scmEventOutputNames[] = {STRID(CNF)};
const SFBInterfaceSpec FORTE_F_DT_TO_TOD::scmFBInterfaceSpec = {
  1, scmEventInputNames, nullptr, scmEIWith, scmEIWithIndexes,
  1, scmEventOutputNames, nullptr, scmEOWith, scmEOWithIndexes,
  1, scmDataInputNames, scmDataInputTypeIds,
  1, scmDataOutputNames, scmDataOutputTypeIds,
  0, nullptr,
  0, nullptr
};

FORTE_F_DT_TO_TOD::FORTE_F_DT_TO_TOD(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    CSimpleFB(paContainer, scmFBInterfaceSpec, paInstanceNameId, nullptr),
    var_IN(0_DATE_AND_TIME),
    var_OUT(0_TIME_OF_DAY),
    conn_CNF(*this, 0),
    conn_IN(nullptr),
    conn_OUT(*this, 0, var_OUT) {
}

void FORTE_F_DT_TO_TOD::setInitialValues() {
  var_IN = 0_DATE_AND_TIME;
  var_OUT = 0_TIME_OF_DAY;
}

void FORTE_F_DT_TO_TOD::executeEvent(const TEventID paEIID, CEventChainExecutionThread *const paECET) {
  switch(paEIID) {
    case scmEventREQID:
      enterStateREQ(paECET);
      break;
    default:
      break;
  }
}

void FORTE_F_DT_TO_TOD::enterStateREQ(CEventChainExecutionThread *const paECET) {
  alg_REQ();
  sendOutputEvent(scmEventCNFID, paECET);
}

void FORTE_F_DT_TO_TOD::readInputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventREQID: {
      readData(0, var_IN, conn_IN);
      break;
    }
    default:
      break;
  }
}

void FORTE_F_DT_TO_TOD::writeOutputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventCNFID: {
      writeData(scmFBInterfaceSpec.mNumDIs + 0, var_OUT, conn_OUT);
      break;
    }
    default:
      break;
  }
}

CIEC_ANY *FORTE_F_DT_TO_TOD::getDI(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_IN;
  }
  return nullptr;
}

CIEC_ANY *FORTE_F_DT_TO_TOD::getDO(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_OUT;
  }
  return nullptr;
}

CEventConnection *FORTE_F_DT_TO_TOD::getEOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_F_DT_TO_TOD::getDIConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_IN;
  }
  return nullptr;
}

CDataConnection *FORTE_F_DT_TO_TOD::getDOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_OUT;
  }
  return nullptr;
}

CIEC_ANY *FORTE_F_DT_TO_TOD::getVarInternal(size_t) {
  return nullptr;
}

void FORTE_F_DT_TO_TOD::alg_REQ(void) {

  #line 2 "F_DT_TO_TOD.fbt"
  var_OUT = func_DT_TO_TOD(var_IN);
}
