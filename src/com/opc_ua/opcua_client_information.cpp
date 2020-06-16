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

#include <forte_architecture_time.h>
#include "opcua_client_information.h"
#include <basecommfb.h>
#include "opcua_handler_abstract.h" //for logger
#include "opcua_client_config_parser.h"
#include <stdio.h>

std::string gOpcuaClientConfigFile;

CUA_ClientInformation::CUA_ClientInformation(const CIEC_STRING &paEndpoint) :
    mEndpointUrl(paEndpoint), mClient(0), mSubscriptionInfo(0), mMissingAsyncCalls(0), mNeedsReconnection(false), mWaitToInitializeActions(false),
        mIsClientValid(true),
        mLastReconnectionTry(0), mLastActionInitializationTry(0), mSomeActionWasInitialized(false) {
}

CUA_ClientInformation::~CUA_ClientInformation() {
  CCriticalRegion clientRegion(mClientMutex);
  uninitializeClient();
}

bool CUA_ClientInformation::configureClient() {
  bool retVal = true;
  mClient = UA_Client_new();
  UA_ClientConfig *configPointer = UA_Client_getConfig(mClient);

  if(configureClientFromFile(*configPointer)) {
    configPointer->stateCallback = CUA_RemoteCallbackFunctions::clientStateChangeCallback;
    configPointer->logger = COPC_UA_HandlerAbstract::getLogger();
    configPointer->timeout = scmClientTimeoutInMilli;
  } else {
    UA_Client_delete(mClient);
    mClient = 0;
    retVal = false;
  }
  return retVal;
}

bool CUA_ClientInformation::configureClientFromFile(UA_ClientConfig &paConfig) {
  bool retVal = true;

  if("" != gOpcuaClientConfigFile){ //file was provided

    std::string endpoint = mEndpointUrl.getValue();
    CUA_ClientConfigFileParser::UA_ConfigFromFile result = CUA_ClientConfigFileParser::UA_ConfigFromFile(paConfig, mUsername, mPassword);

    retVal = CUA_ClientConfigFileParser::loadConfig(gOpcuaClientConfigFile, endpoint, result);
  } else {
    UA_StatusCode retValOpcUa = UA_ClientConfig_setDefault(&paConfig);
    if(UA_STATUSCODE_GOOD != retValOpcUa) {
      DEVLOG_ERROR("[OPC UA CLIENT]: Error setting client configuration. Error: %s\n", UA_StatusCode_name(retValOpcUa));
      retVal = false;
    }
  }

  return retVal;
}

void CUA_ClientInformation::uninitializeClient() {
  DEVLOG_INFO("[OPC UA CLIENT]: Uninitializing client %s\n", mEndpointUrl.getValue());
  mActionsToBeInitialized.clearAll();
  for(CSinglyLinkedList<CActionInfo *>::Iterator itReferencingActions = mActionsReferencingIt.begin(); itReferencingActions != mActionsReferencingIt.end();
      ++itReferencingActions) {
    uninitializeAction(**itReferencingActions);
    mActionsToBeInitialized.pushBack(*itReferencingActions);
  }
  if(mClient) {
    UA_Client_disconnect(mClient);
    UA_Client_delete(mClient);
    mClient = 0;
  }
  mWaitToInitializeActions = false;
  mNeedsReconnection = false;
  mSomeActionWasInitialized = false;
  mLastReconnectionTry = 0;
  mLastActionInitializationTry = 0;
  mMissingAsyncCalls = 0;
}

bool CUA_ClientInformation::handleClientState() {

  mSomeActionWasInitialized = false;
  bool noMoreChangesNeeded = false;
  bool tryAnotherChangeImmediately = true;

  if(mNeedsReconnection) {
    uint_fast64_t now = getNanoSecondsMonotonic();
    if((now - mLastReconnectionTry) < scmConnectionRetryTimeoutNano) { //if connection timeout didn't happen, return that more changes are still needed
      tryAnotherChangeImmediately = false;
    }
  } else if(mWaitToInitializeActions) {
    uint_fast64_t now = getNanoSecondsMonotonic();
    if((now - mLastActionInitializationTry) < scmInitializeActionRetryNano) { //if an action failed, wait scmInitializeActionRetryNano until next retry to initialize them
      tryAnotherChangeImmediately = false;
    }
  }

  while(tryAnotherChangeImmediately) {
    UA_ClientState currentState = UA_Client_getState(mClient);
    if(UA_CLIENTSTATE_SESSION == currentState) {
      if(initializeAllActions()) {
        noMoreChangesNeeded = true;
      } else {
        mWaitToInitializeActions = true;
        mLastActionInitializationTry = getNanoSecondsMonotonic();
      }
      tryAnotherChangeImmediately = false;
    } else if(UA_CLIENTSTATE_SESSION_RENEWED == currentState) {
      DEVLOG_ERROR("[OPC UA CLIENT]: Client state is session renewed. Check what happens with the subscription here\n");
    } else {
      if(!connectClient()) {
        tryAnotherChangeImmediately = false;
        DEVLOG_ERROR(("[OPC UA CLIENT]: Couldn't connect to endpoint %s. Forte will try to reconnect in %u milliseconds\n"),
          mEndpointUrl.getValue(),
          static_cast<unsigned int>(scmConnectionRetryTimeoutNano / 1E6));
        mNeedsReconnection = true;
        mLastReconnectionTry = getNanoSecondsMonotonic();
      } else { //if connection succeeded, don't break the while and try to handle subscriptions immediately
        mNeedsReconnection = false;
        DEVLOG_INFO("[OPC UA CLIENT]: Client connected to endpoint %s\n", mEndpointUrl.getValue());
      }
    }
  }

  return noMoreChangesNeeded;
}

