/*******************************************************************************
 * Copyright (c) 2005 - 2015 ACIN, Profactor GmbH, fortiss GmbH
 *                      2018 Johannes Kepler University
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Rene Smodic, Thomas Strasser, Gerhard Ebenhofer,
 *   Oliver Hummer, Ingo Hegny, Michael Hofmann, Martin Melik Merkumians
 *    - initial API and implementation and/or initial documentation
 *   Monika Wenger
 *    - fix: apostrophes are deleted in parseWriteConnectionData
 *   Jens Reimann
 *    - Enhance bootfile loading behavior
 *    Alois Zoitl - introduced new CGenFB class for better handling generic FBs
 *******************************************************************************/
#include <string.h>
#include "DEV_MGR.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "DEV_MGR_gen.cpp"
#endif
#include "../../core/device.h"
#include <stdio.h>
#include <stdlib.h>
#include "ForteBootFileLoader.h"
#include "../../core/utils/string_utils.h"

DEFINE_FIRMWARE_FB(DEV_MGR, g_nStringIdDEV_MGR)

const CStringDictionary::TStringId DEV_MGR::scm_anDataInputNames[] = {g_nStringIdQI, g_nStringIdID, g_nStringIdRESP};

const CStringDictionary::TStringId DEV_MGR::scm_anDataInputTypeIds[] = {g_nStringIdBOOL, g_nStringIdWSTRING, g_nStringIdSTRING};

const CStringDictionary::TStringId DEV_MGR::scm_anDataOutputNames[] = {g_nStringIdQO, g_nStringIdSTATUS, g_nStringIdDST, g_nStringIdRQST};

const CStringDictionary::TStringId DEV_MGR::scm_anDataOutputTypeIds[] = {g_nStringIdBOOL, g_nStringIdSTRING, g_nStringIdSTRING, g_nStringIdSTRING};

const TForteInt16 DEV_MGR::scm_anEIWithIndexes[] = {0, -1};
const TDataIOID DEV_MGR::scm_anEIWith[] = {0, 1, 255};
const CStringDictionary::TStringId DEV_MGR::scm_anEventInputNames[] = {g_nStringIdINIT, g_nStringIdREQ};

const TDataIOID DEV_MGR::scm_anEOWith[] = {0, 1, 255, 2, 3, 255};
const TForteInt16 DEV_MGR::scm_anEOWithIndexes[] = {0, -1, 3};
const CStringDictionary::TStringId DEV_MGR::scm_anEventOutputNames[] = {g_nStringIdINITO, g_nStringIdCNF};

const SFBInterfaceSpec DEV_MGR::scm_stFBInterfaceSpec = {
  2,  scm_anEventInputNames,  scm_anEIWith,  scm_anEIWithIndexes,
  2,  scm_anEventOutputNames,  scm_anEOWith, scm_anEOWithIndexes,  3,  scm_anDataInputNames, scm_anDataInputTypeIds,
  4,  scm_anDataOutputNames, scm_anDataOutputTypeIds,
  0, 0
};

const char * const DEV_MGR::scm_sMGMResponseTexts[13] = { "RDY", "BAD_PARAMS", "LOCAL_TERMINATION", "SYSTEM_TERMINATION", "NOT_READY", "UNSUPPORTED_CMD", "UNSUPPORTED_TYPE", "NO_SUCH_OBJECT", "INVALID_OBJECT", "INVALID_OPERATION", "INVALID_STATE", "OVERFLOW", "INVALID_DST" };

