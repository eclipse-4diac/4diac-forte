/*******************************************************************************
 * Copyright (c) 2015-2016 Florian Froschermeier <florian.froschermeier@tum.de>,
 * 							fortiss GmbH
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


#ifndef SRC_MODULES_OPC_UA_OPCUAHANDLER_H_
#define SRC_MODULES_OPC_UA_OPCUAHANDLER_H_

#include <singlet.h>
#include <forte_thread.h>
#include <extevhan.h>
#include <conn.h>
#include "funcbloc.h"
#include <stdio.h>
#include "devlog.h"
#include "comlayer.h"
#include <forte_config_opc_ua.h>
#include "opcua_helper.h"
#include "opcua_layer.h"


struct UA_NodeCallback_Handle {
	forte::com_infra::CComLayer *comLayer;
	const struct UA_TypeConvert *convert;
	unsigned int portIndex;
};

struct UA_ClientEndpointMap;
typedef struct UA_ClientEndpointMap UA_ClientEndpointMap;

// cppcheck-suppress noConstructor
class COPC_UA_Handler : public CExternalEventHandler, public CThread {
DECLARE_SINGLETON(COPC_UA_Handler);

public:


	/* functions needed for the external event handler interface */
	void enableHandler(void);

	void disableHandler(void);

	void setPriority(int pa_nPriority);

	int getPriority(void) const;

	/**
	 * For a given connection SourcePoint between two 61499 FBs add a variable Node to the OPC UA address space.
	 * Node is described by the name of the port and the name of the parent function block.
	 * If creation successful the NodeId is returned otherwise
	 * UA_StatusCode from node creation with error message.
	 * @param parentNode Parent node for the variable
	 * @param varName browse name of the variable
	 * @param varType Datatype of the varialbe
	 * @param varValue initial value of the variable
	 * @param returnVarNodeId the node ID of the created variable node
	 * @param allowWrite if set to false, writing to variable will be disabled
	 * @return UA_STATUSCODE_GOOD on succes, the error code otherwise
	 */
	UA_StatusCode createVariableNode(const UA_NodeId *parentNode, const char *varName, const UA_DataType *varType, void *varValue,
									 UA_NodeId *returnVarNodeId, bool allowWrite);

	/**
	 * Create a new method node in the OPC UA information model.
	 *
	 * @param parentNode parent node where the method should be created
	 * @param namespaceIdx namespace index for the new method
	 * @param methodName browse name and display name of the method
	 * @param callback Callback which is called when the method is invoked
	 * @param callbackHandle handle passed to the callback
	 * @param inputArgumentsSize size of the inputArguments array
	 * @param inputArguments list of input arguments for the method. These are normally the RD ports
	 * @param outputArgumentsSize size of outputArguments array
	 * @param outputArguments list of output arguments for the method. These are normally the SD ports
	 * @param returnNodeId node id of the newly created method
	 * @return UA_STATUSCODE_GOOD on success. Error otherwise
	 */
	UA_StatusCode createMethodNode(const UA_NodeId *parentNode, UA_UInt16 namespaceIdx, const char *methodName, UA_MethodCallback callback,
								   void *callbackHandle, unsigned int inputArgumentsSize, const UA_Argument *inputArguments,
								   unsigned int outputArgumentsSize, const UA_Argument *outputArguments, UA_NodeId *returnNodeId);


	/**
	 * Update UA Address Space node value given by the data pointer to an IEC61499 data object.
	 *
	 * @param nodeId node id of the variable to be updated
	 * @param data new value for the variable
	 * @param convert converter object to convert IEC61499 to OPC UA data value
	 * @return UA_STATUSCODE_GOOD on success. Error otherwise
	 */
	UA_StatusCode updateNodeValue(const UA_NodeId *nodeId, const CIEC_ANY *data, const UA_TypeConvert *convert);

	/**
	 * Register a callback routine to a Node in the Address Space that is executed
	 * on either write or read access on the node. A handle to the caller communication layer
	 * is passed too. This alleviates for the process of searching the
	 * originating layer of the external event.
	 *
	 * @param nodeId
	 * @param comLayer
	 * @param convert
	 * @return
	 */
	UA_StatusCode
	registerNodeCallBack(const UA_NodeId *nodeId, forte::com_infra::CComLayer *comLayer, const struct UA_TypeConvert *convert, unsigned int portIndex);

	/**
	 * Callback when an external client writes to a variable on this server.
	 *
	 * @param h handle passed to the write method.
	 * @param nodeid node id of the changed variable
	 * @param data the new data for the variable
	 * @param range range indicating the size of the data variant.
	 */
	static void onWrite(UA_Server *server, const UA_NodeId *sessionId,
						void *sessionContext, const UA_NodeId *nodeId,
						void *nodeContext, const UA_NumericRange *range,
						const UA_DataValue *data);

	/**
	 * Get the node id of the node which is represented by the given path.
	 * If createIfNotFound is set to true, all nodes which do not exist yet will be created.
	 *
	 * @param foundNodeId the node ID which corresponds to the nodePath or null if it is not found and createIfNotFound is false.
	 * @param nodePath Full path to the node, e.g. '/Objects/FlipFlop'
	 * @param createIfNotFound if true all the missing nodes will be created as FolderType objects.
	 * @param startingNode the nodeId from where the given browse path starts. If NULL, the starting Node is Root and the path has to
	 * 			start with '/Objects'
	 * @param newNodeType type of the new node (only used for the last one), if it has to be created.
	 * 			Default is FolderType which is also used for intermediate nodes, if they don't exist
	 * @param clientInitialized optional client which should be used to get the node. The client has to be already connected if given.
	 * 			If no client is given, a new temporary client will be created and connected to localhost
	 * @return UA_STATUSCODE_GOOD on success or the corresponding error code.
	 */
	UA_StatusCode
	getNodeForPath(UA_NodeId **foundNodeId, const char *nodePath, bool createIfNotFound,
				   const UA_NodeId *startingNode = NULL, const UA_NodeId *newNodeType = NULL,
				   UA_NodeId **parentNodeId = NULL, UA_Client *clientInitialized = NULL,
				   CSinglyLinkedList<UA_NodeId *> *nodeIdsAlongPath = NULL);

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
	getClientForEndpoint(const char *endpointUrl, bool createIfNotFound, CSyncObject **clientMutex);


	CSyncObject *
	getMutexForClient(const UA_Client *client);


	/**
	 * Forces event handling by calling startNewEventChain.
	 *
	 * @param layer the layer for which the event should be handled
	 */
	void forceEventHandling(COPC_UA_Layer *layer) {
		getInstance().startNewEventChain(layer->getCommFB());
	}

	void referencedNodesIncrement(const CSinglyLinkedList<UA_NodeId *> *nodes, const COPC_UA_Layer* layer);

	void referencedNodesDecrement(const CSinglyLinkedList<UA_NodeId *> *nodes, const COPC_UA_Layer* layer, bool deleteIfLastReference);

	const UA_String getDiscoveryUrl() const {
		if (uaServerConfig->networkLayersSize == 0)
			return UA_STRING_NULL;
		return uaServerConfig->networkLayers[0].discoveryUrl;
	}

