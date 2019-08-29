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
#include "E_DEMUX.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "E_DEMUX_gen.cpp"
#endif

DEFINE_FIRMWARE_FB(E_DEMUX, g_nStringIdE_DEMUX)

const CStringDictionary::TStringId E_DEMUX::scm_anDataInputNames[] = {g_nStringIdK};
const CStringDictionary::TStringId E_DEMUX::scm_aunDIDataTypeIds[] = {g_nStringIdUINT};

const TForteInt16 E_DEMUX::scm_anEIWithIndexes[] = {0};
const TDataIOID E_DEMUX::scm_anEIWith[] = {0, 255};
const CStringDictionary::TStringId E_DEMUX::scm_anEventInputNames[] = {g_nStringIdEI};

const CStringDictionary::TStringId E_DEMUX::scm_anEventOutputNames[] = {g_nStringIdEO0, g_nStringIdEO1, g_nStringIdEO2, g_nStringIdEO3};

const SFBInterfaceSpec E_DEMUX::scm_stFBInterfaceSpec = {
  1,
  scm_anEventInputNames,
  scm_anEIWith,
  scm_anEIWithIndexes,
  4,
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

void E_DEMUX::executeEvent(int pa_nEIID){
  if(scm_nEventEIID == pa_nEIID && K() <= 3) {
    sendOutputEvent(K());
    // the value of K corresponds to the outputevent ID;
    // as a result of this we could make a generic E_DEMUX
    // and even a generic E_MUX
  }
}