void DEV_MGR::executeEvent(int paEIID){
  if(scm_nEventINITID == paEIID){
#ifdef FORTE_SUPPORT_BOOT_FILE
    if((true == QI()) && (false == QO())){
      //this is the first time init is called try to load a boot file
      ForteBootFileLoader loader(*this);
      if(loader.needsExit()){
        getResource().getDevice().changeFBExecutionState(cg_nMGM_CMD_Kill);
        return;
      }
      if(loader.isOpen() && LOAD_RESULT_OK == loader.loadBootFile()){
          DEVLOG_INFO("Bootfile correctly loaded\n");
      }
    }
#endif
    CCommFB::executeEvent(paEIID);  //initialize the underlying server FB
  }else{
    if(cg_nExternalEventID == paEIID && //we received a message on the network let the server correctly handle it
        forte::com_infra::e_ProcessDataOk == CCommFB::receiveData()){ //the message was correctly received
      executeRQST();
      //send response
      CCommFB::sendData();
    }
  }
}

void DEV_MGR::executeRQST(void){
  mCommand.mAdditionalParams.clear();
  EMGMResponse resp = parseAndExecuteMGMCommand(DST().getValue(), RQST().getValue());

#ifdef FORTE_SUPPORT_MONITORING
  if (0 != mCommand.mMonitorResponse.length()) {
    generateMonitorResponse(resp, mCommand);
  } else
#endif //FORTE_SUPPORT_MONITORING
  if(0 < mCommand.mAdditionalParams.length()){
    generateLongResponse(resp, mCommand);
  }
  else{
    generateResponse(mCommand.mID, resp);
  }
}

char *DEV_MGR::parseRequest(char *paRequestString, forte::core::SManagementCMD &paCommand){
//first check if it is an management request
  char *acCommandStart = 0;
  static const int scnCommandLength[] = {7, 7, 6, 5, 5, 6, 5, 6, 6};

  if(!strncmp("<Request ID=\"", paRequestString, 13)){
    int i = 13;
    int j;
    paCommand.mID = &(paRequestString[i]);
    for(j = 0; paRequestString[i] != '\"'; ++i, ++j){
      if(j >= 7){
        return 0;
      }
    }
    paRequestString[i] = '\0'; //close ID
    ++i;
    acCommandStart = strchr((&paRequestString[i]), '\"');
    if(acCommandStart != 0){
      acCommandStart++; //this is the real start of the command
      if(!strncmp("CREATE", acCommandStart, 6)){
        paCommand.mCMD = cg_nMGM_CMD_Create_Group;
      }
      else if(!strncmp("DELETE", acCommandStart, 6)){
        paCommand.mCMD = cg_nMGM_CMD_Delete_Group;
      }
      else if(!strncmp("START", acCommandStart, 5)){
        paCommand.mCMD = cg_nMGM_CMD_Start;
      }
      else if(!strncmp("STOP", acCommandStart, 4)){
        paCommand.mCMD = cg_nMGM_CMD_Stop;
      }
      else if(!strncmp("KILL", acCommandStart, 4)){
        paCommand.mCMD = cg_nMGM_CMD_Kill;
      }
      else if(!strncmp("RESET", acCommandStart, 5)){
        paCommand.mCMD = cg_nMGM_CMD_Reset;
      }
      else if(!strncmp("READ", acCommandStart, 4)){
        paCommand.mCMD = cg_nMGM_CMD_Read;
      }
      else if(!strncmp("WRITE", acCommandStart, 5)){
        paCommand.mCMD = cg_nMGM_CMD_Write;
      }
#ifdef FORTE_SUPPORT_QUERY_CMD
      else if(!strncmp("QUERY", acCommandStart, 5)){
        paCommand.mCMD = cg_nMGM_CMD_Query_Group;
      }
#endif
      else{
        return 0;
      }
      acCommandStart += scnCommandLength[paCommand.mCMD];
    }
  }
  return acCommandStart;
}

