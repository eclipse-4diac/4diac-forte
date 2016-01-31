/*******************************************************************************
 * Copyright (c) 2006 - 2015 ACIN, Profactor GmbH, fortiss GmbH, nxtControl GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Alois Zoitl, Thomas Strasser, Gerhard Ebenhofer, Ingo Hegny,
 *   Stanislav Meduna, Micheal Hofmann
 *    - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "DM_KRNL.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "DM_KRNL_gen.cpp"
#endif
#include <typelib.h>

DEFINE_FIRMWARE_FB(DM_KRNL, g_nStringIdDM_KRNL)

const CStringDictionary::TStringId DM_KRNL::scm_aunDataInputNameIds[] = {g_nStringIdQI, g_nStringIdID};

const CStringDictionary::TStringId DM_KRNL::scm_aunDataOutputNameIds[] = {g_nStringIdQO, g_nStringIdSTATUS};

const TForteInt16 DM_KRNL::scm_anEIWithIndexes[] = {0};
const TDataIOID DM_KRNL::scm_anEIWith[] = {0, 1, 255};
const CStringDictionary::TStringId DM_KRNL::scm_aunEventInputNameIds[] = {g_nStringIdINIT};

const TDataIOID DM_KRNL::scm_anEOWith[] = {0, 1, 255};
const TForteInt16 DM_KRNL::scm_anEOWithIndexes[] = {0};
const CStringDictionary::TStringId DM_KRNL::scm_aunEventOutputNameIds[] = {g_nStringIdINITO};

const CStringDictionary::TStringId DM_KRNL::scm_aunDataTypeIds[] = {g_nStringIdBOOL, g_nStringIdWSTRING};


const SFBInterfaceSpec DM_KRNL::scm_stFBInterfaceSpec = {
  1,
  scm_aunEventInputNameIds,
  scm_anEIWith,
  scm_anEIWithIndexes,
  1,
  scm_aunEventOutputNameIds,
  scm_anEOWith,
  scm_anEOWithIndexes,
  2,
  scm_aunDataInputNameIds,
  scm_aunDataTypeIds,
  2,
  scm_aunDataOutputNameIds,
  scm_aunDataTypeIds,
  0,
  0
};

#define replace_1(name)  g_nStringId##name

#define FORTE_GEN_DEV_MGR_NAME(name) replace_1(name)

#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
	extern const CStringDictionary::TStringId FORTE_GEN_DEV_MGR_NAME(FORTE_MGM_COMMAND_PROTOCOL);
#endif

const SCFB_FBInstanceData DM_KRNL::scm_astInternalFBs[] = {
    {g_nStringIdSVR, g_nStringIdSERVER_1_2},
    {g_nStringIdMGR, FORTE_GEN_DEV_MGR_NAME(FORTE_MGM_COMMAND_PROTOCOL)}};

const SCFB_FBConnectionData DM_KRNL::scm_astEventConnections[] = {
    {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdSVR, g_nStringIdINITO), 0, GENERATE_CONNECTION_PORT_ID_1_ARG(g_nStringIdINITO), -1},
    {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdSVR, g_nStringIdIND), 0, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdMGR,g_nStringIdREQ), 1},
    {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdMGR, g_nStringIdCNF), 1, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdSVR,g_nStringIdRSP), 0},
    {GENERATE_CONNECTION_PORT_ID_1_ARG(g_nStringIdINIT), -1, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdMGR,g_nStringIdINIT), 1},
    {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdMGR,g_nStringIdINITO), 1, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdSVR,g_nStringIdINIT), 0}
};

const SCFB_FBConnectionData DM_KRNL::scm_astDataConnections[] = {
    {GENERATE_CONNECTION_PORT_ID_1_ARG(g_nStringIdQI), -1, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdMGR, g_nStringIdQI),1},
    {GENERATE_CONNECTION_PORT_ID_1_ARG(g_nStringIdID),-1,GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdSVR, g_nStringIdID),0},
    {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdSVR,g_nStringIdSTATUS), 0, GENERATE_CONNECTION_PORT_ID_1_ARG(g_nStringIdSTATUS),-1},
    {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdSVR,g_nStringIdQO), 0,GENERATE_CONNECTION_PORT_ID_1_ARG(g_nStringIdQO),-1},
    {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdSVR,g_nStringIdRD_2),0,GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdMGR, g_nStringIdRQST),1},
    {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdMGR,g_nStringIdQO),1,GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdSVR, g_nStringIdQI),0},
    {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdMGR,g_nStringIdRESP),1,GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdSVR, g_nStringIdSD_1),0},
    {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdSVR,g_nStringIdRD_1),0,GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdMGR, g_nStringIdDST),1}
};

const SCFB_FBNData DM_KRNL::scm_stFBNData = {
    2,
    scm_astInternalFBs,
    5,
    scm_astEventConnections,
    0,
    0,
    8,
    scm_astDataConnections,
    0,
    0,
    0,
    0
};

DM_KRNL::~DM_KRNL(){
}