bool CUA_ClientInformation::executeAsyncCalls() {
  return (UA_STATUSCODE_GOOD ==
    UA_Client_run_iterate(mClient, 10));
}

UA_StatusCode CUA_ClientInformation::executeRead(CActionInfo& paActionInfo) {
  CCriticalRegion clientRegion(mClientMutex);

  UA_ReadRequest request;
  UA_ReadRequest_init(&request);
  request.nodesToReadSize = paActionInfo.getNoOfNodePairs();

  UA_ReadValueId *ids = static_cast<UA_ReadValueId *>(UA_Array_new(request.nodesToReadSize, &UA_TYPES[UA_TYPES_READVALUEID]));
  request.nodesToRead = ids;

  size_t indexOfNodePair = 0;
  for(CSinglyLinkedList<CActionInfo::CNodePairInfo*>::Iterator itNodePair = paActionInfo.getNodePairInfo().begin();
      itNodePair != paActionInfo.getNodePairInfo().end(); ++itNodePair, indexOfNodePair++) {
    UA_ReadValueId_init(&ids[indexOfNodePair]);
    ids[indexOfNodePair].attributeId = UA_ATTRIBUTEID_VALUE;
    UA_NodeId_copy((*itNodePair)->mNodeId, &ids[indexOfNodePair].nodeId);
  }

  UA_RemoteCallHandle *remoteCallHandle = new UA_RemoteCallHandle(paActionInfo, *this);

  UA_StatusCode retVal = UA_Client_sendAsyncReadRequest(mClient, &request, CUA_RemoteCallbackFunctions::readAsyncCallback, remoteCallHandle, 0);

  if(UA_STATUSCODE_GOOD != retVal) {
    DEVLOG_ERROR("[OPC UA CLIENT]: Couldn't dispatch read action for FB %s. Error: %s\n", paActionInfo.getLayer().getCommFB()->getInstanceName(), UA_StatusCode_name(retVal));
    delete remoteCallHandle;
  } else {
    addAsyncCall();
  }

  UA_ReadRequest_deleteMembers(&request);

  return retVal;
}

UA_StatusCode CUA_ClientInformation::executeWrite(CActionInfo& paActionInfo) {
  CCriticalRegion clientRegion(mClientMutex);

  UA_StatusCode retVal = UA_STATUSCODE_BADINTERNALERROR;
  UA_WriteRequest request;
  UA_WriteRequest_init(&request);
  request.nodesToWriteSize = paActionInfo.getNoOfNodePairs();

  UA_WriteValue *ids = static_cast<UA_WriteValue *>(UA_Array_new(request.nodesToWriteSize, &UA_TYPES[UA_TYPES_WRITEVALUE]));
  request.nodesToWrite = ids;

  size_t indexOfNodePair = 0;
  const CIEC_ANY *dataToSend = paActionInfo.getDataToSend();
  for(CSinglyLinkedList<CActionInfo::CNodePairInfo*>::Iterator itNodePair = paActionInfo.getNodePairInfo().begin();
      itNodePair != paActionInfo.getNodePairInfo().end(); ++itNodePair, indexOfNodePair++) {

    UA_WriteValue_init(&ids[indexOfNodePair]);
    ids[indexOfNodePair].attributeId = UA_ATTRIBUTEID_VALUE;
    UA_NodeId_copy((*itNodePair)->mNodeId, &ids[indexOfNodePair].nodeId);
    ids[indexOfNodePair].value.hasValue = true;

    COPC_UA_Helper::fillVariant(ids[indexOfNodePair].value.value, dataToSend[indexOfNodePair]);
  }

  UA_RemoteCallHandle *remoteCallHandle = new UA_RemoteCallHandle(paActionInfo, *this);
  retVal = UA_Client_sendAsyncWriteRequest(mClient, &request, CUA_RemoteCallbackFunctions::writeAsyncCallback, remoteCallHandle, 0);

  if(UA_STATUSCODE_GOOD != retVal) {
    DEVLOG_ERROR("[OPC UA CLIENT]: Couldn't dispatch write action for FB %s. Error: %s\n", paActionInfo.getLayer().getCommFB()->getInstanceName(), UA_StatusCode_name(retVal));
    delete remoteCallHandle;
  } else {
    addAsyncCall();
  }

  UA_WriteRequest_deleteMembers(&request);

  return retVal;
}

