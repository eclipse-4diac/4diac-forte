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
#include <extevhan.h>
#include <conn.h>
#include <stdio.h>
#include "comlayer.h"
#include <forte_config_opc_ua.h>
#include "opcua_helper.h"
#include "opcua_layer.h"
#include <forte_string.h>

struct UA_SubscriptionCallback_Handle {
  forte::com_infra::CComLayer *layer;
  const struct UA_TypeConvert *convert;
  unsigned int portIndex;
};

typedef struct UA_ClientEndpointMap UA_ClientEndpointMap;

// cppcheck-suppress noConstructor
class COPC_UA_Client_Handler: public CExternalEventHandler, public CThread {

  CSinglyLinkedList<struct UA_SubscriptionCallback_Handle *> subscriptionCallbackHandles;

  const UA_NodeId *subscriptionNodeId;
  DECLARE_HANDLER(COPC_UA_Client_Handler)

public:

  /* functions needed for the external event handler interface */
  void enableHandler(void);

  void disableHandler(void);

  void setPriority(int pa_nPriority);

  int getPriority(void) const;

  void addMonitoringItem(UA_Client* client, forte::com_infra::CComLayer *comLayer, UA_NodeId nodeId,
      const struct UA_TypeConvert *convert, unsigned int portIndex);
//  UA_StatusCode
//    registerSubscriptionCallBack(UA_Client *client);

  static UA_StatusCode callMethod(UA_Client *client, const UA_NodeId objectId,
                 const UA_NodeId methodId, size_t inputSize, const UA_Variant *input,
                 size_t *outputSize, UA_Variant **output);

  static void stateCallback(UA_Client *client, UA_ClientState clientState);

  static void
  deleteSubscriptionCallback(UA_Client *client, UA_UInt32 subscriptionId,
      void *subscriptionContext);


  static void onSubscriptionValueChanged(UA_Client *client, UA_UInt32 subId, void *subContext,
      UA_UInt32 monId, void *monContext, UA_DataValue *value);
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
  getClientForEndpoint(const char *endpointUrl, bool createIfNotFound, bool subscription,
      CSyncObject **clientMutex);
protected:

private:

  struct UA_ClientEndpointMap {
    UA_Client *client;
    CSyncObject *clientMutex;
    bool subscription;
    CIEC_STRING* endpointUrl;
    UA_UInt32 subscriptionId;
  };

  CSyncObject mClientMapMutex;
  /**
     * Collector list for callback handles to be able to clean them up on destroy.
     */
  CSinglyLinkedList<UA_ClientEndpointMap *> clients;

  // OPC UA Client and configuration
  volatile UA_Boolean uaClientRunningFlag;

  UA_ClientEndpointMap* getHandleForClient(const UA_Client *client);
  /**
   * Starts the OPC UA server, if it is not already running
   */
  void startClient();

  /**
   * Sets the uaServerRunningFlag to false which causes the UA Server to stop.
   */
  void stopClient();

  /**
   * Overridden run() from CThread which loops the UA Server.
   */
  virtual void run();


  struct ReferencedNodeByLayer {
    const UA_NodeId *nodeId;
    CSinglyLinkedList<const COPC_UA_Layer*> referencedByLayer;
  };
  CSinglyLinkedList<struct ReferencedNodeByLayer*> nodeLayerReferences;
  CSyncObject mNodeLayerMutex;
  CSemaphore mServerStarted;


};

#endif /* SRC_MODULES_OPC_UA_OPCUACLIENTHANDLER_H_ */
