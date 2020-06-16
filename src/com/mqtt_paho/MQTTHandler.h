/*******************************************************************************
 * Copyright (c) 2013, 2014 ACIN
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 * Martin Melik Merkumians - initial API and implementation and/or initial documentation
 *******************************************************************************/


#ifndef MQTTHANDLER_H_
#define MQTTHANDLER_H_

#include <extevhan.h>
#include <fortelist.h>
#include <MQTTComLayer.h>
#include <forte_sync.h>
#include <forte_string.h>
#include <forte_thread.h>
#include <forte_sem.h>
#include <string>

extern "C" {
#include <MQTTAsync.h>
}

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
  int registerLayer(const char* paAddress, const char* paClientId, MQTTComLayer* paLayer);

  void unregisterLayer(MQTTComLayer* paLayer);

  MQTTAsync& getClient(void) {
    return smClient;
  }

  //void mqttMessageProcessed(void);

    virtual void enableHandler(void);
    /*!\brief Disable this event source
     */
    virtual void disableHandler(void);
    /*!\brief Sets the priority of the event source
     *
     * \param pa_nPriority new priority of the event source
     */
    virtual void setPriority(int pa_nPriority);
    /*!\brief Get the current priority of the event source
     *
     * \return current priority
     */
    virtual int getPriority(void) const;

protected:
    virtual void run();

private:

    int mqttSubscribe(const MQTTComLayer* paLayer);
    int mqttConnect();

    void resumeSelfSuspend();
    void selfSuspend();

    static void onMqttConnectionLost(void* paContext, char* paCause);

    static int onMqttMessageArrived(void *paContext, char *paTopicName, int paTopicLen, MQTTAsync_message *paMessage);

    static void onMqttConnectionSucceed(void *paContext, MQTTAsync_successData *paResponse);
    static void onMqttConnectionFailed(void *paContext, MQTTAsync_failureData *paResponse);

    static void onSubscribeSucceed(void* paContext, MQTTAsync_successData* paResponse);
    static void onSubscribeFailed(void* paContext, MQTTAsync_failureData* paResponse);

    static CIEC_STRING smClientId;
    static CIEC_STRING smAddress;
    static std::string scmUsername;
    static std::string scmPassword;

    static CSyncObject smMQTTMutex;

    static MQTTAsync smClient;
    static MQTTAsync_connectOptions smClientConnectionOptions;

    CSinglyLinkedList<MQTTComLayer*> mlayers;

    CSinglyLinkedList<MQTTComLayer*> mToResubscribe;

    static forte::arch::CSemaphore mStateSemaphore;

    static bool mIsSemaphoreEmpty;

    static MQTTStates smMQTTS_STATE;

};

#endif /* MQTTHANDLER_H_ */