UA_StatusCode CUA_ClientInformation::executeCallMethod(CActionInfo& paActionInfo) {
  CCriticalRegion clientRegion(mClientMutex);

  UA_StatusCode retVal = UA_STATUSCODE_BADINTERNALERROR;
  UA_CallRequest request;
  UA_CallRequest_init(&request);
  request.methodsToCallSize = 1;
  request.methodsToCall = static_cast<UA_CallMethodRequest *>(UA_Array_new(request.methodsToCallSize, &UA_TYPES[UA_TYPES_CALLMETHODREQUEST]));

  UA_CallMethodRequest *methodRequest = &request.methodsToCall[0];

  methodRequest->inputArgumentsSize = paActionInfo.getSendSize();
  methodRequest->inputArguments = static_cast<UA_Variant *>(UA_Array_new(methodRequest->inputArgumentsSize, &UA_TYPES[UA_TYPES_VARIANT]));

  const CIEC_ANY *dataToSend = paActionInfo.getDataToSend();

  CSinglyLinkedList<CActionInfo::CNodePairInfo*>::Iterator itNodePair = paActionInfo.getNodePairInfo().begin();
  UA_NodeId_copy((*itNodePair)->mNodeId, &methodRequest->methodId);
  ++itNodePair;
  UA_NodeId_copy((*itNodePair)->mNodeId, &methodRequest->objectId);

  for(size_t i = 0; i < methodRequest->inputArgumentsSize; i++) {
    COPC_UA_Helper::fillVariant(methodRequest->inputArguments[i], dataToSend[i]);
  }

  UA_RemoteCallHandle *remoteCallHandle = new UA_RemoteCallHandle(paActionInfo, *this);
  retVal = UA_Client_sendAsyncRequest(mClient, &request, &UA_TYPES[UA_TYPES_CALLREQUEST], CUA_RemoteCallbackFunctions::callMethodAsyncCallback,
        &UA_TYPES[UA_TYPES_CALLRESPONSE], remoteCallHandle, 0);

  if(UA_STATUSCODE_GOOD != retVal) {
    DEVLOG_ERROR("[OPC UA CLIENT]: Couldn't dispatch call action for FB %s. Error %s\n", paActionInfo.getLayer().getCommFB()->getInstanceName(), UA_StatusCode_name(retVal));
    delete remoteCallHandle;
  } else {
    addAsyncCall();
  }

  UA_CallRequest_deleteMembers(&request);

  return retVal;
}

void CUA_ClientInformation::addAction(CActionInfo& paActionInfo) {
  mActionsReferencingIt.pushBack(&paActionInfo);
  mActionsToBeInitialized.pushBack(&paActionInfo);
  mWaitToInitializeActions = false;
}

void CUA_ClientInformation::removeAction(CActionInfo& paActionInfo) {
  uninitializeAction(paActionInfo);
  mActionsReferencingIt.erase(&paActionInfo);
}

bool CUA_ClientInformation::isActionInitialized(const CActionInfo &paActionInfo) {
  CCriticalRegion clientRegion(mClientMutex);
  bool retVal = true;
  for(CSinglyLinkedList<CActionInfo *>::Iterator itClientInformation = mActionsToBeInitialized.begin(); itClientInformation != mActionsToBeInitialized.end();
      ++itClientInformation) {
    if((*itClientInformation) == &paActionInfo) {
      retVal = false;
      break;
    }
  }
  return retVal;
}

bool CUA_ClientInformation::connectClient() {
  if(0 == mUsername.compare("")) {
    return (UA_STATUSCODE_GOOD == UA_Client_connect(mClient, mEndpointUrl.getValue()));
  } else {
    return (UA_STATUSCODE_GOOD == UA_Client_connect_username(mClient, mEndpointUrl.getValue(), mUsername.c_str(), mPassword.c_str()));
  }
}

bool CUA_ClientInformation::initializeAllActions() {
  bool somethingFailed = false;

  CSinglyLinkedList<CActionInfo *> initializedActions;
  for(CSinglyLinkedList<CActionInfo *>::Iterator itActionInfo = mActionsToBeInitialized.begin(); itActionInfo != mActionsToBeInitialized.end();
      ++itActionInfo) {

    if(!initializeAction(**itActionInfo)) {
      initializedActions.pushBack(*itActionInfo);
    } else {
      somethingFailed = true;
    }
  }

  if(!initializedActions.isEmpty()) { //if one action (FB) related to the client was initialized, copy it to the main thread
    mSomeActionWasInitialized = true;
    for(CSinglyLinkedList<CActionInfo *>::Iterator itActionInfo = initializedActions.begin(); itActionInfo != initializedActions.end();
        ++itActionInfo) {
      mActionsToBeInitialized.erase(*itActionInfo);
    }
  }

  return !somethingFailed;
}

