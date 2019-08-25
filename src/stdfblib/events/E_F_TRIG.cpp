/*******************************************************************************
 * Copyright (c) 2007 - 2013 ACIN, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Ingo Hegny
 *     - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "E_F_TRIG.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "E_F_TRIG_gen.cpp"
#endif
#include <typelib.h>

DEFINE_FIRMWARE_FB(E_F_TRIG, g_nStringIdE_F_TRIG)

const CStringDictionary::TStringId E_F_TRIG::scm_anDataInputNames[] = {g_nStringIdQI};
const CStringDictionary::TStringId E_F_TRIG::scm_aunDIDataTypeIds[] = {g_nStringIdBOOL};

const TForteInt16 E_F_TRIG::scm_anEIWithIndexes[] = {0};
const TDataIOID E_F_TRIG::scm_anEIWith[] = {0, 255};
const CStringDictionary::TStringId E_F_TRIG::scm_anEventInputNames[] = {g_nStringIdEI};

const CStringDictionary::TStringId E_F_TRIG::scm_anEventOutputNames[] = {g_nStringIdEO};

const SFBInterfaceSpec E_F_TRIG::scm_stFBInterfaceSpec = {
  1,
  scm_anEventInputNames,
  scm_anEIWith,
  scm_anEIWithIndexes,
  1,
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

void E_F_TRIG::executeEvent(int pa_nEIID){
  if(scm_nEventEIID == pa_nEIID){
    if((m_bOldVal != QI()) && (false == QI())){
      //Falling edge
      sendOutputEvent(scm_nEventEOID);
    }
    m_bOldVal = QI();
  }
}
