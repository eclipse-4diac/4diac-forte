/*******************************************************************************
 * Copyright (c) 2019 fortiss GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Jose Cabral - initial implementation
 *******************************************************************************/

#ifndef SRC_MODULES_OPC_UA_OPCUA_CLIENT_INFORMATION_H_
#define SRC_MODULES_OPC_UA_OPCUA_CLIENT_INFORMATION_H_

#include "opcua_action_info.h"
#include <fortelist.h>
#include <forte_sync.h>
#include <criticalregion.h>
#include <string>

/**
 * Contains all the information needed for a client to execute remote calls. It's the only class tha access
 * the OPC UA library for remote calls
 */
class CUA_ClientInformation {
  public:

    /**
     * Constructor of the class
     * @param paEndpoint Endpoint of the remote
     */
    explicit CUA_ClientInformation(const CIEC_STRING &paEndpoint);

    /**
     * Destructor of the class
     */
    ~CUA_ClientInformation();

    /**
     * Initialize the client
     * @return True if no error occurred, false otherwise
     */
    bool configureClient();

    /**
     * Removes all allocated information and sets the client to its default state
     */
    void uninitializeClient();

    /**
     * Try to connect to the remote endpoint and initialized all actions assigned
     * @return True if all actions were initialized, false otherwise
     */
    bool handleClientState();

    /**
     * Call the OPC UA API to execute all the asyncrhonous calls
     * @return True if no error ocurred, false otherwise
     */
    bool executeAsyncCalls();

    /**
     * Check if the client needs another async call. If a subscription action is present, this will stay true
     * @return True if a new async call is needed, false otherwise
     */
    inline bool isAsyncNeeded() const {
      return (0 != mMissingAsyncCalls);
    }

    /**
     * Check if an action is present in the client
     * @return True if an action is present in the client, false otherwise
     */
    bool hasActions() const {
      return !mActionsReferencingIt.isEmpty();
    }

    /**
     * Check if at least one action was initialized in the last iteration of handleClientState()
     * @return True if at least one action was initialized in the last iteration of handleClientState(), false otherwise
     */
    bool someActionWasInitialized() const {
      return mSomeActionWasInitialized;
    }

    /**
     * Getter of the endpoint
     * @return Endpoint of the clinet
     */
    const CIEC_STRING& getEndpoint() const {
      return mEndpointUrl;
    }

    /**
     * Getter of the mutex of the client
     * @return Mutex of the clinet
     */
    inline CSyncObject& getMutex() {
      return mClientMutex;
    }

    void setClientToInvalid() {
      mIsClientValid = false;
    }

    bool isClientValid() const {
      return mIsClientValid;
    }

    /**
     * Place the request to the OPC UA API for an asynchronous read of a remote variable
     * @param paActionInfo Action to be performed
     * @return UA_STATUSCODE_GOOD is no problem occurred, other value otherwise
     */
    UA_StatusCode executeRead(CActionInfo& paActionInfo);

    /**
     * Place the request to the OPC UA API for an asynchronous write of a remote variable
     * @param paActionInfo Action to be performed
     * @return UA_STATUSCODE_GOOD is no problem occurred, other value otherwise
     */
    UA_StatusCode executeWrite(CActionInfo& paActionInfo);

    /**
     * Place the request to the OPC UA API for an asynchronous method call of a remote variable
     * @param paActionInfo Action to be performed
     * @return UA_STATUSCODE_GOOD is no problem occurred, other value otherwise
     */
    UA_StatusCode executeCallMethod(CActionInfo& paActionInfo);

    /**
     * Add an action to the client
     * @param paActionInfo Action to be added
     */
    void addAction(CActionInfo& paActionInfo);

    /**
     * Remove an action from the client
     * @param paActionInfo Action to be removed
     */
    void removeAction(CActionInfo& paActionInfo);

    /**
     * Check if an action was already initialized in the client
     * @param paActionInfo Action to be checked
     * @return True if the action was already initialized, false otherwise
     */
    bool isActionInitialized(const CActionInfo &paActionInfo);

    /**
     * Class containing all remote callback functions that are called by the OPC UA stack
     */
    class CUA_RemoteCallbackFunctions {
      public:

        /**
         * Async callback for read action
         */
        static void readAsyncCallback(UA_Client *paClient, void *paUserdata, UA_UInt32 paRequestId, UA_ReadResponse *paResponse);

        /**
         * Async callback for write action
         */
        static void writeAsyncCallback(UA_Client *paClient, void *paUserdata, UA_UInt32 paRequestId, UA_WriteResponse *paResponse);

        /**
         * Async callback for method call action
         */
        static void callMethodAsyncCallback(UA_Client *paClient, void *paUserdata, UA_UInt32 paRequestId, void *paResponse);

