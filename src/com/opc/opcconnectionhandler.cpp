/*******************************************************************************
 * Copyright (c) 2012, 2022 AIT, fortiss GmbH, Hit robot group
 *               2024 Samator Indo Gas
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Filip Andren, Alois Zoitl - initial API and implementation and/or initial documentation
 *   ys guo - Fix opc module compilation errors and deadlock bug
 *   Ketut Kumajaya - Code refactoring from char* to std::string
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

COpcConnection* COpcConnectionHandler::getOpcConnection(const std::string& paHost, const std::string& paServerName, CComLayer* paComCallback){
  COpcConnection *newConnection = findOpcConnection(paHost, paServerName);
  if(newConnection == nullptr){
    newConnection = new COpcConnection(paHost, paServerName, &(getExtEvHandler<COpcEventHandler>(*paComCallback->getCommFB())));

    mOpcConnectionList.pushBack(newConnection);
  }
  return newConnection;
}

void COpcConnectionHandler::removeOpcConnection(const std::string& paHost, const std::string& paServerName, const std::string& paGroupName){
  COpcConnection *existingCon = findOpcConnection(paHost, paServerName);
  if(existingCon != nullptr){
    if(0 == existingCon->send_disconnect(paGroupName)){
      DEVLOG_INFO("The connection is closed and the group is removed![%s].\n",paGroupName.c_str());
    }
    else{
      DEVLOG_INFO("The connection is kept untouched![%s].\n",paGroupName.c_str());
    }
  }
}

COpcConnection* COpcConnectionHandler::findOpcConnection(const std::string& paHost, const std::string& paServerName){
  TOpcConnectionList::Iterator itEnd = mOpcConnectionList.end();
  for(TOpcConnectionList::Iterator it = mOpcConnectionList.begin(); it != itEnd; ++it){
    if((it->getHost() == paHost) && (it->getServerName() == paServerName)) {
      return (*it);
    }
  }
  return nullptr;
}

void COpcConnectionHandler::deleteOpcConnection(const std::string& paHost, const std::string& paServerName){
  TOpcConnectionList::Iterator itDelete = mOpcConnectionList.begin();
  TOpcConnectionList::Iterator it = mOpcConnectionList.begin();
  TOpcConnectionList::Iterator itEnd = mOpcConnectionList.end();

  if(it != itEnd){
    if((it->getHost() == paHost) && (it->getServerName() == paServerName)){
      mOpcConnectionList.popFront();
      return;
    }
    ++it;
    while(it != itEnd){
      if((it->getHost() == paHost) && (it->getServerName() != paServerName)){
        mOpcConnectionList.eraseAfter(itDelete);
        return;
      }
      ++itDelete;
      ++it;
    }
  }
}