bool CUA_ClientInformation::initializeAction(CActionInfo& paActionInfo) {
  bool somethingFailed = false;
  if(CActionInfo::eCallMethod == paActionInfo.getAction()) {
    if(!initializeCallMethod(paActionInfo)) {
      somethingFailed = true;
    }
  } else {
    size_t runnerHelper = 0;
    for(CSinglyLinkedList<CActionInfo::CNodePairInfo*>::Iterator itNodePair = paActionInfo.getNodePairInfo().begin();
        itNodePair != paActionInfo.getNodePairInfo().end();
        ++itNodePair, runnerHelper++) {

      if(!somethingFailed && "" != (*itNodePair)->mBrowsePath) { //if browsepath was given, look for NodeId, even if NodeID was also provided
        UA_NodeId *nodeId;
        UA_StatusCode retVal = COPC_UA_Helper::getRemoteNodeForPath(*mClient, (*itNodePair)->mBrowsePath.getValue(), 0, &nodeId); //we don't care about the parent

        if(UA_STATUSCODE_GOOD != retVal) {
          DEVLOG_ERROR("[OPC UA CLIENT]: The index %u of the FB %s could not be initialized because the requested nodeId was not found. Error: %s\n",
            runnerHelper, paActionInfo.getLayer().getCommFB()->getInstanceName(), UA_StatusCode_name(retVal));
          somethingFailed = true;
        } else {
          if((*itNodePair)->mNodeId) {
            if(!UA_NodeId_equal((*itNodePair)->mNodeId, nodeId)) { //if NodeId was provided, check if found is the same
              DEVLOG_ERROR("[OPC UA CLIENT]: The call from FB %s failed the found nodeId of the method doesn't match the provided one\n",
                paActionInfo.getLayer().getCommFB()->getInstanceName());
              somethingFailed = true;
            }
            UA_NodeId_delete(nodeId);
          } else {
            (*itNodePair)->mNodeId = nodeId;
          }
        }
      }
    }

    //for subscription, more things are needed
    if(!somethingFailed && !initializeSubscription(paActionInfo)) { //won't initialize subscription if some nodeID is missing
      somethingFailed = true;
    }
  }
  return somethingFailed;
}

bool CUA_ClientInformation::initializeCallMethod(CActionInfo& paActionInfo) {
  bool somethingFailed = false;

  CSinglyLinkedList<CActionInfo::CNodePairInfo*>::Iterator itNodePair = paActionInfo.getNodePairInfo().begin();
  //get parentNodeId and also the method NodeId
  UA_NodeId *methodNode;
  UA_NodeId *parentNode;

  UA_StatusCode retVal = COPC_UA_Helper::getRemoteNodeForPath(*mClient, (*itNodePair)->mBrowsePath.getValue(), &parentNode, &methodNode);

  if(UA_STATUSCODE_GOOD != retVal) {
    DEVLOG_ERROR("[OPC UA CLIENT]: The method call from FB %s failed because the requested node was not found. Error: %s\n",
      paActionInfo.getLayer().getCommFB()->getInstanceName(), UA_StatusCode_name(retVal));
    somethingFailed = true;
  } else {
    if((*itNodePair)->mNodeId) {
      if(!UA_NodeId_equal((*itNodePair)->mNodeId, methodNode)) { //if NodeId of method was provided, check if found is the same
        DEVLOG_ERROR("[OPC UA CLIENT]: The method call from FB %s failed the found nodeId of the method doesn't match the provided one\n",
          paActionInfo.getLayer().getCommFB()->getInstanceName());
        somethingFailed = true;
      }
      UA_NodeId_delete(methodNode);
    } else {
      (*itNodePair)->mNodeId = methodNode;
    }
    if(!somethingFailed) {
      //store the parentNodeId in the second position. BrowseName is not needed
      paActionInfo.getNodePairInfo().pushBack(new CActionInfo::CNodePairInfo(parentNode, ""));
    } else {
      UA_NodeId_delete(parentNode);
    }
  }

  return !somethingFailed;
}

bool CUA_ClientInformation::initializeSubscription(CActionInfo& paActionInfo) {
  bool somethingFailed = false;
  if(CActionInfo::eSubscribe == paActionInfo.getAction() && allocAndCreateSubscription()) {

    size_t itemsAddedToList = 0;

    CSinglyLinkedList<UA_MonitoringItemInfo>::Iterator itFirstNewMonitoringItemInfo = mSubscriptionInfo->mMonitoredItems.end();

    for(size_t i = 0; i < paActionInfo.getNoOfNodePairs(); i++) {
      UA_MonitoringItemInfo monitoringItemInfo(UA_SubscribeContext_Handle(paActionInfo, itemsAddedToList));
      mSubscriptionInfo->mMonitoredItems.pushBack(monitoringItemInfo);
      if(itFirstNewMonitoringItemInfo == mSubscriptionInfo->mMonitoredItems.end()) { //store the first added item
        itFirstNewMonitoringItemInfo = mSubscriptionInfo->mMonitoredItems.back();
      }
      itemsAddedToList++;
    }

    CSinglyLinkedList<CActionInfo::CNodePairInfo*>::Iterator itNodePairInfo = paActionInfo.getNodePairInfo().begin();
    size_t itemsAddedToLibrary = 0;

    CSinglyLinkedList<UA_MonitoringItemInfo>::Iterator itAddedMonitoringItemInfo = itFirstNewMonitoringItemInfo;

    for(itemsAddedToLibrary = 0; itemsAddedToLibrary < itemsAddedToList; ++itAddedMonitoringItemInfo, ++itNodePairInfo) {
      if(!addMonitoringItem(*itAddedMonitoringItemInfo, *(*itNodePairInfo)->mNodeId)) {
        somethingFailed = true;
        break;
      }
      itemsAddedToLibrary++;
    }

    if(!somethingFailed) {
      addAsyncCall();
    } else { //if something failed, remove added monitoring items and fail the whole action

      for(size_t i = 0; i < itemsAddedToList; i++) {
        if(i < itemsAddedToLibrary) { //remove items from the library
          UA_StatusCode retVal = UA_Client_MonitoredItems_deleteSingle(mClient, mSubscriptionInfo->mSubscriptionId,
            (*itFirstNewMonitoringItemInfo).mMonitoringItemId);
          if(UA_STATUSCODE_GOOD != retVal) {
            DEVLOG_ERROR("[OPC UA CLIENT]: Couldn't delete recently added monitored item %u. Error: %s\n", (*itFirstNewMonitoringItemInfo).mMonitoringItemId, UA_StatusCode_name(retVal));
          }
        }
        itAddedMonitoringItemInfo = itFirstNewMonitoringItemInfo;
        ++itFirstNewMonitoringItemInfo;
        mSubscriptionInfo->mMonitoredItems.erase(*itAddedMonitoringItemInfo);
      }
    }
  }
  return !somethingFailed;
}

