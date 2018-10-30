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

#include "opcua_layer.h"
#include "opcua_handler.h"
#include "opcua_client_handler.h"
#include "../../core/datatypes/forte_date_and_time.h"
#include "../../core/cominfra/basecommfb.h"
#include "../../arch/devlog.h"
#include <forte_string.h>
#include <forte_printer.h>
#include <criticalregion.h>
#include <string>

using namespace forte::com_infra;

CSinglyLinkedList<UA_ParentNodeHandler *> COPC_UA_Layer::parentsLayers;

// method call timeout in seconds. Within this time the method call has to return a value
#define METHOD_CALL_TIMEOUT 4

struct AsyncCallPayload {
  UA_Variant *variants;
  unsigned int variantsSize;
};

UA_Boolean running = true;
UA_Boolean clientRunning = false;

COPC_UA_Layer::COPC_UA_Layer(CComLayer *paUpperLayer, CBaseCommFB *paComFB) :
    CComLayerAsync(paUpperLayer, paComFB), mUseClient(false), mClientSdConverter(
    NULL), mClientRdConverter(NULL), mInterruptResp(e_Nothing), mFbNodeId(
    NULL), mFbNodeIdParent(NULL), mMethodNodeId(NULL), mRemoteNodeClass(
        UA_NODECLASS_UNSPECIFIED), mSendDataNodeIds(NULL), mReadDataNodeIds(
    NULL), mClientEndpointUrl(NULL), mClientNodePath(NULL), mClientMutex(
    NULL), mMutexServerMethodCall(), mServerMethodCallResultReady(false), mReferencedNodes() {
  // constructor list initialization

}

COPC_UA_Layer::~COPC_UA_Layer() {
}

void COPC_UA_Layer::splitUrlAndPath(const char *paFullUrl,
    const char** paEndpoint, const char **paNodePath, CIEC_STRING& paResult) {

  paResult.clear();
  if (strlen(paFullUrl) == 0) {
    return;
  }

  paResult = paFullUrl;
  char *idStr = paResult.getValue();

  char *endpointPtr = strtok(idStr, "#");

  if (endpointPtr == NULL) {
    return;
  }

  if (paEndpoint) {
    *paEndpoint = endpointPtr;
  }

  if (paNodePath != NULL) {
    *paNodePath = strtok(NULL, "#");
  }
  if (paNodePath != NULL && *paNodePath == NULL) {
    // # not found, check if it is path or endpoint
    if (idStr[0] == '/') {
      // full url is only node path
      if (paEndpoint) {
        *paEndpoint = NULL;
      }
      *paNodePath = idStr;
    } else {
      // full url is only endpoint
      if (paEndpoint) {
        *paEndpoint = idStr;
      }
      *paNodePath = NULL;
    }
  }
}

EComResponse COPC_UA_Layer::openConnection(char *paLayerParameter) {

  getExtEvHandler<COPC_UA_Handler>().referencedNodesDecrement(&mReferencedNodes, this, true);
  for (CSinglyLinkedList<UA_NodeId *>::Iterator iter = mReferencedNodes.begin(); iter != mReferencedNodes.end(); ++iter) {
    UA_NodeId_delete((*iter));
  }
  mReferencedNodes.clearAll();

  if (mFbNodeId != NULL) {
    DEVLOG_WARNING("OPC UA: Already connected. Nothing to do.\n");
    return e_InitTerminated;
  }

  const char *endpoint = NULL;
  const char *nodePath = NULL;
  CIEC_STRING result;
  splitUrlAndPath(paLayerParameter, &endpoint, &nodePath, result);

  mUseClient = getCommFB()->getComServiceType() == e_Client ||
  ((getCommFB()->getComServiceType() == e_Subscriber || getCommFB()->getComServiceType() == e_Publisher) && endpoint != NULL);

  if (mUseClient && !endpoint) {
    DEVLOG_ERROR("OPC UA: Invalid client identifier. Endpoint URL must be separated by hash '#' from the node path. Given: %s\n", paLayerParameter);
    return e_InitTerminated;
  }

  if (mUseClient && !nodePath) {
    DEVLOG_ERROR("OPC UA: Invalid client identifier. Node path must be given after the hash '#'. Given: %s\n", paLayerParameter);
    return e_InitTerminated;
  }

  // Create all the nodes up to the given node (ID parameter of the FB)
  if (!mUseClient) {
    UA_StatusCode retVal;
    CSinglyLinkedList<UA_NodeId *> nodesAlongPath = CSinglyLinkedList<UA_NodeId *>();
    if (getCommFB()->getComServiceType() == e_Subscriber || getCommFB()->getComServiceType() == e_Publisher) {
      if ((retVal = getExtEvHandler<COPC_UA_Handler>().getNodeForPath(&mFbNodeId, paLayerParameter, true, NULL, NULL, NULL, NULL, &nodesAlongPath)) != UA_STATUSCODE_GOOD) {
        DEVLOG_ERROR("OPC UA: Could not get node for path: '%s': %s\n", paLayerParameter, UA_StatusCode_name(retVal));
        return e_InitTerminated;
      }
    }
    getExtEvHandler<COPC_UA_Handler>().referencedNodesIncrement(&nodesAlongPath, this);

    for (CSinglyLinkedList<UA_NodeId *>::Iterator iter = nodesAlongPath.begin(); iter != nodesAlongPath.end(); ++iter) {
      mReferencedNodes.pushBack((*iter));
    }
  }

  EComResponse response = e_InitOk;
  char* subNodeParameter = strrchr(paLayerParameter, '/');
  if (0 == subNodeParameter) {
    DEVLOG_ERROR("Node path should not end with a slash \n");
    return e_InitTerminated;
  }
  *subNodeParameter='\0';
  subNodeParameter++;
  CIEC_STRING lastNode = CIEC_STRING(subNodeParameter);
  switch (getCommFB()->getComServiceType()) {
    case e_Publisher:
    if (mUseClient) {
      if (getCommFB()->getNumSD() != 1) {
        DEVLOG_ERROR("OPC UA: A Publisher for writing an OPC UA variable only supports one SD. %s\n", getCommFB()->getInstanceName());
        response = e_InitTerminated;
        break;
      }
      DEVLOG_DEBUG("OPC UA: Creating OPC UA Client %s\n", getCommFB()->getInstanceName());
      response = this->createClient(endpoint, nodePath, false);
    } else {
      // Make sure all the nodes exist and have the corresponding variable
      DEVLOG_DEBUG("OPC UA: Creating OPC UA Nodes for publisher %s\n", getCommFB()->getInstanceName());
      response = this->createPubSubNodes(&this->mSendDataNodeIds, getCommFB()->getNumSD(), true);
    }
    break;
    case e_Subscriber:
    {
      if (mUseClient) {
        if (getCommFB()->getNumRD() != 1) {
          DEVLOG_ERROR("OPC UA: A Subscriber for subscribing on an OPC UA variable only supports one RD. %s\n", getCommFB()->getInstanceName());
          response = e_InitTerminated;
          break;
        }
        DEVLOG_DEBUG("OPC UA: Creating subscription. %s\n", getCommFB()->getInstanceName());
        this->createClient(endpoint, nodePath, true);
        response = this->createSubscription(&this->mSendDataNodeIds, getCommFB()->getNumSD(), false, nodePath);
      } else {
        DEVLOG_DEBUG("OPC UA: Creating OPC UA Nodes for subscriber %s\n", getCommFB()->getInstanceName());
        response = this->createPubSubNodes(&this->mReadDataNodeIds, getCommFB()->getNumRD(), false);
      }
      break;
    }
    case e_Server:
    DEVLOG_DEBUG("OPC UA: Creating OPC UA Method for server %s\n", getCommFB()->getInstanceName());
    response = this->createMethodNode(paLayerParameter, lastNode.getValue());
    break;
    case e_Client:
    DEVLOG_DEBUG("OPC UA: Creating OPC UA Client %s\n", getCommFB()->getInstanceName());
    response = this->createClient(endpoint, nodePath, false);
    break;
    default:
    DEVLOG_WARNING("OPC UA: Invalid Comm Service Type for Function Block\n");
  }

  return response;
}

