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

const char * const COPC_UA_Local_Handler::mEnglishLocaleForNodes = "en-US";
const char * const COPC_UA_Local_Handler::mDefaultDescriptionForVariableNodes = "Digital port of Function Block";

TForteUInt16 gOpcuaServerPort = FORTE_COM_OPC_UA_PORT;

using namespace forte::com_infra;

DEFINE_HANDLER(COPC_UA_Local_Handler);

COPC_UA_Local_Handler::COPC_UA_Local_Handler(CDeviceExecution &paDeviceExecution) :
    COPC_UA_HandlerAbstract(paDeviceExecution), mUaServer(0), mUaServerRunningFlag(UA_FALSE)
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

  CCriticalRegion criticalRegion(mNodesReferencesMutex);
  for(CSinglyLinkedList<nodesReferencedByActions *>::Iterator iter = mNodesReferences.begin(); iter != mNodesReferences.end(); ++iter) {
    UA_NodeId_delete(const_cast<UA_NodeId*>((*iter)->mNodeId));
    delete *iter;
  }
  mNodesReferences.clearAll();

#ifdef FORTE_COM_OPC_UA_MULTICAST
  for(CSinglyLinkedList<UA_String*>::Iterator iter = mRegisteredWithLds.begin(); iter != mRegisteredWithLds.end(); ++iter) {
    UA_String_delete(*iter);
  }
#endif //FORTE_COM_OPC_UA_MULTICAST
}

void COPC_UA_Local_Handler::enableHandler(void) {
  startServer();
}

void COPC_UA_Local_Handler::disableHandler(void) {
  COPC_UA_Local_Handler::stopServer();
  end();
}

