/*******************************************************************************
 * Copyright (c) 2015-2016 Florian Froschermeier <florian.froschermeier@tum.de>,
 *               fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Florian Froschermeier
 *      - initial integration of the OPC-UA protocol
 *    Stefan Profanter
 *      - refactoring and adaption to new concept
 *******************************************************************************/

#ifndef SRC_MODULES_OPC_UA_OPCUA_LAYER_H_
#define SRC_MODULES_OPC_UA_OPCUA_LAYER_H_

#include "comlayer.h"
#include <forte_any.h>
#include "comlayer_async.h"
#include "opcua_helper.h"

struct UA_ParentNodeHandler {
  UA_NodeId *parentNodeId;
  UA_NodeId *methodNodeId;
  forte::com_infra::CComLayer* layer;
};

class CIEC_STRING;

class COPC_UA_Layer: public forte::com_infra::CComLayerAsync {
public:

  COPC_UA_Layer(forte::com_infra::CComLayer *paUpperLayer,
      forte::com_infra::CBaseCommFB *paComFB);

  virtual ~COPC_UA_Layer();

  forte::com_infra::EComResponse sendData(void *paData,
      unsigned int paSize);

  forte::com_infra::EComResponse recvData(const void *paData,
      unsigned int paSize);

  virtual forte::com_infra::EComResponse processInterruptChild();

private:
  static CSinglyLinkedList<UA_ParentNodeHandler *> parentsLayers;

  struct FB_NodeIds {
    UA_NodeId *functionBlockId;
    UA_NodeId *variableId;
    const UA_TypeConvert *convert;
  };

  /**
   * set to true if we are connecting to a remote OPC UA server, i.e., we need to use a client
   */
  bool mUseClient;

  /**
   * Delete/free the node ids up to the given maxIndex index.
   *
   * @param nodeIds list of node ids
   * @param maxIndex size of the nodeIds array to be freed
   */
  void deleteNodeIds(struct FB_NodeIds **paNodeIds, unsigned int paMaxIndex);

  forte::com_infra::EComResponse openConnection(char *paLayerParameter);

  void closeConnection();

  /**
   * Get the counterpart function block and port for a given port index.
   *
   * This is achieved by following the connection and reading out the interface specification.
   *
   * @param portIndex port index of the local port in the SD/RD array.
   * @param isSD indicates if portIndex is for SD or for RD array
   * @param connectedToFb function block to which the port is connected to
   * @param connectedToPortName name of the remote port
   * @param convert convert information to convert IEC61499 to OPC UA data type based on the connected to port type
   * @return true if all the information is successfully retrieved. false otherwise. If false, some or all of the passed pointers may be nullptr
   */
  bool getPortConnectionInfo(unsigned int paPortIndex, bool paIsSD,
      const CFunctionBlock **paConnectedToFb, const char **paConnectedToPortName,
      const UA_TypeConvert **paConvert) const;

  /**
   * Create OPC UA nodes in the information model of the server for every connected function block and port combination.
   *
   * @param nodeIds list of node ids of the newly created nodes
   * @param numPorts number of RD/SD ports
   * @param isSD should the nodes be created for the RD or SD ports
   * @return e_InitOk on success
   */
  forte::com_infra::EComResponse createPubSubNodes(struct FB_NodeIds **paNodeIds,
      unsigned int paNumPorts, bool paIsSD);

  UA_StatusCode referenceNodes(UA_NodeId **paFoundNodeId, const char *paSubnodePath,
      bool paCreatenewNode);

  forte::com_infra::EComResponse createSubscription(struct FB_NodeIds **paNodeIds,
      unsigned int paNumPorts, bool paIsSD, const char * const paSubnodePath);

  /**
   * Creates the OPC UA method node for the SERVER function block.
   * The method's name is the same as the name of the SERVER FB. The input and output arguments are initialized based on the RD (input) and SD (output) ports.
   *
   * @return e_InitOk on success
   * @see createMethodArguments
   */
  forte::com_infra::EComResponse createMethodNode(const char * const paMethodPath,
      const char * const paMethodName);

  /**
   * Called by createMethodNode to get the list of input/output arguments based on the RD (input) and SD (output) ports.
   * @param arguments newly created array of arguments
   * @param numPorts number of RD/SD ports which is at the same time the number of arguments
   * @param isSD if true, output arguments are created, based on SD ports. If false, input arguments based on RD are created.
   * @return e_InitOk on success
   */
  forte::com_infra::EComResponse createMethodArguments(UA_Argument **paArguments,
      unsigned int paNumPorts, bool paIsSD);

  const UA_TypeConvert **mClientSdConverter;
  const UA_TypeConvert **mClientRdConverter;
  forte::com_infra::EComResponse clientCreateConverter(
      const UA_TypeConvert **paConverterList[], unsigned int paNumPorts, bool paIsSD);

