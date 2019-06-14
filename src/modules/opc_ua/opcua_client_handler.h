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
#include "opcua_handler_abstract.h"
#include <conn.h>
#include <stdio.h>
#include "comlayer.h"
#include <forte_config.h>
#include "opcua_helper.h"
#include "opcua_layer.h"
#include <forte_string.h>

// cppcheck-suppress noConstructor
class COPC_UA_Client_Handler : public COPC_UA_HandlerAbstract, public CThread {
  DECLARE_HANDLER(COPC_UA_Client_Handler)
  public:

    /* functions needed for the external event handler interface */
    void enableHandler(void);

    void disableHandler(void);

    static UA_StatusCode callMethod(UA_Client *client, const UA_NodeId objectId, const UA_NodeId methodId, size_t inputSize, const UA_Variant *input,
        size_t *outputSize, UA_Variant **output);

    static void stateCallback(UA_Client *client, UA_ClientState clientState);

    static void
    deleteSubscriptionCallback(UA_Client *client, UA_UInt32 subscriptionId, void *subscriptionContext);

    static void onSubscriptionValueChanged(UA_Client *client, UA_UInt32 subId, void *subContext, UA_UInt32 monId, void *monContext, UA_DataValue *value);
#ifdef FORTE_COM_OPC_UA_MASTER_BRANCH
//not used
#else
    static void onAsyncCallReturn(UA_Client *paClient, void *paUserdata, UA_UInt32 paRequestId, void *paResponse, const UA_DataType *paResponseType);
#endif
    /**
     * Get the opc ua client for the given endpoint url. If there exists already an OPC UA client
     * which is connected to the given URL, that one will be returned.
     * If createIfNotFound is false and there is no client yet, NULL will be returned. Otherwise
     * a new client is instantiated and connected.
     *
     * @param endpointUrl The endpoint url for the client
     * @param createIfNotFound if true, a new client will be created if not found
     * @return the (new) client, or NULL of not found and not created.
     */
    UA_Client *
    getClientForEndpoint(const char *endpointUrl, bool subscription, CSyncObject **clientMutex);

  protected:
    virtual UA_StatusCode initializeAction(COPC_UA_HandlerAbstract::CActionInfo& paInfo);

    virtual UA_StatusCode executeAction(COPC_UA_HandlerAbstract::CActionInfo& paInfo);

    virtual UA_StatusCode uninitializeAction(COPC_UA_HandlerAbstract::CActionInfo& paInfo);


  private:

    struct UA_MonitoringItemInfo {
        UA_UInt32 mMonitoringItemId;
        UA_VariableCallback_Handle mVariableInfo;
        COPC_UA_HandlerAbstract::CActionInfo* mAction; //action who created. Used to remove when the action is uninitialized
    };

    class UA_subscriptionInfo {
      public:
        UA_subscriptionInfo() :
            mSubscriptionAlreadyCreated(false), mSubscriptionId(0) {
        }

        bool mSubscriptionAlreadyCreated;
        UA_UInt32 mSubscriptionId;
        CSinglyLinkedList<UA_MonitoringItemInfo*> mMonitoredItems;
    };

    class UA_ClientInformation {
      public:
        UA_ClientInformation() :
            mClient(0), mSubscriptionInfo(0), mMissingAsyncCalls(0), mNeedsReconnection(false), mLastTry(0) {
        }

        UA_Client *mClient;
        CSyncObject mClientMutex;
        UA_subscriptionInfo* mSubscriptionInfo;
        size_t mMissingAsyncCalls;
        CIEC_STRING mEndpointUrl;
        CSinglyLinkedList<COPC_UA_HandlerAbstract::CActionInfo*> mActionsReferencingIt;
        CSinglyLinkedList<COPC_UA_HandlerAbstract::CActionInfo*> mActionsToBeInitialized;

        bool mNeedsReconnection;
        uint_fast64_t mLastTry;

    };

    struct UA_RemoteCallHandle {
        COPC_UA_HandlerAbstract::CActionInfo* mAction;
        UA_ClientInformation* mClient;
    };

    class UA_ConnectionHandler : public CThread {
      public:
        explicit UA_ConnectionHandler(COPC_UA_Client_Handler& paClientHandler);

        ~UA_ConnectionHandler();

        void startConnectionsThread();

        void stopConnectionsThread();

        void addClient(UA_ClientInformation* paClientInformation);

        void removeClient(UA_ClientInformation* paClientInformation);

        void removeAction(UA_ClientInformation* paClientInformation, COPC_UA_HandlerAbstract::CActionInfo* paAction);

        bool connectClient(COPC_UA_Client_Handler::UA_ClientInformation* paClient);

      private:

        void run();

        bool handleClientState(UA_ClientInformation* paClientInformation);

        bool handleClients();

        bool createSubscription(UA_ClientInformation* paClientEndpoint);

        bool initializeClient(COPC_UA_Client_Handler::UA_ClientInformation* paClient);

