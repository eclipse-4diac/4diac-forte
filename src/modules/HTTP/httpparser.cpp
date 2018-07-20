/*******************************************************************************
* Copyright (c) 2017-2018 Marc Jakobi, github.com/MrcJkb, fortiss GmbH
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
*
* Contributors:
*    Marc Jakobi - initial implementation
*    Jose Cabral - Use CIEC_STRING instead of const char* and cleanning of functions
********************************************************************************/

#include "httpparser.h"
#include <stdio.h>
#include <string.h>
#include "devlog.h"

using namespace forte::com_infra;

CHttpParser::CHttpParser() {
}

CHttpParser::~CHttpParser(){
}

void CHttpParser::createGetRequest(CIEC_STRING& paDest, const CIEC_STRING& paHost, const CIEC_STRING& paPath) {
  CHttpParser::addCommonHeader(paDest, paHost, paPath, CHttpComLayer::e_GET);
  CHttpParser::addHeaderEnding(paDest);
}

void CHttpParser::createPutPostRequest(CIEC_STRING& paDest, const CIEC_STRING& paHost, const CIEC_STRING& paPath, const CIEC_STRING& paData, const CIEC_STRING& paContentType, CHttpComLayer::ERequestType paType){
  CHttpParser::addCommonHeader(paDest, paHost, paPath, paType);
  paDest.append("\r\nContent-type: ");
  paDest.append(paContentType.getValue());
  paDest.append("\r\nContent-length: ");
  changePutPostData(paDest, paData);
}

bool CHttpParser::changePutPostData(CIEC_STRING& paDest, const CIEC_STRING& paData){
  char* helperChar = strstr(paDest.getValue(), "length: ");
  if(0 != helperChar){
    helperChar += 8;
    *helperChar = '\0';
    paDest = paDest.getValue(); //will shrink the length of the string to the new ending
    char contentLength[6]; //TODO: should this be fixed or not?
    sprintf(contentLength, "%zu", strlen(paData.getValue()));
    paDest.append(contentLength);
    CHttpParser::addHeaderEnding(paDest);
    paDest.append(paData.getValue());
    return true;
  }
  else{ // wrong request?
    DEVLOG_ERROR("[HTTP Parser] PUT/POST request was wrongly created\n");
    return false;
  }
}

void CHttpParser::addCommonHeader(CIEC_STRING& paDest, const CIEC_STRING& paHost, const CIEC_STRING& paPath, CHttpComLayer::ERequestType paType) {
  switch(paType){
    case CHttpComLayer::e_GET:
      paDest = "GET /";
      break;
    case CHttpComLayer::e_PUT:
      paDest = "PUT /";
      break;
    case CHttpComLayer::e_POST:
      paDest = "POST /";
      break;
    default:
      DEVLOG_ERROR("[HTTP Parser] Unexpected HTTP Type when adding header\n");
      break;
  }

  paDest.append(paPath.getValue());
  paDest.append(" HTTP/1.1\r\n");
  paDest.append("Host: ");
  paDest.append(paHost.getValue());
}

void CHttpParser::addHeaderEnding(CIEC_STRING& paDest) {
  paDest.append("\r\n\r\n");
}

bool CHttpParser::parseGetResponse(CIEC_STRING& paDest, char* paSrc, CIEC_STRING& paExpectedCode){
  CIEC_STRING receivedCode;
  if(CHttpParser::getHttpResponseCode(receivedCode, paSrc)){
    if(receivedCode == paExpectedCode || "*" == paExpectedCode){
      char* helperChar = strstr(paSrc, "\r\n\r\n"); // Extract data from HTTP GET response char
      if(0 != helperChar){
        helperChar += 4;
        paDest = helperChar;
        return true;
      }else{ // Empty response received?
        DEVLOG_INFO("[HTTP Parser] Empty GET content response received\n");
        paDest = "";
        return false;
      }
    }else{
      DEVLOG_INFO("[HTTP Parser] Unexpected GET response code. Expected was %s and received was: \n", paExpectedCode.getValue(), receivedCode.getValue());
    }
  }
  return false;
}

bool CHttpParser::parsePutPostResponse(CIEC_STRING& paDest, char* paSrc, CIEC_STRING& paExpectedCode) {
  if(CHttpParser::getHttpResponseCode(paDest, paSrc)){
    if(paDest == paExpectedCode || "*" == paExpectedCode){
      return true;
    }
    else{
      DEVLOG_ERROR("[HTTP Parser] Unexpected PUT response code\n");
    }
  }
  return false;
}

bool CHttpParser::getHttpResponseCode(CIEC_STRING& paDest, char* paSrc) {
  char* helperChar = strstr(paSrc, "\r\n");
  if (helperChar != 0) {
    *helperChar = '\0';
    paDest = paSrc;
    *helperChar = '\r';
    return true;
  }
  else {
    DEVLOG_ERROR("[HTTP Parser] Invalid HTTP response\n");
    paDest = "Invalid response";
    return false;
  }
}

bool forte::com_infra::CHttpParser::parseGetRequest(CIEC_STRING& paPath, CSinglyLinkedList<CIEC_STRING>& paParameterNames, CSinglyLinkedList<CIEC_STRING>& paParameterValues, char* paData){
  char* helperChar = strstr(paData, "GET ");
   if (helperChar != 0) {
     helperChar += 4;
     char* endOfPath = strstr(helperChar, " ");
     if (endOfPath != 0) {
       *endOfPath = '\0';
       char* startOfParameters = strstr(paData, "?");
       if(startOfParameters != 0){
         *startOfParameters = '\0';
         startOfParameters++;
         parseParameters(startOfParameters, paParameterNames, paParameterValues);
       }
       paPath = helperChar;
     }else{
      DEVLOG_ERROR("[HTTP Parser] Invalid HTTP Get request. No space after path found\n");
      return false;
     }
     return true;
   }
   else {
     DEVLOG_ERROR("[HTTP Parser] Invalid HTTP Get request. No GET string found\n");
     return false;
   }
}

bool forte::com_infra::CHttpParser::createReponse(CIEC_STRING& paDest, const CIEC_STRING& paResult, const CIEC_STRING& paContentType, const CIEC_STRING& paData){
  paDest.append("HTTP/1.1 ");
  paDest.append(paResult.getValue());
  paDest.append("\r\nContent-type: ");
  paDest.append(paContentType.getValue());
  paDest.append("\r\nContent-length: ");
  changePutPostData(paDest, paData);
  return true;
}

unsigned int forte::com_infra::CHttpParser::parseParameters(char* paParameters, CSinglyLinkedList<CIEC_STRING>& paParameterNames, CSinglyLinkedList<CIEC_STRING>& paParameterValues){
  paParameterNames.clearAll();
  paParameterValues.clearAll();

  unsigned int retVal = 0;

  char* startOfName = paParameters;

  while('\0' != *startOfName){
    char* startOfValue = strstr(startOfName, "=");
    if(0 != startOfValue){
      *startOfValue = '\0';
      startOfValue++;
    }else{
      paParameterNames.clearAll();
      paParameterValues.clearAll();
      retVal = 0;
      break;
    }
    char* nextName = strstr(startOfValue, "&");
    bool endOfParameters = false;;
    if(0 != nextName){
      *nextName = '\0';
    }else{
      endOfParameters = true;
    }
    paParameterNames.push_back(startOfName);
    paParameterValues.push_back(startOfValue);
    retVal++;
    if(!endOfParameters){
      startOfName = ++nextName;
    }else{
      break;
    }

  }

  return retVal;
}




























