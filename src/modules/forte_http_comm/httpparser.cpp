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

void CHttpParser::createGetRequest(char* dest, const char* params) {
	char ipParams[kAllocSize]; // address + port
	char path[kAllocSize]; // path for HTTP request
	sscanf(params, "%99[^/]/%511s[^/n]", ipParams, path);
	strncpy(dest, "GET /", 6);
	strncat(dest, path, kAllocSize);
	CHttpParser::addHost(dest, ipParams);
	CHttpParser::addRequestEnding(dest);
}

void CHttpParser::createPutRequest(char* dest, const char* params, const char* data) {
	char ipParams[kAllocSize]; // address + port
	char path[kAllocSize]; // path for HTTP request
	sscanf(params, "%99[^/]/%511s[^/n]", ipParams, path);
	strncpy(dest, "PUT /", 6);
	strncat(dest, path, kAllocSize);
	CHttpParser::addHost(dest, ipParams);
	strncat(dest, "\r\nContent-type: text/html\r\nContent-length: ", 43);
	char contentLength[kAllocSize];
	sprintf(contentLength, "%zu", strlen(data));
	strncat(dest, contentLength, kAllocSize);
	CHttpParser::addRequestEnding(dest);
	strncat(dest, data, kAllocSize);
}

void CHttpParser::addHost(char* dest, const char* host) {
	strncat(dest, " HTTP/1.1\r\nHost: ", 17);
	strncat(dest, host, kAllocSize);
}

void CHttpParser::addRequestEnding(char* dest) {
	strncat(dest, "\r\n\r\n", 4);
}

bool CHttpParser::parseGetResponse(char* dest, char* src) {
	if (CHttpParser::isOKresponse(src)) {
		// Extract data from HTTP GET respnse char
		char* data = strstr(src, "\r\n\r\n");
		if (strlen(data) > 6) { // \r\n\r\n + body + \r\n
			if (0 != data) {
				data += 4;
				sscanf(data, "%99s[^/n])", dest);
				return true;
			}
		}
		DEVLOG_INFO("Empty HTTP response received\n");
		// Empty response received?
		strncpy(dest, "0", 2);
		return false;
	}
	// Bad response received?
	DEVLOG_INFO("Unexpected HTTP GET response code\n");
	CHttpParser::getHttpResponseCode(dest, src);
	return false;
}

bool CHttpParser::parsePutResponse(char* dest, char* src) {
	if (CHttpParser::isOKresponse(src)) {
		strcpy(dest, mExpectedRspCode);
		return true;
	}
	DEVLOG_INFO("Unexpected HTTP PUT response code\n");
	CHttpParser::getHttpResponseCode(dest, src);
	return false;
}

void CHttpParser::getHttpResponseCode(char* dest, char* src) {
	char* tmp = strtok(src, "\r\n");
	if (tmp != 0) {
		memcpy(dest, tmp, strlen(tmp) + 1);
	}
	else {
		DEVLOG_INFO("Invalid HTTP response\n");
		strncpy(dest, "Invalid response", 17);
	}
}

bool CHttpParser::isOKresponse(char* response) {
	if (strnlen(response, 16) > 15) {
		return 0 != strstr(response, mExpectedRspCode);
	}
	return false;
}

void CHttpParser::setExpectedRspCode(const char* rsp) {
	strcpy(mExpectedRspCode, rsp);
}
