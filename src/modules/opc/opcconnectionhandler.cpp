/*******************************************************************************
 * Copyright (c) 2012 -2014 AIT, fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Filip Andren, Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "opcconnectionhandler.h"
#include "opcconnection.h"

using namespace forte::com_infra;

DEFINE_SINGLETON(COpcConnectionHandler);

COpcConnectionHandler::COpcConnectionHandler(){

}

COpcConnectionHandler::~COpcConnectionHandler(){
  TOpcConnectionList::Iterator itEnd = m_lOpcConnectionList.end();
  for(TOpcConnectionList::Iterator it = m_lOpcConnectionList.begin(); it != itEnd; ++it){
    delete (*it);
  }
}

COpcConnection* COpcConnectionHandler::getOpcConnection(const char *pa_acHost, const char *pa_acServerName, const char* pa_acGroupName, unsigned long pa_nReqUpdateRate, float pa_nDeadBand, CComLayer* pa_pComCallback){

  COpcConnection *newConnection = findOpcConnection(pa_acHost, pa_acServerName);
  if(newConnection == NULL){
    newConnection = new COpcConnection(pa_acHost, pa_acServerName, GET_HANDLER_FROM_FB(*pa_pComCallback->getCommFB(), COpcEventHandler));

    m_lOpcConnectionList.push_back(newConnection);
  }

  newConnection->addGroup(pa_acGroupName, pa_nReqUpdateRate, pa_nDeadBand, pa_pComCallback);

  return newConnection;
}

void COpcConnectionHandler::removeOpcConnection(const char *pa_acHost, const char *pa_acServerName, const char* pa_acGroupName){
  COpcConnection *existingCon = findOpcConnection(pa_acHost, pa_acServerName);
  if(existingCon != NULL){
    existingCon->removeGroup(pa_acGroupName);

    if(existingCon->getGroupCount() == 0)
      deleteOpcConnection(pa_acHost, pa_acServerName);
  }
}

COpcConnection* COpcConnectionHandler::findOpcConnection(const char* pa_acHost, const char* pa_acServerName){
  TOpcConnectionList::Iterator itEnd = m_lOpcConnectionList.end();
  for(TOpcConnectionList::Iterator it = m_lOpcConnectionList.begin(); it != itEnd; ++it){
    if(strcmp(it->getHost(), pa_acHost) == 0 && strcmp(it->getServerName(), pa_acServerName) == 0)
      return (*it);
  }

  return NULL;
}

void COpcConnectionHandler::deleteOpcConnection(const char* pa_acHost, const char* pa_acServerName){
  TOpcConnectionList::Iterator itDelete = m_lOpcConnectionList.begin();
  TOpcConnectionList::Iterator it = m_lOpcConnectionList.begin();
  TOpcConnectionList::Iterator itEnd = m_lOpcConnectionList.end();

  if(it != itEnd){
    if(strcmp(it->getHost(), pa_acHost) == 0 && strcmp(it->getServerName(), pa_acServerName)){
      m_lOpcConnectionList.pop_front();
      return;
    }
    ++it;
    while(it != itEnd){
      if(strcmp(it->getHost(), pa_acHost) == 0 && strcmp(it->getServerName(), pa_acServerName)){
        m_lOpcConnectionList.eraseAfter(itDelete);
        return;
      }

      ++itDelete;
      ++it;
    }
  }
}
