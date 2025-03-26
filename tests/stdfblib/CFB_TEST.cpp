/*******************************************************************************
 * Copyright (c) 2014 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl  - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "CFB_TEST.h"

USE_STRING_ID(BOOL);
USE_STRING_ID(CFB_TEST);
USE_STRING_ID(CHANGED);
USE_STRING_ID(CNF);
USE_STRING_ID(E_DEMUX_2);
USE_STRING_ID(EI);
USE_STRING_ID(EI1);
USE_STRING_ID(EI2);
USE_STRING_ID(E_MUX_2);
USE_STRING_ID(EO);
USE_STRING_ID(EO0);
USE_STRING_ID(EO1);
USE_STRING_ID(EO2);
USE_STRING_ID(E_PERMIT);
USE_STRING_ID(E_SR);
USE_STRING_ID(E_SWITCH);
USE_STRING_ID(Event);
USE_STRING_ID(G);
USE_STRING_ID(K);
USE_STRING_ID(PERMIT);
USE_STRING_ID(PERMIT_OP);
USE_STRING_ID(Q);
USE_STRING_ID(QI);
USE_STRING_ID(QO);
USE_STRING_ID(R);
USE_STRING_ID(RESET);
USE_STRING_ID(RESET_CHANGED);
USE_STRING_ID(S);
USE_STRING_ID(SET);
USE_STRING_ID(SET_CHANGED);


#include "resource.h"
#include "criticalregion.h"

DEFINE_FIRMWARE_FB(FORTE_CFB_TEST, STRID(CFB_TEST))

const CStringDictionary::TStringId FORTE_CFB_TEST::scmDataInputNames[] = {STRID(QI)};
const CStringDictionary::TStringId FORTE_CFB_TEST::scmDataInputTypeIds[] = {STRID(BOOL)};
const CStringDictionary::TStringId FORTE_CFB_TEST::scmDataOutputNames[] = {STRID(QO)};
const CStringDictionary::TStringId FORTE_CFB_TEST::scmDataOutputTypeIds[] = {STRID(BOOL)};
const TDataIOID FORTE_CFB_TEST::scmEIWith[] = {0, scmWithListDelimiter, 0, scmWithListDelimiter};
const TForteInt16 FORTE_CFB_TEST::scmEIWithIndexes[] = {0, 2};
const CStringDictionary::TStringId FORTE_CFB_TEST::scmEventInputNames[] = {STRID(SET), STRID(RESET)};
const CStringDictionary::TStringId FORTE_CFB_TEST::scmEventInputTypeIds[] = {STRID(Event), STRID(Event)};
const TDataIOID FORTE_CFB_TEST::scmEOWith[] = {0, scmWithListDelimiter, 0, scmWithListDelimiter};
const TForteInt16 FORTE_CFB_TEST::scmEOWithIndexes[] = {0, 2};
const CStringDictionary::TStringId FORTE_CFB_TEST::scmEventOutputNames[] = {STRID(CNF), STRID(CHANGED)};
const CStringDictionary::TStringId FORTE_CFB_TEST::scmEventOutputTypeIds[] = {STRID(Event), STRID(Event)};
const SFBInterfaceSpec FORTE_CFB_TEST::scmFBInterfaceSpec = {
  2, scmEventInputNames, scmEventInputTypeIds, scmEIWith, scmEIWithIndexes,
  2, scmEventOutputNames, scmEventOutputTypeIds, scmEOWith, scmEOWithIndexes,
  1, scmDataInputNames, scmDataInputTypeIds,
  1, scmDataOutputNames, scmDataOutputTypeIds,
  0, nullptr,
  0, nullptr
};

FORTE_CFB_TEST::FORTE_CFB_TEST(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    CCompositeFB(paContainer, scmFBInterfaceSpec, paInstanceNameId, scmFBNData),
    fb_PERMIT_OP(STRID(PERMIT_OP), *this),
    fb_E_SR(STRID(E_SR), *this),
    fb_SET_CHANGED(STRID(SET_CHANGED), *this),
    fb_E_DEMUX_2(STRID(E_DEMUX_2), "E_DEMUX_2", *this),
    fb_E_MUX_2(STRID(E_MUX_2), "E_MUX_2", *this),
    fb_RESET_CHANGED(STRID(RESET_CHANGED), *this),
    conn_CNF(*this, 0),
    conn_CHANGED(*this, 1),
    conn_QI(nullptr),
    conn_QO(*this, 0, 0_BOOL),
    conn_if2in_QI(*this, 0, 0_BOOL) {
};

void FORTE_CFB_TEST::setInitialValues() {
    conn_if2in_QI.getValue() = 0_BOOL;
    fb_E_SR->conn_Q.getValue() = 0_BOOL;
}

const SCFB_FBInstanceData FORTE_CFB_TEST::scmInternalFBs[] = {
  {STRID(PERMIT_OP), STRID(E_PERMIT)},
  {STRID(E_SR), STRID(E_SR)},
  {STRID(SET_CHANGED), STRID(E_SWITCH)},
  {STRID(E_DEMUX_2), STRID(E_DEMUX_2)},
  {STRID(E_MUX_2), STRID(E_MUX_2)},
  {STRID(RESET_CHANGED), STRID(E_SWITCH)},
};

const SCFB_FBConnectionData FORTE_CFB_TEST::scmEventConnections[] = {
  {GENERATE_CONNECTION_PORT_ID_1_ARG(STRID(SET)), -1, GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_MUX_2), STRID(EI1)), 4},
  {GENERATE_CONNECTION_PORT_ID_1_ARG(STRID(RESET)), -1, GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_MUX_2), STRID(EI2)), 4},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_MUX_2), STRID(EO)), 4, GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(PERMIT_OP), STRID(EI)), 0},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(SET_CHANGED), STRID(EO1)), 2, GENERATE_CONNECTION_PORT_ID_1_ARG(STRID(CNF)), -1},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(SET_CHANGED), STRID(EO0)), 2, GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_SR), STRID(S)), 1},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(RESET_CHANGED), STRID(EO0)), 5, GENERATE_CONNECTION_PORT_ID_1_ARG(STRID(CNF)), -1},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(RESET_CHANGED), STRID(EO1)), 5, GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_SR), STRID(R)), 1},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(PERMIT_OP), STRID(EO)), 0, GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_DEMUX_2), STRID(EI)), 3},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_DEMUX_2), STRID(EO1)), 3, GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(SET_CHANGED), STRID(EI)), 2},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_DEMUX_2), STRID(EO2)), 3, GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(RESET_CHANGED), STRID(EI)), 5},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_SR), STRID(EO)), 1, GENERATE_CONNECTION_PORT_ID_1_ARG(STRID(CNF)), -1},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_SR), STRID(EO)), 1, GENERATE_CONNECTION_PORT_ID_1_ARG(STRID(CHANGED)), -1},
};

const SCFB_FBConnectionData FORTE_CFB_TEST::scmDataConnections[] = {
  {GENERATE_CONNECTION_PORT_ID_1_ARG(STRID(QI)), -1, GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(PERMIT_OP), STRID(PERMIT)), 0},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_SR), STRID(Q)), 1, GENERATE_CONNECTION_PORT_ID_1_ARG(STRID(QO)), -1},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_SR), STRID(Q)), 1, GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(SET_CHANGED), STRID(G)), 2},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_SR), STRID(Q)), 1, GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(RESET_CHANGED), STRID(G)), 5},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_MUX_2), STRID(K)), 4, GENERATE_CONNECTION_PORT_ID_2_ARG(STRID(E_DEMUX_2), STRID(K)), 3},
};

const SCFB_FBNData FORTE_CFB_TEST::scmFBNData = {
  6, scmInternalFBs,
  12, scmEventConnections,
  5, scmDataConnections,
  0, nullptr,
  0, nullptr
};

void FORTE_CFB_TEST::readInputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventSETID: {
      readData(0, conn_if2in_QI.getValue(), conn_QI);
      break;
    }
    case scmEventRESETID: {
      readData(0, conn_if2in_QI.getValue(), conn_QI);
      break;
    }
    default:
      break;
  }
}

void FORTE_CFB_TEST::writeOutputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventCNFID: {
      writeData(0, fb_E_SR->conn_Q.getValue(), conn_QO);
      break;
    }
    case scmEventCHANGEDID: {
      writeData(0, fb_E_SR->conn_Q.getValue(), conn_QO);
      break;
    }
    default:
      break;
  }
}

CIEC_ANY *FORTE_CFB_TEST::getDI(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &conn_if2in_QI.getValue();
  }
  return nullptr;
}

CIEC_ANY *FORTE_CFB_TEST::getDO(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &fb_E_SR->conn_Q.getValue();
  }
  return nullptr;
}

CEventConnection *FORTE_CFB_TEST::getEOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_CNF;
    case 1: return &conn_CHANGED;
  }
  return nullptr;
}

CDataConnection **FORTE_CFB_TEST::getDIConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_QI;
  }
  return nullptr;
}

CDataConnection *FORTE_CFB_TEST::getDOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_QO;
  }
  return nullptr;
}

CDataConnection *FORTE_CFB_TEST::getIf2InConUnchecked(TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_if2in_QI;
  }
  return nullptr;
}