bool CUA_ClientInformation::allocAndCreateSubscription() {
  bool somethingFailed = false;
  if(!mSubscriptionInfo) {
    mSubscriptionInfo = new UA_subscriptionInfo();
    if(!createSubscription()) {
      delete mSubscriptionInfo;
      mSubscriptionInfo = 0;
      somethingFailed = true;
    }
  }
  return !somethingFailed;
}

bool CUA_ClientInformation::createSubscription() {
  UA_CreateSubscriptionRequest request = UA_CreateSubscriptionRequest_default();
  request.requestedPublishingInterval = FORTE_COM_OPC_UA_CLIENT_PUB_INTERVAL;
  UA_CreateSubscriptionResponse response = UA_Client_Subscriptions_create(mClient, request, this, 0, CUA_RemoteCallbackFunctions::deleteSubscriptionCallback);
  if(UA_STATUSCODE_GOOD == response.responseHeader.serviceResult) {
    DEVLOG_INFO("[OPC UA CLIENT]: Create subscription to %s succeeded, id %u\n", mEndpointUrl.getValue(), response.subscriptionId);
    mSubscriptionInfo->mSubscriptionId = response.subscriptionId;
    return true;
  } else {
    DEVLOG_ERROR("[OPC UA CLIENT]: Create subscription to %s failed. Error: %s\n", mEndpointUrl.getValue(), UA_StatusCode_name(response.responseHeader.serviceResult));
  }

  return false;
}

bool CUA_ClientInformation::addMonitoringItem(UA_MonitoringItemInfo &paMonitoringInfo, const UA_NodeId &paNodeId) {

  const UA_MonitoredItemCreateRequest monRequest = UA_MonitoredItemCreateRequest_default(paNodeId);
  UA_MonitoredItemCreateResult monResponse = UA_Client_MonitoredItems_createDataChange(mClient, mSubscriptionInfo->mSubscriptionId, UA_TIMESTAMPSTORETURN_BOTH,
    monRequest, static_cast<void *>(&paMonitoringInfo.mVariableInfo), CUA_RemoteCallbackFunctions::subscriptionValueChangedCallback, 0);
  if(UA_STATUSCODE_GOOD == monResponse.statusCode) {
    DEVLOG_INFO("[OPC UA CLIENT]: Monitoring of FB %s at index %u succeeded. The monitoring item id is %u\n",
      paMonitoringInfo.mVariableInfo.mActionInfo.getLayer().getCommFB()->getInstanceName(), paMonitoringInfo.mVariableInfo.mPortIndex,
      monResponse.monitoredItemId);
    paMonitoringInfo.mMonitoringItemId = monResponse.monitoredItemId;
  } else {
    DEVLOG_ERROR("[OPC UA CLIENT]: Monitoring of FB %s at index %u failed. Error: %s\n",
      paMonitoringInfo.mVariableInfo.mActionInfo.getLayer().getCommFB()->getInstanceName(), paMonitoringInfo.mVariableInfo.mPortIndex,
      UA_StatusCode_name(monResponse.statusCode));
  }

  return (UA_STATUSCODE_GOOD == monResponse.statusCode);
}

void CUA_ClientInformation::addAsyncCall() {
  mMissingAsyncCalls++;
}

void CUA_ClientInformation::removeAsyncCall() {
  mMissingAsyncCalls--;
}

void CUA_ClientInformation::uninitializeAction(CActionInfo& paActionInfo) {
  mActionsToBeInitialized.erase(&paActionInfo); //remove in case it is still not initialized
  if(CActionInfo::eSubscribe == paActionInfo.getAction()) { //only subscription has something to release
    uninitializeSubscribeAction(paActionInfo);
  }
}

