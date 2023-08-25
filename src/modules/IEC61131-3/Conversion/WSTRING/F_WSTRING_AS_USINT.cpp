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

#include "F_WSTRING_AS_USINT.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "F_WSTRING_AS_USINT_gen.cpp"
#endif

#include "criticalregion.h"
#include "resource.h"
#include "forte_usint.h"
#include "forte_wstring.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"

DEFINE_FIRMWARE_FB(FORTE_F_WSTRING_AS_USINT, g_nStringIdF_WSTRING_AS_USINT)

const CStringDictionary::TStringId FORTE_F_WSTRING_AS_USINT::scmDataInputNames[] = {g_nStringIdIN};

const CStringDictionary::TStringId FORTE_F_WSTRING_AS_USINT::scmDataInputTypeIds[] = {g_nStringIdWSTRING};

const CStringDictionary::TStringId FORTE_F_WSTRING_AS_USINT::scmDataOutputNames[] = {g_nStringIdOUT};

const CStringDictionary::TStringId FORTE_F_WSTRING_AS_USINT::scmDataOutputTypeIds[] = {g_nStringIdUSINT};

const TDataIOID FORTE_F_WSTRING_AS_USINT::scmEIWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_F_WSTRING_AS_USINT::scmEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_F_WSTRING_AS_USINT::scmEventInputNames[] = {g_nStringIdREQ};

const TDataIOID FORTE_F_WSTRING_AS_USINT::scmEOWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_F_WSTRING_AS_USINT::scmEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_F_WSTRING_AS_USINT::scmEventOutputNames[] = {g_nStringIdCNF};


const SFBInterfaceSpec FORTE_F_WSTRING_AS_USINT::scmFBInterfaceSpec = {
  1, scmEventInputNames, scmEIWith, scmEIWithIndexes,
  1, scmEventOutputNames, scmEOWith, scmEOWithIndexes,
  1, scmDataInputNames, scmDataInputTypeIds,
  1, scmDataOutputNames, scmDataOutputTypeIds,
  0, nullptr,
  0, nullptr
};


FORTE_F_WSTRING_AS_USINT::FORTE_F_WSTRING_AS_USINT(CStringDictionary::TStringId paInstanceNameId, CResource *paSrcRes) :
    CSimpleFB(paSrcRes, &scmFBInterfaceSpec, paInstanceNameId, nullptr),
    var_IN(CIEC_WSTRING("")),
    var_OUT(CIEC_USINT(0)),
    var_conn_OUT(var_OUT),
    conn_CNF(this, 0),
    conn_IN(nullptr),
    conn_OUT(this, 0, &var_conn_OUT) {
}

void FORTE_F_WSTRING_AS_USINT::alg_REQ(void) {
  
  var_OUT = func_WSTRING_AS_USINT(var_IN);
}


void FORTE_F_WSTRING_AS_USINT::executeEvent(TEventID paEIID){
  switch(paEIID) {
    case scmEventREQID:
      alg_REQ();
      break;
    default:
      break;
  }
  sendOutputEvent(scmEventCNFID);
}

void FORTE_F_WSTRING_AS_USINT::readInputData(TEventID paEIID) {
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

void FORTE_F_WSTRING_AS_USINT::writeOutputData(TEventID paEIID) {
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

CIEC_ANY *FORTE_F_WSTRING_AS_USINT::getDI(size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_IN;
  }
  return nullptr;
}

CIEC_ANY *FORTE_F_WSTRING_AS_USINT::getDO(size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_OUT;
  }
  return nullptr;
}

CEventConnection *FORTE_F_WSTRING_AS_USINT::getEOConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_F_WSTRING_AS_USINT::getDIConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_IN;
  }
  return nullptr;
}

CDataConnection *FORTE_F_WSTRING_AS_USINT::getDOConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_OUT;
  }
  return nullptr;
}

CIEC_ANY *FORTE_F_WSTRING_AS_USINT::getVarInternal(size_t) {
  return nullptr;
}


