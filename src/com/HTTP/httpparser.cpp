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
 *    Martin Melik Merkumians - Change CIEC_STRING to std::string
 ********************************************************************************/

#include "httpparser.h"

#include <stdio.h>
#include <string.h>

#include "arch/datatype.h"
#include "arch/devlog.h"
#include "core/util/parameterParser.h"

using namespace forte::com_infra;
using namespace std::string_literals;

void CHttpParser::createGetRequest (std::string &paDest, const std::string &paHost, const TForteUInt16 paPort, const std::string &paPath, const std::string &paAuth, const std::string &paParams) {
  addCommonHeader(paDest, paHost, paPort, paPath, paAuth, paParams, CHttpComLayer::e_GET);
  addHeaderEnding(paDest);
}

void CHttpParser::addBodyToRequest (const std::string &paBody, const std::string &paContentType, std::string &paDest) {
  if(!paContentType.empty()){
    paDest += "\r\nContent-Type: "s;
    paDest += paContentType;
    paDest += "\r\nContent-Length: "s;
    paDest += std::to_string(paBody.length());
    addHeaderEnding(paDest);
  }
  if (!paBody.empty()) {
    paDest += paBody;
  }
}

void CHttpParser::createPutPostRequest (std::string &paDest, const std::string &paHost, const TForteUInt16 paPort, const std::string &paPath, const std::string &paAuth, const std::string &paParams, const std::string &paBody,
    const std::string& paContentType, CHttpComLayer::ERequestType paType) {
  addCommonHeader(paDest, paHost, paPort, paPath, paAuth, paParams, paType);
  addBodyToRequest(paBody, paContentType, paDest);
}

bool CHttpParser::parseResponse(std::string& paBody, std::string& paResponseCode, char* paSrc) {
  if(getHttpResponseCode(paResponseCode, paSrc)) {
    char* helperChar = strstr(paSrc, "\r\n\r\n"); // Extract data from HTTP response char
    if(nullptr != helperChar) {
      helperChar += sizeof("\r\n\r\n") - 1;
      paBody = std::string(helperChar);
    } else { // Empty response received
      DEVLOG_INFO("[HTTP Parser] Empty content response received\n");
      paBody = std::string("");
    }
    return true;
  }
  return false;
}

bool forte::com_infra::CHttpParser::parseGetRequest(std::string& paPath, CSinglyLinkedList<std::string>& paParameterNames,
    CSinglyLinkedList<std::string>& paParameterValues, char* paData) {
  if(0 == strncmp(paData, "GET ", 4)) {
    paData = paData + 4;

    char* endOfPath = strstr(paData, " ");
    if(endOfPath != nullptr) {
      *endOfPath = '\0';
      char* startOfParameters = strstr(paData + 1, "?");
      if(startOfParameters != nullptr) {
        *startOfParameters = '\0';
        startOfParameters++;
        parseGETParameters(startOfParameters, paParameterNames, paParameterValues);
      }
      paPath = std::string(paData);
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

bool forte::com_infra::CHttpParser::parsePutPostRequest(std::string& paPath, std::string &paContent, char* paData) {
  if(0 == strncmp(paData, "PUT ", 4)) {
    paData += sizeof("PUT ") - 1;
  } else if(0 == strncmp(paData, "POST ", 5)) {
    paData += sizeof("POST ") - 1;
  } else {
    DEVLOG_ERROR("[HTTP Parser] Invalid HTTP PUT/POST request. No PUT/POST string found\n");
    return false;
  }

  char* endOfPath = strstr(paData, " ");
  if(endOfPath != nullptr) {
    *endOfPath = '\0';
    paPath = std::string(paData);
    paData = strstr(endOfPath + 1, "\r\n\r\n");
    if(paData != nullptr) {
      paData += sizeof("\r\n\r\n") - 1;
      paContent = std::string(paData);
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

void forte::com_infra::CHttpParser::createResponse(std::string& paDest, const std::string& paResult, const std::string& paContentType,
    const std::string& paBody) {
  paDest = paResult;
  addBodyToRequest(paBody, paContentType, paDest);
}

void CHttpParser::addCommonHeader (std::string &paDest, const std::string &paHost, const TForteUInt16 paPort, const std::string &paPath, const std::string &paAuth, const std::string &paParams, CHttpComLayer::ERequestType paType) {
  switch(paType){
    case CHttpComLayer::e_GET:
      paDest = "GET "s;
      break;
    case CHttpComLayer::e_PUT:
      paDest = "PUT "s;
      break;
    case CHttpComLayer::e_POST:
      paDest = "POST "s;
      break;
    default:
      DEVLOG_ERROR("[HTTP Parser] Unexpected HTTP Type when adding header\n");
      break;
  }
  paDest += paPath;
  if(!paParams.empty()){
    paDest += "?"s;
    paDest += paParams;
  }
  paDest += " HTTP/1.1\r\n"s;
  paDest += "Host: "s;
  paDest += paHost;
  paDest += ":"s;
  paDest += std::to_string(paPort);
  if(!paAuth.empty()){
    paDest += "\r\nAuthorization: "s;
    paDest += paAuth;
  }
}

void CHttpParser::addHeaderEnding(std::string& paDest) {
  paDest += "\r\n\r\n"s;
}

bool CHttpParser::getHttpResponseCode(std::string& paDest, char* paSrc) {
  //HTTP-Version SP Status-Code SP Reason-Phrase CRLF (SP = space)
  char* helperChar = strstr(paSrc, "\r\n");
  if(helperChar != nullptr) {
    *helperChar = '\0';
    CParameterParser parser(paSrc, ' ');
    if(3 <= parser.parseParameters()) { //Reason-Phrase can contain spaces in it
      paDest = std::string(parser[1]);
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

unsigned int forte::com_infra::CHttpParser::parseGETParameters(char* paParameters, CSinglyLinkedList<std::string>& paParameterNames,
    CSinglyLinkedList<std::string>& paParameterValues) {
  paParameterNames.clearAll();
  paParameterValues.clearAll();

  unsigned int retVal = 0;

  char* startOfName = paParameters;
  bool endOfParameters = false;
  while('\0' != *startOfName && !endOfParameters) {
    char* startOfValue = strstr(startOfName, "=");
    if(nullptr != startOfValue) {
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
    if(nullptr != nextName) {
      *nextName = '\0';
    } else {
      endOfParameters = true;
    }
    paParameterNames.pushBack(std::string(startOfName));
    paParameterValues.pushBack(std::string(startOfValue));
    retVal++;
    if(!endOfParameters) {
      startOfName = ++nextName;
    }
  }

  return retVal;
}
