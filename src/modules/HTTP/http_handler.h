/*******************************************************************************
 * Copyright (c) 2018 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Jose Cabral - initial API and implementation and/or initial documentation
 *******************************************************************************/

#ifndef SRC_MODULES_HTTP_OPCUAHANDLER_H_
#define SRC_MODULES_HTTP_OPCUAHANDLER_H_

#include <forte_thread.h>
#include <extevhan.h>
#include <conn.h>
#include <stdio.h>
#include <sockhand.h>
#include "forte_string.h"
#include "httplayer.h"
#include "comCallback.h"
#include "forte_date_and_time.h"

// cppcheck-suppress noConstructor
class CHTTP_Handler : public CExternalEventHandler, public CThread, public forte::com_infra::CComCallback {
  DECLARE_HANDLER(CHTTP_Handler)

  public:
    /* functions needed for the external event handler interface */
    void enableHandler(void);

    void disableHandler(void);

    void setPriority(int pa_nPriority);

    int getPriority(void) const;

    virtual forte::com_infra::EComResponse recvData(const void *paData, unsigned int paSize);

    bool sendClientData(forte::com_infra::CHttpComLayer* paLayer, CIEC_STRING& paToSend);

    bool addServerPath(forte::com_infra::CHttpComLayer* paLayer, CIEC_STRING& paPath);

    void removeServerPath(CIEC_STRING& paPath);

    void sendServerAnswer(forte::com_infra::CHttpComLayer* paLayer, CIEC_STRING& paAnswer);

    void sendServerAnswerFromRecv(forte::com_infra::CHttpComLayer* paLayer, CIEC_STRING& paAnswer);

    void forceClose(forte::com_infra::CHttpComLayer* paLayer);

    void forceCloseFromRecv(forte::com_infra::CHttpComLayer* paLayer);

  private:

    /**
     * Overridden run() from CThread which loops the UA Server.
     */
    virtual void run();

    void checkClientLayers();

    void checkAcceptedSockets();

    void startTimeoutThread();

    void stopTimeoutThread();

    void openHTTPServer();

    void closeHTTPServer();

    void removeAndCloseSocket(const CIPComSocketHandler::TSocketDescriptor paSocket);

    void resumeSelfsuspend();

    void selfSuspend();

    void sendServerAnswerHelper(forte::com_infra::CHttpComLayer* paLayer, CIEC_STRING& paAnswer, bool paFromRecv);

    void forceCloseHelper(forte::com_infra::CHttpComLayer* paLayer, bool paFromRecv);

    bool recvClients(const CIPComSocketHandler::TSocketDescriptor paSocket, const int paRecvLength);

    bool recvServers(const CIPComSocketHandler::TSocketDescriptor paSocket);

    void removeSocketFromAccepted(const CIPComSocketHandler::TSocketDescriptor paSocket);

    void handlerReceivedWrongPath(const CIPComSocketHandler::TSocketDescriptor paSocket, CIEC_STRING& paPath);

    void clearServerLayers();

    void clearClientLayers();

    void clearAcceptedSockets();

    struct HTTPServerWaiting {
        forte::com_infra::CHttpComLayer* mLayer;
        CIEC_STRING mPath;
        CSinglyLinkedList<CIPComSocketHandler::TSocketDescriptor> mSockets; //to handle many connections to the same path
    };

    struct HTTPClientWaiting {
        forte::com_infra::CHttpComLayer* mLayer;
        CIPComSocketHandler::TSocketDescriptor mSocket;
        CIEC_DATE_AND_TIME mStartTime;
    };

    struct HTTPAcceptedSockets {
        CIPComSocketHandler::TSocketDescriptor mSocket;
        CIEC_DATE_AND_TIME mStartTime;
    };

    CSinglyLinkedList<HTTPServerWaiting*> mServerLayers;
    CSyncObject mServerMutex;

    CSinglyLinkedList<HTTPClientWaiting*> mClientLayers;
    CSyncObject mClientMutex;

    CSinglyLinkedList<HTTPAcceptedSockets*> mAcceptedSockets;
    CSyncObject mAcceptedMutex;

    CSemaphore mSuspendSemaphore;

    static CIPComSocketHandler::TSocketDescriptor smServerListeningSocket;

    static char sRecvBuffer[cg_unIPLayerRecvBufferSize];
    static unsigned int sBufFillSize;

    static const unsigned int scmSendTimeout;
    static const unsigned int scmAcceptedTimeout;

    CSemaphore mThreadStarted;
};

#endif /* SRC_MODULES_HTTP_OPCUAHANDLER_H_ */