#ifdef FORTE_DYNAMIC_TYPE_LOAD
bool DEV_MGR::parseXType(char *paRequestPartLeft, forte::core::SManagementCMD &paCommand, const char *paRequestType) {
  bool retVal = false;
  size_t nReqLength = strlen(paRequestType);
  if(!strncmp(paRequestType, paRequestPartLeft, nReqLength)){
    paRequestPartLeft = &(paRequestPartLeft[nReqLength]);
    if('*' != paRequestPartLeft[0]){
      int i = parseIdentifier(paRequestPartLeft, paCommand.mFirstParam);
      paRequestPartLeft = (-1 == i) ? 0 : strchr(&(paRequestPartLeft[i + 1]), '>');
    }
    if(0 != paRequestPartLeft){
      paRequestPartLeft++;
      char* endOfRequest = strchr(paRequestPartLeft, '<');
      *endOfRequest = '\0';
      forte::core::util::transformEscapedXMLToNonEscapedText(paRequestPartLeft);
      paCommand.mAdditionalParams = paRequestPartLeft;
      retVal = true;
    }
  }
  return retVal;
}
#endif

bool DEV_MGR::parseFBData(char *paRequestPartLeft, forte::core::SManagementCMD &paCommand){
  bool retVal = false;

  if(!strncmp("FB Name=\"", paRequestPartLeft, 9)){
    char *acBuf = &(paRequestPartLeft[9]);
    int i = 0;
    if(acBuf[0] != '*'){
      i = parseIdentifier(acBuf, paCommand.mFirstParam);
      acBuf = (-1 == i) ? 0 : strchr(&(acBuf[i + 1]), '\"');
    }
    else{
      acBuf = strchr(&(acBuf[i + 2]), '\"');
    }

    if(acBuf != 0){
      if(acBuf[1] != '*'){
        ++acBuf;
        i = parseIdentifier(acBuf, paCommand.mSecondParam);
        if(-1 != i){
          acBuf = strchr(&(acBuf[i + 1]), '\"');
          if(acBuf != 0){
            // We have an application name given
            ++acBuf;
            TForteUInt16 nBufLength = static_cast<TForteUInt16>(strcspn(acBuf, "\"") + 1);
            paCommand.mAdditionalParams.assign(acBuf, nBufLength);
          }
        }
        else{
          return false;
        }
      }
      retVal = true;
    }
  }
  return retVal;
}

int DEV_MGR::parseIdentifier(char *paIdentifierStart, forte::core::TNameIdentifier &paIdentifier){
  for(char *runner = paIdentifierStart, *start = paIdentifierStart; '\0' != *runner; ++runner){
    if('.' == *runner){
      *runner = '\0';
      if(!paIdentifier.pushBack(CStringDictionary::getInstance().insert(start))){
        return -1;
      }
      *runner = '.';
      start = runner + 1;
    } else if ('"' == *runner){
      *runner = '\0';
      if(!paIdentifier.pushBack(CStringDictionary::getInstance().insert(start))){
        return -1;
      }
      *runner = '"';
      return static_cast<int>(runner - paIdentifierStart);
    }
  }
  return -1;
}

bool DEV_MGR::parseConnectionData(char *paRequestPartLeft, forte::core::SManagementCMD &paCommand){
  bool bRetVal = false;
  if(!strncmp("Connection Source=\"", paRequestPartLeft, sizeof("Connection Source=\"") - 1)){
    int i = parseIdentifier(&(paRequestPartLeft[19]), paCommand.mFirstParam);
    if(-1 != i){
      char *acBuf = strchr(&(paRequestPartLeft[i + 21]), '\"');
      if(acBuf != 0){
        parseIdentifier(&(acBuf[1]), paCommand.mSecondParam);
        bRetVal = (-1 != i);
      }
    }
  }
  return bRetVal;
}

bool DEV_MGR::parseWriteConnectionData(char *paRequestPartLeft, forte::core::SManagementCMD &paCommand){
  bool retVal = false;
  if(!strncmp("Connection Source=\"", paRequestPartLeft, sizeof("Connection Source=\"") - 1)){
    paRequestPartLeft = &(paRequestPartLeft[19]);

    char* endOfSource = strchr(paRequestPartLeft, '\"');
    if(0 == endOfSource){
      return false;
    }
    *endOfSource = '\0';
    forte::core::util::transformEscapedXMLToNonEscapedText(paRequestPartLeft);
    paCommand.mAdditionalParams = paRequestPartLeft;
    *endOfSource = '"'; // restore the string
    paRequestPartLeft = strchr(endOfSource + 1, '\"');
    if(0 != paRequestPartLeft){
      retVal = (-1 != parseIdentifier(&paRequestPartLeft[1], paCommand.mFirstParam));
    }
  }
  return retVal;
}