        /**
         * Async callback for subscription action
         */
        static void subscriptionValueChangedCallback(UA_Client *paClient, UA_UInt32 paSubId, void *paSubContext, UA_UInt32 paMonId, void *paMonContext,
            UA_DataValue *paValue);

        /**
         * Callback when the subscription (one for all monitored items) is deleted
         */
        static void deleteSubscriptionCallback(UA_Client *paClient, UA_UInt32 paSubscriptionId, void *paSubscriptionContext);

        /**
         * Function called when the state of the client changed
         */
        static void clientStateChangeCallback(UA_Client *paClient, UA_ClientState paClientState);
    };

  private:

    /**
     *  this is the same structure as UA_VariableContext_Handle in localhandler, but couldn't find where to put without copying
     *  since there's always some include issue
     */
    struct UA_SubscribeContext_Handle {
        UA_SubscribeContext_Handle(CActionInfo& paActionInfo, size_t paPortIndex) :
            mActionInfo(paActionInfo), mPortIndex(paPortIndex) {
        }

        //default copy constructor should be enough

        bool operator==(UA_SubscribeContext_Handle const& paRightObject) const {
          return (&mActionInfo == &paRightObject.mActionInfo && mPortIndex == paRightObject.mPortIndex);
        }

        CActionInfo& mActionInfo;
        size_t mPortIndex;
    };

    /**
     * Encapsulation of the information needed for a monitoring item (a variable subsription)
     */
    struct UA_MonitoringItemInfo {
        UA_MonitoringItemInfo(const UA_SubscribeContext_Handle& paVariableInfo, UA_UInt32 paMonitoringItemId = 0) :
            mVariableInfo(paVariableInfo), mMonitoringItemId(paMonitoringItemId) {
        }

        //default copy constructor should be enough

        bool operator==(UA_MonitoringItemInfo const& paRightObject) const {
          return (mVariableInfo == paRightObject.mVariableInfo && mMonitoringItemId == paRightObject.mMonitoringItemId);
        }

        UA_SubscribeContext_Handle mVariableInfo;
        UA_UInt32 mMonitoringItemId;
    };

    /**
     * Encapsulation of the information needed the subscription. In a subscription you can have many monitored items. We have only one sunscription,
     * and put all monitored items in it
     */
    struct UA_subscriptionInfo {
        UA_subscriptionInfo() :
            mSubscriptionId(0) {
        }

        UA_UInt32 mSubscriptionId;
        CSinglyLinkedList<UA_MonitoringItemInfo> mMonitoredItems;
    };

    /**
     * For read, write and call method, this encpasulation is used as a context to know who executed the action
     */
    class UA_RemoteCallHandle {
      public:
        UA_RemoteCallHandle(CActionInfo& paActionInfo, CUA_ClientInformation& paClientInformation) :
            mActionInfo(paActionInfo), mClientInformation(paClientInformation) {
        }
        ~UA_RemoteCallHandle() {

        }

        CActionInfo& mActionInfo;
        CUA_ClientInformation& mClientInformation;
      private:
        UA_RemoteCallHandle(const UA_RemoteCallHandle &paObj);
        UA_RemoteCallHandle& operator=(const UA_RemoteCallHandle& other);
    };

    /**
     * Look for the configuration file and load the configuration from it, otherwise it loads the default configuration
     * @param paConfigPointer Place to store the configuration
     * @return True if no error ocurred, false otherwise
     */
    bool configureClientFromFile(UA_ClientConfig &paConfigPointer);

    /**
     * Try to connect the client
     * @return True if the connection succeeded, false otherwise
     */
    bool connectClient();

    /**
     * Initialize all actions in the client.
     * @return True if all actions were initialized, false otherwise
     */
    bool initializeAllActions();

    /**
     * Initialize an action. It looks for the remote node and prepares for future calls
     * @param paActionInfo Action to be initialized
     * @return True if no error occurred, false otherwise
     */
    bool initializeAction(CActionInfo& paActionInfo);

    /**
     * A method call method needs special care. This function takes care of it
     * @param paActionInfo Method call action to be initialized
     * @return True if no error occurred, false otherwise
     */
    bool initializeCallMethod(CActionInfo& paActionInfo);

    /**
     * A subscription needs special care. This function takes care of it
     * @param paActionInfo Subscribe action to be initialized
     * @return True if no error occurred, false otherwise
     */
    bool initializeSubscription(CActionInfo& paActionInfo);

    /**
     * Allocate the memory to store the subscription information and create the subscription in the OPC UA stack
     * @return True if no error occurred, false otherwise
     */
    bool allocAndCreateSubscription();