void COPC_UA_Layer::closeConnection() {

  getExtEvHandler<COPC_UA_Handler>().referencedNodesDecrement(&mReferencedNodes, this, true);
  for (CSinglyLinkedList<UA_NodeId *>::Iterator iter = mReferencedNodes.begin(); iter != mReferencedNodes.end(); ++iter) {
    UA_NodeId_delete((*iter));
  }
  mReferencedNodes.clearAll();

  CSinglyLinkedList<UA_ParentNodeHandler *>::Iterator itRunner(parentsLayers.begin());
  CSinglyLinkedList<UA_ParentNodeHandler *>::Iterator itRefNode(parentsLayers.end());
  CSinglyLinkedList<UA_ParentNodeHandler *>::Iterator const itEnd(parentsLayers.end());
  while(itRunner != itEnd) {
    if(itRunner->layer == this) {
      if(itRefNode == itEnd) {
        parentsLayers.popFront();
      }
      else {
        parentsLayers.eraseAfter(itRefNode);
      }
      break;
    }
    itRefNode = itRunner;
    ++itRunner;
  }

  if (mFbNodeId != NULL) {
    UA_NodeId_delete(mFbNodeId);
    mFbNodeId = NULL;
  }
  if (mMethodNodeId != NULL) {
    UA_NodeId_delete(mMethodNodeId);
    mMethodNodeId = NULL;
  }
  this->deleteNodeIds(&mSendDataNodeIds, getCommFB()->getNumSD());
  this->deleteNodeIds(&mReadDataNodeIds, getCommFB()->getNumRD());
  if (mClientSdConverter) {
    forte_free(mClientSdConverter);
    mClientSdConverter = NULL;
  }
  if (mClientRdConverter) {
    forte_free(mClientRdConverter);
    mClientRdConverter = NULL;
  }
  if (mClientEndpointUrl) {
    forte_free(mClientEndpointUrl);
    mClientEndpointUrl = NULL;
  }
  if (mClientNodePath) {
    forte_free(mClientNodePath);
    mClientNodePath = NULL;
  }
  if (mFbNodeIdParent) {
    UA_NodeId_delete(mFbNodeIdParent);
    mFbNodeIdParent = NULL;
  }
}

/**
 * This method is required to inline initialize the connection point.
 */
static const SConnectionPoint& getFirstListEntry(
    const CSinglyLinkedList<SConnectionPoint>& list) {
  CSinglyLinkedList<SConnectionPoint>::Iterator it = list.begin();
  return *it;
}

void COPC_UA_Layer::deleteNodeIds(struct FB_NodeIds **paNodeIds,
    unsigned int paCurrentSize) {
  if (*paNodeIds == NULL)
    return;
  for (unsigned int j = 0; j < paCurrentSize; j++) {
    if ((*paNodeIds)[j].functionBlockId != NULL)
      UA_NodeId_delete((*paNodeIds)[j].functionBlockId);
    if ((*paNodeIds)[j].functionBlockId != NULL)
      UA_NodeId_delete((*paNodeIds)[j].variableId);
  }
  forte_free(*paNodeIds);
  (*paNodeIds) = NULL;
}

