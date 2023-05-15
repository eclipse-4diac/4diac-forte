/*******************************************************************************
 * Copyright (c) 2011 - 2013 ACIN, Profactor GmbH, fortiss GmbH
 *               2023 Martin Erich Jobst
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Monika Wenger, Alois Zoitl, Gerhard Ebenhofer
 *   - initial API and implementation and/or initial documentation
 *   Martin Jobst
 *     - refactor for ANY variant
 *******************************************************************************/

#include "F_LIMIT.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "F_LIMIT_gen.cpp"
#endif

#include "criticalregion.h"
#include "resource.h"

DEFINE_FIRMWARE_FB(FORTE_F_LIMIT, g_nStringIdF_LIMIT)

const CStringDictionary::TStringId FORTE_F_LIMIT::scm_anDataInputNames[] = {g_nStringIdMN, g_nStringIdIN, g_nStringIdMX};

const CStringDictionary::TStringId FORTE_F_LIMIT::scm_anDataInputTypeIds[] = {g_nStringIdANY_ELEMENTARY, g_nStringIdANY_ELEMENTARY, g_nStringIdANY_ELEMENTARY};

const CStringDictionary::TStringId FORTE_F_LIMIT::scm_anDataOutputNames[] = {g_nStringIdOUT};

const CStringDictionary::TStringId FORTE_F_LIMIT::scm_anDataOutputTypeIds[] = {g_nStringIdANY_ELEMENTARY};

const TDataIOID FORTE_F_LIMIT::scm_anEIWith[] = {0, 2, 1, 255};
const TForteInt16 FORTE_F_LIMIT::scm_anEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_F_LIMIT::scm_anEventInputNames[] = {g_nStringIdREQ};

const TDataIOID FORTE_F_LIMIT::scm_anEOWith[] = {0, 255};
const TForteInt16 FORTE_F_LIMIT::scm_anEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_F_LIMIT::scm_anEventOutputNames[] = {g_nStringIdCNF};


const SFBInterfaceSpec FORTE_F_LIMIT::scm_stFBInterfaceSpec = {
  1, scm_anEventInputNames, scm_anEIWith, scm_anEIWithIndexes,
  1, scm_anEventOutputNames, scm_anEOWith, scm_anEOWithIndexes,
  3, scm_anDataInputNames, scm_anDataInputTypeIds,
  1, scm_anDataOutputNames, scm_anDataOutputTypeIds,
  0, nullptr
};

FORTE_F_LIMIT::FORTE_F_LIMIT(const CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes) :
    CFunctionBlock( pa_poSrcRes, &scm_stFBInterfaceSpec, pa_nInstanceNameId),
    var_MN(CIEC_ANY_ELEMENTARY_VARIANT()),
    var_IN(CIEC_ANY_ELEMENTARY_VARIANT()),
    var_MX(CIEC_ANY_ELEMENTARY_VARIANT()),
    var_OUT(CIEC_ANY_ELEMENTARY_VARIANT()),
    var_conn_OUT(var_OUT),
    conn_CNF(this, 0),
    conn_MN(nullptr),
    conn_IN(nullptr),
    conn_MX(nullptr),
    conn_OUT(this, 0, &var_conn_OUT) {
};

void FORTE_F_LIMIT::executeEvent(TEventID pa_nEIID) {
  switch(pa_nEIID) {
    case scm_nEventREQID:
      var_OUT = var_IN <= var_MX ? (var_IN >= var_MN ? var_IN : var_MN) : var_MX;
      sendOutputEvent(scm_nEventCNFID);
      break;
  }
}

void FORTE_F_LIMIT::readInputData(TEventID pa_nEIID) {
  switch(pa_nEIID) {
    case scm_nEventREQID: {
      RES_DATA_CON_CRITICAL_REGION();
      readData(0, &var_MN, conn_MN);
      readData(2, &var_MX, conn_MX);
      readData(1, &var_IN, conn_IN);
      break;
    }
    default:
      break;
  }
}

void FORTE_F_LIMIT::writeOutputData(TEventID pa_nEIID) {
  switch(pa_nEIID) {
    case scm_nEventCNFID: {
      RES_DATA_CON_CRITICAL_REGION();
      writeData(0, &var_OUT, &conn_OUT);
      break;
    }
    default:
      break;
  }
}

CIEC_ANY *FORTE_F_LIMIT::getDI(size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_MN;
    case 1: return &var_IN;
    case 2: return &var_MX;
  }
  return nullptr;
}

CIEC_ANY *FORTE_F_LIMIT::getDO(size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_OUT;
  }
  return nullptr;
}

CEventConnection *FORTE_F_LIMIT::getEOConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_F_LIMIT::getDIConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_MN;
    case 1: return &conn_IN;
    case 2: return &conn_MX;
  }
  return nullptr;
}

CDataConnection *FORTE_F_LIMIT::getDOConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_OUT;
  }
  return nullptr;
}


