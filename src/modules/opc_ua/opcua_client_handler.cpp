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

#include "opcua_client_handler.h"
#include "../../core/devexec.h"
#include "../../core/iec61131_functions.h"
#include "../../core/cominfra/basecommfb.h"
#include <criticalregion.h>
#include <forte_printer.h>

using namespace forte::com_infra;

DEFINE_HANDLER(COPC_UA_Client_Handler);

COPC_UA_Client_Handler::COPC_UA_Client_Handler(CDeviceExecution& paDeviceExecution) :
    COPC_UA_HandlerAbstract(paDeviceExecution), mConnectionHandler(*this), mNewClientsPresent(false) {
}

COPC_UA_Client_Handler::~COPC_UA_Client_Handler() {
  stopLoops();
  cleanResources();
}

void COPC_UA_Client_Handler::cleanResources() {
  CCriticalRegion criticalRegion(mAllClientListMutex);
  for(CSinglyLinkedList<UA_ClientInformation *>::Iterator iter = mAllClients.begin(); iter != mAllClients.end(); ++iter) {
    cleanClient(*iter);
    delete *iter;
  }
  mAllClients.clearAll();
}

void COPC_UA_Client_Handler::enableHandler(void) {
  startLoops();
}

void COPC_UA_Client_Handler::disableHandler(void) {
  stopLoops();
}

void COPC_UA_Client_Handler::startLoops() {
  if(!isAlive()) {
    start();
    mClientsThreadStarted.waitIndefinitely();
  }
  mConnectionHandler.startConnectionsThread();
}

void COPC_UA_Client_Handler::stopLoops() {
  mConnectionHandler.stopConnectionsThread();
  setAlive(false);
  mAsyncIsNeeded.inc();
  end();
}

void COPC_UA_Client_Handler::addClientToInitializedList(UA_ClientInformation* paClientInformation) {
  CCriticalRegion connectionListRegion(mNewClientstMutex);
  addClientToList(paClientInformation, mNewClients);
  if(paClientInformation->mMissingAsyncCalls) { //for subscription the missing calls will be set already, for others not
    mAsyncIsNeeded.inc();
  }
  mNewClientsPresent = true;
}

void COPC_UA_Client_Handler::removeClientFromInitializedList(UA_ClientInformation* paClientInformation) {
  CCriticalRegion iniitalizedClientsRegion(mInitializedClientstMutex);
  CCriticalRegion newClientsRegion(mNewClientstMutex);
  mNewClients.erase(paClientInformation); //client could still be in the newList
  mInitializeClients.erase(paClientInformation);
}

void COPC_UA_Client_Handler::updateClientList() {
  CCriticalRegion iniitalizedClientsRegion(mInitializedClientstMutex);
  CCriticalRegion newClientsRegion(mNewClientstMutex);
  for(CSinglyLinkedList<UA_ClientInformation*>::Iterator it = mNewClients.begin(); it != mNewClients.end(); ++it) {
    addClientToList(*it, mInitializeClients);
  }
  mNewClients.clearAll();
  mNewClientsPresent = false;
}

void COPC_UA_Client_Handler::addClientToConnectionList(UA_ClientInformation* paClientInformation) {
  mConnectionHandler.addClient(paClientInformation);
}

void COPC_UA_Client_Handler::run() {
  DEVLOG_INFO("[OPC UA REMOTE]: Starting OPC UA Client handler\n");

  mClientsThreadStarted.inc();
  while(isAlive()) {

    if(mNewClientsPresent) {
      updateClientList();
    }

    bool needsAsync = handleClients();
    if(isAlive()) {
      if(needsAsync) {
        mAsyncIsNeeded.timedWait(scmNanosecondsToSleep);
      } else {
        mAsyncIsNeeded.waitIndefinitely();
      }
    }
  }
  DEVLOG_INFO("[OPC UA REMOTE]: Stopping OPC UA Client handler\n");
}