void DEV_MGR::parseCreateData(char *paRequestPartLeft, forte::core::SManagementCMD &paCommand){
  paCommand.mCMD = cg_nMGM_CMD_INVALID;
  if(0 != paRequestPartLeft){
      switch (paRequestPartLeft[0]){
#ifdef FORTE_DYNAMIC_TYPE_LOAD
        case 'A': // we have an Adapter to Create
          if(parseXType(paRequestPartLeft, paCommand, "AdapterType Name=\"")){
            paCommand.mCMD = cg_nMGM_CMD_Create_AdapterType;
          }
          break;
#endif
        case 'F': // we have an FB to Create
          if(parseFBData(paRequestPartLeft, paCommand)){
            paCommand.mCMD = cg_nMGM_CMD_Create_FBInstance;
          }
#ifdef FORTE_DYNAMIC_TYPE_LOAD
          else if(parseXType(paRequestPartLeft, paCommand, "FBType Name=\"")){
            paCommand.mCMD = cg_nMGM_CMD_Create_FBType;
          }
#endif
          break;
        case 'C': // we have an Connection to Create
          if(parseConnectionData(paRequestPartLeft, paCommand)){
            paCommand.mCMD = cg_nMGM_CMD_Create_Connection;
          }
          break;
#ifdef FORTE_SUPPORT_MONITORING
        case 'W': // we have an Watch to Add
          if(parseMonitoringData(paRequestPartLeft, paCommand)){
            paCommand.mCMD = cg_nMGM_CMD_Monitoring_Add_Watch;
          }
          break;
#endif //FORTE_SUPPORT_MONITORING
        default:
          break;
      }
  }
}

void DEV_MGR::parseDeleteData(char *paRequestPartLeft, forte::core::SManagementCMD &paCommand){
  paCommand.mCMD = cg_nMGM_CMD_INVALID;
  if(0 != paRequestPartLeft){
    switch (paRequestPartLeft[0]){
      case 'F': // we have an FB to delete
        if(parseFBData(paRequestPartLeft, paCommand)){
          paCommand.mCMD = cg_nMGM_CMD_Delete_FBInstance;
        }
        break;
      case 'C': // we have an Connection to delete
        if(parseConnectionData(paRequestPartLeft, paCommand)){
          paCommand.mCMD = cg_nMGM_CMD_Delete_Connection;
        }
        break;
#ifdef FORTE_SUPPORT_MONITORING
      case 'W': // we have an Watch to remove
        if(parseMonitoringData(paRequestPartLeft, paCommand)){
           paCommand.mCMD = cg_nMGM_CMD_Monitoring_Remove_Watch;
        }
        break;
#endif // FORTE_SUPPORT_MONITORING
      default:
        break;
    }
  }
}

void DEV_MGR::parseAdditionalStateCommandData(char *paRequestPartLeft, forte::core::SManagementCMD &paCommand){
  if(0 != paRequestPartLeft && '/' != paRequestPartLeft[0] && //if we have an additional xml token parse if it is an FB definition
    !parseFBData(paRequestPartLeft, paCommand)) {
    paCommand.mCMD = cg_nMGM_CMD_INVALID;
  }
}

void DEV_MGR::parseReadData(char *paRequestPartLeft, forte::core::SManagementCMD &paCommand){
  paCommand.mCMD = cg_nMGM_CMD_INVALID;
  if(0 != paRequestPartLeft){
#ifdef FORTE_SUPPORT_MONITORING
    if('W' == paRequestPartLeft[0]){
          paCommand.mCMD = cg_nMGM_CMD_Monitoring_Read_Watches;
    } else
#endif // FORTE_SUPPORT_MONITORING
      if(parseConnectionData(paRequestPartLeft, paCommand)){
        paCommand.mCMD = cg_nMGM_CMD_Read;
      }
  }
}

