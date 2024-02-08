/*************************************************************************  
 *** Copyright (c) 2012, 2023 TU Wien ACIN, HR Agrartechnik GmbH  
 *** This program and the accompanying materials are made available under the  
 *** terms of the Eclipse Public License 2.0 which is available at  
 *** http://www.eclipse.org/legal/epl-2.0.  
 ***  
 *** SPDX-License-Identifier: EPL-2.0  
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x NG!
 ***
 *** Name: E_IMPULSE
 *** Description: Generate a Impulse
 *** Version:
 ***     1.0: 2023-08-21/Franz Höpfinger - HR Agrartechnik GmbH -
 *************************************************************************/

#include "E_IMPULSE_fbt.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "E_IMPULSE_fbt_gen.cpp"
#endif

#include "criticalregion.h"
#include "resource.h"

DEFINE_FIRMWARE_FB(FORTE_E_IMPULSE, g_nStringIdE_IMPULSE)

const CStringDictionary::TStringId FORTE_E_IMPULSE::scmDataInputNames[] = {g_nStringIdTM};
const CStringDictionary::TStringId FORTE_E_IMPULSE::scmDataInputTypeIds[] = {g_nStringIdTIME};
const CStringDictionary::TStringId FORTE_E_IMPULSE::scmDataOutputNames[] = {g_nStringIdQO};
const CStringDictionary::TStringId FORTE_E_IMPULSE::scmDataOutputTypeIds[] = {g_nStringIdBOOL};
const TDataIOID FORTE_E_IMPULSE::scmEIWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_E_IMPULSE::scmEIWithIndexes[] = {0, -1};
const CStringDictionary::TStringId FORTE_E_IMPULSE::scmEventInputNames[] = {g_nStringIdREQ, g_nStringIdR};
const TDataIOID FORTE_E_IMPULSE::scmEOWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_E_IMPULSE::scmEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_E_IMPULSE::scmEventOutputNames[] = {g_nStringIdCNF};
const SFBInterfaceSpec FORTE_E_IMPULSE::scmFBInterfaceSpec = {
  2, scmEventInputNames, scmEIWith, scmEIWithIndexes,
  1, scmEventOutputNames, scmEOWith, scmEOWithIndexes,
  1, scmDataInputNames, scmDataInputTypeIds,
  1, scmDataOutputNames, scmDataOutputTypeIds,
  0, nullptr,
  0, nullptr
};

FORTE_E_IMPULSE::FORTE_E_IMPULSE(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    CCompositeFB(paContainer, &scmFBInterfaceSpec, paInstanceNameId, scmFBNData),
    var_conn_QO(var_QO),
    conn_CNF(this, 0),
    conn_TM(nullptr),
    conn_QO(this, 0, &var_conn_QO) {
};

void FORTE_E_IMPULSE::setInitialValues() {
  var_TM = 0_TIME;
  var_QO = 0_BOOL;
}

const SCFB_FBInstanceData FORTE_E_IMPULSE::scmInternalFBs[] = {
  {g_nStringIdE_DELAY, g_nStringIdE_DELAY},
  {g_nStringIdE_SR, g_nStringIdE_SR}
};


const SCFB_FBConnectionData FORTE_E_IMPULSE::scmEventConnections[] = {
  {GENERATE_CONNECTION_PORT_ID_1_ARG(g_nStringIdREQ), -1, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdE_DELAY, g_nStringIdSTART), 0},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdE_DELAY, g_nStringIdEO), 0, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdE_SR, g_nStringIdR), 1},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdE_SR, g_nStringIdEO), 1, GENERATE_CONNECTION_PORT_ID_1_ARG(g_nStringIdCNF), -1},
  {GENERATE_CONNECTION_PORT_ID_1_ARG(g_nStringIdR), -1, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdE_DELAY, g_nStringIdSTOP), 0},
};

const SCFB_FBFannedOutConnectionData FORTE_E_IMPULSE::scmFannedOutEventConnections[] = {
  {0, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdE_SR, g_nStringIdS), 1},
  {3, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdE_SR, g_nStringIdR), 1},
};

const SCFB_FBConnectionData FORTE_E_IMPULSE::scmDataConnections[] = {
  {GENERATE_CONNECTION_PORT_ID_1_ARG(g_nStringIdTM), -1, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdE_DELAY, g_nStringIdDT), 0},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdE_SR, g_nStringIdQ), 1, GENERATE_CONNECTION_PORT_ID_1_ARG(g_nStringIdQO), -1},
};

const SCFB_FBNData FORTE_E_IMPULSE::scmFBNData = {
  2, scmInternalFBs,
  4, scmEventConnections,
  2, scmFannedOutEventConnections,
  2, scmDataConnections,
  0, nullptr,
  0, nullptr,
  0, nullptr
};

void FORTE_E_IMPULSE::readInternal2InterfaceOutputData(const TEventID paEOID) {
  switch(paEOID) {
    case scmEventCNFID: {
      if(CDataConnection *conn = getIn2IfConUnchecked(0); conn) { conn->readData(var_QO); }
      break;
    }
    default:
      break;
  }
}
void FORTE_E_IMPULSE::readInputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventREQID: {
      readData(0, var_TM, conn_TM);
      break;
    }
    default:
      break;
  }
}

void FORTE_E_IMPULSE::writeOutputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventCNFID: {
      writeData(0, var_QO, conn_QO);
      break;
    }
    default:
      break;
  }
}

CIEC_ANY *FORTE_E_IMPULSE::getDI(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_TM;
  }
  return nullptr;
}

CIEC_ANY *FORTE_E_IMPULSE::getDO(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_QO;
  }
  return nullptr;
}

CEventConnection *FORTE_E_IMPULSE::getEOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_E_IMPULSE::getDIConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_TM;
  }
  return nullptr;
}

CDataConnection *FORTE_E_IMPULSE::getDOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_QO;
  }
  return nullptr;
}

