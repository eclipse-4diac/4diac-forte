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

#include "F_ABS.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "F_ABS_gen.cpp"
#endif

#include "criticalregion.h"
#include "resource.h"

DEFINE_FIRMWARE_FB(FORTE_F_ABS, g_nStringIdF_ABS)

const CStringDictionary::TStringId FORTE_F_ABS::scmDataInputNames[] = {g_nStringIdIN};

const CStringDictionary::TStringId FORTE_F_ABS::scmDataInputTypeIds[] = {g_nStringIdANY_NUM};

const CStringDictionary::TStringId FORTE_F_ABS::scmDataOutputNames[] = {g_nStringIdOUT};

const CStringDictionary::TStringId FORTE_F_ABS::scmDataOutputTypeIds[] = {g_nStringIdANY_NUM};

const TDataIOID FORTE_F_ABS::scmEIWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_F_ABS::scmEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_F_ABS::scmEventInputNames[] = {g_nStringIdREQ};

const TDataIOID FORTE_F_ABS::scmEOWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_F_ABS::scmEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_F_ABS::scmEventOutputNames[] = {g_nStringIdCNF};


const SFBInterfaceSpec FORTE_F_ABS::scmFBInterfaceSpec = {
  1, scmEventInputNames, scmEIWith, scmEIWithIndexes,
  1, scmEventOutputNames, scmEOWith, scmEOWithIndexes,
  1, scmDataInputNames, scmDataInputTypeIds,
  1, scmDataOutputNames, scmDataOutputTypeIds,
  0, nullptr,
  0, nullptr
};

FORTE_F_ABS::FORTE_F_ABS(const CStringDictionary::TStringId paInstanceNameId, CResource *paSrcRes) :
    CFunctionBlock( paSrcRes, &scmFBInterfaceSpec, paInstanceNameId),
    var_IN(CIEC_ANY_NUM_VARIANT()),
    var_OUT(CIEC_ANY_NUM_VARIANT()),
    var_conn_OUT(var_OUT),
    conn_CNF(this, 0),
    conn_IN(nullptr),
    conn_OUT(this, 0, &var_conn_OUT) {
};

void FORTE_F_ABS::executeEvent(TEventID paEIID) {
  switch(paEIID) {
    case scmEventREQID:
      var_OUT = std::visit([](auto &&paIN) -> CIEC_ANY_NUM_VARIANT {
        return func_ABS(paIN);
      }, var_IN);
      sendOutputEvent(scmEventCNFID);
      break;
  }
}

void FORTE_F_ABS::readInputData(TEventID paEIID) {
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

void FORTE_F_ABS::writeOutputData(TEventID paEIID) {
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

CIEC_ANY *FORTE_F_ABS::getDI(size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_IN;
  }
  return nullptr;
}

CIEC_ANY *FORTE_F_ABS::getDO(size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_OUT;
  }
  return nullptr;
}

CEventConnection *FORTE_F_ABS::getEOConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_F_ABS::getDIConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_IN;
  }
  return nullptr;
}

CDataConnection *FORTE_F_ABS::getDOConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_OUT;
  }
  return nullptr;
}


