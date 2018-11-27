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

#ifndef _HTTPIPPARSER_H_
#define _HTTPIPPARSER_H_

#include <forte_string.h>
#include "httplayer.h"
#include "fortelist.h"

namespace forte {

  namespace com_infra {

    class CHttpParser{
      public:
        CHttpParser();
        virtual ~CHttpParser();

        /**
         * Generates a HTTP GET request. The request is written to paDest.
         * paHost: host to send the request, e.g., 192.168.1.144:80
         * paPath: path to retrieve, e.g., /example/path
         */
        static void createGetRequest(CIEC_STRING& paDest, const CIEC_STRING& paHost, const CIEC_STRING& paPath);
        /**
         * Generates a HTTP PUT request. The request is written to paDest.
         * paHost: host to send the request, e.g., 192.168.1.144:80
         * paPath: path to send, e.g., /example/path
         * data: Data to be written to the server location.
         */
        static void createPutPostRequest(CIEC_STRING& paDest, const CIEC_STRING& paHost, const CIEC_STRING& paPath, const CIEC_STRING& paData, const CIEC_STRING& paContentType, CHttpComLayer::ERequestType paType);
        /**
         * Extracts data from a response to a HTTP GET request stored in paSrc and writes it to dest.
         * If the response header is not as paExpectedCode, the header is copied instead.
         * Returns true if the response indicates a successful request.
         */
        static bool parseGetResponse(CIEC_STRING& paDest, char* paSrc, CIEC_STRING& paExpectedCode);
        /**
         * Extracts the header from an HTTP PUT response.
         * Returns true if the response indicates a successful request.
         */
        static bool parsePutPostResponse(CIEC_STRING& paDest, char* paSrc, CIEC_STRING& paExpectedCode);

        /**
         * Takes a previous PUT request in paDest, and updates the content of it
         * Returns true if the response indicates a successful request.
         */
        static bool changePutPostData(CIEC_STRING& paDest, const CIEC_STRING& paData);

        static bool parseGetRequest(CIEC_STRING& paPath, CSinglyLinkedList<CIEC_STRING>& paParameterNames, CSinglyLinkedList<CIEC_STRING>& paParameterValues, char* paData);

        static bool createResponse(CIEC_STRING& paDest, const CIEC_STRING& paResult, const CIEC_STRING& paContentType, const CIEC_STRING& paData);

      private:

        /** Creates the common parts of the headers of the supported types */
        static void addCommonHeader(CIEC_STRING& paDest, const CIEC_STRING& paHost, const CIEC_STRING& paPath, CHttpComLayer::ERequestType paType);
        /** Appends the ending "\r\n\r\n" to the HTTP request */
        static void addHeaderEnding(CIEC_STRING& paDest);
        /** Extracts the HTTP response code from paSrc and writes it to paDest */
        static bool getHttpResponseCode(CIEC_STRING& paDest, char* paSrc);

        /**
         *
         * @param paParameters The string containing the parameters of the request
         * @param paParameterNames The result where the names of the parameters are stored
         * @param paParameterValues The result where the values of the parameters are stored
         * @return The number of parameters found. If wrong parameters are found, the result is treated as no parameters, meaning it returns zero
         */
        static unsigned int parseParameters(char* paParameters, CSinglyLinkedList<CIEC_STRING>& paParameterNames, CSinglyLinkedList<CIEC_STRING>& paParameterValues);
    };
  }
}

#endif /* _HTTPIPPARSER_H_ */
