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

void CHttpParser::createPutRequest(CIEC_STRING& paDest, const CIEC_STRING& paHost, const CIEC_STRING& paPath, const CIEC_STRING& paData, const CIEC_STRING& paContentType){
  CHttpParser::addCommonHeader(paDest, paHost, paPath, CHttpComLayer::e_PUT);
  paDest.append("\r\nContent-type: ");
  paDest.append(paContentType.getValue());
  paDest.append("\r\nContent-length: ");
  changePutData(paDest, paData);
}

bool CHttpParser::changePutData(CIEC_STRING& paDest, const CIEC_STRING& paData){
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
    DEVLOG_ERROR("[HTTP Parser] PUT request was wrongly created\n");
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
  if(CHttpParser::getHttpResponseCode(paDest, paSrc)){
    if(paDest == paExpectedCode){
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
    }
  }else{
    // Bad response received?
    DEVLOG_INFO("[HTTP Parser] Unexpected GET response code\n");
  }
  return false;
}

bool CHttpParser::parsePutResponse(CIEC_STRING& paDest, char* paSrc, CIEC_STRING& paExpectedCode) {
  if(CHttpParser::getHttpResponseCode(paDest, paSrc)){
    if(paDest == paExpectedCode){
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
