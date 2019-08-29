/*******************************************************************************
 * Copyright (c) 2014 - 2015 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Waldemar Eisenmenger, Monika Wenger - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "IX.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "IX_gen.cpp"
#endif

DEFINE_FIRMWARE_FB(FORTE_IX, g_nStringIdIX)

const CStringDictionary::TStringId FORTE_IX::scm_anDataInputNames[] = {g_nStringIdQI, g_nStringIdPARAMS};

const CStringDictionary::TStringId FORTE_IX::scm_anDataInputTypeIds[] = {g_nStringIdBOOL, g_nStringIdSTRING};

const CStringDictionary::TStringId FORTE_IX::scm_anDataOutputNames[] = {g_nStringIdQO, g_nStringIdSTATUS, g_nStringIdIN};

const CStringDictionary::TStringId FORTE_IX::scm_anDataOutputTypeIds[] = {g_nStringIdBOOL, g_nStringIdSTRING, g_nStringIdBOOL};

const TForteInt16 FORTE_IX::scm_anEIWithIndexes[] = {0, 3};
const TDataIOID FORTE_IX::scm_anEIWith[] = {0, 1, 255, 0, 255};
const CStringDictionary::TStringId FORTE_IX::scm_anEventInputNames[] = {g_nStringIdINIT, g_nStringIdREQ};

const TDataIOID FORTE_IX::scm_anEOWith[] = {0, 1, 255, 0, 1, 2, 255, 0, 1, 2, 255};
const TForteInt16 FORTE_IX::scm_anEOWithIndexes[] = {0, 3, 7, -1};
const CStringDictionary::TStringId FORTE_IX::scm_anEventOutputNames[] = {g_nStringIdINITO, g_nStringIdCNF, g_nStringIdIND};

const SFBInterfaceSpec FORTE_IX::scm_stFBInterfaceSpec = {
  2,  scm_anEventInputNames,  scm_anEIWith,  scm_anEIWithIndexes,
  3,  scm_anEventOutputNames,  scm_anEOWith, scm_anEOWithIndexes,  2,  scm_anDataInputNames, scm_anDataInputTypeIds,
  3,  scm_anDataOutputNames, scm_anDataOutputTypeIds,
  0, 0
};


void FORTE_IX::executeEvent(int pa_nEIID){
  QO() = QI();
  switch(pa_nEIID){
    case cg_nExternalEventID:
      sendOutputEvent(scm_nEventINDID);
      break;
    case scm_nEventINITID:
      if(true == QI()){
        QO() = CProcessInterface::initialise(true);  //initialise as input
      }
      else{
        QO() = CProcessInterface::deinitialise();
      }
      sendOutputEvent(scm_nEventINITOID);
      break;
    case scm_nEventREQID:
      if(true == QI()){
        QO() = CProcessInterface::readPin();
      }
      sendOutputEvent(scm_nEventCNFID);
      break;
  }
}