#ifdef FORTE_COM_OPC_UA_MULTICAST
	void registerWithLds(const UA_String *discoveryUrl);
	void removeLdsRegister(const UA_String *discoveryUrl);
#endif
protected:

private:
	// OPC UA Server and configuration
	UA_Server *uaServer;
	UA_ServerConfig *uaServerConfig;

	// OPC UA Client and configuration
	volatile UA_Boolean *uaServerRunningFlag;

	/**
	 * Sets the uaServerRunningFlag to true. To start the server, you also need to call run.
	 */
	void setServerRunning();

	/**
	 * Sets the uaServerRunningFlag to false which causes the UA Server to stop.
	 */
	void stopServerRunning();

	/**
	 * Creates the configuration for the OPC UA Server.
	 * @param UAServerPort port where the OPC UA Server should listen.
	 */
	void configureUAServer(TForteUInt16 UAServerPort);

	/**
	 * Overridden run() from CThread which loops the UA Server.
	 */
	virtual void run();

	/**
	 * Mutex for making sure that getNodeForPath is only called by one layer at a time.
	 * If a race condition would occur, two layers may try to create the same node at the same time.
	 */
	CSyncObject getNodeForPathMutex;

	/**
	 * Collector list for callback handles to be able to clean them up on destroy.
	 */
	CSinglyLinkedList<struct UA_NodeCallback_Handle *> nodeCallbackHandles;

	/**
	 * Collector list for callback handles to be able to clean them up on destroy.
	 */
	CSinglyLinkedList<struct UA_ClientEndpointMap *> clients;


#ifdef FORTE_COM_OPC_UA_MULTICAST

	/**
	 * List of LDS servers where this instance is already registered.
	 */
	CSinglyLinkedList<const char *> registeredWithLds;
#endif

	struct ReferencedNodeByLayer {
		const UA_NodeId *nodeId;
		CSinglyLinkedList<const COPC_UA_Layer*> referencedByLayer;
	};
	CSinglyLinkedList<struct ReferencedNodeByLayer*> nodeLayerReferences;

};

#endif /* SRC_MODULES_OPC_UA_OPCUAHANDLER_H_ */
