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

MQTTClient_connectOptions MQTTHandler::clientConnectionOptions =
MQTTClient_connectOptions_initializer;

CSyncObject MQTTHandler::mLockMessageDelivery = CSyncObject();

MQTTClient MQTTHandler::client = 0;
char* MQTTHandler::mClientId = 0;
char* MQTTHandler::mAddress = 0;

MQTTHandler::MQTTHandler(){

}

MQTTHandler::~MQTTHandler(){
  MQTTClient_disconnect(client, 10000);
  MQTTClient_destroy(&client);
}

int MQTTHandler::mqttMessageArrived(void* context, char* topicName, int topicLen, MQTTClient_message* message){
  //TODO: Check if handler allowed


  if(0 != context){
    MQTTHandler *handler = static_cast<MQTTHandler *>(context);

    void* pPayLoad = message->payload;
    unsigned int payLoadSize = static_cast<unsigned int>(message->payloadlen);

    CCriticalRegion section(mLockMessageDelivery); //Start critical section proected the list
    for(CSinglyLinkedList<MQTTComLayer*>::Iterator it = handler->layers.begin(); it != handler->layers.end(); ++it){
      if(0 == strcmp((*it)->getTopicName(), topicName)){
        if(forte::com_infra::e_Nothing != (*it)->recvData(pPayLoad, payLoadSize)){
          handler->startNewEventChain((*it)->getCommFB());
        }
      }
    }
    //End critical section
  }
  MQTTClient_freeMessage(&message);
  MQTTClient_free(topicName);

  return 1;
}

int MQTTHandler::registerLayer(char* paAddress, char* paClientId, MQTTComLayer* paLayer){

  int rc;
  if(client == 0){
    mClientId = paClientId;
    mAddress = paAddress;
    MQTTClient_create(&client, mAddress, mClientId,
    MQTTCLIENT_PERSISTENCE_NONE, NULL);
    clientConnectionOptions.keepAliveInterval = 20;
    clientConnectionOptions.cleansession = 1;
    rc = MQTTClient_setCallbacks(client, this, NULL, MQTTHandler::mqttMessageArrived, NULL);
    if(rc != MQTTCLIENT_SUCCESS){
      return eConnectionFailed;
    }

    rc = MQTTClient_connect(client, &clientConnectionOptions);
    if(rc != MQTTCLIENT_SUCCESS){
      return eConnectionFailed;
    }
  }
  else if((0 != strcmp(paClientId, mClientId)) || (0 != strcmp(paAddress, mAddress))){
    return eWrongClientID;
  }
  MQTTHandler::layers.push_back(paLayer);
  return eRegisterLayerSucceeded;
}

void MQTTHandler::unregisterLayer(MQTTComLayer* paLayer){
  //TODO: Implement unregister; search list and delete node
}

void MQTTHandler::enableHandler(void){
  //TODO: Should also work empty
}

void MQTTHandler::disableHandler(void){
  //TODO: Should also work empty
}

void MQTTHandler::setPriority(int pa_nPriority){
  //TODO: Should also work empty
}

int MQTTHandler::getPriority(void) const{
  return 0;
}
