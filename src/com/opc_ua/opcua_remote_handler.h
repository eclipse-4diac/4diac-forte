/*******************************************************************************
 * Copyright (c) 2018 - 2019 fortiss GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
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

/**
 * Parent class used by the remote handler and the connection thread. They both iterate a list of clients and executed periodically action on it
 * This class contains the common code of them and uses a strategy pattern in run(), where the action ot be executed periodically on the clients
 * is done in the pure virtual handleClients().
 * The iteration can sleep forever or sleep for some time and then iterate again. In both cases, the thread can be awaken.
 */
class COPC_UA_Client_IterationList : public CThread {
  public:

    /**
     * Constructor of the class
     */
    COPC_UA_Client_IterationList();

    /**
     * Destructor of the class
     */
    virtual ~COPC_UA_Client_IterationList();

    /**
     * Start the iteration thread that will execute periodically handleClients()
     */
    void startIterationThread();

    /**
     * Stops the iteration thread
     */
    void stopIterationThread();

    /**
     * Adds a new client to the list of clients where handleClients() is executed. The new clients are added asyncrhonously so the iteration doesn't block the thread calling this. The client is usually no longer used by the caller
     * @param paClientInformation The client to be added to the iteration
     */
    void addClient(CUA_ClientInformation &paClientInformation);

    /**
     * Removes a client from the iteration list syncrhonously. Usually, the client is then accessed after removed is then deleted, so the caller has to wait that the client is free before accessing it
     * @param paClientInformation The client to be removed from the list
     */
    void removeClient(CUA_ClientInformation &paClientInformation);

    /**
     * Get the added clients to the iteration list, and execute handleClients(). Depending on the result, it will wait forever or just for some time (the cyclic period)
     */
    void run();

  protected:

    /**
     * Function to be executed by iteration and that implements the action to be performed cyclic in the clients
     * @return True if a new iteration is needed, false otherwise
     */
    virtual bool handleClients() = 0;

    /**
     * If the iteration loop is waiting forever, this function will awake it and iterate again
     */
    void resumeIterationLoop();

    /**
     * Access to private member mIterationClients
     * @return mIterationClients
     */

    CSinglyLinkedList<CUA_ClientInformation*>& getIterationClients() {
      return mIterationClients;
    }

    /**
     * Access to private member mIterationClientsMutex
     * @return mIterationClientsMutex
     */
    CSyncObject& getIterationClientsMutex() {
      return mIterationClientsMutex;
    }

    /**
     * Access to private member mNewClients
     * @return mNewClients
     */
    CSinglyLinkedList<CUA_ClientInformation*>& getNewClients() {
      return mNewClients;
    }

    /**
     * Access to private member mIterationClients
     * @return mNewClientsMutex
     */
    CSyncObject& getNewClientsMutex() {
      return mNewClientsMutex;
    }

  private:

    /**
     * List of clients on which the iteration is done
     */
    CSinglyLinkedList<CUA_ClientInformation*> mIterationClients;

    /**
     * Mutex to access the list of clients which is iterated
     */
    CSyncObject mIterationClientsMutex;

    /**
     * List of new clients. It serves as a buffer that is latter added to the main iteration list
     */
    CSinglyLinkedList<CUA_ClientInformation*> mNewClients;


        /**
     * Mutex to access the list of new clients
     */
    CSyncObject mNewClientsMutex;

    /**
     * Adds a client to a list. If the client is already present, no client is added
     * @param paClientInformation Client to be added
     * @param paList List where the client is added
     */
    void addClientToList(CUA_ClientInformation &paClientInformation, CSinglyLinkedList<CUA_ClientInformation*> &paList) const;

    /**
     * Add the new clients to the main iteration list and clears the new clients list
     */
    void updateClientList();

    /**
     * Indicates that a new client is present and needs to be added to the main iteration list
     */
    bool mNewClientsPresent;

    /**
     * Indicates if the iteration should continue
     */
    CSemaphore mNeedsIteration;

    /**
     * Cyclic time to sleep if another iteration is needed. This is the period at which the iteration occurs normally
     */
    static const TForteUInt64 scmNanosecondsToSleep = static_cast<TForteUInt64>(FORTE_COM_OPC_UA_CLIENT_PUB_INTERVAL * 1000000); //ms to nanoseconds
};

