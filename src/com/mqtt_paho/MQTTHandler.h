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

#ifndef MQTTHANDLER_H_
#define MQTTHANDLER_H_

#include <extevhan.h>
#include <fortelist.h>
#include <MQTTComLayer.h>
#include <forte_sync.h>
#include <forte_thread.h>
#include <forte_sem.h>
#include <string>
#include <vector>
#include <memory>

extern "C" {
#include <MQTTAsync.h>
}


class CMQTTClient;

enum MQTTStates {
  NOT_CONNECTED,
  CONNECTION_ASKED,
  SUBSCRIBING,
  ALL_SUBSCRIBED,
};

class MQTTHandler : public CExternalEventHandler, public CThread {
    DECLARE_HANDLER(MQTTHandler)
public:
  enum RegisterLayerReturnCodes {
    eRegisterLayerSucceeded,
    eWrongClientID,
    eConnectionFailed
  };

  int registerLayer(std::string& paAddress, std::string& paClientId, MQTTComLayer* paLayer);

  void unregisterLayer(MQTTComLayer* paLayer);

  void enableHandler() override;
  /*!\brief Disable this event source
   */
  void disableHandler() override;
  /*!\brief Sets the priority of the event source
   *
   * \param pa_nPriority new priority of the event source
   */
  void setPriority(int pa_nPriority) override;
  /*!\brief Get the current priority of the event source
   *
   * \return current priority
   */
  int getPriority() const override;
  
  int mqttSubscribe(const MQTTComLayer* paLayer, CMQTTClient* paClient);
  int mqttConnect(CMQTTClient* paClient);
  
  static void onMqttConnectionLost(void* paContext, char* paCause);
  static int onMqttMessageArrived(void* paContext, char* paTopicName, int paTopicLen, MQTTAsync_message* paMessage);
  
  static void onMqttConnectionSucceed(void* paContext, MQTTAsync_successData* paResponse);
  static void onMqttConnectionFailed(void* paContext, MQTTAsync_failureData* paResponse);

protected:
    void run() override;

private:
  std::shared_ptr<CMQTTClient> getClient(std::string& paAddress, std::string& paClientId);
  void resumeSelfSuspend();
  void selfSuspend();
  
  static void onSubscribeSucceed(void* paContext, MQTTAsync_successData* paResponse);
  static void onSubscribeFailed(void* paContext, MQTTAsync_failureData* paResponse);
  
  static CSyncObject smMQTTMutex;
  
  static MQTTAsync_connectOptions smClientConnectionOptions;
  static const int smTimeout = 10000;
  static const int smSleepTime = 5000;
  
  static forte::arch::CSemaphore mStateSemaphore;
  static bool mIsSemaphoreEmpty;


  std::vector<std::shared_ptr<CMQTTClient>> mClients;
};

#endif /* MQTTHANDLER_H_ */
