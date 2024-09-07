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

#include "F_MIN_fbt.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "F_MIN_fbt_gen.cpp"
#endif

#include "criticalregion.h"
#include "resource.h"

DEFINE_FIRMWARE_FB(FORTE_F_MIN, g_nStringIdF_MIN)

const CStringDictionary::TStringId FORTE_F_MIN::scmDataInputNames[] = {g_nStringIdIN1, g_nStringIdIN2};

const CStringDictionary::TStringId FORTE_F_MIN::scmDataInputTypeIds[] = {g_nStringIdANY_ELEMENTARY, g_nStringIdANY_ELEMENTARY};

const CStringDictionary::TStringId FORTE_F_MIN::scmDataOutputNames[] = {g_nStringIdOUT};

const CStringDictionary::TStringId FORTE_F_MIN::scmDataOutputTypeIds[] = {g_nStringIdANY_ELEMENTARY};

const TDataIOID FORTE_F_MIN::scmEIWith[] = {0, 1, scmWithListDelimiter};
const TForteInt16 FORTE_F_MIN::scmEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_F_MIN::scmEventInputNames[] = {g_nStringIdREQ};

const TDataIOID FORTE_F_MIN::scmEOWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_F_MIN::scmEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_F_MIN::scmEventOutputNames[] = {g_nStringIdCNF};


const SFBInterfaceSpec FORTE_F_MIN::scmFBInterfaceSpec = {
  1, scmEventInputNames, scmEIWith, scmEIWithIndexes,
  1, scmEventOutputNames, scmEOWith, scmEOWithIndexes,
  2, scmDataInputNames, scmDataInputTypeIds,
  1, scmDataOutputNames, scmDataOutputTypeIds,
  0, nullptr,
  0, nullptr
};

FORTE_F_MIN::FORTE_F_MIN(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    CFunctionBlock(paContainer, scmFBInterfaceSpec, paInstanceNameId),
    var_IN1(CIEC_ANY_ELEMENTARY_VARIANT()),
    var_IN2(CIEC_ANY_ELEMENTARY_VARIANT()),
    var_OUT(CIEC_ANY_ELEMENTARY_VARIANT()),
    var_conn_OUT(var_OUT),
    conn_CNF(this, 0),
    conn_IN1(nullptr),
    conn_IN2(nullptr),
    conn_OUT(this, 0, &var_conn_OUT) {
};

void FORTE_F_MIN::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
  switch(paEIID) {
    case scmEventREQID:
      var_OUT = var_IN1 <= var_IN2 ? var_IN1 : var_IN2;
      sendOutputEvent(scmEventCNFID, paECET);
      break;
  }
}

void FORTE_F_MIN::readInputData(TEventID paEIID) {
  switch(paEIID) {
    case scmEventREQID: {
      readData(0, var_IN1, conn_IN1);
      readData(1, var_IN2, conn_IN2);
      break;
    }
    default:
      break;
  }
}

void FORTE_F_MIN::writeOutputData(TEventID paEIID) {
  switch(paEIID) {
    case scmEventCNFID: {
      writeData(0, var_OUT, conn_OUT);
      break;
    }
    default:
      break;
  }
}

CIEC_ANY *FORTE_F_MIN::getDI(size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_IN1;
    case 1: return &var_IN2;
  }
  return nullptr;
}

CIEC_ANY *FORTE_F_MIN::getDO(size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_OUT;
  }
  return nullptr;
}

CEventConnection *FORTE_F_MIN::getEOConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_F_MIN::getDIConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_IN1;
    case 1: return &conn_IN2;
  }
  return nullptr;
}

CDataConnection *FORTE_F_MIN::getDOConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_OUT;
  }
  return nullptr;
}


