/*******************************************************************************
 * Copyright (c) 2018, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Jose Cabral - initial implementation
 *******************************************************************************/

#include "GET_STRUCT_VALUE.h"

#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "GET_STRUCT_VALUE_gen.cpp"
#endif

#include <arch/devlog.h>

DEFINE_FIRMWARE_FB(FORTE_GET_STRUCT_VALUE, g_nStringIdGET_STRUCT_VALUE)

const CStringDictionary::TStringId FORTE_GET_STRUCT_VALUE::scm_anDataInputNames[] = { g_nStringIdin_struct, g_nStringIdmember };
const CStringDictionary::TStringId FORTE_GET_STRUCT_VALUE::scm_anDataInputTypeIds[] = { g_nStringIdANY, g_nStringIdSTRING };
const CStringDictionary::TStringId FORTE_GET_STRUCT_VALUE::scm_anDataOutputNames[] = { g_nStringIdQO, g_nStringIdoutput };
const CStringDictionary::TStringId FORTE_GET_STRUCT_VALUE::scm_anDataOutputTypeIds[] = { g_nStringIdBOOL, g_nStringIdANY };
const TForteInt16 FORTE_GET_STRUCT_VALUE::scm_anEIWithIndexes[] = { 0 };
const TDataIOID FORTE_GET_STRUCT_VALUE::scm_anEIWith[] = { 1, 0, 255 };
const CStringDictionary::TStringId FORTE_GET_STRUCT_VALUE::scm_anEventInputNames[] = { g_nStringIdREQ };
const TDataIOID FORTE_GET_STRUCT_VALUE::scm_anEOWith[] = { 0, 1, 255 };
const TForteInt16 FORTE_GET_STRUCT_VALUE::scm_anEOWithIndexes[] = { 0, -1 };
const CStringDictionary::TStringId FORTE_GET_STRUCT_VALUE::scm_anEventOutputNames[] = { g_nStringIdCNF };
const SFBInterfaceSpec FORTE_GET_STRUCT_VALUE::scm_stFBInterfaceSpec = { 1, scm_anEventInputNames, scm_anEIWith, scm_anEIWithIndexes, 1, scm_anEventOutputNames,
  scm_anEOWith, scm_anEOWithIndexes, 2, scm_anDataInputNames, scm_anDataInputTypeIds, 2, scm_anDataOutputNames, scm_anDataOutputTypeIds, 0, 0 };

CIEC_ANY* FORTE_GET_STRUCT_VALUE::getMemberFromName(CIEC_STRUCT* paWhereToLook, char* paMemberName) {
  CIEC_ANY* retVal = 0;

  CStringDictionary::TStringId elementNameId = CStringDictionary::getInstance().getId(paMemberName);
  if(CStringDictionary::scm_nInvalidStringId != elementNameId) {
    retVal = paWhereToLook->getMemberNamed(elementNameId);
  } else {
    DEVLOG_ERROR("[GET_STRUCT_VALUE]: In instance %s, member %s was not found\n", getInstanceName(), paMemberName);
  }

  return retVal;
}

CIEC_ANY* FORTE_GET_STRUCT_VALUE::lookForMember(CIEC_STRUCT* paWhereToLook, char* paMemberName) {
  CIEC_ANY* retVal = 0;

  char* startOfName = paMemberName;
  bool errorOcurred = false;

  while('\0' != *paMemberName) {
    if('.' == *paMemberName) {
      *paMemberName = '\0';

      CIEC_ANY *resultMember = getMemberFromName(paWhereToLook, startOfName);

      if(0 != resultMember) {
        if(CIEC_ANY::e_STRUCT == resultMember->getDataTypeID()) {
          retVal = lookForMember(static_cast<CIEC_STRUCT*>(resultMember), paMemberName + 1);
          if(0 == retVal) {
            errorOcurred = true;
          }
        } else {
          DEVLOG_ERROR("[GET_STRUCT_VALUE]: In instance %s, the non-struct member %s is followed by a sub-member which is not allowed\n", getInstanceName(),
            startOfName);
          errorOcurred = true;
        }
      } else {
        DEVLOG_ERROR("[GET_STRUCT_VALUE]: In instance %s, the member %s in structure %s doesn't exist\n", getInstanceName(), startOfName,
          CStringDictionary::getInstance().get(paWhereToLook->getStructTypeNameID()));
        errorOcurred = true;
      }
    }

    if(errorOcurred || 0 != retVal) {
      break;
    }
    paMemberName++;
  }

  if(!errorOcurred && 0 == retVal) {
    retVal = getMemberFromName(paWhereToLook, startOfName);
  }

  return retVal;
}

void FORTE_GET_STRUCT_VALUE::executeEvent(int paEIID) {
  if(scm_nEventREQID == paEIID) {
    QO() = 0;
    if(CIEC_ANY::e_STRUCT == in_struct().getDataTypeID()) {
      CIEC_STRING copyOfMember = member();
      CIEC_ANY *foundMember = lookForMember(static_cast<CIEC_STRUCT*>(&in_struct()), copyOfMember.getValue());
      if(0 != foundMember) {
        if(foundMember->getDataTypeID() == output().getDataTypeID()) {
          output().setValue(*foundMember);
          QO() = 1;
        } else {
          DEVLOG_ERROR("[GET_STRUCT_VALUE]: In instance %s, the member %s was found but it doesn't match the output type %d\n",
            getInstanceName(), member().getValue(), output().getDataTypeID());
        }
      } //error logging was done already in the internal function
    } else {
      DEVLOG_ERROR("[GET_STRUCT_VALUE]: In instance %s, the input structure is not of type structure but of type %d\n", getInstanceName(),
        in_struct().getDataTypeID());
    }

    sendOutputEvent(scm_nEventCNFID);
  }
}

