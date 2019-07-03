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
  for(CSinglyLinkedList<CUA_ClientInformation*>::Iterator itClientInformation = mAllClients.begin(); itClientInformation != mAllClients.end();
      ++itClientInformation) {
    mConnectionHandler.removeClient(**itClientInformation);
    removeClientFromInitializedList(**itClientInformation);
    delete *itClientInformation;
  }
  mAllClients.clearAll();
}

void COPC_UA_Client_Handler::enableHandler(void) {
  startLoops();
}

void COPC_UA_Client_Handler::disableHandler(void) {
  stopLoops();
}

void COPC_UA_Client_Handler::resumeAsyncLoop() {
  mAsyncIsNeeded.inc();
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
  resumeAsyncLoop();
  end();
}

void COPC_UA_Client_Handler::addClientToInitializedList(CUA_ClientInformation& paClientInformation) {
  CCriticalRegion connectionListRegion(mNewClientstMutex);
  addClientToList(paClientInformation, mNewClients);
  if(paClientInformation.isAsyncNeeded()) { //for subscription the missing calls will be set already, for others not
    resumeAsyncLoop();
  }
  mNewClientsPresent = true;
}

void COPC_UA_Client_Handler::removeClientFromInitializedList(CUA_ClientInformation& paClientInformation) {
  CCriticalRegion iniitalizedClientsRegion(mInitializedClientstMutex);
  CCriticalRegion newClientsRegion(mNewClientstMutex);
  mNewClients.erase(&paClientInformation); //client could still be in the newList
  mInitializedClients.erase(&paClientInformation);
}

void COPC_UA_Client_Handler::updateClientList() {
  CCriticalRegion iniitalizedClientsRegion(mInitializedClientstMutex);
  CCriticalRegion newClientsRegion(mNewClientstMutex);
  for(CSinglyLinkedList<CUA_ClientInformation *>::Iterator itClientInformation = mNewClients.begin(); itClientInformation != mNewClients.end();
      ++itClientInformation) {
    addClientToList(**itClientInformation, mInitializedClients);
  }
  mNewClients.clearAll();
  mNewClientsPresent = false;
}

void COPC_UA_Client_Handler::addClientToConnectionList(CUA_ClientInformation& paClientInformation) {
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
  CSinglyLinkedList<CUA_ClientInformation *> failedClients;
  bool asyncIsNeeded = false;
  for(CSinglyLinkedList<CUA_ClientInformation *>::Iterator itClientInformation = mInitializedClients.begin(); itClientInformation != mInitializedClients.end();
      ++itClientInformation) {
    CCriticalRegion criticalRegionClienMutex((*itClientInformation)->getMutex());
    if((*itClientInformation)->isAsyncNeeded() && (*itClientInformation)->executeAsyncCalls()) {
      failedClients.pushBack(*itClientInformation);
    } else {
      asyncIsNeeded = (*itClientInformation)->isAsyncNeeded();
    }
    if(!isAlive()) {
      break;
    }
  }

  if(isAlive() && !failedClients.isEmpty()) {
    for(CSinglyLinkedList<CUA_ClientInformation *>::Iterator itClientInformation = failedClients.begin(); itClientInformation != failedClients.end();
        ++itClientInformation) {
      DEVLOG_ERROR("[OPC UA REMOTE]: There was a problem checking remote %s.\n", (*itClientInformation)->getEndpoint().getValue());
      (*itClientInformation)->resetMissingAsyncCalls();
      mInitializedClients.erase(*itClientInformation);
      addClientToConnectionList(**itClientInformation);
    }
  }
  return asyncIsNeeded;
}

void COPC_UA_Client_Handler::addClientToList(CUA_ClientInformation& paClientInformation, CSinglyLinkedList<CUA_ClientInformation *>& paList) {
  bool elementAlreadyPresent = false;
  for(CSinglyLinkedList<CUA_ClientInformation *>::Iterator itClientInformation = paList.begin(); itClientInformation != paList.end(); ++itClientInformation) {
    if(&paClientInformation == (*itClientInformation)) {
      elementAlreadyPresent = true;
      break;
    }
  }

  if(!elementAlreadyPresent) {
    paList.pushBack(&paClientInformation);
  }
}

