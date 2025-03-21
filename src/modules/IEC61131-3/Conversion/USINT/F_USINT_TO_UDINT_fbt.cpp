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

#include "F_USINT_TO_UDINT_fbt.h"

USE_STRING_ID(CNF);
USE_STRING_ID(Event);
USE_STRING_ID(F_USINT_TO_UDINT);
USE_STRING_ID(IN);
USE_STRING_ID(OUT);
USE_STRING_ID(REQ);
USE_STRING_ID(UDINT);
USE_STRING_ID(USINT);


#include "criticalregion.h"
#include "resource.h"
#include "forte_udint.h"
#include "forte_usint.h"
#include "iec61131_functions.h"
#include "forte_array_common.h"
#include "forte_array.h"
#include "forte_array_fixed.h"
#include "forte_array_variable.h"

DEFINE_FIRMWARE_FB(FORTE_F_USINT_TO_UDINT, STRID(F_USINT_TO_UDINT))

const CStringDictionary::TStringId FORTE_F_USINT_TO_UDINT::scmDataInputNames[] = {STRID(IN)};

const CStringDictionary::TStringId FORTE_F_USINT_TO_UDINT::scmDataInputTypeIds[] = {STRID(USINT)};

const CStringDictionary::TStringId FORTE_F_USINT_TO_UDINT::scmDataOutputNames[] = {STRID(OUT)};

const CStringDictionary::TStringId FORTE_F_USINT_TO_UDINT::scmDataOutputTypeIds[] = {STRID(UDINT)};

const TDataIOID FORTE_F_USINT_TO_UDINT::scmEIWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_F_USINT_TO_UDINT::scmEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_F_USINT_TO_UDINT::scmEventInputNames[] = {STRID(REQ)};
const CStringDictionary::TStringId FORTE_F_USINT_TO_UDINT::scmEventInputTypeIds[] = {STRID(Event)};

const TDataIOID FORTE_F_USINT_TO_UDINT::scmEOWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_F_USINT_TO_UDINT::scmEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_F_USINT_TO_UDINT::scmEventOutputNames[] = {STRID(CNF)};
const CStringDictionary::TStringId FORTE_F_USINT_TO_UDINT::scmEventOutputTypeIds[] = {STRID(Event)};


const SFBInterfaceSpec FORTE_F_USINT_TO_UDINT::scmFBInterfaceSpec = {
  1, scmEventInputNames, scmEventInputTypeIds, scmEIWith, scmEIWithIndexes,
  1, scmEventOutputNames, scmEventOutputTypeIds, scmEOWith, scmEOWithIndexes,
  1, scmDataInputNames, scmDataInputTypeIds,
  1, scmDataOutputNames, scmDataOutputTypeIds,
  0, nullptr,
  0, nullptr
};


FORTE_F_USINT_TO_UDINT::FORTE_F_USINT_TO_UDINT(CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    CSimpleFB(paContainer, scmFBInterfaceSpec, paInstanceNameId, nullptr),
    var_IN(CIEC_USINT(0)),
    var_OUT(CIEC_UDINT(0)),
    conn_CNF(this, 0),
    conn_IN(nullptr),
    conn_OUT(this, 0, var_OUT) {
}

void FORTE_F_USINT_TO_UDINT::alg_REQ(void) {
  
  var_OUT = func_USINT_TO_UDINT(var_IN);
}


void FORTE_F_USINT_TO_UDINT::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
  switch(paEIID) {
    case scmEventREQID:
      alg_REQ();
      break;
    default:
      break;
  }
  sendOutputEvent(scmEventCNFID, paECET);
}

void FORTE_F_USINT_TO_UDINT::readInputData(TEventID paEIID) {
  switch(paEIID) {
    case scmEventREQID: {
      readData(0, var_IN, conn_IN);
      break;
    }
    default:
      break;
  }
}

void FORTE_F_USINT_TO_UDINT::writeOutputData(TEventID paEIID) {
  switch(paEIID) {
    case scmEventCNFID: {
      writeData(0, var_OUT, conn_OUT);
      break;
    }
    default:
      break;
  }
}

CIEC_ANY *FORTE_F_USINT_TO_UDINT::getDI(size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_IN;
  }
  return nullptr;
}

CIEC_ANY *FORTE_F_USINT_TO_UDINT::getDO(size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_OUT;
  }
  return nullptr;
}

CEventConnection *FORTE_F_USINT_TO_UDINT::getEOConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_F_USINT_TO_UDINT::getDIConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_IN;
  }
  return nullptr;
}

CDataConnection *FORTE_F_USINT_TO_UDINT::getDOConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_OUT;
  }
  return nullptr;
}

CIEC_ANY *FORTE_F_USINT_TO_UDINT::getVarInternal(size_t) {
  return nullptr;
}


