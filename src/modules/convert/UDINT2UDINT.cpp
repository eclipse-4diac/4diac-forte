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

#include "UDINT2UDINT.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "UDINT2UDINT_gen.cpp"
#endif

#include "criticalregion.h"
#include "resource.h"
#include "forte_udint.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"

DEFINE_FIRMWARE_FB(FORTE_UDINT2UDINT, g_nStringIdUDINT2UDINT)

const CStringDictionary::TStringId FORTE_UDINT2UDINT::scm_anDataInputNames[] = {g_nStringIdIN};

const CStringDictionary::TStringId FORTE_UDINT2UDINT::scm_anDataInputTypeIds[] = {g_nStringIdUDINT};

const CStringDictionary::TStringId FORTE_UDINT2UDINT::scm_anDataOutputNames[] = {g_nStringIdOUT};

const CStringDictionary::TStringId FORTE_UDINT2UDINT::scm_anDataOutputTypeIds[] = {g_nStringIdUDINT};

const TDataIOID FORTE_UDINT2UDINT::scm_anEIWith[] = {0, 255};
const TForteInt16 FORTE_UDINT2UDINT::scm_anEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_UDINT2UDINT::scm_anEventInputNames[] = {g_nStringIdREQ};

const TDataIOID FORTE_UDINT2UDINT::scm_anEOWith[] = {0, 255};
const TForteInt16 FORTE_UDINT2UDINT::scm_anEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_UDINT2UDINT::scm_anEventOutputNames[] = {g_nStringIdCNF};


const SFBInterfaceSpec FORTE_UDINT2UDINT::scm_stFBInterfaceSpec = {
  1, scm_anEventInputNames, scm_anEIWith, scm_anEIWithIndexes,
  1, scm_anEventOutputNames, scm_anEOWith, scm_anEOWithIndexes,
  1, scm_anDataInputNames, scm_anDataInputTypeIds,
  1, scm_anDataOutputNames, scm_anDataOutputTypeIds,
  0, nullptr
};


FORTE_UDINT2UDINT::FORTE_UDINT2UDINT(CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes) :
    CSimpleFB(pa_poSrcRes, &scm_stFBInterfaceSpec, pa_nInstanceNameId, nullptr),
    var_IN(CIEC_UDINT(0)),
    var_OUT(CIEC_UDINT(0)),
    var_conn_OUT(var_OUT),
    conn_CNF(this, 0),
    conn_IN(nullptr),
    conn_OUT(this, 0, &var_conn_OUT) {
}

void FORTE_UDINT2UDINT::alg_REQ(void) {
  
  var_OUT = var_IN;
}


void FORTE_UDINT2UDINT::executeEvent(int pa_nEIID){
  switch(pa_nEIID) {
    case scm_nEventREQID:
      alg_REQ();
      break;
    default:
      break;
  }
  sendOutputEvent(scm_nEventCNFID);
}

void FORTE_UDINT2UDINT::readInputData(size_t pa_nEIID) {
  switch(pa_nEIID) {
    case scm_nEventREQID: {
      CCriticalRegion criticalRegion(getResource().m_oResDataConSync);
      readData(0, &var_IN, conn_IN);
      break;
    }
    default:
      break;
  }
}

void FORTE_UDINT2UDINT::writeOutputData(size_t pa_nEIID) {
  switch(pa_nEIID) {
    case scm_nEventCNFID: {
      CCriticalRegion criticalRegion(getResource().m_oResDataConSync);
      writeData(0, &var_OUT, &conn_OUT);
      break;
    }
    default:
      break;
  }
}

CIEC_ANY *FORTE_UDINT2UDINT::getDI(size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_IN;
  }
  return nullptr;
}

CIEC_ANY *FORTE_UDINT2UDINT::getDO(size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_OUT;
  }
  return nullptr;
}

CEventConnection *FORTE_UDINT2UDINT::getEOConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_UDINT2UDINT::getDIConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_IN;
  }
  return nullptr;
}

CDataConnection *FORTE_UDINT2UDINT::getDOConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_OUT;
  }
  return nullptr;
}

CIEC_ANY *FORTE_UDINT2UDINT::getVarInternal(size_t) {
  return nullptr;
}