void COPC_UA_Local_Handler::run() {
  DEVLOG_INFO("[OPC UA LOCAL]: Starting OPC UA Server: opc.tcp://localhost:%d\n", gOpcuaServerPort);

  UA_ServerConfig *uaServerConfig = 0;

#ifdef FORTE_COM_OPC_UA_MASTER_BRANCH
  mUaServer = UA_Server_new();
  if(mUaServer) {
    uaServerConfig = UA_Server_getConfig(mUaServer);
    UA_ServerConfig_setDefault(uaServerConfig);
    configureUAServer(gOpcuaServerPort, *uaServerConfig);
#else
  uaServerConfig = UA_ServerConfig_new_minimal(gOpcuaServerPort, 0);
  configureUAServer(gOpcuaServerPort, *uaServerConfig);
  mUaServer = UA_Server_new(uaServerConfig);
  if(mUaServer) {
#endif

    if(initializeNodesets(*mUaServer)) {
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

void COPC_UA_Local_Handler::configureUAServer(TForteUInt16 paUAServerPort, UA_ServerConfig &paUaServerConfig) {
  char helperBuffer[scmMaxServerNameLength + 1];
  forte_snprintf(helperBuffer, scmMaxServerNameLength, "forte_%d", paUAServerPort);

  paUaServerConfig.logger = COPC_UA_HandlerAbstract::getLogger();

#ifdef FORTE_COM_OPC_UA_MULTICAST
  paUaServerConfig.applicationDescription.applicationType = UA_APPLICATIONTYPE_DISCOVERYSERVER;
  // hostname will be added by mdns library
# ifdef FORTE_COM_OPC_UA_MASTER_BRANCH
  UA_String_deleteMembers(&paUaServerConfig.discovery.mdns.mdnsServerName);
  paUaServerConfig.discovery.mdns.mdnsServerName = UA_String_fromChars(helperBuffer);
# else //FORTE_COM_OPC_UA_MASTER_BRANCH
  UA_String_deleteMembers(&paUaServerConfig.mdnsServerName);
  paUaServerConfig.mdnsServerName = UA_String_fromChars(helperBuffer);
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
  UA_String_deleteMembers(&paUaServerConfig.applicationDescription.applicationUri);
  UA_LocalizedText_deleteMembers(&paUaServerConfig.applicationDescription.applicationName);

  paUaServerConfig.applicationDescription.applicationUri = UA_String_fromChars(helperBuffer);
  paUaServerConfig.applicationDescription.applicationName.locale = UA_STRING_NULL;
  paUaServerConfig.applicationDescription.applicationName.text = UA_String_fromChars(hostname);

  for(size_t i = 0; i < paUaServerConfig.endpointsSize; i++) {
#ifdef FORTE_COM_OPC_UA_MASTER_BRANCH
    UA_String_deleteMembers(&paUaServerConfig.endpoints[i].server.applicationUri);
    UA_LocalizedText_deleteMembers(&paUaServerConfig.endpoints[i].server.applicationName);
    UA_String_copy(&paUaServerConfig.applicationDescription.applicationUri, &paUaServerConfig.endpoints[i].server.applicationUri);
    UA_LocalizedText_copy(&paUaServerConfig.applicationDescription.applicationName, &paUaServerConfig.endpoints[i].server.applicationName);
#else //FORTE_COM_OPC_UA_MASTER_BRANCH
    UA_String_deleteMembers(&paUaServerConfig.endpoints[i].endpointDescription.server.applicationUri);
    UA_LocalizedText_deleteMembers(&paUaServerConfig.endpoints[i].endpointDescription.server.applicationName);
    UA_String_copy(&paUaServerConfig.applicationDescription.applicationUri, &paUaServerConfig.endpoints[i].endpointDescription.server.applicationUri);
    UA_LocalizedText_copy(&paUaServerConfig.applicationDescription.applicationName, &paUaServerConfig.endpoints[i].endpointDescription.server.applicationName);
#endif //FORTE_COM_OPC_UA_MASTER_BRANCH
  }
}

void COPC_UA_Local_Handler::referencedNodesIncrement(const CSinglyLinkedList<UA_NodeId *> &paNodes, CActionInfo &paActionInfo) {
  CCriticalRegion criticalRegion(mNodesReferencesMutex);
  for(CSinglyLinkedList<UA_NodeId *>::Iterator iterNode = paNodes.begin(); iterNode != paNodes.end(); ++iterNode) {
    bool found = false;
    for(CSinglyLinkedList<nodesReferencedByActions *>::Iterator iterRef = mNodesReferences.begin(); iterRef != mNodesReferences.end(); ++iterRef) {
      if(UA_NodeId_equal((*iterRef)->mNodeId, (*iterNode))) {
        found = true;
        (*iterRef)->mActionsReferencingIt.pushFront(&paActionInfo);
        break;
      }
    }
    if(!found) {
      nodesReferencedByActions *newRef = new nodesReferencedByActions();
      UA_NodeId *newNode = UA_NodeId_new();
      UA_NodeId_copy((*iterNode), newNode);
      newRef->mNodeId = newNode;
      newRef->mActionsReferencingIt = CSinglyLinkedList<CActionInfo*>();
      newRef->mActionsReferencingIt.pushFront(&paActionInfo);
      mNodesReferences.pushFront(newRef);
    }
  }
}

void COPC_UA_Local_Handler::referencedNodesDecrement(const CActionInfo &paActionInfo) {
  CCriticalRegion criticalRegion(mNodesReferencesMutex);
  CSinglyLinkedList<const UA_NodeId *> nodesReferencedByAction;
  getNodesReferencedByAction(paActionInfo, nodesReferencedByAction);

  for(CSinglyLinkedList<const UA_NodeId *>::Iterator iterNode = nodesReferencedByAction.begin(); iterNode != nodesReferencedByAction.end(); ++iterNode) {
    CSinglyLinkedList<nodesReferencedByActions *>::Iterator nodeReferencedToDelete = mNodesReferences.end();
    for(CSinglyLinkedList<nodesReferencedByActions *>::Iterator iterRef = mNodesReferences.begin(); iterRef != mNodesReferences.end(); ++iterRef) {
      if(UA_NodeId_equal((*iterRef)->mNodeId, (*iterNode))) {

        bool stillSomethingThere = true;
        while(stillSomethingThere) {
          CSinglyLinkedList<CActionInfo *>::Iterator itActionToDelete = (*iterRef)->mActionsReferencingIt.end();
          for(CSinglyLinkedList<CActionInfo *>::Iterator itAction = (*iterRef)->mActionsReferencingIt.begin();
              itAction != (*iterRef)->mActionsReferencingIt.end(); ++itAction) {
            if((*itAction) == &paActionInfo) {
              itActionToDelete = itAction;
              break;
            }
          }

          if((*iterRef)->mActionsReferencingIt.end() == itActionToDelete) {
            stillSomethingThere = false;
          } else {
            (*iterRef)->mActionsReferencingIt.erase(*itActionToDelete);
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

void COPC_UA_Local_Handler::getNodesReferencedByAction(const CActionInfo &paActionInfo, CSinglyLinkedList<const UA_NodeId *> &paNodes) {
  for(CSinglyLinkedList<nodesReferencedByActions *>::Iterator iterRef = mNodesReferences.begin(); iterRef != mNodesReferences.end(); ++iterRef) {
    for(CSinglyLinkedList<CActionInfo *>::Iterator iterAction = (*iterRef)->mActionsReferencingIt.begin();
        iterAction != (*iterRef)->mActionsReferencingIt.end(); ++iterAction) {
      if((*iterAction) == &paActionInfo) {
        paNodes.pushFront((*iterRef)->mNodeId);
        break;
      }
    }
  }
}

#ifdef FORTE_COM_OPC_UA_MULTICAST

const UA_String* COPC_UA_Local_Handler::getDiscoveryUrl() const {

#ifdef FORTE_COM_OPC_UA_MASTER_BRANCH
  UA_ServerConfig *mServerConfig = UA_Server_getConfig(mUaServer); //change mServerConfig to serverConfig when only master branch is present
#else
  //do nothing
#endif
  if(0 == mServerConfig->networkLayersSize) {
    return 0;
  }
  return &mServerConfig->networkLayers[0].discoveryUrl;
}

void COPC_UA_Local_Handler::serverOnNetworkCallback(const UA_ServerOnNetwork *paServerOnNetwork, UA_Boolean paIsServerAnnounce, UA_Boolean paIsTxtReceived,
    void *paData) { //NOSONAR
  COPC_UA_Local_Handler *handler = static_cast<COPC_UA_Local_Handler*>(paData);

  const UA_String *ownDiscoverUrl = handler->getDiscoveryUrl();

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
  for(CSinglyLinkedList<UA_String*>::Iterator iter = mRegisteredWithLds.begin(); iter != mRegisteredWithLds.end(); ++iter) {
    if(UA_String_equal(paDiscoveryUrl, *iter)) {
      return;
    }
  }

  // will be freed when removed from list
  UA_String *discoveryUrlChar = 0;
  UA_String_copy(paDiscoveryUrl, discoveryUrlChar);

  mRegisteredWithLds.pushFront(discoveryUrlChar);
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
  UA_String *toDelete = 0;
  for(CSinglyLinkedList<UA_String*>::Iterator iter = mRegisteredWithLds.begin(); iter != mRegisteredWithLds.end(); ++iter) {
    if(UA_String_equal(paDiscoveryUrl, *iter)) {
      toDelete = *iter;
      break;
    }
  }
  if(toDelete) {
    mRegisteredWithLds.erase(toDelete);
    UA_String_delete(toDelete);
  }
}

#endif //FORTE_COM_OPC_UA_MULTICAST

UA_StatusCode COPC_UA_Local_Handler::initializeAction(CActionInfo &paActionInfo) {
  enableHandler();
  UA_StatusCode retVal = UA_STATUSCODE_BADINTERNALERROR;
  if(mUaServer) { //if the server failed at starting, nothing will be initialized
    // other thread may currently create nodes, thus mutex
    CCriticalRegion criticalRegion(mCreateNodesMutex);
    switch(paActionInfo.getAction()){
      case CActionInfo::eRead:
        retVal = initializeVariable(paActionInfo, false);
        break;
      case CActionInfo::eWrite:
        retVal = initializeVariable(paActionInfo, true);
        break;
      case CActionInfo::eCreateMethod:
        retVal = initializeCreateMethod(paActionInfo);
        break;
      case CActionInfo::eCreateObject:
        retVal = initializeCreateObject(paActionInfo);
        break;
      case CActionInfo::eDeleteObject:
        retVal = initializeDeleteObject(paActionInfo);
        break;
      case CActionInfo::eCallMethod:
      case CActionInfo::eSubscribe:
        DEVLOG_ERROR("[OPC UA LOCAL]: Cannot perform action %s locally. Initialization failed\n", CActionInfo::mActionNames[paActionInfo.getAction()]);
        break;
      default:
        DEVLOG_ERROR("[OPC UA LOCAL]: Unknown action %d to be initialized\n", paActionInfo.getAction());
        break;
    }
  }
  return retVal;
}

UA_StatusCode COPC_UA_Local_Handler::executeAction(CActionInfo &paActionInfo) {
  UA_StatusCode retVal = UA_STATUSCODE_BADINTERNALERROR;

  switch(paActionInfo.getAction()){
    case CActionInfo::eWrite:
      retVal = executeWrite(paActionInfo);
      break;
    case CActionInfo::eCreateMethod:
      retVal = executeCreateMethod(paActionInfo);
      break;
    case CActionInfo::eCreateObject:
      retVal = executeCreateObject(paActionInfo);
      break;
    case CActionInfo::eDeleteObject:
      retVal = executeDeleteObject(paActionInfo);
      break;
    default: //eCallMethod, eSubscribe will never reach here since they weren't initialized. eRead is a Subscribe FB
      DEVLOG_ERROR("[OPC UA LOCAL]: Action %d to be executed is unknown or invalid\n", paActionInfo.getAction());
      break;
  }

  return retVal;
}

UA_StatusCode COPC_UA_Local_Handler::uninitializeAction(CActionInfo &paActionInfo) {
  UA_StatusCode retVal = UA_STATUSCODE_BADINTERNALERROR;
  switch(paActionInfo.getAction()){
    case CActionInfo::eRead:
    case CActionInfo::eWrite:
    case CActionInfo::eCreateMethod:
    case CActionInfo::eCreateObject:
    case CActionInfo::eDeleteObject:
      referencedNodesDecrement(paActionInfo);
      retVal = UA_STATUSCODE_GOOD;
      break;
    default:
      DEVLOG_ERROR("[OPC UA LOCAL]: Action %d to be uninitialized is unknown or invalid\n", paActionInfo.getAction());
      break;
  }
  return retVal;
}

UA_StatusCode COPC_UA_Local_Handler::initializeVariable(CActionInfo &paActionInfo, bool paWrite) {
  UA_StatusCode retVal = UA_STATUSCODE_GOOD;

  size_t indexOfNodePair = 0;
  CSinglyLinkedList<UA_NodeId*> referencedNodes;
  const CIEC_ANY *variables = paWrite ? paActionInfo.getDataToSend() : paActionInfo.getDataToReceive();
  for(CSinglyLinkedList<CActionInfo::CNodePairInfo*>::Iterator itMain = paActionInfo.getNodePairInfo().begin();
      itMain != paActionInfo.getNodePairInfo().end() && UA_STATUSCODE_GOOD == retVal; ++itMain, indexOfNodePair++) {

    CSinglyLinkedList<UA_NodeId*> presentNodes;
    bool isNodePresent = false;
    retVal = getNode(**itMain, presentNodes, &isNodePresent);

    if(UA_STATUSCODE_GOOD == retVal) {
      if(isNodePresent) {
        retVal = handleExistingVariable(paActionInfo, **itMain, variables[indexOfNodePair], indexOfNodePair, paWrite);

        handlePresentNodes(presentNodes, referencedNodes, UA_STATUSCODE_GOOD != retVal);
      } else { //node does not exist
        //presentNodes shouldn't have any allocated NodeId at this point
        retVal = handleNonExistingVariable(paActionInfo, **itMain, variables[indexOfNodePair], indexOfNodePair, referencedNodes, paWrite);
      }
    }
  }

  //we add the references first even if it fails, since some nodes might have been created,
  //and/or some might have been already there, so deleting them will be taken care of by
  //the referencedNodesDecrement function later
  referencedNodesIncrement(referencedNodes, paActionInfo);

  if(UA_STATUSCODE_GOOD != retVal) {
    referencedNodesDecrement(paActionInfo);
  }

  for(CSinglyLinkedList<UA_NodeId*>::Iterator itRerencedNodes = referencedNodes.begin(); itRerencedNodes != referencedNodes.end(); ++itRerencedNodes) {
    UA_NodeId_delete(*itRerencedNodes);
  }
  return retVal;
}

UA_StatusCode COPC_UA_Local_Handler::handleExistingVariable(CActionInfo &paActionInfo, CActionInfo::CNodePairInfo &paNodePairInfo, const CIEC_ANY &paVariable,
    size_t paIndexOfNodePair, bool paWrite) {

  UA_NodeId outDataType;
  UA_StatusCode retVal = UA_Server_readDataType(mUaServer, *paNodePairInfo.mNodeId, &outDataType);

  if(UA_STATUSCODE_GOOD == retVal) {
    if(UA_NodeId_equal(&outDataType, &COPC_UA_Helper::getOPCUATypeFromAny(paVariable)->typeId)) {
      if(!paWrite) { //If we are reading a variable, it should be writable from the outside
        retVal = addWritePermission(*paNodePairInfo.mNodeId);
        if(UA_STATUSCODE_GOOD == retVal) {
          void *handle = 0;
          retVal = UA_Server_getNodeContext(mUaServer, *paNodePairInfo.mNodeId, &handle);
          if(UA_STATUSCODE_GOOD == retVal) {
            if(!handle) {
              retVal = registerVariableCallBack(*paNodePairInfo.mNodeId, paActionInfo, paIndexOfNodePair);
            } else {
              DEVLOG_ERROR("[OPC UA LOCAL]: At FB %s RD_%d the node %s has already a FB who is reading from it. Cannot add another one\n",
                paActionInfo.getLayer().getCommFB()->getInstanceName(), paIndexOfNodePair, paNodePairInfo.mBrowsePath.getValue());
              retVal = UA_STATUSCODE_BADUNEXPECTEDERROR;
            }
          } else {
            DEVLOG_ERROR("[OPC UA LOCAL]: At FB %s RD_%d the node %s could not retrieve context. Error: %s\n",
                paActionInfo.getLayer().getCommFB()->getInstanceName(), paIndexOfNodePair, paNodePairInfo.mBrowsePath.getValue()), UA_StatusCode_name(retVal);
          }
        } else {
          DEVLOG_ERROR("[OPC UA LOCAL]: Cannot set write permission of node for port %d", paIndexOfNodePair);
        }
      }
    } else {
      DEVLOG_ERROR("[OPC UA LOCAL]: At FB %s index %d there was the type of the existing node doesn't match the new one",
        paActionInfo.getLayer().getCommFB()->getInstanceName(), paIndexOfNodePair);
      retVal = UA_STATUSCODE_BADUNEXPECTEDERROR;
    }

  } else {
    DEVLOG_ERROR("[OPC UA LOCAL]: At FB %s index %d there was a problem reading the type of the existing node",
      paActionInfo.getLayer().getCommFB()->getInstanceName(), paIndexOfNodePair);
  }

  return retVal;
}

UA_StatusCode COPC_UA_Local_Handler::handleNonExistingVariable(CActionInfo &paActionInfo, CActionInfo::CNodePairInfo &paNodePairInfo,
    const CIEC_ANY &paVariable, size_t paIndexOfNodePair, CSinglyLinkedList<UA_NodeId*> &paReferencedNodes, bool paWrite) {

  CIEC_STRING nodeName;
  UA_StatusCode retVal = splitAndCreateFolders(paNodePairInfo.mBrowsePath, nodeName, paReferencedNodes);
  if(UA_STATUSCODE_GOOD == retVal) {
    CCreateVariableInfo variableInformation;
    initializeCreateInfo(nodeName, paNodePairInfo, paReferencedNodes.isEmpty() ? 0 : *(paReferencedNodes.back()), variableInformation);

    variableInformation.mTypeConvert = COPC_UA_Helper::getOPCUATypeFromAny(paVariable);
    variableInformation.mInitData = &paVariable;
    variableInformation.mAllowWrite = !paWrite; // write FB here means that from the outside should not be possible to write and the other way around for read

    retVal = createVariableNode(variableInformation);
    if(UA_STATUSCODE_GOOD == retVal) {
      UA_NodeId *tmp = UA_NodeId_new();
      UA_NodeId_copy(variableInformation.mReturnedNodeId, tmp);
      paReferencedNodes.pushBack(tmp);
      if(!paWrite) {
        retVal = registerVariableCallBack(*variableInformation.mReturnedNodeId, paActionInfo, paIndexOfNodePair);
      }
      if(UA_STATUSCODE_GOOD == retVal) {
        if(!paNodePairInfo.mNodeId) {
          paNodePairInfo.mNodeId = UA_NodeId_new();
          UA_NodeId_copy(variableInformation.mReturnedNodeId, paNodePairInfo.mNodeId);
        }
      }
    }
  }

  return retVal;
}

UA_StatusCode COPC_UA_Local_Handler::createVariableNode(CCreateVariableInfo &paCreateVariableInfo) {

  UA_NodeId requestedNodeId;
  if(paCreateVariableInfo.mRequestedNodeId) {
    UA_NodeId_copy(paCreateVariableInfo.mRequestedNodeId, &requestedNodeId);
  } else {
    requestedNodeId = UA_NODEID_NUMERIC(paCreateVariableInfo.mBrowseName->namespaceIndex, 0); //if not provided, let the server generate the NodeId using the namespace from the browsename
  }
  UA_NodeId parentNodeId;
  if(paCreateVariableInfo.mParentNodeId) {
    UA_NodeId_copy(paCreateVariableInfo.mParentNodeId, &parentNodeId);
  } else {
    parentNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_ROOTFOLDER);
    DEVLOG_WARNING(
      "[OPC UA LOCAL]: You are creating a variable %.*s in the /Root folder. This is not a good practice. Try to create them in the /Root/Objects folder\n",
      paCreateVariableInfo.mBrowseName->name.length, reinterpret_cast<const char*>(paCreateVariableInfo.mBrowseName->name.data));
  }

  void *paVarValue = UA_new(paCreateVariableInfo.mTypeConvert);

  UA_init(paVarValue, paCreateVariableInfo.mTypeConvert);
  COPC_UA_Helper::convertToOPCUAType(*paCreateVariableInfo.mInitData, paVarValue);

  // create variable attributes
  UA_VariableAttributes variableAttributes;
  UA_VariableAttributes_init(&variableAttributes);
  variableAttributes.dataType = paCreateVariableInfo.mTypeConvert->typeId;
  variableAttributes.valueRank = -1; // value is a scalar
  variableAttributes.displayName.locale = UA_STRING_ALLOC(mEnglishLocaleForNodes);
  UA_String_copy(&paCreateVariableInfo.mBrowseName->name, &variableAttributes.displayName.text);
  variableAttributes.description = UA_LOCALIZEDTEXT_ALLOC(mEnglishLocaleForNodes, mDefaultDescriptionForVariableNodes);
  variableAttributes.userAccessLevel = UA_ACCESSLEVELMASK_READ;
  if(paCreateVariableInfo.mAllowWrite) {
    variableAttributes.userAccessLevel |= UA_ACCESSLEVELMASK_WRITE;
  }
  variableAttributes.accessLevel = variableAttributes.userAccessLevel;
  UA_Variant_setScalar(&variableAttributes.value, paVarValue, paCreateVariableInfo.mTypeConvert);

  UA_StatusCode retVal = UA_Server_addVariableNode(mUaServer, // server
    requestedNodeId, // requestedNewNodeId
    parentNodeId, // parentNodeId
    UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT), // referenceTypeId   Reference to the type definition for the variable node
    *paCreateVariableInfo.mBrowseName, // browseName
    UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE), // typeDefinition
    variableAttributes, // Variable attributes
    0, // instantiation callback
    paCreateVariableInfo.mReturnedNodeId); // return Node Id

  if(UA_STATUSCODE_GOOD != retVal) {
    DEVLOG_ERROR("[OPC UA LOCAL]: AddressSpace adding Variable Node %s failed. Error: %s\n", paCreateVariableInfo.mBrowseName->name.data,
      UA_StatusCode_name(retVal));
  }
  UA_NodeId_deleteMembers(&parentNodeId);
  UA_NodeId_deleteMembers(&requestedNodeId);
  UA_VariableAttributes_deleteMembers(&variableAttributes);
  return retVal;
}

UA_StatusCode COPC_UA_Local_Handler::updateNodeValue(const UA_NodeId &paNodeId, const CIEC_ANY *paData) {
  UA_Variant *nodeValue = UA_Variant_new();
  UA_Variant_init(nodeValue);

  const UA_DataType* dataType = COPC_UA_Helper::getOPCUATypeFromAny(*paData);
  void *varValue = UA_new(dataType);
  COPC_UA_Helper::convertToOPCUAType(*paData, varValue);
  UA_Variant_setScalarCopy(nodeValue, varValue, dataType);
  UA_StatusCode retVal = UA_Server_writeValue(mUaServer, paNodeId, *nodeValue);
  UA_delete(varValue, dataType);
  UA_Variant_delete(nodeValue);
  return retVal;
}

UA_StatusCode COPC_UA_Local_Handler::registerVariableCallBack(const UA_NodeId &paNodeId, CActionInfo &paActionInfo, size_t paPortIndex) {

  UA_StatusCode retVal = UA_Server_setVariableNode_valueCallback(mUaServer, paNodeId, { 0, COPC_UA_Local_Handler::CUA_LocalCallbackFunctions::onWrite });
  if(UA_STATUSCODE_GOOD == retVal) {

    UA_VariableContext_Handle variableContext(paActionInfo, paPortIndex);
    mNodeCallbackHandles.pushFront(variableContext);
    CSinglyLinkedList<UA_VariableContext_Handle>::Iterator contextIterator = mNodeCallbackHandles.begin();

    if(UA_STATUSCODE_GOOD != UA_Server_setNodeContext(mUaServer, paNodeId, &(*contextIterator))) {
      DEVLOG_ERROR("[OPC UA LOCAL]: Could not set callback context for node. Error: %s\n", UA_StatusCode_name(retVal));
      mNodeCallbackHandles.popFront();
      retVal = UA_STATUSCODE_BADUNEXPECTEDERROR;
    }
  } else {
    DEVLOG_ERROR("[OPC UA LOCAL]: Could not set callback function for node. Error: %s\n", UA_StatusCode_name(retVal));
    retVal = UA_STATUSCODE_BADUNEXPECTEDERROR;
  }
  return retVal;
}

UA_StatusCode COPC_UA_Local_Handler::addWritePermission(const UA_NodeId &paNodeId) {
  UA_StatusCode retVal = UA_Server_writeAccessLevel(mUaServer, paNodeId, UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE);
  if(UA_STATUSCODE_GOOD != retVal) {
    DEVLOG_WARNING("[OPC UA LOCAL]: Cannot set write permission of node\n");
  }
  return retVal;
}

UA_StatusCode COPC_UA_Local_Handler::initializeCreateMethod(CActionInfo &paActionInfo) {

  CSinglyLinkedList<UA_NodeId*> referencedNodes;
  CSinglyLinkedList<UA_NodeId*> presentNodes;

  CSinglyLinkedList<CActionInfo::CNodePairInfo*>::Iterator itMethodNodePairInfo = paActionInfo.getNodePairInfo().begin();

  bool isNodePresent = false;
  UA_StatusCode retVal = getNode(**itMethodNodePairInfo, presentNodes, &isNodePresent);

  if(UA_STATUSCODE_GOOD == retVal) {
    if(isNodePresent) {

      UA_NodeId parent = UA_NODEID_NUMERIC(0, UA_NS0ID_ROOTFOLDER);

      //look for parent object
      for(CSinglyLinkedList<UA_NodeId*>::Iterator itPresentNodes = presentNodes.begin(); itPresentNodes != presentNodes.end();) {
        CSinglyLinkedList<UA_NodeId*>::Iterator currentIterator = itPresentNodes;
        ++itPresentNodes;
        if(itPresentNodes == presentNodes.back()) {
          parent = **currentIterator;
          break;
        }
      }

      retVal = handleExistingMethod(paActionInfo, parent);

      handlePresentNodes(presentNodes, referencedNodes, UA_STATUSCODE_GOOD != retVal);
    } else { //node does not exist
      //presentNodes shouldn't have any allocated NodeId at this point

      CIEC_STRING nodeName;
      retVal = splitAndCreateFolders((*itMethodNodePairInfo)->mBrowsePath, nodeName, referencedNodes);
      if(UA_STATUSCODE_GOOD == retVal) {
        CCreateMethodInfo methodInformation(static_cast<CLocalMethodInfo&>(paActionInfo));
        initializeCreateInfo(nodeName, (**itMethodNodePairInfo), referencedNodes.isEmpty() ? 0 : *(referencedNodes.back()), methodInformation);

        methodInformation.mOutputSize = paActionInfo.getSendSize();
        methodInformation.mInputSize = paActionInfo.getReceiveSize();

        createMethodArguments(paActionInfo, methodInformation);

        retVal = createMethodNode(methodInformation, &(*itMethodNodePairInfo)->mNodeId);
        if(UA_STATUSCODE_GOOD == retVal) {
          UA_NodeId *tmp = UA_NodeId_new();
          UA_NodeId_copy(methodInformation.mReturnedNodeId, tmp);
          referencedNodes.pushBack(tmp);
        }
      }
    }
  }

  //we add the references first even if it fails, since some nodes might have been created,
  //and/or some might have been already there, so deleting them will be taken care of by
  //the referencedNodesDecrement function later
  referencedNodesIncrement(referencedNodes, paActionInfo);

  if(UA_STATUSCODE_GOOD != retVal) {
    referencedNodesDecrement(paActionInfo);
  }

  for(CSinglyLinkedList<UA_NodeId*>::Iterator itRerencedNodes = referencedNodes.begin(); itRerencedNodes != referencedNodes.end(); ++itRerencedNodes) {
    UA_NodeId_delete(*itRerencedNodes);
  }
  return retVal;
}

UA_StatusCode COPC_UA_Local_Handler::handleExistingMethod(CActionInfo &paActionInfo, UA_NodeId &paParentNode) {
  CSinglyLinkedList<CActionInfo::CNodePairInfo*>::Iterator it = paActionInfo.getNodePairInfo().begin();
  UA_StatusCode retVal = UA_STATUSCODE_GOOD;

  DEVLOG_INFO("[OPC UA LOCAL]: Adding a callback for an existing method at %s\n", (*it)->mBrowsePath.getValue());

  //check if the method was already referenced by another FB
  for(CSinglyLinkedList<UA_ParentNodeHandler>::Iterator iter = mMethodsContexts.begin(); iter != mMethodsContexts.end(); ++iter) {
    if(UA_NodeId_equal(&paParentNode, (*iter).mParentNodeId) && UA_NodeId_equal((*it)->mNodeId, (*iter).mMethodNodeId)) {
      DEVLOG_ERROR(
        "[OPC UA LOCAL]: The FB %s is trying to reference a local method at %s which has already a FB who is referencing it. Cannot add another one\n",
        paActionInfo.getLayer().getCommFB()->getInstanceName(), (*it)->mBrowsePath.getValue());
      retVal = UA_STATUSCODE_BADINTERNALERROR;
      break;
    }
  }

  //TODO: check types of existing method to this layer

  if(UA_STATUSCODE_GOOD == retVal) {
    retVal = UA_Server_setMethodNode_callback(mUaServer, *(*it)->mNodeId, COPC_UA_Local_Handler::CUA_LocalCallbackFunctions::onServerMethodCall);
    if(UA_STATUSCODE_GOOD == retVal) {
      retVal = UA_Server_setNodeContext(mUaServer, *(*it)->mNodeId, &mMethodsContexts);
      if(UA_STATUSCODE_GOOD == retVal) {
        UA_ParentNodeHandler parentNodeContext(paParentNode, (*it)->mNodeId, static_cast<CLocalMethodInfo&>(paActionInfo));
        mMethodsContexts.pushBack(parentNodeContext);
      } else {
        DEVLOG_ERROR("[OPC UA LOCAL]: Could not set context function for method at %s. Error: %s\n", paActionInfo.getLayer().getCommFB()->getInstanceName(),
          UA_StatusCode_name(retVal));
      }
    } else {
      DEVLOG_ERROR("[OPC UA LOCAL]: Could not set callback function for method at %s. Error: %s\n", paActionInfo.getLayer().getCommFB()->getInstanceName(),
        UA_StatusCode_name(retVal));
    }
  }
  return retVal;
}

void COPC_UA_Local_Handler::createMethodArguments(CActionInfo &paActionInfo, CCreateMethodInfo &paCreateMethodInfo) {
  const SFBInterfaceSpec *interfaceFB = paActionInfo.getLayer().getCommFB()->getFBInterfaceSpec();

  const CIEC_ANY *dataToSend = paActionInfo.getDataToSend();
  CIEC_ANY *dataToReceive = paActionInfo.getDataToReceive();

  paCreateMethodInfo.mOutputArguments = static_cast<UA_Argument *>(UA_Array_new(paCreateMethodInfo.mOutputSize, &UA_TYPES[UA_TYPES_ARGUMENT]));
  paCreateMethodInfo.mInputArguments = static_cast<UA_Argument *>(UA_Array_new(paCreateMethodInfo.mInputSize, &UA_TYPES[UA_TYPES_ARGUMENT]));

  for(size_t i = 0; i < paCreateMethodInfo.mOutputSize + paCreateMethodInfo.mInputSize; i++) {
    UA_Argument *arg;
    if(i < paCreateMethodInfo.mOutputSize) {
      arg = &(paCreateMethodInfo.mOutputArguments)[i];
      UA_Argument_init(arg);
      arg->name = UA_STRING_ALLOC(CStringDictionary::getInstance().get(interfaceFB->m_aunDINames[i + 2])); //we store the names of the SDs/RDs as names for the arguments names. Not so nice. + 2 skips the QI and ID
      arg->dataType = COPC_UA_Helper::getOPCUATypeFromAny(dataToSend[i])->typeId;
    } else {
      arg = &(paCreateMethodInfo.mInputArguments)[i - paCreateMethodInfo.mOutputSize];
      UA_Argument_init(arg);
      arg->name = UA_STRING_ALLOC(CStringDictionary::getInstance().get(interfaceFB->m_aunDONames[i - paCreateMethodInfo.mOutputSize + 2])); // + 2 skips the QO and STATUS
      arg->dataType = COPC_UA_Helper::getOPCUATypeFromAny(dataToReceive[i - paCreateMethodInfo.mOutputSize])->typeId;
    }

    arg->arrayDimensionsSize = 0;
    arg->arrayDimensions = 0;
    arg->description = UA_LOCALIZEDTEXT_ALLOC(mEnglishLocaleForNodes, "Method parameter");
    arg->valueRank = -1;
  }
}

UA_StatusCode COPC_UA_Local_Handler::createMethodNode(CCreateMethodInfo &paCreateMethodInfo, UA_NodeId **paNodeId) {

  UA_NodeId requestedNodeId;
  if(paCreateMethodInfo.mRequestedNodeId) {
    UA_NodeId_copy(paCreateMethodInfo.mRequestedNodeId, &requestedNodeId);
  } else {
    requestedNodeId = UA_NODEID_NUMERIC(paCreateMethodInfo.mBrowseName->namespaceIndex, 0); //if not provided, let the server generate the NodeId using the namespace from the browsename
  }

  UA_NodeId parentNodeId;
  if(paCreateMethodInfo.mParentNodeId) {
    UA_NodeId_copy(paCreateMethodInfo.mParentNodeId, &parentNodeId);
  } else {
    parentNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_ROOTFOLDER);
    DEVLOG_WARNING(
      "[OPC UA LOCAL]: You are creating a Method %.*s in the /Root folder. This is not a good practice. Try to create them in the /Root/Objects folder\n",
      paCreateMethodInfo.mBrowseName->name.length, reinterpret_cast<const char*>(paCreateMethodInfo.mBrowseName->name.data));
  }

  UA_MethodAttributes methodAttributes;
  UA_MethodAttributes_init(&methodAttributes);
  methodAttributes.description = UA_LOCALIZEDTEXT_ALLOC(mEnglishLocaleForNodes, "Method which can be called");
  methodAttributes.executable = true;
  methodAttributes.userExecutable = true;
  methodAttributes.displayName.locale = UA_STRING_ALLOC(mEnglishLocaleForNodes);
  UA_String_copy(&paCreateMethodInfo.mBrowseName->name, &methodAttributes.displayName.text);

  UA_StatusCode retVal = UA_Server_addMethodNode(mUaServer, requestedNodeId, parentNodeId, UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
    *paCreateMethodInfo.mBrowseName, methodAttributes, COPC_UA_Local_Handler::CUA_LocalCallbackFunctions::onServerMethodCall, paCreateMethodInfo.mInputSize,
    paCreateMethodInfo.mInputArguments, paCreateMethodInfo.mOutputSize, paCreateMethodInfo.mOutputArguments, &mMethodsContexts,
    paCreateMethodInfo.mReturnedNodeId);

  if(UA_STATUSCODE_GOOD == retVal) {
    if(!*paNodeId) {
      *paNodeId = UA_NodeId_new();
      UA_NodeId_copy(paCreateMethodInfo.mReturnedNodeId, *paNodeId);
    }

    UA_ParentNodeHandler parentNodeContext(parentNodeId, *paNodeId, paCreateMethodInfo.mLocalMethodInfo);
    mMethodsContexts.pushBack(parentNodeContext);
  } else {
    DEVLOG_ERROR("[OPC UA LOCAL]: OPC UA could not create method at %s. Error: %s\n",
      paCreateMethodInfo.mLocalMethodInfo.getLayer().getCommFB()->getInstanceName(), UA_StatusCode_name(retVal));
  }

  UA_NodeId_deleteMembers(&parentNodeId);
  UA_NodeId_deleteMembers(&requestedNodeId);
  UA_MethodAttributes_deleteMembers(&methodAttributes);
  return retVal;
}

UA_StatusCode COPC_UA_Local_Handler::initializeCreateObject(CActionInfo &paActionInfo) {
  //The main process is done in the execution
  UA_StatusCode retVal = UA_STATUSCODE_GOOD;

  CSinglyLinkedList<CActionInfo::CNodePairInfo*>::Iterator it = paActionInfo.getNodePairInfo().begin();
  ++it;

  if("" == (*it)->mBrowsePath) { //the browsename of the instance is mandatory
    retVal = UA_STATUSCODE_BADINTERNALERROR;
  }
  return retVal;
}

UA_StatusCode COPC_UA_Local_Handler::initializeDeleteObject(CActionInfo &) {
  //nothing to do here
  return UA_STATUSCODE_GOOD;
}

UA_StatusCode COPC_UA_Local_Handler::executeWrite(CActionInfo &paActionInfo) {
  UA_StatusCode retVal = UA_STATUSCODE_GOOD;
  const CIEC_ANY *dataToSend = paActionInfo.getDataToSend();
  size_t indexOfNodePair = 0;
  for(CSinglyLinkedList<CActionInfo::CNodePairInfo*>::Iterator it = paActionInfo.getNodePairInfo().begin(); it != paActionInfo.getNodePairInfo().end();
      ++it, indexOfNodePair++) {

    if(UA_STATUSCODE_GOOD != updateNodeValue(*(*it)->mNodeId, &dataToSend[indexOfNodePair])) {
      DEVLOG_ERROR("[OPC UA LOCAL]: Could not convert value to write for port %d at FB %s.\n", indexOfNodePair,
        paActionInfo.getLayer().getCommFB()->getInstanceName());
      retVal = UA_STATUSCODE_BADINTERNALERROR;
      break;
    }

  }
  return retVal;
}

UA_StatusCode COPC_UA_Local_Handler::executeCreateMethod(CActionInfo &paActionInfo) {
  //This is the return of a local method call, when RSP is triggered

  UA_StatusCode retVal = UA_STATUSCODE_BADUNEXPECTEDERROR;
  CLocalMethodCall *localMethodCall = getLocalMethodCall(static_cast<CLocalMethodInfo&>(paActionInfo));
  if(localMethodCall) {
    const CIEC_ANY *dataToSend = paActionInfo.getDataToSend();
    // copy SD values to output
    for(size_t i = 0; i < localMethodCall->mSendHandle.mSize; i++) {
      COPC_UA_Helper::fillVariant(*localMethodCall->mSendHandle.mData[i], dataToSend[i]);
    }

    localMethodCall->mActionInfo.getResultReady().inc();
    retVal = UA_STATUSCODE_GOOD;
  } else {
    DEVLOG_ERROR("[OPC UA LOCAL]: The method being returned hasn't been called before of FB %s\n", paActionInfo.getLayer().getCommFB()->getInstanceName());
  }

  return retVal;
}

UA_StatusCode COPC_UA_Local_Handler::executeCreateObject(CActionInfo &paActionInfo) {
  UA_StatusCode retVal = UA_STATUSCODE_BADINTERNALERROR;

  CSinglyLinkedList<CActionInfo::CNodePairInfo*>::Iterator itTypeNodePairInfo = paActionInfo.getNodePairInfo().begin();

  //look for type first
  if(isTypeOfObjectPresent(**itTypeNodePairInfo)) {

    CSinglyLinkedList<CActionInfo::CNodePairInfo*>::Iterator itInstance = paActionInfo.getNodePairInfo().begin();
    ++itInstance;

    CSinglyLinkedList<UA_NodeId*> referencedNodes;
    bool isNodePresent = false;
    //check if an instance is already present
    retVal = getNode(**itInstance, referencedNodes, &isNodePresent);

    if(UA_STATUSCODE_GOOD == retVal) {
      if(!isNodePresent) {

        CIEC_STRING nodeName;
        retVal = splitAndCreateFolders((*itInstance)->mBrowsePath, nodeName, referencedNodes);
        if(UA_STATUSCODE_GOOD == retVal) {
          CCreateObjectInfo createInformation;

          initializeCreateInfo(nodeName, (**itInstance), referencedNodes.isEmpty() ? 0 : *(referencedNodes.back()), createInformation);
          createInformation.mTypeNodeId = (*itTypeNodePairInfo)->mNodeId;

          retVal = createObjectNode(createInformation);

          if(UA_STATUSCODE_GOOD == retVal) {
            UA_NodeId *tmp = UA_NodeId_new();
            UA_NodeId_copy(createInformation.mReturnedNodeId, tmp);
            referencedNodes.pushBack(tmp);
          }
        }
      } else {
        DEVLOG_ERROR("[OPC UA LOCAL]: You are trying to create a node which already exists at FB %s\n", paActionInfo.getLayer().getCommFB()->getInstanceName());
        retVal = UA_STATUSCODE_BADINTERNALERROR;
      }
    }

    //we add the references first even if itType fails, since some nodes might have been created,
    //and/or some might have been already there, so deleting them will be taken care of by
    //the referencedNodesDecrement function later
    referencedNodesIncrement(referencedNodes, paActionInfo);

    if(UA_STATUSCODE_GOOD != retVal) {
      referencedNodesDecrement(paActionInfo);
    }

    for(CSinglyLinkedList<UA_NodeId*>::Iterator itRerencedNodes = referencedNodes.begin(); itRerencedNodes != referencedNodes.end(); ++itRerencedNodes) {
      UA_NodeId_delete(*itRerencedNodes);
    }

  } else {
    DEVLOG_ERROR("[OPC UA LOCAL]: The type of the object to create could not be found \n", paActionInfo.getLayer().getCommFB()->getInstanceName());
  }

  return retVal;
}

UA_StatusCode COPC_UA_Local_Handler::createObjectNode(CCreateObjectInfo &paCreateVariableInfo) {

  UA_StatusCode retVal = UA_STATUSCODE_BADINTERNALERROR;

  UA_NodeId requestedNodeId;
  if(paCreateVariableInfo.mRequestedNodeId) {
    UA_NodeId_copy(paCreateVariableInfo.mRequestedNodeId, &requestedNodeId);
  } else {
    requestedNodeId = UA_NODEID_NUMERIC(paCreateVariableInfo.mBrowseName->namespaceIndex, 0); //if not provided, let the server generate the NodeId using the namespace from the browsename
  }

  UA_NodeId parentNodeId;
  if(paCreateVariableInfo.mParentNodeId) {
    UA_NodeId_copy(paCreateVariableInfo.mParentNodeId, &parentNodeId);
  } else {
    parentNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_ROOTFOLDER);
    DEVLOG_WARNING(
      "[OPC UA LOCAL]: You are creating an Object %.*s in the /Root folder. This is not a good practice. Try to create them in the /Root/Objects folder\n",
      paCreateVariableInfo.mBrowseName->name.length, reinterpret_cast<const char*>(paCreateVariableInfo.mBrowseName->name.data));
  }

  CIEC_STRING nodeName;
  nodeName.assign(reinterpret_cast<const char*>(paCreateVariableInfo.mBrowseName->name.data),
    static_cast<TForteUInt16>(paCreateVariableInfo.mBrowseName->name.length));

  UA_ObjectAttributes oAttr;
  UA_ObjectAttributes_init(&oAttr);
  oAttr.description = UA_LOCALIZEDTEXT_ALLOC("", nodeName.getValue());
  oAttr.displayName = UA_LOCALIZEDTEXT_ALLOC("", nodeName.getValue());
  retVal = UA_Server_addObjectNode(mUaServer, requestedNodeId, parentNodeId, UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT), *paCreateVariableInfo.mBrowseName,
    *paCreateVariableInfo.mTypeNodeId, oAttr, 0, paCreateVariableInfo.mReturnedNodeId);
  if(UA_STATUSCODE_GOOD != retVal) {
    DEVLOG_ERROR("[OPC UA LOCAL]: Could not addObjectNode. Error: %s\n", UA_StatusCode_name(retVal));
  }
  UA_NodeId_deleteMembers(&requestedNodeId);
  UA_ObjectAttributes_deleteMembers(&oAttr);

  return retVal;
}

bool COPC_UA_Local_Handler::isTypeOfObjectPresent(CActionInfo::CNodePairInfo &paNodePairInfo) {
  bool isNodePresent = false;
  CSinglyLinkedList<UA_NodeId*> referencedNodes;

  UA_StatusCode retVal = getNode(paNodePairInfo, referencedNodes, &isNodePresent);

  if(UA_STATUSCODE_GOOD == retVal) {
    if(isNodePresent) {
      isNodePresent = true;
    }
    //we don't need the nodes from the type
    for(CSinglyLinkedList<UA_NodeId*>::Iterator itPresentNodes = referencedNodes.begin(); itPresentNodes != referencedNodes.end(); ++itPresentNodes) {
      UA_NodeId_delete(*itPresentNodes);
    }
    referencedNodes.clearAll();
  }
  return isNodePresent;
}

UA_StatusCode COPC_UA_Local_Handler::executeDeleteObject(CActionInfo &paActionInfo) {

  bool isNodePresent = false;
  CSinglyLinkedList<CActionInfo::CNodePairInfo*>::Iterator itInstance = paActionInfo.getNodePairInfo().begin();
  CSinglyLinkedList<UA_NodeId*> referencedNodes;

  //look for instance
  UA_StatusCode retVal = getNode(**itInstance, referencedNodes, &isNodePresent);

  if(UA_STATUSCODE_GOOD == retVal) {
    if(isNodePresent) {
      retVal = UA_Server_deleteNode(mUaServer, *(*itInstance)->mNodeId, true);
    } else {
      DEVLOG_ERROR("[OPC UA LOCAL]: The instance of the object to delete could not be found for FB %s\n",
        paActionInfo.getLayer().getCommFB()->getInstanceName());
      retVal = UA_STATUSCODE_BADINTERNALERROR;
    }

    for(CSinglyLinkedList<UA_NodeId*>::Iterator itPresendNodes = referencedNodes.begin(); itPresendNodes != referencedNodes.end(); ++itPresendNodes) {
      UA_NodeId_delete(*itPresendNodes);
    }
    referencedNodes.clearAll();
  }

  return retVal;
}

void COPC_UA_Local_Handler::initializeCreateInfo(CIEC_STRING &paNodeName, CActionInfo::CNodePairInfo &paNodePairInfo, const UA_NodeId *paParentNodeId,
    CCreateInfo &paResult) {
  COPC_UA_Helper::getBrowsenameFromNodeName(paNodeName.getValue(), scmDefaultBrowsenameNameSpace, *paResult.mBrowseName); //this cannot fail here anymore, since it was checked already with getNode
  paResult.mRequestedNodeId = paNodePairInfo.mNodeId;
  paResult.mParentNodeId = paParentNodeId;
}

UA_StatusCode COPC_UA_Local_Handler::getNode(CActionInfo::CNodePairInfo &paNodePairInfo, CSinglyLinkedList<UA_NodeId*> &paFoundNodeIds, bool *paIsPresent) {
  UA_StatusCode retVal = UA_STATUSCODE_GOOD;
  *paIsPresent = false;

  if("" != paNodePairInfo.mBrowsePath) {
    UA_BrowsePath *browsePaths = 0;
    size_t pathCount = 0;
    size_t firstNonExistingNode = 0;

    CSinglyLinkedList<UA_NodeId*> existingNodeIds;
    retVal = translateBrowseNameAndStore(paNodePairInfo.mBrowsePath.getValue(), &browsePaths, &pathCount, &firstNonExistingNode, existingNodeIds);
    if(UA_STATUSCODE_GOOD == retVal) {
      if(firstNonExistingNode == pathCount) { //all nodes exist
        *paIsPresent = true;
        if(paNodePairInfo.mNodeId) { //nodeID was provided
          if(!UA_NodeId_equal(paNodePairInfo.mNodeId, *existingNodeIds.back())) {
            *paIsPresent = false; // the found Node has not the same NodeId.
          }
        } else { //if no nodeID was provided, the found Node is stored in the nodePairInfo
          paNodePairInfo.mNodeId = UA_NodeId_new();
          UA_NodeId_copy(*existingNodeIds.back(), paNodePairInfo.mNodeId);
        }
      }

      for(CSinglyLinkedList<UA_NodeId*>::Iterator it = existingNodeIds.begin(); it != existingNodeIds.end(); ++it) {
        if(!*paIsPresent) {
          UA_NodeId_delete(*it);
        } else {
          paFoundNodeIds.pushBack((*it));
        }
      }
      COPC_UA_Helper::releaseBrowseArgument(*browsePaths, pathCount);
    }
  } else {
    void *handle = 0;
    //we use UA_Server_getNodeContext just to check if the node exist in the addressspace
    if(UA_STATUSCODE_BADNODEIDUNKNOWN != UA_Server_getNodeContext(mUaServer, *paNodePairInfo.mNodeId, &handle)) {
      *paIsPresent = true;
    }
  }

  return retVal;
}

UA_StatusCode COPC_UA_Local_Handler::translateBrowseNameAndStore(const char *paBrowsePath, UA_BrowsePath **paBrowsePaths, size_t *paFoldercount,
    size_t *paFirstNonExistingNode, CSinglyLinkedList<UA_NodeId*> &paFoundNodeIds) {

  UA_StatusCode retVal = COPC_UA_Helper::prepareBrowseArgument(paBrowsePath, paBrowsePaths, paFoldercount);

  if(UA_STATUSCODE_GOOD == retVal) {
    UA_BrowsePathResult *browsePathsResults = 0;
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

UA_StatusCode COPC_UA_Local_Handler::storeAlreadyExistingNodes(UA_BrowsePathResult *paBrowsePathsResults, size_t paFolderCnt, size_t *paFirstNonExistingNode,
    CSinglyLinkedList<UA_NodeId*> &paCreatedNodeIds) {
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
      return UA_STATUSCODE_BADINTERNALERROR;
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

UA_StatusCode COPC_UA_Local_Handler::createFolders(const char *paFolders, CSinglyLinkedList<UA_NodeId*> &paCreatedNodeIds) {

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

    // create all the nodes on the way
    for(size_t j = firstNonExistingNode; j < folderCnt; j++) {

      UA_NodeId type = UA_NODEID_NUMERIC(0, UA_NS0ID_FOLDERTYPE);
      CCreateObjectInfo createInformation;

      createInformation.mRequestedNodeId = 0;
      createInformation.mParentNodeId = &parentNodeId;
      UA_QualifiedName_copy(&browsePaths[folderCnt - 1].relativePath.elements[j].targetName, createInformation.mBrowseName);
      createInformation.mTypeNodeId = &type;

      retVal = createObjectNode(createInformation);

      if(UA_STATUSCODE_GOOD != retVal) {
        DEVLOG_ERROR("[OPC UA LOCAL]: Could not create folder %.*s in path %s. Error: %s\n", createInformation.mBrowseName->name.length,
          reinterpret_cast<const char*>(createInformation.mBrowseName->name.data), paFolders, UA_StatusCode_name(retVal));
        break;
      }

      UA_NodeId *tmp = UA_NodeId_new();
      UA_NodeId_copy(createInformation.mReturnedNodeId, tmp);
      createdNodeIds.pushBack(tmp);

      UA_NodeId_deleteMembers(&parentNodeId);
      UA_NodeId_copy(createInformation.mReturnedNodeId, &parentNodeId);
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

    COPC_UA_Helper::releaseBrowseArgument(*browsePaths, folderCnt);
  }
  return retVal;
}

UA_StatusCode COPC_UA_Local_Handler::splitAndCreateFolders(CIEC_STRING &paBrowsePath, CIEC_STRING &paNodeName,
    CSinglyLinkedList<UA_NodeId*> &paRreferencedNodes) {
  CIEC_STRING folders;
  UA_StatusCode retVal = UA_STATUSCODE_BADINTERNALERROR;
  if(splitFoldersFromNode(paBrowsePath, folders, paNodeName)) {
    retVal = UA_STATUSCODE_GOOD;
    if("" != folders) {
      retVal = createFolders(folders.getValue(), paRreferencedNodes);
    }
  }
  return retVal;
}

bool COPC_UA_Local_Handler::splitFoldersFromNode(const CIEC_STRING &paOriginal, CIEC_STRING &paFolder, CIEC_STRING &paNodeName) {

  bool retVal = false;

  if(COPC_UA_Helper::isBrowsePathValid(paOriginal.getValue())) {
    retVal = true;

    CIEC_STRING copyOfOriginal(paOriginal);

    char *begin = copyOfOriginal.getValue();
    char *runner = begin + copyOfOriginal.length() - 1;

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

void COPC_UA_Local_Handler::handlePresentNodes(CSinglyLinkedList<UA_NodeId*> &paPresentNodes, CSinglyLinkedList<UA_NodeId*> &paReferencedNodes, bool paFailed) {
  for(CSinglyLinkedList<UA_NodeId*>::Iterator itPresendNodes = paPresentNodes.begin(); itPresendNodes != paPresentNodes.end(); ++itPresendNodes) {
    if(paFailed) {
      UA_NodeId_delete(*itPresendNodes);
    } else {
      paReferencedNodes.pushBack(*itPresendNodes);
    }
  }
}

COPC_UA_Local_Handler::CLocalMethodCall& COPC_UA_Local_Handler::addMethodCall(CLocalMethodInfo &paActionInfo,
    COPC_UA_Helper::UA_SendVariable_handle &paHandleRecv) {
  CCriticalRegion criticalRegion(mMethodCallsMutex);
  mMethodCalls.pushBack(CLocalMethodCall(paActionInfo, paHandleRecv));
  CSinglyLinkedList<CLocalMethodCall>::Iterator justPushed = mMethodCalls.back();
  return *justPushed;
}

void COPC_UA_Local_Handler::removeMethodCall(CLocalMethodCall &toRemove) {
  CCriticalRegion criticalRegion(mMethodCallsMutex);
  mMethodCalls.erase(toRemove);
}

COPC_UA_Local_Handler::CLocalMethodCall *COPC_UA_Local_Handler::getLocalMethodCall(CLocalMethodInfo &paActionInfo) {
  CCriticalRegion criticalRegion(mMethodCallsMutex);
  CLocalMethodCall *retVal = 0;
  for(CSinglyLinkedList<CLocalMethodCall>::Iterator it = mMethodCalls.begin(); it != mMethodCalls.end(); ++it) {
    if(&(*it).mActionInfo == &paActionInfo) {
      retVal = &(*it);
      break;
    }
  }
  return retVal;
}

// ******************** CALLBACKS *************************

UA_StatusCode COPC_UA_Local_Handler::CUA_LocalCallbackFunctions::onServerMethodCall(UA_Server *, const UA_NodeId *, void *, const UA_NodeId *paMethodNodeId,
    void *paMethodContext, //NOSONAR
    const UA_NodeId *paParentNodeId, void *, //NOSONAR
    size_t paInputSize, const UA_Variant *paInput, size_t paOutputSize, UA_Variant *paOutput) {

  UA_StatusCode retVal = UA_STATUSCODE_BADUNEXPECTEDERROR;
  CLocalMethodInfo *localMethodHandle = 0;
  CSinglyLinkedList<UA_ParentNodeHandler> *methodsContexts = static_cast<CSinglyLinkedList<UA_ParentNodeHandler>*>(paMethodContext);
  for(CSinglyLinkedList<UA_ParentNodeHandler>::Iterator iter = methodsContexts->begin(); iter != methodsContexts->end(); ++iter) {
    if(UA_NodeId_equal(paParentNodeId, (*iter).mParentNodeId) && UA_NodeId_equal(paMethodNodeId, (*iter).mMethodNodeId)) {
      localMethodHandle = &(*iter).mActionInfo;
      break;
    }
  }

  if(!localMethodHandle) {
    DEVLOG_ERROR("[OPC UA LOCAL]: Method doesn't have any FB referencing it\n");
  } else {
    if(paInputSize != localMethodHandle->getReceiveSize() || paOutputSize != localMethodHandle->getSendSize()) {
      DEVLOG_ERROR("[OPC UA LOCAL]: method call got invalid number of arguments. In: %d==%d, Out: %d==%d\n", localMethodHandle->getReceiveSize(), paInputSize,
        localMethodHandle->getSendSize(), paOutput);
    } else {
      // other thread may currently create nodes for the same path, thus mutex
      CCriticalRegion criticalRegion(localMethodHandle->getMutex()); //TODO: do we need this mutex?

      COPC_UA_Helper::UA_SendVariable_handle sendHandle(paOutputSize);
      COPC_UA_Helper::UA_RecvVariable_handle recvHandle(paInputSize);

      for(size_t i = 0; i < paInputSize; i++) {
        recvHandle.mData[i] = &paInput[i];
      }

      for(size_t i = 0; i < paOutputSize; i++) {
        sendHandle.mData[i] = &paOutput[i];
      }

      // Handle return of receive mData
      if(e_ProcessDataOk == localMethodHandle->getLayer().recvData(static_cast<const void *>(&recvHandle), 0)) { //TODO: add multidimensional mData handling with 'range'.

        localMethodHandle->getLayer().getCommFB()->interruptCommFB(&localMethodHandle->getLayer());

        //when the method finishes, and RSP is triggered, sendHandle will be filled with the right information
        CLocalMethodCall &localMethodCall = ::getExtEvHandler<COPC_UA_Local_Handler>(*localMethodHandle->getLayer().getCommFB()).addMethodCall(
          *localMethodHandle, sendHandle);

        ::getExtEvHandler<COPC_UA_Local_Handler>(*localMethodHandle->getLayer().getCommFB()).startNewEventChain(localMethodHandle->getLayer().getCommFB());

        //wait For semaphore, which will be released by execute Local Method in this handler
        if(!localMethodHandle->getResultReady().timedWait(scmMethodCallTimeoutInNanoSeconds)) {
          DEVLOG_ERROR("[OPC UA LOCAL]: method call did not get result values within timeout of %d seconds.\n", scmMethodCallTimeoutInNanoSeconds);
          retVal = UA_STATUSCODE_BADTIMEOUT;
        } else {
          retVal = sendHandle.mFailed ? UA_STATUSCODE_BADUNEXPECTEDERROR : UA_STATUSCODE_GOOD;
        }

        ::getExtEvHandler<COPC_UA_Local_Handler>(*localMethodHandle->getLayer().getCommFB()).removeMethodCall(localMethodCall);
      }
    }
  }
  return retVal;
}

void COPC_UA_Local_Handler::CUA_LocalCallbackFunctions::onWrite(UA_Server *, const UA_NodeId *, void *, const UA_NodeId *, void *nodeContext, //NOSONAR
    const UA_NumericRange *, const UA_DataValue *data) {

  UA_VariableContext_Handle *variableCallbackHandle = static_cast<UA_VariableContext_Handle *>(nodeContext);

  COPC_UA_Helper::UA_RecvVariable_handle handleRecv(1);

  handleRecv.mData[0] = data->hasValue ? &data->value : 0; //TODO: check this empty data
  handleRecv.mOffset = variableCallbackHandle->mPortIndex;

  EComResponse retVal = variableCallbackHandle->mActionInfo.getLayer().recvData(static_cast<const void *>(&handleRecv), 0); //TODO: add multidimensional mData handling with 'range'.

  if(e_Nothing != retVal) {
    variableCallbackHandle->mActionInfo.getLayer().getCommFB()->interruptCommFB(&variableCallbackHandle->mActionInfo.getLayer());
    ::getExtEvHandler<COPC_UA_Local_Handler>(*variableCallbackHandle->mActionInfo.getLayer().getCommFB()).startNewEventChain(
      variableCallbackHandle->mActionInfo.getLayer().getCommFB());
  }
}
