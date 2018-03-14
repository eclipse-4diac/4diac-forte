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

#ifndef _HTTPIPPARSER_H_
#define _HTTPIPPARSER_H_

#include <stdio.h>

namespace forte {

namespace com_infra {

    class CHttpParser {
    public:
      CHttpParser();
      virtual ~CHttpParser();

      /**
      * Generates a HTTP GET request. The request is written to the "dest" input.
      * params: host and path, e.g., 192.168.1.144:80/example/path
      */
      virtual void createGetRequest(char* paDest, const char* paParams);
      /**
      * Generates a HTTP PUT request. The request is written to the "dest" input.
      * params: host and path, e.g., 192.168.1.144:80/example/path
      * data: Data to be written to the server location.
      */
      virtual void createPutRequest(char* paDest, const char* paParams, const char* paData);
      /**
      * Extracts data from a response to a HTTP GET request stored in src and writes it to dest.
      * If the respones header is not HTTP OK, the header is copied instead.
      * Returns true if the response indicates a successful request.
      */
      virtual bool parseGetResponse(char* paDest, char* paSrc);
      /**
      * Extracts the header from an HTTP response.
      * Returns true if the response indicates a successful request.
      */
      virtual bool parsePutResponse(char* paDest, char* paSrc);
      /**
      * Sets the expected response.
      */
      virtual void setExpectedRspCode(const char* paRsp);


    private:

      /** Appends the host to the HTTP request */
      static void addHost(char* paDest, const char* paHost);
      /** Appends the ending "\r\n\r\n" to the HTTP request */
      static void addRequestEnding(char* paDest);
      /** Extracts the HTTP response code from src and writes it to dest */
      static void getHttpResponseCode(char* paDest, char* paSrc);

      /** Returns true if HTTP response code is HTTP/1.1 200 OK */
      bool isOKresponse(char* paResponse);

      /** Size with which to allocate char arrays */
      static const size_t kAllocSize = 512;

      /** Expected response code (default: HTTP/1.1 200 OK) */
      char mExpectedRspCode[kAllocSize];
    };
  }

}

#endif /* _HTTPIPPARSER_H_ */
