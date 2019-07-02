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
 *    Jose Cabral:
 *      - refactoring to cleaner architecture
 *******************************************************************************/

#include "../../core/devexec.h"
#include "../../core/iec61131_functions.h"
#include "../../core/cominfra/basecommfb.h"
#include "../../core/utils/parameterParser.h"
#include "../../core/utils/string_utils.h"
#include <criticalregion.h>
#include <forte_printer.h>
#include "../../arch/utils/mainparam_utils.h"
#include "opcua_local_handler.h"

#ifndef FORTE_COM_OPC_UA_CUSTOM_HOSTNAME
#include <sockhand.h>
#endif

const char* const COPC_UA_Local_Handler::mLocaleForNodes = "en-US";
const char* const COPC_UA_Local_Handler::mDescriptionForVariableNodes = "Digital port of Function Block";
CSinglyLinkedList<COPC_UA_Local_Handler::UA_ParentNodeHandler *> COPC_UA_Local_Handler::methodsWithoutContext = CSinglyLinkedList<
    COPC_UA_Local_Handler::UA_ParentNodeHandler *>();

TForteUInt16 gOpcuaServerPort = FORTE_COM_OPC_UA_PORT;

using namespace forte::com_infra;

DEFINE_HANDLER(COPC_UA_Local_Handler);

void COPC_UA_Local_Handler::configureUAServer(TForteUInt16 paUAServerPort, UA_ServerConfig **paUaServerConfig) {

  char helperBuffer[scmMaxServerNameLength + 1];
  forte_snprintf(helperBuffer, scmMaxServerNameLength, "forte_%d", paUAServerPort);

  (*paUaServerConfig)->logger = COPC_UA_HandlerAbstract::getLogger();

#ifdef FORTE_COM_OPC_UA_MULTICAST
  (*paUaServerConfig)->applicationDescription.applicationType = UA_APPLICATIONTYPE_DISCOVERYSERVER;
  // hostname will be added by mdns library
# ifdef FORTE_COM_OPC_UA_MASTER_BRANCH
  UA_String_deleteMembers(&(*paUaServerConfig)->discovery.mdns.mdnsServerName);
  (*paUaServerConfig)->discovery.mdns.mdnsServerName = UA_String_fromChars(helperBuffer);
# else //FORTE_COM_OPC_UA_MASTER_BRANCH
  UA_String_deleteMembers(&(*paUaServerConfig)->mdnsServerName);
  (*paUaServerConfig)->mdnsServerName = UA_String_fromChars(helperBuffer);
# endif//FORTE_COM_OPC_UA_MASTER_BRANCH
#endif //FORTE_COM_OPC_UA_MULTICAST

  char hostname[scmMaxServerNameLength + 1];
#ifdef FORTE_COM_OPC_UA_CUSTOM_HOSTNAME
  forte_snprintf(hostname, scmMaxServerNameLength, "%s-%s", FORTE_COM_OPC_UA_CUSTOM_HOSTNAME, helperBuffer);
#else
  if(gethostname(hostname, scmMaxServerNameLength) == 0) {
    size_t offset = strlen(hostname);
    size_t nameLen = strlen(helperBuffer);
    if(offset + nameLen + 1 > scmMaxServerNameLength) {
      offset = MAX(scmMaxServerNameLength - nameLen - 1, (size_t) 0);
    }
    forte_snprintf(hostname + offset, scmMaxServerNameLength - offset, "-%s", helperBuffer);
  }
#endif

  forte_snprintf(helperBuffer, scmMaxServerNameLength, "org.eclipse.4diac.%s", hostname);

  // delete pre-initialized values
  UA_String_deleteMembers(&(*paUaServerConfig)->applicationDescription.applicationUri);
  UA_LocalizedText_deleteMembers(&(*paUaServerConfig)->applicationDescription.applicationName);

  (*paUaServerConfig)->applicationDescription.applicationUri = UA_String_fromChars(helperBuffer);
  (*paUaServerConfig)->applicationDescription.applicationName.locale = UA_STRING_NULL;
  (*paUaServerConfig)->applicationDescription.applicationName.text = UA_String_fromChars(hostname);

  for(size_t i = 0; i < (*paUaServerConfig)->endpointsSize; i++) {
#ifdef FORTE_COM_OPC_UA_MASTER_BRANCH
    UA_String_deleteMembers(&(*paUaServerConfig)->endpoints[i].server.applicationUri);
    UA_LocalizedText_deleteMembers(&(*paUaServerConfig)->endpoints[i].server.applicationName);
    UA_String_copy(&(*paUaServerConfig)->applicationDescription.applicationUri, &(*paUaServerConfig)->endpoints[i].server.applicationUri);
    UA_LocalizedText_copy(&(*paUaServerConfig)->applicationDescription.applicationName, &(*paUaServerConfig)->endpoints[i].server.applicationName);
#else //FORTE_COM_OPC_UA_MASTER_BRANCH
    UA_String_deleteMembers(&(*paUaServerConfig)->endpoints[i].endpointDescription.server.applicationUri);
    UA_LocalizedText_deleteMembers(&(*paUaServerConfig)->endpoints[i].endpointDescription.server.applicationName);
    UA_String_copy(&(*paUaServerConfig)->applicationDescription.applicationUri, &(*paUaServerConfig)->endpoints[i].endpointDescription.server.applicationUri);
    UA_LocalizedText_copy(&(*paUaServerConfig)->applicationDescription.applicationName,
      &(*paUaServerConfig)->endpoints[i].endpointDescription.server.applicationName);
#endif //FORTE_COM_OPC_UA_MASTER_BRANCH
  }
}

#ifdef FORTE_COM_OPC_UA_MULTICAST

const UA_String* COPC_UA_Local_Handler::getDiscoveryUrl() const {

#ifdef FORTE_COM_OPC_UA_MASTER_BRANCH
  UA_ServerConfig* mServerConfig = UA_Server_getConfig(mUaServer); //change mServerConfig to serverConfig when only master branch is present
#else
  //do nothing
#endif
  if(0 == mServerConfig->networkLayersSize) {
    return 0;
  }
  return &mServerConfig->networkLayers[0].discoveryUrl;
}

void COPC_UA_Local_Handler::serverOnNetworkCallback(const UA_ServerOnNetwork *paServerOnNetwork, UA_Boolean paIsServerAnnounce, UA_Boolean paIsTxtReceived,
    void* paData) {
  COPC_UA_Local_Handler* handler = static_cast<COPC_UA_Local_Handler*>(paData);

  const UA_String* ownDiscoverUrl = handler->getDiscoveryUrl();

  if(!ownDiscoverUrl || UA_String_equal(&paServerOnNetwork->discoveryUrl, ownDiscoverUrl)) {
    // skip self
    return;
  }

  if(!paIsTxtReceived) {
    return; // we wait until the corresponding TXT record is announced.
  }

  DEVLOG_DEBUG("[OPC UA LOCAL]: mDNS %s '%.*s' with url '%.*s'\n", paIsServerAnnounce ? "announce" : "remove", paServerOnNetwork->serverName.length,
    paServerOnNetwork->serverName.data, paServerOnNetwork->discoveryUrl.length, paServerOnNetwork->discoveryUrl.data);

  // check if server is LDS, and then register
  UA_String ldsStr = UA_String_fromChars("LDS");
  for(unsigned int i = 0; i < paServerOnNetwork->serverCapabilitiesSize; i++) {
    if(UA_String_equal(&paServerOnNetwork->serverCapabilities[i], &ldsStr)) {
      if(paIsServerAnnounce) {
        handler->registerWithLds(&paServerOnNetwork->discoveryUrl);
      } else {
        handler->removeLdsRegister(&paServerOnNetwork->discoveryUrl);
      }
      break;
    }
  }
  UA_String_deleteMembers(&ldsStr);
}

void COPC_UA_Local_Handler::registerWithLds(const UA_String *paDiscoveryUrl) {
  // check if already registered with the given LDS
  for(CSinglyLinkedList<UA_String*>::Iterator iter = registeredWithLds.begin(); iter != registeredWithLds.end(); ++iter) {
    if(UA_String_equal(paDiscoveryUrl, *iter)) {
      return;
    }
  }

  // will be freed when removed from list
  UA_String* discoveryUrlChar = 0;
  UA_String_copy(paDiscoveryUrl, discoveryUrlChar);

  registeredWithLds.pushFront(discoveryUrlChar);
  DEVLOG_INFO("[OPC UA LOCAL]: Registering with LDS '%.*s'\n", paDiscoveryUrl->length, paDiscoveryUrl->data);
  UA_StatusCode retVal = UA_Server_addPeriodicServerRegisterCallback(mUaServer,
#ifdef FORTE_COM_OPC_UA_MASTER_BRANCH
    0,
#else //FORTE_COM_OPC_UA_MASTER_BRANCH
    //nothing here
#endif //FORTE_COM_OPC_UA_MASTER_BRANCH
    reinterpret_cast<const char*>(discoveryUrlChar->data), 10 * 60 * 1000, 500, 0);
  if( UA_STATUSCODE_GOOD != retVal) {
    DEVLOG_ERROR("[OPC UA LOCAL]: Could not register with LDS. Error: %s\n", UA_StatusCode_name(retVal));
  }
}

void COPC_UA_Local_Handler::removeLdsRegister(const UA_String *paDiscoveryUrl) {
  UA_String* toDelete = 0;
  for(CSinglyLinkedList<UA_String*>::Iterator iter = registeredWithLds.begin(); iter != registeredWithLds.end(); ++iter) {
    if(UA_String_equal(paDiscoveryUrl, *iter)) {
      toDelete = *iter;
      break;
    }
  }
  if(toDelete) {
    registeredWithLds.erase(toDelete);
    UA_String_delete(toDelete);
  }
}

#endif //FORTE_COM_OPC_UA_MULTICAST

COPC_UA_Local_Handler::COPC_UA_Local_Handler(CDeviceExecution& paDeviceExecution) :
    COPC_UA_HandlerAbstract(paDeviceExecution), mUaServer(0), mUaServerRunningFlag(UA_FALSE), nodeCallbackHandles()
