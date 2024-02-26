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
  enum MQTTStates {
    NOT_CONNECTED,
    CONNECTION_ASKED,
    SUBSCRIBING,
    ALL_SUBSCRIBED,
  };

  CMQTTClient(const std::string& paAddress, const std::string& paClientId, MQTTHandler& paHandler);

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

  MQTTStates getMQTTState() const {
    return mMQTT_STATE;
  }

  void addLayer(MQTTComLayer* paLayer);

  void removeLayer(MQTTComLayer* paLayer);

  int sendData(void *paData, unsigned int paSize, const std::string& paTopicName);

  int mqttSubscribe(const MQTTComLayer* paLayer);
  int mqttConnect();

  static void onMqttConnectionLost(void* paContext, char* paCause);
  static int onMqttMessageArrived(void* paContext, char* paTopicName, int paTopicLen, MQTTAsync_message* paMessage);

  static void onMqttConnectionSucceed(void* paContext, MQTTAsync_successData* paResponse);
  static void onMqttConnectionFailed(void* paContext, MQTTAsync_failureData* paResponse);

  static std::shared_ptr<CMQTTClient> getNewClient(const std::string&  paAddress, const std::string&  paClientId, MQTTHandler& paHandler);

private:

  static void onSubscribeSucceed(void* paContext, MQTTAsync_successData* paResponse);
  static void onSubscribeFailed(void* paContext, MQTTAsync_failureData* paResponse);

  void removeLayerHelper(MQTTComLayer* paLayer, std::vector<MQTTComLayer*>& paLayerList);

  void removeToResubscribe(MQTTComLayer* paLayer);

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
  CSyncObject mMQTTMutex;

  MQTTHandler& mHandler;
  std::vector<MQTTComLayer*> mLayers;
  std::vector<MQTTComLayer*> mToResubscribe;
};

#endif /*CMQTTCLIENT_H*/
