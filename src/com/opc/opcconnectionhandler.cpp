/*******************************************************************************
 * Copyright (c) 2012 -2014 AIT, fortiss GmbH, Hit robot group
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Filip Andren, Alois Zoitl - initial API and implementation and/or initial documentation
 *   ys guo - Fix opc module compilation errors and deadlock bug
 *******************************************************************************/
#include "opcconnectionhandler.h"
#include "opcconnection.h"
#include <extevhandlerhelper.h>

#include "opceventhandler.h"
#include "commfb.h"
using namespace forte::com_infra;

DEFINE_SINGLETON(COpcConnectionHandler);

COpcConnectionHandler::COpcConnectionHandler(){

}

COpcConnectionHandler::~COpcConnectionHandler(){
  TOpcConnectionList::Iterator itEnd = mOpcConnectionList.end();
  for(TOpcConnectionList::Iterator it = mOpcConnectionList.begin(); it != itEnd; ++it){
    delete (*it);
  }
}

COpcConnection* COpcConnectionHandler::getOpcConnection(const char *paHost, const char *paServerName, const char* paGroupName, unsigned long paReqUpdateRate, float paDeadBand, CComLayer* paComCallback){

  COpcConnection *newConnection = findOpcConnection(paHost, paServerName);
  if(newConnection == nullptr){
    newConnection = new COpcConnection(paHost, paServerName, &(getExtEvHandler<COpcEventHandler>(*paComCallback->getCommFB())));

    mOpcConnectionList.pushBack(newConnection);
  }

  newConnection->addGroup(paGroupName, paReqUpdateRate, paDeadBand, paComCallback);

  return newConnection;
}

void COpcConnectionHandler::removeOpcConnection(const char *paHost, const char *paServerName, const char* paGroupName){
  COpcConnection *existingCon = findOpcConnection(paHost, paServerName);
  if(existingCon != nullptr){
    existingCon->removeGroup(paGroupName);

    if(existingCon->getGroupCount() == 0) {
      deleteOpcConnection(paHost, paServerName);
    }
  }
}

COpcConnection* COpcConnectionHandler::findOpcConnection(const char* paHost, const char* paServerName){
  TOpcConnectionList::Iterator itEnd = mOpcConnectionList.end();
  for(TOpcConnectionList::Iterator it = mOpcConnectionList.begin(); it != itEnd; ++it){
    if(strcmp(it->getHost(), paHost) == 0 && strcmp(it->getServerName(), paServerName) == 0) {
      return (*it);
    }
  }

  return nullptr;
}

void COpcConnectionHandler::deleteOpcConnection(const char* paHost, const char* paServerName){
  TOpcConnectionList::Iterator itDelete = mOpcConnectionList.begin();
  TOpcConnectionList::Iterator it = mOpcConnectionList.begin();
  TOpcConnectionList::Iterator itEnd = mOpcConnectionList.end();

  if(it != itEnd){
    if(strcmp(it->getHost(), paHost) == 0 && strcmp(it->getServerName(), paServerName)){
      mOpcConnectionList.popFront();
      return;
    }
    ++it;
    while(it != itEnd){
      if(strcmp(it->getHost(), paHost) == 0 && strcmp(it->getServerName(), paServerName)){
        mOpcConnectionList.eraseAfter(itDelete);
        return;
      }

      ++itDelete;
      ++it;
    }
  }
}
