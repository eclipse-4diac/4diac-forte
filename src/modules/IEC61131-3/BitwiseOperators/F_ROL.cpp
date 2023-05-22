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

#include "F_ROL.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "F_ROL_gen.cpp"
#endif

#include "criticalregion.h"
#include "resource.h"

DEFINE_FIRMWARE_FB(FORTE_F_ROL, g_nStringIdF_ROL)

const CStringDictionary::TStringId FORTE_F_ROL::scm_anDataInputNames[] = {g_nStringIdIN, g_nStringIdN};

const CStringDictionary::TStringId FORTE_F_ROL::scm_anDataInputTypeIds[] = {g_nStringIdANY_BIT, g_nStringIdANY_INT};

const CStringDictionary::TStringId FORTE_F_ROL::scm_anDataOutputNames[] = {g_nStringIdOUT};

const CStringDictionary::TStringId FORTE_F_ROL::scm_anDataOutputTypeIds[] = {g_nStringIdANY_BIT};

const TDataIOID FORTE_F_ROL::scm_anEIWith[] = {0, 1, scmWithListDelimiter};
const TForteInt16 FORTE_F_ROL::scm_anEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_F_ROL::scm_anEventInputNames[] = {g_nStringIdREQ};

const TDataIOID FORTE_F_ROL::scm_anEOWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_F_ROL::scm_anEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_F_ROL::scm_anEventOutputNames[] = {g_nStringIdCNF};


const SFBInterfaceSpec FORTE_F_ROL::scm_stFBInterfaceSpec = {
  1, scm_anEventInputNames, scm_anEIWith, scm_anEIWithIndexes,
  1, scm_anEventOutputNames, scm_anEOWith, scm_anEOWithIndexes,
  2, scm_anDataInputNames, scm_anDataInputTypeIds,
  1, scm_anDataOutputNames, scm_anDataOutputTypeIds,
  0, nullptr
};

FORTE_F_ROL::FORTE_F_ROL(const CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes) :
    CFunctionBlock( pa_poSrcRes, &scm_stFBInterfaceSpec, pa_nInstanceNameId),
    var_IN(CIEC_ANY_BIT_VARIANT()),
    var_N(CIEC_ANY_INT_VARIANT()),
    var_OUT(CIEC_ANY_BIT_VARIANT()),
    var_conn_OUT(var_OUT),
    conn_CNF(this, 0),
    conn_IN(nullptr),
    conn_N(nullptr),
    conn_OUT(this, 0, &var_conn_OUT) {
};

void FORTE_F_ROL::executeEvent(TEventID pa_nEIID) {
  switch(pa_nEIID) {
    case scm_nEventREQID:
      var_OUT = std::visit([](auto &&paIN, auto&&paN) -> CIEC_ANY_BIT_VARIANT {
          using T = std::decay_t<decltype(paIN)>;
          if constexpr (!std::is_same<T, CIEC_BOOL>::value) {
            return func_ROL(paIN, paN);
          }
          DEVLOG_ERROR("Rotating left incompatible types %s and %s\n",
                       CStringDictionary::getInstance().get(paIN.getTypeNameID()),
                       CStringDictionary::getInstance().get(paN.getTypeNameID()));
          return CIEC_ANY_BIT_VARIANT();
      }, var_IN, var_N);
      sendOutputEvent(scm_nEventCNFID);
      break;
  }
}

void FORTE_F_ROL::readInputData(TEventID pa_nEIID) {
  switch(pa_nEIID) {
    case scm_nEventREQID: {
      RES_DATA_CON_CRITICAL_REGION();
      readData(0, &var_IN, conn_IN);
      readData(1, &var_N, conn_N);
      break;
    }
    default:
      break;
  }
}

void FORTE_F_ROL::writeOutputData(TEventID pa_nEIID) {
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

CIEC_ANY *FORTE_F_ROL::getDI(size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_IN;
    case 1: return &var_N;
  }
  return nullptr;
}

CIEC_ANY *FORTE_F_ROL::getDO(size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_OUT;
  }
  return nullptr;
}

CEventConnection *FORTE_F_ROL::getEOConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_F_ROL::getDIConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_IN;
    case 1: return &conn_N;
  }
  return nullptr;
}

CDataConnection *FORTE_F_ROL::getDOConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_OUT;
  }
  return nullptr;
}


