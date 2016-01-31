/*******************************************************************************
 * Copyright (c) 2012, 2013 ACIN, fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Alois Zoitl
 *    - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "SERVER_1_2.h"

DEFINE_FIRMWARE_FB(SERVER_1_2, g_nStringIdSERVER_1_2)

const CStringDictionary::TStringId SERVER_1_2::scm_anDataInputNames[] = {g_nStringIdQI, g_nStringIdID, g_nStringIdSD_1};

const CStringDictionary::TStringId SERVER_1_2::scm_anDataOutputNames[] = {g_nStringIdQO, g_nStringIdSTATUS, g_nStringIdRD_1, g_nStringIdRD_2};
const CStringDictionary::TStringId SERVER_1_2::scm_aunDIDataTypeIds[] = {g_nStringIdBOOL, g_nStringIdSTRING, g_nStringIdANY};
const CStringDictionary::TStringId SERVER_1_2::scm_aunDODataTypeIds[] = {g_nStringIdBOOL, g_nStringIdSTRING, g_nStringIdANY, g_nStringIdANY};

const TForteInt16 SERVER_1_2::scm_anEIWithIndexes[] = {0, 3};
const TDataIOID SERVER_1_2::scm_anEIWith[] = {0, 1, 255, 0, 2, 3, 255};
const CStringDictionary::TStringId SERVER_1_2::scm_anEventInputNames[] = {g_nStringIdINIT, g_nStringIdRSP};

const TDataIOID SERVER_1_2::scm_anEOWith[] = {0, 1, 255, 0, 1, 2, 3, 255};
const TForteInt16 SERVER_1_2::scm_anEOWithIndexes[] = {0, 3};
const CStringDictionary::TStringId SERVER_1_2::scm_anEventOutputNames[] = {g_nStringIdINITO, g_nStringIdIND};

const SFBInterfaceSpec SERVER_1_2::scm_stFBInterfaceSpec = {
  2,
  scm_anEventInputNames,
  scm_anEIWith,
  scm_anEIWithIndexes,
  2,
  scm_anEventOutputNames,
  scm_anEOWith,
  scm_anEOWithIndexes,
  3,
  scm_anDataInputNames, scm_aunDIDataTypeIds,
  4,
  scm_anDataOutputNames, scm_aunDODataTypeIds,
  0,
  0
};


SERVER_1_2::SERVER_1_2(CStringDictionary::TStringId pa_nInstanceNameId, CResource *pa_poSrcRes) :
  CCommFB(pa_nInstanceNameId, pa_poSrcRes, &scm_stFBInterfaceSpec, m_anFBConnData, m_anFBVarsData, forte::com_infra::e_Server){
}

SERVER_1_2::~SERVER_1_2(){

}

