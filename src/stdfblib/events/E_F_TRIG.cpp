/*******************************************************************************
 * Copyright (c) 2007 - 2013 ACIN, fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Alois Zoitl, Ingo Hegny
 *     - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "E_F_TRIG.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "E_F_TRIG_gen.cpp"
#endif
#include <typelib.h>

#ifndef FMU

DEFINE_FIRMWARE_FB(E_F_TRIG, g_nStringIdE_F_TRIG)

const CStringDictionary::TStringId E_F_TRIG::scm_anDataInputNames[] = {g_nStringIdQI};
const CStringDictionary::TStringId E_F_TRIG::scm_aunDIDataTypeIds[] = {g_nStringIdBOOL};

const TForteInt16 E_F_TRIG::scm_anEIWithIndexes[] = {0};
const TDataIOID E_F_TRIG::scm_anEIWith[] = {0, 255};
const CStringDictionary::TStringId E_F_TRIG::scm_anEventInputNames[] = {g_nStringIdEI};

const CStringDictionary::TStringId E_F_TRIG::scm_anEventOutputNames[] = {g_nStringIdEO};

const SFBInterfaceSpec E_F_TRIG::scm_stFBInterfaceSpec = {
  1,
  scm_anEventInputNames,
  scm_anEIWith,
  scm_anEIWithIndexes,
  1,
  scm_anEventOutputNames,
  0,
  0,
  1,
  scm_anDataInputNames, scm_aunDIDataTypeIds,
  0,
  0,
  0,
  0,
  0
};

void E_F_TRIG::executeEvent(int pa_nEIID){
  if(scm_nEventEIID == pa_nEIID){
    if((m_bOldVal != QI()) && (false == QI())){
      //Falling edge
      sendOutputEvent(scm_nEventEOID);
    }
    m_bOldVal = QI();
  }
}

#else

DEFINE_FIRMWARE_FB(E_F_TRIG, g_nStringIdE_F_TRIG)

const CStringDictionary::TStringId E_F_TRIG::scm_anDataInputNames[] = {g_nStringIdQI};

const CStringDictionary::TStringId E_F_TRIG::scm_anDataInputTypeIds[] = {g_nStringIdBOOL};

const TForteInt16 E_F_TRIG::scm_anEIWithIndexes[] = {0};
const TDataIOID E_F_TRIG::scm_anEIWith[] = {0, 255};
const CStringDictionary::TStringId E_F_TRIG::scm_anEventInputNames[] = {g_nStringIdEI};

const TForteInt16 E_F_TRIG::scm_anEOWithIndexes[] = {-1, -1};
const CStringDictionary::TStringId E_F_TRIG::scm_anEventOutputNames[] = {g_nStringIdEO};

const SFBInterfaceSpec E_F_TRIG::scm_stFBInterfaceSpec = {
  1,  scm_anEventInputNames,  scm_anEIWith,  scm_anEIWithIndexes,
  1,  scm_anEventOutputNames,  0, 0,  1,  scm_anDataInputNames, scm_anDataInputTypeIds,
  0,  0, 0,
  0, 0
};


const SCFB_FBInstanceData E_F_TRIG::scm_astInternalFBs[] = {
  {g_nStringIdD, g_nStringIdE_D_FF},
  {g_nStringIdSW, g_nStringIdE_SWITCH},
};

const SCFB_FBConnectionData E_F_TRIG::scm_astEventConnections[] = {
  {GENERATE_CONNECTION_PORT_ID_1_ARG(g_nStringIdEI), -1, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdD, g_nStringIdCLK), 0},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdD, g_nStringIdEO), 0, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdSW, g_nStringIdEI), 1},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdSW, g_nStringIdEO0), 1, GENERATE_CONNECTION_PORT_ID_1_ARG(g_nStringIdEO), -1},
};

const SCFB_FBConnectionData E_F_TRIG::scm_astDataConnections[] = {
  {GENERATE_CONNECTION_PORT_ID_1_ARG(g_nStringIdQI), -1, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdD, g_nStringIdD), 0},
  {GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdD, g_nStringIdQ), 0, GENERATE_CONNECTION_PORT_ID_2_ARG(g_nStringIdSW, g_nStringIdG), 1},
};

const SCFB_FBNData E_F_TRIG::scm_stFBNData = {
  2, scm_astInternalFBs,
  3, scm_astEventConnections,
  0, 0,
  2, scm_astDataConnections,
  0, 0,
  0, 0
};




#endif



