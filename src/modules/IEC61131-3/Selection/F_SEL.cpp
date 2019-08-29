/*******************************************************************************
 * Copyright (c) 2011 - 2013 ACIN, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Monika Wenger
 *   - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "F_SEL.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "F_SEL_gen.cpp"
#endif
#include <anyhelper.h>

DEFINE_FIRMWARE_FB(FORTE_F_SEL, g_nStringIdF_SEL)

const CStringDictionary::TStringId FORTE_F_SEL::scm_anDataInputNames[] = {g_nStringIdG, g_nStringIdIN0, g_nStringIdIN1};

const CStringDictionary::TStringId FORTE_F_SEL::scm_anDataInputTypeIds[] = {g_nStringIdBOOL, g_nStringIdANY, g_nStringIdANY};

const CStringDictionary::TStringId FORTE_F_SEL::scm_anDataOutputNames[] = {g_nStringIdOUT};

const CStringDictionary::TStringId FORTE_F_SEL::scm_anDataOutputTypeIds[] = {g_nStringIdANY};

const TForteInt16 FORTE_F_SEL::scm_anEIWithIndexes[] = {0};
const TDataIOID FORTE_F_SEL::scm_anEIWith[] = {0, 2, 1, 255};
const CStringDictionary::TStringId FORTE_F_SEL::scm_anEventInputNames[] = {g_nStringIdREQ};

const TDataIOID FORTE_F_SEL::scm_anEOWith[] = {0, 255};
const TForteInt16 FORTE_F_SEL::scm_anEOWithIndexes[] = {0, -1};
const CStringDictionary::TStringId FORTE_F_SEL::scm_anEventOutputNames[] = {g_nStringIdCNF};

const SFBInterfaceSpec FORTE_F_SEL::scm_stFBInterfaceSpec = {
  1,  scm_anEventInputNames,  scm_anEIWith,  scm_anEIWithIndexes,
  1,  scm_anEventOutputNames,  scm_anEOWith, scm_anEOWithIndexes,  3,  scm_anDataInputNames, scm_anDataInputTypeIds,
  1,  scm_anDataOutputNames, scm_anDataOutputTypeIds,
  0, 0
};


void FORTE_F_SEL::executeEvent(int pa_nEIID){
  if (scm_nEventREQID == pa_nEIID) {
       anyElementaryFBHelper<FORTE_F_SEL>(IN0().getDataTypeID(), *this);
       sendOutputEvent(scm_nEventCNFID);
   }
}



