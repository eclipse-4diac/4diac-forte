/*******************************************************************************
 * Copyright (c) 2006 - 2013 Profactor GmbH, ACIN
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Thomas Strasser, Alois Zoitl, Gerhard Ebenhofer, Ingo Hegny,
 *   - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "DWORD2DWORD.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "DWORD2DWORD_gen.cpp"
#endif

DEFINE_FIRMWARE_FB(DWORD2DWORD, g_nStringIdDWORD2DWORD)


const CStringDictionary::TStringId DWORD2DWORD::scm_anDataInputNames[] = {g_nStringIdIN};

const CStringDictionary::TStringId DWORD2DWORD::scm_anDataOutputNames[] = {g_nStringIdOUT};
const CStringDictionary::TStringId DWORD2DWORD::scm_aunDIDataTypeIds[] = {g_nStringIdDWORD};
const CStringDictionary::TStringId DWORD2DWORD::scm_aunDODataTypeIds[] = {g_nStringIdDWORD};

const TForteInt16 DWORD2DWORD::scm_anEIWithIndexes[] = {0};
const TDataIOID DWORD2DWORD::scm_anEIWith[] = {0, 255};
const CStringDictionary::TStringId DWORD2DWORD::scm_anEventInputNames[] = {g_nStringIdREQ};

const TDataIOID DWORD2DWORD::scm_anEOWith[] = {0, 255};
const TForteInt16 DWORD2DWORD::scm_anEOWithIndexes[] = {0};
const CStringDictionary::TStringId DWORD2DWORD::scm_anEventOutputNames[] = {g_nStringIdCNF};

const SFBInterfaceSpec DWORD2DWORD::scm_stFBInterfaceSpec = {
  1,
  scm_anEventInputNames,
  scm_anEIWith,
  scm_anEIWithIndexes,
  1,
  scm_anEventOutputNames,
  scm_anEOWith,
  scm_anEOWithIndexes,
  1,
  scm_anDataInputNames, scm_aunDIDataTypeIds,
  1,
  scm_anDataOutputNames, scm_aunDODataTypeIds,
  0,
  0
};

void DWORD2DWORD::executeEvent(int pa_nEIID){
  if(scm_nEventREQID == pa_nEIID){
    st_OUT() = st_IN();
    sendOutputEvent(scm_nEventCNFID);
  }
}

DWORD2DWORD::~DWORD2DWORD(){
}