bool COPC_UA_Layer::getPortConnectionInfo(unsigned int paPortIndex, bool paIsSD,
    const CFunctionBlock **paConnectedToFb, const char **paConnectedToPortName,
    const UA_TypeConvert **paConvert) const {
  int portId = paPortIndex + 2;
  /* Retrieve Publisher, Connection and Signals Source Function Block Information */
  const SFBInterfaceSpec *interfaceSpec = getCommFB()->getFBInterfaceSpec();
  const CStringDictionary::TStringId portNameId =
      paIsSD ?
          interfaceSpec->m_aunDINames[portId] :
          interfaceSpec->m_aunDONames[portId];

  DEVLOG_DEBUG("OPC UA: Processing %s signal %s at port %i.\n",
      paIsSD ? "publish" : "subscribe",
      CStringDictionary::getInstance().get(portNameId), portId);

  const CDataConnection *portConnection =
      paIsSD ?
          getCommFB()->getDIConnection(portNameId) :
          getCommFB()->getDOConnection(portNameId);
  if (portConnection == NULL) {
    DEVLOG_ERROR(
        "OPC UA: Got invalid port connection on FB %s at port %s. It must be connected to another FB.\n",
        this->getCommFB()->getInstanceName(),
        CStringDictionary::getInstance().get(portNameId));
    return false;
  }

  //TODO for now we assume that the subscriber connection only has one destination. Needs fix!

  if (!paIsSD && portConnection->getDestinationList().isEmpty()) {
    DEVLOG_WARNING("OPC UA: Subscriber does not have any connection.\n");
    return false;
  }

  const SConnectionPoint remoteConnectionPoint =
      paIsSD ?
          portConnection->getSourceId() :
          getFirstListEntry(portConnection->getDestinationList());
  if ((*paConnectedToFb = remoteConnectionPoint.mFB) == NULL) {
    return false;
  }
  const SFBInterfaceSpec *sourceInterfaceSpec =
      (*paConnectedToFb)->getFBInterfaceSpec();
  const CStringDictionary::TStringId sourceNameId =
      paIsSD ?
          sourceInterfaceSpec->m_aunDONames[remoteConnectionPoint.mPortId] :
          sourceInterfaceSpec->m_aunDINames[remoteConnectionPoint.mPortId];
  *paConnectedToPortName = CStringDictionary::getInstance().get(sourceNameId);

  const CIEC_ANY *remotePort =
      paIsSD ?
          (*paConnectedToFb)->getDOFromPortId(remoteConnectionPoint.mPortId) :
          (*paConnectedToFb)->getDIFromPortId(remoteConnectionPoint.mPortId);

  if (!COPC_UA_Helper::isTypeIdValid(remotePort->getDataTypeID())) {
    DEVLOG_ERROR_VAR(
        const CStringDictionary::TStringId connectedToType = paIsSD ? sourceInterfaceSpec->m_aunDODataTypeNames[remoteConnectionPoint.mPortId] : sourceInterfaceSpec->m_aunDIDataTypeNames[remoteConnectionPoint.mPortId]);
    DEVLOG_ERROR("OPC UA: Mapping of type %s to OPC UA not defined.\n",
        CStringDictionary::getInstance().get(connectedToType));
    return false;
  }
  *paConvert =
      &COPC_UA_Helper::mapForteTypeIdToOpcUa[remotePort->getDataTypeID()];

  return true;
}

UA_StatusCode COPC_UA_Layer::referenceNodes(UA_NodeId **paFoundNodeId,
    const char *paSubnodePath, bool paCreatenewNode) {
  // create/get node for connected FB
  size_t len = strlen(paSubnodePath) + 2; // include slash and nullbyte
  char *fbBrowseName = static_cast<char *>(forte_malloc(sizeof(char) * len));
  forte_snprintf(fbBrowseName, len, "/%s", paSubnodePath);
  UA_NodeId newNodeType = UA_NODEID_NUMERIC(0, UA_NS0ID_BASEOBJECTTYPE);
  CSinglyLinkedList<UA_NodeId *> nodesAlongPath =
      CSinglyLinkedList<UA_NodeId *>();
    UA_StatusCode retVal = getExtEvHandler<COPC_UA_Handler>().getNodeForPath(paFoundNodeId, fbBrowseName, paCreatenewNode, mFbNodeId, &newNodeType, NULL, NULL,
      &nodesAlongPath);
  forte_free(fbBrowseName);

    getExtEvHandler<COPC_UA_Handler>().referencedNodesIncrement(&nodesAlongPath, this);

  for (CSinglyLinkedList<UA_NodeId *>::Iterator iter = nodesAlongPath.begin();
      iter != nodesAlongPath.end(); ++iter) {
    mReferencedNodes.pushBack((*iter));
  }
  return retVal;
}

forte::com_infra::EComResponse COPC_UA_Layer::createSubscription(
    struct FB_NodeIds **paNodeIds, unsigned int paNumPorts, bool paIsSD,
    const char * const paSubnodePath) {
  UA_NodeId objectsFolder = UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER);
  const UA_TypeConvert *conv;
  const CFunctionBlock *connectedToFb = NULL;
  const char *connectedToName = NULL;
  if (!getPortConnectionInfo(0, paIsSD, &connectedToFb, &connectedToName,
      &conv)) {
    deleteNodeIds(paNodeIds, 0);
    return e_InitInvalidId;
  }
  size_t len = strlen(paSubnodePath) + 1; // include nullbyte
  char *sourceVarBrowseName = static_cast<char *>(forte_malloc(sizeof(char) * len));
  forte_snprintf(sourceVarBrowseName, len, "%s", paSubnodePath);
  UA_NodeId* subscriptionNode = UA_NodeId_new();
  CSinglyLinkedList<UA_NodeId *> nodesAlongPath = CSinglyLinkedList<UA_NodeId *>();
  clientConnect();
  UA_StatusCode retVal = getExtEvHandler<COPC_UA_Handler>().getNodeForPath(&subscriptionNode, sourceVarBrowseName, false, &objectsFolder, NULL, NULL, mUaClient,
    &nodesAlongPath);
  if(UA_STATUSCODE_GOOD != retVal) {
    DEVLOG_ERROR("[OPC-UA] Error at getting node for subscription %s\n", UA_StatusCode_name(retVal));
    forte_free(sourceVarBrowseName);
    return e_InitInvalidId;
  }

  getExtEvHandler<COPC_UA_Client_Handler>().addMonitoringItem(mUaClient, this, *subscriptionNode, conv, 0);
  forte_free(sourceVarBrowseName);

  return e_InitOk;
}

