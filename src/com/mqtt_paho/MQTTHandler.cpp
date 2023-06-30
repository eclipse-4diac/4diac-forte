/*******************************************************************************
 * Copyright (c) 2013, 2023 ACIN, Primetals Technologies Austria GmbH,
 *                          Johannes Kepler University Linz
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 * Martin Melik Merkumians - initial API and implementation and/or initial documentation
 *                         - change CIEC_STRING to std::string
 * Markus Meingast         - refactoring and adaption to new Client class,
 *                           enabling connection to multiple servers
 *******************************************************************************/

#include "MQTTHandler.h"
#include "../../core/cominfra/commfb.h"
#include <criticalregion.h>

#include "MQTTClient.h"

DEFINE_HANDLER(MQTTHandler);

CSyncObject MQTTHandler::smMQTTMutex;
forte::arch::CSemaphore MQTTHandler::mStateSemaphore;
bool MQTTHandler::mIsSemaphoreEmpty = true;

MQTTHandler::MQTTHandler(CDeviceExecution& paDeviceExecution) : CExternalEventHandler(paDeviceExecution) {
  if (!isAlive()) {
    start();
  }
}

MQTTHandler::~MQTTHandler() {
  if (isAlive()) {
    setAlive(false);
    resumeSelfSuspend();
  }
}


void MQTTHandler::startNewEventChain(MQTTComLayer* layer) {
  CExternalEventHandler::startNewEventChain(layer->getCommFB());
}

std::shared_ptr<CMQTTClient> MQTTHandler::getClient(const std::string& paAddress, const std::string& paClientId) {
  for (std::shared_ptr<CMQTTClient> client : mClients) {
    if (paAddress == client->getAddress()) {
      if (paClientId == client->getClientId()) {
        return client;
      }
      else {
        // Wrong ClientId
        DEVLOG_ERROR("Wrong ClientID for Address %s, ClientID: %s\n", paAddress, paClientId);
        return nullptr;
      }
    }
  }
  std::shared_ptr<CMQTTClient> newClient = CMQTTClient::getNewClient(paAddress, paClientId, *this);
  if (newClient != nullptr) {
    mClients.push_back(std::move(newClient));
    return mClients.back();
  }
  return nullptr;
}


int MQTTHandler::registerLayer(const std::string& paAddress, const std::string& paClientId, MQTTComLayer* paLayer) {
  std::shared_ptr<CMQTTClient> client = getClient(paAddress, paClientId);
  if (client == nullptr) {
    return eConnectionFailed;
  }
  client->addLayer(paLayer);
  paLayer->setClient(client);
  return eRegisterLayerSucceeded;
}

void MQTTHandler::unregisterLayer(MQTTComLayer* paLayer) {
  std::shared_ptr<CMQTTClient> client = paLayer->getClient();
  if (client != nullptr) {
    client->removeLayer(paLayer);
    paLayer->setClient(nullptr);
  }
}

void MQTTHandler::enableHandler() {
  if (!isAlive()) {
    start();
  }
}

void MQTTHandler::disableHandler() {
  if (isAlive()) {
    setAlive(false);
    resumeSelfSuspend();
    end();
  }
}

void MQTTHandler::setPriority(int) {
  //NOTE: Priorities are currently not supported by MQTTHandler, therefore this function remains empty
}

int MQTTHandler::getPriority() const {
  return 0;
}

void MQTTHandler::run() {
  while (isAlive()) {
    bool needSleep = false;
    selfSuspend();
    if (!isAlive()) {
      break;
    }
    for (std::shared_ptr<CMQTTClient> client : mClients) {
      if (client->getMQTTState() != CMQTTClient::ALL_SUBSCRIBED) {
        needSleep = needSleep || client->runClient();
      }
    }
    if (needSleep) {
      //sleep in case a subscribe fails while connected
      needSleep = false;
      CThread::sleepThread(smSleepTime);
    }
  }
}

void MQTTHandler::resumeSelfSuspend() {
  if (mIsSemaphoreEmpty) { //avoid incrementing many times
    {
      CCriticalRegion section(smMQTTMutex);
      mStateSemaphore.inc();
      mIsSemaphoreEmpty = false;
    }
  }
}

void MQTTHandler::selfSuspend() {
  mStateSemaphore.waitIndefinitely();
  {
    CCriticalRegion section(smMQTTMutex);
    mIsSemaphoreEmpty = true;
  }
}
