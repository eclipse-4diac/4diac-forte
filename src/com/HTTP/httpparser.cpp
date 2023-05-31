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
#include "devlog.h"

using namespace forte::com_infra;
using namespace std::string_literals;

void CHttpParser::createGetRequest(std::string& paDest, const std::string& paHost, const std::string& paPath) {
  CHttpParser::addCommonHeader(paDest, paHost, paPath, CHttpComLayer::e_GET);
  CHttpParser::addHeaderEnding(paDest);
}

void CHttpParser::createPutPostRequest(std::string& paDest, const std::string& paHost, const std::string& paPath, const std::string& paData,
    const std::string& paContentType, CHttpComLayer::ERequestType paType) {
  CHttpParser::addCommonHeader(paDest, paHost, paPath, paType);
  paDest.append("\r\nContent-type: "s);
  paDest.append(paContentType);
  paDest.append("\r\nContent-length: "s);
  changePutPostData(paDest, paData);
}

bool CHttpParser::changePutPostData(std::string& paDest, const std::string& paData) {
  char* helperChar = strstr(paDest.data(), "length: ");
  if(nullptr != helperChar) {
    helperChar += sizeof("length: ") - 1;
    *helperChar = '\0';
    paDest = std::string(paDest.c_str()); // will shrink the length of the string to the new ending
    char contentLength[std::numeric_limits<size_t>::digits10 + 1];
    snprintf(contentLength, sizeof(contentLength), "%zu", strlen(paData.c_str()));
    paDest.append(contentLength);
    CHttpParser::addHeaderEnding(paDest);
    paDest.append(paData);
    return true;
  } else { // wrong request?
    DEVLOG_ERROR("[HTTP Parser] PUT/POST request was wrongly created\n");
    return false;
  }
}

bool CHttpParser::parseResponse(std::string& paBody, std::string& paResponseCode, char* paSrc) {
  if(CHttpParser::getHttpResponseCode(paResponseCode, paSrc)) {
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
    const std::string& paData) {
  paDest = paResult;
  if(paData.empty()) {
    paDest.append("\r\n"s);
  } else {
    paDest.append("\r\nContent-type: "s);
    paDest.append(paContentType);
    paDest.append("\r\nContent-length: "s);
    changePutPostData(paDest, paData);
  }
}

void CHttpParser::addCommonHeader(std::string& paDest, const std::string& paHost, const std::string& paPath, CHttpComLayer::ERequestType paType) {
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

  paDest.append(paPath);
  paDest.append(" HTTP/1.1\r\n"s);
  paDest.append("Host: "s);
  paDest.append(paHost);
}

void CHttpParser::addHeaderEnding(std::string& paDest) {
  paDest.append("\r\n\r\n"s);
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