forte::com_infra::EComResponse COPC_UA_Layer::createPubSubNodes(
    struct FB_NodeIds **paNodeIds, unsigned int paNumPorts, bool paIsSD) {
  if (*paNodeIds != NULL) {
    DEVLOG_ERROR("OPC UA: Publish/Subscribe Nodes already initialized.\n");
    return e_InitInvalidId;
  }

  if (paNumPorts == 0) {
    DEVLOG_ERROR(
        "OPC UA: OPC UA Publisher/Subscriber without SD/RD Signal, pure event handling\n");
    return e_InitInvalidId;
  }

  *paNodeIds = static_cast<struct FB_NodeIds *>(forte_malloc(
      sizeof(struct FB_NodeIds) * paNumPorts));

  const CIEC_ANY *initData =
      paIsSD ? getCommFB()->getSDs() : getCommFB()->getRDs();

  for (unsigned int i = 0; i < paNumPorts; i++) {

    const CFunctionBlock *connectedToFb = NULL;
    const char *connectedToName = NULL;
    const UA_TypeConvert *conv;

    if (!getPortConnectionInfo(i, paIsSD, &connectedToFb, &connectedToName,
        &conv)) {
      deleteNodeIds(paNodeIds, i);
      return e_InitInvalidId;
    }

    const char *subnodePath = connectedToFb->getInstanceName();
    UA_StatusCode retVal = referenceNodes(&(*paNodeIds)[i].functionBlockId,
        subnodePath, true);

    if (retVal == UA_STATUSCODE_GOOD) {
      // create/get variable node for port on connected FB
      size_t len = strlen(connectedToName) + 2; // include slash and nullbyte
      char *sourceVarBrowseName = static_cast<char *>(forte_malloc(
          sizeof(char) * len));
      forte_snprintf(sourceVarBrowseName, len, "/%s", connectedToName);
      retVal = getExtEvHandler<COPC_UA_Handler>().getNodeForPath(&(*paNodeIds)[i].variableId, sourceVarBrowseName, false, (*paNodeIds)[i].functionBlockId);
      forte_free(sourceVarBrowseName);
      if (retVal == UA_STATUSCODE_GOOD) {
        if ((*paNodeIds)[i].variableId == NULL) {
          // we need to create the variable
          (*paNodeIds)[i].convert = conv;
          void *varValue = UA_new(conv->type);
          UA_init(varValue, conv->type);
          (*paNodeIds)[i].variableId = UA_NodeId_new();
          if (!conv->get(&initData[i], varValue)) {
            DEVLOG_WARNING(
                "OPC UA: Cannot convert value of port %d for initialization",
                i);
          }
            retVal = getExtEvHandler<COPC_UA_Handler>().createVariableNode((*paNodeIds)[i].functionBlockId, connectedToName, conv->type,
              varValue, (*paNodeIds)[i].variableId, !paIsSD);
          UA_delete(varValue, conv->type);
          if (retVal == UA_STATUSCODE_GOOD && !paIsSD) {
              getExtEvHandler<COPC_UA_Handler>().registerNodeCallBack((*paNodeIds)[i].variableId, this, conv, i);
          }
        } else if(!paIsSD) {
          // the node already exists. It could be the case that the node was previously created
          // for a subscribe FB, which sets the access to read only. Since this is now a publish FB
          // we need to change the access to read & write
          if(UA_STATUSCODE_GOOD
            != getExtEvHandler<COPC_UA_Handler>().updateNodeUserAccessLevel((*paNodeIds)[i].variableId, UA_ACCESSLEVELMASK_READ & UA_ACCESSLEVELMASK_WRITE)) {
            DEVLOG_WARNING("OPC UA: Cannot set write permission of node for port %d", i);
          }
        }
      } // else if retVal = UA_STATUSCODE_GOOD the node already exists

    }

    if (retVal != UA_STATUSCODE_GOOD) {
      deleteNodeIds(paNodeIds, i + 1);
      return e_InitInvalidId;
    }
  }
  return e_InitOk;
}

