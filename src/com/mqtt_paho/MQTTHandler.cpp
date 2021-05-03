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


#include "MQTTHandler.h"
#include "../../core/cominfra/commfb.h"
#include <criticalregion.h>
#include <string>

#include "MQTTClientConfigParser.h"

std::string gMqttClientConfigFile;

DEFINE_HANDLER(MQTTHandler);

MQTTAsync_connectOptions MQTTHandler::smClientConnectionOptions =
    MQTTAsync_connectOptions_initializer;

CSyncObject MQTTHandler::smMQTTMutex;

MQTTAsync MQTTHandler::smClient = 0;
CIEC_STRING MQTTHandler::smClientId;
CIEC_STRING MQTTHandler::smAddress;
std::string MQTTHandler::scmUsername;
std::string MQTTHandler::scmPassword;


MQTTStates MQTTHandler::smMQTTS_STATE = NOT_CONNECTED;
forte::arch::CSemaphore MQTTHandler::mStateSemaphore;
bool MQTTHandler::mIsSemaphoreEmpty = true;

MQTTHandler::MQTTHandler(CDeviceExecution& paDeviceExecution) : CExternalEventHandler(paDeviceExecution)  {
  if(!isAlive()){
    start();
  }
}

MQTTHandler::~MQTTHandler(){
  CCriticalRegion sectionState(smMQTTMutex);
  if(isAlive()){
    setAlive(false);
    resumeSelfSuspend();
  }
  if(0 != smClient){
    MQTTAsync_disconnectOptions disconnectOptions = MQTTAsync_disconnectOptions_initializer;
    disconnectOptions.timeout = 10000;
    MQTTAsync_disconnect(smClient, &disconnectOptions);
    MQTTAsync_destroy(&smClient);
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
  if(0 != paContext){
    CCriticalRegion section(smMQTTMutex);

    MQTTHandler *handler = static_cast<MQTTHandler *>(paContext);
    void* pPayLoad = paMessage->payload;
    unsigned int payLoadSize = static_cast<unsigned int>(paMessage->payloadlen);

    for(CSinglyLinkedList<MQTTComLayer*>::Iterator it = handler->mlayers.begin(); it != handler->mlayers.end(); ++it){
      if(0 == strcmp((*it)->getTopicName(), paTopicName)){
        if(forte::com_infra::e_Nothing != (*it)->recvData(pPayLoad, payLoadSize)){
          handler->startNewEventChain((*it)->getCommFB());
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
  DEVLOG_ERROR("MQTT: Disconnected from broker. Cause: %s\n", paCause);
  if(0 != paContext){
    CCriticalRegion section(smMQTTMutex);

    MQTTHandler *handler = static_cast<MQTTHandler *>(paContext);
    smMQTTS_STATE = NOT_CONNECTED;

    handler->mToResubscribe.clearAll();
    for(CSinglyLinkedList<MQTTComLayer*>::Iterator it = handler->mlayers.begin(); it != handler->mlayers.end(); ++it){
      if(e_Subscriber == (*it)->getCommFB()->getComServiceType()){
        handler->mToResubscribe.pushBack((*it));
      }
    }
    handler->resumeSelfSuspend();
  }
}

void MQTTHandler::onMqttConnectionSucceed(void *paContext, MQTTAsync_successData *){
  DEVLOG_INFO("MQTT: successfully connected\n");
  {
    MQTTHandler *handler = static_cast<MQTTHandler *>(paContext);
    CCriticalRegion sectionState(smMQTTMutex);
    smMQTTS_STATE = SUBSCRIBING;
    handler->resumeSelfSuspend();
  }
}

void MQTTHandler::onMqttConnectionFailed(void *paContext, MQTTAsync_failureData *){
  DEVLOG_ERROR("MQTT connection failed.\n");
  {
    MQTTHandler *handler = static_cast<MQTTHandler *>(paContext);
    CCriticalRegion sectionState(smMQTTMutex);
    smMQTTS_STATE = NOT_CONNECTED;
    handler->resumeSelfSuspend();
  }
}

void MQTTHandler::onSubscribeSucceed(void* paContext, MQTTAsync_successData* ){
  if(0 != paContext){
    CCriticalRegion sectionState(smMQTTMutex);
    MQTTComLayer* layer = static_cast<MQTTComLayer*>(paContext);
    MQTTHandler& handler = ::getExtEvHandler<MQTTHandler>(*layer->getCommFB());
    DEVLOG_INFO("MQTT: Subscription succeed. Topic: -%s-\n", layer->getTopicName());

    handler.mToResubscribe.erase(layer);
    if(handler.mToResubscribe.isEmpty()){
      smMQTTS_STATE = ALL_SUBSCRIBED;
    }
    else{
      handler.resumeSelfSuspend();
    }
  }
}

void MQTTHandler::onSubscribeFailed(void* paContext, MQTTAsync_failureData*){
  if(0 != paContext){
    CCriticalRegion sectionState(smMQTTMutex);
    MQTTComLayer* layer = static_cast<MQTTComLayer*>(paContext);
    DEVLOG_ERROR("MQTT: Subscription failed. Topic: -%s-\n", layer->getTopicName());
    ::getExtEvHandler<MQTTHandler>(*layer->getCommFB()).resumeSelfSuspend();
  }
}

/*
 * END OF CALLBACKS AND START OF HELPER FUNCTIONS
 */

int MQTTHandler::mqttConnect(){
  DEVLOG_INFO("MQTT: Requesting connection to broker\n");
  int rc = MQTTAsync_connect(smClient, &smClientConnectionOptions);
  if(MQTTASYNC_SUCCESS != rc){
    DEVLOG_ERROR("MQTT: Request to mqtt library failed\n");
  }else{
    DEVLOG_INFO("MQTT: Connection to broker requested\n");
  }
  return rc;
}

int MQTTHandler::mqttSubscribe(const MQTTComLayer* paLayer){
  DEVLOG_INFO("MQTT: subscribing to topic -%s-\n", paLayer->getTopicName());
  MQTTAsync_responseOptions opts = MQTTAsync_responseOptions_initializer;
  opts.onSuccess = onSubscribeSucceed;
  opts.onFailure = onSubscribeFailed;
  opts.context = (void*)paLayer;
  int rc = MQTTAsync_subscribe(smClient, paLayer->getTopicName(), QOS, &opts);
  if(MQTTASYNC_SUCCESS != rc){ //call failed
    CCriticalRegion sectionState(smMQTTMutex);
    DEVLOG_INFO("MQTT: subscribe request failed with val = %d\n", rc);
    resumeSelfSuspend();
  }else{
    DEVLOG_INFO("MQTT: subscribe Requested\n");
  }
  return rc;
}

int MQTTHandler::registerLayer(const char* paAddress, const char* paClientId, MQTTComLayer* paLayer){
  if(smClient == 0){
    smClientId = paClientId;
    smAddress = paAddress;
    MQTTAsync_create(&smClient, smAddress.getValue(), smClientId.getValue(), MQTTCLIENT_PERSISTENCE_NONE, NULL);
    smClientConnectionOptions.keepAliveInterval = 20;
    smClientConnectionOptions.cleansession = 1;
    smClientConnectionOptions.onSuccess = onMqttConnectionSucceed;
    smClientConnectionOptions.onFailure = onMqttConnectionFailed;
    smClientConnectionOptions.context = this;

    if("" != gMqttClientConfigFile) { //file was provided

      CMQTTClientConfigFileParser::MQTTConfigFromFile result = CMQTTClientConfigFileParser::MQTTConfigFromFile(scmUsername, scmPassword);
      std::string endpoint = smAddress.getValue();

      if(CMQTTClientConfigFileParser::loadConfig(gMqttClientConfigFile, endpoint, result)) {
        smClientConnectionOptions.username = scmUsername.c_str();
        smClientConnectionOptions.password = scmPassword.c_str();
      } else {
        return eWrongClientID;
      }
    }

    if(MQTTASYNC_SUCCESS != MQTTAsync_setCallbacks(smClient, this, MQTTHandler::onMqttConnectionLost, onMqttMessageArrived, NULL)){
      return eConnectionFailed;
    }
    {
      CCriticalRegion sectionState(smMQTTMutex);
      if(MQTTASYNC_SUCCESS != mqttConnect()){
        return eConnectionFailed;
      }
      smMQTTS_STATE = CONNECTION_ASKED;
    }
  }
  else if((smClientId != paClientId) || (smAddress != paAddress)){
    return eWrongClientID;
  }
  {
    CCriticalRegion section(smMQTTMutex);
    mlayers.pushBack(paLayer);
    if (e_Subscriber == paLayer->getCommFB()->getComServiceType()){
      mToResubscribe.pushBack(paLayer);
      if(ALL_SUBSCRIBED == smMQTTS_STATE){
        smMQTTS_STATE = SUBSCRIBING;
        this->resumeSelfSuspend();
      }
    }

  }
  return eRegisterLayerSucceeded;
}

void MQTTHandler::unregisterLayer(MQTTComLayer* paLayer){
  CCriticalRegion section(smMQTTMutex);
  mlayers.erase(paLayer);
}

void MQTTHandler::enableHandler(void){
  if (!isAlive()){
    start();
  }
}

void MQTTHandler::disableHandler(void){
  if (isAlive()){
    setAlive(false);
    resumeSelfSuspend();
    end();
  }
}

void MQTTHandler::setPriority(int ){
  //TODO: Should also work empty
}

int MQTTHandler::getPriority(void) const{
  return 0;
}

void MQTTHandler::run(){
  while(isAlive()){
    bool needSleep = false;
    selfSuspend();
    if(!isAlive()){
      break;
    }
    {
      CCriticalRegion sectionState(smMQTTMutex);
      switch(smMQTTS_STATE){
        case NOT_CONNECTED:
          if (MQTTASYNC_SUCCESS == mqttConnect()){
             smMQTTS_STATE = CONNECTION_ASKED;
          }
          break;
        case SUBSCRIBING:{
          if(!mToResubscribe.isEmpty()){
            //only try subscribe one at a time. There were some problems in some cases when subscribing one after the other, because the MQTTAsync_subscribe hangs.
            const MQTTComLayer *layer = *mToResubscribe.begin();
            smMQTTMutex.unlock();
            if(MQTTASYNC_SUCCESS != mqttSubscribe(layer)){
              needSleep = true;
            }
            smMQTTMutex.lock();
          }
          else{
            smMQTTS_STATE = ALL_SUBSCRIBED;
          }
          break;
        }
        default:
          break;
      }
    }
    if(needSleep){
      //sleep in case a subscribe fails while connected
      needSleep = false;
      CThread::sleepThread(5000);
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
