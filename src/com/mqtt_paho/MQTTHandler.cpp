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

MQTTHandler::MQTTHandler(CDeviceExecution& paDeviceExecution) : CExternalEventHandler(paDeviceExecution)  {
  if(!isAlive()){
    start();
  }
}

MQTTHandler::~MQTTHandler(){
  if (isAlive()) {
    setAlive(false);
    resumeSelfSuspend();
  }
  for (std::shared_ptr<CMQTTClient> client : mClients) {
    CCriticalRegion sectionState(smMQTTMutex);
    MQTTAsync asClient = client->getAsClient();
    if (asClient != nullptr) {
      MQTTAsync_disconnectOptions disconnectOptions = MQTTAsync_disconnectOptions_initializer;
      disconnectOptions.timeout = smTimeout;
      MQTTAsync_disconnect(asClient, &disconnectOptions);
      MQTTAsync_destroy(&asClient);
    }
  }
}

/*
 * START OF CALLBACKS
 */

/** Callback for handling message reception.
 *
 * For convenience and performance it would be great to have the paContext param set subscribing topic.
 * However Paho only allows one callback per client. Therefore we have to search for the layers attached to this topic.
 * For details see discussion in Bug 545111.
 *
 */
int MQTTHandler::onMqttMessageArrived(void* paContext, char* paTopicName, int, MQTTAsync_message* paMessage){
  //TODO: Check if handler allowed
  if(nullptr != paContext){
    CMQTTClient* client = static_cast<CMQTTClient*>(paContext);
    CCriticalRegion section(smMQTTMutex);
    void* pPayLoad = paMessage->payload;
    unsigned int payLoadSize = static_cast<unsigned int>(paMessage->payloadlen);

    for (MQTTComLayer* layer : client->getLayers()) {
      if (0 == strcmp(layer->getTopicName(), paTopicName)) {
        if (forte::com_infra::e_Nothing != layer->recvData(pPayLoad, payLoadSize)) {
          client->getHandler().startNewEventChain(layer->getCommFB());
        }
      }
    }
    //End critical section
  }
  MQTTAsync_freeMessage(&paMessage);
  MQTTAsync_free(paTopicName);

  return 1;
}

void MQTTHandler::onMqttConnectionLost(void* paContext, char* paCause){
  if(nullptr != paContext){
    CMQTTClient* client = static_cast<CMQTTClient*>(paContext);
    DEVLOG_ERROR("MQTT: Disconnected from broker at -%s. Cause: %s\n", client->getAddress().c_str(), paCause);
    CCriticalRegion section(smMQTTMutex);
    client->reconnect();
    client->getHandler().resumeSelfSuspend();
  }
}

void MQTTHandler::onMqttConnectionSucceed(void *paContext, MQTTAsync_successData *){
  if (paContext != nullptr) {
    CMQTTClient* client = static_cast<CMQTTClient*>(paContext);
    DEVLOG_INFO("MQTT: @%s: Successfully connected\n", client->getAddress().c_str());
    CCriticalRegion sectionState(smMQTTMutex);
    client->setMQTTState(SUBSCRIBING);
    client->getHandler().resumeSelfSuspend();
  }
}

void MQTTHandler::onMqttConnectionFailed(void *paContext, MQTTAsync_failureData *){
  if(paContext != nullptr){
    CMQTTClient* client = static_cast<CMQTTClient*>(paContext);
    DEVLOG_ERROR("MQTT: Connection failed to -%s-\n", client->getAddress().c_str());
    CCriticalRegion sectionState(smMQTTMutex);
    client->setMQTTState(NOT_CONNECTED);
    client->getHandler().resumeSelfSuspend();
  }
}

void MQTTHandler::onSubscribeSucceed(void* paContext, MQTTAsync_successData* ){
  if(nullptr != paContext){
    MQTTComLayer* layer = static_cast<MQTTComLayer*>(paContext);
    std::shared_ptr<CMQTTClient> client = layer->getClient();
    DEVLOG_INFO("MQTT: @%s: Subscription succeed.\n Topic: -%s-\n", client->getAddress().c_str(), layer->getTopicName());
    CCriticalRegion sectionState(smMQTTMutex);
    client->removeToResubscribe(layer);
    if (client->getMQTTState() != ALL_SUBSCRIBED) {
      client->getHandler().resumeSelfSuspend();
    }
  }
}

void MQTTHandler::onSubscribeFailed(void* paContext, MQTTAsync_failureData*){
  if(nullptr != paContext){
    MQTTComLayer* layer = static_cast<MQTTComLayer*>(paContext);
    std::shared_ptr<CMQTTClient> client = layer->getClient();
    CCriticalRegion sectionState(smMQTTMutex);
    DEVLOG_ERROR("MQTT: @%s: Subscription failed.\n Topic: -%s-\n", client->getAddress().c_str(), layer->getTopicName());
    client->getHandler().resumeSelfSuspend();
  }
}

/*
 * END OF CALLBACKS AND START OF HELPER FUNCTIONS
 */

