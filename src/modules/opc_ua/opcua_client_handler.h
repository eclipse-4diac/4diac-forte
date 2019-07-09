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

class COPC_UA_Client_IterationList : public CThread {
  public:
    COPC_UA_Client_IterationList();

    virtual ~COPC_UA_Client_IterationList();

    void startIterationThread();

    void stopIterationThread();

    void addClient(CUA_ClientInformation& paClientInformation);

    void removeClient(CUA_ClientInformation& paClientInformation);

    void run();

  protected:
    static void addClientToList(CUA_ClientInformation& paClientInformation, CSinglyLinkedList<CUA_ClientInformation *>& paList);

    virtual bool handleClients() = 0;

    void resumeIterationLoop();

    CSyncObject mNewClientsMutex;
    CSinglyLinkedList<CUA_ClientInformation *> mNewClients;

    CSyncObject mIterationClientsMutex;
    CSinglyLinkedList<CUA_ClientInformation *> mIterationClients;

  private:

    void updateClientList();



    bool mNewClientsPresent;

    CSemaphore mNeedsIteration;

    CSemaphore mClientsThreadStarted;

    static const TForteUInt64 scmNanosecondsToSleep = 100000000; //100ms
};

// cppcheck-suppress noConstructor
class COPC_UA_Client_Handler : public COPC_UA_HandlerAbstract, public COPC_UA_Client_IterationList {
  DECLARE_HANDLER(COPC_UA_Client_Handler)
  public:

    /* functions needed for the external event handler interface */
    void enableHandler(void);

    void disableHandler(void);

  protected:

    virtual UA_StatusCode initializeAction(CActionInfo& paActionInfo);

    virtual UA_StatusCode executeAction(CActionInfo& paActionInfo);

    virtual UA_StatusCode uninitializeAction(CActionInfo& paActionInfo);

  private:

    class UA_ConnectionHandler : public COPC_UA_Client_IterationList {
      public:
        explicit UA_ConnectionHandler(COPC_UA_Client_Handler& paClientHandler);

        ~UA_ConnectionHandler();

      private:

        bool handleClients();

        COPC_UA_Client_Handler& mClientHandler;

    };

    bool handleClients();

    void cleanResources();

    CUA_ClientInformation* getClient(CIEC_STRING& paEndpoint);

    void addClientToConnectionList(CUA_ClientInformation& paClientInformation);

    UA_StatusCode initializeActionInClient(CActionInfo& paActionInfo);

    void referencedClientsIncrement(CActionInfo& paActionInfo);

    void referencedClientsDecrement(CActionInfo& paActionInfo);

    void removeClientFromAllLists(CUA_ClientInformation& paClientInformation);

    UA_ConnectionHandler mConnectionHandler;

    CSyncObject mAllClientListMutex;
    CSinglyLinkedList<CUA_ClientInformation*> mAllClients;

    CSemaphore mAsyncIsNeeded;
};

#endif /* SRC_MODULES_OPC_UA_OPCUACLIENTHANDLER_H_ */
