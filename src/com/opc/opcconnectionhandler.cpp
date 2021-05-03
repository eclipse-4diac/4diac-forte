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
  TOpcConnectionList::Iterator itEnd = m_lOpcConnectionList.end();
  for(TOpcConnectionList::Iterator it = m_lOpcConnectionList.begin(); it != itEnd; ++it){
    delete (*it);
  }
}

COpcConnection* COpcConnectionHandler::getOpcConnection(const char *pa_acHost, const char *pa_acServerName, const char* pa_acGroupName, unsigned long pa_nReqUpdateRate, float pa_nDeadBand, CComLayer* pa_pComCallback){

  COpcConnection *newConnection = findOpcConnection(pa_acHost, pa_acServerName);
  if(newConnection == NULL){
    newConnection = new COpcConnection(pa_acHost, pa_acServerName, &(getExtEvHandler<COpcEventHandler>(*pa_pComCallback->getCommFB())));

    m_lOpcConnectionList.pushBack(newConnection);
  }

  newConnection->addGroup(pa_acGroupName, pa_nReqUpdateRate, pa_nDeadBand, pa_pComCallback);

  return newConnection;
}

void COpcConnectionHandler::removeOpcConnection(const char *pa_acHost, const char *pa_acServerName, const char* pa_acGroupName){
  COpcConnection *existingCon = findOpcConnection(pa_acHost, pa_acServerName);
  if(existingCon != NULL){
    existingCon->removeGroup(pa_acGroupName);

    if(existingCon->getGroupCount() == 0) {
      deleteOpcConnection(pa_acHost, pa_acServerName);
    }
  }
}

COpcConnection* COpcConnectionHandler::findOpcConnection(const char* pa_acHost, const char* pa_acServerName){
  TOpcConnectionList::Iterator itEnd = m_lOpcConnectionList.end();
  for(TOpcConnectionList::Iterator it = m_lOpcConnectionList.begin(); it != itEnd; ++it){
    if(strcmp(it->getHost(), pa_acHost) == 0 && strcmp(it->getServerName(), pa_acServerName) == 0) {
      return (*it);
    }
  }

  return NULL;
}

void COpcConnectionHandler::deleteOpcConnection(const char* pa_acHost, const char* pa_acServerName){
  TOpcConnectionList::Iterator itDelete = m_lOpcConnectionList.begin();
  TOpcConnectionList::Iterator it = m_lOpcConnectionList.begin();
  TOpcConnectionList::Iterator itEnd = m_lOpcConnectionList.end();

  if(it != itEnd){
    if(strcmp(it->getHost(), pa_acHost) == 0 && strcmp(it->getServerName(), pa_acServerName)){
      m_lOpcConnectionList.popFront();
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
