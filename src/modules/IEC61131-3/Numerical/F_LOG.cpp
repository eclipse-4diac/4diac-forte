/*******************************************************************************
 * Copyright (c) 2013 ACIN, fortiss GmbH
 *               2023 Martin Erich Jobst
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Monika Wenger, Alois Zoitl
 *   - initial API and implementation and/or initial documentation
 *   Martin Jobst
 *     - refactor for ANY variant
 *******************************************************************************/

#include "F_LOG.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "F_LOG_gen.cpp"
#endif

#include "criticalregion.h"
#include "resource.h"

DEFINE_FIRMWARE_FB(FORTE_F_LOG, g_nStringIdF_LOG)

const CStringDictionary::TStringId FORTE_F_LOG::scmDataInputNames[] = {g_nStringIdIN};

const CStringDictionary::TStringId FORTE_F_LOG::scmDataInputTypeIds[] = {g_nStringIdANY_REAL};

const CStringDictionary::TStringId FORTE_F_LOG::scmDataOutputNames[] = {g_nStringIdOUT};

const CStringDictionary::TStringId FORTE_F_LOG::scmDataOutputTypeIds[] = {g_nStringIdANY_REAL};

const TDataIOID FORTE_F_LOG::scmEIWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_F_LOG::scmEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_F_LOG::scmEventInputNames[] = {g_nStringIdREQ};

const TDataIOID FORTE_F_LOG::scmEOWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_F_LOG::scmEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_F_LOG::scmEventOutputNames[] = {g_nStringIdCNF};


const SFBInterfaceSpec FORTE_F_LOG::scmFBInterfaceSpec = {
  1, scmEventInputNames, scmEIWith, scmEIWithIndexes,
  1, scmEventOutputNames, scmEOWith, scmEOWithIndexes,
  1, scmDataInputNames, scmDataInputTypeIds,
  1, scmDataOutputNames, scmDataOutputTypeIds,
  0, nullptr,
  0, nullptr
};

FORTE_F_LOG::FORTE_F_LOG(const CStringDictionary::TStringId paInstanceNameId, CResource *paSrcRes) :
    CFunctionBlock( paSrcRes, &scmFBInterfaceSpec, paInstanceNameId),
    var_IN(CIEC_ANY_REAL_VARIANT()),
    var_OUT(CIEC_ANY_REAL_VARIANT()),
    var_conn_OUT(var_OUT),
    conn_CNF(this, 0),
    conn_IN(nullptr),
    conn_OUT(this, 0, &var_conn_OUT) {
};

void FORTE_F_LOG::executeEvent(TEventID paEIID) {
  switch(paEIID) {
    case scmEventREQID:
      var_OUT = std::visit([](auto &&paIN) -> CIEC_ANY_REAL_VARIANT {
          return func_LOG(paIN);
      }, var_IN);
      sendOutputEvent(scmEventCNFID);
      break;
  }
}

void FORTE_F_LOG::readInputData(TEventID paEIID) {
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

void FORTE_F_LOG::writeOutputData(TEventID paEIID) {
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

CIEC_ANY *FORTE_F_LOG::getDI(size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_IN;
  }
  return nullptr;
}

CIEC_ANY *FORTE_F_LOG::getDO(size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_OUT;
  }
  return nullptr;
}

CEventConnection *FORTE_F_LOG::getEOConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_F_LOG::getDIConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_IN;
  }
  return nullptr;
}

CDataConnection *FORTE_F_LOG::getDOConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_OUT;
  }
  return nullptr;
}


