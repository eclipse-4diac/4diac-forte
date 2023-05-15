/*******************************************************************************
 * Copyright (c) 2014 Profactor GmbH
 *               2023 Martin Erich Jobst
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Matthias Plasch
 *   - initial API and implementation and/or initial documentation
 *   Martin Jobst
 *     - refactor for ANY variant
 *******************************************************************************/

#include "GET_AT_INDEX.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "GET_AT_INDEX_gen.cpp"
#endif

#include "criticalregion.h"
#include "resource.h"

DEFINE_FIRMWARE_FB(FORTE_GET_AT_INDEX, g_nStringIdGET_AT_INDEX)

const CStringDictionary::TStringId FORTE_GET_AT_INDEX::scm_anDataInputNames[] = {g_nStringIdIN_ARRAY, g_nStringIdINDEX};

const CStringDictionary::TStringId FORTE_GET_AT_INDEX::scm_anDataInputTypeIds[] = {g_nStringIdANY, g_nStringIdUINT};

const CStringDictionary::TStringId FORTE_GET_AT_INDEX::scm_anDataOutputNames[] = {g_nStringIdQO, g_nStringIdOUT};

const CStringDictionary::TStringId FORTE_GET_AT_INDEX::scm_anDataOutputTypeIds[] = {g_nStringIdBOOL, g_nStringIdANY};

const TDataIOID FORTE_GET_AT_INDEX::scm_anEIWith[] = {0, 1, 255};
const TForteInt16 FORTE_GET_AT_INDEX::scm_anEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_GET_AT_INDEX::scm_anEventInputNames[] = {g_nStringIdREQ};

const TDataIOID FORTE_GET_AT_INDEX::scm_anEOWith[] = {0, 1, 255};
const TForteInt16 FORTE_GET_AT_INDEX::scm_anEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_GET_AT_INDEX::scm_anEventOutputNames[] = {g_nStringIdCNF};


const SFBInterfaceSpec FORTE_GET_AT_INDEX::scm_stFBInterfaceSpec = {
  1, scm_anEventInputNames, scm_anEIWith, scm_anEIWithIndexes,
  1, scm_anEventOutputNames, scm_anEOWith, scm_anEOWithIndexes,
  2, scm_anDataInputNames, scm_anDataInputTypeIds,
  2, scm_anDataOutputNames, scm_anDataOutputTypeIds,
  0, nullptr
};

FORTE_GET_AT_INDEX::FORTE_GET_AT_INDEX(const CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes) :
    CFunctionBlock( pa_poSrcRes, &scm_stFBInterfaceSpec, pa_nInstanceNameId),
    var_IN_ARRAY(CIEC_ANY_VARIANT()),
    var_INDEX(CIEC_UINT(0)),
    var_QO(CIEC_BOOL(0)),
    var_OUT(CIEC_ANY_VARIANT()),
    var_conn_QO(var_QO),
    var_conn_OUT(var_OUT),
    conn_CNF(this, 0),
    conn_IN_ARRAY(nullptr),
    conn_INDEX(nullptr),
    conn_QO(this, 0, &var_conn_QO),
    conn_OUT(this, 1, &var_conn_OUT) {
};

void FORTE_GET_AT_INDEX::executeEvent(TEventID pa_nEIID) {
  switch(pa_nEIID) {
    case scm_nEventREQID:
      if (std::holds_alternative<CIEC_ANY_UNIQUE_PTR<CIEC_ARRAY>>(var_IN_ARRAY)) {
        auto &inArray = std::get<CIEC_ANY_UNIQUE_PTR<CIEC_ARRAY>>(var_IN_ARRAY);
        // check if index is within bounds
        if (static_cast<CIEC_UINT::TValueType>(var_INDEX) >= inArray->getLowerBound()
            && static_cast<CIEC_UINT::TValueType>(var_INDEX) <= inArray->getUpperBound()) {
          var_OUT.setValue(inArray->at(var_INDEX));
          var_QO = CIEC_BOOL(true);
        } else {
          var_QO = CIEC_BOOL(false);
        }
      } else {
        var_QO = CIEC_BOOL(false);
      }
      sendOutputEvent(scm_nEventCNFID);
      break;
  }
}

void FORTE_GET_AT_INDEX::readInputData(TEventID pa_nEIID) {
  switch(pa_nEIID) {
    case scm_nEventREQID: {
      RES_DATA_CON_CRITICAL_REGION();
      readData(0, &var_IN_ARRAY, conn_IN_ARRAY);
      readData(1, &var_INDEX, conn_INDEX);
      break;
    }
    default:
      break;
  }
}

void FORTE_GET_AT_INDEX::writeOutputData(TEventID pa_nEIID) {
  switch(pa_nEIID) {
    case scm_nEventCNFID: {
      RES_DATA_CON_CRITICAL_REGION();
      writeData(0, &var_QO, &conn_QO);
      writeData(1, &var_OUT, &conn_OUT);
      break;
    }
    default:
      break;
  }
}

CIEC_ANY *FORTE_GET_AT_INDEX::getDI(size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_IN_ARRAY;
    case 1: return &var_INDEX;
  }
  return nullptr;
}

CIEC_ANY *FORTE_GET_AT_INDEX::getDO(size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_QO;
    case 1: return &var_OUT;
  }
  return nullptr;
}

CEventConnection *FORTE_GET_AT_INDEX::getEOConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_GET_AT_INDEX::getDIConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_IN_ARRAY;
    case 1: return &conn_INDEX;
  }
  return nullptr;
}

CDataConnection *FORTE_GET_AT_INDEX::getDOConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_QO;
    case 1: return &conn_OUT;
  }
  return nullptr;
}