        bool initializeAction(COPC_UA_HandlerAbstract::CActionInfo* paAction, COPC_UA_Client_Handler::UA_ClientInformation* paClient);

        bool initializeSubscription(COPC_UA_HandlerAbstract::CActionInfo* paAction, COPC_UA_Client_Handler::UA_ClientInformation* paClient);

        bool initializeCallMethod(COPC_UA_HandlerAbstract::CActionInfo* paAction, COPC_UA_Client_Handler::UA_ClientInformation* paClient);

        void uninitializeAction(COPC_UA_HandlerAbstract::CActionInfo* paAction, COPC_UA_Client_Handler::UA_ClientInformation* paClient);

        void uninitializeSubscription(COPC_UA_HandlerAbstract::CActionInfo* paAction, COPC_UA_Client_Handler::UA_ClientInformation* paClient);

        bool addMonitoringItem(COPC_UA_Client_Handler::UA_ClientInformation* paClient, UA_MonitoringItemInfo* paMonitoringInfo, UA_NodeId* paNodeId);

        void updateClientList();

        COPC_UA_Client_Handler& mClientHandler;

        CSyncObject mClientstMutex;
        CSinglyLinkedList<UA_ClientInformation*> mClients;

        CSyncObject mNewClientstMutex;
        CSinglyLinkedList<UA_ClientInformation*> mNewClients;

        bool mNewClientsPresent;

        CSemaphore mClientPresent;

        CSemaphore mThreadStarted;

        static const TForteUInt64 scmNanosecondsToSleep = 1000000000; //1s
        static const uint_fast64_t scmConnectionRetryTimeoutMilli = 8000; //8s

    };

    static void addClientToList(UA_ClientInformation* paClientInformation, CSinglyLinkedList<UA_ClientInformation*>& paList);

    void addClientToInitializedList(UA_ClientInformation* paClientInformation);

    void removeClientFromInitializedList(UA_ClientInformation* paClientInformation);

    bool handleClients();

    void cleanResources();

    UA_ClientInformation* getClient(CIEC_STRING& paEndpoint);
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

    UA_StatusCode initialize(COPC_UA_HandlerAbstract::CActionInfo& paInfo);

    UA_StatusCode executeRead(COPC_UA_HandlerAbstract::CActionInfo& paInfo, COPC_UA_Client_Handler::UA_ClientInformation* paClientInfo);

    UA_StatusCode executeWrite(COPC_UA_HandlerAbstract::CActionInfo& paInfo, COPC_UA_Client_Handler::UA_ClientInformation* paClientInfo);

    UA_StatusCode executeCallMethod(COPC_UA_HandlerAbstract::CActionInfo& paInfo, COPC_UA_Client_Handler::UA_ClientInformation* paClientInfo);

    void cleanClient(UA_ClientInformation * paClientInformation);

    void addClientToConnectionList(UA_ClientInformation* paClientInformation);

    bool isActionInitialized(UA_ClientInformation * paClient, COPC_UA_HandlerAbstract::CActionInfo *paActionInfo);

    void referencedClientsIncrement(COPC_UA_HandlerAbstract::CActionInfo * const paActionInfo);

    void referencedClientsDecrement(COPC_UA_HandlerAbstract::CActionInfo *paActionInfo);

    void addAsyncCall(UA_ClientInformation* paClientInformation);

    static void removeAsyncCall(UA_ClientInformation* paClientInformation);

    static void resetSubscription(UA_ClientInformation* paClientInformation);

    static void handleReadReturn(UA_Client *paClient, void *paUserdata, UA_UInt32 paRequestId, UA_ReadResponse *paResponse);

    static void handleWriteReturn(UA_Client *paClient, void *paUserdata, UA_UInt32 paRequestId, UA_WriteResponse *paResponse);

    static void handleCallReturn(UA_Client *paClient, void *paUserdata, UA_UInt32 paRequestId,
#ifdef FORTE_COM_OPC_UA_MASTER_BRANCH
        void* paResponse);
#else
        UA_CallResponse *paResponse);
#endif

    void updateClientList();

    UA_ConnectionHandler mConnectionHandler;

    CSyncObject mAllClientListMutex;
    CSinglyLinkedList<UA_ClientInformation*> mAllClients;



    CSyncObject mNewClientstMutex;
    CSinglyLinkedList<UA_ClientInformation*> mNewClients;

    CSyncObject mInitializedClientstMutex;
    CSinglyLinkedList<UA_ClientInformation*> mInitializeClients;

    CSemaphore mAsyncIsNeeded;

    bool mNewClientsPresent;

    CSemaphore mClientsThreadStarted;

    static const TForteUInt64 scmNanosecondsToSleep = 100000000; //100ms

    static const UA_UInt32 scmClientTimeoutInMilliseconds = 3000; //3s
};

#endif /* SRC_MODULES_OPC_UA_OPCUACLIENTHANDLER_H_ */
