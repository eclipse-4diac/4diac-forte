/*******************************************************************************
 * Copyright (c) 2013 ACIN
 *               2023 Martin Erich Jobst
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Monika Wenger
 *   - initial API and implementation and/or initial documentation
 *   Martin Jobst
 *     - refactor for ANY variant
 *******************************************************************************/

#include "F_CONCAT_DATE_TOD.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "F_CONCAT_DATE_TOD_gen.cpp"
#endif

#include "criticalregion.h"
#include "resource.h"

DEFINE_FIRMWARE_FB(FORTE_F_CONCAT_DATE_TOD, g_nStringIdF_CONCAT_DATE_TOD)

const CStringDictionary::TStringId FORTE_F_CONCAT_DATE_TOD::scmDataInputNames[] = {g_nStringIdIN1, g_nStringIdIN2};

const CStringDictionary::TStringId FORTE_F_CONCAT_DATE_TOD::scmDataInputTypeIds[] = {g_nStringIdDATE, g_nStringIdTIME_OF_DAY};

const CStringDictionary::TStringId FORTE_F_CONCAT_DATE_TOD::scmDataOutputNames[] = {g_nStringIdOUT};

const CStringDictionary::TStringId FORTE_F_CONCAT_DATE_TOD::scmDataOutputTypeIds[] = {g_nStringIdDATE_AND_TIME};

const TDataIOID FORTE_F_CONCAT_DATE_TOD::scmEIWith[] = {0, 1, scmWithListDelimiter};
const TForteInt16 FORTE_F_CONCAT_DATE_TOD::scmEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_F_CONCAT_DATE_TOD::scmEventInputNames[] = {g_nStringIdREQ};

const TDataIOID FORTE_F_CONCAT_DATE_TOD::scmEOWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_F_CONCAT_DATE_TOD::scmEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_F_CONCAT_DATE_TOD::scmEventOutputNames[] = {g_nStringIdCNF};


const SFBInterfaceSpec FORTE_F_CONCAT_DATE_TOD::scmFBInterfaceSpec = {
  1, scmEventInputNames, scmEIWith, scmEIWithIndexes,
  1, scmEventOutputNames, scmEOWith, scmEOWithIndexes,
  2, scmDataInputNames, scmDataInputTypeIds,
  1, scmDataOutputNames, scmDataOutputTypeIds,
  0, nullptr,
  0, nullptr
};

FORTE_F_CONCAT_DATE_TOD::FORTE_F_CONCAT_DATE_TOD(const CStringDictionary::TStringId paInstanceNameId, CResource *paSrcRes) :
    CFunctionBlock( paSrcRes, &scmFBInterfaceSpec, paInstanceNameId),
    var_IN1(CIEC_DATE(0)),
    var_IN2(CIEC_TIME_OF_DAY(0)),
    var_OUT(CIEC_DATE_AND_TIME(0)),
    var_conn_OUT(var_OUT),
    conn_CNF(this, 0),
    conn_IN1(nullptr),
    conn_IN2(nullptr),
    conn_OUT(this, 0, &var_conn_OUT) {
};

void FORTE_F_CONCAT_DATE_TOD::executeEvent(TEventID paEIID) {
  switch(paEIID) {
    case scmEventREQID:
      var_OUT = func_CONCAT_DATE_TOD(var_IN1, var_IN2);
      sendOutputEvent(scmEventCNFID);
      break;
  }
}

void FORTE_F_CONCAT_DATE_TOD::readInputData(TEventID paEIID) {
  switch(paEIID) {
    case scmEventREQID: {
      RES_DATA_CON_CRITICAL_REGION();
      readData(0, var_IN1, conn_IN1);
      readData(1, var_IN2, conn_IN2);
      break;
    }
    default:
      break;
  }
}

void FORTE_F_CONCAT_DATE_TOD::writeOutputData(TEventID paEIID) {
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

CIEC_ANY *FORTE_F_CONCAT_DATE_TOD::getDI(size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_IN1;
    case 1: return &var_IN2;
  }
  return nullptr;
}

CIEC_ANY *FORTE_F_CONCAT_DATE_TOD::getDO(size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_OUT;
  }
  return nullptr;
}

CEventConnection *FORTE_F_CONCAT_DATE_TOD::getEOConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_F_CONCAT_DATE_TOD::getDIConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_IN1;
    case 1: return &conn_IN2;
  }
  return nullptr;
}

CDataConnection *FORTE_F_CONCAT_DATE_TOD::getDOConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_OUT;
  }
  return nullptr;
}


