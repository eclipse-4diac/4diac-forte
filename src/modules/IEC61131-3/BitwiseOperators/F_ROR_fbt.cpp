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

#include "F_ROR_fbt.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "F_ROR_fbt_gen.cpp"
#endif

#include "criticalregion.h"
#include "resource.h"

DEFINE_FIRMWARE_FB(FORTE_F_ROR, g_nStringIdF_ROR)

const CStringDictionary::TStringId FORTE_F_ROR::scmDataInputNames[] = {g_nStringIdIN, g_nStringIdN};

const CStringDictionary::TStringId FORTE_F_ROR::scmDataInputTypeIds[] = {g_nStringIdANY_BIT, g_nStringIdANY_INT};

const CStringDictionary::TStringId FORTE_F_ROR::scmDataOutputNames[] = {g_nStringIdOUT};

const CStringDictionary::TStringId FORTE_F_ROR::scmDataOutputTypeIds[] = {g_nStringIdANY_BIT};

const TDataIOID FORTE_F_ROR::scmEIWith[] = {0, 1, scmWithListDelimiter};
const TForteInt16 FORTE_F_ROR::scmEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_F_ROR::scmEventInputNames[] = {g_nStringIdREQ};

const TDataIOID FORTE_F_ROR::scmEOWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_F_ROR::scmEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_F_ROR::scmEventOutputNames[] = {g_nStringIdCNF};


const SFBInterfaceSpec FORTE_F_ROR::scmFBInterfaceSpec = {
  1, scmEventInputNames, scmEIWith, scmEIWithIndexes,
  1, scmEventOutputNames, scmEOWith, scmEOWithIndexes,
  2, scmDataInputNames, scmDataInputTypeIds,
  1, scmDataOutputNames, scmDataOutputTypeIds,
  0, nullptr,
  0, nullptr
};

FORTE_F_ROR::FORTE_F_ROR(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    CFunctionBlock(paContainer, scmFBInterfaceSpec, paInstanceNameId),
    var_IN(CIEC_ANY_BIT_VARIANT()),
    var_N(CIEC_ANY_INT_VARIANT()),
    var_OUT(CIEC_ANY_BIT_VARIANT()),
    var_conn_OUT(var_OUT),
    conn_CNF(this, 0),
    conn_IN(nullptr),
    conn_N(nullptr),
    conn_OUT(this, 0, &var_conn_OUT) {
};

void FORTE_F_ROR::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
  switch(paEIID) {
    case scmEventREQID:
      var_OUT = std::visit([](auto &&paIN, auto&&paN) -> CIEC_ANY_BIT_VARIANT {
          using T = std::decay_t<decltype(paIN)>;
          if constexpr (!std::is_same<T, CIEC_BOOL>::value) {
            return func_ROR(paIN, paN);
          }
          DEVLOG_ERROR("Rotating right incompatible types %s and %s\n",
                       CStringDictionary::getInstance().get(paIN.getTypeNameID()),
                       CStringDictionary::getInstance().get(paN.getTypeNameID()));
          return CIEC_ANY_BIT_VARIANT();
      }, static_cast<CIEC_ANY_BIT_VARIANT::variant&>(var_IN),
         static_cast<CIEC_ANY_INT_VARIANT::variant&>(var_N));
      sendOutputEvent(scmEventCNFID, paECET);
      break;
  }
}

void FORTE_F_ROR::readInputData(TEventID paEIID) {
  switch(paEIID) {
    case scmEventREQID: {
      readData(0, var_IN, conn_IN);
      readData(1, var_N, conn_N);
      break;
    }
    default:
      break;
  }
}

void FORTE_F_ROR::writeOutputData(TEventID paEIID) {
  switch(paEIID) {
    case scmEventCNFID: {
      writeData(0, var_OUT, conn_OUT);
      break;
    }
    default:
      break;
  }
}

CIEC_ANY *FORTE_F_ROR::getDI(size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_IN;
    case 1: return &var_N;
  }
  return nullptr;
}

CIEC_ANY *FORTE_F_ROR::getDO(size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_OUT;
  }
  return nullptr;
}

CEventConnection *FORTE_F_ROR::getEOConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_F_ROR::getDIConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_IN;
    case 1: return &conn_N;
  }
  return nullptr;
}

CDataConnection *FORTE_F_ROR::getDOConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_OUT;
  }
  return nullptr;
}


