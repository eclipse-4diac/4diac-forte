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

DEFINE_FIRMWARE_FB(FORTE_CFB_TEST, g_nStringIdCFB_TEST)

const CStringDictionary::TStringId FORTE_CFB_TEST::scm_anDataInputNames[] = {g_nStringIdQI};

const CStringDictionary::TStringId FORTE_CFB_TEST::scm_anDataInputTypeIds[] = {g_nStringIdBOOL};

const CStringDictionary::TStringId FORTE_CFB_TEST::scm_anDataOutputNames[] = {g_nStringIdSR};

const CStringDictionary::TStringId FORTE_CFB_TEST::scm_anDataOutputTypeIds[] = {g_nStringIdBOOL};

const TForteInt16 FORTE_CFB_TEST::scm_anEIWithIndexes[] = {0, 2};
const TDataIOID FORTE_CFB_TEST::scm_anEIWith[] = {0, 255, 0, 255};
const CStringDictionary::TStringId FORTE_CFB_TEST::scm_anEventInputNames[] = {g_nStringIdSET, g_nStringIdRESET};

const TDataIOID FORTE_CFB_TEST::scm_anEOWith[] = {0, 255, 0, 255};
const TForteInt16 FORTE_CFB_TEST::scm_anEOWithIndexes[] = {0, 2, -1};
const CStringDictionary::TStringId FORTE_CFB_TEST::scm_anEventOutputNames[] = {g_nStringIdCNF, g_nStringIdCHANGED};

const SFBInterfaceSpec FORTE_CFB_TEST::scm_stFBInterfaceSpec = {
  2,  scm_anEventInputNames,  scm_anEIWith,  scm_anEIWithIndexes,
  2,  scm_anEventOutputNames,  scm_anEOWith, scm_anEOWithIndexes,  1,  scm_anDataInputNames, scm_anDataInputTypeIds,
  1,  scm_anDataOutputNames, scm_anDataOutputTypeIds,
  0, 0
};


const SCFB_FBInstanceData FORTE_CFB_TEST::scm_astInternalFBs[] = {
  {g_nStringIdPERMIT_OP, g_nStringIdE_PERMIT},
  {g_nStringIdE_SR, g_nStringIdE_SR},
  {g_nStringIdSET_CHANGED, g_nStringIdE_SWITCH},
  {g_nStringIdE_DEMUX_2, g_nStringIdE_DEMUX_2},
  {g_nStringIdE_MUX_2, g_nStringIdE_MUX_2},
  {g_nStringIdRESET_CHANGED, g_nStringIdE_SWITCH},
};

const SCFB_FBConnectionData FORTE_CFB_TEST::scm_astEventConnections[] = {
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

const SCFB_FBFannedOutConnectionData FORTE_CFB_TEST::scm_astFannedOutEventConnections[] = {
  {10, GENERATE_CONNECTION_PORT_ID_1_ARG(g_nStringIdCHANGED), -1},
};

const SCFB_FBConnectionData FORTE_CFB_TEST::scm_astDataConnections[] = {
  {GENERATE_CONNECTION_PORT_ID_1_ARG(g_nStringIdQI), -1, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdPERMIT_OP, g_nStringIdPERMIT), 0},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdE_SR, g_nStringIdQ), 1, GENERATE_CONNECTION_PORT_ID_1_ARG(g_nStringIdSR), -1},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdE_MUX_2, g_nStringIdK), 4, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdE_DEMUX_2, g_nStringIdK), 3},
};

const SCFB_FBFannedOutConnectionData FORTE_CFB_TEST::scm_astFannedOutDataConnections[] = {
  {1, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdSET_CHANGED, g_nStringIdG), 2},
  {1, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdRESET_CHANGED, g_nStringIdG), 5},
};

const SCFB_FBNData FORTE_CFB_TEST::scm_stFBNData = {
  6, scm_astInternalFBs,
  11, scm_astEventConnections,
  1, scm_astFannedOutEventConnections,
  3, scm_astDataConnections,
  2, scm_astFannedOutDataConnections,
  0, 0
};


