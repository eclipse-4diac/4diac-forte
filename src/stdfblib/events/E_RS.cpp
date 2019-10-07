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
#include "E_RS.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "E_RS_gen.cpp"
#endif

DEFINE_FIRMWARE_FB(E_RS, g_nStringIdE_RS)

const CStringDictionary::TStringId E_RS::scm_anDataOutputNames[] = {g_nStringIdQ};
const CStringDictionary::TStringId E_RS::scm_aunDODataTypeIds[] = {g_nStringIdBOOL};

const CStringDictionary::TStringId E_RS::scm_anEventInputNames[] = {g_nStringIdR, g_nStringIdS};

const TDataIOID E_RS::scm_anEOWith[] = {0, 255};
const TForteInt16 E_RS::scm_anEOWithIndexes[] = {0};
const CStringDictionary::TStringId E_RS::scm_anEventOutputNames[] = {g_nStringIdEO};

const SFBInterfaceSpec E_RS::scm_stFBInterfaceSpec = {
  2,
  scm_anEventInputNames,
  0,
  0,
  1,
  scm_anEventOutputNames,
  scm_anEOWith,
  scm_anEOWithIndexes,
  0,
  0,
  0,
  1,
  scm_anDataOutputNames, scm_aunDODataTypeIds,
  0,
  0
};

void E_RS::executeEvent(int pa_nEIID){
  switch(pa_nEIID){
    case scm_nEventRID:
      if(true == Q()){
        Q() = false;
        sendOutputEvent( scm_nEventEOID);
      }
      break;
    case scm_nEventSID:
      if(false == Q()){
        Q() = true;
        sendOutputEvent( scm_nEventEOID);
      }
      break;
    default:
      break;
  }
}