forte::com_infra::EComResponse COPC_UA_Layer::createMethodNode(
    const char * const paMethodPath, const char * const paMethodName) {

  DEVLOG_DEBUG("OPC UA: OPC UA creating method for %s\n",
      getCommFB()->getInstanceName());

  if (mSendDataNodeIds != NULL || mReadDataNodeIds != NULL) {
    DEVLOG_ERROR("OPC UA: Method node already initialized.\n");
    return e_InitInvalidId;
  }

  if (this->getCommFB()->getNumRD() == 0
      && this->getCommFB()->getNumSD() == 0) {
    DEVLOG_INFO(
        "OPC UA: OPC UA Method without SD/RD Signal, pure event handling\n");
  }

  // create the list of input arguments of the method which corresponds to the RD ports (i.e. output of the FB)
  UA_Argument *inputArguments;
  if (createMethodArguments(&inputArguments, getCommFB()->getNumRD(), false)
      != e_InitOk) {
    return e_InitInvalidId;
  }

  UA_Argument *outputArguments;
  if (createMethodArguments(&outputArguments, getCommFB()->getNumSD(), true)
      != e_InitOk) {
    UA_Array_delete(inputArguments, getCommFB()->getNumRD(),
        &UA_TYPES[UA_TYPES_ARGUMENT]);
    return e_InitInvalidId;
  }

  forte::com_infra::EComResponse result = e_InitOk;

  CIEC_STRING sourceVarBrowseName = paMethodPath;
  sourceVarBrowseName.append("/");
  sourceVarBrowseName.append(paMethodName);

  UA_StatusCode retVal;
  UA_NodeId* methodNode = UA_NodeId_new();
  retVal = referenceNodes(&methodNode, sourceVarBrowseName.getValue(), false);
  bool create;

  if(methodNode != NULL) {
    create = false;
  } else {
    create = true;
    methodNode = mFbNodeId;
  }
  retVal = getExtEvHandler<COPC_UA_Handler>().getNodeForPath(&mFbNodeId, paMethodPath, true);

  mMethodNodeId = UA_NodeId_new();

  if((retVal = getExtEvHandler<COPC_UA_Handler>().createMethodNode(mFbNodeId, paMethodName, COPC_UA_Layer::onServerMethodCall, this, getCommFB()->getNumRD(),
    inputArguments, getCommFB()->getNumSD(), outputArguments, mMethodNodeId, create)) != UA_STATUSCODE_GOOD) {
    DEVLOG_ERROR("OPC UA: OPC UA could not create method node: %s\n", UA_StatusCode_name(retVal));
    result = e_InitInvalidId;
  }
  UA_ParentNodeHandler *handle = new UA_ParentNodeHandler();

  handle->parentNodeId = mFbNodeId;
  handle->layer = this;
  if (create) {
    handle->methodNodeId = mMethodNodeId;
  } else {
    handle->methodNodeId = methodNode;

  }

  COPC_UA_Layer::parentsLayers.pushBack(handle);

  UA_Array_delete(inputArguments, getCommFB()->getNumRD(),
      &UA_TYPES[UA_TYPES_ARGUMENT]);
  UA_Array_delete(outputArguments, getCommFB()->getNumSD(),
      &UA_TYPES[UA_TYPES_ARGUMENT]);
  return result;
}

forte::com_infra::EComResponse COPC_UA_Layer::createMethodArguments(
    UA_Argument **paArguments, unsigned int paNumPorts, bool paIsSD) {
  *paArguments = static_cast<UA_Argument *>(UA_Array_new(paNumPorts,
      &UA_TYPES[UA_TYPES_ARGUMENT]));

  for (unsigned int i = 0; i < paNumPorts; i++) {

    UA_Argument *arg = &(*paArguments)[i];
    UA_Argument_init(arg);
    arg->arrayDimensionsSize = 0;
    arg->arrayDimensions = NULL;

    const CFunctionBlock *connectedToFb = NULL;
    const char *connectedToName = NULL;
    const UA_TypeConvert *conv;

    if (!getPortConnectionInfo(i, paIsSD, &connectedToFb, &connectedToName,
        &conv)) {
      UA_Array_delete(*paArguments, paNumPorts, &UA_TYPES[UA_TYPES_ARGUMENT]);
      return e_InitInvalidId;
    }

    arg->dataType = conv->type->typeId;
    arg->description = UA_LOCALIZEDTEXT_ALLOC("en-US", "Method parameter");
    arg->name = UA_STRING_ALLOC(connectedToFb->getInstanceName());
    arg->valueRank = -1;

  }

  return e_InitOk;
}

forte::com_infra::EComResponse COPC_UA_Layer::clientCreateConverter(
    const UA_TypeConvert **paConverterList[], unsigned int paNumPorts,
    bool paIsSD) {
  if (*paConverterList != NULL) {
    DEVLOG_ERROR("OPC UA: Converter list already initialized.\n");
    return e_InitInvalidId;
  }

  if (paNumPorts == 0)
    return e_InitOk;

  *paConverterList = static_cast<const UA_TypeConvert **>(forte_malloc(
      sizeof(UA_TypeConvert *) * paNumPorts));

  for (unsigned int i = 0; i < paNumPorts; i++) {

    const CFunctionBlock *connectedToFb = NULL;
    const char *connectedToName = NULL;

    if (!getPortConnectionInfo(i, paIsSD, &connectedToFb, &connectedToName,
        &(*paConverterList)[i])) {
      forte_free(*paConverterList);
      return e_InitInvalidId;
    }
  }
  return e_InitOk;
}

forte::com_infra::EComResponse COPC_UA_Layer::clientConnect() {
  UA_ClientState state = UA_Client_getState(mUaClient);

  if (state
      == UA_CLIENTSTATE_CONNECTED&& mFbNodeId != NULL && mFbNodeIdParent != NULL)
    // ready. Nothing to do
    return e_InitOk;

  // other thread may currently create nodes for the same path, thus mutex
  CCriticalRegion criticalRegion(*this->mClientMutex);

  if (state != UA_CLIENTSTATE_CONNECTED) {
    if (mFbNodeId != NULL) {
      UA_NodeId_delete(mFbNodeId);
      mFbNodeId = NULL;
    }
    if (mFbNodeIdParent != NULL) {
      UA_NodeId_delete(mFbNodeIdParent);
      mFbNodeIdParent = NULL;
    }

    DEVLOG_INFO("OPC UA: Client connecting to %s\n", mClientEndpointUrl);
    UA_StatusCode retVal = UA_Client_connect(mUaClient, mClientEndpointUrl);

    if (retVal != UA_STATUSCODE_GOOD) {
      DEVLOG_ERROR(
          "OPC UA: Could not connect client to endpoint %s. Error: %s\n",
          mClientEndpointUrl, UA_StatusCode_name(retVal));
      return e_InitTerminated;
    }
  }

  UA_StatusCode retVal = getExtEvHandler<COPC_UA_Handler>().getNodeForPath(&mFbNodeId, mClientNodePath, false, NULL, NULL, &mFbNodeIdParent, this->mUaClient);
  if(retVal != UA_STATUSCODE_GOOD) {
    DEVLOG_ERROR("OPC UA: Could not get node for path from server '%s': '%s'. %s\n", mClientEndpointUrl, mClientNodePath, UA_StatusCode_name(retVal));
    return e_InitTerminated;
  }

  if (mFbNodeId == NULL) {
    DEVLOG_ERROR(
        "OPC UA: Could not find the method node on the server '%s': '%s'\n",
        mClientEndpointUrl, mClientNodePath);
    return e_InitTerminated;
  }

  if (mFbNodeIdParent == NULL) {
    DEVLOG_ERROR(
        "OPC UA: Could not find the parent node of the method on the server '%s': '%s'\n",
        mClientEndpointUrl, mClientNodePath);
    return e_InitTerminated;
  }

  return e_InitOk;
}

