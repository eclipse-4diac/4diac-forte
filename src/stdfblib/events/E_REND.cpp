/*******************************************************************************
 * Copyright (c) 2006 - 2013 ACIN, Profactor GmbH, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Gerhard Ebenhofer, Ingo Hegny
 *     - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "E_REND.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "E_REND_gen.cpp"
#endif

DEFINE_FIRMWARE_FB(E_REND, g_nStringIdE_REND)

const CStringDictionary::TStringId E_REND::scm_anEventInputNames[] = {g_nStringIdEI1, g_nStringIdEI2, g_nStringIdR};

const CStringDictionary::TStringId E_REND::scm_anEventOutputNames[] = {g_nStringIdEO};

const SFBInterfaceSpec E_REND::scm_stFBInterfaceSpec = {
  3,
  scm_anEventInputNames,
  0,
  0,
  1,
  scm_anEventOutputNames,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0
};

void E_REND::executeEvent(int pa_nEIID){
  switch(pa_nEIID){
    case scm_nEventEI1ID:
      if(m_bE2Occ){
        m_bE2Occ = false;
        sendOutputEvent( scm_nEventEOID);
      }
      else{
        m_bE1Occ = true;
      }
      break;
    case scm_nEventEI2ID:
      if(m_bE1Occ){
        m_bE1Occ = false;
        sendOutputEvent( scm_nEventEOID);
      }
      else{
        m_bE2Occ = true;
      }
      break;
    case scm_nEventRID:
      m_bE1Occ = m_bE2Occ = false;
      break;
    default:
      break;
  }
}
