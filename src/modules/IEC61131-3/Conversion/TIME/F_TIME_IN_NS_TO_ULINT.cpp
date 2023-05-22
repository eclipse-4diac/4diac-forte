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

#include "F_TIME_IN_NS_TO_ULINT.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "F_TIME_IN_NS_TO_ULINT_gen.cpp"
#endif

#include "criticalregion.h"
#include "resource.h"
#include "forte_ulint.h"
#include "forte_time.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"

DEFINE_FIRMWARE_FB(FORTE_F_TIME_IN_NS_TO_ULINT, g_nStringIdF_TIME_IN_NS_TO_ULINT)

const CStringDictionary::TStringId FORTE_F_TIME_IN_NS_TO_ULINT::scm_anDataInputNames[] = {g_nStringIdIN};

const CStringDictionary::TStringId FORTE_F_TIME_IN_NS_TO_ULINT::scm_anDataInputTypeIds[] = {g_nStringIdTIME};

const CStringDictionary::TStringId FORTE_F_TIME_IN_NS_TO_ULINT::scm_anDataOutputNames[] = {g_nStringIdOUT};

const CStringDictionary::TStringId FORTE_F_TIME_IN_NS_TO_ULINT::scm_anDataOutputTypeIds[] = {g_nStringIdULINT};

const TDataIOID FORTE_F_TIME_IN_NS_TO_ULINT::scm_anEIWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_F_TIME_IN_NS_TO_ULINT::scm_anEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_F_TIME_IN_NS_TO_ULINT::scm_anEventInputNames[] = {g_nStringIdREQ};

const TDataIOID FORTE_F_TIME_IN_NS_TO_ULINT::scm_anEOWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_F_TIME_IN_NS_TO_ULINT::scm_anEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_F_TIME_IN_NS_TO_ULINT::scm_anEventOutputNames[] = {g_nStringIdCNF};


const SFBInterfaceSpec FORTE_F_TIME_IN_NS_TO_ULINT::scm_stFBInterfaceSpec = {
  1, scm_anEventInputNames, scm_anEIWith, scm_anEIWithIndexes,
  1, scm_anEventOutputNames, scm_anEOWith, scm_anEOWithIndexes,
  1, scm_anDataInputNames, scm_anDataInputTypeIds,
  1, scm_anDataOutputNames, scm_anDataOutputTypeIds,
  0, nullptr
};


FORTE_F_TIME_IN_NS_TO_ULINT::FORTE_F_TIME_IN_NS_TO_ULINT(CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes) :
    CSimpleFB(pa_poSrcRes, &scm_stFBInterfaceSpec, pa_nInstanceNameId, nullptr),
    var_IN(CIEC_TIME(0)),
    var_OUT(CIEC_ULINT(0)),
    var_conn_OUT(var_OUT),
    conn_CNF(this, 0),
    conn_IN(nullptr),
    conn_OUT(this, 0, &var_conn_OUT) {
}

void FORTE_F_TIME_IN_NS_TO_ULINT::alg_REQ(void) {
  
  var_OUT = func_TIME_IN_NS_TO_ULINT(var_IN);
}


void FORTE_F_TIME_IN_NS_TO_ULINT::executeEvent(TEventID pa_nEIID){
  switch(pa_nEIID) {
    case scm_nEventREQID:
      alg_REQ();
      break;
    default:
      break;
  }
  sendOutputEvent(scm_nEventCNFID);
}

void FORTE_F_TIME_IN_NS_TO_ULINT::readInputData(TEventID pa_nEIID) {
  switch(pa_nEIID) {
    case scm_nEventREQID: {
      RES_DATA_CON_CRITICAL_REGION();
      readData(0, &var_IN, conn_IN);
      break;
    }
    default:
      break;
  }
}

void FORTE_F_TIME_IN_NS_TO_ULINT::writeOutputData(TEventID pa_nEIID) {
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

CIEC_ANY *FORTE_F_TIME_IN_NS_TO_ULINT::getDI(size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_IN;
  }
  return nullptr;
}

CIEC_ANY *FORTE_F_TIME_IN_NS_TO_ULINT::getDO(size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_OUT;
  }
  return nullptr;
}

CEventConnection *FORTE_F_TIME_IN_NS_TO_ULINT::getEOConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_F_TIME_IN_NS_TO_ULINT::getDIConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_IN;
  }
  return nullptr;
}

CDataConnection *FORTE_F_TIME_IN_NS_TO_ULINT::getDOConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_OUT;
  }
  return nullptr;
}

CIEC_ANY *FORTE_F_TIME_IN_NS_TO_ULINT::getVarInternal(size_t) {
  return nullptr;
}


