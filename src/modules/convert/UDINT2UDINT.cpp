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
#include "UDINT2UDINT.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "UDINT2UDINT_gen.cpp"
#endif

DEFINE_FIRMWARE_FB(UDINT2UDINT, g_nStringIdUDINT2UDINT)

const CStringDictionary::TStringId UDINT2UDINT::scm_anDataInputNames[] = {g_nStringIdIN};

const CStringDictionary::TStringId UDINT2UDINT::scm_anDataOutputNames[] = {g_nStringIdOUT};
const CStringDictionary::TStringId UDINT2UDINT::scm_aunDIDataTypeIds[] = {g_nStringIdUDINT};
const CStringDictionary::TStringId UDINT2UDINT::scm_aunDODataTypeIds[] = {g_nStringIdUDINT};

const TForteInt16 UDINT2UDINT::scm_anEIWithIndexes[] = {0};
const TDataIOID UDINT2UDINT::scm_anEIWith[] = {0, 255};
const CStringDictionary::TStringId UDINT2UDINT::scm_anEventInputNames[] = {g_nStringIdREQ};

const TDataIOID UDINT2UDINT::scm_anEOWith[] = {0, 255};
const TForteInt16 UDINT2UDINT::scm_anEOWithIndexes[] = {0};
const CStringDictionary::TStringId UDINT2UDINT::scm_anEventOutputNames[] = {g_nStringIdCNF};

const SFBInterfaceSpec UDINT2UDINT::scm_stFBInterfaceSpec = {
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

void UDINT2UDINT::executeEvent(int pa_nEIID){
  if(scm_nEventREQID == pa_nEIID){
    st_OUT() = st_IN();
    sendOutputEvent(scm_nEventCNFID);
  }
}

UDINT2UDINT::~UDINT2UDINT(){
}
