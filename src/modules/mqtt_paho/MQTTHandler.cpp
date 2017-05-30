/*******************************************************************************
 * Copyright (c) 2013, 2014 ACIN
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 * Martin Melik Merkumians - initial API and implementation and/or initial documentation
 *******************************************************************************/


#include <MQTTHandler.h>
#include <string.h>
#include <criticalregion.h>
#include <commfb.h>

DEFINE_SINGLETON(MQTTHandler);

MQTTAsync_connectOptions MQTTHandler::smClientConnectionOptions =
    MQTTAsync_connectOptions_initializer;

CSyncObject MQTTHandler::smMQTTMutex = CSyncObject();

MQTTAsync MQTTHandler::smClient = 0;
CIEC_STRING MQTTHandler::smClientId;
CIEC_STRING MQTTHandler::smAddress;

MQTTStates MQTTHandler::smMQTTS_STATE = NOT_CONNECTED;
forte::arch::CSemaphore MQTTHandler::mStateSemaphore = forte::arch::CSemaphore();
bool MQTTHandler::mIsSemaphoreEmpty = true;

MQTTHandler::MQTTHandler(){
  if(!isAlive()){
    start();
  }
}

MQTTHandler::~MQTTHandler(){
  if(isAlive()){
    CCriticalRegion sectionState(smMQTTMutex);
    setAlive(false);
    resumeSelfSuspend();
    end();
  }

  MQTTAsync_disconnectOptions disc_opts = MQTTAsync_disconnectOptions_initializer;
  disc_opts.timeout = 10000;
  MQTTAsync_disconnect(smClient, &disc_opts);
  MQTTAsync_destroy(&smClient);
}

/*
 * START OF CALLBACKS
 */
int MQTTHandler::onMqttMessageArrived(void* context, char* topicName, int, MQTTAsync_message* message){
  //TODO: Check if handler allowed
  if(0 != context){
    CCriticalRegion section(smMQTTMutex);

    MQTTHandler *handler = static_cast<MQTTHandler *>(context);
    void* pPayLoad = message->payload;
    unsigned int payLoadSize = static_cast<unsigned int>(message->payloadlen);

    for(CSinglyLinkedList<MQTTComLayer*>::Iterator it = handler->mlayers.begin(); it != handler->mlayers.end(); ++it){
      if(0 == strcmp((*it)->getTopicName(), topicName)){
        if(forte::com_infra::e_Nothing != (*it)->recvData(pPayLoad, payLoadSize)){
          handler->startNewEventChain((*it)->getCommFB());
        }
      }
    }
    //End critical section
  }
  MQTTAsync_freeMessage(&message);
  MQTTAsync_free(topicName);

  return 1;
}

void MQTTHandler::onMqttConnectionLost(void* context, char* cause){
  DEVLOG_ERROR("MQTT: Disconnected from broker. Cause: %s\n", cause);
  if(0 != context){
    CCriticalRegion section(smMQTTMutex);

    MQTTHandler *handler = static_cast<MQTTHandler *>(context);
    smMQTTS_STATE = NOT_CONNECTED;

    handler->mToResubscribe.clearAll();
    for(CSinglyLinkedList<MQTTComLayer*>::Iterator it = handler->mlayers.begin(); it != handler->mlayers.end(); ++it){
      if(e_Subscriber == (*it)->getCommFB()->getComServiceType()){
        handler->mToResubscribe.push_back((*it));
      }
    }
    MQTTHandler::getInstance().resumeSelfSuspend();
  }
}

void MQTTHandler::onMqttConnectionSucceed(void *, MQTTAsync_successData *){
  DEVLOG_INFO("MQTT: successfully connected\n");
  {
    CCriticalRegion sectionState(smMQTTMutex);
    smMQTTS_STATE = SUBSCRIBING;
    MQTTHandler::getInstance().resumeSelfSuspend();
  }
}

void MQTTHandler::onMqttConnectionFailed(void *, MQTTAsync_failureData *){
  DEVLOG_ERROR("MQTT connection failed.\n");
  {
    CCriticalRegion sectionState(smMQTTMutex);
    smMQTTS_STATE = NOT_CONNECTED;
    MQTTHandler::getInstance().resumeSelfSuspend();
  }
}

