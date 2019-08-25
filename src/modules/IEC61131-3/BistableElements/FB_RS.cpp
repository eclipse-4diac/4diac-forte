/*******************************************************************************
 * Copyright (c) 2009 - 2013 ACIN, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Monika Wenger, Alois Zoitl, Ingo Hengy
 *   - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "FB_RS.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "FB_RS_gen.cpp"
#endif

DEFINE_FIRMWARE_FB(FB_RS, g_nStringIdFB_RS)

const CStringDictionary::TStringId FB_RS::scm_anDataInputNames[] = {g_nStringIdS, g_nStringIdR1};

const CStringDictionary::TStringId FB_RS::scm_anDataOutputNames[] = {g_nStringIdQ1};
const CStringDictionary::TStringId FB_RS::scm_aunDIDataTypeIds[] = {g_nStringIdBOOL, g_nStringIdBOOL};
const CStringDictionary::TStringId FB_RS::scm_aunDODataTypeIds[] = {g_nStringIdBOOL};

const TForteInt16 FB_RS::scm_anEIWithIndexes[] = {0};
const TDataIOID FB_RS::scm_anEIWith[] = {0, 1, 255};
const CStringDictionary::TStringId FB_RS::scm_anEventInputNames[] = {g_nStringIdREQ};

const TDataIOID FB_RS::scm_anEOWith[] = {0, 255};
const TForteInt16 FB_RS::scm_anEOWithIndexes[] = {0};
const CStringDictionary::TStringId FB_RS::scm_anEventOutputNames[] = {g_nStringIdCNF};

const SFBInterfaceSpec FB_RS::scm_stFBInterfaceSpec = {
    1,
    scm_anEventInputNames,
    scm_anEIWith,
    scm_anEIWithIndexes,
    1,
    scm_anEventOutputNames,
    scm_anEOWith,
    scm_anEOWithIndexes,
    2,
    scm_anDataInputNames, scm_aunDIDataTypeIds,
    1,
    scm_anDataOutputNames, scm_aunDODataTypeIds,
    0,
    0
};

void FB_RS::executeEvent(int pa_nEIID){
  switch(pa_nEIID){
    case scm_nEventREQID:
      Q1() = !R1() && (S() || Q1());
      sendOutputEvent(scm_nEventCNFID);
      break;
  }
}
