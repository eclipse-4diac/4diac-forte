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

#include "F_LEFT.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "F_LEFT_gen.cpp"
#endif

#include "criticalregion.h"
#include "resource.h"

DEFINE_FIRMWARE_FB(FORTE_F_LEFT, g_nStringIdF_LEFT)

const CStringDictionary::TStringId FORTE_F_LEFT::scm_anDataInputNames[] = {g_nStringIdIN, g_nStringIdL};

const CStringDictionary::TStringId FORTE_F_LEFT::scm_anDataInputTypeIds[] = {g_nStringIdANY_STRING, g_nStringIdANY_INT};

const CStringDictionary::TStringId FORTE_F_LEFT::scm_anDataOutputNames[] = {g_nStringIdOUT};

const CStringDictionary::TStringId FORTE_F_LEFT::scm_anDataOutputTypeIds[] = {g_nStringIdANY_STRING};

const TDataIOID FORTE_F_LEFT::scm_anEIWith[] = {0, 1, scmWithListDelimiter};
const TForteInt16 FORTE_F_LEFT::scm_anEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_F_LEFT::scm_anEventInputNames[] = {g_nStringIdREQ};

const TDataIOID FORTE_F_LEFT::scm_anEOWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_F_LEFT::scm_anEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_F_LEFT::scm_anEventOutputNames[] = {g_nStringIdCNF};


const SFBInterfaceSpec FORTE_F_LEFT::scm_stFBInterfaceSpec = {
  1, scm_anEventInputNames, scm_anEIWith, scm_anEIWithIndexes,
  1, scm_anEventOutputNames, scm_anEOWith, scm_anEOWithIndexes,
  2, scm_anDataInputNames, scm_anDataInputTypeIds,
  1, scm_anDataOutputNames, scm_anDataOutputTypeIds,
  0, nullptr
};

FORTE_F_LEFT::FORTE_F_LEFT(const CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes) :
    CFunctionBlock( pa_poSrcRes, &scm_stFBInterfaceSpec, pa_nInstanceNameId),
    var_IN(CIEC_ANY_STRING_VARIANT()),
    var_L(CIEC_ANY_INT_VARIANT()),
    var_OUT(CIEC_ANY_STRING_VARIANT()),
    var_conn_OUT(var_OUT),
    conn_CNF(this, 0),
    conn_IN(nullptr),
    conn_L(nullptr),
    conn_OUT(this, 0, &var_conn_OUT) {
};

void FORTE_F_LEFT::executeEvent(TEventID pa_nEIID) {
  switch(pa_nEIID) {
    case scm_nEventREQID:
      var_OUT = std::visit([](auto &&paIN, auto&&paL) -> CIEC_ANY_STRING_VARIANT {
          return func_LEFT(paIN, paL);
      }, var_IN, var_L);
      sendOutputEvent(scm_nEventCNFID);
      break;
  }
}

void FORTE_F_LEFT::readInputData(TEventID pa_nEIID) {
  switch(pa_nEIID) {
    case scm_nEventREQID: {
      RES_DATA_CON_CRITICAL_REGION();
      readData(0, &var_IN, conn_IN);
      readData(1, &var_L, conn_L);
      break;
    }
    default:
      break;
  }
}

void FORTE_F_LEFT::writeOutputData(TEventID pa_nEIID) {
  switch(pa_nEIID) {
    case scm_nEventCNFID: {
      RES_DATA_CON_CRITICAL_REGION();
      writeData(0, &var_OUT, &conn_OUT);
      break;
    }
    default:
      break;
  }
}

CIEC_ANY *FORTE_F_LEFT::getDI(size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_IN;
    case 1: return &var_L;
  }
  return nullptr;
}

CIEC_ANY *FORTE_F_LEFT::getDO(size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_OUT;
  }
  return nullptr;
}

CEventConnection *FORTE_F_LEFT::getEOConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_F_LEFT::getDIConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_IN;
    case 1: return &conn_L;
  }
  return nullptr;
}

CDataConnection *FORTE_F_LEFT::getDOConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_OUT;
  }
  return nullptr;
}


