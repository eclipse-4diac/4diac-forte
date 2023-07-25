/*************************************************************************
 * Copyright (c) 2015 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Gerd Kainz - initial API and implementation and/or initial documentation
 *************************************************************************/

#include "IW.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "IW_gen.cpp"
#endif

DEFINE_FIRMWARE_FB(FORTE_IW, g_nStringIdIW)

const CStringDictionary::TStringId FORTE_IW::scm_anDataInputNames[] = {g_nStringIdQI, g_nStringIdPARAMS};

const CStringDictionary::TStringId FORTE_IW::scm_anDataInputTypeIds[] = {g_nStringIdBOOL, g_nStringIdSTRING};

const CStringDictionary::TStringId FORTE_IW::scm_anDataOutputNames[] = {g_nStringIdQO, g_nStringIdSTATUS, g_nStringIdIN};

const CStringDictionary::TStringId FORTE_IW::scm_anDataOutputTypeIds[] = {g_nStringIdBOOL, g_nStringIdSTRING, g_nStringIdWORD};

const TForteInt16 FORTE_IW::scm_anEIWithIndexes[] = {0, 3};
const TDataIOID FORTE_IW::scm_anEIWith[] = {0, 1, scmWithListDelimiter, 0, scmWithListDelimiter};
const CStringDictionary::TStringId FORTE_IW::scm_anEventInputNames[] = {g_nStringIdINIT, g_nStringIdREQ};

const TDataIOID FORTE_IW::scm_anEOWith[] = {0, 1, scmWithListDelimiter, 0, 1, 2, scmWithListDelimiter, 0, 1, 2, scmWithListDelimiter};
const TForteInt16 FORTE_IW::scm_anEOWithIndexes[] = {0, 3, 7, -1};
const CStringDictionary::TStringId FORTE_IW::scm_anEventOutputNames[] = {g_nStringIdINITO, g_nStringIdCNF, g_nStringIdIND};

const SFBInterfaceSpec FORTE_IW::scm_stFBInterfaceSpec = {
  2, scm_anEventInputNames, scm_anEIWith, scm_anEIWithIndexes,
  3, scm_anEventOutputNames, scm_anEOWith, scm_anEOWithIndexes,
  2, scm_anDataInputNames, scm_anDataInputTypeIds,
  3, scm_anDataOutputNames, scm_anDataOutputTypeIds,
  0, nullptr,
  0, nullptr
};


void FORTE_IW::executeEvent(TEventID pa_nEIID){
  QO() = QI();
  switch(pa_nEIID){
    case cg_nExternalEventID:
      sendOutputEvent(scm_nEventINDID);
      break;
    case scm_nEventINITID:
      if(true == QI()){
        QO() = CIEC_BOOL(CProcessInterface::initialise(true)); //initialise as input
      }
      else{
        QO() = CIEC_BOOL(CProcessInterface::deinitialise());
      }
      sendOutputEvent(scm_nEventINITOID);
      break;
    case scm_nEventREQID:
      if(true == QI()){
        QO() = CIEC_BOOL(CProcessInterface::readWord());
      }
      sendOutputEvent(scm_nEventCNFID);
      break;
  }
}


