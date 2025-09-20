/*******************************************************************************
 * Copyright (c) 2018, 2023 fortiss GmbH
 *               HIT robot group
 *               Primetals Austria GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Jose Cabral - initial API and implementation and/or initial documentation
 *    Tibalt Zhao - remove the client http layer when socket error in recv
 *    Martin Melik Merkumians - Change CIEC_STRING to std::string
 *******************************************************************************/

#pragma once

#include "forte/arch/forte_thread.h"
#include "forte/extevhan.h"
#include "forte/conn.h"
#include <stdio.h>
#include "forte/arch/sockhand.h"
#include "forte/datatypes/forte_string.h"
#include "httplayer.h"
#include "forte/cominfra/comCallback.h"
#include "forte/datatypes/forte_date_and_time.h"

#include <vector>

namespace forte::com_infra::http {
  // cppcheck-suppress noConstructor
  class CHTTP_Handler : public CExternalEventHandler,
                        public RegisterExternalEventHandler<CHTTP_Handler>,
                        public arch::CThread,
                        public CComCallback {
    public:
      explicit CHTTP_Handler(CDeviceExecution &paDeviceExecution);
      ~CHTTP_Handler() override;

      /* functions needed for the external event handler interface */
      void enableHandler() override;

      void disableHandler() override;

      EComResponse recvData(const void *paData, unsigned int paSize) override;

      bool sendClientData(CHttpComLayer *paLayer, const std::string &paToSend);

      bool addServerPath(CHttpComLayer *paLayer, const std::string &paPath);

      void removeServerPath(const std::string &paPath);

      void sendServerAnswer(CHttpComLayer *paLayer, const std::string &paAnswer);

      void sendServerAnswerFromRecv(CHttpComLayer *paLayer, const std::string &paAnswer);

      void forceClose(CHttpComLayer *paLayer);

      void forceCloseFromRecv(CHttpComLayer *paLayer);

    private:
      /**
       * Overridden run() from CThread which loops the UA Server.
       */
      void run() override;

      void checkClientLayers();

      void checkAcceptedSockets();

      void startTimeoutThread();

      void stopTimeoutThread();

      void openHTTPServer();

      void closeHTTPServer();

      void removeAndCloseSocket(const arch::CIPComSocketHandler::TSocketDescriptor paSocket);

      bool removeHTTPLayerFromClientList(const arch::CIPComSocketHandler::TSocketDescriptor paSocket);

      void resumeSelfsuspend();

      void selfSuspend();

      void sendServerAnswerHelper(CHttpComLayer *paLayer, const std::string &paAnswer, bool paFromRecv);

      void forceCloseHelper(CHttpComLayer *paLayer, bool paFromRecv);

      bool recvClients(const arch::CIPComSocketHandler::TSocketDescriptor paSocket, const int paRecvLength);

      bool recvServers(const arch::CIPComSocketHandler::TSocketDescriptor paSocket);

      void removeSocketFromAccepted(const arch::CIPComSocketHandler::TSocketDescriptor paSocket);

      void handlerReceivedWrongPath(const arch::CIPComSocketHandler::TSocketDescriptor paSocket,
                                    const std::string &paPath);

      void clearServerLayers();

      void clearClientLayers();

      void clearAcceptedSockets();

      struct HTTPServerWaiting {
          CHttpComLayer *mLayer;
          std::string mPath;
          std::vector<arch::CIPComSocketHandler::TSocketDescriptor>
              mSockets; // to handle many connections to the same path
      };

      struct HTTPClientWaiting {
          CHttpComLayer *mLayer;
          arch::CIPComSocketHandler::TSocketDescriptor mSocket;
          CIEC_TIME mStartTime;
      };

      struct HTTPAcceptedSockets {
          arch::CIPComSocketHandler::TSocketDescriptor mSocket;
          CIEC_TIME mStartTime;
      };

      std::vector<HTTPServerWaiting> mServerLayers;
      arch::CSyncObject mServerMutex;

      std::vector<HTTPClientWaiting> mClientLayers;
      arch::CSyncObject mClientMutex;

      std::vector<HTTPAcceptedSockets> mAcceptedSockets;
      arch::CSyncObject mAcceptedMutex;

      arch::CSemaphore mSuspendSemaphore;

      static arch::CIPComSocketHandler::TSocketDescriptor smServerListeningSocket;

      static char sRecvBuffer[cgIPLayerRecvBufferSize];
      static unsigned int sBufFillSize;

      static const unsigned int scmSendTimeout;
      static const unsigned int scmAcceptedTimeout;

      arch::CSemaphore mThreadStarted;
  };
} // namespace forte::com_infra::http
