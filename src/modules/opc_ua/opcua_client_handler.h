/*******************************************************************************
 * Copyright (c) 2018 fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Jose Cabral, Kirill Dorofeev - initial implementation
 *******************************************************************************/

#ifndef SRC_MODULES_OPC_UA_OPCUACLIENTHANDLER_H_
#define SRC_MODULES_OPC_UA_OPCUACLIENTHANDLER_H_

#include <forte_thread.h>
#include <forte_config.h>
#include "opcua_handler_abstract.h"
#include "opcua_client_information.h"

// cppcheck-suppress noConstructor
class COPC_UA_Client_Handler : public COPC_UA_HandlerAbstract, public CThread {
  DECLARE_HANDLER(COPC_UA_Client_Handler)
  public:

    /* functions needed for the external event handler interface */
    void enableHandler(void);

    void disableHandler(void);

    void resumeAsyncLoop();

  protected:

    virtual UA_StatusCode initializeAction(CActionInfo& paActionInfo);

    virtual UA_StatusCode executeAction(CActionInfo& paActionInfo);

    virtual UA_StatusCode uninitializeAction(CActionInfo& paActionInfo);


  private:

    class UA_ConnectionHandler : public CThread {
      public:
        explicit UA_ConnectionHandler(COPC_UA_Client_Handler& paClientHandler);

        ~UA_ConnectionHandler();

        void startConnectionsThread();

        void stopConnectionsThread();

        void addClient(CUA_ClientInformation& paClientInformation);

        void removeClient(CUA_ClientInformation& paClientInformation);

      private:

        void run();

        bool handleClients();

        void updateClientList();

        COPC_UA_Client_Handler& mClientHandler;

        CSyncObject mClientstMutex;
        CSinglyLinkedList<CUA_ClientInformation*> mClients;

        CSyncObject mNewClientstMutex;
        CSinglyLinkedList<CUA_ClientInformation *> mNewClients;

        bool mNewClientsPresent;

        CSemaphore mClientPresent;

        CSemaphore mThreadStarted;

        static const TForteUInt64 scmNanosecondsToSleep = 1000000000; //1s
    };

    static void addClientToList(CUA_ClientInformation& paClientInformation, CSinglyLinkedList<CUA_ClientInformation *>& paList);

    void addClientToInitializedList(CUA_ClientInformation& paClientInformation);

    void removeClientFromInitializedList(CUA_ClientInformation& paClientInformation);

    bool handleClients();

    void cleanResources();

    CUA_ClientInformation* getClient(CIEC_STRING& paEndpoint);
    /**
     * Starts the OPC UA server, if it is not already running
     */
    void startLoops();

    /**
     * Sets the uaServerRunningFlag to false which causes the UA Server to stop.
     */
    void stopLoops();

    /**
     * Overridden run() from CThread which loops the UA Server.
     */
    virtual void run();

    UA_StatusCode initializeActionInClient(CActionInfo& paActionInfo);

    
    void removeClientFromAllLists(CUA_ClientInformation& paClientInformation);

    void addClientToConnectionList(CUA_ClientInformation& paClientInformation);

    void referencedClientsIncrement(CActionInfo& paActionInfo);

    void referencedClientsDecrement(CActionInfo& paActionInfo);

    void updateClientList();

    UA_ConnectionHandler mConnectionHandler;

    CSyncObject mAllClientListMutex;
    CSinglyLinkedList<CUA_ClientInformation*> mAllClients;

    CSyncObject mNewClientstMutex;
    CSinglyLinkedList<CUA_ClientInformation *> mNewClients;

    CSyncObject mInitializedClientstMutex;
    CSinglyLinkedList<CUA_ClientInformation *> mInitializedClients;

    CSemaphore mAsyncIsNeeded;

    bool mNewClientsPresent;

    CSemaphore mClientsThreadStarted;

    static const TForteUInt64 scmNanosecondsToSleep = 100000000; //100ms
};

#endif /* SRC_MODULES_OPC_UA_OPCUACLIENTHANDLER_H_ */
