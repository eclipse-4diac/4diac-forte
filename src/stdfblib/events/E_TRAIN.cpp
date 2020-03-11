/*******************************************************************************
 * Copyright (c) 2017 fortiss GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl
 *     - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "E_TRAIN.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "E_TRAIN_gen.cpp"
#endif

DEFINE_FIRMWARE_FB(FORTE_E_TRAIN, g_nStringIdE_TRAIN)

const CStringDictionary::TStringId FORTE_E_TRAIN::scm_anDataInputNames[] = {g_nStringIdDT, g_nStringIdN};

const CStringDictionary::TStringId FORTE_E_TRAIN::scm_anDataInputTypeIds[] = {g_nStringIdTIME, g_nStringIdUINT};

const CStringDictionary::TStringId FORTE_E_TRAIN::scm_anDataOutputNames[] = {g_nStringIdCV};

const CStringDictionary::TStringId FORTE_E_TRAIN::scm_anDataOutputTypeIds[] = {g_nStringIdUINT};

const TForteInt16 FORTE_E_TRAIN::scm_anEIWithIndexes[] = {0, 2};
const TDataIOID FORTE_E_TRAIN::scm_anEIWith[] = {0, 255, 0, 255};
const CStringDictionary::TStringId FORTE_E_TRAIN::scm_anEventInputNames[] = {g_nStringIdSTART, g_nStringIdSTOP};

const TDataIOID FORTE_E_TRAIN::scm_anEOWith[] = {0, 255};
const TForteInt16 FORTE_E_TRAIN::scm_anEOWithIndexes[] = {0, -1};
const CStringDictionary::TStringId FORTE_E_TRAIN::scm_anEventOutputNames[] = {g_nStringIdEO};

const SFBInterfaceSpec FORTE_E_TRAIN::scm_stFBInterfaceSpec = {
  2,  scm_anEventInputNames,  scm_anEIWith,  scm_anEIWithIndexes,
  1,  scm_anEventOutputNames,  scm_anEOWith, scm_anEOWithIndexes,  2,  scm_anDataInputNames, scm_anDataInputTypeIds,
  1,  scm_anDataOutputNames, scm_anDataOutputTypeIds,
  0, 0
};


const SCFB_FBInstanceData FORTE_E_TRAIN::scm_astInternalFBs[] = {
  {g_nStringIdCTR, g_nStringIdE_CTU},
  {g_nStringIdGATE, g_nStringIdE_SWITCH},
  {g_nStringIdDLY, g_nStringIdE_DELAY},
};

const SCFB_FBConnectionData FORTE_E_TRAIN::scm_astEventConnections[] = {
  {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdCTR, g_nStringIdCUO), 0, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdGATE, g_nStringIdEI), 1},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdCTR, g_nStringIdRO), 0, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdGATE, g_nStringIdEI), 1},
  {GENERATE_CONNECTION_PORT_ID_1_ARG(g_nStringIdSTART), -1, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdCTR, g_nStringIdR), 0},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdGATE, g_nStringIdEO0), 1, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdDLY, g_nStringIdSTART), 2},
  {GENERATE_CONNECTION_PORT_ID_1_ARG(g_nStringIdSTOP), -1, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdDLY, g_nStringIdSTOP), 2},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdDLY, g_nStringIdEO), 2, GENERATE_CONNECTION_PORT_ID_1_ARG(g_nStringIdEO), -1},
};

const SCFB_FBFannedOutConnectionData FORTE_E_TRAIN::scm_astFannedOutEventConnections[] = {
  {5, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdCTR, g_nStringIdCU), 0},
};

const SCFB_FBConnectionData FORTE_E_TRAIN::scm_astDataConnections[] = {
  {GENERATE_CONNECTION_PORT_ID_1_ARG(g_nStringIdN), -1, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdCTR, g_nStringIdPV), 0},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdCTR, g_nStringIdCV), 0, GENERATE_CONNECTION_PORT_ID_1_ARG(g_nStringIdCV), -1},
  {GENERATE_CONNECTION_PORT_ID_1_ARG(g_nStringIdDT), -1, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdDLY, g_nStringIdDT), 2},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdCTR, g_nStringIdQ), 0, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdGATE, g_nStringIdG), 1},
};

const SCFB_FBNData FORTE_E_TRAIN::scm_stFBNData = {
  3, scm_astInternalFBs,
  6, scm_astEventConnections,
  1, scm_astFannedOutEventConnections,
  4, scm_astDataConnections,
  0, 0,
  0, 0
};


