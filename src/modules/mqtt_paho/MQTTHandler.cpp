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

#include <unistd.h>

DEFINE_SINGLETON(MQTTHandler);

MQTTAsync_connectOptions MQTTHandler::smClientConnectionOptions =
    MQTTAsync_connectOptions_initializer;

CSyncObject MQTTHandler::smLayersMutex = CSyncObject();

MQTTAsync MQTTHandler::smClient = 0;
CIEC_STRING MQTTHandler::smClientId;
CIEC_STRING MQTTHandler::smAddress;

MQTTStates MQTTHandler::smMQTTS_STATE = NOT_CONNECTED;
forte::arch::CSemaphore MQTTHandler::mStateSemaphore = forte::arch::CSemaphore();
unsigned int  MQTTHandler::mWaiting = 0;
CSyncObject MQTTHandler::mWaitingMutex = CSyncObject();
CSyncObject MQTTHandler::mStateMutex = CSyncObject();
CSyncObject MQTTHandler::mToResubscribeMutex = CSyncObject();


MQTTHandler::MQTTHandler(){
  if(!isAlive()){
    start();
  }
}

MQTTHandler::~MQTTHandler(){
  if(isAlive()){
    setAlive(false);
    resumeSelfSuspend();
    end();
  }

  MQTTAsync_disconnectOptions disc_opts = MQTTAsync_disconnectOptions_initializer;
  disc_opts.timeout = 10000;
  MQTTAsync_disconnect(smClient, &disc_opts);
  MQTTAsync_destroy(&smClient);
}

