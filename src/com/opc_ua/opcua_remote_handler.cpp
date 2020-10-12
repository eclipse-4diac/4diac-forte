/*******************************************************************************
 * Copyright (c) 2018 - 2019 fortiss GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Jose Cabral, Kirill Dorofeev - initial implementation
 *******************************************************************************/

#include "opcua_remote_handler.h"
#include "../../core/devexec.h"
#include "../../core/iec61131_functions.h"
#include "../../core/cominfra/basecommfb.h"
#include <criticalregion.h>
#include <forte_printer.h>

using namespace forte::com_infra;

DEFINE_HANDLER(COPC_UA_Remote_Handler);

COPC_UA_Client_IterationList::COPC_UA_Client_IterationList() :
    mNewClientsPresent(false) {
}

COPC_UA_Client_IterationList::~COPC_UA_Client_IterationList() {
}

void COPC_UA_Client_IterationList::startIterationThread() {
  if(!isAlive()) {
    start();
  }
}

void COPC_UA_Client_IterationList::stopIterationThread() {
  setAlive(false);
  resumeIterationLoop();
  end();
}

void COPC_UA_Client_IterationList::addClient(CUA_ClientInformation& paClientInformation) {
  if(paClientInformation.isClientValid()) {
    CCriticalRegion iterationListRegion(getNewClientsMutex());
    addClientToList(paClientInformation, getNewClients());
    mNeedsIteration.inc();
    mNewClientsPresent = true;
  }
}

void COPC_UA_Client_IterationList::removeClient(CUA_ClientInformation& paClientInformation) {
  CCriticalRegion iterationListRegion(getIterationClientsMutex());
  CCriticalRegion newClientsRegion(getNewClientsMutex());
  getNewClients().erase(&paClientInformation); //client could still be in the newList
  getIterationClients().erase(&paClientInformation);
}

void COPC_UA_Client_IterationList::run() {
  while(isAlive()) {

    if(mNewClientsPresent) {
      updateClientList();
    }
    bool needsRetry = handleClients();
    if(isAlive()) {
      if(needsRetry) {
        mNeedsIteration.timedWait(scmNanosecondsToSleep);
      } else {
        mNeedsIteration.waitIndefinitely();
      }
    }
  }
}

void COPC_UA_Client_IterationList::resumeIterationLoop() {
  mNeedsIteration.inc();
}

void COPC_UA_Client_IterationList::addClientToList(CUA_ClientInformation &paClientInformation, CSinglyLinkedList<CUA_ClientInformation*> &paList) const {
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

void COPC_UA_Client_IterationList::updateClientList() {
  CCriticalRegion iterationListRegion(getIterationClientsMutex());
  CCriticalRegion newClientsRegion(getNewClientsMutex());
  for(CSinglyLinkedList<CUA_ClientInformation*>::Iterator itClientInformation = getNewClients().begin(); itClientInformation != getNewClients().end();
      ++itClientInformation) {
    addClientToList(**itClientInformation, getIterationClients());
  }
  getNewClients().clearAll();
  mNewClientsPresent = false;
}

// ***************** CLIENT HANDLER ************* //

COPC_UA_Remote_Handler::COPC_UA_Remote_Handler(CDeviceExecution& paDeviceExecution) :
    COPC_UA_HandlerAbstract(paDeviceExecution), mConnectionHandler(*this) {
}

COPC_UA_Remote_Handler::~COPC_UA_Remote_Handler() {
  mConnectionHandler.stopIterationThread();
  stopIterationThread();
  cleanResources();
}

void COPC_UA_Remote_Handler::enableHandler(void) {
  startIterationThread();
  mConnectionHandler.startIterationThread();
}

void COPC_UA_Remote_Handler::disableHandler(void) {
  mConnectionHandler.stopIterationThread();
  stopIterationThread();
}

UA_StatusCode COPC_UA_Remote_Handler::initializeAction(CActionInfo& paActionInfo) {
  enableHandler();
  UA_StatusCode retVal = UA_STATUSCODE_BADINTERNALERROR;
  switch(paActionInfo.getAction()){
    case CActionInfo::eRead:
    case CActionInfo::eWrite:
    case CActionInfo::eCallMethod:
    case CActionInfo::eSubscribe:
      retVal = getClientAndAddAction(paActionInfo);
      break;
    case CActionInfo::eCreateMethod:
    case CActionInfo::eCreateObject:
    case CActionInfo::eDeleteObject:
      DEVLOG_ERROR("[OPC UA REMOTE]: Cannot perform action %s remotely. Initialization failed\n", CActionInfo::mActionNames[paActionInfo.getAction()]);
      break;
    default:
      DEVLOG_ERROR("[OPC UA REMOTE]: Unknown action %d to be initialized\n", paActionInfo.getAction());
      break;
  }
  return retVal;
}

UA_StatusCode COPC_UA_Remote_Handler::executeAction(CActionInfo& paActionInfo) {

  CUA_ClientInformation *clientInfo = getClient(paActionInfo.getEndpoint());
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
        DEVLOG_ERROR("[OPC UA REMOTE]: Action %d to be executed is unknown or invalid\n", paActionInfo.getAction());
        break;
    }
  } else {
    DEVLOG_ERROR("[OPC UA REMOTE]: Cannot execute action from FB %s. It was not properly initialized\n",
      paActionInfo.getLayer().getCommFB()->getInstanceName());
  }

  if(UA_STATUSCODE_GOOD == retVal) {
    resumeIterationLoop();
  }

  return retVal;
}

