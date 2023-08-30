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

#include "F_TRUNC.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "F_TRUNC_gen.cpp"
#endif

#include "criticalregion.h"
#include "resource.h"

DEFINE_FIRMWARE_FB(FORTE_F_TRUNC, g_nStringIdF_TRUNC)

const CStringDictionary::TStringId FORTE_F_TRUNC::scmDataInputNames[] = {g_nStringIdIN};

const CStringDictionary::TStringId FORTE_F_TRUNC::scmDataInputTypeIds[] = {g_nStringIdANY_REAL};

const CStringDictionary::TStringId FORTE_F_TRUNC::scmDataOutputNames[] = {g_nStringIdOUT};

const CStringDictionary::TStringId FORTE_F_TRUNC::scmDataOutputTypeIds[] = {g_nStringIdANY_INT};

const TDataIOID FORTE_F_TRUNC::scmEIWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_F_TRUNC::scmEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_F_TRUNC::scmEventInputNames[] = {g_nStringIdREQ};

const TDataIOID FORTE_F_TRUNC::scmEOWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_F_TRUNC::scmEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_F_TRUNC::scmEventOutputNames[] = {g_nStringIdCNF};


const SFBInterfaceSpec FORTE_F_TRUNC::scmFBInterfaceSpec = {
  1, scmEventInputNames, scmEIWith, scmEIWithIndexes,
  1, scmEventOutputNames, scmEOWith, scmEOWithIndexes,
  1, scmDataInputNames, scmDataInputTypeIds,
  1, scmDataOutputNames, scmDataOutputTypeIds,
  0, nullptr,
  0, nullptr
};

FORTE_F_TRUNC::FORTE_F_TRUNC(const CStringDictionary::TStringId paInstanceNameId, CResource *paSrcRes) :
    CFunctionBlock( paSrcRes, &scmFBInterfaceSpec, paInstanceNameId),
    var_IN(CIEC_ANY_REAL_VARIANT()),
    var_OUT(CIEC_ANY_INT_VARIANT()),
    var_conn_OUT(var_OUT),
    conn_CNF(this, 0),
    conn_IN(nullptr),
    conn_OUT(this, 0, &var_conn_OUT) {
};

void FORTE_F_TRUNC::executeEvent(TEventID paEIID) {
  switch(paEIID) {
    case scmEventREQID:
      std::visit([](auto &&paIN, auto &&paOUT) -> void {
        using T = std::decay_t<decltype(paOUT)>;
        paOUT = func_TRUNC<T>(paIN);
      }, var_IN, var_OUT);
      sendOutputEvent(scmEventCNFID);
      break;
  }
}

void FORTE_F_TRUNC::readInputData(TEventID paEIID) {
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

void FORTE_F_TRUNC::writeOutputData(TEventID paEIID) {
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

CIEC_ANY *FORTE_F_TRUNC::getDI(size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_IN;
  }
  return nullptr;
}

CIEC_ANY *FORTE_F_TRUNC::getDO(size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_OUT;
  }
  return nullptr;
}

CEventConnection *FORTE_F_TRUNC::getEOConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_F_TRUNC::getDIConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_IN;
  }
  return nullptr;
}

CDataConnection *FORTE_F_TRUNC::getDOConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_OUT;
  }
  return nullptr;
}