    /**
     * Create the subscription in the OPC UA stack
     * @return True if no error occurred, false otherwise
     */
    bool createSubscription();

    /**
     * Add a monitoring item to the OPC UA stack
     * @param paMonitoringInfo Contains the needed information to create the monitoring item in the OPC UA Stack
     * @param paNodeId NodeId of the variable to monitor
     * @return True if no error occurred, false otherwise
     */
    bool addMonitoringItem(UA_MonitoringItemInfo &paMonitoringInfo, const UA_NodeId &paNodeId);

    /**
     * Increments the amount of missing async calls to be performed. A subscription keeps always the missing calls at least at
     */
    void addAsyncCall();

    /**
     * Removes the amount of missing async calls to be performed
     */
    void removeAsyncCall();

    /**
     * Uninitialze an action
     * @param paActionInfo Action to be uninitialized
     */
    void uninitializeAction(CActionInfo& paActionInfo);

    /**
     * Uninitialze a subscription action
     * @param paActionInfo Action to be uninitialized
     */
    void uninitializeSubscribeAction(const CActionInfo &paActionInfo);

    /**
     * Reset the subscription information.
     * @param paDeleteSubscription The subscription must be deleted from the OPC UA stack
     */
    void resetSubscription(bool paDeleteSubscription);

    /**
     * Copy constructor is private and not defined to avoid its usage
     */
    CUA_ClientInformation(const CUA_ClientInformation &paObj);

    /**
     * Assignment operator is private and not defined to avoid its usage
     */
    CUA_ClientInformation& operator=(const CUA_ClientInformation& other);

    /**
     * Endpoint of the remote
     */
    CIEC_STRING mEndpointUrl;

    /**
     * Username to be used to connect to the server
     */
    std::string mUsername;

    /**
     * * Password to be used to connect to the server
     */
    std::string mPassword;

    /**
     * Handler of the OPC UA stack client
     */
    UA_Client *mClient;

    /**
     * Mutex of the client
     */
    CSyncObject mClientMutex;

    /**
     * Information needed for subscription
     */
    UA_subscriptionInfo *mSubscriptionInfo;

    /**
     * Amount of missing async calls to be performed. Read, write and method call add one when they are called, and reduced in the callbacks
     * Subscription increases one when created, and only reduced when no more subscription are present
     */
    size_t mMissingAsyncCalls;

    /**
     * List of actions that use this client
     */
    CSinglyLinkedList<CActionInfo *> mActionsReferencingIt;

    /**
     * List of actions that need to be initialized
     */
    CSinglyLinkedList<CActionInfo *> mActionsToBeInitialized;

    /**
     * Indicates if the client should wait scmConnectionRetryTimeoutNano before trying to reconnect. This is true when an action fails to connect once
     */
    bool mNeedsReconnection;

    /**
     * Indicates if the client should wait scmInitializeActionRetryNano before trying to initialize the actions. This is true when an action fails, so
     * it doesn't fail too often. If an action is added after another fail, this becomes false, so the new action can be initialized and doesn't have to wait
     */
    bool mWaitToInitializeActions;

    /**
     * Indicate the client is about to be deleted, so it's not added to new lists.
     * The reason behind this variable, is because the following race condition happened in COPC_UA_Remote_Handler::removeClientFromAllLists
     *  - The client is removed from the connection list
     *  - Before removing from the normal handler list, the async call fails and the client is reconfigured and re-added to the connection handler
     *  - The Client is deleted from allClients lists
     *  - The Client is deleted (C++ wise)
     *  - Since the connectionHandler has still the client, it tries to use it and crash
     *
     *  So this variable prevents the re-adding to any iteration list if it was set already to invalid, when is about to be deleted
     */
    bool mIsClientValid;

    /**
     * Store the time when the connection last failed
     */
    uint_fast64_t mLastReconnectionTry;

    /**
     * Store the time when an action last failed to initialized
     */
    uint_fast64_t mLastActionInitializationTry;

    /**
     * True if an action was initialized in the last iteration of handleClientState()
     */
    bool mSomeActionWasInitialized;

    /**
     * Time in nanoseconds that the client should wait before another reconnection try
     */
    static const uint_fast64_t scmConnectionRetryTimeoutNano = 8E9; //8s

    /**
     * Time in nanoseconds that the client should wait before try to initialize the actions
     */
    static const uint_fast64_t scmInitializeActionRetryNano = 3E9; //3s

    /**
     * Time in milliseconds for connection timeout configured in the OPC UA stack client
     */
    static const UA_UInt32 scmClientTimeoutInMilli = 5E3; //5s

};

#endif /* SRC_MODULES_OPC_UA_OPCUA_CLIENT_INFORMATION_H_ */
