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

MQTTClient_connectOptions MQTTHandler::smClientConnectionOptions =
MQTTClient_connectOptions_initializer;

CSyncObject MQTTHandler::smLockMessageDelivery = CSyncObject();

MQTTClient MQTTHandler::smClient = 0;
CIEC_STRING MQTTHandler::smClientId;
CIEC_STRING MQTTHandler::smAddress;

MQTTHandler::MQTTHandler(){

}

MQTTHandler::~MQTTHandler(){
  MQTTClient_disconnect(smClient, 10000);
  MQTTClient_destroy(&smClient);
}

int MQTTHandler::mqttMessageArrived(void* context, char* topicName, int , MQTTClient_message* message){
  //TODO: Check if handler allowed


  if(0 != context){
    MQTTHandler *handler = static_cast<MQTTHandler *>(context);

    void* pPayLoad = message->payload;
    unsigned int payLoadSize = static_cast<unsigned int>(message->payloadlen);

    CCriticalRegion section(smLockMessageDelivery); //Start critical section protect the list
    for(CSinglyLinkedList<MQTTComLayer*>::Iterator it = handler->mlayers.begin(); it != handler->mlayers.end(); ++it){
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
  if(smClient == 0){
    smClientId = paClientId;
    smAddress = paAddress;
    MQTTClient_create(&smClient, smAddress.getValue(), smClientId.getValue(), MQTTCLIENT_PERSISTENCE_NONE, NULL);
    smClientConnectionOptions.keepAliveInterval = 20;
    smClientConnectionOptions.cleansession = 1;
    int rc = MQTTClient_setCallbacks(smClient, this, NULL, MQTTHandler::mqttMessageArrived, NULL);
    if(rc != MQTTCLIENT_SUCCESS){
      return eConnectionFailed;
    }

    rc = MQTTClient_connect(smClient, &smClientConnectionOptions);
    if(rc != MQTTCLIENT_SUCCESS){
      return eConnectionFailed;
    }
  }
  else if((smClientId != paClientId) || (smAddress != paAddress)){
    return eWrongClientID;
  }
  CCriticalRegion section(smLockMessageDelivery); //Start critical section protect the list
  mlayers.push_back(paLayer);
  return eRegisterLayerSucceeded;
}

void MQTTHandler::unregisterLayer(MQTTComLayer* paLayer){
  CCriticalRegion section(smLockMessageDelivery); //Start critical section protect the list

  CSinglyLinkedList<MQTTComLayer*>::Iterator itRunner(mlayers.begin());
  CSinglyLinkedList<MQTTComLayer*>::Iterator itRefNode(mlayers.end());
  CSinglyLinkedList<MQTTComLayer*>::Iterator itEnd(mlayers.end());

  while(itRunner != itEnd){
    if(*itRunner == paLayer){
      if(itRefNode == itEnd){
        mlayers.pop_front();
      }
      else{
        mlayers.eraseAfter(itRefNode);
      }
      break;
    }

    itRefNode = itRunner;
    ++itRunner;
  }
}

void MQTTHandler::enableHandler(void){
  //TODO: Should also work empty
}

void MQTTHandler::disableHandler(void){
  //TODO: Should also work empty
}

void MQTTHandler::setPriority(int ){
  //TODO: Should also work empty
}

int MQTTHandler::getPriority(void) const{
  return 0;
}