void CUA_ClientInformation::uninitializeSubscribeAction(const CActionInfo &paActionInfo) {
  if(mSubscriptionInfo) {
    CSinglyLinkedList<UA_MonitoringItemInfo> toDelete;
    for(CSinglyLinkedList<UA_MonitoringItemInfo>::Iterator itMonitoringItemInfo = mSubscriptionInfo->mMonitoredItems.begin();
        itMonitoringItemInfo != mSubscriptionInfo->mMonitoredItems.end(); ++itMonitoringItemInfo) {
      if(&(*itMonitoringItemInfo).mVariableInfo.mActionInfo == &paActionInfo) {
        toDelete.pushBack(*itMonitoringItemInfo);
      }
    }
    for(CSinglyLinkedList<UA_MonitoringItemInfo>::Iterator itMonitoringItemInfo = toDelete.begin(); itMonitoringItemInfo != toDelete.end();
        ++itMonitoringItemInfo) {
      UA_StatusCode retVal = UA_Client_MonitoredItems_deleteSingle(mClient, mSubscriptionInfo->mSubscriptionId, (*itMonitoringItemInfo).mMonitoringItemId);
      if(UA_STATUSCODE_GOOD != retVal) {
        DEVLOG_ERROR("[OPC UA CLIENT]: Couldn't delete monitored item %u. No further actions will be taken. Error: %s\n",
          (*itMonitoringItemInfo).mMonitoringItemId, UA_StatusCode_name(retVal));

        // if the remote is unplugged the missing subscription is detected and deleted with the previous call to the stack,
        // so the callback is called and the subscription is cleaned already by this point
        if(!mSubscriptionInfo) {
          return;
        }
      }

      mSubscriptionInfo->mMonitoredItems.erase(*itMonitoringItemInfo);
    }

    if(mSubscriptionInfo->mMonitoredItems.isEmpty()) {
      resetSubscription(true);
    }
  }
}

void CUA_ClientInformation::resetSubscription(bool paDeleteSubscription) {
  if(mSubscriptionInfo) {
    removeAsyncCall();
    if(paDeleteSubscription) {
      UA_StatusCode retval = UA_Client_Subscriptions_deleteSingle(mClient, mSubscriptionInfo->mSubscriptionId);
      if(UA_STATUSCODE_GOOD != retval) {
        DEVLOG_ERROR("[OPC UA CLIENT]: Couldn't delete subscription %u. Failed with error %s. No further actions will be taken\n",
          mSubscriptionInfo->mSubscriptionId, UA_StatusCode_name(retval));
      }
    }

    delete mSubscriptionInfo;
    mSubscriptionInfo = 0;
  }
}

// ******************** CALLBACKS *************************

void CUA_ClientInformation::CUA_RemoteCallbackFunctions::readAsyncCallback(UA_Client *, void *paUserdata, UA_UInt32, UA_ReadResponse *paResponse) { //NOSONAR
  UA_RemoteCallHandle *remoteCallHandle = static_cast<UA_RemoteCallHandle*>(paUserdata);
  remoteCallHandle->mClientInformation.removeAsyncCall();

  COPC_UA_Helper::UA_RecvVariable_handle varHandle(paResponse->resultsSize);
  if(UA_STATUSCODE_GOOD == paResponse->responseHeader.serviceResult) {
    if(paResponse->resultsSize == remoteCallHandle->mActionInfo.getNoOfNodePairs()) {
      //check if all results are OK first
      for(size_t i = 0; i < paResponse->resultsSize; i++) {
        if(paResponse->results[i].hasStatus && UA_STATUSCODE_GOOD != paResponse->results[i].status) {
          DEVLOG_ERROR("[OPC UA CLIENT]: Reading for FB %s in client %s failed because the response for index %u has status %s\n",
            remoteCallHandle->mActionInfo.getLayer().getCommFB()->getInstanceName(), remoteCallHandle->mActionInfo.getEndpoint().getValue(), i,
            UA_StatusCode_name(paResponse->results[i].status));
          varHandle.mFailed = true;
          break;
        }
      }

      if(!varHandle.mFailed) {
        size_t indexOfPair = 0;
        for(CSinglyLinkedList<CActionInfo::CNodePairInfo*>::Iterator itNodePairs = remoteCallHandle->mActionInfo.getNodePairInfo().begin();
            itNodePairs != remoteCallHandle->mActionInfo.getNodePairInfo().end(); ++itNodePairs, indexOfPair++) {
          varHandle.mData[indexOfPair] = &paResponse->results[indexOfPair].value;
        }
      }
    } else {
      DEVLOG_ERROR("[OPC UA CLIENT]: Reading for FB %s in client %s failed because the response size is %u but the FB has %u values to read\n",
        remoteCallHandle->mActionInfo.getLayer().getCommFB()->getInstanceName(), remoteCallHandle->mActionInfo.getEndpoint().getValue(),
        paResponse->resultsSize, remoteCallHandle->mActionInfo.getNoOfNodePairs());
      varHandle.mFailed = true;
    }
  } else {
    DEVLOG_ERROR("[OPC UA CLIENT]: Reading for FB %s in client %s failed. Error: %s\n",
      remoteCallHandle->mActionInfo.getLayer().getCommFB()->getInstanceName(), remoteCallHandle->mActionInfo.getEndpoint().getValue(),
      UA_StatusCode_name(paResponse->responseHeader.serviceResult));
    varHandle.mFailed = true;
  }

  remoteCallHandle->mActionInfo.getLayer().recvData(static_cast<const void *>(&varHandle), 0);
  remoteCallHandle->mActionInfo.getLayer().getCommFB()->interruptCommFB(&remoteCallHandle->mActionInfo.getLayer());
  remoteCallHandle->mActionInfo.getLayer().triggerNewEvent();
  delete remoteCallHandle;
}