UA_StatusCode COPC_UA_Remote_Handler::uninitializeAction(CActionInfo& paActionInfo) {
  UA_StatusCode retVal = UA_STATUSCODE_BADINTERNALERROR;
  switch(paActionInfo.getAction()){
    case CActionInfo::eRead:
    case CActionInfo::eWrite:
    case CActionInfo::eCallMethod:
    case CActionInfo::eSubscribe:
      removeActionFromClient(paActionInfo);
      retVal = UA_STATUSCODE_GOOD;
      break;
    default:
      DEVLOG_ERROR("[OPC UA REMOTE]: Action %d to be uninitialized is unknown or invalid\n", paActionInfo.getAction());
      break;
  }
  return retVal;
}

void COPC_UA_Remote_Handler::cleanResources() {
  CCriticalRegion criticalRegion(mAllClientListMutex);
  for(CSinglyLinkedList<CUA_ClientInformation*>::Iterator itClientInformation = mAllClients.begin(); itClientInformation != mAllClients.end();
      ++itClientInformation) {
    mConnectionHandler.removeClient(**itClientInformation);
    removeClient(**itClientInformation);
    delete *itClientInformation;
  }
  mAllClients.clearAll();
}

UA_StatusCode COPC_UA_Remote_Handler::getClientAndAddAction(CActionInfo& paActionInfo) {
  UA_StatusCode retVal = UA_STATUSCODE_BADINTERNALERROR;
  const CUA_ClientInformation *clientInfo = getClient(paActionInfo.getEndpoint());
  if(clientInfo) {
    addActionToClient(paActionInfo);
    retVal = UA_STATUSCODE_GOOD;
  }
  return retVal;
}

CUA_ClientInformation* COPC_UA_Remote_Handler::getClient(const CIEC_STRING &paEndpoint) {
  CCriticalRegion allClientsRegion(mAllClientListMutex);

  CUA_ClientInformation *client = 0;
  for(CSinglyLinkedList<CUA_ClientInformation *>::Iterator itClientInformation = mAllClients.begin(); itClientInformation != mAllClients.end();
      ++itClientInformation) {
    if((*itClientInformation)->getEndpoint() == paEndpoint) {
      client = (*itClientInformation);
      break;
    }
  }
  if(!client) {
    client = new CUA_ClientInformation(paEndpoint);
    if(client->configureClient()) {
      mAllClients.pushBack(client);
    } else {
      delete client;
      client = 0;
    }
  }

  return client;
}

void COPC_UA_Remote_Handler::addActionToClient(CActionInfo& paActionInfo) {
  CCriticalRegion allClientsRegion(mAllClientListMutex);
  for(CSinglyLinkedList<CUA_ClientInformation *>::Iterator itClientInformation = mAllClients.begin(); itClientInformation != mAllClients.end();
      ++itClientInformation) {
    CCriticalRegion clientRegion((*itClientInformation)->getMutex());
    if((*itClientInformation)->getEndpoint() == paActionInfo.getEndpoint()) {
      (*itClientInformation)->addAction(paActionInfo);
      addClientToConnectionHandler(**itClientInformation);
      break;
    }
  }
}

