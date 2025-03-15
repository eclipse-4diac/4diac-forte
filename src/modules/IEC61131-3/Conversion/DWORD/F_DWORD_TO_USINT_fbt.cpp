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

#include "F_DWORD_TO_USINT_fbt.h"

USE_STRING_ID(CNF);
USE_STRING_ID(DWORD);
USE_STRING_ID(Event);
USE_STRING_ID(F_DWORD_TO_USINT);
USE_STRING_ID(IN);
USE_STRING_ID(OUT);
USE_STRING_ID(REQ);
USE_STRING_ID(USINT);


#include "criticalregion.h"
#include "resource.h"
#include "forte_usint.h"
#include "forte_dword.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"

DEFINE_FIRMWARE_FB(FORTE_F_DWORD_TO_USINT, STRID(F_DWORD_TO_USINT))

const CStringDictionary::TStringId FORTE_F_DWORD_TO_USINT::scmDataInputNames[] = {STRID(IN)};

const CStringDictionary::TStringId FORTE_F_DWORD_TO_USINT::scmDataInputTypeIds[] = {STRID(DWORD)};

const CStringDictionary::TStringId FORTE_F_DWORD_TO_USINT::scmDataOutputNames[] = {STRID(OUT)};

const CStringDictionary::TStringId FORTE_F_DWORD_TO_USINT::scmDataOutputTypeIds[] = {STRID(USINT)};

const TDataIOID FORTE_F_DWORD_TO_USINT::scmEIWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_F_DWORD_TO_USINT::scmEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_F_DWORD_TO_USINT::scmEventInputNames[] = {STRID(REQ)};
const CStringDictionary::TStringId FORTE_F_DWORD_TO_USINT::scmEventInputTypeIds[] = {STRID(Event)};

const TDataIOID FORTE_F_DWORD_TO_USINT::scmEOWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_F_DWORD_TO_USINT::scmEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_F_DWORD_TO_USINT::scmEventOutputNames[] = {STRID(CNF)};
const CStringDictionary::TStringId FORTE_F_DWORD_TO_USINT::scmEventOutputTypeIds[] = {STRID(Event)};


const SFBInterfaceSpec FORTE_F_DWORD_TO_USINT::scmFBInterfaceSpec = {
  1, scmEventInputNames, scmEventInputTypeIds, scmEIWith, scmEIWithIndexes,
  1, scmEventOutputNames, scmEventOutputTypeIds, scmEOWith, scmEOWithIndexes,
  1, scmDataInputNames, scmDataInputTypeIds,
  1, scmDataOutputNames, scmDataOutputTypeIds,
  0, nullptr,
  0, nullptr
};


FORTE_F_DWORD_TO_USINT::FORTE_F_DWORD_TO_USINT(CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    CSimpleFB(paContainer, scmFBInterfaceSpec, paInstanceNameId, nullptr),
    var_IN(CIEC_DWORD(0)),
    var_OUT(CIEC_USINT(0)),
    var_conn_OUT(var_OUT),
    conn_CNF(this, 0),
    conn_IN(nullptr),
    conn_OUT(this, 0, &var_conn_OUT) {
}

void FORTE_F_DWORD_TO_USINT::alg_REQ(void) {
  
  var_OUT = func_DWORD_TO_USINT(var_IN);
}


void FORTE_F_DWORD_TO_USINT::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
  switch(paEIID) {
    case scmEventREQID:
      alg_REQ();
      break;
    default:
      break;
  }
  sendOutputEvent(scmEventCNFID, paECET);
}

void FORTE_F_DWORD_TO_USINT::readInputData(TEventID paEIID) {
  switch(paEIID) {
    case scmEventREQID: {
      readData(0, var_IN, conn_IN);
      break;
    }
    default:
      break;
  }
}

void FORTE_F_DWORD_TO_USINT::writeOutputData(TEventID paEIID) {
  switch(paEIID) {
    case scmEventCNFID: {
      writeData(0, var_OUT, conn_OUT);
      break;
    }
    default:
      break;
  }
}

CIEC_ANY *FORTE_F_DWORD_TO_USINT::getDI(size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_IN;
  }
  return nullptr;
}

CIEC_ANY *FORTE_F_DWORD_TO_USINT::getDO(size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_OUT;
  }
  return nullptr;
}

CEventConnection *FORTE_F_DWORD_TO_USINT::getEOConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_F_DWORD_TO_USINT::getDIConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_IN;
  }
  return nullptr;
}

CDataConnection *FORTE_F_DWORD_TO_USINT::getDOConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_OUT;
  }
  return nullptr;
}

CIEC_ANY *FORTE_F_DWORD_TO_USINT::getVarInternal(size_t) {
  return nullptr;
}