void CUA_ClientInformation::CUA_RemoteCallbackFunctions::writeAsyncCallback(UA_Client *, void *paUserdata, UA_UInt32, UA_WriteResponse *paResponse) { //NOSONAR
  UA_RemoteCallHandle *remoteCallHandle = static_cast<UA_RemoteCallHandle*>(paUserdata);
  remoteCallHandle->mClientInformation.removeAsyncCall();
  COPC_UA_Helper::UA_RecvVariable_handle varHandle(0);
  if(UA_STATUSCODE_GOOD == paResponse->responseHeader.serviceResult) {
    if(paResponse->resultsSize == remoteCallHandle->mActionInfo.getNoOfNodePairs()) {
      for(size_t i = 0; i < paResponse->resultsSize; i++) {
        if(UA_STATUSCODE_GOOD != paResponse->results[i]) {
          DEVLOG_ERROR("[OPC UA CLIENT]: Writing for FB %s in client %s failed because the response for index %u has status %s\n",
            remoteCallHandle->mActionInfo.getLayer().getCommFB()->getInstanceName(), remoteCallHandle->mActionInfo.getEndpoint().getValue(), i,
            UA_StatusCode_name(paResponse->results[i]));
          varHandle.mFailed = true;
          break;
        }
      }
    } else {
      DEVLOG_ERROR("[OPC UA CLIENT]: Writing for FB %s in client %s failed because the response size is %u but the FB has %u values to write\n",
        remoteCallHandle->mActionInfo.getLayer().getCommFB()->getInstanceName(), remoteCallHandle->mActionInfo.getEndpoint().getValue(),
        paResponse->resultsSize, remoteCallHandle->mActionInfo.getNoOfNodePairs());
      varHandle.mFailed = true;
    }
  } else {
    DEVLOG_ERROR("[OPC UA CLIENT]: Writing for FB %s in client %s failed. Error: %s\n",
      remoteCallHandle->mActionInfo.getLayer().getCommFB()->getInstanceName(), remoteCallHandle->mActionInfo.getEndpoint().getValue(),
      UA_StatusCode_name(paResponse->responseHeader.serviceResult));
    varHandle.mFailed = true;
  }

  remoteCallHandle->mActionInfo.getLayer().recvData(static_cast<const void *>(&varHandle), 0);
  remoteCallHandle->mActionInfo.getLayer().getCommFB()->interruptCommFB(&remoteCallHandle->mActionInfo.getLayer());
  remoteCallHandle->mActionInfo.getLayer().triggerNewEvent();

  delete remoteCallHandle;
}