forte::com_infra::EComResponse COPC_UA_Layer::createClient(
    const char* paEndpoint, const char* paNodePath, bool paSubscription) {

  if (mClientEndpointUrl != NULL || mClientNodePath != NULL) {
    DEVLOG_ERROR("OPC UA: Client already initialized.");
    return e_InitTerminated;
  }

  // start the async call thread
  this->start();

  this->mUaClient = getExtEvHandler<COPC_UA_Client_Handler>().getClientForEndpoint(paEndpoint, true, paSubscription, &mClientMutex);
  if(!this->mUaClient) {
    return e_InitTerminated;
  }

  size_t nodePathLen = strlen(paNodePath);
  this->mClientNodePath = (char*) forte_malloc(nodePathLen + 1);
  strcpy(this->mClientNodePath, paNodePath);
  while (nodePathLen && this->mClientNodePath[nodePathLen - 1] == '/') {
    this->mClientNodePath[nodePathLen - 1] = 0;
    nodePathLen--;
  }

  this->mClientEndpointUrl = (char*) forte_malloc(strlen(paEndpoint) + 1);
  strcpy(this->mClientEndpointUrl, paEndpoint);

  forte::com_infra::EComResponse resp = clientCreateConverter(
      &mClientRdConverter, getCommFB()->getNumRD(), false);
  if (resp != e_InitOk)
    return resp;

  return clientCreateConverter(&mClientSdConverter, getCommFB()->getNumSD(),
      true);

}

EComResponse COPC_UA_Layer::sendData(void *paData, unsigned int paSize) {

  EComResponse retVal = e_ProcessDataOk;

  if (getCommFB()->getComServiceType() == e_Server) {
    // continue method call
    mServerMethodCallResultReady.inc();
    return retVal;
  }

  if (getCommFB()->getComServiceType() == e_Client) {
    return this->clientCallAsync(
        paSize ? static_cast<CIEC_ANY *>(paData) : NULL, paSize);
  }

  if(paSize == 0) {
  } else {
    const CIEC_ANY *SDs(static_cast<CIEC_ANY *>(paData));

    if(mUseClient) {
      // write variable value to remote server
      if(getCommFB()->getComServiceType() == e_Publisher)
        return this->clientCallAsync(static_cast<CIEC_ANY *>(paData), paSize);
    } else {
      // change variable value in locale server
      for(unsigned int i = 0; i < paSize; i++) {
        FB_NodeIds *ni = &this->mSendDataNodeIds[i];
        if(getExtEvHandler<COPC_UA_Handler>().updateNodeValue(ni->variableId, &SDs[i], ni->convert) != UA_STATUSCODE_GOOD) {
          DEVLOG_ERROR("OPC UA: Could not convert publisher value for port %d to OPC UA.\n", i);
          retVal = e_ProcessDataDataTypeError;
        }
      }
    }
  }
  return retVal;
}

EComResponse COPC_UA_Layer::recvData(const void *paData, unsigned int) {
  mInterruptResp = e_ProcessDataRecvFaild;

  struct recvData_handle {
    const struct UA_TypeConvert *convert;
    unsigned int portIndex;
    const UA_Variant *data;
  };

  this->start();

  const struct recvData_handle *handleRecv = static_cast<const recvData_handle *>(paData);
  if(!mUseClient) {
    if (getCommFB()->getNumRD() == 0) {
      mInterruptResp = e_ProcessDataOk;
    } else {
      const struct recvData_handle *handleRecv =
          static_cast<const recvData_handle *>(paData);
      // map variable value from local opc ua server to the FB
      if (UA_Variant_isScalar(handleRecv->data)
          && handleRecv->data->type == handleRecv->convert->type
          && handleRecv->data->data) {
        if (handleRecv->convert->set(handleRecv->data->data,
            &getCommFB()->getRDs()[handleRecv->portIndex])) {
          mInterruptResp = e_ProcessDataOk;
          getCommFB()->interruptCommFB(this);
        }
      } else {
        if (UA_Variant_isScalar(handleRecv->data) && handleRecv->data->data) {
          if (handleRecv->convert->set(handleRecv->data->data,
              &getCommFB()->getRDs()[handleRecv->portIndex])) {
            mInterruptResp = e_ProcessDataOk;
            handleAsyncEvent();
          }

        }
      }
    }
  } else {
    if (UA_Variant_isScalar(handleRecv->data) && handleRecv->data->data) {
      if (handleRecv->convert->set(handleRecv->data->data,
          &getCommFB()->getRDs()[handleRecv->portIndex])) {
        mInterruptResp = e_ProcessDataOk;
        handleAsyncEvent();
      }
    }
  }
  return mInterruptResp;
}