CUA_ClientInformation* COPC_UA_Client_Handler::getClient(CIEC_STRING& paEndpoint) {
  CCriticalRegion allClientsRegion(mAllClientListMutex);

  CUA_ClientInformation* client = 0;

  for(CSinglyLinkedList<CUA_ClientInformation *>::Iterator itClientInformation = mAllClients.begin(); itClientInformation != mAllClients.end();
      ++itClientInformation) {
    if((*itClientInformation)->getEndpoint() == paEndpoint) {
      client = (*itClientInformation);
      break;
    }
  }
  if(!client) {

    client = new CUA_ClientInformation(paEndpoint);
    if(client->initialize()) {
      mAllClients.pushBack(client);
    } else {
      delete client;
      client = 0;
    }
  }

  return client;
}

void COPC_UA_Client_Handler::referencedClientsIncrement(CActionInfo& paActionInfo) {
  CCriticalRegion allClientsRegion(mAllClientListMutex);
  for(CSinglyLinkedList<CUA_ClientInformation *>::Iterator itClientInformation = mAllClients.begin(); itClientInformation != mAllClients.end();
      ++itClientInformation) {
    CCriticalRegion clientRegion((*itClientInformation)->getMutex());
    if((*itClientInformation)->getEndpoint() == paActionInfo.getEndpoint()) {
      (*itClientInformation)->addAction(paActionInfo);
      addClientToConnectionList(**itClientInformation);
      break;
    }
  }
}

void COPC_UA_Client_Handler::referencedClientsDecrement(CActionInfo& paActionInfo) {
  CCriticalRegion allClientsRegion(mAllClientListMutex);

  CUA_ClientInformation * clientToDelete = 0;
  for(CSinglyLinkedList<CUA_ClientInformation *>::Iterator itClientInformation = mAllClients.begin(); itClientInformation != mAllClients.end();
      ++itClientInformation) {
    CCriticalRegion clientRegion((*itClientInformation)->getMutex());
    if((*itClientInformation)->getEndpoint() == paActionInfo.getEndpoint()) {
      (*itClientInformation)->removeAction(paActionInfo);
      clientToDelete = *itClientInformation;
      break;
    }
  }

  if(clientToDelete && !clientToDelete->hasActions()) {
    removeClientFromAllLists(*clientToDelete);
  }
}

void COPC_UA_Client_Handler::removeClientFromAllLists(CUA_ClientInformation& paClientInformation) {
  mConnectionHandler.removeClient(paClientInformation);
  removeClientFromInitializedList(paClientInformation);
  mAllClients.erase(&paClientInformation);
  delete &paClientInformation;
}

UA_StatusCode COPC_UA_Client_Handler::initializeAction(CActionInfo& paActionInfo) {
  enableHandler();
  UA_StatusCode retVal = UA_STATUSCODE_BADINTERNALERROR;
  switch(paActionInfo.getAction()){
    case CActionInfo::eRead:
    case CActionInfo::eWrite:
    case CActionInfo::eCallMethod:
    case CActionInfo::eSubscribe:
      retVal = initializeActionInClient(paActionInfo);
      break;
    case CActionInfo::eCreateMethod:
    case CActionInfo::eCreateObject:
    case CActionInfo::eDeleteObject:
      DEVLOG_ERROR("[OPC UA REMOTE]: Cannot perform action %s remotely. Initialization failed\n",
        CActionInfo::mActionNames[paActionInfo.getAction()]);
      break;
    default:
      DEVLOG_ERROR("[OPC UA REMOTE]: Unknown action %d to be initialized\n", paActionInfo.getAction());
      break;
  }
  return retVal;
}

