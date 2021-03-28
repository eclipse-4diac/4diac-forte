/*******************************************************************************
 * Copyright (c) 2014 Profactor GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Matthias Plasch
 *   - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "GET_AT_INDEX.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "GET_AT_INDEX_gen.cpp"
#endif

DEFINE_FIRMWARE_FB(FORTE_GET_AT_INDEX, g_nStringIdGET_AT_INDEX)

  const CStringDictionary::TStringId FORTE_GET_AT_INDEX::scm_anDataInputNames[] = {g_nStringIdIN_ARRAY, g_nStringIdINDEX};

const CStringDictionary::TStringId FORTE_GET_AT_INDEX::scm_anDataInputTypeIds[] = {g_nStringIdANY, g_nStringIdUINT};

const CStringDictionary::TStringId FORTE_GET_AT_INDEX::scm_anDataOutputNames[] = {g_nStringIdQO, g_nStringIdOUT};

const CStringDictionary::TStringId FORTE_GET_AT_INDEX::scm_anDataOutputTypeIds[] = {g_nStringIdBOOL, g_nStringIdANY};

const TForteInt16 FORTE_GET_AT_INDEX::scm_anEIWithIndexes[] = {0};
const TDataIOID FORTE_GET_AT_INDEX::scm_anEIWith[] = {0, 1, 255};
const CStringDictionary::TStringId FORTE_GET_AT_INDEX::scm_anEventInputNames[] = {g_nStringIdREQ};

const TDataIOID FORTE_GET_AT_INDEX::scm_anEOWith[] = {0, 1, 255};
const TForteInt16 FORTE_GET_AT_INDEX::scm_anEOWithIndexes[] = {0, -1};
const CStringDictionary::TStringId FORTE_GET_AT_INDEX::scm_anEventOutputNames[] = {g_nStringIdCNF};

const SFBInterfaceSpec FORTE_GET_AT_INDEX::scm_stFBInterfaceSpec = {
  1,  scm_anEventInputNames,  scm_anEIWith,  scm_anEIWithIndexes,
  1,  scm_anEventOutputNames,  scm_anEOWith, scm_anEOWithIndexes,  2,  scm_anDataInputNames, scm_anDataInputTypeIds,
  2,  scm_anDataOutputNames, scm_anDataOutputTypeIds,
  0, 0
};


void FORTE_GET_AT_INDEX::executeEvent(int pa_nEIID){
  switch(pa_nEIID){
  case scm_nEventREQID:

    if(CIEC_ANY::e_ARRAY == IN_ARRAY().getDataTypeID()){
      CIEC_ARRAY &rInArray = static_cast<CIEC_ARRAY&>(IN_ARRAY());
      //check if data types match
      if(rInArray.getElementDataTypeID() == st_OUT().getDataTypeID()){
        //now check array length of input array
        if(INDEX() < rInArray.size()){
          //update the output value
          st_OUT().setValue(*rInArray[INDEX()]);
          QO() = true;
        }
        else{
          DEVLOG_DEBUG("Access index out of range.\n");
          QO() = false;
        }
      }
      else{
        DEVLOG_DEBUG("Inequal element data types.\n");
        QO() = false;
      }
    }
    else{
      DEVLOG_DEBUG("No 'Array' typed input and output data.\n");
      QO() = false;
    }
    sendOutputEvent(scm_nEventCNFID);

    break;
  }
}



