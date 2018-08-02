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

#ifndef FORTE_COM_OPC_UA_CUSTOM_HOSTNAME
#include <sockhand.h>
#endif

using namespace forte::com_infra;

DEFINE_HANDLER(COPC_UA_Client_Handler);

COPC_UA_Client_Handler::COPC_UA_Client_Handler(
    CDeviceExecution& pa_poDeviceExecution) :
    CExternalEventHandler(pa_poDeviceExecution), uaClientRunningFlag(UA_FALSE), subscriptionCallbackHandles(), clients() {

}

COPC_UA_Client_Handler::UA_ClientEndpointMap* COPC_UA_Client_Handler::getHandleForClient(
    const UA_Client *client) {
  CCriticalRegion criticalRegion(mClientMapMutex);
  for (CSinglyLinkedList<struct UA_ClientEndpointMap *>::Iterator iter =
      clients.begin(); iter != clients.end(); ++iter) {
    if ((*iter)->client == client)
      return (*iter);
  }
  return NULL;
}

void COPC_UA_Client_Handler::setPriority(int) {

  //currently we are doing nothing here.
}

int COPC_UA_Client_Handler::getPriority(void) const {
  //the same as for setPriority
  return 0;
}

COPC_UA_Client_Handler::~COPC_UA_Client_Handler() {
  stopClient();
  {
  CCriticalRegion criticalRegion(mClientMapMutex);
  for (CSinglyLinkedList<struct UA_ClientEndpointMap *>::Iterator iter =
      clients.begin(); iter != clients.end(); ++iter) {
    UA_Client_disconnect((*iter)->client);
    UA_Client_delete((*iter)->client);
    delete ((*iter)->clientMutex);
    delete ((*iter)->endpointUrl);
    delete((*iter));
  }

  clients.clearAll();

  for (CSinglyLinkedList<struct UA_SubscriptionCallback_Handle *>::Iterator iter =
      subscriptionCallbackHandles.begin();
      iter != subscriptionCallbackHandles.end(); ++iter)
    delete(*iter);
  subscriptionCallbackHandles.clearAll();
  }
  //end();
}

void COPC_UA_Client_Handler::run() {
  DEVLOG_INFO("OPC UA Client: Starting OPC UA Client handler:\n");
  mServerStarted.inc();
  while (uaClientRunningFlag) {
    {
      CCriticalRegion criticalRegion(mClientMapMutex);
      for (CSinglyLinkedList<struct UA_ClientEndpointMap *>::Iterator iter =
          clients.begin(); iter != clients.end(); ++iter) {
        if(!uaClientRunningFlag){
          break;
        }
        if ((*iter)->subscription) {
          CCriticalRegion criticalRegion(*(*iter)->clientMutex);
          /* if already connected, this will return GOOD and do nothing */
          /* if the connection is closed/errored, the connection will be reset and then reconnected */
          /* Alternatively you can also use UA_Client_getState to get the current state */
          UA_StatusCode retval = UA_Client_connect((*iter)->client, (*iter)->endpointUrl->getValue());
          if(retval != UA_STATUSCODE_GOOD){
            DEVLOG_ERROR("Not connected to %s. Error %s Retrying to connect again soon\n", (*iter)->endpointUrl->getValue(), UA_StatusCode_name(retval));
            /* The connect may timeout after 1 second (see above) or it may fail immediately on network errors */
            /* E.g. name resolution errors or unreachable network. Thus there should be a small sleep here */
            //CThread::sleepThread(1000);
            //continue;
          }else{
            UA_Client_run_iterate((*iter)->client, 10);
          }
        }
        if(!uaClientRunningFlag){
          break;
        }
      }
    }
    CThread::sleepThread(100);
  };
  DEVLOG_INFO("OPC UA Client: Stopping OPC UA Client handler:\n");
}

void COPC_UA_Client_Handler::enableHandler(void) {
  startClient();
}

void COPC_UA_Client_Handler::disableHandler(void) {
  COPC_UA_Client_Handler::stopClient();
  end();
}

void COPC_UA_Client_Handler::startClient() {
  if (uaClientRunningFlag)
    return;
  uaClientRunningFlag = UA_TRUE;

  if (!isAlive()) {
    start();
  }
}

void COPC_UA_Client_Handler::stopClient() {
  uaClientRunningFlag = UA_FALSE;
}

UA_Client *COPC_UA_Client_Handler::getClientForEndpoint(const char *endpointUrl,
    bool createIfNotFound, bool subscription, CSyncObject **clientMutex) {
  CCriticalRegion criticalRegion(mClientMapMutex);
  if (clientMutex == NULL)
    return NULL;

  size_t urlLen = strlen(endpointUrl);
  while (urlLen && endpointUrl[urlLen - 1] == '/') {
    urlLen--;
  }

  for (CSinglyLinkedList<struct UA_ClientEndpointMap *>::Iterator iter =
      clients.begin(); iter != clients.end(); ++iter) {
    if (strncmp((*iter)->endpointUrl->getValue(), endpointUrl, urlLen) == 0) {
      if (clientMutex)
        *clientMutex = (*iter)->clientMutex;
      return (*iter)->client;
    }
  }

  if (!createIfNotFound)
    return NULL;

  struct UA_ClientEndpointMap *clientMap =
      static_cast<UA_ClientEndpointMap *>(forte_malloc(
          sizeof(struct UA_ClientEndpointMap)));

  UA_ClientConfig config = UA_ClientConfig_default;
  config.timeout = 8000;

  clientMap->client = UA_Client_new(config);
  clientMap->endpointUrl = new CIEC_STRING(endpointUrl);
  clientMap->clientMutex = new CSyncObject();

  *clientMutex = clientMap->clientMutex;

  // store it in the list so we can delete it to avoid mem leaks
  clients.pushBack(clientMap);
  startClient();
  clientMap->subscription = subscription;
  if (subscription) {
    config.stateCallback = stateCallback;
  }

  return clientMap->client;

}

