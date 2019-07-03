/*******************************************************************************
 * Copyright (c) 2019 fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
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

class CUA_ClientInformation {
  public:
    explicit CUA_ClientInformation(CIEC_STRING& paEndpoint);

    ~CUA_ClientInformation();

    bool initialize();

    bool handleClientState();

    bool executeAsyncCalls();

    inline bool isAsyncNeeded() {
      return (0 != mMissingAsyncCalls);
    }

    void resetMissingAsyncCalls() {
      mMissingAsyncCalls = 0;
    }

    CIEC_STRING getEndpoint() {
      return mEndpointUrl;
    }

    inline CSyncObject& getMutex() {
      return mClientMutex;
    }

    bool hasActions() {
      return mActionsReferencingIt.isEmpty();
    }

    bool needsAlreadyAsync() {
      return mNeedsAlreadyAsync;
    }

    bool someActionWasInitialized() {
      return mSomeActionWasInitialized;
    }

    UA_StatusCode executeRead(CActionInfo& paActionInfo);

    UA_StatusCode executeWrite(CActionInfo& paActionInfo);

    UA_StatusCode executeCallMethod(CActionInfo& paActionInfo);

    void addAction(CActionInfo& paActionInfo);

    void removeAction(CActionInfo& paActionInfo);

    bool isActionInitialized(CActionInfo& paActionInfo);

    void resetSubscription();

    class CUA_CallbackFunctions {
      public:
        static void readAsyncCallback(UA_Client *paClient, void *paUserdata, UA_UInt32 paRequestId, UA_ReadResponse *paResponse);

        static void writeAsyncCallback(UA_Client *paClient, void *paUserdata, UA_UInt32 paRequestId, UA_WriteResponse *paResponse);

        static void callMethodAsyncCallback(UA_Client *paClient, void *paUserdata, UA_UInt32 paRequestId,
#ifdef FORTE_COM_OPC_UA_MASTER_BRANCH
            void* paResponse);
#else
            UA_CallResponse *paResponse);
#endif

        static void clientStateChangeCallback(UA_Client *client, UA_ClientState clientState);

        static void
        deleteSubscriptionCallback(UA_Client *client, UA_UInt32 subscriptionId, void *subscriptionContext);

        static void subscriptionValueChangedCallback(UA_Client *client, UA_UInt32 subId, void *subContext, UA_UInt32 monId, void *monContext, UA_DataValue *value);
#ifdef FORTE_COM_OPC_UA_MASTER_BRANCH
        //not used
#else
        static void anyAsyncCallback(UA_Client *paClient, void *paUserdata, UA_UInt32 paRequestId, void *paResponse, const UA_DataType *paResponseType);
#endif
    };

  private:

    //this is the same structure as UA_VariableContext_Handle in localhandler, but couldn't find where to put without copying
    //since there's always some include issue
    struct UA_SubscribeContext_Handle {
        UA_SubscribeContext_Handle(CActionInfo& paActionInfo, const COPC_UA_Helper::UA_TypeConvert *paConvert, size_t paPortIndex) :
            mActionInfo(paActionInfo), mConvert(paConvert), mPortIndex(paPortIndex) {
        }

        //default copy constructor should be enough

        bool operator==(UA_SubscribeContext_Handle const& paRightObject) const {
          return (&mActionInfo == &paRightObject.mActionInfo && mConvert == paRightObject.mConvert && mPortIndex == paRightObject.mPortIndex);
        }

        CActionInfo& mActionInfo;
        const struct COPC_UA_Helper::UA_TypeConvert *mConvert;
        size_t mPortIndex;
    };

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

    struct UA_subscriptionInfo {
        UA_subscriptionInfo() :
            mSubscriptionAlreadyCreated(false), mSubscriptionId(0) {
        }

        bool mSubscriptionAlreadyCreated;
        UA_UInt32 mSubscriptionId;
        CSinglyLinkedList<UA_MonitoringItemInfo> mMonitoredItems;
    };

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


    CUA_ClientInformation(const CUA_ClientInformation &paObj);
    CUA_ClientInformation& operator=(const CUA_ClientInformation& other);

    bool connectClient();

    bool createSubscription();

    bool initializeClient();

    void addAsyncCall();

    void removeAsyncCall();

    bool initializeAction(CActionInfo& paActionInfo);

    bool initializeSubscription(CActionInfo& paActionInfo);

    bool initializeCallMethod(CActionInfo& paActionInfo);

    void uninitializeAction(CActionInfo& paActionInfo);

    void uninitializeSubscription(CActionInfo& paActionInfo);

    bool addMonitoringItem(UA_MonitoringItemInfo& paMonitoringInfo, UA_NodeId& paNodeId);

    UA_Client *mClient;
    CSyncObject mClientMutex;
    UA_subscriptionInfo* mSubscriptionInfo;
    size_t mMissingAsyncCalls;
    CIEC_STRING& mEndpointUrl;
    CSinglyLinkedList<CActionInfo *> mActionsReferencingIt;
    CSinglyLinkedList<CActionInfo *> mActionsToBeInitialized;

    bool mNeedsReconnection;
    uint_fast64_t mLastTry;

    bool mNeedsAlreadyAsync;
    bool mSomeActionWasInitialized;

    static const uint_fast64_t scmConnectionRetryTimeoutMilli = 8000; //8s

    static const UA_UInt32 scmClientTimeoutInMilliseconds = 3000; //3s

};

#endif /* SRC_MODULES_OPC_UA_OPCUA_CLIENT_INFORMATION_H_ */