void COPC_UA_Remote_Handler::removeActionFromClient(CActionInfo& paActionInfo) {
  CCriticalRegion allClientsRegion(mAllClientListMutex);

  CUA_ClientInformation *clientToDelete = 0;
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

void COPC_UA_Remote_Handler::removeClientFromAllLists(CUA_ClientInformation& paClientInformation) {
  paClientInformation.setClientToInvalid();
  mConnectionHandler.removeClient(paClientInformation);
  removeClient(paClientInformation);
  mAllClients.erase(&paClientInformation);
  delete &paClientInformation;
}

void COPC_UA_Remote_Handler::addClientToConnectionHandler(CUA_ClientInformation& paClientInformation) {
  mConnectionHandler.addClient(paClientInformation);
}

bool COPC_UA_Remote_Handler::handleClients() {
  CCriticalRegion iterationCriticalRegion(getIterationClientsMutex()); //this is needed because removing a client from the list could cause trouble
  CSinglyLinkedList<CUA_ClientInformation *> failedClients;
  bool asyncIsNeeded = false;
  for(CSinglyLinkedList<CUA_ClientInformation*>::Iterator itClientInformation = getIterationClients().begin();
      itClientInformation != getIterationClients().end();
      ++itClientInformation) {
    CCriticalRegion criticalRegionClienMutex((*itClientInformation)->getMutex());
    if((*itClientInformation)->isAsyncNeeded()) {
      if(!(*itClientInformation)->executeAsyncCalls()) {
        failedClients.pushBack(*itClientInformation);
      } else {
        asyncIsNeeded = (*itClientInformation)->isAsyncNeeded();
      }
    }
    if(!isAlive()) {
      break;
    }
  }

  if(isAlive() && !failedClients.isEmpty()) {
    for(CSinglyLinkedList<CUA_ClientInformation *>::Iterator itClientInformation = failedClients.begin(); itClientInformation != failedClients.end();
        ++itClientInformation) {
      DEVLOG_ERROR("[OPC UA REMOTE]: There was a problem checking remote %s.\n", (*itClientInformation)->getEndpoint().getValue());

      //we cannot use COPC_UA_Client_IterationList::remove here because it locks mIterationClientsMutex
      CCriticalRegion newClientsRegion(getNewClientsMutex());
      CCriticalRegion criticalRegionClienMutex((*itClientInformation)->getMutex());
      getIterationClients().erase(*itClientInformation);
      getNewClients().erase(*itClientInformation); //client could still be in the newList
      (*itClientInformation)->uninitializeClient(); //reset all in client and pass it back to the connection handler
      (*itClientInformation)->configureClient();
      addClientToConnectionHandler(**itClientInformation);
    }
  }
  return asyncIsNeeded;
}

//************************** CONECTION HANDLER ************** //

COPC_UA_Remote_Handler::UA_ConnectionHandler::UA_ConnectionHandler(COPC_UA_Remote_Handler& paClientHandler) :
    mClientHandler(paClientHandler) {
}

COPC_UA_Remote_Handler::UA_ConnectionHandler::~UA_ConnectionHandler() {
}

bool COPC_UA_Remote_Handler::UA_ConnectionHandler::handleClients() {
  CCriticalRegion clientsClientsRegion(getIterationClientsMutex());
  CSinglyLinkedList<CUA_ClientInformation *> clientsToRemove;
  bool needsRetry = false;
  for(CSinglyLinkedList<CUA_ClientInformation*>::Iterator itClientInformation = getIterationClients().begin();
      itClientInformation != getIterationClients().end();
      ++itClientInformation) {
    CCriticalRegion clientRegion((*itClientInformation)->getMutex());
    if((*itClientInformation)->handleClientState()) {
      clientsToRemove.pushBack(*itClientInformation);
    } else {
      needsRetry = true;
    }
    if((*itClientInformation)->someActionWasInitialized()) {
      mClientHandler.addClient(**itClientInformation);
    }
    if(!isAlive()) {
      break;
    }
  }

  if(isAlive() && !clientsToRemove.isEmpty()) {
    for(CSinglyLinkedList<CUA_ClientInformation *>::Iterator itClientInformation = clientsToRemove.begin(); itClientInformation != clientsToRemove.end();
        ++itClientInformation) {
      getIterationClients().erase(*itClientInformation);
    }
  }
  return needsRetry;
}

