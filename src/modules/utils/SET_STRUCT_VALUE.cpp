/*******************************************************************************
 * Copyright (c) 2018, 2023 fortiss GmbH, Primetals Technologies Austria GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Jose Cabral - initial implementation
 *   Alois Zoitl - adapted this FB from the code in GET_STRUCT_VALUE
 *******************************************************************************/

#include "SET_STRUCT_VALUE.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "SET_STRUCT_VALUE_gen.cpp"
#endif

#include "GET_STRUCT_VALUE.h"

DEFINE_FIRMWARE_FB(FORTE_SET_STRUCT_VALUE, g_nStringIdSET_STRUCT_VALUE)

const CStringDictionary::TStringId FORTE_SET_STRUCT_VALUE::scm_anDataInputNames[] = {g_nStringIdin_struct, g_nStringIdmember, g_nStringIdelement_value};

const CStringDictionary::TStringId FORTE_SET_STRUCT_VALUE::scm_anDataInputTypeIds[] = {g_nStringIdANY, g_nStringIdSTRING, g_nStringIdANY};

const CStringDictionary::TStringId FORTE_SET_STRUCT_VALUE::scm_anDataOutputNames[] = {g_nStringIdout_struct};

const CStringDictionary::TStringId FORTE_SET_STRUCT_VALUE::scm_anDataOutputTypeIds[] = {g_nStringIdANY};

const TDataIOID FORTE_SET_STRUCT_VALUE::scm_anEIWith[] = {1, 0, 255};
const TForteInt16 FORTE_SET_STRUCT_VALUE::scm_anEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_SET_STRUCT_VALUE::scm_anEventInputNames[] = {g_nStringIdREQ};

const TDataIOID FORTE_SET_STRUCT_VALUE::scm_anEOWith[] = {0, 255};
const TForteInt16 FORTE_SET_STRUCT_VALUE::scm_anEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_SET_STRUCT_VALUE::scm_anEventOutputNames[] = {g_nStringIdCNF};


const SFBInterfaceSpec FORTE_SET_STRUCT_VALUE::scm_stFBInterfaceSpec = {
  1, scm_anEventInputNames, scm_anEIWith, scm_anEIWithIndexes,
  1, scm_anEventOutputNames, scm_anEOWith, scm_anEOWithIndexes,
  3, scm_anDataInputNames, scm_anDataInputTypeIds,
  1, scm_anDataOutputNames, scm_anDataOutputTypeIds,
  0, nullptr
};

void FORTE_SET_STRUCT_VALUE::executeEvent(int paEIID) {
  if(scm_nEventREQID == paEIID){
    if(checkStructPorts()){
      st_out_struct().setValue(st_in_struct());
      CIEC_STRING copyOfMember = st_member();
      CIEC_ANY *foundMember = FORTE_GET_STRUCT_VALUE::lookForMember(static_cast<CIEC_STRUCT&>(st_out_struct()), copyOfMember.getValue());
      if(nullptr != foundMember){
        if(foundMember->getDataTypeID() == st_element_value().getDataTypeID()){
          foundMember->setValue(st_element_value());
        }
        else{
          DEVLOG_ERROR("[SET_STRUCT_VALUE]: In instance %s, the member %s was found but it doesn't match the given type %d\n", getInstanceName(), st_member().getValue(), st_element_value().getDataTypeID());
        }
      } else {
        DEVLOG_ERROR("[SET_STRUCT_VALUE]: In instance %s, member %s was not found\n", getInstanceName(), st_member().getValue());
      }
    }
    sendOutputEvent(scm_nEventCNFID);
  }
}

bool FORTE_SET_STRUCT_VALUE::checkStructPorts(){
  if(CIEC_ANY::e_STRUCT == st_in_struct().getDataTypeID()){
    if(CIEC_ANY::e_STRUCT == st_out_struct().getDataTypeID()){
      return true;
    }
    else{
      DEVLOG_ERROR("[SET_STRUCT_VALUE]: In instance %s, the output structure is not of type structure but of type %d\n", getInstanceName(), st_out_struct().getDataTypeID());
    }
  }
  else{
    DEVLOG_ERROR("[SET_STRUCT_VALUE]: In instance %s, the input structure is not of type structure but of type %d\n", getInstanceName(), st_in_struct().getDataTypeID());
  }
  return false;
}