#ifdef FORTE_COM_OPC_UA_MULTICAST
# ifdef FORTE_COM_OPC_UA_MASTER_BRANCH
        //do nothing
# else //FORTE_COM_OPC_UA_MASTER_BRANCH
        , mServerConfig(0)
# endif //FORTE_COM_OPC_UA_MASTER_BRANCH
#endif //FORTE_COM_OPC_UA_MULTICAST
{
}

COPC_UA_Local_Handler::~COPC_UA_Local_Handler() {
  stopServer();

  for(CSinglyLinkedList<struct UA_NodeCallback_Handle *>::Iterator iter = nodeCallbackHandles.begin(); iter != nodeCallbackHandles.end(); ++iter) {
    delete *iter;
  }
  nodeCallbackHandles.clearAll();
  cleanNodeReferences();

  for(CSinglyLinkedList<UA_ParentNodeHandler *>::Iterator iter = methodsWithoutContext.begin(); iter != methodsWithoutContext.end(); ++iter) {
    UA_NodeId_delete((*iter)->mParentNodeId);
    delete *iter;
  }

  cleanMethodCalls();

#ifdef FORTE_COM_OPC_UA_MULTICAST
  for(CSinglyLinkedList<UA_String*>::Iterator iter = registeredWithLds.begin(); iter != registeredWithLds.end(); ++iter) {
    UA_String_delete(*iter);
  }
  registeredWithLds.clearAll();
#endif //FORTE_COM_OPC_UA_MULTICAST
}

void COPC_UA_Local_Handler::cleanNodeReferences() {
  CCriticalRegion criticalRegion(mNodeLayerMutex);
  for(CSinglyLinkedList<struct nodesReferencedByActions *>::Iterator iter = mNodesReferences.begin(); iter != mNodesReferences.end(); ++iter) {
    UA_NodeId_delete(const_cast<UA_NodeId*>((*iter)->mNodeId));
    delete *iter;
  }
  mNodesReferences.clearAll();
}

