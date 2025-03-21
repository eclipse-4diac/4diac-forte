/*************************************************************************
 *** Copyright (c) 2023, 2024 HR Agrartechnik GmbH  
 *** This program and the accompanying materials are made available under the  
 *** terms of the Eclipse Public License 2.0 which is available at  
 *** http://www.eclipse.org/legal/epl-2.0.  
 ***  
 *** SPDX-License-Identifier: EPL-2.0  
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x NG!
 ***
 *** Name: E_PULSE
 *** Description: standard timer function block (pulse)
 *** Version:
 ***     1.0: 2023-08-21/Franz Hoepfinger - HR Agrartechnik GmbH - initial implementation as E_IMPULSE
 ***     1.0: 2024-03-05/Franz Hoepfinger - HR Agrartechnik GmbH - renamed to E_PULSE
 ***     1.1: 2024-04-23/Franz Hoepfinger - HR Agrartechnik GmbH - Add a Reset to Timer FBs
 *************************************************************************/

#include "E_PULSE_fbt.h"

USE_STRING_ID(BOOL);
USE_STRING_ID(CNF);
USE_STRING_ID(DT);
USE_STRING_ID(E_DELAY);
USE_STRING_ID(EO);
USE_STRING_ID(E_PULSE);
USE_STRING_ID(E_SR);
USE_STRING_ID(Event);
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

DEFINE_FIRMWARE_FB(FORTE_E_PULSE, STRID(E_PULSE))

const CStringDictionary::TStringId FORTE_E_PULSE::scmDataInputNames[] = {STRID(PT)};
const CStringDictionary::TStringId FORTE_E_PULSE::scmDataInputTypeIds[] = {STRID(TIME)};
const CStringDictionary::TStringId FORTE_E_PULSE::scmDataOutputNames[] = {STRID(Q)};
const CStringDictionary::TStringId FORTE_E_PULSE::scmDataOutputTypeIds[] = {STRID(BOOL)};
const TDataIOID FORTE_E_PULSE::scmEIWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_E_PULSE::scmEIWithIndexes[] = {0, -1};
const CStringDictionary::TStringId FORTE_E_PULSE::scmEventInputNames[] = {STRID(REQ), STRID(R)};
const CStringDictionary::TStringId FORTE_E_PULSE::scmEventInputTypeIds[] = {STRID(Event), STRID(Event)};
const TDataIOID FORTE_E_PULSE::scmEOWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_E_PULSE::scmEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_E_PULSE::scmEventOutputNames[] = {STRID(CNF)};
const CStringDictionary::TStringId FORTE_E_PULSE::scmEventOutputTypeIds[] = {STRID(Event)};
const SFBInterfaceSpec FORTE_E_PULSE::scmFBInterfaceSpec = {
  2, scmEventInputNames, scmEventInputTypeIds, scmEIWith, scmEIWithIndexes,
  1, scmEventOutputNames, scmEventOutputTypeIds, scmEOWith, scmEOWithIndexes,
  1, scmDataInputNames, scmDataInputTypeIds,
  1, scmDataOutputNames, scmDataOutputTypeIds,
  0, nullptr,
  0, nullptr
};

FORTE_E_PULSE::FORTE_E_PULSE(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    CCompositeFB(paContainer, scmFBInterfaceSpec, paInstanceNameId, scmFBNData),
    fb_E_DELAY(STRID(E_DELAY), *this),
    fb_E_SR(STRID(E_SR), *this),
    conn_CNF(this, 0),
    conn_PT(nullptr),
    conn_Q(this, 0, 0_BOOL),
    conn_if2in_PT(this, 0, 0_TIME) {
};

void FORTE_E_PULSE::setInitialValues() {
    conn_if2in_PT.getValue() = 0_TIME;
    fb_E_SR->conn_Q.getValue() = 0_BOOL;
}

const SCFB_FBInstanceData FORTE_E_PULSE::scmInternalFBs[] = {
  {STRID(E_DELAY), STRID(E_DELAY)},
  {STRID(E_SR), STRID(E_SR)}
};


const SCFB_FBConnectionData FORTE_E_PULSE::scmEventConnections[] = {
  {GENERATE_CONNECTION_PORT_ID_1_ARG(STRID(REQ)), -1, GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_DELAY), STRID(START)), 0},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_DELAY), STRID(EO)), 0, GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_SR), STRID(R)), 1},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_SR), STRID(EO)), 1, GENERATE_CONNECTION_PORT_ID_1_ARG(STRID(CNF)), -1},
  {GENERATE_CONNECTION_PORT_ID_1_ARG(STRID(R)), -1, GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_DELAY), STRID(STOP)), 0},
};

const SCFB_FBFannedOutConnectionData FORTE_E_PULSE::scmFannedOutEventConnections[] = {
  {0, GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_SR), STRID(S)), 1},
  {3, GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_SR), STRID(R)), 1},
};

const SCFB_FBConnectionData FORTE_E_PULSE::scmDataConnections[] = {
  {GENERATE_CONNECTION_PORT_ID_1_ARG(STRID(PT)), -1, GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_DELAY), STRID(DT)), 0},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_SR), STRID(Q)), 1, GENERATE_CONNECTION_PORT_ID_1_ARG(STRID(Q)), -1},
};

const SCFB_FBNData FORTE_E_PULSE::scmFBNData = {
  2, scmInternalFBs,
  4, scmEventConnections,
  2, scmFannedOutEventConnections,
  2, scmDataConnections,
  0, nullptr,
  0, nullptr,
  0, nullptr
};

void FORTE_E_PULSE::readInputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventREQID: {
      readData(0, conn_if2in_PT.getValue(), conn_PT);
      break;
    }
    default:
      break;
  }
}

void FORTE_E_PULSE::writeOutputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventCNFID: {
      writeData(0, fb_E_SR->conn_Q.getValue(), conn_Q);
      break;
    }
    default:
      break;
  }
}

CIEC_ANY *FORTE_E_PULSE::getDI(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &conn_if2in_PT.getValue();
  }
  return nullptr;
}

CIEC_ANY *FORTE_E_PULSE::getDO(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &fb_E_SR->conn_Q.getValue();
  }
  return nullptr;
}

CEventConnection *FORTE_E_PULSE::getEOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_E_PULSE::getDIConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_PT;
  }
  return nullptr;
}

CDataConnection *FORTE_E_PULSE::getDOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_Q;
  }
  return nullptr;
}

CDataConnection *FORTE_E_PULSE::getIf2InConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_if2in_PT;
  }
  return nullptr;
}

