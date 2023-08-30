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

#include "F_LINT_TO_DWORD.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "F_LINT_TO_DWORD_gen.cpp"
#endif

#include "criticalregion.h"
#include "resource.h"
#include "forte_dword.h"
#include "forte_lint.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"

DEFINE_FIRMWARE_FB(FORTE_F_LINT_TO_DWORD, g_nStringIdF_LINT_TO_DWORD)

const CStringDictionary::TStringId FORTE_F_LINT_TO_DWORD::scmDataInputNames[] = {g_nStringIdIN};

const CStringDictionary::TStringId FORTE_F_LINT_TO_DWORD::scmDataInputTypeIds[] = {g_nStringIdLINT};

const CStringDictionary::TStringId FORTE_F_LINT_TO_DWORD::scmDataOutputNames[] = {g_nStringIdOUT};

const CStringDictionary::TStringId FORTE_F_LINT_TO_DWORD::scmDataOutputTypeIds[] = {g_nStringIdDWORD};

const TDataIOID FORTE_F_LINT_TO_DWORD::scmEIWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_F_LINT_TO_DWORD::scmEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_F_LINT_TO_DWORD::scmEventInputNames[] = {g_nStringIdREQ};

const TDataIOID FORTE_F_LINT_TO_DWORD::scmEOWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_F_LINT_TO_DWORD::scmEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_F_LINT_TO_DWORD::scmEventOutputNames[] = {g_nStringIdCNF};


const SFBInterfaceSpec FORTE_F_LINT_TO_DWORD::scmFBInterfaceSpec = {
  1, scmEventInputNames, scmEIWith, scmEIWithIndexes,
  1, scmEventOutputNames, scmEOWith, scmEOWithIndexes,
  1, scmDataInputNames, scmDataInputTypeIds,
  1, scmDataOutputNames, scmDataOutputTypeIds,
  0, nullptr,
  0, nullptr
};


FORTE_F_LINT_TO_DWORD::FORTE_F_LINT_TO_DWORD(CStringDictionary::TStringId paInstanceNameId, CResource *paSrcRes) :
    CSimpleFB(paSrcRes, &scmFBInterfaceSpec, paInstanceNameId, nullptr),
    var_IN(CIEC_LINT(0)),
    var_OUT(CIEC_DWORD(0)),
    var_conn_OUT(var_OUT),
    conn_CNF(this, 0),
    conn_IN(nullptr),
    conn_OUT(this, 0, &var_conn_OUT) {
}

void FORTE_F_LINT_TO_DWORD::alg_REQ(void) {
  
  var_OUT = func_LINT_TO_DWORD(var_IN);
}


void FORTE_F_LINT_TO_DWORD::executeEvent(TEventID paEIID){
  switch(paEIID) {
    case scmEventREQID:
      alg_REQ();
      break;
    default:
      break;
  }
  sendOutputEvent(scmEventCNFID);
}

void FORTE_F_LINT_TO_DWORD::readInputData(TEventID paEIID) {
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

void FORTE_F_LINT_TO_DWORD::writeOutputData(TEventID paEIID) {
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

CIEC_ANY *FORTE_F_LINT_TO_DWORD::getDI(size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_IN;
  }
  return nullptr;
}

CIEC_ANY *FORTE_F_LINT_TO_DWORD::getDO(size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_OUT;
  }
  return nullptr;
}

CEventConnection *FORTE_F_LINT_TO_DWORD::getEOConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_F_LINT_TO_DWORD::getDIConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_IN;
  }
  return nullptr;
}

CDataConnection *FORTE_F_LINT_TO_DWORD::getDOConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_OUT;
  }
  return nullptr;
}

CIEC_ANY *FORTE_F_LINT_TO_DWORD::getVarInternal(size_t) {
  return nullptr;
}


