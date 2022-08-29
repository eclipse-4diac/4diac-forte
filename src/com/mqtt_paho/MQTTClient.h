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

#ifndef CMQTTCLIENT_H
#define CMQTTCLIENT_H

#include "MQTTHandler.h"
#include <string>
#include <vector>

extern "C" {
#include <MQTTAsync.h>
}


class CMQTTClient {
public:
  enum ClientState {
    eInitialized,
    eUninitialized
  };

  CMQTTClient(std::string& paAddress, std::string& paClientId, MQTTHandler& paHandler);

  ~CMQTTClient();

  int initClient();

  bool runClient();

  void reconnect();

  const std::string& getClientId() const {
    return mClientId;
  }

  const std::string& getAddress() const {
    return mAddress;
  }

  const ClientState getState() const {
    return mState;
  }

  void addLayer(MQTTComLayer* paLayer) {
    mLayers.push_back(paLayer);
  }

  //const std::vector<MQTTComLayer*>& getLayers() const {
  //  return mLayers;
  //}

  void removeLayer(MQTTComLayer* paLayer) {
    removeLayerHelper(paLayer, mLayers);
  }

  void addToResubscribe(MQTTComLayer* paToResubscribe) {
    mToResubscribe.push_back(paToResubscribe);
  }

  //const std::vector<MQTTComLayer*>& getToResubscribe() const {
  //  return mToResubscribe;
  //}

  void removeToResubscribe(MQTTComLayer* paLayer) {
    removeLayerHelper(paLayer, mToResubscribe);
    if (mToResubscribe.empty()) {
      mMQTT_STATE = ALL_SUBSCRIBED;
    }
  }

  //MQTTHandler& getHandler() const {
  //  return mHandler;
  //}

  void setMQTTState(MQTTStates paState) {
    mMQTT_STATE = paState;
  }

  const MQTTStates getMQTTState() const {
    return mMQTT_STATE;
  }

  int sendData(void* paData, unsigned int paSize, char* paTopicName);

  int mqttSubscribe(const MQTTComLayer* paLayer);
  int mqttConnect();

  static void onMqttConnectionLost(void* paContext, char* paCause);
  static int onMqttMessageArrived(void* paContext, char* paTopicName, int paTopicLen, MQTTAsync_message* paMessage);

  static void onMqttConnectionSucceed(void* paContext, MQTTAsync_successData* paResponse);
  static void onMqttConnectionFailed(void* paContext, MQTTAsync_failureData* paResponse);

private:

  static void onSubscribeSucceed(void* paContext, MQTTAsync_successData* paResponse);
  static void onSubscribeFailed(void* paContext, MQTTAsync_failureData* paResponse);

  void removeLayerHelper(MQTTComLayer* paLayer, std::vector<MQTTComLayer*>& paLayerList);

  void clearToResubscribe() {
    mToResubscribe.clear();
  }

  static const int smKeepAliveInterval = 20;
  static const int smCleanSession = 1;

  static const int smTimeout = 10000;

  std::string mAddress;
  std::string mClientId;

  MQTTAsync mAsClient;
  MQTTAsync_connectOptions mClientConnectionOptions;
  MQTTStates mMQTT_STATE;
  ClientState mState;
  CSyncObject mMQTTMutex;

  MQTTHandler& mHandler;
  std::vector<MQTTComLayer*> mLayers;
  std::vector<MQTTComLayer*> mToResubscribe;
};

#endif /*CMQTTCLIENT_H*/
