/*******************************************************************************
 * Copyright (c) 2006 - 2013 ACIN, Profactor GmbH, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Gerhard Ebenhofer, Martin Melik Merkumians, Ingo Hegny
 *     - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "E_SWITCH.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "E_SWITCH_gen.cpp"
#endif

DEFINE_FIRMWARE_FB(E_SWITCH, g_nStringIdE_SWITCH)

const CStringDictionary::TStringId E_SWITCH::scm_anDataInputNames[] = {g_nStringIdG};
const CStringDictionary::TStringId E_SWITCH::scm_aunDIDataTypeIds[] = {g_nStringIdBOOL};

const TForteInt16 E_SWITCH::scm_anEIWithIndexes[] = {0};
const TDataIOID E_SWITCH::scm_anEIWith[] = {0, 255};
const CStringDictionary::TStringId E_SWITCH::scm_anEventInputNames[] = {g_nStringIdEI};

const CStringDictionary::TStringId E_SWITCH::scm_anEventOutputNames[] = {g_nStringIdEO0, g_nStringIdEO1};

const SFBInterfaceSpec E_SWITCH::scm_stFBInterfaceSpec = {
  1,
  scm_anEventInputNames,
  scm_anEIWith,
  scm_anEIWithIndexes,
  2,
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

void E_SWITCH::executeEvent(int pa_nEIID){
  if(scm_nEventEIID == pa_nEIID){
    //sendOutputEvent( (true == G) ? scm_nEventEO1ID : scm_nEventEO0ID);
    if(true == G()){
      sendOutputEvent(scm_nEventEO1ID);
    }
    else{
      sendOutputEvent(scm_nEventEO0ID);
    }
  }
}