bool COPC_UA_Client_Handler::handleClients() {
  CCriticalRegion iniitalizedClientsRegion(mInitializedClientstMutex); //this is needed because removing a client from the list could cause trouble
  CSinglyLinkedList<UA_ClientInformation *> failedClients;
  bool asyncIsNeeded = false;
  for(CSinglyLinkedList<UA_ClientInformation *>::Iterator iter = mInitializeClients.begin(); iter != mInitializeClients.end(); ++iter) {
    CCriticalRegion criticalRegionClienMutex((*iter)->mClientMutex);
    if((*iter)->mMissingAsyncCalls) {
      if(UA_STATUSCODE_GOOD !=
#ifdef FORTE_COM_OPC_UA_MASTER_BRANCH
        UA_Client_run_iterate(
#else
        UA_Client_runAsync(
#endif
          (*iter)->mClient, 10)) {
        failedClients.pushBack(*iter);
      } else {
        if((*iter)->mMissingAsyncCalls) {
          asyncIsNeeded = true;
        }
      }
      if(!isAlive()) {
        break;
      }
    }
  }

  if(isAlive() && !failedClients.isEmpty()) {
    for(CSinglyLinkedList<UA_ClientInformation *>::Iterator iter = failedClients.begin(); iter != failedClients.end(); ++iter) {
      DEVLOG_ERROR("[OPC UA REMOTE]: There was a problem checking remote %s.\n", (*iter)->mEndpointUrl.getValue());
      (*iter)->mMissingAsyncCalls = 0;
      mInitializeClients.erase(*iter);
      addClientToConnectionList(*iter);
    }
  }
  return asyncIsNeeded;
}

void COPC_UA_Client_Handler::addClientToList(UA_ClientInformation* paClientInformation, CSinglyLinkedList<UA_ClientInformation*>& paList) {
  bool elementAlreadyPresent = false;
  for(CSinglyLinkedList<UA_ClientInformation*>::Iterator it = paList.begin(); it != paList.end(); ++it) {
    if(paClientInformation == (*it)) {
      elementAlreadyPresent = true;
      break;
    }
  }

  if(!elementAlreadyPresent) {
    paList.pushBack(paClientInformation);
  }
}

void COPC_UA_Client_Handler::addAsyncCall(UA_ClientInformation* paClientInformation) {
  paClientInformation->mMissingAsyncCalls++;
  mAsyncIsNeeded.inc();
}

void COPC_UA_Client_Handler::removeAsyncCall(UA_ClientInformation* paClientInformation) {
  paClientInformation->mMissingAsyncCalls--;
}

void COPC_UA_Client_Handler::resetSubscription(UA_ClientInformation* paClientInformation) {
  removeAsyncCall(paClientInformation);
  if(paClientInformation->mSubscriptionInfo) {
    paClientInformation->mSubscriptionInfo->mSubscriptionAlreadyCreated = false;
  }
  //TODO: add to connection thread and check when exactly this could be called
}

#ifdef FORTE_COM_OPC_UA_MASTER_BRANCH
//not used
#else
void COPC_UA_Client_Handler::onAsyncCallReturn(UA_Client*paClient, void* paUserdata, UA_UInt32 paRequestId, void* paResponse, //NOSONAR
    const UA_DataType* paResponseType) {
  if(&UA_TYPES[UA_TYPES_READRESPONSE] == paResponseType) {
    handleReadReturn(paClient, paUserdata, paRequestId, static_cast<UA_ReadResponse*>(paResponse));
  } else if(&UA_TYPES[UA_TYPES_WRITERESPONSE] == paResponseType) {
    handleWriteReturn(paClient, paUserdata, paRequestId, static_cast<UA_WriteResponse*>(paResponse));
  } else if(&UA_TYPES[UA_TYPES_CALLRESPONSE] == paResponseType) {
    handleCallReturn(paClient, paUserdata, paRequestId, static_cast<UA_CallResponse*>(paResponse));
  } else {
    DEVLOG_ERROR("[OPC UA REMOTE]: Client has a return call from an unknown type %s\n", paResponseType->typeName);
  }
}
#endif

void COPC_UA_Client_Handler::handleReadReturn(UA_Client *, void *paUserdata, UA_UInt32, UA_ReadResponse *paResponse) {
  UA_RemoteCallHandle* handle = static_cast<UA_RemoteCallHandle*>(paUserdata);
  removeAsyncCall(handle->mClient);

  COPC_UA_HandlerAbstract::UA_RecvVariable_handle varHandle(paResponse->resultsSize);
  if(UA_STATUSCODE_GOOD == paResponse->responseHeader.serviceResult) {
    if(paResponse->resultsSize == handle->mAction->getNoOfNodePairs()) {
      //check if all results are OK first
      for(size_t i = 0; i < paResponse->resultsSize; i++) {
        if(paResponse->results[i].hasStatus && UA_STATUSCODE_GOOD != paResponse->results[i].status) {
          DEVLOG_ERROR("[OPC UA REMOTE]: Reading for FB %s in client %s failed because the response for index %u has status %s\n",
            handle->mAction->getLayer()->getCommFB()->getInstanceName(), handle->mAction->getEndpoint().getValue(), i,
            UA_StatusCode_name(paResponse->results[i].status));
          varHandle.mFailed = true;
          break;
        }
      }

      if(!varHandle.mFailed) {
        size_t indexOfPair = 0;
        CSinglyLinkedList<UA_TypeConvert*>::Iterator itType = handle->mAction->getLayer()->getTypeConveverters().begin();
        for(CSinglyLinkedList<CNodePairInfo*>::Iterator itNodePairs = handle->mAction->getNodePairInfo().begin();
            itNodePairs != handle->mAction->getNodePairInfo().end(); ++itNodePairs, indexOfPair++, ++itType) {

          varHandle.mData[indexOfPair] = &paResponse->results[indexOfPair].value;
          varHandle.mConvert[indexOfPair] = *itType;
        }
      }
    } else {
      DEVLOG_ERROR("[OPC UA REMOTE]: Reading for FB %s in client %s failed because the response size is %u but the FB has %u values to read\n",
        handle->mAction->getLayer()->getCommFB()->getInstanceName(), handle->mAction->getEndpoint().getValue(), paResponse->resultsSize,
        handle->mAction->getNoOfNodePairs());
      varHandle.mFailed = true;
    }
  } else {
    DEVLOG_ERROR("[OPC UA REMOTE]: Reading for FB %s in client %s failed with error: %s\n", handle->mAction->getLayer()->getCommFB()->getInstanceName(),
      handle->mAction->getEndpoint().getValue(), UA_StatusCode_name(paResponse->responseHeader.serviceResult));
    varHandle.mFailed = true;
  }

  handle->mAction->getLayer()->recvData(static_cast<const void *>(&varHandle), 0);
  handle->mAction->getLayer()->getCommFB()->interruptCommFB(handle->mAction->getLayer());
  ::getExtEvHandler<COPC_UA_Client_Handler>(*handle->mAction->getLayer()->getCommFB()).startNewEventChain(handle->mAction->getLayer()->getCommFB());
  delete handle;
}

void COPC_UA_Client_Handler::handleWriteReturn(UA_Client *, void *paUserdata, UA_UInt32, UA_WriteResponse *paResponse) {
  UA_RemoteCallHandle* handle = static_cast<UA_RemoteCallHandle*>(paUserdata);
  removeAsyncCall(handle->mClient);
  COPC_UA_HandlerAbstract::UA_RecvVariable_handle varHandle(0);
  if(UA_STATUSCODE_GOOD == paResponse->responseHeader.serviceResult) {
    if(paResponse->resultsSize == handle->mAction->getNoOfNodePairs()) {
      for(size_t i = 0; i < paResponse->resultsSize; i++) {
        if(UA_STATUSCODE_GOOD != paResponse->results[i]) {
          DEVLOG_ERROR("[OPC UA REMOTE]: Writing for FB %s in client %s failed because the response for index %u has status %s\n",
            handle->mAction->getLayer()->getCommFB()->getInstanceName(), handle->mAction->getEndpoint().getValue(), i,
            UA_StatusCode_name(paResponse->results[i]));
          varHandle.mFailed = true;
          break;
        }
      }
    } else {
      DEVLOG_ERROR("[OPC UA REMOTE]: Writing for FB %s in client %s failed because the response size is %u but the FB has %u values to write\n",
        handle->mAction->getLayer()->getCommFB()->getInstanceName(), handle->mAction->getEndpoint().getValue(), paResponse->resultsSize,
        handle->mAction->getNoOfNodePairs());
      varHandle.mFailed = true;
    }
  } else {
    DEVLOG_ERROR("[OPC UA REMOTE]: Writing for FB %s in client %s failed with error: %s\n", handle->mAction->getLayer()->getCommFB()->getInstanceName(),
      handle->mAction->getEndpoint().getValue(), UA_StatusCode_name(paResponse->responseHeader.serviceResult));
    varHandle.mFailed = true;
  }

  handle->mAction->getLayer()->recvData(static_cast<const void *>(&varHandle), 0);
  handle->mAction->getLayer()->getCommFB()->interruptCommFB(handle->mAction->getLayer());
  ::getExtEvHandler<COPC_UA_Client_Handler>(*handle->mAction->getLayer()->getCommFB()).startNewEventChain(handle->mAction->getLayer()->getCommFB());

  delete handle;
}

void COPC_UA_Client_Handler::handleCallReturn(UA_Client *, void *paUserdata, UA_UInt32,
#ifdef FORTE_COM_OPC_UA_MASTER_BRANCH
    void* paResponse_) {
  UA_CallResponse *paResponse = static_cast<UA_CallResponse*>(paResponse_);
#else
  UA_CallResponse *paResponse) {
#endif

  bool somethingFailed = false;

  UA_RemoteCallHandle* handle = static_cast<UA_RemoteCallHandle*>(paUserdata);
  removeAsyncCall(handle->mClient);

  if(UA_STATUSCODE_GOOD == paResponse->responseHeader.serviceResult) {
    if(1 == paResponse->resultsSize) {
      if(UA_STATUSCODE_GOOD == paResponse->results[0].statusCode) {

        if(handle->mAction->getLayer()->getCommFB()->getNumRD() != paResponse->results[0].outputArgumentsSize) {
          DEVLOG_ERROR(
            "[OPC UA REMOTE]: Calling for FB %s in client %s failed because the number of RD connectors of the client %u does not match the number of returned values %u from the method call\n",
            handle->mAction->getLayer()->getCommFB()->getInstanceName(), handle->mAction->getEndpoint().getValue(),
            handle->mAction->getLayer()->getCommFB()->getNumRD(), paResponse->results->outputArgumentsSize);
          somethingFailed = true;
        } else {
          for(size_t i = 0; i < paResponse->results->inputArgumentResultsSize; i++) {
            if(UA_STATUSCODE_GOOD != paResponse->results->inputArgumentResults[i]) {
              DEVLOG_ERROR("[OPC UA REMOTE]: Calling for FB %s in client %s failed because the input response for index %u has status %s\n",
                handle->mAction->getLayer()->getCommFB()->getInstanceName(), handle->mAction->getEndpoint().getValue(), i,
                UA_StatusCode_name(paResponse->results->inputArgumentResults[i]));
              somethingFailed = true;
              break;
            }
          }
        }
      } else {
        DEVLOG_ERROR("[OPC UA REMOTE]: Calling for FB %s in client %s failed with the specific error: %s\n",
          handle->mAction->getLayer()->getCommFB()->getInstanceName(), handle->mAction->getEndpoint().getValue(),
          UA_StatusCode_name(paResponse->results->statusCode));
        somethingFailed = true;
      }
    } else {
      DEVLOG_ERROR("[OPC UA REMOTE]: Calling for FB %s in client %s failed because the response size is %u, different from 1\n",
        handle->mAction->getLayer()->getCommFB()->getInstanceName(), handle->mAction->getEndpoint().getValue(), paResponse->resultsSize);
      somethingFailed = true;
    }
  } else {
    DEVLOG_ERROR("[OPC UA REMOTE]: Calling for FB %s in client %s failed with the main error: %s\n",
      handle->mAction->getLayer()->getCommFB()->getInstanceName(), handle->mAction->getEndpoint().getValue(),
      UA_StatusCode_name(paResponse->responseHeader.serviceResult));
    somethingFailed = true;
  }
  size_t outputSize = 0;
  if(!somethingFailed) {
    outputSize = paResponse->results->outputArgumentsSize;
  }
  //call layer even when it failed, to let the FB know
  COPC_UA_HandlerAbstract::UA_SendVariable_handle varHandle(outputSize);
  varHandle.mFailed = somethingFailed;

  if(!varHandle.mFailed) {
    for(size_t i = 0; i < outputSize; i++) {
      varHandle.mData[i] = &paResponse->results->outputArguments[i];
    }
    CSinglyLinkedList<UA_TypeConvert*>::Iterator itType = handle->mAction->getLayer()->getTypeConveverters().begin();

    //skip all SDs first
    for(size_t i = 0; i < handle->mAction->getLayer()->getCommFB()->getNumSD(); i++, ++itType) {
    }

    for(size_t i = 0; i < paResponse->results->outputArgumentsSize; i++, ++itType) {
      varHandle.mConvert[i] = *itType;
    }
  }

  handle->mAction->getLayer()->recvData(static_cast<const void *>(&varHandle), 0);
  handle->mAction->getLayer()->getCommFB()->interruptCommFB(handle->mAction->getLayer());
  ::getExtEvHandler<COPC_UA_Client_Handler>(*handle->mAction->getLayer()->getCommFB()).startNewEventChain(handle->mAction->getLayer()->getCommFB());

  delete handle;
}

void COPC_UA_Client_Handler::onSubscriptionValueChanged(UA_Client *, UA_UInt32, void *, UA_UInt32, void *paMonContext, UA_DataValue *paData) { //NOSONAR
  if(paData->hasValue) {
    UA_VariableCallback_Handle *handle = static_cast<struct UA_VariableCallback_Handle *>(paMonContext);

    UA_RecvVariable_handle handleRecv(1);

    const UA_Variant* value = &paData->value;
    handleRecv.mData[0] = value;
    handleRecv.mOffset = handle->mPortIndex;
    handleRecv.mConvert[0] = handle->mConvert;

    EComResponse retVal = handle->mLayer->recvData(static_cast<const void *>(&handleRecv), 0);

    if(e_Nothing != retVal) {
      handle->mLayer->getCommFB()->interruptCommFB(handle->mLayer);
      ::getExtEvHandler<COPC_UA_Client_Handler>(*handle->mLayer->getCommFB()).startNewEventChain(handle->mLayer->getCommFB());
    }
  }
}

void COPC_UA_Client_Handler::deleteSubscriptionCallback(UA_Client *, UA_UInt32 subscriptionId, void *subscriptionContext) { //NOSONAR
  DEVLOG_INFO("[OPC UA REMOTE]: Subscription Id %u was deleted in client with endpoint %s\n", subscriptionId,
    static_cast<UA_ClientInformation*>(subscriptionContext)->mEndpointUrl.getValue());
  COPC_UA_Client_Handler::resetSubscription(static_cast<UA_ClientInformation*>(subscriptionContext));
}

void COPC_UA_Client_Handler::stateCallback(UA_Client *, UA_ClientState clientState) {
//TODO: check in which state should we renovate the susbcription
  switch(clientState){
    case UA_CLIENTSTATE_DISCONNECTED:
      DEVLOG_INFO("[OPC UA REMOTE]: The client is disconnected\n");
      break;
    case UA_CLIENTSTATE_CONNECTED:
      DEVLOG_INFO("[OPC UA REMOTE]: A TCP connection to the server is open\n");
      break;
    case UA_CLIENTSTATE_SECURECHANNEL:
      DEVLOG_INFO("[OPC UA REMOTE]: A SecureChannel to the server is open\n");
      break;
    case UA_CLIENTSTATE_SESSION:
      DEVLOG_INFO("[OPC UA REMOTE]: A session with the server is open\n");
      break;
    case UA_CLIENTSTATE_SESSION_RENEWED:
      DEVLOG_INFO("[OPC UA REMOTE]: A session with the server is open (renewed)\n");
      break;
    default:
      DEVLOG_ERROR("[OPC UA REMOTE]: Unknown state of client %d\n", clientState);
  }
  return;
}

COPC_UA_Client_Handler::UA_ClientInformation* COPC_UA_Client_Handler::getClient(CIEC_STRING& paEndpoint) {
  CCriticalRegion allClientsRegion(mAllClientListMutex);

  UA_ClientInformation* client = 0;

  for(CSinglyLinkedList<UA_ClientInformation *>::Iterator iter = mAllClients.begin(); iter != mAllClients.end(); ++iter) {
    if((*iter)->mEndpointUrl == paEndpoint) {
      client = (*iter);
      break;
    }
  }
  if(!client) {
    client = new UA_ClientInformation();
    client->mEndpointUrl = paEndpoint;

#ifdef FORTE_COM_OPC_UA_MASTER_BRANCH
    client->mClient = UA_Client_new();
    UA_ClientConfig *configPointer = UA_Client_getConfig(client->mClient);
    UA_ClientConfig config = *configPointer;
    if(UA_STATUSCODE_GOOD != UA_ClientConfig_setDefault(configPointer)) {
      DEVLOG_ERROR("[OPC UA REMOTE]: Error setting default client config\n");
      UA_Client_delete(client->mClient);
      delete client;
      return 0;
    }
    configPointer->stateCallback = stateCallback;
    configPointer->logger = COPC_UA_HandlerAbstract::getLogger();
    configPointer->timeout = scmClientTimeoutInMilliseconds;
#else //FORTE_COM_OPC_UA_MASTER_BRANCH
    UA_ClientConfig config = UA_ClientConfig_default;
    config.stateCallback = stateCallback;
    config.logger = COPC_UA_HandlerAbstract::getLogger();
    config.timeout = scmClientTimeoutInMilliseconds;
    client->mClient = UA_Client_new(config);
#endif //FORTE_COM_OPC_UA_MASTER_BRANCH

    mAllClients.pushBack(client);
  }

  return client;
}

bool COPC_UA_Client_Handler::isActionInitialized(UA_ClientInformation * paClient, COPC_UA_HandlerAbstract::CActionInfo *paActionInfo) {
  CCriticalRegion clientRegion(paClient->mClientMutex);
  bool retVal = true;
  for(CSinglyLinkedList<COPC_UA_HandlerAbstract::CActionInfo*>::Iterator it = paClient->mActionsToBeInitialized.begin();
      it != paClient->mActionsToBeInitialized.end(); ++it) {
    if((*it) == paActionInfo) {
      retVal = false;
      break;
    }
  }
  return retVal;
}

void COPC_UA_Client_Handler::referencedClientsIncrement(COPC_UA_HandlerAbstract::CActionInfo *paActionInfo) {
  CCriticalRegion allClientsRegion(mAllClientListMutex);
  for(CSinglyLinkedList<UA_ClientInformation *>::Iterator iterRef = mAllClients.begin(); iterRef != mAllClients.end(); ++iterRef) {
    CCriticalRegion clientRegion((*iterRef)->mClientMutex);
    if((*iterRef)->mEndpointUrl == paActionInfo->mEndpoint) {
      (*iterRef)->mActionsReferencingIt.pushBack(paActionInfo);
      (*iterRef)->mActionsToBeInitialized.pushBack(paActionInfo);
      addClientToConnectionList((*iterRef));
      break;
    }
  }
}

void COPC_UA_Client_Handler::referencedClientsDecrement(COPC_UA_HandlerAbstract::CActionInfo *paActionInfo) {
  CCriticalRegion allClientsRegion(mAllClientListMutex);

  UA_ClientInformation * clientToDelete = 0;
  for(CSinglyLinkedList<UA_ClientInformation *>::Iterator iterRef = mAllClients.begin(); iterRef != mAllClients.end(); ++iterRef) {
    CCriticalRegion clientRegion((*iterRef)->mClientMutex);
    if((*iterRef)->mEndpointUrl == paActionInfo->mEndpoint) {
      (*iterRef)->mActionsToBeInitialized.erase(paActionInfo);
      (*iterRef)->mActionsReferencingIt.erase(paActionInfo);
      mConnectionHandler.removeAction(*iterRef, paActionInfo);
      clientToDelete = *iterRef;
      break;
    }
  }

  if(clientToDelete && clientToDelete->mActionsReferencingIt.isEmpty()) {
    cleanClient(clientToDelete);
  }
}

void COPC_UA_Client_Handler::cleanClient(UA_ClientInformation * paClientInformation) {
  mConnectionHandler.removeClient(paClientInformation);
  removeClientFromInitializedList(paClientInformation);

  for(CSinglyLinkedList<COPC_UA_HandlerAbstract::CActionInfo*>::Iterator itActions = paClientInformation->mActionsReferencingIt.begin();
      itActions != paClientInformation->mActionsReferencingIt.end(); ++itActions) {
    mConnectionHandler.removeAction(paClientInformation, *itActions); //will uninitialize everything
  }

  paClientInformation->mActionsReferencingIt.clearAll();

  UA_Client_disconnect(paClientInformation->mClient);
  UA_Client_delete(paClientInformation->mClient);
  mAllClients.erase(paClientInformation);
  delete paClientInformation;
}

UA_StatusCode COPC_UA_Client_Handler::initializeAction(COPC_UA_HandlerAbstract::CActionInfo& paInfo) {
  enableHandler();
  UA_StatusCode retVal = UA_STATUSCODE_BADINTERNALERROR;
  switch(paInfo.getAction()){
    case eRead:
    case eWrite:
    case eCallMethod:
    case eSubscribe:
      retVal = initialize(paInfo);
      break;
    case eCreateMethod:
      DEVLOG_ERROR("[OPC UA REMOTE]: Cannot create a method remotely. Initialization failed\n");
      break;
    case eCreateObject:
      DEVLOG_ERROR("[OPC UA REMOTE]: Cannot create an object remotely. Execution failed\n");
      break;
    case eDeleteObject:
      DEVLOG_ERROR("[OPC UA REMOTE]: Cannot delete an object remotely. Initialization failed\n");
      break;
    default:
      DEVLOG_ERROR("[OPC UA REMOTE]: Unknown action %d to be initialized", paInfo.getAction());
  }
  return retVal;
}

UA_StatusCode COPC_UA_Client_Handler::executeAction(COPC_UA_HandlerAbstract::CActionInfo& paInfo) {

  COPC_UA_Client_Handler::UA_ClientInformation* clientInfo = getClient(paInfo.mEndpoint);
  UA_StatusCode retVal = UA_STATUSCODE_BADINTERNALERROR;

  if(isActionInitialized(clientInfo, &paInfo)) {
    switch(paInfo.getAction()){
      case eRead:
        retVal = executeRead(paInfo, clientInfo);
        break;
      case eWrite:
        retVal = executeWrite(paInfo, clientInfo);
        break;
      case eCreateMethod:
        DEVLOG_ERROR("[OPC UA REMOTE]: Cannot create a method remotely. Execution failed\n");
        break;
      case eCallMethod:
        retVal = executeCallMethod(paInfo, clientInfo);
        break;
      case eSubscribe:
        DEVLOG_ERROR("[OPC UA REMOTE]: You shouldn't be here. Cannot execute a subscription. They are triggered automatically when the value changes\n");
        break;
      case eCreateObject:
        DEVLOG_ERROR("[OPC UA REMOTE]: Cannot create an object remotely. Execution failed\n");
        break;
      case eDeleteObject:
        DEVLOG_ERROR("[OPC UA REMOTE]: Cannot delete an object remotely. Execution failed\n");
        break;
      default:
        DEVLOG_ERROR("[OPC UA REMOTE]: Unknown action %d to be executed", paInfo.getAction());
    }
  } else {
    DEVLOG_ERROR("[OPC UA REMOTE]: Cannot execute action from FB %s. It was not properly initialized\n", paInfo.mLayer->getCommFB()->getInstanceName());
  }


  return retVal;
}

UA_StatusCode COPC_UA_Client_Handler::uninitializeAction(COPC_UA_HandlerAbstract::CActionInfo& paInfo) {
  UA_StatusCode retVal = UA_STATUSCODE_BADINTERNALERROR;
  switch(paInfo.getAction()){
    case eRead:
    case eWrite:
    case eCallMethod:
    case eSubscribe:
      referencedClientsDecrement(&paInfo);
      retVal = UA_STATUSCODE_GOOD;
      break;
    case eCreateMethod:
      DEVLOG_ERROR("[OPC UA REMOTE]: Cannot create a method remotely. Un-nitialization failed\n");
      break;
    case eCreateObject:
      DEVLOG_ERROR("[OPC UA REMOTE]: Cannot create an object remotely. Un-initialization failed\n");
      break;
    case eDeleteObject:
      DEVLOG_ERROR("[OPC UA REMOTE]: Cannot delete an object remotely. Un-initialization failed\n");
      break;
    default:
      DEVLOG_ERROR("[OPC UA REMOTE]: Unknown action %d to be uninitialized", paInfo.getAction());
  }
  return retVal;
}

UA_StatusCode COPC_UA_Client_Handler::initialize(COPC_UA_HandlerAbstract::CActionInfo& paInfo) {
  UA_StatusCode retVal = UA_STATUSCODE_BADINTERNALERROR;
  COPC_UA_Client_Handler::UA_ClientInformation* clientInfo = getClient(paInfo.mEndpoint);
  if(clientInfo) {
    referencedClientsIncrement(&paInfo);
    retVal = UA_STATUSCODE_GOOD;
  }
  return retVal;
}

UA_StatusCode COPC_UA_Client_Handler::executeRead(COPC_UA_HandlerAbstract::CActionInfo& paInfo, COPC_UA_Client_Handler::UA_ClientInformation* paClientInfo) {
  CCriticalRegion clientRegion(paClientInfo->mClientMutex);

  UA_ReadRequest request;
  UA_ReadRequest_init(&request);
  request.nodesToReadSize = paInfo.getNoOfNodePairs();

  UA_ReadValueId* ids = static_cast<UA_ReadValueId *>(UA_Array_new(request.nodesToReadSize, &UA_TYPES[UA_TYPES_READVALUEID]));
  request.nodesToRead = ids;

  size_t indexOfNodePair = 0;
  for(CSinglyLinkedList<CNodePairInfo*>::Iterator itMain = paInfo.getNodePairInfo().begin(); itMain != paInfo.getNodePairInfo().end();
      ++itMain, indexOfNodePair++) {
    UA_ReadValueId_init(&ids[indexOfNodePair]);
    ids[indexOfNodePair].attributeId = UA_ATTRIBUTEID_VALUE;
    UA_NodeId_copy((*itMain)->mNodeId, &ids[indexOfNodePair].nodeId);
  }

  UA_RemoteCallHandle* handle = new UA_RemoteCallHandle();
  handle->mAction = &paInfo;
  handle->mClient = paClientInfo;
  UA_StatusCode retVal =

#ifdef FORTE_COM_OPC_UA_MASTER_BRANCH
    UA_Client_sendAsyncReadRequest(paClientInfo->mClient, &request, handleReadReturn, handle, 0);
#else
  UA_Client_AsyncService_read(paClientInfo->mClient, &request, onAsyncCallReturn, handle, 0);
#endif
   

  if(UA_STATUSCODE_GOOD != retVal) {
    DEVLOG_ERROR("[OPC UA REMOTE]: Couldn't dispatch read action for FB  %s\n", paInfo.getLayer()->getCommFB()->getInstanceName());
    delete handle;
  } else {
    addAsyncCall(paClientInfo);
  }

  UA_ReadRequest_deleteMembers(&request);

  return retVal;
}

UA_StatusCode COPC_UA_Client_Handler::executeWrite(COPC_UA_HandlerAbstract::CActionInfo& paInfo, COPC_UA_Client_Handler::UA_ClientInformation* paClientInfo) {
  CCriticalRegion clientRegion(paClientInfo->mClientMutex);

  UA_StatusCode retVal = UA_STATUSCODE_BADINTERNALERROR;
  UA_WriteRequest request;
  UA_WriteRequest_init(&request);
  request.nodesToWriteSize = paInfo.getNoOfNodePairs();

  UA_WriteValue* ids = static_cast<UA_WriteValue *>(UA_Array_new(request.nodesToWriteSize, &UA_TYPES[UA_TYPES_WRITEVALUE]));
  request.nodesToWrite = ids;

  bool somethingFailed = false;
  size_t indexOfNodePair = 0;
  CSinglyLinkedList<UA_TypeConvert *>::Iterator typeIterator = paInfo.getLayer()->getTypeConveverters().begin();
  for(CSinglyLinkedList<CNodePairInfo*>::Iterator itMain = paInfo.getNodePairInfo().begin(); itMain != paInfo.getNodePairInfo().end();
      ++itMain, ++typeIterator, indexOfNodePair++) {
    UA_WriteValue_init(&ids[indexOfNodePair]);
    ids[indexOfNodePair].attributeId = UA_ATTRIBUTEID_VALUE;
    UA_NodeId_copy((*itMain)->mNodeId, &ids[indexOfNodePair].nodeId);

    UA_Variant_init(&ids[indexOfNodePair].value.value);
    ids[indexOfNodePair].value.hasValue = true;
    ids[indexOfNodePair].value.value.type = (*typeIterator)->type;

    void *varValue = UA_new((*typeIterator)->type);
    UA_init(varValue, (*typeIterator)->type);
    if(!(*typeIterator)->get(&paInfo.getLayer()->getCommFB()->getSDs()[indexOfNodePair], varValue)) {
      DEVLOG_ERROR("[OPC UA REMOTE]: The index %d of the FB %s could not be converted\n", indexOfNodePair, paInfo.getLayer()->getCommFB()->getInstanceName());
      somethingFailed = true;
      break;
    } else {
      UA_Variant_setScalarCopy(&ids[indexOfNodePair].value.value, varValue, (*typeIterator)->type);
      ids[indexOfNodePair].value.value.storageType = UA_VARIANT_DATA;
    }
    UA_delete(varValue, (*typeIterator)->type);
  }

  if(!somethingFailed) {
    UA_RemoteCallHandle* handle = new UA_RemoteCallHandle();
    handle->mAction = &paInfo;
    handle->mClient = paClientInfo;
    retVal =
#ifdef FORTE_COM_OPC_UA_MASTER_BRANCH
      UA_Client_sendAsyncWriteRequest(paClientInfo->mClient, &request, handleWriteReturn, handle, 0);
#else
    UA_Client_AsyncService_write(paClientInfo->mClient, &request, onAsyncCallReturn, handle, 0);
#endif


    if(UA_STATUSCODE_GOOD != retVal) {
      DEVLOG_ERROR("[OPC UA REMOTE]: Couldn't dispatch write action for FB  %s\n", paInfo.getLayer()->getCommFB()->getInstanceName());
      delete handle;
    } else {
      addAsyncCall(paClientInfo);
    }
  }

  UA_WriteRequest_deleteMembers(&request);

  return retVal;
}

UA_StatusCode COPC_UA_Client_Handler::executeCallMethod(COPC_UA_HandlerAbstract::CActionInfo& paInfo,
    COPC_UA_Client_Handler::UA_ClientInformation* paClientInfo) {
  CCriticalRegion clientRegion(paClientInfo->mClientMutex);

  UA_StatusCode retVal = UA_STATUSCODE_BADINTERNALERROR;
  UA_CallRequest request;
  UA_CallRequest_init(&request);
  request.methodsToCallSize = 1;
  request.methodsToCall = static_cast<UA_CallMethodRequest *>(UA_Array_new(request.methodsToCallSize, &UA_TYPES[UA_TYPES_CALLMETHODREQUEST]));

  UA_CallMethodRequest* methodRequest = &request.methodsToCall[0];

  methodRequest->inputArgumentsSize = paInfo.getLayer()->getCommFB()->getNumSD();
  methodRequest->inputArguments = static_cast<UA_Variant *>(UA_Array_new(methodRequest->inputArgumentsSize, &UA_TYPES[UA_TYPES_VARIANT]));

  bool somethingFailed = false;

  CSinglyLinkedList<UA_TypeConvert *>::Iterator typeIterator = paInfo.getLayer()->getTypeConveverters().begin();
  CSinglyLinkedList<CNodePairInfo*>::Iterator itMain = paInfo.getNodePairInfo().begin();
  UA_NodeId_copy((*itMain)->mNodeId, &methodRequest->methodId);
  ++itMain;
  UA_NodeId_copy((*itMain)->mNodeId, &methodRequest->objectId);

  for(size_t i = 0; i < methodRequest->inputArgumentsSize; ++typeIterator, i++) {
    UA_Variant_init(&methodRequest->inputArguments[i]);
    methodRequest->inputArguments[i].type = (*typeIterator)->type;

    void *varValue = UA_new((*typeIterator)->type);
    UA_init(varValue, (*typeIterator)->type);
    if(!(*typeIterator)->get(&paInfo.getLayer()->getCommFB()->getSDs()[i], varValue)) {
      DEVLOG_ERROR("[OPC UA REMOTE]: The index %d of the FB %s could not be converted\n", i, paInfo.getLayer()->getCommFB()->getInstanceName());
      somethingFailed = true;
      break;
    } else {
      UA_Variant_setScalarCopy(&methodRequest->inputArguments[i], varValue, (*typeIterator)->type);
      methodRequest->inputArguments[i].storageType = UA_VARIANT_DATA;
    }
    UA_delete(varValue, (*typeIterator)->type);
  }

  if(!somethingFailed) {
    UA_RemoteCallHandle* handle = new UA_RemoteCallHandle();
    handle->mAction = &paInfo;
    handle->mClient = paClientInfo;
    retVal =

#ifdef FORTE_COM_OPC_UA_MASTER_BRANCH
      UA_Client_sendAsyncRequest(paClientInfo->mClient, &request, &UA_TYPES[UA_TYPES_CALLREQUEST],
        handleCallReturn, &UA_TYPES[UA_TYPES_CALLRESPONSE], handle,
        0);
#else
    UA_Client_AsyncService_call(paClientInfo->mClient, &request, onAsyncCallReturn, handle, 0);
#endif


    if(UA_STATUSCODE_GOOD != retVal) {
      DEVLOG_ERROR("[OPC UA REMOTE]: Couldn't dispatch call action for FB  %s\n", paInfo.getLayer()->getCommFB()->getInstanceName());
      delete handle;
    } else {
      addAsyncCall(paClientInfo);
    }
  }

  UA_CallRequest_deleteMembers(&request);

  return retVal;
}

/* CONECTION HANDLER */

COPC_UA_Client_Handler::UA_ConnectionHandler::UA_ConnectionHandler(COPC_UA_Client_Handler& paClientHandler) :
    mClientHandler(paClientHandler), mNewClientsPresent(false) {
}

COPC_UA_Client_Handler::UA_ConnectionHandler::~UA_ConnectionHandler() {
  //do nothing
}


void COPC_UA_Client_Handler::UA_ConnectionHandler::startConnectionsThread() {
  if(!isAlive()) {
    start();
    mThreadStarted.waitIndefinitely();
  }
}

void COPC_UA_Client_Handler::UA_ConnectionHandler::stopConnectionsThread() {
  setAlive(false);
  mClientPresent.inc();
  end();
}

void COPC_UA_Client_Handler::UA_ConnectionHandler::addClient(UA_ClientInformation* paClientInformation) {
  CCriticalRegion connectionListRegion(mNewClientstMutex);
  addClientToList(paClientInformation, mNewClients);
  mClientPresent.inc();
  mNewClientsPresent = true;
}

void COPC_UA_Client_Handler::UA_ConnectionHandler::removeClient(UA_ClientInformation* paClientInformation) {
  CCriticalRegion connectionListRegion(mClientstMutex);
  CCriticalRegion newClientsRegion(mNewClientstMutex);
  mNewClients.erase(paClientInformation); //client could still be in the newList
  mClients.erase(paClientInformation);
}

void COPC_UA_Client_Handler::UA_ConnectionHandler::removeAction(UA_ClientInformation* paClientInformation, COPC_UA_HandlerAbstract::CActionInfo* paAction) {
  uninitializeAction(paAction, paClientInformation);
}

void COPC_UA_Client_Handler::UA_ConnectionHandler::updateClientList() {
  CCriticalRegion clientsClientsRegion(mClientstMutex);
  CCriticalRegion connectionListRegion(mNewClientstMutex);
  for(CSinglyLinkedList<UA_ClientInformation*>::Iterator it = mNewClients.begin(); it != mNewClients.end(); ++it) {
    addClientToList(*it, mClients);
  }
  mNewClients.clearAll();
  mNewClientsPresent = false;
}

void COPC_UA_Client_Handler::UA_ConnectionHandler::run() {
  DEVLOG_INFO("[OPC UA REMOTE]: Starting OPC UA Connection handler\n");
  mThreadStarted.inc();
  while(isAlive()) {

    if(mNewClientsPresent) {
      updateClientList();
    }
    bool needsRetry = handleClients();
    if(isAlive()) {
      if(needsRetry) {
        mClientPresent.timedWait(scmNanosecondsToSleep);
      } else {
        mClientPresent.waitIndefinitely();
      }
    }
  }
  DEVLOG_INFO("[OPC UA REMOTE]: Stopping OPC UA Connection handler\n");
}

bool COPC_UA_Client_Handler::UA_ConnectionHandler::handleClients() {
  CCriticalRegion clientsClientsRegion(mClientstMutex);
  CSinglyLinkedList<UA_ClientInformation*> clientsToRemove;
  bool needsRetry = false;
  for(CSinglyLinkedList<UA_ClientInformation *>::Iterator iter = mClients.begin(); iter != mClients.end(); ++iter) {
    CCriticalRegion clientRegion((*iter)->mClientMutex);
    if(handleClientState(*iter)) {
      clientsToRemove.pushBack(*iter);
    } else {
      needsRetry = true;
    }
    if(!isAlive()) {
      break;
    }
  }

  if(isAlive() && !clientsToRemove.isEmpty()) {
    for(CSinglyLinkedList<UA_ClientInformation*>::Iterator it = clientsToRemove.begin(); it != clientsToRemove.end(); ++it) {
      mClients.erase(*it);
    }
  }
  return needsRetry;
}

bool COPC_UA_Client_Handler::UA_ConnectionHandler::handleClientState(UA_ClientInformation* paClientInformation) {

  bool noMoreChangesNeeded = false;
  bool tryAnotherChangeImmediately = true;

  if(paClientInformation->mNeedsReconnection) {
    uint_fast64_t now = CTimerHandler::smFORTETimer->getForteTime();
    if((now - paClientInformation->mLastTry) < scmConnectionRetryTimeoutMilli) { //if connection timeout didn't happen, return that more changes are still needed
      tryAnotherChangeImmediately = false;
    }
  }

  while(tryAnotherChangeImmediately && isAlive()) {
    UA_ClientState currentState = UA_Client_getState(paClientInformation->mClient);
    if(UA_CLIENTSTATE_SESSION == currentState) {
      if(initializeClient(paClientInformation)) {
        noMoreChangesNeeded = true;
      }
      tryAnotherChangeImmediately = false;
    } else if(UA_CLIENTSTATE_SESSION_RENEWED == currentState) {
      DEVLOG_ERROR("[OPC UA REMOTE]: Client state is session renewed. Check what happens with the subscription here\n");
    } else {
      if(!connectClient(paClientInformation)) {
        tryAnotherChangeImmediately = false;
        DEVLOG_ERROR("[OPC UA REMOTE]: Couldn't connect to endpoint %s. Forte will try to reconnect in %d seconds\n",
          paClientInformation->mEndpointUrl.getValue(), scmConnectionRetryTimeoutMilli / 1000);
        paClientInformation->mNeedsReconnection = true;
        paClientInformation->mLastTry = CTimerHandler::smFORTETimer->getForteTime();
      } else { //if connection succeeded, don't break the while and try to handle subscriptions immediately
        paClientInformation->mNeedsReconnection = false;
        DEVLOG_INFO("[OPC UA REMOTE]: Client connected to endpoint %s\n", paClientInformation->mEndpointUrl.getValue());
      }
    }
  }

  return noMoreChangesNeeded;
}

bool COPC_UA_Client_Handler::UA_ConnectionHandler::connectClient(COPC_UA_Client_Handler::UA_ClientInformation* paClient) {
  return (UA_STATUSCODE_GOOD == UA_Client_connect(paClient->mClient, paClient->mEndpointUrl.getValue()));
}

bool COPC_UA_Client_Handler::UA_ConnectionHandler::initializeClient(COPC_UA_Client_Handler::UA_ClientInformation* paClient) {

  bool somethingFailed = false;

  CSinglyLinkedList<COPC_UA_HandlerAbstract::CActionInfo*> initializedActions;
  for(CSinglyLinkedList<COPC_UA_HandlerAbstract::CActionInfo*>::Iterator itActions = paClient->mActionsToBeInitialized.begin();
      itActions != paClient->mActionsToBeInitialized.end(); ++itActions) {

    if(!initializeAction((*itActions), paClient)) {
      initializedActions.pushBack(*itActions);
    } else {
      somethingFailed = true;
    }
  }

  if(!initializedActions.isEmpty()) { //if one action (FB) related to the client was initialized, copy it to the main thread
    mClientHandler.addClientToInitializedList(paClient);
    for(CSinglyLinkedList<COPC_UA_HandlerAbstract::CActionInfo*>::Iterator itActions = initializedActions.begin(); itActions != initializedActions.end();
        ++itActions) {
      paClient->mActionsToBeInitialized.erase(*itActions);
    }
  }

  return !somethingFailed;
}

bool COPC_UA_Client_Handler::UA_ConnectionHandler::initializeAction(COPC_UA_HandlerAbstract::CActionInfo* paAction,
    COPC_UA_Client_Handler::UA_ClientInformation* paClient) {
  bool somethingFailed = false;
  if(eCallMethod == paAction->getAction()) {
    if(!initializeCallMethod(paAction, paClient)) {
      somethingFailed = true;
    }
  } else {
    size_t runnerHelper = 0;
    for(CSinglyLinkedList<CNodePairInfo*>::Iterator it = paAction->getNodePairInfo().begin(); it != paAction->getNodePairInfo().end();
        ++it, runnerHelper++) {

      if(!somethingFailed) {
        if("" != (*it)->mBrowsePath) { //if browsepath was given, look for NodeId, even if NodeID was also provided
          UA_NodeId* nodeId;
          UA_StatusCode retVal = COPC_UA_Helper::getRemoteNodeForPath(paClient->mClient, &nodeId, (*it)->mBrowsePath.getValue(), 0, 0);//we don't care about the parent nor the starting Node

          if(UA_STATUSCODE_GOOD != retVal) {
            DEVLOG_ERROR("[OPC UA REMOTE]: The index %u of the FB %s could not be initialized because the requested nodeId was not found. Error: %s\n",
              runnerHelper, paAction->getLayer()->getCommFB()->getInstanceName(), UA_StatusCode_name(retVal));
            somethingFailed = true;
          } else {
            if((*it)->mNodeId) {
              if(!UA_NodeId_equal((*it)->mNodeId, nodeId)) { //if NodeId was provided, check if found is the same
                DEVLOG_ERROR("[OPC UA REMOTE]: The call from FB %s failed the found nodeId of the method doesn't match the provided one\n",
                  paAction->getLayer()->getCommFB()->getInstanceName());
                somethingFailed = true;
              }
              UA_NodeId_delete(nodeId);
            } else {
              (*it)->mNodeId = nodeId;
            }
          }
        }
      }
    }

    //for subscription, more things are needed
    if(!somethingFailed && !initializeSubscription(paAction, paClient)) { //won't initialize subscription if some nodeID is missing
      somethingFailed = true;
    }
  }
  return somethingFailed;
}

void COPC_UA_Client_Handler::UA_ConnectionHandler::uninitializeAction(COPC_UA_HandlerAbstract::CActionInfo* paAction,
    COPC_UA_Client_Handler::UA_ClientInformation* paClient) {
  paClient->mActionsToBeInitialized.erase(paAction); //remove in case it is still not initialized
  if(eSubscribe == paAction->getAction()) { //only subscription has something to release
    uninitializeSubscription(paAction, paClient);
  }
}

void COPC_UA_Client_Handler::UA_ConnectionHandler::uninitializeSubscription(COPC_UA_HandlerAbstract::CActionInfo* paAction,
    COPC_UA_Client_Handler::UA_ClientInformation* paClient) {
  if(paClient->mSubscriptionInfo) {
    CSinglyLinkedList<UA_MonitoringItemInfo*> toDelete;
    for(CSinglyLinkedList<UA_MonitoringItemInfo*>::Iterator it = paClient->mSubscriptionInfo->mMonitoredItems.begin();
        it != paClient->mSubscriptionInfo->mMonitoredItems.end(); ++it) {
      if((*it)->mAction == paAction) {
        toDelete.pushBack(*it);
      }
    }
    for(CSinglyLinkedList<UA_MonitoringItemInfo*>::Iterator it = toDelete.begin(); it != toDelete.end(); ++it) {
      UA_MonitoringItemInfo* monitoredInfo = *it;
      if(UA_STATUSCODE_GOOD
        != UA_Client_MonitoredItems_deleteSingle(paClient->mClient, paClient->mSubscriptionInfo->mSubscriptionId, monitoredInfo->mMonitoringItemId)) {
        DEVLOG_ERROR("[OPC UA REMOTE]: Couldn't delete monitored item %u. No further actions will be taken\n", (*it)->mMonitoringItemId);
      }

      paClient->mSubscriptionInfo->mMonitoredItems.erase(monitoredInfo);
      delete *it;
    }
    if(paClient->mSubscriptionInfo->mMonitoredItems.isEmpty()) {
      UA_StatusCode retval = UA_Client_Subscriptions_deleteSingle(paClient->mClient, paClient->mSubscriptionInfo->mSubscriptionId);

      if(UA_STATUSCODE_GOOD != retval) {
        DEVLOG_ERROR("[OPC UA REMOTE]: Couldn't delete subscription %u. Failed with error %s. No further actions will be taken\n",
          paClient->mSubscriptionInfo->mSubscriptionId, UA_StatusCode_name(retval));
      }
      delete paClient->mSubscriptionInfo;
      paClient->mSubscriptionInfo = 0;
    }
  }
}

bool COPC_UA_Client_Handler::UA_ConnectionHandler::initializeCallMethod(COPC_UA_HandlerAbstract::CActionInfo* paAction,
    COPC_UA_Client_Handler::UA_ClientInformation* paClient) {
  bool somethingFailed = false;

  CSinglyLinkedList<CNodePairInfo*>::Iterator nodePair = paAction->getNodePairInfo().begin();
  //get parentNodeId and also the method NodeId
  UA_NodeId* methodNode;
  UA_NodeId* parentNode;

  UA_StatusCode retVal = COPC_UA_Helper::getRemoteNodeForPath(paClient->mClient, &methodNode, (*nodePair)->mBrowsePath.getValue(), 0, &parentNode); //we don't care about the parent nor the starting Node

  if(UA_STATUSCODE_GOOD != retVal) {
    DEVLOG_ERROR("[OPC UA REMOTE]: The method call from FB %s failed because the requested node was not found. Error: %s\n",
      paAction->getLayer()->getCommFB()->getInstanceName(), UA_StatusCode_name(retVal));
    somethingFailed = true;
  } else {
    if((*nodePair)->mNodeId) {
      if(!UA_NodeId_equal((*nodePair)->mNodeId, methodNode)) { //if NodeId of method was provided, check if found is the same
        DEVLOG_ERROR("[OPC UA REMOTE]: The method call from FB %s failed the found nodeId of the method doesn't match the provided one\n",
          paAction->getLayer()->getCommFB()->getInstanceName());
        somethingFailed = true;
      }
      UA_NodeId_delete(methodNode);
    } else {
      (*nodePair)->mNodeId = methodNode;
    }
    if(!somethingFailed) {
      //store the parentNodeId in the second position. BrowseName is not needed
      paAction->getNodePairInfo().pushBack(new CNodePairInfo(parentNode, ""));
    } else {
      UA_NodeId_delete(parentNode);
    }
  }

  return !somethingFailed;
}

bool COPC_UA_Client_Handler::UA_ConnectionHandler::initializeSubscription(COPC_UA_HandlerAbstract::CActionInfo* paAction,
    COPC_UA_Client_Handler::UA_ClientInformation* paClient) {
  bool somethingFailed = false;
  if(eSubscribe == paAction->getAction()) {

    if(!paClient->mSubscriptionInfo) {
      paClient->mSubscriptionInfo = new UA_subscriptionInfo();
    }

    if(!paClient->mSubscriptionInfo->mSubscriptionAlreadyCreated && !createSubscription(paClient)) {
      somethingFailed = true;
    }

    if(!somethingFailed) {
      size_t runnerHelper = 0;
      CSinglyLinkedList<UA_MonitoringItemInfo*> newMonitoredItems;

      CSinglyLinkedList<UA_TypeConvert *>::Iterator typeIterator = paAction->getLayer()->getTypeConveverters().begin();
      for(CSinglyLinkedList<CNodePairInfo*>::Iterator it = paAction->getNodePairInfo().begin(); it != paAction->getNodePairInfo().end();
          ++it, ++typeIterator, runnerHelper++) {

        UA_MonitoringItemInfo *handle = new UA_MonitoringItemInfo();

        /* Add a MonitoredItem */
        handle->mVariableInfo.mConvert = *typeIterator;
        handle->mVariableInfo.mPortIndex = runnerHelper;
        handle->mVariableInfo.mLayer = paAction->getLayer();
        handle->mAction = paAction; //don't copy it fully, just the pointer, so no deallocation is needed here

        newMonitoredItems.pushBack(handle);
      }

      CSinglyLinkedList<UA_MonitoringItemInfo*> addedMonitoredItems;
      CSinglyLinkedList<CNodePairInfo*>::Iterator nodeId = paAction->getNodePairInfo().begin();
      for(CSinglyLinkedList<UA_MonitoringItemInfo*>::Iterator itMonitoringItem = newMonitoredItems.begin(); itMonitoringItem != newMonitoredItems.end();
          ++itMonitoringItem, ++nodeId) {

        if(addMonitoringItem(paClient, *itMonitoringItem, (*nodeId)->mNodeId)) {
          addedMonitoredItems.pushBack(*itMonitoringItem);
        } else {
          somethingFailed = true;
          break;
        }
      }

      if(somethingFailed) { //if something failed, remove added monitoring items and fail the whole action
        for(CSinglyLinkedList<UA_MonitoringItemInfo*>::Iterator it = addedMonitoredItems.begin(); it != addedMonitoredItems.end(); ++it) {
          UA_StatusCode retVal = UA_Client_MonitoredItems_deleteSingle(paClient->mClient, paClient->mSubscriptionInfo->mSubscriptionId,
            (*it)->mMonitoringItemId);
          if(UA_STATUSCODE_GOOD != retVal) {
            DEVLOG_ERROR("[OPC UA REMOTE]: Couldn't delete recently added monitored item %u\n", (*it)->mMonitoringItemId);
          }
        }
        for(CSinglyLinkedList<UA_MonitoringItemInfo*>::Iterator it = newMonitoredItems.begin(); it != newMonitoredItems.end(); ++it) {
          delete (*it);
        }
      } else {
        for(CSinglyLinkedList<UA_MonitoringItemInfo*>::Iterator it = newMonitoredItems.begin(); it != newMonitoredItems.end(); ++it) {
          paClient->mSubscriptionInfo->mMonitoredItems.pushBack(*it);
        }
        mClientHandler.addAsyncCall(paClient);
      }
    }
  }
  return !somethingFailed;
}

bool COPC_UA_Client_Handler::UA_ConnectionHandler::createSubscription(COPC_UA_Client_Handler::UA_ClientInformation* paClient) {
  UA_CreateSubscriptionRequest request = UA_CreateSubscriptionRequest_default();
  UA_CreateSubscriptionResponse response = UA_Client_Subscriptions_create(paClient->mClient, request, paClient, 0, deleteSubscriptionCallback);
  if(UA_STATUSCODE_GOOD == response.responseHeader.serviceResult) {
    DEVLOG_INFO("[OPC UA REMOTE]: Create subscription to %s succeeded, id %u\n", paClient->mEndpointUrl.getValue(), response.subscriptionId);
    paClient->mSubscriptionInfo->mSubscriptionId = response.subscriptionId;
    paClient->mSubscriptionInfo->mSubscriptionAlreadyCreated = true;
  } else {
    DEVLOG_ERROR("[OPC UA REMOTE]: Create subscription to %s failed, %s\n", paClient->mEndpointUrl.getValue(),
      UA_StatusCode_name(response.responseHeader.serviceResult));
  }

  return paClient->mSubscriptionInfo->mSubscriptionAlreadyCreated;
}

bool COPC_UA_Client_Handler::UA_ConnectionHandler::addMonitoringItem(COPC_UA_Client_Handler::UA_ClientInformation* paClient,
    UA_MonitoringItemInfo* paMonitoringInfo, UA_NodeId* paNodeId) {

  UA_MonitoredItemCreateRequest monRequest = UA_MonitoredItemCreateRequest_default(*paNodeId);
  UA_MonitoredItemCreateResult monResponse = UA_Client_MonitoredItems_createDataChange(paClient->mClient, paClient->mSubscriptionInfo->mSubscriptionId,
    UA_TIMESTAMPSTORETURN_BOTH, monRequest, static_cast<void *>(&paMonitoringInfo->mVariableInfo), onSubscriptionValueChanged, 0);
  if(UA_STATUSCODE_GOOD == monResponse.statusCode) {
    DEVLOG_INFO("[OPC UA REMOTE]: Monitoring of FB %s at index %u succeeded. The monitoring item id is %u\n",
      paMonitoringInfo->mVariableInfo.mLayer->getCommFB()->getInstanceName(), paMonitoringInfo->mVariableInfo.mPortIndex, monResponse.monitoredItemId);
    paMonitoringInfo->mMonitoringItemId = monResponse.monitoredItemId;
  } else {
    DEVLOG_ERROR("[OPC UA REMOTE]: Monitoring of FB %s at index %u failed with error: %s\n",
      paMonitoringInfo->mVariableInfo.mLayer->getCommFB()->getInstanceName(), paMonitoringInfo->mVariableInfo.mPortIndex,
      UA_StatusCode_name(monResponse.statusCode));
  }

  return (UA_STATUSCODE_GOOD == monResponse.statusCode);
}

