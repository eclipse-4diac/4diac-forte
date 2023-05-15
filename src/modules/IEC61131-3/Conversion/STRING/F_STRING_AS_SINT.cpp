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

#include "F_STRING_AS_SINT.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "F_STRING_AS_SINT_gen.cpp"
#endif

#include "criticalregion.h"
#include "resource.h"
#include "forte_sint.h"
#include "forte_string.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"

DEFINE_FIRMWARE_FB(FORTE_F_STRING_AS_SINT, g_nStringIdF_STRING_AS_SINT)

const CStringDictionary::TStringId FORTE_F_STRING_AS_SINT::scm_anDataInputNames[] = {g_nStringIdIN};

const CStringDictionary::TStringId FORTE_F_STRING_AS_SINT::scm_anDataInputTypeIds[] = {g_nStringIdSTRING};

const CStringDictionary::TStringId FORTE_F_STRING_AS_SINT::scm_anDataOutputNames[] = {g_nStringIdOUT};

const CStringDictionary::TStringId FORTE_F_STRING_AS_SINT::scm_anDataOutputTypeIds[] = {g_nStringIdSINT};

const TDataIOID FORTE_F_STRING_AS_SINT::scm_anEIWith[] = {0, 255};
const TForteInt16 FORTE_F_STRING_AS_SINT::scm_anEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_F_STRING_AS_SINT::scm_anEventInputNames[] = {g_nStringIdREQ};

const TDataIOID FORTE_F_STRING_AS_SINT::scm_anEOWith[] = {0, 255};
const TForteInt16 FORTE_F_STRING_AS_SINT::scm_anEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_F_STRING_AS_SINT::scm_anEventOutputNames[] = {g_nStringIdCNF};


const SFBInterfaceSpec FORTE_F_STRING_AS_SINT::scm_stFBInterfaceSpec = {
  1, scm_anEventInputNames, scm_anEIWith, scm_anEIWithIndexes,
  1, scm_anEventOutputNames, scm_anEOWith, scm_anEOWithIndexes,
  1, scm_anDataInputNames, scm_anDataInputTypeIds,
  1, scm_anDataOutputNames, scm_anDataOutputTypeIds,
  0, nullptr
};


FORTE_F_STRING_AS_SINT::FORTE_F_STRING_AS_SINT(CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes) :
    CSimpleFB(pa_poSrcRes, &scm_stFBInterfaceSpec, pa_nInstanceNameId, nullptr),
    var_IN(CIEC_STRING("")),
    var_OUT(CIEC_SINT(0)),
    var_conn_OUT(var_OUT),
    conn_CNF(this, 0),
    conn_IN(nullptr),
    conn_OUT(this, 0, &var_conn_OUT) {
}

void FORTE_F_STRING_AS_SINT::alg_REQ(void) {
  
  var_OUT = func_STRING_AS_SINT(var_IN);
}


void FORTE_F_STRING_AS_SINT::executeEvent(TEventID pa_nEIID){
  switch(pa_nEIID) {
    case scm_nEventREQID:
      alg_REQ();
      break;
    default:
      break;
  }
  sendOutputEvent(scm_nEventCNFID);
}

void FORTE_F_STRING_AS_SINT::readInputData(TEventID pa_nEIID) {
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

void FORTE_F_STRING_AS_SINT::writeOutputData(TEventID pa_nEIID) {
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

CIEC_ANY *FORTE_F_STRING_AS_SINT::getDI(size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_IN;
  }
  return nullptr;
}

CIEC_ANY *FORTE_F_STRING_AS_SINT::getDO(size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_OUT;
  }
  return nullptr;
}

CEventConnection *FORTE_F_STRING_AS_SINT::getEOConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_F_STRING_AS_SINT::getDIConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_IN;
  }
  return nullptr;
}

CDataConnection *FORTE_F_STRING_AS_SINT::getDOConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_OUT;
  }
  return nullptr;
}

CIEC_ANY *FORTE_F_STRING_AS_SINT::getVarInternal(size_t) {
  return nullptr;
}


