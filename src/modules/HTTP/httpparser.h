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

#ifndef _HTTPIPPARSER_H_
#define _HTTPIPPARSER_H_

#include <forte_string.h>
#include "httplayer.h"
#include "fortelist.h"

namespace forte {

  namespace com_infra {

    class CHttpParser {
      public:
        /**
         * Generates a HTTP GET request. The request is written to paDest
         * @param paDest place to store the request
         * @param paHost host of the server to connect
         * @param paPath path to look for
         */
        static void createGetRequest(CIEC_STRING& paDest, const CIEC_STRING& paHost, const CIEC_STRING& paPath);

        /**
         * Generates a HTTP PUT request. The request is written to paDest.
         * @param paDest place to store the request
         * @param paHost host of the server to connect
         * @param paPath path to access in the server
         * @param paData data in the body to send to the server
         * @param paContentType content type of the request
         * @param paType PUT or POST
         */
        static void createPutPostRequest(CIEC_STRING& paDest, const CIEC_STRING& paHost, const CIEC_STRING& paPath, const CIEC_STRING& paData,
            const CIEC_STRING& paContentType, CHttpComLayer::ERequestType paType);

        /**
         * Takes a previous PUT request in paDest, and updates the content of it
         * @param paDest old request
         * @param paData new data
         * @return true if no error happened, false otherwise
         */
        static bool changePutPostData(CIEC_STRING& paDest, const CIEC_STRING& paData);

        /**
         * Extracts data from a response to a HTTP request
         * @param paDest place to store the body of the response
         * @param paResponseCode place to store the response code of the response
         * @param paSrc response received
         * @return true if no error happened, false otherwise
         */
        static bool parseResponse(CIEC_STRING& paBody, CIEC_STRING& paResponseCode, char* paSrc);

        /**
         * Parse a GET request received as a server
         * @param paPath place to store the received path
         * @param paParameterNames place to store the names of the parameters
         * @param paParameterValues place to store the value of the parameters
         * @param paData the received  GET request
         * @return true if no error happened, false otherwise
         */
        static bool parseGetRequest(CIEC_STRING& paPath, CSinglyLinkedList<CIEC_STRING>& paParameterNames, CSinglyLinkedList<CIEC_STRING>& paParameterValues,
            char* paData);

        /**
         * Parse a PUT/POST request received as a server
         * @param paPath place to store the received path
         * @param paContent place to store the content of the request
         * @param paData the received PUT/POST  request
         * @return true if no error happened, false otherwise
         */
        static bool parsePutPostRequest(CIEC_STRING& paPath, CIEC_STRING &paContent, char* paData);

        /**
         * Create a HTTP response
         * @param paDest
         * @param paResult the status line of the response
         * @param paContentType the content type of the response
         * @param paData the body of the response
         */
        static void createResponse(CIEC_STRING& paDest, const CIEC_STRING& paResult, const CIEC_STRING& paContentType, const CIEC_STRING& paData);

        /**
         * Look in the request for its type
         * @param paRequest request to look into
         * @return Type of request
         */
        static CHttpComLayer::ERequestType getTypeOfRequest(const char* paRequest);

      private:
        CHttpParser();
        virtual ~CHttpParser();

        /**
         *  Creates the common parts of the headers of the supported types
         * @param paDest place to store the header
         * @param paHost server host to be written in the header
         * @param paPath the path to be written in the header
         * @param paType the type of request to be written in the header
         */
        static void addCommonHeader(CIEC_STRING& paDest, const CIEC_STRING& paHost, const CIEC_STRING& paPath, CHttpComLayer::ERequestType paType);

        /**
         * Appends the ending "\r\n\r\n" to the HTTP request
         * @param paDest place to store the header
         */
        static void addHeaderEnding(CIEC_STRING& paDest);

        /**
         * Extracts the HTTP response
         * @param paDest place to store the header
         * @param paSrc source of the reponse
         * @return true if no error happened, false otherwise
         */
        static bool getHttpResponseCode(CIEC_STRING& paDest, char* paSrc);

        /**
         * Parse parameter of GET request as a server
         * @param paParameters The string containing the parameters of the request
         * @param paParameterNames The result where the names of the parameters are stored
         * @param paParameterValues The result where the values of the parameters are stored
         * @return The number of parameters found. If wrong parameters are found, the result is treated as no parameters, meaning it returns zero
         */
        static unsigned int parseGETParameters(char* paParameters, CSinglyLinkedList<CIEC_STRING>& paParameterNames,
            CSinglyLinkedList<CIEC_STRING>& paParameterValues);

        static const size_t scmMaxLengthOfContent = 6; //The limit of the amount to send in a PUT/POST request is set to 99999 bytes for now. Change this to try to send more
    };
  }
}

#endif /* _HTTPIPPARSER_H_ */
