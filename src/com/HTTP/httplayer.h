/*******************************************************************************
 * Copyright (c) 2017-2018 Marc Jakobi, github.com/MrcJkb, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Marc Jakobi - initial implementation for HTTP clients
 *    Jose Cabral - Merge old HTTPIpLayer to this one and use CIEC_STRING
 ********************************************************************************/

#ifndef _HTTPCOMLAYER_H_
#define _HTTPCOMLAYER_H_

#include <forte_config.h>
#include <forte_sem.h>
#include "comlayer.h"
#include <forte_string.h>
#include "ipcomlayer.h"
#include "../../core/utils/parameterParser.h"

namespace forte {

  namespace com_infra {

    class CHttpComLayer : public CComLayer {
      public:
        CHttpComLayer(CComLayer* paUpperLayer, CBaseCommFB* paComFB);
        virtual ~CHttpComLayer();

        EComResponse sendData(void *paData, unsigned int paSize); // top interface, called from top
        EComResponse recvData(const void *paData, unsigned int paSize);

        EComResponse recvServerData(CSinglyLinkedList<CIEC_STRING>& paParameterNames, CSinglyLinkedList<CIEC_STRING>& paParameterValues);

        EComResponse openConnection(char* paLayerParameter);

        void closeConnection();

        EComResponse processInterrupt();

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

        CIEC_STRING& getHost();

        TForteUInt16 getPort() const;

      private:

        /**
         * Parse the HTTP response and checks the returned code
         * @param paData buffer with the HTTP response
         * @return OK if return code is as expected
         */
        EComResponse handleHTTPResponse(char *paData);

        /** Serializes the data to a char* */
        bool serializeData(const CIEC_ANY& paCIECData);

        void sendDataAsServer(const void *paData);

        void sendDataAsClient(const void *paData);

        EComResponse openClientConnection(char* paLayerParameter);

        bool checkSDInPOSTAndPUT(size_t paNoOfSD);

        bool checkSDsAndRDsType() const;

        bool handleAddress(const char* paAddress, size_t paNoOfSDs);

        bool handleContentAndRequestType(CParameterParser &paParser, size_t paNoOfParameters);

        bool storeRequestType(const char* paType);

        EComResponse mInterruptResp;

        /** Represents the HTTP request type (0 = GET, 1 = PUT). */
        ERequestType mRequestType;
        /** HTTP Host */
        CIEC_STRING mHost;
        /** Path in host */
        CIEC_STRING mPath;
        /** Data to be sent */
        CIEC_STRING mReqData;
        /** Port of the host */
        TForteUInt16 mPort;
        /** Request  to be sent to Host */
        CIEC_STRING mRequest;

        char mRecvBuffer[cg_unIPLayerRecvBufferSize];
        unsigned int mBufFillSize;

        CIEC_STRING mContentType;

        bool mCorrectlyInitialized;

        bool mHasParameterInSD;
    };

  }
}

#endif /* _HTTPCOMLAYER_H_ */
