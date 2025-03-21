/*************************************************************************
 *** Copyright (c) 2024 HR Agrartechnik GmbH  
 *** This program and the accompanying materials are made available under the  
 *** terms of the Eclipse Public License 2.0 which is available at  
 *** http://www.eclipse.org/legal/epl-2.0.  
 ***  
 *** SPDX-License-Identifier: EPL-2.0  
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x NG!
 ***
 *** Name: E_TP
 *** Description: standard timer function block (pulse)
 *** Version:
 ***     1.0: 2024-03-04/Franz Hoepfinger - HR Agrartechnik GmbH -
 ***     1.1: 2024-04-23/Franz Hoepfinger - HR Agrartechnik GmbH - Add a Reset to Timer FBs
 *************************************************************************/

#include "E_TP_fbt.h"

USE_STRING_ID(BOOL);
USE_STRING_ID(CNF);
USE_STRING_ID(DT);
USE_STRING_ID(E_DELAY);
USE_STRING_ID(EI);
USE_STRING_ID(EO);
USE_STRING_ID(E_PERMIT);
USE_STRING_ID(E_RS);
USE_STRING_ID(E_TP);
USE_STRING_ID(Event);
USE_STRING_ID(IN);
USE_STRING_ID(PERMIT);
USE_STRING_ID(PT);
USE_STRING_ID(Q);
USE_STRING_ID(R);
USE_STRING_ID(REQ);
USE_STRING_ID(S);
USE_STRING_ID(START);
USE_STRING_ID(STOP);
USE_STRING_ID(TIME);


#include "criticalregion.h"
#include "resource.h"

DEFINE_FIRMWARE_FB(FORTE_E_TP, STRID(E_TP))

const CStringDictionary::TStringId FORTE_E_TP::scmDataInputNames[] = {STRID(IN), STRID(PT)};
const CStringDictionary::TStringId FORTE_E_TP::scmDataInputTypeIds[] = {STRID(BOOL), STRID(TIME)};
const CStringDictionary::TStringId FORTE_E_TP::scmDataOutputNames[] = {STRID(Q)};
const CStringDictionary::TStringId FORTE_E_TP::scmDataOutputTypeIds[] = {STRID(BOOL)};
const TDataIOID FORTE_E_TP::scmEIWith[] = {0, 1, scmWithListDelimiter, 0, scmWithListDelimiter};
const TForteInt16 FORTE_E_TP::scmEIWithIndexes[] = {0, 3};
const CStringDictionary::TStringId FORTE_E_TP::scmEventInputNames[] = {STRID(REQ), STRID(R)};
const CStringDictionary::TStringId FORTE_E_TP::scmEventInputTypeIds[] = {STRID(Event), STRID(Event)};
const TDataIOID FORTE_E_TP::scmEOWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_E_TP::scmEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_E_TP::scmEventOutputNames[] = {STRID(CNF)};
const CStringDictionary::TStringId FORTE_E_TP::scmEventOutputTypeIds[] = {STRID(Event)};
const SFBInterfaceSpec FORTE_E_TP::scmFBInterfaceSpec = {
  2, scmEventInputNames, scmEventInputTypeIds, scmEIWith, scmEIWithIndexes,
  1, scmEventOutputNames, scmEventOutputTypeIds, scmEOWith, scmEOWithIndexes,
  2, scmDataInputNames, scmDataInputTypeIds,
  1, scmDataOutputNames, scmDataOutputTypeIds,
  0, nullptr,
  0, nullptr
};

FORTE_E_TP::FORTE_E_TP(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    CCompositeFB(paContainer, scmFBInterfaceSpec, paInstanceNameId, scmFBNData),
    fb_E_DELAY(STRID(E_DELAY), *this),
    fb_E_RS(STRID(E_RS), *this),
    fb_E_PERMIT(STRID(E_PERMIT), *this),
    conn_CNF(this, 0),
    conn_IN(nullptr),
    conn_PT(nullptr),
    conn_Q(this, 0, var_Q) {
};

void FORTE_E_TP::setInitialValues() {
    var_IN = 0_BOOL;
    var_PT = 0_TIME;
    var_Q = 0_BOOL;
}

const SCFB_FBInstanceData FORTE_E_TP::scmInternalFBs[] = {
  {STRID(E_DELAY), STRID(E_DELAY)},
  {STRID(E_RS), STRID(E_RS)},
  {STRID(E_PERMIT), STRID(E_PERMIT)}
};


const SCFB_FBConnectionData FORTE_E_TP::scmEventConnections[] = {
  {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_RS), STRID(EO)), 1, GENERATE_CONNECTION_PORT_ID_1_ARG(STRID(CNF)), -1},
  {GENERATE_CONNECTION_PORT_ID_1_ARG(STRID(R)), -1, GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_DELAY), STRID(STOP)), 0},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_DELAY), STRID(EO)), 0, GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_RS), STRID(R)), 1},
  {GENERATE_CONNECTION_PORT_ID_1_ARG(STRID(REQ)), -1, GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_PERMIT), STRID(EI)), 2},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_PERMIT), STRID(EO)), 2, GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_RS), STRID(S)), 1},
};

const SCFB_FBFannedOutConnectionData FORTE_E_TP::scmFannedOutEventConnections[] = {
  {1, GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_RS), STRID(R)), 1},
  {4, GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_DELAY), STRID(START)), 0},
};

const SCFB_FBConnectionData FORTE_E_TP::scmDataConnections[] = {
  {GENERATE_CONNECTION_PORT_ID_1_ARG(STRID(PT)), -1, GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_DELAY), STRID(DT)), 0},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_RS), STRID(Q)), 1, GENERATE_CONNECTION_PORT_ID_1_ARG(STRID(Q)), -1},
  {GENERATE_CONNECTION_PORT_ID_1_ARG(STRID(IN)), -1, GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_PERMIT), STRID(PERMIT)), 2},
};

const SCFB_FBNData FORTE_E_TP::scmFBNData = {
  3, scmInternalFBs,
  5, scmEventConnections,
  2, scmFannedOutEventConnections,
  3, scmDataConnections,
  0, nullptr,
  0, nullptr,
  0, nullptr
};

void FORTE_E_TP::readInternal2InterfaceOutputData(const TEventID paEOID) {
  switch(paEOID) {
    case scmEventCNFID: {
      if(CDataConnection *conn = getIn2IfConUnchecked(0); conn) { conn->readData(var_Q); }
      break;
    }
    default:
      break;
  }
}
void FORTE_E_TP::readInputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventREQID: {
      readData(0, var_IN, conn_IN);
      readData(1, var_PT, conn_PT);
      break;
    }
    case scmEventRID: {
      readData(0, var_IN, conn_IN);
      break;
    }
    default:
      break;
  }
}

void FORTE_E_TP::writeOutputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventCNFID: {
      writeData(0, var_Q, conn_Q);
      break;
    }
    default:
      break;
  }
}

CIEC_ANY *FORTE_E_TP::getDI(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_IN;
    case 1: return &var_PT;
  }
  return nullptr;
}

CIEC_ANY *FORTE_E_TP::getDO(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_Q;
  }
  return nullptr;
}

CEventConnection *FORTE_E_TP::getEOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_E_TP::getDIConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_IN;
    case 1: return &conn_PT;
  }
  return nullptr;
}

CDataConnection *FORTE_E_TP::getDOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_Q;
  }
  return nullptr;
}