int MQTTHandler::mqttMessageArrived(void* context, char* topicName, int, MQTTAsync_message* message){
  //TODO: Check if handler allowed
  if(0 != context){
    MQTTHandler *handler = static_cast<MQTTHandler *>(context);

    void* pPayLoad = message->payload;
    unsigned int payLoadSize = static_cast<unsigned int>(message->payloadlen);

    CCriticalRegion section(smLayersMutex); //Start critical section protect the list
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

void MQTTHandler::mqttConnectionLost(void* context, char* cause){
  DEVLOG_ERROR("MQTT: Disconnected from broker. Cause: %s\n", cause);
  if(0 != context){

    MQTTHandler *handler = static_cast<MQTTHandler *>(context);

    CCriticalRegion sectionState(handler->mStateMutex);
    CCriticalRegion sectionResubscribe(handler->mToResubscribeMutex);
    CCriticalRegion section(handler->smLayersMutex);

    smMQTTS_STATE = NOT_CONNECTED;

    handler->mToResubscribe.clearAll();
    for(CSinglyLinkedList<MQTTComLayer*>::Iterator it = handler->mlayers.begin(); it != handler->mlayers.end(); ++it){
      if(e_Subscriber == (*it)->getCommFB()->getComServiceType()){
        handler->mToResubscribe.push_back((*it));
      }
    }
    handler->resumeSelfSuspend();
  }
}

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

void MQTTHandler::mqttConnectionSucceed(void *, MQTTAsync_successData *){
  DEVLOG_INFO("MQTT: successfully connected\n");
  {
    CCriticalRegion sectionState(MQTTHandler::mStateMutex);
    smMQTTS_STATE = SUBSCRIBING;
    MQTTHandler::resumeSelfSuspend();
  }
}

void MQTTHandler::mqttConnectionFailed(void *, MQTTAsync_failureData *){
  DEVLOG_ERROR("MQTT connection failed.\n");
  {
    CCriticalRegion sectionState(MQTTHandler::mStateMutex);
    smMQTTS_STATE = NOT_CONNECTED;
    MQTTHandler::resumeSelfSuspend();
  }
}

int MQTTHandler::mqttSubscribe(MQTTComLayer* pa_comLayer){
  DEVLOG_INFO("MQTT: subscribing to topic -%s-\n", pa_comLayer->getTopicName());
  MQTTAsync_responseOptions opts = MQTTAsync_responseOptions_initializer;
  opts.onSuccess = MQTTHandler::onSubscribeSucceed;
  opts.onFailure = MQTTHandler::onSubscribeFailed;
  opts.context = pa_comLayer;
  int rc = MQTTAsync_subscribe(smClient, pa_comLayer->getTopicName(), QOS, &opts);
  if(MQTTASYNC_SUCCESS != rc){
    DEVLOG_INFO("MQTT: subscribe request failed with val = %d\n", rc);
    MQTTHandler::addLayerToSubscribe(pa_comLayer);
    MQTTHandler::resumeSelfSuspend();;
  }else{
    DEVLOG_INFO("MQTT: subscribe Requested\n");
  }
  return rc;
}

void MQTTHandler::onSubscribeSucceed(void* context, MQTTAsync_successData* ){
  if(0 != context){
    MQTTComLayer* layer = static_cast<MQTTComLayer*>(context);
    DEVLOG_INFO("MQTT: Subscription succeed. Topic: -%s-\n", layer->getTopicName());
    {

      CCriticalRegion section(MQTTHandler::mToResubscribeMutex);
      CCriticalRegion sectionState(MQTTHandler::mStateMutex);

      popLayerFromList(layer, &MQTTHandler::getInstance().mToResubscribe);
      if(MQTTHandler::getInstance().mToResubscribe.isEmpty()){
        smMQTTS_STATE = ALL_SUBSCRIBED;
      }
      else{
        MQTTHandler::resumeSelfSuspend();;
        smMQTTS_STATE = SUBSCRIBING;
      }
    }
  }
}

void MQTTHandler::onSubscribeFailed(void* context, MQTTAsync_failureData*){
  if(0 != context){
    MQTTComLayer* layer = static_cast<MQTTComLayer*>(context);
    DEVLOG_ERROR("MQTT: Subscription failed. Topic: -%s-\n", layer->getTopicName());
    MQTTHandler::addLayerToSubscribe(layer);
    MQTTHandler::resumeSelfSuspend();
  }
}

int MQTTHandler::registerLayer(char* paAddress, char* paClientId, MQTTComLayer* paLayer){
  if(smClient == 0){
    smClientId = paClientId;
    smAddress = paAddress;
    MQTTAsync_create(&smClient, smAddress.getValue(), smClientId.getValue(), MQTTCLIENT_PERSISTENCE_NONE, NULL);
    smClientConnectionOptions.keepAliveInterval = 20;
    smClientConnectionOptions.cleansession = 1;
    smClientConnectionOptions.onSuccess = MQTTHandler::mqttConnectionSucceed;
    smClientConnectionOptions.onFailure = MQTTHandler::mqttConnectionFailed;
    smClientConnectionOptions.context = this;
    if(MQTTASYNC_SUCCESS != MQTTAsync_setCallbacks(smClient, this, MQTTHandler::mqttConnectionLost, MQTTHandler::mqttMessageArrived, NULL)){
      return eConnectionFailed;
    }
    {
      CCriticalRegion sectionState(mStateMutex);
      if(MQTTASYNC_SUCCESS != MQTTHandler::mqttConnect()){
        return eConnectionFailed;
      }
      smMQTTS_STATE = CONNECTION_ASKED;
    }
  }
  else if((smClientId != paClientId) || (smAddress != paAddress)){
    return eWrongClientID;
  }
  {
    CCriticalRegion section(smLayersMutex); //Start critical section protect the list
    mlayers.push_back(paLayer);
  }
  return eRegisterLayerSucceeded;
}

void MQTTHandler::unregisterLayer(MQTTComLayer* paLayer){
  CCriticalRegion section(smLayersMutex);
  popLayerFromList(paLayer, &mlayers);
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

void MQTTHandler::addLayerToSubscribe(MQTTComLayer* paLayer){
  CCriticalRegion sectionState(MQTTHandler::mStateMutex);
  CCriticalRegion section(MQTTHandler::mToResubscribeMutex);
  smMQTTS_STATE = SUBSCRIBING;
  bool found = false;
  for(CSinglyLinkedList<MQTTComLayer*>::Iterator it = MQTTHandler::getInstance().mToResubscribe.begin(); it != MQTTHandler::getInstance().mToResubscribe.end(); ++it){
    if(*it == paLayer){
      found = true;
      break;
    }
  }
  if(!found){
    MQTTHandler::getInstance().mToResubscribe.push_back(paLayer);
  }
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
      CCriticalRegion sectionState(mStateMutex);
      switch(smMQTTS_STATE){
        case NOT_CONNECTED:
          if (MQTTASYNC_SUCCESS == MQTTHandler::mqttConnect()){
             smMQTTS_STATE = CONNECTION_ASKED;
          }
          break;
        case SUBSCRIBING:{
          CSinglyLinkedList<MQTTComLayer*> listCopy;
          {
            CCriticalRegion section(mToResubscribeMutex);
            if(!mToResubscribe.isEmpty()){
              //Copy the mutexed list to a new one, in order to avoid problems when the subscribe's callbacks try to access the mutexed list
              for(CSinglyLinkedList<MQTTComLayer*>::Iterator it = mToResubscribe.begin(); it != mToResubscribe.end(); ++it){
                listCopy.push_back((*it));
              }
            }
          }
          if(!listCopy.isEmpty()){
            CSinglyLinkedList<MQTTComLayer*>::Iterator it = listCopy.begin(); //only try subscribe one at a time. There were some problems in some cases when subscribing one after the other, because the MQTTAsync_subscribe hangs.
            if(MQTTASYNC_SUCCESS != MQTTHandler::mqttSubscribe((*it))){
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
#ifdef WIN32
      Sleep(5000);
#else
      usleep(5000000);
#endif
    }
  }
}

void MQTTHandler::resumeSelfSuspend(){
  CCriticalRegion section(mWaitingMutex);
  if(0 == mWaiting){
    mWaiting++;
    mStateSemaphore.semInc();
  }
}

void MQTTHandler::selfSuspend(){
  mStateSemaphore.semWaitIndefinitly();
  {
    CCriticalRegion section(mWaitingMutex);
    mWaiting = 0;
  }

}
