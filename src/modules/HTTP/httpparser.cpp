/*******************************************************************************
*
********************************************************************************
*******************************************************************************
* Copyright (c) 2017-2018 Marc Jakobi, github.com/MrcJkb
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
*
* Contributors:
*    Marc Jakobi - initial implementation
*******************************************************************************
******************************************************************************
*
*
********************************************************************************
* Class for parsing HTTP requests and responses
********************************************************************************/
#include "httpparser.h"
#include <stdio.h>
#include <string.h>
#include "devlog.h"

using namespace forte::com_infra;

CHttpParser::CHttpParser() {
  setExpectedRspCode("HTTP/1.1 200 OK");
}

CHttpParser::~CHttpParser(){
}

void CHttpParser::createGetRequest(char* paDest, const char* paParams) {
  char ipParams[kAllocSize]; // address + port
  char path[kAllocSize]; // path for HTTP request
  sscanf(paParams, "%99[^/]/%511s[^/n]", ipParams, path);
  strncpy(paDest, "GET /", 6);
  strncat(paDest, path, kAllocSize);
  CHttpParser::addHost(paDest, ipParams);
  CHttpParser::addRequestEnding(paDest);
}

void CHttpParser::createPutRequest(char* paDest, const char* paParams, const char* paData) {
  char ipParams[kAllocSize]; // address + port
  char path[kAllocSize]; // path for HTTP request
  sscanf(paParams, "%99[^/]/%511s[^/n]", ipParams, path);
  strncpy(paDest, "PUT /", 6);
  strncat(paDest, path, kAllocSize);
  CHttpParser::addHost(paDest, ipParams);
  strncat(paDest, "\r\nContent-type: text/html\r\nContent-length: ", 43);
  char contentLength[kAllocSize];
  sprintf(contentLength, "%zu", strlen(paData));
  strncat(paDest, contentLength, kAllocSize);
  CHttpParser::addRequestEnding(paDest);
  strncat(paDest, paData, kAllocSize);
}

void CHttpParser::addHost(char* paDest, const char* paHost) {
  strncat(paDest, " HTTP/1.1\r\nHost: ", 17);
  strncat(paDest, paHost, kAllocSize);
}

void CHttpParser::addRequestEnding(char* paDest) {
  strncat(paDest, "\r\n\r\n", 4);
}

bool CHttpParser::parseGetResponse(char* paDest, char* paSrc) {
  if (CHttpParser::isOKresponse(paSrc)) {
    // Extract data from HTTP GET respnse char
    char* data = strstr(paSrc, "\r\n\r\n");
    if (0 != data && strlen(data) > 6) { // \r\n\r\n + body + \r\n
        data += 4;
        sscanf(data, "%99s[^/n])", paDest);
        return true;
    }
    DEVLOG_INFO("Empty HTTP response received\n");
    // Empty response received?
    strncpy(paDest, "0", 2);
    return false;
  }
  // Bad response received?
  DEVLOG_INFO("Unexpected HTTP GET response code\n");
  CHttpParser::getHttpResponseCode(paDest, paSrc);
  return false;
}

bool CHttpParser::parsePutResponse(char* paDest, char* paSrc) {
  if (CHttpParser::isOKresponse(paSrc)) {
    strcpy(paDest, mExpectedRspCode);
    return true;
  }
  DEVLOG_INFO("Unexpected HTTP PUT response code\n");
  CHttpParser::getHttpResponseCode(paDest, paSrc);
  return false;
}

void CHttpParser::getHttpResponseCode(char* paDest, char* paSrc) {
  char* tmp = strtok(paSrc, "\r\n");
  if (tmp != 0) {
    memcpy(paDest, tmp, strlen(tmp) + 1);
  }
  else {
    DEVLOG_INFO("Invalid HTTP response\n");
    strncpy(paDest, "Invalid response", 17);
  }
}

bool CHttpParser::isOKresponse(char* paResponse) {
  if (strnlen(paResponse, 16) > 15) {
    return 0 != strstr(paResponse, mExpectedRspCode);
  }
  return false;
}

void CHttpParser::setExpectedRspCode(const char* paRsp) {
  strcpy(mExpectedRspCode, paRsp);
}
