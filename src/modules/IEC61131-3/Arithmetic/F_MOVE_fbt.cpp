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

#include "F_MOVE_fbt.h"

USE_STRING_ID(ANY);
USE_STRING_ID(CNF);
USE_STRING_ID(Event);
USE_STRING_ID(F_MOVE);
USE_STRING_ID(IN);
USE_STRING_ID(OUT);
USE_STRING_ID(REQ);


#include "criticalregion.h"
#include "resource.h"

DEFINE_FIRMWARE_FB(FORTE_F_MOVE, STRID(F_MOVE))

const CStringDictionary::TStringId FORTE_F_MOVE::scmDataInputNames[] = {STRID(IN)};

const CStringDictionary::TStringId FORTE_F_MOVE::scmDataInputTypeIds[] = {STRID(ANY)};

const CStringDictionary::TStringId FORTE_F_MOVE::scmDataOutputNames[] = {STRID(OUT)};

const CStringDictionary::TStringId FORTE_F_MOVE::scmDataOutputTypeIds[] = {STRID(ANY)};

const TDataIOID FORTE_F_MOVE::scmEIWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_F_MOVE::scmEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_F_MOVE::scmEventInputNames[] = {STRID(REQ)};
const CStringDictionary::TStringId FORTE_F_MOVE::scmEventInputTypeIds[] = {STRID(Event)};

const TDataIOID FORTE_F_MOVE::scmEOWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_F_MOVE::scmEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_F_MOVE::scmEventOutputNames[] = {STRID(CNF)};
const CStringDictionary::TStringId FORTE_F_MOVE::scmEventOutputTypeIds[] = {STRID(Event)};


const SFBInterfaceSpec FORTE_F_MOVE::scmFBInterfaceSpec = {
  1, scmEventInputNames, scmEventInputTypeIds, scmEIWith, scmEIWithIndexes,
  1, scmEventOutputNames, scmEventOutputTypeIds, scmEOWith, scmEOWithIndexes,
  1, scmDataInputNames, scmDataInputTypeIds,
  1, scmDataOutputNames, scmDataOutputTypeIds,
  0, nullptr,
  0, nullptr
};

FORTE_F_MOVE::FORTE_F_MOVE(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    CFunctionBlock(paContainer, scmFBInterfaceSpec, paInstanceNameId),
    var_IN(CIEC_ANY_VARIANT()),
    var_OUT(CIEC_ANY_VARIANT()),
    var_conn_OUT(var_OUT),
    conn_CNF(this, 0),
    conn_IN(nullptr),
    conn_OUT(this, 0, &var_conn_OUT) {
};

void FORTE_F_MOVE::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
  switch(paEIID) {
    case scmEventREQID:
      var_OUT = var_IN;
      sendOutputEvent(scmEventCNFID, paECET);
      break;
  }
}

void FORTE_F_MOVE::readInputData(TEventID paEIID) {
  switch(paEIID) {
    case scmEventREQID: {
      readData(0, var_IN, conn_IN);
      break;
    }
    default:
      break;
  }
}

void FORTE_F_MOVE::writeOutputData(TEventID paEIID) {
  switch(paEIID) {
    case scmEventCNFID: {
      writeData(0, var_OUT, conn_OUT);
      break;
    }
    default:
      break;
  }
}

CIEC_ANY *FORTE_F_MOVE::getDI(size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_IN;
  }
  return nullptr;
}

CIEC_ANY *FORTE_F_MOVE::getDO(size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_OUT;
  }
  return nullptr;
}

CEventConnection *FORTE_F_MOVE::getEOConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_F_MOVE::getDIConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_IN;
  }
  return nullptr;
}

CDataConnection *FORTE_F_MOVE::getDOConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_OUT;
  }
  return nullptr;
}


