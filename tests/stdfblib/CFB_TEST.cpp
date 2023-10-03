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
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "CFB_TEST_gen.cpp"
#endif

#include "resource.h"
#include "criticalregion.h"

DEFINE_FIRMWARE_FB(FORTE_CFB_TEST, g_nStringIdCFB_TEST)

const CStringDictionary::TStringId FORTE_CFB_TEST::scmDataInputNames[] = {g_nStringIdQI};

const CStringDictionary::TStringId FORTE_CFB_TEST::scmDataInputTypeIds[] = {g_nStringIdBOOL};

const CStringDictionary::TStringId FORTE_CFB_TEST::scmDataOutputNames[] = {g_nStringIdSR};

const CStringDictionary::TStringId FORTE_CFB_TEST::scmDataOutputTypeIds[] = {g_nStringIdBOOL};

const TForteInt16 FORTE_CFB_TEST::scmEIWithIndexes[] = {0, 2};
const TDataIOID FORTE_CFB_TEST::scmEIWith[] = {0, scmWithListDelimiter, 0, scmWithListDelimiter};
const CStringDictionary::TStringId FORTE_CFB_TEST::scmEventInputNames[] = {g_nStringIdSET, g_nStringIdRESET};

const TDataIOID FORTE_CFB_TEST::scmEOWith[] = {0, scmWithListDelimiter, 0, scmWithListDelimiter};
const TForteInt16 FORTE_CFB_TEST::scmEOWithIndexes[] = {0, 2, -1};
const CStringDictionary::TStringId FORTE_CFB_TEST::scmEventOutputNames[] = {g_nStringIdCNF, g_nStringIdCHANGED};

const SFBInterfaceSpec FORTE_CFB_TEST::scmFBInterfaceSpec = {
  2, scmEventInputNames, scmEIWith, scmEIWithIndexes,
  2, scmEventOutputNames, scmEOWith,scmEOWithIndexes, 1, scmDataInputNames, scmDataInputTypeIds,
  1, scmDataOutputNames, scmDataOutputTypeIds,
  0, nullptr,
  0, nullptr
};


const SCFB_FBInstanceData FORTE_CFB_TEST::scmInternalFBs[] = {
  {g_nStringIdPERMIT_OP, g_nStringIdE_PERMIT},
  {g_nStringIdE_SR, g_nStringIdE_SR},
  {g_nStringIdSET_CHANGED, g_nStringIdE_SWITCH},
  {g_nStringIdE_DEMUX_2, g_nStringIdE_DEMUX_2},
  {g_nStringIdE_MUX_2, g_nStringIdE_MUX_2},
  {g_nStringIdRESET_CHANGED, g_nStringIdE_SWITCH},
};

const SCFB_FBConnectionData FORTE_CFB_TEST::scmEventConnections[] = {
  {GENERATE_CONNECTION_PORT_ID_1_ARG(g_nStringIdSET), -1, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdE_MUX_2, g_nStringIdEI1), 4},
  {GENERATE_CONNECTION_PORT_ID_1_ARG(g_nStringIdRESET), -1, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdE_MUX_2, g_nStringIdEI2), 4},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdE_MUX_2, g_nStringIdEO), 4, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdPERMIT_OP, g_nStringIdEI), 0},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdSET_CHANGED, g_nStringIdEO1), 2, GENERATE_CONNECTION_PORT_ID_1_ARG(g_nStringIdCNF), -1},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdSET_CHANGED, g_nStringIdEO0), 2, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdE_SR, g_nStringIdS), 1},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdRESET_CHANGED, g_nStringIdEO0), 5, GENERATE_CONNECTION_PORT_ID_1_ARG(g_nStringIdCNF), -1},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdRESET_CHANGED, g_nStringIdEO1), 5, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdE_SR, g_nStringIdR), 1},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdPERMIT_OP, g_nStringIdEO), 0, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdE_DEMUX_2, g_nStringIdEI), 3},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdE_DEMUX_2, g_nStringIdEO1), 3, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdSET_CHANGED, g_nStringIdEI), 2},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdE_DEMUX_2, g_nStringIdEO2), 3, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdRESET_CHANGED, g_nStringIdEI), 5},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdE_SR, g_nStringIdEO), 1, GENERATE_CONNECTION_PORT_ID_1_ARG(g_nStringIdCNF), -1},
};

const SCFB_FBFannedOutConnectionData FORTE_CFB_TEST::scmFannedOutEventConnections[] = {
  {10, GENERATE_CONNECTION_PORT_ID_1_ARG(g_nStringIdCHANGED), -1},
};

const SCFB_FBConnectionData FORTE_CFB_TEST::scmDataConnections[] = {
  {GENERATE_CONNECTION_PORT_ID_1_ARG(g_nStringIdQI), -1, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdPERMIT_OP, g_nStringIdPERMIT), 0},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdE_SR, g_nStringIdQ), 1, GENERATE_CONNECTION_PORT_ID_1_ARG(g_nStringIdSR), -1},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdE_MUX_2, g_nStringIdK), 4, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdE_DEMUX_2, g_nStringIdK), 3},
};

const SCFB_FBFannedOutConnectionData FORTE_CFB_TEST::scmFannedOutDataConnections[] = {
  {1, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdSET_CHANGED, g_nStringIdG), 2},
  {1, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdRESET_CHANGED, g_nStringIdG), 5},
};

const SCFB_FBNData FORTE_CFB_TEST::scmFBNData = {
  6, scmInternalFBs,
  11, scmEventConnections,
  1, scmFannedOutEventConnections,
  3, scmDataConnections,
  2, scmFannedOutDataConnections,
  0, nullptr,
  0, nullptr
};

void FORTE_CFB_TEST::readInputData(TEventID) {
  RES_DATA_CON_CRITICAL_REGION();
  readData(0, *mDIs[0], mDIConns[0]);
}

void FORTE_CFB_TEST::writeOutputData(TEventID) {
  RES_DATA_CON_CRITICAL_REGION();
  writeData(0, *mDOs[0], mDOConns[0]);
}

void FORTE_CFB_TEST::readInternal2InterfaceOutputData(TEventID) {
  RES_DATA_CON_CRITICAL_REGION();
  if(CDataConnection *conn = getIn2IfConUnchecked(0); conn) { conn->readData(*mDOs[0]); }
}