void COPC_UA_Local_Handler::run() {
  DEVLOG_INFO("[OPC UA LOCAL]: Starting OPC UA Server: opc.tcp://localhost:%d\n", gOpcuaServerPort);

  UA_ServerConfig *uaServerConfig = 0;
#ifdef FORTE_COM_OPC_UA_MASTER_BRANCH
  mUaServer = UA_Server_new();
  if(mUaServer) {
    uaServerConfig = UA_Server_getConfig(mUaServer);
    UA_ServerConfig_setDefault(uaServerConfig);
    configureUAServer(gOpcuaServerPort, &uaServerConfig);
#else
  uaServerConfig = UA_ServerConfig_new_minimal(gOpcuaServerPort, 0);
  configureUAServer(gOpcuaServerPort, &uaServerConfig);
  mUaServer = UA_Server_new(uaServerConfig);
  if(mUaServer) {
#endif

    if(initializeNodesets(mUaServer)) {
#ifdef FORTE_COM_OPC_UA_MULTICAST
# ifdef FORTE_COM_OPC_UA_MASTER_BRANCH
      //do nothing
# else
      mServerConfig = uaServerConfig;
# endif

      UA_Server_setServerOnNetworkCallback(mUaServer, serverOnNetworkCallback, this);
#endif //FORTE_COM_OPC_UA_MULTICAST
      mUaServerRunningFlag = UA_TRUE;
      mServerStarted.inc();
      UA_StatusCode retVal = UA_Server_run(mUaServer, &mUaServerRunningFlag); // server keeps iterating as long as running is true
      if(retVal != UA_STATUSCODE_GOOD) {
        DEVLOG_ERROR("[OPC UA LOCAL]: Server exited with error: %s\n", UA_StatusCode_name(retVal));
      } else {
        DEVLOG_INFO("[OPC UA LOCAL]: Server successfully stopped\n");
      }
    } else {
      DEVLOG_ERROR("[OPC UA LOCAL]: Couldn't initialize Nodesets\n", gOpcuaServerPort);
    }
    UA_Server_delete(mUaServer);
    mUaServer = 0;
  }
#ifdef FORTE_COM_OPC_UA_MASTER_BRANCH
  //nothing to do in master branch. Config is deleted with the server
#else
  UA_ServerConfig_delete(uaServerConfig);
#endif
  mServerStarted.inc(); //this will avoid locking startServer() for all cases where the starting of server failed
}

void COPC_UA_Local_Handler::enableHandler(void) {
  startServer();
}

void COPC_UA_Local_Handler::disableHandler(void) {
  COPC_UA_Local_Handler::stopServer();
  end();
}

void COPC_UA_Local_Handler::startServer() {
  if(!isAlive()) {
    start();
    mServerStarted.waitIndefinitely();
  }
}

void COPC_UA_Local_Handler::stopServer() {
  mUaServerRunningFlag = UA_FALSE;
  end();
}

UA_StatusCode COPC_UA_Local_Handler::updateNodeUserAccessLevel(const UA_NodeId *paNodeId, UA_Byte paNewAccessLevel) {
  UA_StatusCode retVal = UA_Server_writeAccessLevel(mUaServer, *paNodeId, paNewAccessLevel);
  if(UA_STATUSCODE_GOOD != retVal) {
    DEVLOG_WARNING("[OPC UA LOCAL]: Cannot set write permission of node\n");
  }
  return retVal;
}

UA_StatusCode COPC_UA_Local_Handler::updateNodeValue(const UA_NodeId *paNodeId, const CIEC_ANY *paData, const UA_TypeConvert *paConvert) {
  UA_Variant *nodeValue = UA_Variant_new();
  UA_Variant_init(nodeValue);

  void *varValue = UA_new(paConvert->type);
  if(!paConvert->get(paData, varValue)) {
    UA_delete(varValue, paConvert->type);
    return UA_STATUSCODE_BADUNEXPECTEDERROR;
  }

  UA_Variant_setScalarCopy(nodeValue, varValue, paConvert->type);
  UA_StatusCode retVal = UA_Server_writeValue(mUaServer, *paNodeId, *nodeValue);
  UA_delete(varValue, paConvert->type);
  UA_Variant_delete(nodeValue);
  return retVal;
}

UA_StatusCode COPC_UA_Local_Handler::registerVariableCallBack(const UA_NodeId *paNodeId, COPC_UA_Layer *paComLayer, const struct UA_TypeConvert *paConvert,
    size_t paPortIndex) {
  // needs new, otherwise it will be removed as soon as registerNodecallBack exits, and thus handle is not valid in the callback
  UA_VariableCallback_Handle *handle = new UA_VariableCallback_Handle();

  handle->mConvert = paConvert;
  handle->mLayer = paComLayer;
  handle->mPortIndex = paPortIndex;
  // store it in the list so we can delete it to avoid mem leaks
  nodeCallbackHandles.pushBack(handle);
  UA_Server_setNodeContext(mUaServer, *paNodeId, handle);
  return UA_Server_setVariableNode_valueCallback(mUaServer, *paNodeId, { 0, this->onWrite });
}

void COPC_UA_Local_Handler::onWrite(UA_Server *, const UA_NodeId *, void *, const UA_NodeId *, void *nodeContext, const UA_NumericRange *,
    const UA_DataValue *data) { //NOSONAR

  struct UA_VariableCallback_Handle *handle = static_cast<struct UA_VariableCallback_Handle *>(nodeContext);

  UA_RecvVariable_handle handleRecv(1);

  handleRecv.mData[0] = data->hasValue ? &data->value : 0; //TODO: check this empty data
  handleRecv.mOffset = handle->mPortIndex;
  handleRecv.mConvert[0] = handle->mConvert;

  EComResponse retVal = handle->mLayer->recvData(static_cast<const void *>(&handleRecv), 0); //TODO: add multidimensional mData handling with 'range'.

  if(e_Nothing != retVal) {
    handle->mLayer->getCommFB()->interruptCommFB(handle->mLayer);
    ::getExtEvHandler<COPC_UA_Local_Handler>(*handle->mLayer->getCommFB()).startNewEventChain(handle->mLayer->getCommFB());
  }
}

UA_StatusCode COPC_UA_Local_Handler::executeCreateMethod(COPC_UA_HandlerAbstract::CActionInfo& paInfo) {
  //This is the return of a local method call, when RSP is triggered

  UA_StatusCode retVal = UA_STATUSCODE_BADUNEXPECTEDERROR;
  CLocalMethodCall* call = getLocalMethodCall(static_cast<COPC_UA_HandlerAbstract::CLocalMethodInfo*>(&paInfo));
  if(call) {
    bool somethingFailed = false;
    // copy SD values to output
    for(size_t i = 0; i < call->mSendHandle->mSize; i++) {

      void *varValue = UA_new(call->mSendHandle->mConvert[i]->type);
      if(!call->mSendHandle->mConvert[i]->get(&paInfo.getLayer()->getCommFB()->getSDs()[i], varValue)) {
        DEVLOG_ERROR("[OPC UA LOCAL]: can not convert forte type to method outputArgument at idx %d of FB %s\n", i,
          paInfo.getLayer()->getCommFB()->getInstanceName());
        somethingFailed = true;
      } else {
        UA_Variant_setScalarCopy(call->mSendHandle->mData[i], varValue, call->mSendHandle->mConvert[i]->type);
        call->mSendHandle->mData[i]->storageType = UA_VARIANT_DATA;
      }

      UA_delete(varValue, call->mSendHandle->mConvert[i]->type);

      if(somethingFailed) {
        break;
      }
    }

    if(!somethingFailed) {
      retVal = UA_STATUSCODE_GOOD;
    }
    call->mSendHandle->mFailed = somethingFailed;
    call->mActionInfo->getResultReady().inc();
  } else {
    DEVLOG_ERROR("[OPC UA LOCAL]: The method being returned hasn't been called before of FB %s\n", paInfo.getLayer()->getCommFB()->getInstanceName());
  }

  return retVal;
}

UA_StatusCode COPC_UA_Local_Handler::onServerMethodCall(UA_Server *, const UA_NodeId *, void *, const UA_NodeId * paMethodNodeId, void *paMethodContext, //NOSONAR
    const UA_NodeId *paParentNodeId, void *, //NOSONAR
    size_t paInputSize, const UA_Variant *paInput, size_t paOutputSize, UA_Variant *paOutput) {

  UA_StatusCode retVal = UA_STATUSCODE_BADUNEXPECTEDERROR;
  struct COPC_UA_HandlerAbstract::CLocalMethodInfo *handle = 0;
  if(paMethodContext) {
    handle = static_cast<struct COPC_UA_HandlerAbstract::CLocalMethodInfo *>(paMethodContext);
  } else { //methods without context are the ones craeted with CreateObject. The "context" are stored in the list
    for(CSinglyLinkedList<UA_ParentNodeHandler *>::Iterator iter = methodsWithoutContext.begin(); iter != methodsWithoutContext.end(); ++iter) {
      if(UA_NodeId_equal(paParentNodeId, (*iter)->mParentNodeId) && UA_NodeId_equal(paMethodNodeId, (*iter)->mMethodNodeId)) {
        handle = (*iter)->mActionInfo;
        break;
      }
    }
  }

  if(!handle) {
    DEVLOG_ERROR("[OPC UA LOCAL]: Method doesn't have any FB referencing it\n");
  } else {
    if(paInputSize != handle->mLayer->getCommFB()->getNumRD() || paOutputSize != handle->mLayer->getCommFB()->getNumSD()) {
      DEVLOG_ERROR("[OPC UA LOCAL]: method call got invalid number of arguments. In: %d==%d, Out: %d==%d\n", handle->mLayer->getCommFB()->getNumRD(),
        paInputSize, handle->mLayer->getCommFB()->getNumSD(), paOutput);
    } else {
      // other thread may currently create nodes for the same path, thus mutex
      CCriticalRegion criticalRegion(handle->getMutex()); //TODO: do we need this mutex?

      UA_SendVariable_handle sendHandle(paOutputSize);
      UA_RecvVariable_handle recvHandle(paInputSize);

      for(size_t i = 0; i < paInputSize; i++) {
        recvHandle.mData[i] = &paInput[i];
      }

      for(size_t i = 0; i < paOutputSize; i++) {
        sendHandle.mData[i] = &paOutput[i];
      }

      CSinglyLinkedList<UA_TypeConvert*>::Iterator itType = handle->mLayer->getTypeConveverters().begin();
      for(size_t i = 0; i < paOutputSize; i++, ++itType) {
        sendHandle.mConvert[i] = *itType;
      }

      for(size_t i = 0; i < paInputSize; i++, ++itType) {
        recvHandle.mConvert[i] = *itType;
      }

      /* Handle return of receive mData */
      if(e_ProcessDataOk == handle->mLayer->recvData(static_cast<const void *>(&recvHandle), 0)) { //TODO: add multidimensional mData handling with 'range'.

        handle->mLayer->getCommFB()->interruptCommFB(handle->mLayer);

        //when the method finishes, and RSP is triggered, sendHandle will be filled with the right information
        CLocalMethodCall* call = ::getExtEvHandler<COPC_UA_Local_Handler>(*handle->mLayer->getCommFB()).addMethodCall(handle, &sendHandle);

        ::getExtEvHandler<COPC_UA_Local_Handler>(*handle->mLayer->getCommFB()).startNewEventChain(handle->mLayer->getCommFB());

        //wait For semaphore, which will be released by execute Local Method in this handler
        if(!handle->getResultReady().timedWait(scmMethodCallTimeout * 1E9)) {
          DEVLOG_ERROR("[OPC UA LOCAL]: method call did not get result values within timeout of %d seconds.\n", scmMethodCallTimeout);
          retVal = UA_STATUSCODE_BADTIMEOUT;
        } else {
          retVal = sendHandle.mFailed ? UA_STATUSCODE_BADUNEXPECTEDERROR : UA_STATUSCODE_GOOD;
        }

        ::getExtEvHandler<COPC_UA_Local_Handler>(*handle->mLayer->getCommFB()).removeMethodCall(call);
      }
    }
  }
  return retVal;
}

void COPC_UA_Local_Handler::referencedNodesIncrement(const CSinglyLinkedList<UA_NodeId *> *paNodes, COPC_UA_HandlerAbstract::CActionInfo * const paActionInfo) {
  CCriticalRegion criticalRegion(mNodeLayerMutex);
  for(CSinglyLinkedList<UA_NodeId *>::Iterator iterNode = paNodes->begin(); iterNode != paNodes->end(); ++iterNode) {
    bool found = false;
    for(CSinglyLinkedList<struct nodesReferencedByActions *>::Iterator iterRef = mNodesReferences.begin(); iterRef != mNodesReferences.end(); ++iterRef) {
      if(UA_NodeId_equal((*iterRef)->mNodeId, (*iterNode))) {
        found = true;
        (*iterRef)->mActionsReferencingIt.pushFront(paActionInfo);
        break;
      }
    }
    if(!found) {
      nodesReferencedByActions *newRef = new nodesReferencedByActions();
      UA_NodeId *newNode = UA_NodeId_new();
      UA_NodeId_copy((*iterNode), newNode);
      newRef->mNodeId = newNode;
      newRef->mActionsReferencingIt = CSinglyLinkedList<COPC_UA_HandlerAbstract::CActionInfo*>();
      newRef->mActionsReferencingIt.pushFront(paActionInfo);
      mNodesReferences.pushFront(newRef);
    }
  }
}

void COPC_UA_Local_Handler::referencedNodesDecrement(const COPC_UA_HandlerAbstract::CActionInfo *paActionInfo) {
  CCriticalRegion criticalRegion(mNodeLayerMutex);
  CSinglyLinkedList<const UA_NodeId *> nodesReferencedByAction;
  getNodesReferencedByAction(paActionInfo, nodesReferencedByAction);

  for(CSinglyLinkedList<const UA_NodeId *>::Iterator iterNode = nodesReferencedByAction.begin(); iterNode != nodesReferencedByAction.end(); ++iterNode) {
    CSinglyLinkedList<struct nodesReferencedByActions *>::Iterator nodeReferencedToDelete = mNodesReferences.end();
    for(CSinglyLinkedList<struct nodesReferencedByActions *>::Iterator iterRef = mNodesReferences.begin(); iterRef != mNodesReferences.end(); ++iterRef) {
      if(UA_NodeId_equal((*iterRef)->mNodeId, (*iterNode))) {

        bool stillSomethingThere = true;
        while(stillSomethingThere) {
          CSinglyLinkedList<COPC_UA_HandlerAbstract::CActionInfo *>::Iterator iterToDelete = (*iterRef)->mActionsReferencingIt.end();
          for(CSinglyLinkedList<COPC_UA_HandlerAbstract::CActionInfo *>::Iterator iterLayer = (*iterRef)->mActionsReferencingIt.begin();
              iterLayer != (*iterRef)->mActionsReferencingIt.end(); ++iterLayer) {
            if((*iterLayer) == paActionInfo) {
              iterToDelete = iterLayer;
              break;
            }
          }

          if((*iterRef)->mActionsReferencingIt.end() == iterToDelete) {
            stillSomethingThere = false;
          } else {
            (*iterRef)->mActionsReferencingIt.erase(*iterToDelete);
          }
        }

        if((*iterRef)->mActionsReferencingIt.isEmpty()) {
          nodeReferencedToDelete = iterRef;
          if(0 != (*iterRef)->mNodeId->namespaceIndex && mUaServer) {
            UA_Server_deleteNode(mUaServer, *(*iterRef)->mNodeId, UA_TRUE);
          }
        }
        break;
      }
    }
    if(mNodesReferences.end() != nodeReferencedToDelete) {
      nodesReferencedByActions *toDelete = *nodeReferencedToDelete;
      mNodesReferences.erase(toDelete);
      UA_NodeId_delete(const_cast<UA_NodeId*>(toDelete->mNodeId));
      delete toDelete;
    }
  }
}

void COPC_UA_Local_Handler::getNodesReferencedByAction(const COPC_UA_HandlerAbstract::CActionInfo *paActionInfo,
    CSinglyLinkedList<const UA_NodeId *>& paNodes) {
  for(CSinglyLinkedList<nodesReferencedByActions *>::Iterator iterRef = mNodesReferences.begin(); iterRef != mNodesReferences.end(); ++iterRef) {
    for(CSinglyLinkedList<COPC_UA_HandlerAbstract::CActionInfo *>::Iterator iterAction = (*iterRef)->mActionsReferencingIt.begin();
        iterAction != (*iterRef)->mActionsReferencingIt.end(); ++iterAction) {
      if((*iterAction) == paActionInfo) {
        paNodes.pushFront((*iterRef)->mNodeId);
        break;
      }
    }
  }
}

UA_StatusCode COPC_UA_Local_Handler::storeAlreadyExistingNodes(UA_BrowsePathResult* paBrowsePathsResults, size_t paFolderCnt, size_t* paFirstNonExistingNode,
    CSinglyLinkedList<UA_NodeId*>& paCreatedNodeIds) {
  size_t foundFolderOffset = 0;
  int retVal; // no unsigned, because we need to check for -1
  bool offsetFound = false;
  for(retVal = static_cast<int>(paFolderCnt) - 1; retVal >= 0; retVal--) {
    if(UA_STATUSCODE_GOOD == paBrowsePathsResults[retVal].statusCode) { // find first existing node. Check for isInverse = TRUE
      foundFolderOffset = 0;
      offsetFound = true;
    } else if(UA_STATUSCODE_GOOD == paBrowsePathsResults[paFolderCnt + retVal].statusCode) { // and for isInverse = FALSE
      foundFolderOffset = paFolderCnt;
      offsetFound = true;
    }

    if(offsetFound) {
      break;
    }
  }

  if(-1 != retVal) {
    if(paBrowsePathsResults[foundFolderOffset + retVal].targetsSize == 0) {
      DEVLOG_ERROR("[OPC UA LOCAL]: Could not translate browse paths to node IDs. Target size is 0.\n");
      return UA_STATUSCODE_BADUNEXPECTEDERROR;
    } else {
      if(paBrowsePathsResults[foundFolderOffset + retVal].targetsSize > 1) {
        DEVLOG_WARNING("[OPC UA LOCAL]: The given browse path has multiple results for the same path. Taking the first result.\n");
      }
    }
  }
  retVal++;

  for(int j = 0; j < retVal; j++) {
    UA_NodeId *tmp = UA_NodeId_new();
    UA_NodeId_copy(&paBrowsePathsResults[foundFolderOffset + j].targets[0].targetId.nodeId, tmp);
    paCreatedNodeIds.pushBack(tmp);
  }

  *paFirstNonExistingNode = static_cast<size_t>(retVal);
  return UA_STATUSCODE_GOOD;
}

void COPC_UA_Local_Handler::createMethodArguments(createMethodInfo * paMethodInformation, COPC_UA_HandlerAbstract::CActionInfo & paInfo,
    CSinglyLinkedList<CIEC_STRING> &paNames) {

  size_t noOfSDs = paInfo.getLayer()->getCommFB()->getNumSD();
  size_t counterHelper = 0;
  for(CSinglyLinkedList<UA_TypeConvert*>::Iterator it = paInfo.getLayer()->getTypeConveverters().begin(); it != paInfo.getLayer()->getTypeConveverters().end();
      ++it) {
    counterHelper++; //get the total number of arguments
  }

  paMethodInformation->mOutputArguments = static_cast<UA_Argument *>(UA_Array_new(noOfSDs, &UA_TYPES[UA_TYPES_ARGUMENT]));
  paMethodInformation->mInputArguments = static_cast<UA_Argument *>(UA_Array_new(counterHelper - noOfSDs, &UA_TYPES[UA_TYPES_ARGUMENT]));

  counterHelper = 0; //use to count the current amount of outputs
  CSinglyLinkedList<CIEC_STRING>::Iterator itName = paNames.begin();
  for(CSinglyLinkedList<UA_TypeConvert*>::Iterator it = paInfo.getLayer()->getTypeConveverters().begin(); it != paInfo.getLayer()->getTypeConveverters().end();
      ++it, ++itName, counterHelper++) {
    UA_Argument *arg;
    if(counterHelper < noOfSDs) {
      arg = &(paMethodInformation->mOutputArguments)[counterHelper];
    } else {
      arg = &(paMethodInformation->mInputArguments)[counterHelper - noOfSDs];
    }

    UA_Argument_init(arg);

    arg->arrayDimensionsSize = 0;
    arg->arrayDimensions = 0;
    arg->dataType = (*it)->type->typeId;
    arg->description = UA_LOCALIZEDTEXT_ALLOC("en-US", "Method parameter"); //TODO: this shouldn't be the same for all
    arg->name = UA_STRING_ALLOC((*itName).getValue());
    arg->valueRank = -1;
  }
}

UA_StatusCode COPC_UA_Local_Handler::createMethodNode(createMethodInfo* paMethodInfo) {

  UA_NodeId requestedNodeId;
  if(paMethodInfo->mRequestedNodeId) {
    UA_NodeId_copy(paMethodInfo->mRequestedNodeId, &requestedNodeId);
  } else {
    requestedNodeId = UA_NODEID_NUMERIC(paMethodInfo->mBrowseName->namespaceIndex, 0); //if not provided, let the server generate the NodeId using the namespace from the browsename
  }

  UA_NodeId parentNodeId;
  if(paMethodInfo->mParentNodeId) {
    UA_NodeId_copy(paMethodInfo->mParentNodeId, &parentNodeId);
  } else {
    parentNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_ROOTFOLDER);
    DEVLOG_WARNING(
      "[OPC UA LOCAL]: You are creating a Method %.*s in the /Root folder. This is not a good practice. Try to create them in the /Root/Objects folder\n",
      paMethodInfo->mBrowseName->name.length, reinterpret_cast<const char*>(paMethodInfo->mBrowseName->name.data));
  }

  UA_MethodAttributes methodAttributes;
  UA_MethodAttributes_init(&methodAttributes);
  methodAttributes.description = UA_LOCALIZEDTEXT_ALLOC("en-US", "Method which can be called");
  methodAttributes.executable = true;
  methodAttributes.userExecutable = true;
  methodAttributes.displayName.locale = UA_STRING_ALLOC(mLocaleForNodes);
  UA_String_copy(&paMethodInfo->mBrowseName->name, &methodAttributes.displayName.text);

  UA_StatusCode retVal = UA_Server_addMethodNode(mUaServer, requestedNodeId, parentNodeId, UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
    *paMethodInfo->mBrowseName,
    methodAttributes,
    COPC_UA_Local_Handler::onServerMethodCall, paMethodInfo->mInputSize, paMethodInfo->mInputArguments, paMethodInfo->mOutputSize,
    paMethodInfo->mOutputArguments, paMethodInfo->mCallback, paMethodInfo->mReturnedNodeId);

  UA_NodeId_deleteMembers(&parentNodeId);
  UA_NodeId_deleteMembers(&requestedNodeId);
  UA_MethodAttributes_deleteMembers(&methodAttributes);
  return retVal;
}