  /**
   * Creates the OPC UA client for the CLIENT function block.
   * The client's endpoint is defined by the ID field of the FB. Clients will be reused if they have the same endpoint url.
   *
   * @return e_InitOk on success
   */
  forte::com_infra::EComResponse createClient(const char* paEndpoint,
      const char* paNodePath, bool paSubscription);

  /**
   * Response for the processInterrupt() method
   */
  forte::com_infra::EComResponse mInterruptResp;

  /**
   * Node id of the folder node represented by the ID browse path setting of the FB.
   */
  UA_NodeId *mFbNodeId;

  /**
   * Node id of the parent folder node represented by the ID browse path setting of the FB.
   */
  UA_NodeId *mFbNodeIdParent;

  /**
   * On SERVER FB this is set to the node id of the method which is created in the information model
   */
  UA_NodeId *mMethodNodeId;

  /**
   * Node class of the node on the remote server identified by the ID field of the FB.
   * Can be either Variable or Method
   */
  UA_NodeClass mRemoteNodeClass;

  /**
   * On CLIENT FB a client is created for communication with a remote OPC UA server.
   */
  UA_Client *mUaClient;

  /**
   * List of node ids created for the SD ports (PUBLISHER)
   */
  struct FB_NodeIds *mSendDataNodeIds;

  /**
   * List of node ids created for the RD ports (SUBSCRIBER)
   */
  struct FB_NodeIds *mReadDataNodeIds;

  /**
   * Callback for the SERVER FB method from OPC UA.
   * If a client calls the method, this callback will be executed. It uses mutex to make sure clients can call the method only sequentially, not in parallel.
   * The RD ports are set to the input arguments, IND is fired and then it waits until RSP is triggered. Note that the default timeout for OPC UA method calls
   * is 5 seconds. This means that the RSP should be triggered within this time to ensure the call does not time out.
   *
   * @param methodHandle points to the COPC_UA_Layer
   * @param objectId parent object, on which the method was called. This should be identical to fbNodeId.
   * @param inputSize number of input arguments. Should be identical to the number of RD ports
   * @param input value of input arguments. The RD ports are set to these values.
   * @param outputSize number of output arguments. Should be identical to the number of SD ports
   * @param output value of output arguments. These will be read from SD ports
   * @return UA_STATUSCODE_GOOD on success. The return value is passed to the caller, i.e., the client.
   */
  static UA_StatusCode onServerMethodCall(UA_Server *paServer,
      const UA_NodeId *paSessionId, void *paSessionContext,
      const UA_NodeId *paMethodId, void *paMethodContext, const UA_NodeId *paObjectId,
      void *paObjectContext, size_t paInputSize, const UA_Variant *paInput,
      size_t paOutputSize, UA_Variant *paOutput);

  char *mClientEndpointUrl;
  char *mClientNodePath;
  CSyncObject *mClientMutex;

  forte::com_infra::EComResponse clientConnect();

  forte::com_infra::EComResponse clientCallAsync(const CIEC_ANY *paSd,
      unsigned int paSdSize);

  /**
   * Mutex to ensure clients can call the server method not in parallel.
   */
  CSyncObject mMutexServerMethodCall;

  /**
   * Flag used by the server method call to wait for the data to be ready.
   * It is set to false in the beginning. As soon as the RSP port is triggered, it is set to true and the method callback can process SD ports.
   */
  CSemaphore mServerMethodCallResultReady;

  virtual void* handleAsyncCall(const unsigned int paCallId, void *paPayload);

  virtual void handleAsyncCallResult(const unsigned int paCallId, void *paPayload,
      void *paResult);

  virtual void handleAsyncEvent();

  CSinglyLinkedList<UA_NodeId *> mReferencedNodes;

  /**
   * split the ID parameter:
   * opc.tcp://10.100.1.0:4840#/Objects/1:Adder
   * between the hash sign
   *
   * @param fullUrl The full url which is splitted. This parameter will be changed due to the use of strtok
   * @param endpoint the endpoint part pointing at the beginning within the returned char, or NULL if no endpoint part
   * @param nodePath the path part pointing at the beginning within the returned char, or NULL if no path part
   * @return duplicated string of fullUrl where endpoint and nodePath point to. Needs to be freed.
   */
  static void splitUrlAndPath(const char *paFullUrl, const char** paEndpoint,
      const char **paNodePath, CIEC_STRING& paResult);

  /**
   * Get the node class of the given node from the remote server using the already initialized uaClient member.
   *
   */
  UA_NodeClass getNodeClass(const UA_NodeId paNodeId);

};

#endif /* SRC_MODULES_OPC_UA_OPCUA_LAYER_H_ */

