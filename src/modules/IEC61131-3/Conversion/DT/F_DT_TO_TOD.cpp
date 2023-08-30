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

#include "F_DT_TO_TOD.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "F_DT_TO_TOD_gen.cpp"
#endif

#include "criticalregion.h"
#include "resource.h"
#include "forte_time_of_day.h"
#include "forte_date_and_time.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"

DEFINE_FIRMWARE_FB(FORTE_F_DT_TO_TOD, g_nStringIdF_DT_TO_TOD)

const CStringDictionary::TStringId FORTE_F_DT_TO_TOD::scmDataInputNames[] = {g_nStringIdIN};

const CStringDictionary::TStringId FORTE_F_DT_TO_TOD::scmDataInputTypeIds[] = {g_nStringIdDATE_AND_TIME};

const CStringDictionary::TStringId FORTE_F_DT_TO_TOD::scmDataOutputNames[] = {g_nStringIdOUT};

const CStringDictionary::TStringId FORTE_F_DT_TO_TOD::scmDataOutputTypeIds[] = {g_nStringIdTIME_OF_DAY};

const TDataIOID FORTE_F_DT_TO_TOD::scmEIWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_F_DT_TO_TOD::scmEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_F_DT_TO_TOD::scmEventInputNames[] = {g_nStringIdREQ};

const TDataIOID FORTE_F_DT_TO_TOD::scmEOWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_F_DT_TO_TOD::scmEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_F_DT_TO_TOD::scmEventOutputNames[] = {g_nStringIdCNF};


const SFBInterfaceSpec FORTE_F_DT_TO_TOD::scmFBInterfaceSpec = {
  1, scmEventInputNames, scmEIWith, scmEIWithIndexes,
  1, scmEventOutputNames, scmEOWith, scmEOWithIndexes,
  1, scmDataInputNames, scmDataInputTypeIds,
  1, scmDataOutputNames, scmDataOutputTypeIds,
  0, nullptr,
  0, nullptr
};


FORTE_F_DT_TO_TOD::FORTE_F_DT_TO_TOD(CStringDictionary::TStringId paInstanceNameId, CResource *paSrcRes) :
    CSimpleFB(paSrcRes, &scmFBInterfaceSpec, paInstanceNameId, nullptr),
    var_IN(CIEC_DATE_AND_TIME(0)),
    var_OUT(CIEC_TIME_OF_DAY(0)),
    var_conn_OUT(var_OUT),
    conn_CNF(this, 0),
    conn_IN(nullptr),
    conn_OUT(this, 0, &var_conn_OUT) {
}

void FORTE_F_DT_TO_TOD::alg_REQ(void) {
  
  var_OUT = func_DT_TO_TOD(var_IN);
}


void FORTE_F_DT_TO_TOD::executeEvent(TEventID paEIID){
  switch(paEIID) {
    case scmEventREQID:
      alg_REQ();
      break;
    default:
      break;
  }
  sendOutputEvent(scmEventCNFID);
}

void FORTE_F_DT_TO_TOD::readInputData(TEventID paEIID) {
  switch(paEIID) {
    case scmEventREQID: {
      RES_DATA_CON_CRITICAL_REGION();
      readData(0, var_IN, conn_IN);
      break;
    }
    default:
      break;
  }
}

void FORTE_F_DT_TO_TOD::writeOutputData(TEventID paEIID) {
  switch(paEIID) {
    case scmEventCNFID: {
      RES_DATA_CON_CRITICAL_REGION();
      writeData(0, var_OUT, conn_OUT);
      break;
    }
    default:
      break;
  }
}

CIEC_ANY *FORTE_F_DT_TO_TOD::getDI(size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_IN;
  }
  return nullptr;
}

CIEC_ANY *FORTE_F_DT_TO_TOD::getDO(size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_OUT;
  }
  return nullptr;
}

CEventConnection *FORTE_F_DT_TO_TOD::getEOConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_F_DT_TO_TOD::getDIConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_IN;
  }
  return nullptr;
}

CDataConnection *FORTE_F_DT_TO_TOD::getDOConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_OUT;
  }
  return nullptr;
}

CIEC_ANY *FORTE_F_DT_TO_TOD::getVarInternal(size_t) {
  return nullptr;
}