UA_StatusCode COPC_UA_Local_Handler::createVariableNode(createVariableInfo* paNodeInformation) {

  UA_NodeId requestedNodeId;
  if(paNodeInformation->mRequestedNodeId) {
    UA_NodeId_copy(paNodeInformation->mRequestedNodeId, &requestedNodeId);
  } else {
    requestedNodeId = UA_NODEID_NUMERIC(paNodeInformation->mBrowseName->namespaceIndex, 0); //if not provided, let the server generate the NodeId using the namespace from the browsename
  }
  UA_NodeId parentNodeId;
  if(paNodeInformation->mParentNodeId) {
    UA_NodeId_copy(paNodeInformation->mParentNodeId, &parentNodeId);
  } else {
    parentNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_ROOTFOLDER);
    DEVLOG_WARNING(
      "[OPC UA LOCAL]: You are creating a variable %.*s in the /Root folder. This is not a good practice. Try to create them in the /Root/Objects folder\n",
      paNodeInformation->mBrowseName->name.length,
      reinterpret_cast<const char*>(paNodeInformation->mBrowseName->name.data));
  }

  void *paVarValue = UA_new(paNodeInformation->mTypeConvert->type);

  UA_init(paVarValue, paNodeInformation->mTypeConvert->type);
  if(!paNodeInformation->mTypeConvert->get(paNodeInformation->mInitData, paVarValue)) {
    DEVLOG_WARNING("[OPC UA LOCAL]: Cannot convert initial value for variable %.*s", paNodeInformation->mBrowseName->name.length,
      reinterpret_cast<const char*>(paNodeInformation->mBrowseName->name.data));
  }

  // create variable attributes
  UA_VariableAttributes variableAttributes;
  UA_VariableAttributes_init(&variableAttributes);
  variableAttributes.dataType = paNodeInformation->mTypeConvert->type->typeId;
  variableAttributes.valueRank = -1; /* value is a scalar */
  variableAttributes.displayName.locale = UA_STRING_ALLOC(mLocaleForNodes);
  UA_String_copy(&paNodeInformation->mBrowseName->name, &variableAttributes.displayName.text);
  variableAttributes.description = UA_LOCALIZEDTEXT_ALLOC(mLocaleForNodes, mDescriptionForVariableNodes);
  variableAttributes.userAccessLevel = UA_ACCESSLEVELMASK_READ;
  if(paNodeInformation->mAllowWrite) {
    variableAttributes.userAccessLevel |= UA_ACCESSLEVELMASK_WRITE;
  }
  variableAttributes.accessLevel = variableAttributes.userAccessLevel;
  UA_Variant_setScalar(&variableAttributes.value, paVarValue, paNodeInformation->mTypeConvert->type);

  UA_StatusCode retVal = UA_Server_addVariableNode(mUaServer, // server
    requestedNodeId, // requestedNewNodeId
    parentNodeId, // parentNodeId
    UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT), // referenceTypeId   Reference to the type definition for the variable node
    *paNodeInformation->mBrowseName, // browseName
    UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE), // typeDefinition
    variableAttributes, // Variable attributes
    0, // instantiation callback
    paNodeInformation->mReturnedNodeId); // return Node Id

  if(UA_STATUSCODE_GOOD != retVal) {
    DEVLOG_ERROR("[OPC UA LOCAL]: AddressSpace adding Variable Node %s failed. Error: %s\n", paNodeInformation->mBrowseName->name.data,
      UA_StatusCode_name(retVal));
  }
  UA_NodeId_deleteMembers(&parentNodeId);
  UA_NodeId_deleteMembers(&requestedNodeId);
  UA_VariableAttributes_deleteMembers(&variableAttributes);
  return retVal;
}