UA_StatusCode COPC_UA_Client_Handler::executeAction(CActionInfo& paActionInfo) {

  CUA_ClientInformation* clientInfo = getClient(paActionInfo.getEndpoint());
  UA_StatusCode retVal = UA_STATUSCODE_BADINTERNALERROR;

  if(clientInfo && clientInfo->isActionInitialized(paActionInfo)) {
    switch(paActionInfo.getAction()){
      case CActionInfo::eRead:
        retVal = clientInfo->executeRead(paActionInfo);
        break;
      case CActionInfo::eWrite:
        retVal = clientInfo->executeWrite(paActionInfo);
        break;
      case CActionInfo::eCallMethod:
        retVal = clientInfo->executeCallMethod(paActionInfo);
        break;
      default: //eCreateMethod, eCreateObject, eDeleteObject will never reach here since they weren't initialized. eSubscribe is a Subscribe FB
        DEVLOG_ERROR("[OPC UA REMOTE]: Action %d to be executed is unknown or invalid", paActionInfo.getAction());
        break;
    }
  } else {
    DEVLOG_ERROR("[OPC UA REMOTE]: Cannot execute action from FB %s. It was not properly initialized\n",
      paActionInfo.getLayer().getCommFB()->getInstanceName());
  }

  if(UA_STATUSCODE_GOOD == retVal) {
    resumeAsyncLoop();
  }

  return retVal;
}

UA_StatusCode COPC_UA_Client_Handler::uninitializeAction(CActionInfo& paActionInfo) {
  UA_StatusCode retVal = UA_STATUSCODE_BADINTERNALERROR;
  switch(paActionInfo.getAction()){
    case CActionInfo::eRead:
    case CActionInfo::eWrite:
    case CActionInfo::eCallMethod:
    case CActionInfo::eSubscribe:
      referencedClientsDecrement(paActionInfo);
      retVal = UA_STATUSCODE_GOOD;
      break;
    default:
      DEVLOG_ERROR("[OPC UA REMOTE]: Action %d to be uninitialized is unknown or invalid\n", paActionInfo.getAction());
      break;
  }
  return retVal;
}

UA_StatusCode COPC_UA_Client_Handler::initializeActionInClient(CActionInfo& paActionInfo) {
  UA_StatusCode retVal = UA_STATUSCODE_BADINTERNALERROR;
  CUA_ClientInformation* clientInfo = getClient(paActionInfo.getEndpoint());
  if(clientInfo) {
    referencedClientsIncrement(paActionInfo);
    retVal = UA_STATUSCODE_GOOD;
  }
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

void COPC_UA_Client_Handler::UA_ConnectionHandler::addClient(CUA_ClientInformation& paClientInformation) {
  CCriticalRegion connectionListRegion(mNewClientstMutex);
  addClientToList(paClientInformation, mNewClients);
  mClientPresent.inc();
  mNewClientsPresent = true;
}

void COPC_UA_Client_Handler::UA_ConnectionHandler::removeClient(CUA_ClientInformation& paClientInformation) {
  CCriticalRegion connectionListRegion(mClientstMutex);
  CCriticalRegion newClientsRegion(mNewClientstMutex);
  mNewClients.erase(&paClientInformation); //client could still be in the newList
  mClients.erase(&paClientInformation);
}

void COPC_UA_Client_Handler::UA_ConnectionHandler::updateClientList() {
  CCriticalRegion clientsClientsRegion(mClientstMutex);
  CCriticalRegion connectionListRegion(mNewClientstMutex);
  for(CSinglyLinkedList<CUA_ClientInformation *>::Iterator itClientInformation = mNewClients.begin(); itClientInformation != mNewClients.end();
      ++itClientInformation) {
    addClientToList(**itClientInformation, mClients);
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
  CSinglyLinkedList<CUA_ClientInformation *> clientsToRemove;
  bool needsRetry = false;
  for(CSinglyLinkedList<CUA_ClientInformation *>::Iterator itClientInformation = mClients.begin(); itClientInformation != mClients.end();
      ++itClientInformation) {
    CCriticalRegion clientRegion((*itClientInformation)->getMutex());
    if((*itClientInformation)->handleClientState()) {
      clientsToRemove.pushBack(*itClientInformation);
    } else {
      needsRetry = true;
    }
    if((*itClientInformation)->someActionWasInitialized()) {
      mClientHandler.addClientToInitializedList(**itClientInformation);
    }
    if((*itClientInformation)->needsAlreadyAsync()) {
      mClientHandler.resumeAsyncLoop();
    }
    if(!isAlive()) {
      break;
    }
  }

  if(isAlive() && !clientsToRemove.isEmpty()) {
    for(CSinglyLinkedList<CUA_ClientInformation *>::Iterator itClientInformation = clientsToRemove.begin(); itClientInformation != clientsToRemove.end();
        ++itClientInformation) {
      mClients.erase(*itClientInformation);
    }
  }
  return needsRetry;
}