void DEV_MGR::parseWriteData(char *paRequestPartLeft, forte::core::SManagementCMD &paCommand){
  //We need an additional xml connection token parse if it is an connection definition
  paCommand.mCMD = cg_nMGM_CMD_INVALID;
  if(0 != paRequestPartLeft && parseWriteConnectionData(paRequestPartLeft, paCommand)){
#ifdef FORTE_SUPPORT_MONITORING
    char *pch = strstr(paRequestPartLeft, "force=\"");
    if (0 != pch) {
      if (!strncmp(&pch[7], "true", sizeof("true") - 1)) {
        paCommand.mCMD = cg_nMGM_CMD_Monitoring_Force;
      } else if (!strncmp(&pch[7], "false", sizeof("false") - 1)) {
        paCommand.mCMD = cg_nMGM_CMD_Monitoring_ClearForce;
      }
    } else if ((2 == paCommand.mAdditionalParams.length()) &&
      (('$' == paCommand.mAdditionalParams.getValue()[0]) &&
        (('e' == paCommand.mAdditionalParams.getValue()[1]) ||('E' == paCommand.mAdditionalParams.getValue()[1]) ))){
      paCommand.mCMD = cg_nMGM_CMD_Monitoring_Trigger_Event;
    }else if ((3 == paCommand.mAdditionalParams.length()) &&
      (('$' == paCommand.mAdditionalParams.getValue()[0]) &&
       (('e' == paCommand.mAdditionalParams.getValue()[1]) ||('E' == paCommand.mAdditionalParams.getValue()[1]) ) &&
       (('r' == paCommand.mAdditionalParams.getValue()[2]) ||('R' == paCommand.mAdditionalParams.getValue()[2]) ) )){
      paCommand.mCMD = cg_nMGM_CMD_Monitoring_Reset_Event_Count;
    }else
#endif // FORTE_SUPPORT_MONITORING
      paCommand.mCMD = cg_nMGM_CMD_Write;
  }
}

#ifdef FORTE_SUPPORT_QUERY_CMD
void DEV_MGR::parseQueryData(char *paRequestPartLeft, forte::core::SManagementCMD &paCommand){
  paCommand.mCMD = cg_nMGM_CMD_INVALID;
  if(0 != paRequestPartLeft){
    switch (paRequestPartLeft[0]){
      case 'F': // query fb or fb type list
        if(!strncmp(paRequestPartLeft, "FBT", sizeof("FBT") - 1)){
          if(parseTypeListData(paRequestPartLeft, paCommand)){
            paCommand.mCMD = cg_nMGM_CMD_QUERY_FBTypes;
          }
#ifdef FORTE_DYNAMIC_TYPE_LOAD
          else if(parseXType(paRequestPartLeft, paCommand, "FBType Name=\"")){
            paCommand.mCMD = cg_nMGM_CMD_QUERY_FBType;
          }
#endif
          else {
            paCommand.mCMD = cg_nMGM_CMD_Query_Group;
          }
        }else if(parseFBData(paRequestPartLeft, paCommand)){
          paCommand.mCMD = cg_nMGM_CMD_QUERY_FB;
        }
        break;
      case 'C': // query connection list
        if(parseConnectionData(paRequestPartLeft, paCommand)){
          paCommand.mCMD = cg_nMGM_CMD_QUERY_Connection;
        }
        break;
      case 'D': // query datatype list
        if(!strncmp(paRequestPartLeft, "DataType", sizeof("DataType") - 1)){
          if(parseTypeListData(paRequestPartLeft, paCommand)){
            paCommand.mCMD = cg_nMGM_CMD_QUERY_DTTypes;
          } else {
            paCommand.mCMD = cg_nMGM_CMD_Query_Group;
          }
        }
        break;
      case 'A': // query adaptertype list
        if(!strncmp(paRequestPartLeft, "AdapterT", sizeof("AdapterT") - 1)){
          if(parseTypeListData(paRequestPartLeft, paCommand)){
            paCommand.mCMD = cg_nMGM_CMD_QUERY_AdapterTypes;
          }
#ifdef FORTE_DYNAMIC_TYPE_LOAD
          else if(parseXType(paRequestPartLeft, paCommand, "AdapterType Name=\"")){
            paCommand.mCMD = cg_nMGM_CMD_QUERY_AdapterType;
          }
#endif
          else {
            paCommand.mCMD = cg_nMGM_CMD_Query_Group;
          }
        }

        break;
      default:
        break;
    }
  }
}