UA_StatusCode COPC_UA_Local_Handler::createObjectNode(createObjectInfo* paNodeInformation) {

  UA_StatusCode retVal = UA_STATUSCODE_BADINTERNALERROR;

  UA_NodeId requestedNodeId;
  if(paNodeInformation->mRequestedNodeId) {
    UA_NodeId_copy(paNodeInformation->mRequestedNodeId, &requestedNodeId);
  } else {
    requestedNodeId = UA_NODEID_NUMERIC(paNodeInformation->mBrowseName->namespaceIndex, 0); //if not provided, let the server generate the NodeId using the namespace from the browsename
  }

  UA_NodeId parentNodeId;
  if(paNodeInformation->mParentNodeId) {
    UA_NodeId_copy(paNodeInformation->mParentNodeId, &parentNodeId);
  } else {
    parentNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_ROOTFOLDER);
    DEVLOG_WARNING(
      "[OPC UA LOCAL]: You are creating an Object %.*s in the /Root folder. This is not a good practice. Try to create them in the /Root/Objects folder\n",
      paNodeInformation->mBrowseName->name.length, reinterpret_cast<const char*>(paNodeInformation->mBrowseName->name.data));
  }

  CIEC_STRING nodeName;
  nodeName.assign(reinterpret_cast<const char*>(paNodeInformation->mBrowseName->name.data),
    static_cast<TForteUInt16>(paNodeInformation->mBrowseName->name.length));

  UA_ObjectAttributes oAttr;
  UA_ObjectAttributes_init(&oAttr);
  oAttr.description = UA_LOCALIZEDTEXT_ALLOC("", nodeName.getValue());
  oAttr.displayName = UA_LOCALIZEDTEXT_ALLOC("", nodeName.getValue());
  retVal = UA_Server_addObjectNode(mUaServer, requestedNodeId, parentNodeId, UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT), *paNodeInformation->mBrowseName,
    *paNodeInformation->mTypeNodeId, oAttr, 0, paNodeInformation->mReturnedNodeId);
  if(UA_STATUSCODE_GOOD != retVal) {
    DEVLOG_ERROR("[OPC UA LOCAL]: Could not addObjectNode. Error: %s\n", UA_StatusCode_name(retVal));
  }
  UA_NodeId_deleteMembers(&requestedNodeId);
  UA_ObjectAttributes_deleteMembers(&oAttr);

  return retVal;
}

UA_StatusCode COPC_UA_Local_Handler::translateBrowseNameAndStore(const char* paBrowsePath, UA_BrowsePath **paBrowsePaths, size_t *paFoldercount,
    size_t* paFirstNonExistingNode, CSinglyLinkedList<UA_NodeId*>& paFoundNodeIds) {

  UA_StatusCode retVal = COPC_UA_Helper::prepareBrowseArgument(paBrowsePath, paBrowsePaths, paFoldercount);

  if(UA_STATUSCODE_GOOD == retVal) {
    // other thread may currently create nodes for the same path, thus mutex
    CCriticalRegion criticalRegion(getNodeForPathMutex);
    UA_BrowsePathResult* browsePathsResults = 0;
    CSinglyLinkedList<UA_NodeId*> storedNodeIds;

    browsePathsResults = static_cast<UA_BrowsePathResult *>(UA_Array_new(*paFoldercount * 2, &UA_TYPES[UA_TYPES_BROWSEPATHRESULT]));
    for(unsigned int i = 0; i < *paFoldercount * 2; i++) {
      browsePathsResults[i] = UA_Server_translateBrowsePathToNodeIds(mUaServer, &(*paBrowsePaths)[i]);
    }
    retVal = storeAlreadyExistingNodes(browsePathsResults, *paFoldercount, paFirstNonExistingNode, storedNodeIds);

    for(CSinglyLinkedList<UA_NodeId*>::Iterator it = storedNodeIds.begin(); it != storedNodeIds.end(); ++it) {
      if(UA_STATUSCODE_GOOD != retVal) {
        UA_NodeId_delete(*it);
      } else {
        paFoundNodeIds.pushBack(*it);
      }
    }

    UA_Array_delete(browsePathsResults, *paFoldercount * 2, &UA_TYPES[UA_TYPES_BROWSEPATHRESULT]);
  }

  return retVal;
}

UA_StatusCode COPC_UA_Local_Handler::createFolders(const char* paFolders, CSinglyLinkedList<UA_NodeId*>& paCreatedNodeIds) {

  UA_BrowsePath *browsePaths = 0;
  size_t folderCnt = 0;
  size_t firstNonExistingNode;
  CSinglyLinkedList<UA_NodeId*> existingNodeIds;
  UA_StatusCode retVal = translateBrowseNameAndStore(paFolders, &browsePaths, &folderCnt, &firstNonExistingNode, existingNodeIds);

  if(UA_STATUSCODE_GOOD == retVal) {
    UA_NodeId parentNodeId;
    if(existingNodeIds.isEmpty()) {
      UA_NodeId objectsNode = UA_NODEID_NUMERIC(0, UA_NS0ID_ROOTFOLDER);
      UA_NodeId_copy(&objectsNode, &parentNodeId);
    } else {
      UA_NodeId_copy(*existingNodeIds.back(), &parentNodeId);
    }

    CSinglyLinkedList<UA_NodeId*> createdNodeIds;
    UA_NodeId returnedNodeId;
    // create all the nodes on the way
    for(size_t j = firstNonExistingNode; j < folderCnt; j++) {

      UA_NodeId type = UA_NODEID_NUMERIC(0, UA_NS0ID_FOLDERTYPE);
      createObjectInfo createInformation;

      createInformation.mRequestedNodeId = 0;
      createInformation.mParentNodeId = &parentNodeId;
      createInformation.mBrowseName = &browsePaths[folderCnt - 1].relativePath.elements[j].targetName;
      createInformation.mReturnedNodeId = &returnedNodeId;
      createInformation.mTypeNodeId = &type;

      retVal = createObjectNode(&createInformation);

      if(UA_STATUSCODE_GOOD != retVal) {
        DEVLOG_ERROR("[OPC UA LOCAL]: Could not create folder %s in path %s. Error: %s\n",
          reinterpret_cast<const char*>(createInformation.mBrowseName->name.data), paFolders, UA_StatusCode_name(retVal));
        break;
      }

      UA_NodeId *tmp = UA_NodeId_new();
      UA_NodeId_copy(&returnedNodeId, tmp);
      createdNodeIds.pushBack(tmp);

      UA_NodeId_deleteMembers(&parentNodeId);
      UA_NodeId_copy(&returnedNodeId, &parentNodeId);
      UA_NodeId_deleteMembers(&returnedNodeId);
    }

    UA_NodeId_deleteMembers(&parentNodeId);
    for(CSinglyLinkedList<UA_NodeId*>::Iterator it = existingNodeIds.begin(); it != existingNodeIds.end(); ++it) {
      if(UA_STATUSCODE_GOOD != retVal) {
        UA_NodeId_delete(*it);
      } else {
        paCreatedNodeIds.pushBack(*it);
      }
    }

    for(CSinglyLinkedList<UA_NodeId*>::Iterator it = createdNodeIds.begin(); it != createdNodeIds.end(); ++it) {
      paCreatedNodeIds.pushBack(*it); //store the NodeId because the object was created, and if the next fails, the node should be referenced
    }

    COPC_UA_Helper::releaseBrowseArgument(browsePaths, folderCnt);
  }
  return retVal;
}

UA_StatusCode COPC_UA_Local_Handler::splitFoldersFromNode(const CIEC_STRING& paOriginal, CIEC_STRING& paFolder, CIEC_STRING& paNodeName) {

  UA_StatusCode retVal = UA_STATUSCODE_GOOD;

  if(COPC_UA_Helper::checkBrowsePath(paOriginal.getValue())) {
    CIEC_STRING copyOfOriginal(paOriginal);

    char* begin = copyOfOriginal.getValue();
    char* runner = begin + copyOfOriginal.length() - 1;

    if('/' == *runner) { // remove tailing slash
      *runner = '\0';
      runner--;
    }

    while('/' != *runner) {
      runner--;
    }

    *runner = '\0';
    paNodeName = runner + 1;

    if(begin != runner) {
      paFolder = begin;
    }
  }

  return retVal;
}

UA_StatusCode COPC_UA_Local_Handler::getNode(const UA_NodeId* paParentNode, CNodePairInfo* paNodeInfo, CSinglyLinkedList<UA_NodeId*>& paFoundNodeIds,
    bool* paIsPresent) {
  UA_StatusCode retVal = UA_STATUSCODE_GOOD;
  *paIsPresent = false;

  if("" != paNodeInfo->mBrowsePath) {
    UA_BrowsePath *browsePaths = 0;
    size_t pathCount = 0;
    size_t firstNonExistingNode = 0;

    CSinglyLinkedList<UA_NodeId*> existingNodeIds;
    if(UA_STATUSCODE_GOOD
      == translateBrowseNameAndStore(paNodeInfo->mBrowsePath.getValue(), &browsePaths, &pathCount, &firstNonExistingNode, existingNodeIds)) {
      if(pathCount) {
        if(firstNonExistingNode == pathCount) { //all nodes exist
          *paIsPresent = true;
          if(paNodeInfo->mNodeId) { //nodeID was provided
            if(!UA_NodeId_equal(paNodeInfo->mNodeId, *existingNodeIds.back())) {
              *paIsPresent = false; // the found Node has not the same NodeId.
            }
          } else { //if no nodeID was provided, the found Node is stored in the nodePairInfo
            paNodeInfo->mNodeId = UA_NodeId_new();
            UA_NodeId_copy(*existingNodeIds.back(), paNodeInfo->mNodeId);
          }
        }

        for(CSinglyLinkedList<UA_NodeId*>::Iterator it = existingNodeIds.begin(); it != existingNodeIds.end(); ++it) {
          if(!*paIsPresent) {
            UA_NodeId_delete(*it);
          } else {
            paFoundNodeIds.pushBack((*it));
          }
        }
      } else {
        DEVLOG_ERROR("[OPC UA LOCAL]: No folder was found in %s. It should start with a slash /\n", paNodeInfo->mBrowsePath.getValue());
        retVal = UA_STATUSCODE_BADINTERNALERROR;
      }
      COPC_UA_Helper::releaseBrowseArgument(browsePaths, pathCount);
    } else {
      DEVLOG_ERROR("[OPC UA LOCAL]: There's something wrong with your browsename %s\n", paNodeInfo->mBrowsePath.getValue());
      retVal = UA_STATUSCODE_BADINTERNALERROR;
    }
  } else {
    void* handle = 0;
    //we use UA_Server_getNodeContext just to check if the node exist in the addressspace
    if(UA_STATUSCODE_BADNODEIDUNKNOWN != UA_Server_getNodeContext(mUaServer, *paNodeInfo->mNodeId, &handle)) {
      *paIsPresent = true;
    } else {
      DEVLOG_ERROR("[OPC UA LOCAL]: Only a nodeId was provided, but no node exist with that nodeId\n");
      retVal = UA_STATUSCODE_BADINTERNALERROR;
    }
  }

  return retVal;
}

