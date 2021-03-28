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
#include "WORD2WORD.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "WORD2WORD_gen.cpp"
#endif

DEFINE_FIRMWARE_FB(WORD2WORD, g_nStringIdWORD2WORD)

const CStringDictionary::TStringId WORD2WORD::scm_anDataInputNames[] = {g_nStringIdIN};

const CStringDictionary::TStringId WORD2WORD::scm_anDataOutputNames[] = {g_nStringIdOUT};
const CStringDictionary::TStringId WORD2WORD::scm_aunDIDataTypeIds[] = {g_nStringIdWORD};
const CStringDictionary::TStringId WORD2WORD::scm_aunDODataTypeIds[] = {g_nStringIdWORD};

const TForteInt16 WORD2WORD::scm_anEIWithIndexes[] = {0};
const TDataIOID WORD2WORD::scm_anEIWith[] = {0, 255};
const CStringDictionary::TStringId WORD2WORD::scm_anEventInputNames[] = {g_nStringIdREQ};

const TDataIOID WORD2WORD::scm_anEOWith[] = {0, 255};
const TForteInt16 WORD2WORD::scm_anEOWithIndexes[] = {0};
const CStringDictionary::TStringId WORD2WORD::scm_anEventOutputNames[] = {g_nStringIdCNF};

const SFBInterfaceSpec WORD2WORD::scm_stFBInterfaceSpec = {
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

void WORD2WORD::executeEvent(int pa_nEIID){
  if(scm_nEventREQID == pa_nEIID){
    st_OUT() = st_IN();
    sendOutputEvent(scm_nEventCNFID);
  }
}

WORD2WORD::~WORD2WORD(){
}
