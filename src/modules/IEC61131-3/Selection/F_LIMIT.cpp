/*******************************************************************************
 * Copyright (c) 2011 - 2013 ACIN, Profactor GmbH, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Monika Wenger, Alois Zoitl, Gerhard Ebenhofer
 *   - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "F_LIMIT.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "F_LIMIT_gen.cpp"
#endif
#include <anyhelper.h>

DEFINE_FIRMWARE_FB(FORTE_F_LIMIT, g_nStringIdF_LIMIT)

const CStringDictionary::TStringId FORTE_F_LIMIT::scm_anDataInputNames[] = {
    g_nStringIdMN, g_nStringIdIN, g_nStringIdMX };

const CStringDictionary::TStringId FORTE_F_LIMIT::scm_anDataInputTypeIds[] = {
    g_nStringIdANY_ELEMENTARY, g_nStringIdANY_ELEMENTARY,
    g_nStringIdANY_ELEMENTARY };

const CStringDictionary::TStringId FORTE_F_LIMIT::scm_anDataOutputNames[] = {
    g_nStringIdOUT };

const CStringDictionary::TStringId FORTE_F_LIMIT::scm_anDataOutputTypeIds[] = {
    g_nStringIdANY_ELEMENTARY };

const TForteInt16 FORTE_F_LIMIT::scm_anEIWithIndexes[] = { 0 };
const TDataIOID FORTE_F_LIMIT::scm_anEIWith[] = { 0, 2, 1, 255 };
const CStringDictionary::TStringId FORTE_F_LIMIT::scm_anEventInputNames[] = {
    g_nStringIdREQ };

const TDataIOID FORTE_F_LIMIT::scm_anEOWith[] = { 0, 255 };
const TForteInt16 FORTE_F_LIMIT::scm_anEOWithIndexes[] = { 0, -1 };
const CStringDictionary::TStringId FORTE_F_LIMIT::scm_anEventOutputNames[] = {
    g_nStringIdCNF };

const SFBInterfaceSpec FORTE_F_LIMIT::scm_stFBInterfaceSpec = { 1,
    scm_anEventInputNames, scm_anEIWith, scm_anEIWithIndexes, 1,
    scm_anEventOutputNames, scm_anEOWith, scm_anEOWithIndexes, 3,
    scm_anDataInputNames, scm_anDataInputTypeIds, 1, scm_anDataOutputNames,
    scm_anDataOutputTypeIds, 0, 0 };

void FORTE_F_LIMIT::executeEvent(int pa_nEIID) {
  if (scm_nEventREQID == pa_nEIID) {
      anyElementaryFBHelper<FORTE_F_LIMIT>(st_IN().getDataTypeID(), *this);
      sendOutputEvent(scm_nEventCNFID);
  }
}

#ifdef FORTE_USE_WSTRING_DATATYPE
template<>
void FORTE_F_LIMIT::calculateValue<CIEC_WSTRING>(){
  //TODO fill this function
}
#endif