UA_StatusCode COPC_UA_Local_Handler::splitAndCreateFolders(CIEC_STRING& paBrowsePath, CIEC_STRING& paFolders, CIEC_STRING& paNodeName,
    CSinglyLinkedList<UA_NodeId*>& paRreferencedNodes) {

  UA_StatusCode retVal = splitFoldersFromNode(paBrowsePath, paFolders, paNodeName);
  if(UA_STATUSCODE_GOOD == retVal && "" != paFolders) {
      retVal = createFolders(paFolders.getValue(), paRreferencedNodes);
  }
  return retVal;
}

UA_StatusCode COPC_UA_Local_Handler::initializeReadWrite(COPC_UA_HandlerAbstract::CActionInfo& paInfo, bool paWrite) {
  UA_StatusCode retVal = UA_STATUSCODE_GOOD;

  size_t indexOfNodePair = 0;
  CSinglyLinkedList<UA_NodeId*> referencedNodes;
  for(CSinglyLinkedList<CNodePairInfo*>::Iterator itMain = paInfo.getNodePairInfo().begin();
      itMain != paInfo.getNodePairInfo().end() && UA_STATUSCODE_GOOD == retVal; ++itMain, indexOfNodePair++) {

    //get the mConvert type that correspond to the NodePair being initialized
    size_t indexOfType = 0;
    const UA_TypeConvert* typeConvert = 0;
    for(CSinglyLinkedList<UA_TypeConvert*>::Iterator itType = paInfo.getLayer()->getTypeConveverters().begin();
        itType != paInfo.getLayer()->getTypeConveverters().end(); ++itType, indexOfType++) {
      if(indexOfType == indexOfNodePair) {
        typeConvert = *itType;
        break;
      }
    }

    CSinglyLinkedList<UA_NodeId*> presentNodes;
    bool isNodePresent = false;
    retVal = getNode(0, (*itMain), presentNodes, &isNodePresent);

    if(UA_STATUSCODE_GOOD == retVal) {
      if(isNodePresent) {
        if(!paWrite) { //If we are reading a variable, it should be writable from the outside
          retVal = updateNodeUserAccessLevel((*itMain)->mNodeId, UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE);
          if(UA_STATUSCODE_GOOD == retVal) {
            void *handle = 0;
            retVal = UA_Server_getNodeContext(mUaServer, *(*itMain)->mNodeId, &handle);
            if(UA_STATUSCODE_GOOD == retVal) {
              if(!handle) {
                retVal = registerVariableCallBack((*itMain)->mNodeId, paInfo.getLayer(), typeConvert, indexOfNodePair);
              } else {
                DEVLOG_ERROR("[OPC UA LOCAL]: At FB %s RD_%d the node %s has already a FB who is reading from it. Cannot add another one\n",
                  paInfo.getLayer()->getCommFB()->getInstanceName(), indexOfNodePair, (*itMain)->mBrowsePath.getValue());
                retVal = UA_STATUSCODE_BADUNEXPECTEDERROR;
              }
            } else {
              DEVLOG_ERROR("[OPC UA LOCAL]: At FB %s RD_%d the node %s could not retrieve context. Error: %s\n",
                  paInfo.getLayer()->getCommFB()->getInstanceName(), indexOfNodePair, (*itMain)->mBrowsePath.getValue()), UA_StatusCode_name(retVal);
            }
          } else {
            DEVLOG_ERROR("[OPC UA LOCAL]: Cannot set write permission of node for port %d", indexOfNodePair);
          }
        }

        for(CSinglyLinkedList<UA_NodeId*>::Iterator itPresendNodes = presentNodes.begin(); itPresendNodes != presentNodes.end(); ++itPresendNodes) {
          if(UA_STATUSCODE_GOOD != retVal) {
            UA_NodeId_delete(*itPresendNodes);
          } else {
            referencedNodes.pushBack(*itPresendNodes);
          }
        }
      } else { //node does not exist
        //presentNodes shouldn't have any allocated NodeId at this point

        CIEC_STRING folders;
        CIEC_STRING nodeName;
        retVal = splitAndCreateFolders((*itMain)->mBrowsePath, folders, nodeName, referencedNodes);
        if(UA_STATUSCODE_GOOD == retVal) {

          createVariableInfo variableInformation;

          UA_NodeId returnedNodeId;
          variableInformation.mRequestedNodeId = (*itMain)->mNodeId;
          variableInformation.mParentNodeId = referencedNodes.isEmpty() ? 0 : *(referencedNodes.back()); //get the last created folder, which is the parent folder

          UA_UInt16 browsenameNamespace = scmDefaultBrowsenameNameSpace;
          CIEC_STRING targetName;
          CParameterParser browseNameParser(nodeName.getValue(), ':');
          size_t parsingResult = browseNameParser.parseParameters();
          if(COPC_UA_Helper::scmMaxNoOfParametersInBrowseName == parsingResult) {
            browsenameNamespace = forte::core::util::strtol(browseNameParser[0], 0, 10);
            targetName = browseNameParser[1];
          } else if(1 == parsingResult) {
            targetName = browseNameParser[0];
          } else {
            DEVLOG_ERROR("[OPC UA LOCAL]: Error by parsing Variable FB browse path %s\n", nodeName.getValue());
            retVal = UA_STATUSCODE_BADINVALIDARGUMENT;
          }

          if(UA_STATUSCODE_GOOD == retVal) {
            UA_QualifiedName browseName = UA_QUALIFIEDNAME_ALLOC(browsenameNamespace, targetName.getValue());
            variableInformation.mBrowseName = &browseName;
            variableInformation.mReturnedNodeId = &returnedNodeId;
            variableInformation.mTypeConvert = typeConvert;
            variableInformation.mAllowWrite = !paWrite; //write FB here means that from the outside should not be possible to write and the other way around for read
            variableInformation.mInitData =
                paWrite ? &paInfo.getLayer()->getCommFB()->getSDs()[indexOfNodePair] : &paInfo.getLayer()->getCommFB()->getRDs()[indexOfNodePair];

            retVal = createVariableNode(&variableInformation);
            if(UA_STATUSCODE_GOOD == retVal) {
              if(!paWrite) {
                retVal = registerVariableCallBack(&returnedNodeId, paInfo.getLayer(), typeConvert, indexOfNodePair);
              }
              if(UA_STATUSCODE_GOOD == retVal) { //store returned NodeId in the pair
                if(!(*itMain)->mNodeId) {
                  (*itMain)->mNodeId = UA_NodeId_new();
                  UA_NodeId_copy(&returnedNodeId, (*itMain)->mNodeId);
                }
                UA_NodeId *tmp = UA_NodeId_new();
                UA_NodeId_copy((*itMain)->mNodeId, tmp);
                referencedNodes.pushBack(tmp);
              }
              UA_NodeId_deleteMembers(&returnedNodeId);
            }
            UA_QualifiedName_deleteMembers(&browseName);
          }
        }
      }
    }
  }

  //we add the references first even if it fails, since some nodes might have been created,
  //and/or some might have been already there, so deleting them will be taken care of by
  //the referencedNodesDecrement function later
  referencedNodesIncrement(&referencedNodes, &paInfo);

  if(UA_STATUSCODE_GOOD != retVal) {
    referencedNodesDecrement(&paInfo);
  }

  for(CSinglyLinkedList<UA_NodeId*>::Iterator itRerencedNodes = referencedNodes.begin(); itRerencedNodes != referencedNodes.end(); ++itRerencedNodes) {
    UA_NodeId_delete(*itRerencedNodes);
  }
  return retVal;
}

UA_StatusCode COPC_UA_Local_Handler::initializeAction(COPC_UA_HandlerAbstract::CActionInfo & paInfo) {
  enableHandler();
  UA_StatusCode retVal = UA_STATUSCODE_BADINTERNALERROR;
  if(mUaServer) { //if the server failed at starting, nothing will be initialized
    switch(paInfo.getAction()){
      case eRead:
        retVal = initializeReadWrite(paInfo, false);
        break;
      case eWrite:
        retVal = initializeReadWrite(paInfo, true);
        break;
      case eCreateMethod:
        retVal = initializeCreateMethod(paInfo);
        break;
      case eCreateObject:
        retVal = initializeCreateObject(paInfo);
        break;
      case eDeleteObject:
        retVal = initializeDeleteObject(paInfo);
        break;
      case eCallMethod:
      case eSubscribe:
        DEVLOG_ERROR("[OPC UA LOCAL]: Cannot perform action %s locally. Initialization failed\n", COPC_UA_HandlerAbstract::mActionNames[paInfo.getAction()]);
        break;
      default:
        DEVLOG_ERROR("[OPC UA LOCAL]: Unknown action %d to be initialized\n", paInfo.getAction());
        break;
    }
  }
  return retVal;
}

UA_StatusCode COPC_UA_Local_Handler::executeAction(COPC_UA_HandlerAbstract::CActionInfo& paInfo) {
  UA_StatusCode retVal = UA_STATUSCODE_BADINTERNALERROR;

  switch(paInfo.getAction()){
    case eWrite:
      retVal = executeWrite(paInfo);
      break;
    case eCreateMethod:
      retVal = executeCreateMethod(paInfo);
      break;
    case eCreateObject:
      retVal = executeCreateObject(paInfo);
      break;
    case eDeleteObject:
      retVal = executeDeleteObject(paInfo);
      break;
    default: //eCallMethod, eSubscribe will never reach here since they weren't initialized. eRead is a Subscribe FB
      DEVLOG_ERROR("[OPC UA LOCAL]: Action %d to be executed is unknown or invalid\n", paInfo.getAction());
      break;
  }

  return retVal;
}