bool DEV_MGR::parseTypeListData(char *paRequestPartLeft, forte::core::SManagementCMD &){
  bool retVal = true;

  if (!strncmp("DataType Name=\"", paRequestPartLeft, sizeof("DataType Name=\"") - 1)) {
    if(paRequestPartLeft[15] != '*'){ //does not support query for DataType-Declaration
      retVal = false;
    }
  }
  else if(!strncmp("FBType Name=\"", paRequestPartLeft, sizeof("FBType Name=\"") - 1)){
    if(paRequestPartLeft[13] != '*'){ //supports query for FBType-Declaration only for DynamicTypeLoad profile (LUA enabled)
      retVal = false;
    }
  }
  else if(!strncmp("AdapterType Name=\"", paRequestPartLeft, sizeof("AdapterType Name=\"") - 1)){
    if(paRequestPartLeft[18] != '*'){ //does not support query for AdapterType-Declaration
      retVal = false;
    }
  }
  return retVal;
}
#endif

void DEV_MGR::generateResponse(const char *paID, EMGMResponse paResp){
  RESP().clear();
  RESP().append("<Response ID=\"");
  if (0 != paID) {
    RESP().append(paID);
  }
  RESP().append("\"");
  if(e_RDY != paResp){
    RESP().append(" Reason=\"");
    RESP().append(scm_sMGMResponseTexts[paResp]);
    RESP().append("\"");
  }
  RESP().append(" />");
}

void DEV_MGR::generateLongResponse(EMGMResponse paResp, forte::core::SManagementCMD &paCMD){
  RESP().clear();
  RESP().reserve(static_cast<TForteUInt16>(255 + (paCMD.mAdditionalParams.length())));
  RESP().append("<Response ID=\"");
  if (0 != paCMD.mID) {
    RESP().append(paCMD.mID);
  }
  RESP().append("\"");
  if(e_RDY != paResp){
    RESP().append(" Reason=\"");
    RESP().append(scm_sMGMResponseTexts[paResp]);
    RESP().append("\">\n  ");
  }
  else{
    RESP().append(">\n  ");
    if(paCMD.mCMD == cg_nMGM_CMD_Read){
      RESP().append("<Connection Source=\"");
      appendIdentifierName(RESP(), paCMD.mFirstParam);
      RESP().append("\" Destination=\"");
      RESP().append(paCMD.mAdditionalParams.getValue());
      RESP().append("\" />");
    }
#ifdef FORTE_SUPPORT_QUERY_CMD
    else if(paCMD.mCMD == cg_nMGM_CMD_QUERY_Connection){
      if ((paCMD.mFirstParam.isEmpty()) &&
          (paCMD.mSecondParam.isEmpty())) { //src & dst = *
          RESP().append(paCMD.mAdditionalParams.getValue());
      }
      else { //either src or dst = * (both != * should be treated by generateResponse
        RESP().append("<EndpointList>\n    ");
        RESP().append(paCMD.mAdditionalParams.getValue());
        RESP().append("\n  </EndpointList>");
      }
    }
    else if(paCMD.mCMD == cg_nMGM_CMD_QUERY_FB){
      if(!paCMD.mFirstParam.isEmpty()) {  //Name != "*"
        if(!paCMD.mSecondParam.isEmpty()){ //Type != "*"
          RESP().append("<FBStatus Status=\"");
          RESP().append(paCMD.mAdditionalParams.getValue());
          RESP().append("\" />");
        } else { //Type == "*"
          RESP().append("<FB Name=\"");
          appendIdentifierName(RESP(), paCMD.mFirstParam);
          RESP().append("\" Type=\"");
          RESP().append(paCMD.mAdditionalParams.getValue());
          RESP().append("\" />");
        }
      }
      else{
        RESP().append("<FBList>\n    ");
        RESP().append(paCMD.mAdditionalParams.getValue());
        RESP().append("\n  </FBList>");
      }
    }
    else if(paCMD.mCMD == cg_nMGM_CMD_QUERY_FBTypes || paCMD.mCMD == cg_nMGM_CMD_QUERY_AdapterTypes){
      RESP().append("<NameList>\n    ");
      RESP().append(paCMD.mAdditionalParams.getValue());
      RESP().append("\n  </NameList>");
    }
    else if(paCMD.mCMD == cg_nMGM_CMD_QUERY_DTTypes){
      RESP().append("<DTList>\n    ");
      RESP().append(paCMD.mAdditionalParams.getValue());
      RESP().append("\n  </DTList>");
    }
    else if(paCMD.mCMD == cg_nMGM_CMD_QUERY_FBType){
      RESP().append("<FBType Comment=\"generated\" ");
      RESP().append(paCMD.mAdditionalParams.getValue());
      RESP().append("  </FBType>");
    }
    else if(paCMD.mCMD == cg_nMGM_CMD_QUERY_AdapterType){
      RESP().append("<AdapterType Comment=\"generated\" ");
      RESP().append(paCMD.mAdditionalParams.getValue());
      RESP().append("   <Service Comment=\"generated\" LeftInterface=\"SOCKET\" RightInterface=\"PLUG\"/>\n</AdapterType>");
    }
#endif
  }
  RESP().append("\n</Response>");
}

