/*******************************************************************************
 * Copyright (c) 2013, 2022 ACIN, Johannes Kepler University Linz
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 * Martin Melik Merkumians - initial API and implementation and/or initial documentation
 * Markus Meingast - refactoring and adaption to new Client class, enabling connection to multiple servers
 *******************************************************************************/

#include "MQTTClient.h"
#include "basecommfb.h"
#include <algorithm>

std::string gMqttClientConfigFile;

CMQTTClient::CMQTTClient(const std::string& paAddress, const std::string& paClientId, MQTTHandler& paHandler) :
  mAddress(paAddress),
  mClientId(paClientId),
  mAsClient(nullptr),
  mClientConnectionOptions(MQTTAsync_connectOptions_initializer),
  mMQTT_STATE(NOT_CONNECTED),
  mHandler(paHandler) {
}

CMQTTClient::~CMQTTClient() {
  CCriticalRegion sectionState(mMQTTMutex);
  if (mAsClient != nullptr) {
    MQTTAsync_disconnectOptions disconnectOptions = MQTTAsync_disconnectOptions_initializer;
    disconnectOptions.timeout = smTimeout;
    MQTTAsync_disconnect(mAsClient, &disconnectOptions);
    MQTTAsync_destroy(&mAsClient);
  }
}

std::shared_ptr<CMQTTClient> CMQTTClient::getNewClient(const std::string& paAddress, const std::string& paClientId, MQTTHandler& paHandler) {
  std::shared_ptr<CMQTTClient> newClient = std::make_shared<CMQTTClient>(paAddress, paClientId, paHandler);
  {
    CCriticalRegion sectionState(newClient->mMQTTMutex);
    if (MQTTHandler::eRegisterLayerSucceeded != newClient->initClient()) {
      return nullptr;
    }
    return newClient;
  }
}