UA_StatusCode COPC_UA_Local_Handler::uninitializeAction(COPC_UA_HandlerAbstract::CActionInfo & paInfo) {
  UA_StatusCode retVal = UA_STATUSCODE_BADINTERNALERROR;
  switch(paInfo.getAction()){
    case eRead:
    case eWrite:
    case eCreateMethod:
    case eCreateObject:
    case eDeleteObject:
      referencedNodesDecrement(&paInfo);
      retVal = UA_STATUSCODE_GOOD;
      break;
    default:
      DEVLOG_ERROR("[OPC UA LOCAL]: Action %d to be uninitialized is unknown or invalid\n", paInfo.getAction());
      break;
  }
  return retVal;
}

UA_StatusCode COPC_UA_Local_Handler::executeWrite(COPC_UA_HandlerAbstract::CActionInfo & paInfo) {
  UA_StatusCode retVal = UA_STATUSCODE_GOOD;
  const CIEC_ANY *SDs = paInfo.getLayer()->getCommFB()->getSDs();
  size_t indexOfNodePair = 0;
  CSinglyLinkedList<UA_TypeConvert*>::Iterator itType = paInfo.getLayer()->getTypeConveverters().begin();
  for(CSinglyLinkedList<CNodePairInfo*>::Iterator it = paInfo.getNodePairInfo().begin(); it != paInfo.getNodePairInfo().end();
      ++it, indexOfNodePair++, ++itType) {

    if(UA_STATUSCODE_GOOD != updateNodeValue((*it)->mNodeId, &SDs[indexOfNodePair], *itType)) {
      DEVLOG_ERROR("[OPC UA LOCAL]: Could not convert value to write for port %d at FB %s.\n", indexOfNodePair,
        paInfo.getLayer()->getCommFB()->getInstanceName());
      retVal = UA_STATUSCODE_BADUNEXPECTEDERROR;
      break;
    }

  }
  return retVal;
}

UA_StatusCode COPC_UA_Local_Handler::handleExistingMethod(COPC_UA_HandlerAbstract::CActionInfo& paInfo, UA_NodeId *paParentNode) {
  CSinglyLinkedList<CNodePairInfo*>::Iterator it = paInfo.getNodePairInfo().begin();

  DEVLOG_INFO("[OPC UA LOCAL]: Adding a callback for an existing method at %s\n", (*it)->mBrowsePath.getValue());

  //TODO: check types of existing method to this layer
  void *handle = 0;
  UA_StatusCode retVal = UA_Server_getNodeContext(mUaServer, *(*it)->mNodeId, &handle);
  if(UA_STATUSCODE_GOOD == retVal) {
    if(!handle) {
      //check if the method was already referenced by another FB
      for(CSinglyLinkedList<UA_ParentNodeHandler *>::Iterator iter = methodsWithoutContext.begin(); iter != methodsWithoutContext.end(); ++iter) {
        if(UA_NodeId_equal(paParentNode, (*iter)->mParentNodeId) && UA_NodeId_equal((*it)->mNodeId, (*iter)->mMethodNodeId)) {
          DEVLOG_ERROR(
            "[OPC UA LOCAL]: The FB %s is trying to reference a local method (created through an object) at %s which has already a FB who is referencing it. Cannot add another one\n",
            paInfo.getLayer()->getCommFB()->getInstanceName(), (*it)->mBrowsePath.getValue());
          retVal = UA_STATUSCODE_BADUNEXPECTEDERROR;
          break;
        }
      }

      if(UA_STATUSCODE_GOOD == retVal) {
        UA_ParentNodeHandler *parentNodeContext = new UA_ParentNodeHandler();

        parentNodeContext->mParentNodeId = UA_NodeId_new();
        UA_NodeId_copy(paParentNode, parentNodeContext->mParentNodeId);
        parentNodeContext->mActionInfo = static_cast<COPC_UA_HandlerAbstract::CLocalMethodInfo*>(&paInfo);
        parentNodeContext->mMethodNodeId = (*it)->mNodeId;

        methodsWithoutContext.pushBack(parentNodeContext);

        retVal = UA_Server_setMethodNode_callback(mUaServer, *(*it)->mNodeId, COPC_UA_Local_Handler::onServerMethodCall);
        if(UA_STATUSCODE_GOOD != retVal) {
          DEVLOG_ERROR("[OPC UA LOCAL]: Could not set callback function for method at %s. Error: %s\n", paInfo.getLayer()->getCommFB()->getInstanceName(),
            UA_StatusCode_name(retVal));
        }
      }
    } else {
      DEVLOG_ERROR(
        "[OPC UA LOCAL]: The FB %s is trying to reference a local method at %s which has already a FB who is referencing it. Cannot add another one\n",
        paInfo.getLayer()->getCommFB()->getInstanceName(), (*it)->mBrowsePath.getValue());
      retVal = UA_STATUSCODE_BADUNEXPECTEDERROR;
    }
  } else {
    DEVLOG_ERROR("[OPC UA LOCAL]: At FB %s the node %s could not retrieve context. Error: %s\n",
        paInfo.getLayer()->getCommFB()->getInstanceName(), (*it)->mBrowsePath.getValue()), UA_StatusCode_name(retVal);
  }
  return retVal;
}

UA_StatusCode COPC_UA_Local_Handler::initializeCreateMethod(COPC_UA_HandlerAbstract::CActionInfo & paInfo) {

  CSinglyLinkedList<UA_NodeId*> referencedNodes;
  CSinglyLinkedList<UA_NodeId*> presentNodes;

  CSinglyLinkedList<CNodePairInfo*>::Iterator it = paInfo.getNodePairInfo().begin();

  bool isNodePresent = false;
  UA_StatusCode retVal = getNode(0, (*it), presentNodes, &isNodePresent);

  if(UA_STATUSCODE_GOOD == retVal) {
    if(isNodePresent) {

      UA_NodeId parent = UA_NODEID_NUMERIC(0, UA_NS0ID_ROOTFOLDER);

      for(CSinglyLinkedList<UA_NodeId*>::Iterator itPresentNodes = presentNodes.begin(); itPresentNodes != presentNodes.end();) {
        CSinglyLinkedList<UA_NodeId*>::Iterator currentIterator = itPresentNodes;
        ++itPresentNodes;
        if(itPresentNodes == presentNodes.back()) {
          parent = **currentIterator;
          break;
        }
      }

      retVal = handleExistingMethod(paInfo, &parent);

      for(CSinglyLinkedList<UA_NodeId*>::Iterator itPresendNodes = presentNodes.begin(); itPresendNodes != presentNodes.end(); ++itPresendNodes) {
        if(UA_STATUSCODE_GOOD != retVal) {
          UA_NodeId_delete(*itPresendNodes);
        } else {
          referencedNodes.pushBack(*itPresendNodes);
        }
      }
    } else { //node does not exist
      //presentNodes shouldn't have any allocated NodeId at this point

      CIEC_STRING folders;
      CIEC_STRING nodeName;

      retVal = splitAndCreateFolders((*it)->mBrowsePath, folders, nodeName, referencedNodes);
      if(UA_STATUSCODE_GOOD == retVal) {

        //we store the names of the SDs/RDs as names for the arguments names. Not so nice for now
        CSinglyLinkedList<CIEC_STRING> names;
        const SFBInterfaceSpec* interfaceFB = paInfo.getLayer()->getCommFB()->getFBInterfaceSpec();

        for(unsigned int i = 2; i < static_cast<unsigned int>(interfaceFB->m_nNumDIs); i++) {
          CIEC_STRING tmp = CStringDictionary::getInstance().get(interfaceFB->m_aunDINames[i]);
          names.pushBack(tmp);
        }

        for(unsigned int i = 2; i < static_cast<unsigned int>(interfaceFB->m_nNumDOs); i++) {
          CIEC_STRING tmp = CStringDictionary::getInstance().get(interfaceFB->m_aunDONames[i]);
          names.pushBack(tmp);
        }

        createMethodInfo methodInformation;
        createMethodArguments(&methodInformation, paInfo, names);

        UA_NodeId returnedNodeId;
        methodInformation.mRequestedNodeId = (*it)->mNodeId;
        methodInformation.mParentNodeId = referencedNodes.isEmpty() ? 0 : *(referencedNodes.back()); //get the last created folder, which is the parent folder

        UA_UInt16 browsenameNamespace = scmDefaultBrowsenameNameSpace;
        CIEC_STRING targetName;
        CParameterParser browseNameParser(nodeName.getValue(), ':');
        size_t parsingResult = browseNameParser.parseParameters();
        if(COPC_UA_Helper::scmMaxNoOfParametersInBrowseName == parsingResult) {
          browsenameNamespace = forte::core::util::strtol(browseNameParser[0], 0, 10);
          targetName = browseNameParser[1];
        } else if(1 == parsingResult) {
          targetName = browseNameParser[0];
        } else {
          DEVLOG_ERROR("[OPC UA LOCAL]: Error by parsing Method FB browse path %s\n", nodeName.getValue());
          retVal = UA_STATUSCODE_BADINVALIDARGUMENT;
        }

        if(UA_STATUSCODE_GOOD == retVal) {
          UA_QualifiedName browseName = UA_QUALIFIEDNAME_ALLOC(browsenameNamespace, targetName.getValue());
          methodInformation.mBrowseName = &browseName;
          methodInformation.mReturnedNodeId = &returnedNodeId;
          methodInformation.mCallback = static_cast<COPC_UA_HandlerAbstract::CLocalMethodInfo*>(&paInfo);
          methodInformation.mOutputSize = paInfo.getLayer()->getCommFB()->getNumSD();
          methodInformation.mInputSize = paInfo.getLayer()->getCommFB()->getNumRD();

          retVal = createMethodNode(&methodInformation);

          if(UA_STATUSCODE_GOOD == retVal) {
            if(!(*it)->mNodeId) {
              (*it)->mNodeId = UA_NodeId_new();
              UA_NodeId_copy(&returnedNodeId, (*it)->mNodeId);
            }
            UA_NodeId *tmp = UA_NodeId_new();
            UA_NodeId_copy((*it)->mNodeId, tmp);
            referencedNodes.pushBack(tmp);

            UA_NodeId_deleteMembers(&returnedNodeId);

          } else {
            DEVLOG_ERROR("[OPC UA LOCAL]: OPC UA could not create method at %s. Error: %s\n", paInfo.getLayer()->getCommFB()->getInstanceName(),
              UA_StatusCode_name(retVal));
          }
          UA_QualifiedName_deleteMembers(&browseName);
        }
        UA_Array_delete(methodInformation.mInputArguments, methodInformation.mInputSize, &UA_TYPES[UA_TYPES_ARGUMENT]);
        UA_Array_delete(methodInformation.mOutputArguments, methodInformation.mOutputSize, &UA_TYPES[UA_TYPES_ARGUMENT]);
      }
    }
  }

  //we add the references first even if it fails, since some nodes might have been created,
  //and/or some might have been already there, so deleting them will be taken care of by
  //the referencedNodesDecrement function later
  referencedNodesIncrement(&referencedNodes, &paInfo);

  if(UA_STATUSCODE_GOOD != retVal) {
    referencedNodesDecrement(&paInfo);
  }

  for(CSinglyLinkedList<UA_NodeId*>::Iterator itRerencedNodes = referencedNodes.begin(); itRerencedNodes != referencedNodes.end(); ++itRerencedNodes) {
    UA_NodeId_delete(*itRerencedNodes);
  }
  return retVal;
}