void DEV_MGR::appendIdentifierName(CIEC_STRING& paDest, forte::core::TNameIdentifier& paIdentifier) {
  if(!paIdentifier.isEmpty()){
    for(forte::core::TNameIdentifier::CIterator runner(paIdentifier.begin());
        runner != paIdentifier.end(); ++runner){
      paDest.append(CStringDictionary::getInstance().get(*runner));
      paDest.append(".");
    }
    paDest.append(CStringDictionary::getInstance().get(paIdentifier.back()));
  }
}

DEV_MGR::DEV_MGR(CStringDictionary::TStringId paInstanceNameId, CResource *paSrcRes) :
    CCommFB(paInstanceNameId, paSrcRes, forte::com_infra::e_Server),
    m_poDevice(paSrcRes->getDevice()) {
  setupFBInterface(&scm_stFBInterfaceSpec, m_anFBConnData, m_anFBVarsData);
  mCommand.mAdditionalParams.reserve(255);
  mCommand.mAdditionalParams.clear();
}

DEV_MGR::~DEV_MGR(){
  freeAllData();
  m_pstInterfaceSpec = 0;  //block any wrong cleanup in the generic fb base class of CBaseCommFB
}

EMGMResponse DEV_MGR::parseAndExecuteMGMCommand(char *paDest, char *paCommand){
  EMGMResponse eResp = e_INVALID_OBJECT;
  if(0 != strchr(paCommand, '>')){
    mCommand.mDestination = (strlen(paDest) != 0) ? CStringDictionary::getInstance().insert(paDest) : CStringDictionary::scm_nInvalidStringId;
    mCommand.mFirstParam.clear();
    mCommand.mSecondParam.clear();
    if ( 255 <= mCommand.mAdditionalParams.getCapacity()) {
      mCommand.mAdditionalParams.reserve(255);
    }
    mCommand.mID=0;
#ifdef FORTE_SUPPORT_MONITORING
  mCommand.mMonitorResponse.clear();
#endif // FORTE_SUPPORT_MONITORING
    char *acRequestPartLeft = parseRequest(paCommand, mCommand);
    if(0 != acRequestPartLeft){
      acRequestPartLeft = strchr(acRequestPartLeft, '<');
      if(0 != acRequestPartLeft){
        acRequestPartLeft++; //point to the next character after the <
      }
      // we got the command for execution
      // now check the rest of the data
      switch (mCommand.mCMD){
        case cg_nMGM_CMD_Create_Group: // create something
          parseCreateData(acRequestPartLeft, mCommand);
          break;
        case cg_nMGM_CMD_Delete_Group: //delete something
          parseDeleteData(acRequestPartLeft, mCommand);
          break;
        case cg_nMGM_CMD_Start:
        case cg_nMGM_CMD_Stop:
        case cg_nMGM_CMD_Kill:
        case cg_nMGM_CMD_Reset:
          parseAdditionalStateCommandData(acRequestPartLeft, mCommand);
          break;
        case cg_nMGM_CMD_Read:
          parseReadData(acRequestPartLeft, mCommand);
          break;
        case cg_nMGM_CMD_Write:
          parseWriteData(acRequestPartLeft, mCommand);
          break;
#ifdef FORTE_SUPPORT_QUERY_CMD
        case cg_nMGM_CMD_Query_Group: // query something
          parseQueryData(acRequestPartLeft, mCommand);
#endif
          break;
        default:
          break;
      }

      if(cg_nMGM_CMD_INVALID != mCommand.mCMD) {
          eResp = m_poDevice.executeMGMCommand(mCommand);
      }
    }
    else {
      eResp = e_UNSUPPORTED_CMD;
    }
  }
  return eResp;
}