void MQTTHandler::onSubscribeSucceed(void* context, MQTTAsync_successData* ){
  if(0 != context){
    CCriticalRegion sectionState(smMQTTMutex);
    MQTTComLayer* layer = static_cast<MQTTComLayer*>(context);
    DEVLOG_INFO("MQTT: Subscription succeed. Topic: -%s-\n", layer->getTopicName());

    MQTTHandler::getInstance().popLayerFromList(layer, &MQTTHandler::getInstance().mToResubscribe);
    if(MQTTHandler::getInstance().mToResubscribe.isEmpty()){
      smMQTTS_STATE = ALL_SUBSCRIBED;
    }
    else{
      MQTTHandler::getInstance().resumeSelfSuspend();
    }
  }
}

void MQTTHandler::onSubscribeFailed(void* context, MQTTAsync_failureData*){
  if(0 != context){
    CCriticalRegion sectionState(smMQTTMutex);
    MQTTComLayer* layer = static_cast<MQTTComLayer*>(context);
    DEVLOG_ERROR("MQTT: Subscription failed. Topic: -%s-\n", layer->getTopicName());
    MQTTHandler::getInstance().resumeSelfSuspend();
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

int MQTTHandler::mqttSubscribe(MQTTComLayer* pa_comLayer){
  DEVLOG_INFO("MQTT: subscribing to topic -%s-\n", pa_comLayer->getTopicName());
  MQTTAsync_responseOptions opts = MQTTAsync_responseOptions_initializer;
  opts.onSuccess = onSubscribeSucceed;
  opts.onFailure = onSubscribeFailed;
  opts.context = pa_comLayer;
  int rc = MQTTAsync_subscribe(smClient, pa_comLayer->getTopicName(), QOS, &opts);
  if(MQTTASYNC_SUCCESS != rc){ //call failed
    CCriticalRegion sectionState(smMQTTMutex);
    DEVLOG_INFO("MQTT: subscribe request failed with val = %d\n", rc);
    resumeSelfSuspend();
  }else{
    DEVLOG_INFO("MQTT: subscribe Requested\n");
  }
  return rc;
}

void MQTTHandler::popLayerFromList(MQTTComLayer* paLayer, CSinglyLinkedList<MQTTComLayer*> *pa_list){
  CSinglyLinkedList<MQTTComLayer*>::Iterator itRunner(pa_list->begin());
  CSinglyLinkedList<MQTTComLayer*>::Iterator itRefNode(pa_list->end());
  CSinglyLinkedList<MQTTComLayer*>::Iterator itEnd(pa_list->end());

  while(itRunner != itEnd){
    if(*itRunner == paLayer){
      if(itRefNode == itEnd){
        pa_list->pop_front();
      }
      else{
        pa_list->eraseAfter(itRefNode);
      }
      break;
    }
    itRefNode = itRunner;
    ++itRunner;
  }
}

int MQTTHandler::registerLayer(char* paAddress, char* paClientId, MQTTComLayer* paLayer){
  if(smClient == 0){
    smClientId = paClientId;
    smAddress = paAddress;
    MQTTAsync_create(&smClient, smAddress.getValue(), smClientId.getValue(), MQTTCLIENT_PERSISTENCE_NONE, NULL);
    smClientConnectionOptions.keepAliveInterval = 20;
    smClientConnectionOptions.cleansession = 1;
    smClientConnectionOptions.onSuccess = onMqttConnectionSucceed;
    smClientConnectionOptions.onFailure = onMqttConnectionFailed;
    smClientConnectionOptions.context = this;
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
    mlayers.push_back(paLayer);
    if (e_Subscriber == paLayer->getCommFB()->getComServiceType()){
      mToResubscribe.push_back(paLayer);
      if(ALL_SUBSCRIBED == smMQTTS_STATE){
        smMQTTS_STATE = SUBSCRIBING;
        MQTTHandler::getInstance().resumeSelfSuspend();
      }
    }

  }
  return eRegisterLayerSucceeded;
}

void MQTTHandler::unregisterLayer(MQTTComLayer* paLayer){
  CCriticalRegion section(smMQTTMutex);
  popLayerFromList(paLayer, &mlayers);
}

void MQTTHandler::enableHandler(void){
  if (!isAlive()){
    start();
  }
}

void MQTTHandler::disableHandler(void){
  if (isAlive()){
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
            CSinglyLinkedList<MQTTComLayer*>::Iterator it = mToResubscribe.begin();
            if(MQTTASYNC_SUCCESS != mqttSubscribe(*it)){
              needSleep = true;
            }
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
    mStateSemaphore.semInc();
    mIsSemaphoreEmpty = false;
  }
}

void MQTTHandler::selfSuspend(){
  mStateSemaphore.semWaitIndefinitly();
  {
    CCriticalRegion section(smMQTTMutex);
    mIsSemaphoreEmpty = true;
  }
}
