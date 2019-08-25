/*******************************************************************************
 * Copyright (c) 2005 - 2013 ACIN, Profactor GmbH, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Rene Smodic, Gerhard Ebenhofer, Ingo Hegny
 *     - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "E_SR.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "E_SR_gen.cpp"
#endif

DEFINE_FIRMWARE_FB(E_SR, g_nStringIdE_SR)

const CStringDictionary::TStringId E_SR::scm_aunDataOutputNameIds[] = {g_nStringIdQ};

const CStringDictionary::TStringId E_SR::scm_aunEventInputNameIds[] = {g_nStringIdS, g_nStringIdR};

const TDataIOID E_SR::scm_anEOWith[] = {0, 255};
const TForteInt16 E_SR::scm_anEOWithIndexes[] = {0};
const CStringDictionary::TStringId E_SR::scm_aunEventOutputNameIds[] = {g_nStringIdEO};

const CStringDictionary::TStringId E_SR::scm_aunDataOutputDataTypeIds[] = {g_nStringIdBOOL};

const SFBInterfaceSpec E_SR::scm_stFBInterfaceSpec = {
  2,
  scm_aunEventInputNameIds,
  0,
  0,
  1,
  scm_aunEventOutputNameIds,
  scm_anEOWith,
  scm_anEOWithIndexes,
  0,
  0,
  0,
  1,
  scm_aunDataOutputNameIds,
  scm_aunDataOutputDataTypeIds,
  0,
  0
};

void E_SR::executeEvent(int pa_nEIID){
  switch(pa_nEIID){
    case scm_nEventSID:
      if(false == Q()){
        Q() = true;
        sendOutputEvent( scm_nEventEOID);
      }
      break;
    case scm_nEventRID:
      if(true == Q()){
        Q() = false;
        sendOutputEvent( scm_nEventEOID);
      }
      break;
    default:
      break;
  }
}