forte::com_infra::EComResponse COPC_UA_Layer::clientCallAsync(
    const CIEC_ANY *paSd, unsigned int paSdSize) {

  UA_Variant *inputs = NULL;

  if (paSdSize > 0) {
    inputs = static_cast<UA_Variant *>(UA_Array_new(paSdSize,
        &UA_TYPES[UA_TYPES_VARIANT]));

    for (unsigned int i = 0; i < paSdSize; i++) {
      UA_Variant_init(&inputs[i]);
      void *varValue = UA_new(mClientSdConverter[i]->type);
      UA_init(varValue, mClientSdConverter[i]->type);
      if (!mClientSdConverter[i]->get(&paSd[i], varValue)) {
        DEVLOG_ERROR(
            "OPC UA: Client could not convert input SD_%d to OPC UA.\n", i + 1);
        UA_delete(varValue, mClientSdConverter[i]->type);
        UA_Array_delete(inputs, paSdSize, &UA_TYPES[UA_TYPES_VARIANT]);
        return e_ProcessDataDataTypeError;
      }
      UA_Variant_setScalarCopy(&inputs[i], varValue,
          mClientSdConverter[i]->type);
      UA_delete(varValue, mClientSdConverter[i]->type);
    }
  }

  struct AsyncCallPayload *payload =
      static_cast<struct AsyncCallPayload *>(forte_malloc(
          sizeof(AsyncCallPayload)));
  payload->variants = inputs;
  payload->variantsSize = paSdSize;
  if (callAsync(payload) == 0)
    return e_ProcessDataSendFailed;
  else
    return e_Nothing;
}

EComResponse COPC_UA_Layer::processInterruptChild() {
  return mInterruptResp;
}

UA_StatusCode COPC_UA_Layer::onServerMethodCall(UA_Server *, const UA_NodeId *,
    void *, const UA_NodeId *, void *paMethodContext, const UA_NodeId *, void *,
    size_t paInputSize, const UA_Variant *paInput, size_t paOutputSize,
    UA_Variant *paOutput) {
  COPC_UA_Layer *self = static_cast<COPC_UA_Layer *>(paMethodContext);

  // other thread may currently create nodes for the same path, thus mutex
  CCriticalRegion criticalRegion(self->mMutexServerMethodCall);

  do {
  } while (self->mServerMethodCallResultReady.tryNoWait()); //get semaphore to zero before start method, since application can trigger wrongly RSP many times increasing the semaphore

  if (paInputSize != self->getCommFB()->getNumRD()
      || paOutputSize != self->getCommFB()->getNumSD()) {
    DEVLOG_ERROR(
        "OPC UA: method call got invalid number of arguments. In: %d==%d, Out: %d==%d\n",
        self->getCommFB()->getNumRD(), paInputSize,
        self->getCommFB()->getNumSD(), paOutputSize);
    return UA_STATUSCODE_BADINVALIDARGUMENT;
  }

  self->mInterruptResp = e_ProcessDataOk;

  // put the input values on the wire, i.e. on the RD ports
  for (unsigned int i = 0; i < self->getCommFB()->getNumRD(); i++) {
    if (UA_Variant_isScalar(&paInput[i]) && paInput[i].data != NULL) {
      if (!COPC_UA_Helper::convertFromUa_typeId(
          self->getCommFB()->getRDs()[i].getDataTypeID(), paInput[i].data,
          &self->getCommFB()->getRDs()[i])) {
        self->mInterruptResp = e_ProcessDataRecvFaild;
        DEVLOG_ERROR(
            "OPC UA: can not convert method inputArgument at idx %d to forte type\n",
            i);
        break;
      }
    }
  }

  if (self->mInterruptResp == e_ProcessDataOk) {
    self->getCommFB()->interruptCommFB(self);
    ::getExtEvHandler<COPC_UA_Handler>(*self->getCommFB()).forceEventHandling(self);
  } else {
    return UA_STATUSCODE_BADINVALIDARGUMENT;
  }

  if (!self->mServerMethodCallResultReady.timedWait(
  METHOD_CALL_TIMEOUT * 1E9)) {
    DEVLOG_ERROR(
        "OPC UA: method call did not get result values within timeout.\n");
    return UA_STATUSCODE_BADTIMEOUT;
  }

  // copy SD values to output
  for (unsigned int i = 0; i < self->getCommFB()->getNumSD(); i++) {

    if (!COPC_UA_Helper::isTypeIdValid(
        self->getCommFB()->getSDs()[i].getDataTypeID())) {
      DEVLOG_ERROR(
          "OPC UA: can not convert forte type to method outputArgument at idx %d\n",
          i);
      break;
    }

    const UA_TypeConvert *convert =
        &COPC_UA_Helper::mapForteTypeIdToOpcUa[self->getCommFB()->getSDs()[i].getDataTypeID()];

    void *varValue = UA_new(convert->type);
    if (!convert->get(&self->getCommFB()->getSDs()[i], varValue)) {
      self->mInterruptResp = e_ProcessDataRecvFaild;
      DEVLOG_ERROR(
          "OPC UA: can not convert forte type to method outputArgument at idx %d\n",
          i);
    } else {
      UA_Variant_setScalarCopy(&paOutput[i], varValue, convert->type);
    }

    UA_delete(varValue, convert->type);

    if (self->mInterruptResp != e_ProcessDataOk)
      break;
  }

  return
      self->mInterruptResp == e_ProcessDataOk ?
          UA_STATUSCODE_GOOD : UA_STATUSCODE_BADUNEXPECTEDERROR;
}

UA_NodeClass COPC_UA_Layer::getNodeClass(const UA_NodeId paNodeId) {

  UA_NodeClass nodeClass = UA_NODECLASS_UNSPECIFIED;

  mClientMutex->lock();
  UA_StatusCode retVal = UA_Client_readNodeClassAttribute(this->mUaClient,
      paNodeId, &nodeClass);
  mClientMutex->unlock();

  if (retVal != UA_STATUSCODE_GOOD) {
    DEVLOG_ERROR("OPC UA: Could not determine NodeClass. %s\n",
        UA_StatusCode_name(retVal));
    return UA_NODECLASS_UNSPECIFIED;
  }

  return nodeClass;
}

