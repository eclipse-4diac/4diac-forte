/*******************************************************************************
 * Copyright (c) 2017-2018 Marc Jakobi, github.com/MrcJkb, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
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

void CHttpParser::createGetRequest(CIEC_STRING& paDest, const CIEC_STRING& paHost, const CIEC_STRING& paPath) {
  CHttpParser::addCommonHeader(paDest, paHost, paPath, CHttpComLayer::e_GET);
  CHttpParser::addHeaderEnding(paDest);
}

void CHttpParser::createPutPostRequest(CIEC_STRING& paDest, const CIEC_STRING& paHost, const CIEC_STRING& paPath, const CIEC_STRING& paData,
    const CIEC_STRING& paContentType, CHttpComLayer::ERequestType paType) {
  CHttpParser::addCommonHeader(paDest, paHost, paPath, paType);
  paDest.append("\r\nContent-type: ");
  paDest.append(paContentType.getValue());
  paDest.append("\r\nContent-length: ");
  changePutPostData(paDest, paData);
}

bool CHttpParser::changePutPostData(CIEC_STRING& paDest, const CIEC_STRING& paData) {
  char* helperChar = strstr(paDest.getValue(), "length: ");
  if(0 != helperChar) {
    helperChar += sizeof("length: ") - 1;
    *helperChar = '\0';
    paDest = paDest.getValue(); //will shrink the length of the string to the new ending
    char contentLength[scmMaxLengthOfContent];
    snprintf(contentLength, sizeof(contentLength), "%zu", strlen(paData.getValue()));
    paDest.append(contentLength);
    CHttpParser::addHeaderEnding(paDest);
    paDest.append(paData.getValue());
    return true;
  } else { // wrong request?
    DEVLOG_ERROR("[HTTP Parser] PUT/POST request was wrongly created\n");
    return false;
  }
}

bool CHttpParser::parseResponse(CIEC_STRING& paBody, CIEC_STRING& paResponseCode, char* paSrc) {
  if(CHttpParser::getHttpResponseCode(paResponseCode, paSrc)) {
    char* helperChar = strstr(paSrc, "\r\n\r\n"); // Extract data from HTTP response char
    if(0 != helperChar) {
      helperChar += sizeof("\r\n\r\n") - 1;
      paBody = helperChar;
    } else { // Empty response received
      DEVLOG_INFO("[HTTP Parser] Empty content response received\n");
      paBody = "";
    }
    return true;
  }
  return false;
}

bool forte::com_infra::CHttpParser::parseGetRequest(CIEC_STRING& paPath, CSinglyLinkedList<CIEC_STRING>& paParameterNames,
    CSinglyLinkedList<CIEC_STRING>& paParameterValues, char* paData) {
  if(0 == strncmp(paData, "GET ", 4)) {
    paData = paData + 4;

    char* endOfPath = strstr(paData, " ");
    if(endOfPath != 0) {
      *endOfPath = '\0';
      char* startOfParameters = strstr(paData + 1, "?");
      if(startOfParameters != 0) {
        *startOfParameters = '\0';
        startOfParameters++;
        parseGETParameters(startOfParameters, paParameterNames, paParameterValues);
      }
      paPath = paData;
    } else {
      DEVLOG_ERROR("[HTTP Parser] Invalid HTTP Get request. No GET string found\n");
      return false;
    }

  } else {
    DEVLOG_ERROR("[HTTP Parser] Invalid HTTP Get request. No space after path found\n");
    return false;
  }
  return true;
}

bool forte::com_infra::CHttpParser::parsePutPostRequest(CIEC_STRING& paPath, CIEC_STRING &paContent, char* paData) {
  if(0 == strncmp(paData, "PUT ", 4)) {
    paData += sizeof("PUT ") - 1;
  } else if(0 == strncmp(paData, "POST ", 5)) {
    paData += sizeof("POST ") - 1;
  } else {
    DEVLOG_ERROR("[HTTP Parser] Invalid HTTP PUT/POST request. No PUT/POST string found\n");
    return false;
  }

  char* endOfPath = strstr(paData, " ");
  if(endOfPath != 0) {
    *endOfPath = '\0';
    paPath = paData;
    paData = strstr(endOfPath + 1, "\r\n\r\n");
    if(paData != 0) {
      paData += sizeof("\r\n\r\n") - 1;
      paContent = paData;
    } else {
      DEVLOG_ERROR("[HTTP Parser] Invalid HTTP PUT/POST request. No content was found\n");
      return false;
    }

  } else {
    DEVLOG_ERROR("[HTTP Parser] Invalid HTTP PUT/POST request. No space after path found\n");
    return false;
  }
  return true;
}


CHttpComLayer::ERequestType forte::com_infra::CHttpParser::getTypeOfRequest(const char* paRequest) {
  if(0 == strncmp(paRequest, "GET ", 4)) {
    return CHttpComLayer::e_GET;
  } else if(0 == strncmp(paRequest, "PUT ", 4)) {
    return CHttpComLayer::e_PUT;
  } else if(0 == strncmp(paRequest, "POST ", 5)) {
    return CHttpComLayer::e_POST;
  } else {
    DEVLOG_ERROR("[HTTP Parser] Invalid HTTP request\n");
    return CHttpComLayer::e_NOTSET;
  }
}

void forte::com_infra::CHttpParser::createResponse(CIEC_STRING& paDest, const CIEC_STRING& paResult, const CIEC_STRING& paContentType,
    const CIEC_STRING& paData) {
  paDest = paResult;
  if(paData.empty()) {
    paDest.append("\r\n");
  } else {
    paDest.append("\r\nContent-type: ");
    paDest.append(paContentType.getValue());
    paDest.append("\r\nContent-length: ");
    changePutPostData(paDest, paData);
  }
}

void CHttpParser::addCommonHeader(CIEC_STRING& paDest, const CIEC_STRING& paHost, const CIEC_STRING& paPath, CHttpComLayer::ERequestType paType) {
  switch(paType){
    case CHttpComLayer::e_GET:
      paDest = "GET ";
      break;
    case CHttpComLayer::e_PUT:
      paDest = "PUT ";
      break;
    case CHttpComLayer::e_POST:
      paDest = "POST ";
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

bool CHttpParser::getHttpResponseCode(CIEC_STRING& paDest, char* paSrc) {
  //HTTP-Version SP Status-Code SP Reason-Phrase CRLF (SP = space)
  char* helperChar = strstr(paSrc, "\r\n");
  if(helperChar != 0) {
    *helperChar = '\0';
    CParameterParser parser(paSrc, ' ');
    if(3 <= parser.parseParameters()) { //Reason-Phrase can contain spaces in it
      paDest = parser[1];
    } else {
      DEVLOG_ERROR("[HTTP Parser] Invalid HTTP response. The status line is not well defined\n");
      return false;
    }
    *helperChar = '\r';
    return true;
  } else {
    DEVLOG_ERROR("[HTTP Parser] Invalid HTTP response. No line ending was found.\n");
    return false;
  }
}

unsigned int forte::com_infra::CHttpParser::parseGETParameters(char* paParameters, CSinglyLinkedList<CIEC_STRING>& paParameterNames,
    CSinglyLinkedList<CIEC_STRING>& paParameterValues) {
  paParameterNames.clearAll();
  paParameterValues.clearAll();

  unsigned int retVal = 0;

  char* startOfName = paParameters;
  bool endOfParameters = false;
  while('\0' != *startOfName && !endOfParameters) {
    char* startOfValue = strstr(startOfName, "=");
    if(0 != startOfValue) {
      *startOfValue = '\0';
      startOfValue++;
    } else {
      paParameterNames.clearAll();
      paParameterValues.clearAll();
      retVal = 0;
      break;
    }
    char* nextName = strstr(startOfValue, "&");
    endOfParameters = false;
    if(0 != nextName) {
      *nextName = '\0';
    } else {
      endOfParameters = true;
    }
    paParameterNames.pushBack(startOfName);
    paParameterValues.pushBack(startOfValue);
    retVal++;
    if(!endOfParameters) {
      startOfName = ++nextName;
    }
  }

  return retVal;
}