UA_StatusCode COPC_UA_Local_Handler::initializeCreateObject(COPC_UA_HandlerAbstract::CActionInfo & paInfo) {
  //The main process is done in the execution
  UA_StatusCode retVal = UA_STATUSCODE_GOOD;

  CSinglyLinkedList<CNodePairInfo*>::Iterator it = paInfo.getNodePairInfo().begin();
  ++it;

  if("" == (*it)->mBrowsePath) { //the browsename of the instance is mandatory
    retVal = UA_STATUSCODE_BADINTERNALERROR;
  }
  return retVal;
}

UA_StatusCode COPC_UA_Local_Handler::executeCreateObject(COPC_UA_HandlerAbstract::CActionInfo & paInfo) {

  CSinglyLinkedList<UA_NodeId*> referencedNodes;

  bool isNodePresent = false;
  CSinglyLinkedList<CNodePairInfo*>::Iterator itType = paInfo.getNodePairInfo().begin();

  //look for type first
  UA_StatusCode retVal = getNode(0, (*itType), referencedNodes, &isNodePresent);

  if(UA_STATUSCODE_GOOD == retVal) {
    if(isNodePresent) {

      //we don't need the nodes from the type
      for(CSinglyLinkedList<UA_NodeId*>::Iterator itPresendNodes = referencedNodes.begin(); itPresendNodes != referencedNodes.end(); ++itPresendNodes) {
        UA_NodeId_delete(*itPresendNodes);
      }
      referencedNodes.clearAll();

      CSinglyLinkedList<CNodePairInfo*>::Iterator itInstance = paInfo.getNodePairInfo().begin();
      ++itInstance;

      isNodePresent = false;
      //check if an instance is already present
      retVal = getNode(0, (*itInstance), referencedNodes, &isNodePresent);

      if(UA_STATUSCODE_GOOD == retVal) {
        if(!isNodePresent) {

          CIEC_STRING folders;
          CIEC_STRING nodeName;
          retVal = splitAndCreateFolders((*itInstance)->mBrowsePath, folders, nodeName, referencedNodes);
          if(UA_STATUSCODE_GOOD == retVal) {

            UA_NodeId returnedNodeId;

            createObjectInfo createInformation;

            createInformation.mRequestedNodeId = (*itInstance)->mNodeId;
            createInformation.mParentNodeId = referencedNodes.isEmpty() ? 0 : *(referencedNodes.back()); //get the last created folder, which is the parent folder

            UA_UInt16 browsenameNamespace = scmDefaultBrowsenameNameSpace;
            CIEC_STRING targetName;
            CParameterParser browseNameParser(nodeName.getValue(), ':');
            size_t parsingResult = browseNameParser.parseParameters();
            if(COPC_UA_Helper::scmMaxNoOfParametersInBrowseName == parsingResult) {
              browsenameNamespace = forte::core::util::strtol(browseNameParser[0], 0, 10);
              targetName = browseNameParser[1];
            } else if(1 == parsingResult) {
              targetName = browseNameParser[0];
            } else {
              DEVLOG_ERROR("[OPC UA LOCAL]: Error by parsing Object FB browse path %s\n", nodeName.getValue());
              retVal = UA_STATUSCODE_BADINVALIDARGUMENT;
            }

            if(UA_STATUSCODE_GOOD == retVal) {
              UA_QualifiedName browseName = UA_QUALIFIEDNAME_ALLOC(browsenameNamespace, targetName.getValue());
              createInformation.mBrowseName = &browseName;
              createInformation.mReturnedNodeId = &returnedNodeId;
              createInformation.mTypeNodeId = (*itType)->mNodeId;

              retVal = createObjectNode(&createInformation);

              if(UA_STATUSCODE_GOOD == retVal) {
                UA_NodeId *tmp = UA_NodeId_new();
                UA_NodeId_copy(&returnedNodeId, tmp);
                referencedNodes.pushBack(tmp);
                UA_NodeId_deleteMembers(&returnedNodeId);
              } else {
                DEVLOG_ERROR("[OPC UA LOCAL]: Could not create object %s in path %s. Error: %s\n", nodeName.getValue(), folders.getValue(),
                  UA_StatusCode_name(retVal));
              }
              UA_QualifiedName_deleteMembers(&browseName);
            }
          }
        } else {
          DEVLOG_ERROR("[OPC UA LOCAL]: You are trying to create a node which already exists at FB %s\n", paInfo.getLayer()->getCommFB()->getInstanceName());
          retVal = UA_STATUSCODE_BADINTERNALERROR;
        }
      }
    } else {
      DEVLOG_ERROR("[OPC UA LOCAL]: The type of the object to create could not be found for FB %s\n", paInfo.getLayer()->getCommFB()->getInstanceName());
      retVal = UA_STATUSCODE_BADINTERNALERROR;
    }
  }

  //we add the references first even if itType fails, since some nodes might have been created,
  //and/or some might have been already there, so deleting them will be taken care of by
  //the referencedNodesDecrement function later
  referencedNodesIncrement(&referencedNodes, &paInfo);

  if(UA_STATUSCODE_GOOD != retVal) {
    referencedNodesDecrement(&paInfo);
  }

  for(CSinglyLinkedList<UA_NodeId*>::Iterator itRerencedNodes = referencedNodes.begin(); itRerencedNodes != referencedNodes.end(); ++itRerencedNodes) {
    UA_NodeId_delete(*itRerencedNodes);
  }

  return retVal;
}

UA_StatusCode COPC_UA_Local_Handler::initializeDeleteObject(COPC_UA_HandlerAbstract::CActionInfo & paInfo) {
  //nothing to do here
  return UA_STATUSCODE_GOOD;
}

UA_StatusCode COPC_UA_Local_Handler::executeDeleteObject(COPC_UA_HandlerAbstract::CActionInfo & paInfo) {

  bool isNodePresent = false;
  CSinglyLinkedList<CNodePairInfo*>::Iterator itInstance = paInfo.getNodePairInfo().begin();
  CSinglyLinkedList<UA_NodeId*> referencedNodes;

  //look for instance
  UA_StatusCode retVal = getNode(0, (*itInstance), referencedNodes, &isNodePresent);

  if(UA_STATUSCODE_GOOD == retVal) {
    if(isNodePresent) {
      retVal = UA_Server_deleteNode(mUaServer, *(*itInstance)->mNodeId, true);
    } else {
      DEVLOG_ERROR("[OPC UA LOCAL]: The instance of the object to delete could not be found for FB %s\n", paInfo.getLayer()->getCommFB()->getInstanceName());
      retVal = UA_STATUSCODE_BADINTERNALERROR;
    }

    for(CSinglyLinkedList<UA_NodeId*>::Iterator itPresendNodes = referencedNodes.begin(); itPresendNodes != referencedNodes.end(); ++itPresendNodes) {
      UA_NodeId_delete(*itPresendNodes);
    }
    referencedNodes.clearAll();
  }

  return retVal;
}

COPC_UA_Local_Handler::CLocalMethodCall* COPC_UA_Local_Handler::addMethodCall(COPC_UA_HandlerAbstract::CLocalMethodInfo *paActionInfo,
    UA_SendVariable_handle* paHandleRecv) {
  CCriticalRegion criticalRegion(mMethodCallsMutex);
  CLocalMethodCall* toAdd = new CLocalMethodCall();
  toAdd->mActionInfo = paActionInfo;
  toAdd->mSendHandle = paHandleRecv;
  mMethodCalls.pushBack(toAdd);
  return toAdd;
}

void COPC_UA_Local_Handler::removeMethodCall(CLocalMethodCall* toDelete) {
  CCriticalRegion criticalRegion(mMethodCallsMutex);
  delete toDelete;
  mMethodCalls.erase(toDelete);
}

void COPC_UA_Local_Handler::cleanMethodCalls() {
  CCriticalRegion criticalRegion(mMethodCallsMutex);
  for(CSinglyLinkedList<CLocalMethodCall*>::Iterator it = mMethodCalls.begin(); it != mMethodCalls.end(); ++it) {
    delete *it;
  }

  mMethodCalls.clearAll();
}

COPC_UA_Local_Handler::CLocalMethodCall* COPC_UA_Local_Handler::getLocalMethodCall(COPC_UA_HandlerAbstract::CLocalMethodInfo *paActionInfo) {
  CCriticalRegion criticalRegion(mMethodCallsMutex);
  CLocalMethodCall* retVal = 0;
  for(CSinglyLinkedList<CLocalMethodCall*>::Iterator it = mMethodCalls.begin(); it != mMethodCalls.end(); ++it) {
    if((*it)->mActionInfo == paActionInfo) {
      retVal = *it;
      break;
    }
  }
  return retVal;
}

