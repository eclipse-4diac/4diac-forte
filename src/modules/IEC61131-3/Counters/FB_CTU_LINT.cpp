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
#include "FB_CTU_LINT.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "FB_CTU_LINT_gen.cpp"
#endif

#ifdef FORTE_USE_64BIT_DATATYPES

DEFINE_FIRMWARE_FB(FB_CTU_LINT, g_nStringIdFB_CTU_LINT)

const CStringDictionary::TStringId FB_CTU_LINT::scm_anDataInputNames[] = {g_nStringIdCU, g_nStringIdR, g_nStringIdPV};

const CStringDictionary::TStringId FB_CTU_LINT::scm_anDataOutputNames[] = {g_nStringIdQ, g_nStringIdCV};
const CStringDictionary::TStringId FB_CTU_LINT::scm_aunDIDataTypeIds[] = {g_nStringIdBOOL, g_nStringIdBOOL, g_nStringIdLINT};
const CStringDictionary::TStringId FB_CTU_LINT::scm_aunDODataTypeIds[] = {g_nStringIdBOOL, g_nStringIdLINT};

const TForteInt16 FB_CTU_LINT::scm_anEIWithIndexes[] = {0};
const TDataIOID FB_CTU_LINT::scm_anEIWith[] = {0, 2, 1, 255};
const CStringDictionary::TStringId FB_CTU_LINT::scm_anEventInputNames[] = {g_nStringIdREQ};

const TDataIOID FB_CTU_LINT::scm_anEOWith[] = {0, 1, 255};
const TForteInt16 FB_CTU_LINT::scm_anEOWithIndexes[] = {0};
const CStringDictionary::TStringId FB_CTU_LINT::scm_anEventOutputNames[] = {g_nStringIdCNF};

const SFBInterfaceSpec FB_CTU_LINT::scm_stFBInterfaceSpec = {
  1,
  scm_anEventInputNames,
  scm_anEIWith,
  scm_anEIWithIndexes,
  1,
  scm_anEventOutputNames,
  scm_anEOWith,
  scm_anEOWithIndexes,
  3,
  scm_anDataInputNames, scm_aunDIDataTypeIds,
  2,
  scm_anDataOutputNames, scm_aunDODataTypeIds,
  0,
  0
};

void FB_CTU_LINT::executeEvent(int pa_nEIID){
  if(pa_nEIID == scm_nEventREQID){
      if(true == R()){
        CV() = 0;
      }
      else if( (true == CU()) && (CV() < CIEC_LINT::scm_nMaxVal)){
        CV() = CV() + 1;
      }
      Q() = (CV() >= PV());
      sendOutputEvent(scm_nEventCNFID);
  }
}

#endif
