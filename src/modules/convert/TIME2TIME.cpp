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

#include "TIME2TIME.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "TIME2TIME_gen.cpp"
#endif

#include "criticalregion.h"
#include "resource.h"
#include "forte_time.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"

DEFINE_FIRMWARE_FB(FORTE_TIME2TIME, g_nStringIdTIME2TIME)

const CStringDictionary::TStringId FORTE_TIME2TIME::scm_anDataInputNames[] = {g_nStringIdIN};

const CStringDictionary::TStringId FORTE_TIME2TIME::scm_anDataInputTypeIds[] = {g_nStringIdTIME};

const CStringDictionary::TStringId FORTE_TIME2TIME::scm_anDataOutputNames[] = {g_nStringIdOUT};

const CStringDictionary::TStringId FORTE_TIME2TIME::scm_anDataOutputTypeIds[] = {g_nStringIdTIME};

const TDataIOID FORTE_TIME2TIME::scm_anEIWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_TIME2TIME::scm_anEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_TIME2TIME::scm_anEventInputNames[] = {g_nStringIdREQ};

const TDataIOID FORTE_TIME2TIME::scm_anEOWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_TIME2TIME::scm_anEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_TIME2TIME::scm_anEventOutputNames[] = {g_nStringIdCNF};


const SFBInterfaceSpec FORTE_TIME2TIME::scm_stFBInterfaceSpec = {
  1, scm_anEventInputNames, scm_anEIWith, scm_anEIWithIndexes,
  1, scm_anEventOutputNames, scm_anEOWith, scm_anEOWithIndexes,
  1, scm_anDataInputNames, scm_anDataInputTypeIds,
  1, scm_anDataOutputNames, scm_anDataOutputTypeIds,
  0, nullptr
};


FORTE_TIME2TIME::FORTE_TIME2TIME(CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes) :
    CSimpleFB(pa_poSrcRes, &scm_stFBInterfaceSpec, pa_nInstanceNameId, nullptr),
    var_IN(CIEC_TIME(0)),
    var_OUT(CIEC_TIME(0)),
    var_conn_OUT(var_OUT),
    conn_CNF(this, 0),
    conn_IN(nullptr),
    conn_OUT(this, 0, &var_conn_OUT) {
}

void FORTE_TIME2TIME::alg_REQ(void) {
  
  var_OUT = var_IN;
}


void FORTE_TIME2TIME::executeEvent(TEventID pa_nEIID){
  switch(pa_nEIID) {
    case scm_nEventREQID:
      alg_REQ();
      break;
    default:
      break;
  }
  sendOutputEvent(scm_nEventCNFID);
}

void FORTE_TIME2TIME::readInputData(TEventID pa_nEIID) {
  switch(pa_nEIID) {
    case scm_nEventREQID: {
      RES_DATA_CON_CRITICAL_REGION();
      readData(0, var_IN, conn_IN);
      break;
    }
    default:
      break;
  }
}

void FORTE_TIME2TIME::writeOutputData(TEventID pa_nEIID) {
  switch(pa_nEIID) {
    case scm_nEventCNFID: {
      RES_DATA_CON_CRITICAL_REGION();
      writeData(0, var_OUT, conn_OUT);
      break;
    }
    default:
      break;
  }
}

CIEC_ANY *FORTE_TIME2TIME::getDI(size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_IN;
  }
  return nullptr;
}

CIEC_ANY *FORTE_TIME2TIME::getDO(size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_OUT;
  }
  return nullptr;
}

CEventConnection *FORTE_TIME2TIME::getEOConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_TIME2TIME::getDIConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_IN;
  }
  return nullptr;
}

CDataConnection *FORTE_TIME2TIME::getDOConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_OUT;
  }
  return nullptr;
}

CIEC_ANY *FORTE_TIME2TIME::getVarInternal(size_t) {
  return nullptr;
}


