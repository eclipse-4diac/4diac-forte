/*******************************************************************************
 * Copyright (c) 2011 - 2013 ACIN, fortiss GmbH
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

#include "F_MUL.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "F_MUL_gen.cpp"
#endif

#include "criticalregion.h"
#include "resource.h"

DEFINE_FIRMWARE_FB(FORTE_F_MUL, g_nStringIdF_MUL)

const CStringDictionary::TStringId FORTE_F_MUL::scmDataInputNames[] = {g_nStringIdIN1, g_nStringIdIN2};

const CStringDictionary::TStringId FORTE_F_MUL::scmDataInputTypeIds[] = {g_nStringIdANY_NUM, g_nStringIdANY_NUM};

const CStringDictionary::TStringId FORTE_F_MUL::scmDataOutputNames[] = {g_nStringIdOUT};

const CStringDictionary::TStringId FORTE_F_MUL::scmDataOutputTypeIds[] = {g_nStringIdANY_NUM};

const TDataIOID FORTE_F_MUL::scmEIWith[] = {0, 1, scmWithListDelimiter};
const TForteInt16 FORTE_F_MUL::scmEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_F_MUL::scmEventInputNames[] = {g_nStringIdREQ};

const TDataIOID FORTE_F_MUL::scmEOWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_F_MUL::scmEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_F_MUL::scmEventOutputNames[] = {g_nStringIdCNF};


const SFBInterfaceSpec FORTE_F_MUL::scmFBInterfaceSpec = {
  1, scmEventInputNames, scmEIWith, scmEIWithIndexes,
  1, scmEventOutputNames, scmEOWith, scmEOWithIndexes,
  2, scmDataInputNames, scmDataInputTypeIds,
  1, scmDataOutputNames, scmDataOutputTypeIds,
  0, nullptr,
  0, nullptr
};

FORTE_F_MUL::FORTE_F_MUL(const CStringDictionary::TStringId paInstanceNameId, CResource *paSrcRes) :
    CFunctionBlock( paSrcRes, &scmFBInterfaceSpec, paInstanceNameId),
    var_IN1(CIEC_ANY_NUM_VARIANT()),
    var_IN2(CIEC_ANY_NUM_VARIANT()),
    var_OUT(CIEC_ANY_NUM_VARIANT()),
    var_conn_OUT(var_OUT),
    conn_CNF(this, 0),
    conn_IN1(nullptr),
    conn_IN2(nullptr),
    conn_OUT(this, 0, &var_conn_OUT) {
};

void FORTE_F_MUL::executeEvent(TEventID paEIID) {
  switch(paEIID) {
    case scmEventREQID:
      var_OUT = std::visit([](auto &&paIN1, auto&&paIN2) -> CIEC_ANY_NUM_VARIANT {
        using T = std::decay_t<decltype(paIN1)>;
        using U = std::decay_t<decltype(paIN2)>;
        using deductedType = typename forte::core::mpl::get_mul_operator_result_type<T, U>::type;
        if constexpr (!std::is_same<deductedType, forte::core::mpl::NullType>::value) {
          return func_MUL(paIN1, paIN2);
        }
        DEVLOG_ERROR("Multiplying incompatible types %s and %s\n",
                     CStringDictionary::getInstance().get(paIN1.getTypeNameID()),
                     CStringDictionary::getInstance().get(paIN2.getTypeNameID()));
        return CIEC_ANY_NUM_VARIANT();
      }, var_IN1, var_IN2);
      sendOutputEvent(scmEventCNFID);
      break;
  }
}

void FORTE_F_MUL::readInputData(TEventID paEIID) {
  switch(paEIID) {
    case scmEventREQID: {
      RES_DATA_CON_CRITICAL_REGION();
      readData(0, var_IN1, conn_IN1);
      readData(1, var_IN2, conn_IN2);
      break;
    }
    default:
      break;
  }
}

void FORTE_F_MUL::writeOutputData(TEventID paEIID) {
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

CIEC_ANY *FORTE_F_MUL::getDI(size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_IN1;
    case 1: return &var_IN2;
  }
  return nullptr;
}

CIEC_ANY *FORTE_F_MUL::getDO(size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_OUT;
  }
  return nullptr;
}

CEventConnection *FORTE_F_MUL::getEOConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_F_MUL::getDIConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_IN1;
    case 1: return &conn_IN2;
  }
  return nullptr;
}

CDataConnection *FORTE_F_MUL::getDOConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_OUT;
  }
  return nullptr;
}


