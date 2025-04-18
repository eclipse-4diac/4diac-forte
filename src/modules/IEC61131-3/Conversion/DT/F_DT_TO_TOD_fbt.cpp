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

USE_STRING_ID(CNF);
USE_STRING_ID(DATE_AND_TIME);
USE_STRING_ID(Event);
USE_STRING_ID(F_DT_TO_TOD);
USE_STRING_ID(IN);
USE_STRING_ID(OUT);
USE_STRING_ID(REQ);
USE_STRING_ID(TIME_OF_DAY);

#include "criticalregion.h"
#include "resource.h"
#include "forte_time_of_day.h"
#include "forte_date_and_time.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"

DEFINE_FIRMWARE_FB(FORTE_F_DT_TO_TOD, STRID(F_DT_TO_TOD))

const CStringDictionary::TStringId FORTE_F_DT_TO_TOD::scmDataInputNames[] = {STRID(IN)};

const CStringDictionary::TStringId FORTE_F_DT_TO_TOD::scmDataInputTypeIds[] = {STRID(DATE_AND_TIME)};

const CStringDictionary::TStringId FORTE_F_DT_TO_TOD::scmDataOutputNames[] = {STRID(OUT)};

const CStringDictionary::TStringId FORTE_F_DT_TO_TOD::scmDataOutputTypeIds[] = {STRID(TIME_OF_DAY)};

const TDataIOID FORTE_F_DT_TO_TOD::scmEIWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_F_DT_TO_TOD::scmEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_F_DT_TO_TOD::scmEventInputNames[] = {STRID(REQ)};
const CStringDictionary::TStringId FORTE_F_DT_TO_TOD::scmEventInputTypeIds[] = {STRID(Event)};

const TDataIOID FORTE_F_DT_TO_TOD::scmEOWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_F_DT_TO_TOD::scmEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_F_DT_TO_TOD::scmEventOutputNames[] = {STRID(CNF)};
const CStringDictionary::TStringId FORTE_F_DT_TO_TOD::scmEventOutputTypeIds[] = {STRID(Event)};

const SFBInterfaceSpec FORTE_F_DT_TO_TOD::scmFBInterfaceSpec = {1,
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

FORTE_F_DT_TO_TOD::FORTE_F_DT_TO_TOD(CStringDictionary::TStringId paInstanceNameId,
                                     forte::core::CFBContainer &paContainer) :
    CSimpleFB(paContainer, scmFBInterfaceSpec, paInstanceNameId, nullptr),
    var_IN(CIEC_DATE_AND_TIME(0)),
    var_OUT(CIEC_TIME_OF_DAY(0)),
    conn_CNF(*this, 0),
    conn_IN(nullptr),
    conn_OUT(*this, 0, var_OUT) {
}

void FORTE_F_DT_TO_TOD::alg_REQ(void) {

  var_OUT = func_DT_TO_TOD(var_IN);
}

void FORTE_F_DT_TO_TOD::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
  switch (paEIID) {
    case scmEventREQID: alg_REQ(); break;
    default: break;
  }
  sendOutputEvent(scmEventCNFID, paECET);
}

void FORTE_F_DT_TO_TOD::readInputData(TEventID paEIID) {
  switch (paEIID) {
    case scmEventREQID: {
      readData(0, var_IN, conn_IN);
      break;
    }
    default: break;
  }
}

void FORTE_F_DT_TO_TOD::writeOutputData(TEventID paEIID) {
  switch (paEIID) {
    case scmEventCNFID: {
      writeData(0, var_OUT, conn_OUT);
      break;
    }
    default: break;
  }
}

CIEC_ANY *FORTE_F_DT_TO_TOD::getDI(size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_IN;
  }
  return nullptr;
}

CIEC_ANY *FORTE_F_DT_TO_TOD::getDO(size_t paIndex) {
  switch (paIndex) {
    case 0: return &var_OUT;
  }
  return nullptr;
}

CEventConnection *FORTE_F_DT_TO_TOD::getEOConUnchecked(TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_F_DT_TO_TOD::getDIConUnchecked(TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_IN;
  }
  return nullptr;
}

CDataConnection *FORTE_F_DT_TO_TOD::getDOConUnchecked(TPortId paIndex) {
  switch (paIndex) {
    case 0: return &conn_OUT;
  }
  return nullptr;
}

CIEC_ANY *FORTE_F_DT_TO_TOD::getVarInternal(size_t) {
  return nullptr;
}