void CUA_ClientInformation::CUA_RemoteCallbackFunctions::callMethodAsyncCallback( //We omit SONAR only for the parameters
    UA_Client*, void *paUserdata, UA_UInt32, void *paResponse) { //NOSONAR
  const UA_CallResponse *response = static_cast<UA_CallResponse*>(paResponse);

  bool somethingFailed = false;

  UA_RemoteCallHandle *remoteCallHandle = static_cast<UA_RemoteCallHandle*>(paUserdata);
  remoteCallHandle->mClientInformation.removeAsyncCall();

  if(UA_STATUSCODE_GOOD == response->responseHeader.serviceResult) {
    if(1 == response->resultsSize) {
      if(UA_STATUSCODE_GOOD == response->results[0].statusCode) {

        if(remoteCallHandle->mActionInfo.getLayer().getCommFB()->getNumRD() != response->results[0].outputArgumentsSize) {
          DEVLOG_ERROR(
            "[OPC UA CLIENT]: Calling for FB %s in client %s failed because the number of RD connectors of the client %u does not match the number of returned values %u from the method call\n",
            remoteCallHandle->mActionInfo.getLayer().getCommFB()->getInstanceName(), remoteCallHandle->mActionInfo.getEndpoint().getValue(),
            remoteCallHandle->mActionInfo.getLayer().getCommFB()->getNumRD(), response->results->outputArgumentsSize);
          somethingFailed = true;
        } else {
          for(size_t i = 0; i < response->results->inputArgumentResultsSize; i++) {
            if(UA_STATUSCODE_GOOD != response->results->inputArgumentResults[i]) {
              DEVLOG_ERROR("[OPC UA CLIENT]: Calling for FB %s in client %s failed because the input response for index %u has status %s\n",
                remoteCallHandle->mActionInfo.getLayer().getCommFB()->getInstanceName(), remoteCallHandle->mActionInfo.getEndpoint().getValue(), i,
                UA_StatusCode_name(response->results->inputArgumentResults[i]));
              somethingFailed = true;
              break;
            }
          }
        }
      } else {
        DEVLOG_ERROR("[OPC UA CLIENT]: Calling for FB %s in client %s failed with the specific error: %s\n",
          remoteCallHandle->mActionInfo.getLayer().getCommFB()->getInstanceName(), remoteCallHandle->mActionInfo.getEndpoint().getValue(),
          UA_StatusCode_name(response->results->statusCode));
        somethingFailed = true;
      }
    } else {
      DEVLOG_ERROR("[OPC UA CLIENT]: Calling for FB %s in client %s failed because the response size is %u, different from 1\n",
        remoteCallHandle->mActionInfo.getLayer().getCommFB()->getInstanceName(), remoteCallHandle->mActionInfo.getEndpoint().getValue(),
        response->resultsSize);
      somethingFailed = true;
    }
  } else {
    DEVLOG_ERROR("[OPC UA CLIENT]: Calling for FB %s in client %s failed with the main error: %s\n",
      remoteCallHandle->mActionInfo.getLayer().getCommFB()->getInstanceName(), remoteCallHandle->mActionInfo.getEndpoint().getValue(),
      UA_StatusCode_name(response->responseHeader.serviceResult));
    somethingFailed = true;
  }
  size_t outputSize = 0;
  if(!somethingFailed) {
    outputSize = response->results->outputArgumentsSize;
  }
  //call layer even when it failed, to let the FB know
  COPC_UA_Helper::UA_SendVariable_handle varHandle(outputSize);
  varHandle.mFailed = somethingFailed;

  if(!varHandle.mFailed) {
    for(size_t i = 0; i < outputSize; i++) {
      varHandle.mData[i] = &response->results->outputArguments[i];
    }
  }

  remoteCallHandle->mActionInfo.getLayer().recvData(static_cast<const void *>(&varHandle), 0);
  remoteCallHandle->mActionInfo.getLayer().getCommFB()->interruptCommFB(&remoteCallHandle->mActionInfo.getLayer());
  remoteCallHandle->mActionInfo.getLayer().triggerNewEvent();

  delete remoteCallHandle;
}

void CUA_ClientInformation::CUA_RemoteCallbackFunctions::subscriptionValueChangedCallback(UA_Client *, UA_UInt32, void *, UA_UInt32, void *paMonContext, //NOSONAR
    UA_DataValue *paData) { //NOSONAR
  if(paData->hasValue) {

    UA_SubscribeContext_Handle *variableContextHandle = static_cast<UA_SubscribeContext_Handle *>(paMonContext);

    COPC_UA_Helper::UA_RecvVariable_handle handleRecv(1);

    const UA_Variant *value = &paData->value;
    handleRecv.mData[0] = value;
    handleRecv.mOffset = variableContextHandle->mPortIndex;

    forte::com_infra::EComResponse retVal = variableContextHandle->mActionInfo.getLayer().recvData(static_cast<const void *>(&handleRecv), 0);

    if(forte::com_infra::e_Nothing != retVal) {
      variableContextHandle->mActionInfo.getLayer().getCommFB()->interruptCommFB(&variableContextHandle->mActionInfo.getLayer());
      variableContextHandle->mActionInfo.getLayer().triggerNewEvent();
    }
  }
}

void CUA_ClientInformation::CUA_RemoteCallbackFunctions::deleteSubscriptionCallback(UA_Client *, UA_UInt32 paSubscriptionId, void *paSubscriptionContext) { //NOSONAR
  DEVLOG_INFO("[OPC UA CLIENT]: Subscription Id %u was deleted in client with endpoint %s\n", paSubscriptionId,
    static_cast<CUA_ClientInformation*>(paSubscriptionContext)->mEndpointUrl.getValue());
  static_cast<CUA_ClientInformation*>(paSubscriptionContext)->resetSubscription(false);
}

void CUA_ClientInformation::CUA_RemoteCallbackFunctions::clientStateChangeCallback( //We omit SONAR only for the parameters
    UA_Client*, UA_ClientState paClientState //NOSONAR
    ) {
  //Don't do anything here. If the subscription is deleted, deleteSubscriptionCallback will be called and handled there
  switch(paClientState){
    case UA_CLIENTSTATE_DISCONNECTED:
      DEVLOG_INFO("[OPC UA CLIENT]: The client is disconnected\n");
      break;
    case UA_CLIENTSTATE_CONNECTED:
      DEVLOG_INFO("[OPC UA CLIENT]: A TCP connection to the server is open\n");
      break;
    case UA_CLIENTSTATE_SECURECHANNEL:
      DEVLOG_INFO("[OPC UA CLIENT]: A SecureChannel to the server is open\n");
      break;
    case UA_CLIENTSTATE_SESSION:
      DEVLOG_INFO("[OPC UA CLIENT]: A session with the server is open\n");
      break;
    case UA_CLIENTSTATE_SESSION_RENEWED:
      DEVLOG_INFO("[OPC UA CLIENT]: A session with the server is open (renewed)\n");
      break;
    default:
      DEVLOG_ERROR("[OPC UA CLIENT]: Unknown state of client %d\n", paClientState);
  }
  return;
}
