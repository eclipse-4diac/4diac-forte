/*******************************************************************************
 * Copyright (c) 2005 - 2013 ACIN, Profactor GmbH, fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Alois Zoitl, Gerhard Ebenhofer
 *     - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "E_CYCLE.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "E_CYCLE_gen.cpp"
#endif

#ifndef FMU

DEFINE_FIRMWARE_FB(E_CYCLE, g_nStringIdE_CYCLE)

#else

DEFINE_FIRMWARE_FB(E_CYCLE, g_nStringIdE_CYCLE)

const CStringDictionary::TStringId E_CYCLE::scm_anDataInputNames[] = {g_nStringIdDT};

const CStringDictionary::TStringId E_CYCLE::scm_anDataInputTypeIds[] = {g_nStringIdTIME};

const TForteInt16 E_CYCLE::scm_anEIWithIndexes[] = {0, -1};
const TDataIOID E_CYCLE::scm_anEIWith[] = {0, 255};
const CStringDictionary::TStringId E_CYCLE::scm_anEventInputNames[] = {g_nStringIdSTART, g_nStringIdSTOP};

const TForteInt16 E_CYCLE::scm_anEOWithIndexes[] = {-1, -1};
const CStringDictionary::TStringId E_CYCLE::scm_anEventOutputNames[] = {g_nStringIdEO};

const SFBInterfaceSpec E_CYCLE::scm_stFBInterfaceSpec = {
  2,  scm_anEventInputNames,  scm_anEIWith,  scm_anEIWithIndexes,
  1,  scm_anEventOutputNames,  0, 0,  1,  scm_anDataInputNames, scm_anDataInputTypeIds,
  0,  0, 0,
  0, 0
};


const SCFB_FBInstanceData E_CYCLE::scm_astInternalFBs[] = {
  {g_nStringIdE_DELAY, g_nStringIdE_DELAY},
};

const SCFB_FBConnectionData E_CYCLE::scm_astEventConnections[] = {
  {GENERATE_CONNECTION_PORT_ID_1_ARG(g_nStringIdSTART), -1, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdE_DELAY, g_nStringIdSTART), 0},
  {GENERATE_CONNECTION_PORT_ID_1_ARG(g_nStringIdSTOP), -1, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdE_DELAY, g_nStringIdSTOP), 0},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdE_DELAY, g_nStringIdEO), 0, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdE_DELAY, g_nStringIdSTART), 0},
};

const SCFB_FBFannedOutConnectionData E_CYCLE::scm_astFannedOutEventConnections[] = {
  {2, GENERATE_CONNECTION_PORT_ID_1_ARG(g_nStringIdEO), -1},
};

const SCFB_FBConnectionData E_CYCLE::scm_astDataConnections[] = {
  {GENERATE_CONNECTION_PORT_ID_1_ARG(g_nStringIdDT), -1, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdE_DELAY, g_nStringIdDT), 0},
};

const SCFB_FBNData E_CYCLE::scm_stFBNData = {
  1, scm_astInternalFBs,
  3, scm_astEventConnections,
  1, scm_astFannedOutEventConnections,
  1, scm_astDataConnections,
  0, 0,
  0, 0
};

#endif

