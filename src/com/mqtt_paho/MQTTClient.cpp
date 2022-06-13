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

CMQTTClient::CMQTTClient(std::string& paAddress, std::string& paClientId, MQTTHandler& paHandler) :
  mAddress(paAddress),
  mClientId(paClientId),
  mAsClient(nullptr),
  mClientConnectionOptions(MQTTAsync_connectOptions_initializer),
  mMQTT_STATE(NOT_CONNECTED),
  mState(eUninitialized),
  mHandler(paHandler) {
}

CMQTTClient::~CMQTTClient() {
}


int CMQTTClient::initClient() {
  if (MQTTASYNC_SUCCESS != MQTTAsync_create(&mAsClient, mAddress.c_str(), mClientId.c_str(), MQTTCLIENT_PERSISTENCE_NONE, NULL)) {
    return MQTTHandler::eConnectionFailed;
  }
  mClientConnectionOptions.keepAliveInterval = smKeepAliveInterval;
  mClientConnectionOptions.cleansession = smCleanSession;
  mClientConnectionOptions.onSuccess = MQTTHandler::onMqttConnectionSucceed;
  mClientConnectionOptions.onFailure = MQTTHandler::onMqttConnectionFailed;
  mClientConnectionOptions.context = this;
  if (MQTTASYNC_SUCCESS != MQTTAsync_setCallbacks(mAsClient, this, MQTTHandler::onMqttConnectionLost, MQTTHandler::onMqttMessageArrived, NULL)) {
    return MQTTHandler::eConnectionFailed;
  }
  if (MQTTASYNC_SUCCESS != mHandler.mqttConnect(this)) {
    return MQTTHandler::eConnectionFailed;
  }
  mMQTT_STATE = CONNECTION_ASKED;
  mState = eInitialized;
  return MQTTHandler::eRegisterLayerSucceeded;
}


bool CMQTTClient::runClient() {
  bool needSleep = false;
  CCriticalRegion sectionState(mMQTTMutex);
  switch (mMQTT_STATE) {
  case NOT_CONNECTED:
    if (MQTTASYNC_SUCCESS == mHandler.mqttConnect(this)) {
      mMQTT_STATE = CONNECTION_ASKED;
    }
    break;
  case SUBSCRIBING: {
    if (!mToResubscribe.empty()) {
      //only try subscribe one at a time. There were some problems in some cases when subscribing one after the other, because the MQTTAsync_subscribe hangs.
      const MQTTComLayer* layer = *mToResubscribe.begin();
      mMQTTMutex.unlock();
      if (MQTTASYNC_SUCCESS != mHandler.mqttSubscribe(layer, this)) {
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
      addToResubscribe(layer);
    }
  }
}

void CMQTTClient::removeLayerHelper(MQTTComLayer* paLayer, std::vector<MQTTComLayer*>& paLayerList) {
  if (!paLayerList.empty()) {
    paLayerList.erase(std::partition(paLayerList.begin(), paLayerList.end(), [paLayer](MQTTComLayer* elem)->bool {return (elem != paLayer); }), paLayerList.end());
  }
}