void COPC_UA_Client_Handler::onSubscriptionValueChanged(UA_Client *client,
    UA_UInt32 subscriptionId, void *subContext, UA_UInt32 monId,
    void *monContext, UA_DataValue *data) {
  struct UA_SubscriptionCallback_Handle *handle =
      static_cast<struct UA_SubscriptionCallback_Handle *>(monContext);
  EComResponse retVal;
  forte::com_infra::CComLayer *layer = handle->layer;

  struct {
    const struct UA_TypeConvert *convert;
    unsigned int portIndex;
    const UA_Variant *data;
  } handleRecv;

  if(data->hasValue){
    handleRecv.data = data->hasValue ? &data->value : NULL;
    handleRecv.portIndex = handle->portIndex;
    handleRecv.convert = handle->convert;

    retVal = layer->recvData(static_cast<const void *>(&handleRecv), 0);

  }

}

void COPC_UA_Client_Handler::deleteSubscriptionCallback(UA_Client *client,
    UA_UInt32 subscriptionId, void *subscriptionContext) {
  DEVLOG_INFO("Subscription Id %u was deleted\n", subscriptionId);
}

void COPC_UA_Client_Handler::addMonitoringItem(UA_Client* client,
    forte::com_infra::CComLayer *comLayer, UA_NodeId nodeId,
    const struct UA_TypeConvert *convert, unsigned int portIndex) {
  struct UA_SubscriptionCallback_Handle *handle =
      static_cast<UA_SubscriptionCallback_Handle *>(forte_malloc(
          sizeof(struct UA_SubscriptionCallback_Handle)));
  UA_UInt32 subId;
  startClient();
  UA_ClientEndpointMap* clientEndpoint = getHandleForClient(client);
  if (clientEndpoint && clientEndpoint->clientMutex) {
    bool createNewSub = true;
    /* Add a MonitoredItem */
    handle->convert = convert;
    handle->portIndex = portIndex;
    handle->layer = comLayer;

    // store it in the list so we can delete it to avoid mem leaks
    subscriptionCallbackHandles.pushBack(handle);

    if (clientEndpoint->subscription) {
      createNewSub = false;
      subId = clientEndpoint->subscriptionId;
    }

    unsigned int tries = 0;
    while(createNewSub && tries++ < 3){
      /* A new session was created. We need to create the subscription. */
      /* Create a subscription */
      {
        CCriticalRegion criticalRegion(*clientEndpoint->clientMutex);
        UA_StatusCode retval = UA_Client_connect(clientEndpoint->client, clientEndpoint->endpointUrl->getValue());
        if(UA_STATUSCODE_GOOD == retval){
          UA_CreateSubscriptionRequest request = UA_CreateSubscriptionRequest_default();
          UA_CreateSubscriptionResponse response = UA_Client_Subscriptions_create(client, request,
          NULL, NULL, deleteSubscriptionCallback);
          if(response.responseHeader.serviceResult == UA_STATUSCODE_GOOD){
            DEVLOG_INFO("Create subscription to %s succeeded, id %u\n", clientEndpoint->endpointUrl->getValue(), response.subscriptionId);
            clientEndpoint->subscriptionId = response.subscriptionId;
            subId = response.subscriptionId;
            clientEndpoint->subscription = true;
            break;
          }
          else{
            DEVLOG_INFO("Create subscription to %s failed, %s\n", clientEndpoint->endpointUrl->getValue(), UA_StatusCode_name(response.responseHeader.serviceResult));
          }
        }else{
          DEVLOG_ERROR("Connecting to %s failed with error %s\n", clientEndpoint->endpointUrl->getValue(), UA_StatusCode_name(retval));
        }
      }
      CThread::sleepThread(300);
    }

    {
      CCriticalRegion criticalRegion(*clientEndpoint->clientMutex);
    UA_MonitoredItemCreateRequest monRequest =
        UA_MonitoredItemCreateRequest_default(nodeId); //UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERSTATUS_CURRENTTIME)
    UA_MonitoredItemCreateResult monResponse =
        UA_Client_MonitoredItems_createDataChange(client, subId,
            UA_TIMESTAMPSTORETURN_BOTH, monRequest, static_cast<void *>(handle),
            onSubscriptionValueChanged,
            NULL);

    if (monResponse.statusCode == UA_STATUSCODE_GOOD) {
      DEVLOG_INFO("Monitoring succeeded', id %u\n",
          monResponse.monitoredItemId);
      //startClient(); desperate try
    } else {
      DEVLOG_ERROR("Adding monitoring variable failed: %s\n",
          UA_StatusCode_name(monResponse.statusCode));
    }
  }
  }

}

void COPC_UA_Client_Handler::stateCallback(UA_Client *client,
    UA_ClientState clientState) {

  switch (clientState) {
  case UA_CLIENTSTATE_DISCONNECTED:
    DEVLOG_INFO("The client is disconnected");
    break;
  case UA_CLIENTSTATE_CONNECTED:
    DEVLOG_INFO("A TCP connection to the server is open");
    break;
  case UA_CLIENTSTATE_SECURECHANNEL:
    DEVLOG_INFO("A SecureChannel to the server is open");
    break;
  case UA_CLIENTSTATE_SESSION: {
    DEVLOG_INFO("A session with the server is open");
  }
    break;
  case UA_CLIENTSTATE_SESSION_RENEWED:
    DEVLOG_INFO("A session with the server is open (renewed)");
    break;
  }
  return;
}
