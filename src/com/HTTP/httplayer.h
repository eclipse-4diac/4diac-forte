/*******************************************************************************
 * Copyright (c) 2017, 2023 Marc Jakobi, github.com/MrcJkb
 *                          fortiss GmbH
 *                          Primetals Technologies Austria GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Marc Jakobi - initial implementation for HTTP clients
 *    Jose Cabral - Merge old HTTPIpLayer to this one and use CIEC_STRING
 *    Martin Melik Merkumians - change CIEC_STRING to std::string
 ********************************************************************************/

#ifndef _HTTPCOMLAYER_H_
#define _HTTPCOMLAYER_H_

#include <forte_config.h>
#include <forte_sem.h>
#include "comlayer.h"
#include <forte_string.h>
#include "ipcomlayer.h"
#include "core/util/parameterParser.h"

namespace forte {

  namespace com_infra {

    class CHttpComLayer : public CComLayer {
      public:
        CHttpComLayer(CComLayer* paUpperLayer, CBaseCommFB* paComFB);
        ~CHttpComLayer() override;

        EComResponse sendData(void *paData, unsigned int paSize) override; // top interface, called from top
        EComResponse recvData(const void *paData, unsigned int paSize) override;

        EComResponse recvServerData(CSinglyLinkedList<std::string> &paParameterNames, CSinglyLinkedList<std::string> &paParameterValues);

        EComResponse openConnection(char* paLayerParameter) override;

        EComResponse startServer(char* paLayerParameter);

        void closeConnection() override;

        EComResponse processInterrupt() override;

        /** enum representing the HTTP request type */
        enum ERequestType {
          /** HTTP GET */
          e_GET,
          /** HTTP PUT */
          e_PUT,
          /** HTTP PUT */
          e_POST,
          /** not ready */
          e_NOTSET,
        };

        std::string &getHost();

        TForteUInt16 getPort() const;

      private:

        /**
         * Parse the HTTP response and checks the returned code
         * @param paData buffer with the HTTP response
         * @return OK if return code is as expected
         */
        EComResponse handleHTTPResponse(char *paData);

        /** Serializes the data to a char* */
        void serializeData(const CIEC_ANY& paSDx, std::string& paMember);

        void sendDataAsServer(void *paData);

        void sendDataAsClient(void *paData);

        const CIEC_ANY& getSDx(void *paData, int paSdNum);

        EComResponse openClientConnection(char* paLayerParameter);

        bool checkSDsAndRDsType() const;

        bool handleAddress(const char* paAddress);

        bool handleContentAndRequestType(CParameterParser &paParser, size_t paNoOfParameters);

        bool storeRequestType(const char* paType);

        void createRequest();
      EComResponse startServer (EComResponse eRetVal, char *paLayerParameter);

        EComResponse mInterruptResp;

        /** Represents the HTTP request type (0 = GET, 1 = PUT). */
        ERequestType mRequestType;
        /** HTTP Host */
        std::string mHost;
        /** Path in host */
        std::string mPath;
        /** Parameters */
        std::string mParams;
        /** Data to be sent */
        std::string mBody;
        /** Port of the host */
        TForteUInt16 mPort;
        /** Authentication */
        std::string mAuth;
        /** Request  to be sent to Host */
        std::string mRequest;

        char mRecvBuffer[cgIPLayerRecvBufferSize];
        unsigned int mBufFillSize;

        std::string mContentType;

        bool mCorrectlyInitialized;
    };

  }
}

#endif /* _HTTPCOMLAYER_H_ */
