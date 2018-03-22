/*******************************************************************************
 * Copyright (c) 2013 Profactor GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Matthias Plasch
 *   - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "F_REAL_TO_BOOL.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "F_REAL_TO_BOOL_gen.cpp"
#endif

DEFINE_FIRMWARE_FB(FORTE_F_REAL_TO_BOOL, g_nStringIdF_REAL_TO_BOOL)

const CStringDictionary::TStringId FORTE_F_REAL_TO_BOOL::scm_anDataInputNames[] = {g_nStringIdIN};

const CStringDictionary::TStringId FORTE_F_REAL_TO_BOOL::scm_anDataInputTypeIds[] = {g_nStringIdREAL};

const CStringDictionary::TStringId FORTE_F_REAL_TO_BOOL::scm_anDataOutputNames[] = {g_nStringIdOUT};

const CStringDictionary::TStringId FORTE_F_REAL_TO_BOOL::scm_anDataOutputTypeIds[] = {g_nStringIdBOOL};

const TForteInt16 FORTE_F_REAL_TO_BOOL::scm_anEIWithIndexes[] = {0};
const TDataIOID FORTE_F_REAL_TO_BOOL::scm_anEIWith[] = {0, 255};
const CStringDictionary::TStringId FORTE_F_REAL_TO_BOOL::scm_anEventInputNames[] = {g_nStringIdREQ};

const TDataIOID FORTE_F_REAL_TO_BOOL::scm_anEOWith[] = {0, 255};
const TForteInt16 FORTE_F_REAL_TO_BOOL::scm_anEOWithIndexes[] = {0, -1};
const CStringDictionary::TStringId FORTE_F_REAL_TO_BOOL::scm_anEventOutputNames[] = {g_nStringIdCNF};

const SFBInterfaceSpec FORTE_F_REAL_TO_BOOL::scm_stFBInterfaceSpec = {
  1,  scm_anEventInputNames,  scm_anEIWith,  scm_anEIWithIndexes,
  1,  scm_anEventOutputNames,  scm_anEOWith, scm_anEOWithIndexes,  1,  scm_anDataInputNames, scm_anDataInputTypeIds,
  1,  scm_anDataOutputNames, scm_anDataOutputTypeIds,
  0, 0
};


void FORTE_F_REAL_TO_BOOL::executeEvent(int pa_nEIID){
  if(scm_nEventREQID == pa_nEIID){
    OUT() = REAL_TO_BOOL(IN());
    sendOutputEvent(scm_nEventCNFID);
  }
}