#ifdef FORTE_SUPPORT_MONITORING

bool DEV_MGR::parseMonitoringData(char *paRequestPartLeft, forte::core::SManagementCMD &paCommand){
  bool bRetVal = false;
  if(!strncmp("Watch Source=\"", paRequestPartLeft, sizeof("Watch Source=\"") - 1)){
    int i = parseIdentifier(&(paRequestPartLeft[14]), paCommand.mFirstParam);
    if(-1 != i){
      char *acBuf = strchr(&(paRequestPartLeft[i + 16]), '\"');
      if(acBuf != 0){
        parseIdentifier(&(acBuf[1]), paCommand.mSecondParam);
        bRetVal = (-1 != i);
      }
    }
  }
  return bRetVal;
}

void DEV_MGR::generateMonitorResponse(EMGMResponse paResp, forte::core::SManagementCMD &paCMD){
  RESP().clear();
  if(e_RDY != paResp){
    RESP().append("<Response ID=\"");
    RESP().append(paCMD.mID);
    RESP().append("\"");
    RESP().append(" Reason=\"");
    RESP().append(scm_sMGMResponseTexts[paResp]);
    RESP().append("\">\n  ");
    RESP().append("\n</Response>");
  }else{
    TForteUInt16 size = static_cast<TForteUInt16>(paCMD.mMonitorResponse.length() + strlen(paCMD.mID) + 74);
    RESP().reserve(size);

    RESP().clear();
    RESP().append("<Response ID=\"");
    RESP().append(paCMD.mID);
    RESP().append("\"");
    RESP().append(">\n  ");
    if(paCMD.mCMD == cg_nMGM_CMD_Monitoring_Read_Watches) {
      RESP().append("<Watches>\n    ");
      RESP().append(paCMD.mMonitorResponse.getValue());
      RESP().append("\n  </Watches>");
    }
    RESP().append("\n</Response>");
  }
  paCMD.mMonitorResponse.clear();
}

#endif // FORTE_SUPPORT_MONITORING

bool DEV_MGR::executeCommand(char *paDest, char *paCommand){
  EMGMResponse eResp = parseAndExecuteMGMCommand(paDest, paCommand);
  if(eResp != e_RDY){
    DEVLOG_ERROR("Boot file error. DEV_MGR says error is %s\n", DEV_MGR::scm_sMGMResponseTexts[eResp]);
  }
  return (eResp == e_RDY);
}