/**
 * Class to handle all action that are executed remotely. It calls the clients to execute their asynchronous calls
 */
// cppcheck-suppress noConstructor
class COPC_UA_Remote_Handler : public COPC_UA_HandlerAbstract, public COPC_UA_Client_IterationList {
  DECLARE_HANDLER(COPC_UA_Remote_Handler)
  public:

    /**
     * Starts the threads
     */
    void enableHandler(void);

    /**
     * Stops the threads
     */
    void disableHandler(void);

  protected:

    /**
     * Perform all the needed initialization of the action
     * @param paActionInfo Action to be initialized
     * @return UA_STATUSCODE_GOOD is no problem occurred, other value otherwise
     */
    virtual UA_StatusCode initializeAction(CActionInfo &paActionInfo);

    /**
     * Execute the action
     * @param paActionInfo Action to be executed
     * @return UA_STATUSCODE_GOOD is no problem occurred, other value otherwise
     */
    virtual UA_StatusCode executeAction(CActionInfo &paActionInfo);

    /**
     * Uninitialize the action
     * @param paActionInfo Action to be Uninitialized
     * @return UA_STATUSCODE_GOOD is no problem occurred, other value otherwise
     */
    virtual UA_StatusCode uninitializeAction(CActionInfo &paActionInfo);

  private:

    /**
     * Class that connects the clients and initializes the actions stored in the clients.
     * It will iterate the clients trying to connect and initialize the actions until they are all initialized
     * If one action on a client is initialized, it will pass the client to the remote handler, so it can execute
     * calls on it, but keep it also on its own list to try to initialize the other actions
     */
    class UA_ConnectionHandler : public COPC_UA_Client_IterationList {
      public:

        /**
         * Constructor of the class
         * @param paClientHandler The remote handler. It uses later to pass the clients who have initialized actions
         */
        explicit UA_ConnectionHandler(COPC_UA_Remote_Handler &paClientHandler);

        /**
         * Destructor of the class
         */
        ~UA_ConnectionHandler();

      private:

        /**
         * Calls the handleClientState() function in the clients.
         * @return True if a new iteration is needed, false otherwise
         */
        bool handleClients();

        /**
         * Remote handler to pass the clients who have at least one action initialized
         */
        COPC_UA_Remote_Handler &mClientHandler;

    };

    /**
     * Clean all resources from the remote handler
     */
    void cleanResources();

    /**
     * Looks for a client with the endpoint of the action and adds the action to it
     * @param paActionInfo Action to be added to the client
     * @return UA_STATUSCODE_GOOD is no problem occurred, other value otherwise
     */
    UA_StatusCode getClientAndAddAction(CActionInfo &paActionInfo);

    /**
     * Looks for a client using an endpoint. If a client is not found, a new one is created
     * @param paEndpoint Endpoint of the client
     * @return 0 if an error occurred, a pointer to the client otherwise
     */
    CUA_ClientInformation* getClient(const CIEC_STRING &paEndpoint);

    /**
     * Adds an action to a client. This is performed when an action is initialized
     * @param paActionInfo Action to be added
     */
    void addActionToClient(CActionInfo &paActionInfo);

    /**
     * Removes an action from the client. If the client has no more actions, it's cleaned and deleted
     * @param paActionInfo
     */
    void removeActionFromClient(CActionInfo &paActionInfo);

    /**
     * Remove a client from all list, and delete it
     */
    void removeClientFromAllLists(CUA_ClientInformation &paClientInformation);

    /**
     * Adds a client to the connection handler
     * @param paClientInformation Client to be added
     */
    void addClientToConnectionHandler(CUA_ClientInformation &paClientInformation);

    /**
     * Trigger the clients to execute their asyncronous calls. If a client fails, it's uninitialized and given back
     * to the connection handler
     * @return True if a new iteration is needed, false otherwise
     */
    bool handleClients();

    /**
     * Connection handler to pass the clients that aren't fully initialized yet
     */
    UA_ConnectionHandler mConnectionHandler;

    /**
     * A list with all clients
     */
    CSinglyLinkedList<CUA_ClientInformation*> mAllClients;

    /**
     *  Mutex to access the list with all clients
     */
    CSyncObject mAllClientListMutex;
};

#endif /* SRC_MODULES_OPC_UA_OPCUACLIENTHANDLER_H_ */