void *COPC_UA_Layer::handleAsyncCall(const unsigned int /*callId*/,
    void *paPayload) {

  struct AsyncCallPayload *callPayload =
      static_cast<struct AsyncCallPayload *>(paPayload);

  if (this->clientConnect() != e_InitOk) {
    return NULL;
  }

  if (this->mRemoteNodeClass == UA_NODECLASS_UNSPECIFIED) {
    this->mRemoteNodeClass = getNodeClass(*this->mFbNodeId);
    if (this->mRemoteNodeClass == UA_NODECLASS_UNSPECIFIED)
      return NULL;
  }

  if (getCommFB()->getComServiceType() == e_Client) {

    if (this->mRemoteNodeClass == UA_NODECLASS_METHOD) {
      size_t outputSize;
      UA_Variant *output;
      mClientMutex->lock();

      UA_StatusCode retval = UA_STATUSCODE_BADCONNECTIONCLOSED;
      // if the connection to the server is broken between last call and now, try a second time
      for (int i = 0; i < 2 && retval == UA_STATUSCODE_BADCONNECTIONCLOSED;
          i++) {
        retval = UA_Client_call(this->mUaClient, *this->mFbNodeIdParent,
            *this->mFbNodeId, callPayload->variantsSize, callPayload->variants,
            &outputSize, &output);
        if (retval == UA_STATUSCODE_BADCONNECTIONCLOSED) {
          // try to reconnect
          if (this->clientConnect() != e_InitOk) {
            return NULL;
          }
        }
      }
      mClientMutex->unlock();
      if (retval == UA_STATUSCODE_GOOD) {
        DEVLOG_DEBUG(
            "OPC UA: Method call was successfull, and %lu returned values available.\n",
            (unsigned long ) outputSize);

        if (getCommFB()->getNumRD() != outputSize) {
          DEVLOG_ERROR(
              "OPC UA: The number of RD connectors of the client does not match the number of returned values from the method call.\n");
        } else {

          struct AsyncCallPayload *outputData =
              static_cast<struct AsyncCallPayload *>(forte_malloc(
                  sizeof(struct AsyncCallPayload)));

          outputData->variants = output;
          outputData->variantsSize = static_cast<unsigned int>(outputSize);

          return outputData;
        }

      }

      DEVLOG_ERROR("OPC UA: Could not call method. Error: %s\n",
          UA_StatusCode_name(retval));
    } else if (this->mRemoteNodeClass == UA_NODECLASS_VARIABLE) {
      if (callPayload->variantsSize != 0) {
        DEVLOG_INFO(
            "OPC UA: Ignoring unsupported SD input values for CLIENT FB.");
      }
      UA_Variant *output = UA_Variant_new();
      mClientMutex->lock();
      UA_StatusCode retval = UA_Client_readValueAttribute(this->mUaClient,
          *this->mFbNodeId, output);
      mClientMutex->unlock();

      if (retval != UA_STATUSCODE_GOOD) {
        DEVLOG_ERROR("OPC UA: Could not read variable node value. Error: %s\n",
            UA_StatusCode_name(retval));
      } else {
        struct AsyncCallPayload *outputData =
            static_cast<struct AsyncCallPayload *>(forte_malloc(
                sizeof(struct AsyncCallPayload)));

        outputData->variants = output;
        outputData->variantsSize = 1;

        return outputData;
      }
    } else {
      DEVLOG_ERROR(
          "OPC UA: Remote node class for client needs to be of class VARIABLE or METHOD.\n");
      return NULL;
    }

  } else if (getCommFB()->getComServiceType() == e_Publisher) {

    if (this->mRemoteNodeClass != UA_NODECLASS_VARIABLE) {
      DEVLOG_ERROR(
          "OPC UA: Remote node class for publisher needs to be of class VARIABLE.\n");
      return NULL;
    }
    if (callPayload->variantsSize == 0) {
      DEVLOG_ERROR("OPC UA: The PUBLISH FB needs at least one input SD.");
      return NULL;
    }
    if (callPayload->variantsSize > 1) {
      DEVLOG_INFO(
          "OPC UA: Ignoring unsupported SD input values for Publish FB.");
    }

    // this is a Publish FB and we need a variable write

    mClientMutex->lock();
    UA_StatusCode retval = UA_Client_writeValueAttribute(this->mUaClient,
        *this->mFbNodeId, callPayload->variants);
    mClientMutex->unlock();

    if (retval != UA_STATUSCODE_GOOD) {
      DEVLOG_ERROR("OPC UA: Could not write variable node value. Error: %s\n",
          UA_StatusCode_name(retval));
    }
  }
  return NULL;
}

void COPC_UA_Layer::handleAsyncCallResult(const unsigned int /*callId*/,
    void *paPayload, void *paResult) {

  struct AsyncCallPayload *inputData =
      static_cast<struct AsyncCallPayload *>(paPayload);
  UA_Array_delete(inputData->variants, inputData->variantsSize,
      &UA_TYPES[UA_TYPES_VARIANT]);
  forte_free(paPayload);

  if (paResult == NULL)
    return;

  struct AsyncCallPayload *outputData =
      static_cast<struct AsyncCallPayload *>(paResult);
  bool failed = false;
  for (unsigned int i = 0; i < outputData->variantsSize; i++) {
    if (!mClientRdConverter[i]->set(outputData->variants[i].data,
        &getCommFB()->getRDs()[i])) {
      DEVLOG_ERROR(
          "OPC UA: Client could not convert returned data from OPC UA to RD_%d.\n",
          i + 1);
      failed = true;
      break;
    }
  }
  UA_Array_delete(outputData->variants, outputData->variantsSize,
      &UA_TYPES[UA_TYPES_VARIANT]);
  forte_free(paResult);
  if (!failed) {
    mInterruptResp = e_ProcessDataOk;
  }

}

void COPC_UA_Layer::handleAsyncEvent() {
  getCommFB()->interruptCommFB(this);
  getExtEvHandler<COPC_UA_Handler>().forceEventHandling(this);
}