int CMQTTClient::sendData(void* paData, unsigned int paSize, const std::string& paTopicName) {
  MQTTAsync_message message = MQTTAsync_message_initializer;
  message.payload = paData;
  message.payloadlen = paSize;
  message.qos = MQTT_QOS;
  message.retained = 0;
  return MQTTAsync_sendMessage(mAsClient, paTopicName.c_str(), &message, NULL);
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
int CMQTTClient::onMqttMessageArrived(void* paContext, char* paTopicName, int, MQTTAsync_message* paMessage) {
  //TODO: Check if handler allowed
  if (nullptr != paContext) {
    CMQTTClient* client = static_cast<CMQTTClient*>(paContext);
    CCriticalRegion section(client->mMQTTMutex);
    void* pPayLoad = paMessage->payload;
    unsigned int payLoadSize = static_cast<unsigned int>(paMessage->payloadlen);

    for (MQTTComLayer* layer : client->mLayers) {
      if (layer->getTopicName() == paTopicName) {
        if (forte::com_infra::e_Nothing != layer->recvData(pPayLoad, payLoadSize)) {
          client->mHandler.startNewEventChain(layer);
        }
      }
    }
    //End critical section
  }
  MQTTAsync_freeMessage(&paMessage);
  MQTTAsync_free(paTopicName);

  return 1;
}

void CMQTTClient::onMqttConnectionLost(void* paContext, char* paCause) {
  if (nullptr != paContext) {
    CMQTTClient* client = static_cast<CMQTTClient*>(paContext);
    DEVLOG_ERROR("MQTT: Disconnected from broker at -%s. Cause: %s\n", client->mAddress.c_str(), paCause);
    CCriticalRegion section(client->mMQTTMutex);
    client->reconnect();
    client->mHandler.resumeSelfSuspend();
  }
}

void CMQTTClient::onMqttConnectionSucceed(void* paContext, MQTTAsync_successData*) {
  if (paContext != nullptr) {
    CMQTTClient* client = static_cast<CMQTTClient*>(paContext);
    DEVLOG_INFO("MQTT: @%s: Successfully connected\n", client->mAddress.c_str());
    CCriticalRegion sectionState(client->mMQTTMutex);
    client->mMQTT_STATE = SUBSCRIBING;
    client->mHandler.resumeSelfSuspend();
  }
}

void CMQTTClient::onMqttConnectionFailed(void* paContext, MQTTAsync_failureData*) {
  if (paContext != nullptr) {
    CMQTTClient* client = static_cast<CMQTTClient*>(paContext);
    DEVLOG_ERROR("MQTT: Connection failed to -%s-\n", client->mAddress.c_str());
    CCriticalRegion sectionState(client->mMQTTMutex);
    client->mMQTT_STATE = NOT_CONNECTED;
    client->mHandler.resumeSelfSuspend();
  }
}

void CMQTTClient::onSubscribeSucceed(void* paContext, MQTTAsync_successData*) {
  if (nullptr != paContext) {
    MQTTComLayer* layer = static_cast<MQTTComLayer*>(paContext);
    std::shared_ptr<CMQTTClient> client = layer->getClient();
    DEVLOG_INFO("MQTT: @%s: Subscription succeed.\n Topic: -%s-\n", client->mAddress.c_str(), layer->getTopicName());
    CCriticalRegion sectionState(client->mMQTTMutex);
    client->removeToResubscribe(layer);
    if (client->mMQTT_STATE != ALL_SUBSCRIBED) {
      client->mHandler.resumeSelfSuspend();
    }
  }
}

void CMQTTClient::onSubscribeFailed(void* paContext, MQTTAsync_failureData*) {
  if (nullptr != paContext) {
    MQTTComLayer* layer = static_cast<MQTTComLayer*>(paContext);
    std::shared_ptr<CMQTTClient> client = layer->getClient();
    CCriticalRegion sectionState(client->mMQTTMutex);
    DEVLOG_ERROR("MQTT: @%s: Subscription failed.\n Topic: -%s-\n", client->mAddress.c_str(), layer->getTopicName());
    client->mHandler.resumeSelfSuspend();
  }
}

/*
 * END OF CALLBACKS AND START OF HELPER FUNCTIONS
 */

int CMQTTClient::mqttConnect() {
  DEVLOG_INFO("MQTT: Requesting connection to broker @%s\n", mAddress.c_str());
  int rc = MQTTAsync_connect(mAsClient, &mClientConnectionOptions);
  if (MQTTASYNC_SUCCESS != rc) {
    DEVLOG_ERROR("MQTT: Request to mqtt library failed\n");
  }
  else {
    DEVLOG_INFO("MQTT: Request successful - Connecting...\n");
  }
  return rc;
}

int CMQTTClient::mqttSubscribe(const MQTTComLayer* paLayer) {
  DEVLOG_INFO("MQTT: Subscribing to topic -%s-\n", paLayer->getTopicName().c_str());
  MQTTAsync_responseOptions opts = MQTTAsync_responseOptions_initializer;
  opts.onSuccess = onSubscribeSucceed;
  opts.onFailure = onSubscribeFailed;
  opts.context = (void*)paLayer;
  int rc = MQTTAsync_subscribe(mAsClient, paLayer->getTopicName().c_str(), MQTT_QOS, &opts);
  if (MQTTASYNC_SUCCESS != rc) { //call failed
    CCriticalRegion sectionState(mMQTTMutex);
    DEVLOG_INFO("MQTT: Subscribe Request failed with val = %d\n", rc);
    mHandler.resumeSelfSuspend();
  }
  else {
    DEVLOG_INFO("MQTT: Subscribe Request successful - Subscribing...\n");
  }
  return rc;
}

int CMQTTClient::initClient() {
  if (MQTTASYNC_SUCCESS != MQTTAsync_create(&mAsClient, mAddress.c_str(), mClientId.c_str(), MQTTCLIENT_PERSISTENCE_NONE, NULL)) {
    return MQTTHandler::eConnectionFailed;
  }
  mClientConnectionOptions.keepAliveInterval = smKeepAliveInterval;
  mClientConnectionOptions.cleansession = smCleanSession;
  mClientConnectionOptions.onSuccess = onMqttConnectionSucceed;
  mClientConnectionOptions.onFailure = onMqttConnectionFailed;
  mClientConnectionOptions.context = this;
  if (MQTTASYNC_SUCCESS != MQTTAsync_setCallbacks(mAsClient, this, onMqttConnectionLost, onMqttMessageArrived, NULL)) {
    return MQTTHandler::eConnectionFailed;
  }
  if (MQTTASYNC_SUCCESS != mqttConnect()) {
    return MQTTHandler::eConnectionFailed;
  }
  mMQTT_STATE = CONNECTION_ASKED;
  return MQTTHandler::eRegisterLayerSucceeded;
}


bool CMQTTClient::runClient() {
  bool needSleep = false;
  CCriticalRegion sectionState(mMQTTMutex);
  switch (mMQTT_STATE) {
  case NOT_CONNECTED:
    if (MQTTASYNC_SUCCESS == mqttConnect()) {
      mMQTT_STATE = CONNECTION_ASKED;
    }
    break;
  case SUBSCRIBING: {
    if (!mToResubscribe.empty()) {
      //only try subscribe one at a time. There were some problems in some cases when subscribing one after the other, because the MQTTAsync_subscribe hangs.
      const MQTTComLayer* layer = *mToResubscribe.begin();
      mMQTTMutex.unlock();
      if (MQTTASYNC_SUCCESS != mqttSubscribe(layer)) {
        needSleep = true;
      }
      mMQTTMutex.lock();
    }
    else {
      mMQTT_STATE = ALL_SUBSCRIBED;
    }
    break;
  }
  default:
    break;
  }
  return needSleep;
}

void CMQTTClient::reconnect() {
  mMQTT_STATE = NOT_CONNECTED;
  clearToResubscribe();
  for (MQTTComLayer* layer : mLayers) {
    if (layer->getCommFB()->getComServiceType() == e_Subscriber) {
      mToResubscribe.push_back(layer);
    }
  }
}

void CMQTTClient::addLayer(MQTTComLayer* paLayer) {
  CCriticalRegion section(mMQTTMutex);
  mLayers.push_back(paLayer);
  if (e_Subscriber == paLayer->getCommFB()->getComServiceType()) {
    mToResubscribe.push_back(paLayer);
    if (ALL_SUBSCRIBED == mMQTT_STATE) {
      mMQTT_STATE = SUBSCRIBING;
      mHandler.resumeSelfSuspend();
    }
  }
}

void CMQTTClient::removeLayer(MQTTComLayer* paLayer) {
  CCriticalRegion section(mMQTTMutex);
  removeLayerHelper(paLayer, mLayers);
}

void CMQTTClient::removeToResubscribe(MQTTComLayer* paLayer) {
  removeLayerHelper(paLayer, mToResubscribe);
  if (mToResubscribe.empty()) {
    mMQTT_STATE = ALL_SUBSCRIBED;
  }
}

void CMQTTClient::removeLayerHelper(MQTTComLayer* paLayer, std::vector<MQTTComLayer*>& paLayerList) {
  if (!paLayerList.empty()) {
    paLayerList.erase(std::partition(paLayerList.begin(), paLayerList.end(), [paLayer](MQTTComLayer* elem)->bool {return (elem != paLayer); }), paLayerList.end());
  }
}