int MQTTHandler::mqttConnect(CMQTTClient* paClient){
  DEVLOG_INFO("MQTT: Requesting connection to broker @%s\n", paClient->getAddress().c_str());
  int rc = MQTTAsync_connect(paClient->getAsClient(), &paClient->getClientConnectionOptions());
  if(MQTTASYNC_SUCCESS != rc){
    DEVLOG_ERROR("MQTT: Request to mqtt library failed\n");
  }else{
    DEVLOG_INFO("MQTT: Request successful - Connecting...\n");
  }
  return rc;
}

int MQTTHandler::mqttSubscribe(const MQTTComLayer* paLayer, CMQTTClient* paClient){
  DEVLOG_INFO("MQTT: Subscribing to topic -%s-\n", paLayer->getTopicName());
  MQTTAsync_responseOptions opts = MQTTAsync_responseOptions_initializer;
  opts.onSuccess = onSubscribeSucceed;
  opts.onFailure = onSubscribeFailed;
  opts.context = (void*)paLayer;
  int rc = MQTTAsync_subscribe(paClient->getAsClient(), paLayer->getTopicName(), QOS, &opts);
  if(MQTTASYNC_SUCCESS != rc){ //call failed
    CCriticalRegion sectionState(smMQTTMutex);
    DEVLOG_INFO("MQTT: Subscribe Request failed with val = %d\n", rc);
    resumeSelfSuspend();
  }else{
    DEVLOG_INFO("MQTT: Subscribe Request successful - Subscribing...\n");
  }
  return rc;
}

std::shared_ptr<CMQTTClient> MQTTHandler::getClient(std::string& paAddress, std::string& paClientId) {
  for (std::shared_ptr<CMQTTClient> client : mClients) {
    if (paAddress == client->getAddress()) {
      if (paClientId.compare(client->getClientId()) == 0) {
        return client;
      }
      else {
        // Wrong ClientId
        DEVLOG_ERROR("Wrong ClientID for Address %s, ClientID: %s\n", paAddress, paClientId);
        return nullptr;
      }
    }
  }
  std::shared_ptr<CMQTTClient> newClient = std::make_shared<CMQTTClient>(paAddress, paClientId, *this);
  {
    CCriticalRegion sectionState(smMQTTMutex);
    newClient->initClient();
    mClients.push_back(std::move(newClient));
    return mClients.back();
  }
}


int MQTTHandler::registerLayer(std::string& paAddress, std::string& paClientId, MQTTComLayer* paLayer) {
  std::shared_ptr<CMQTTClient> client = getClient(paAddress, paClientId);
  if (client != nullptr) {
    if (client->getState() == CMQTTClient::eUninitialized) {
      return eConnectionFailed;
    }
    CCriticalRegion section(smMQTTMutex);
    client->addLayer(paLayer);
    if (e_Subscriber == paLayer->getCommFB()->getComServiceType()) {
      client->addToResubscribe(paLayer);
      if (ALL_SUBSCRIBED == client->getMQTTState()) {
        client->setMQTTState(SUBSCRIBING);
        resumeSelfSuspend();
      }
    }
    paLayer->setClient(client);
    return eRegisterLayerSucceeded;
  }
  return eConnectionFailed;
}

void MQTTHandler::unregisterLayer(MQTTComLayer* paLayer){
  std::shared_ptr<CMQTTClient> client = paLayer->getClient();
  CCriticalRegion section(smMQTTMutex);
  if (client != nullptr) {
    client->removeLayer(paLayer);
    paLayer->setClient(nullptr);
  }
}

void MQTTHandler::enableHandler(){
  if (!isAlive()){
    start();
  }
}

void MQTTHandler::disableHandler(){
  if (isAlive()){
    setAlive(false);
    resumeSelfSuspend();
    end();
  }
}

void MQTTHandler::setPriority(int ){
  //NOTE: Priorities are currently not supported by MQTTHandler, therefore this function remains empty
}

int MQTTHandler::getPriority() const{
  return 0;
}

void MQTTHandler::run(){
  while(isAlive()){
    bool needSleep = false;
    selfSuspend();
    if(!isAlive()){
      break;
    }
    for (std::shared_ptr<CMQTTClient> client : mClients) {        
      if (client->getMQTTState() != ALL_SUBSCRIBED) {
        needSleep = needSleep || client->runClient();
      }
    }
    if(needSleep){
      //sleep in case a subscribe fails while connected
      needSleep = false;
      CThread::sleepThread(smSleepTime);
    }
  }
}

void MQTTHandler::resumeSelfSuspend(){
  if(mIsSemaphoreEmpty){ //avoid incrementing many times
    mStateSemaphore.inc();
    mIsSemaphoreEmpty = false;
  }
}

void MQTTHandler::selfSuspend(){
  mStateSemaphore.waitIndefinitely();
  {
    CCriticalRegion section(smMQTTMutex);
    mIsSemaphoreEmpty = true;
  }
}
