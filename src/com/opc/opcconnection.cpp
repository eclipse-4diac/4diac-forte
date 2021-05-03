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
#include "opcconnection.h"
#include "opcconnectionimpl.h"
#include "opceventhandler.h"
#include "opcprocessvar.h"
#include "Cmd_AddConnection.h"
#include "Cmd_AddOPCProcessVar.h"
#include "Cmd_SetProcessVarValue.h"

using namespace forte::com_infra;

COpcConnection::COpcConnection(const char *pa_acHost, const char *pa_acServerName, COpcEventHandler* pa_eventHandler) :
    m_nGroupCount(0), m_eConnectionEvent(e_Disconnected), m_acHost(pa_acHost), m_acServerName(pa_acServerName), m_acGroupName(0), m_nReqUpdateRate(0), m_nRealUpdateRate(0), m_nDeadBand(0), m_bIsConnected(false), m_bBlockingConnect(false),
   m_eventHandler(pa_eventHandler){
  m_pOpcConnectionImpl = new COpcConnectionImpl(pa_acHost, pa_acServerName, this);
}

COpcConnection::~COpcConnection(){
  delete m_pOpcConnectionImpl;
}

void COpcConnection::addGroup(const char* pa_acGroupName, unsigned long pa_nReqUpdateRate, float pa_nDeadBand, CComLayer* pa_pComCallback){
  m_oSync.lock();
  m_lOpcGroupMapList.pushBack(new SOpcGroupMap(pa_acGroupName, m_eventHandler->addComCallback(pa_pComCallback)));
  m_oSync.unlock();

  m_pOpcConnectionImpl->addGroup(pa_acGroupName, pa_nReqUpdateRate, pa_nDeadBand);

  m_nGroupCount++;
}

void COpcConnection::removeGroup(const char* pa_acGroupName){
  m_oSync.lock();
  TOpcGroupMapList::Iterator itDelete = m_lOpcGroupMapList.begin();
  TOpcGroupMapList::Iterator it_group = m_lOpcGroupMapList.begin();
  TOpcGroupMapList::Iterator itEnd_group = m_lOpcGroupMapList.end();

  if(it_group != itEnd_group){
    if(strcmp(it_group->m_acGroupName, pa_acGroupName) == 0){
      m_lOpcGroupMapList.popFront();
      m_nGroupCount--;
      m_oSync.unlock();
      return;
    }
    ++it_group;
    while(it_group != itEnd_group){
      if(strcmp(it_group->m_acGroupName, pa_acGroupName) == 0){
        m_lOpcGroupMapList.eraseAfter(itDelete);
        m_nGroupCount--;
        m_oSync.unlock();
        return;
      }

      ++itDelete;
      ++it_group;
    }
  }

  m_oSync.unlock();
}

int COpcConnection::send_connect(){
  switch (m_eConnectionEvent){
    case e_Disconnected:
      m_eConnectionEvent = e_Connecting;
      m_eventHandler->sendCommand(new CCmd_AddConnection(m_pOpcConnectionImpl));
      return 0;
    case e_Connecting:
      return 0;
    default: // all other connection states
      return 1;
  }
}

int COpcConnection::send_connect(bool pa_bBlocking){
  m_bBlockingConnect = pa_bBlocking;
  if(pa_bBlocking && !m_bIsConnected){
    m_pOpcConnectionImpl->connect();
    return 1;
  }

  return send_connect();
}

int COpcConnection::send_addItem(COpcProcessVar* pa_pNewItem){
  m_oSync.lock();
  TOpcGroupMapList::Iterator itEnd_group = m_lOpcGroupMapList.end();
  for(TOpcGroupMapList::Iterator it_group = m_lOpcGroupMapList.begin(); it_group != itEnd_group; ++it_group){
    if(strcmp(it_group->m_acGroupName, pa_pNewItem->getItemGroupName()) == 0){
      if(pa_pNewItem->getItemFunction() == COpcProcessVar::e_FBOutput){

        //check if item already added
        TItemDataList::Iterator itEnd_item = (*it_group)->m_lReadItemsList.end();
        for(TItemDataList::Iterator it_item = (*it_group)->m_lReadItemsList.begin(); it_item != itEnd_item; ++it_item){
          if(strcmp(it_item->m_acItemName, pa_pNewItem->getItemName()) == 0){
            m_oSync.unlock();
            return 1;
          }
        }

        it_group->m_lReadItemsList.pushBack(new SOpcItemData(pa_pNewItem->getItemName()));
        break;

      }
      else if(pa_pNewItem->getItemFunction() == COpcProcessVar::e_FBInput){
        //check if item already added
        TItemDataList::Iterator itEnd_item = (*it_group)->m_lWriteItemsList.end();
        for(TItemDataList::Iterator it_item = (*it_group)->m_lWriteItemsList.begin(); it_item != itEnd_item; ++it_item){
          if(strcmp(it_item->m_acItemName, pa_pNewItem->getItemName()) == 0){
            m_oSync.unlock();
            return 1;
          }
        }

        it_group->m_lWriteItemsList.pushBack(new SOpcItemData(pa_pNewItem->getItemName()));
        break;

      }
    }
  }
  m_oSync.unlock();

  if(m_eConnectionEvent == e_Connected){
    m_eventHandler->sendCommand(new CCmd_AddOPCProcessVar(m_pOpcConnectionImpl, pa_pNewItem));

    return 0;
  }

  return -1;
}

int COpcConnection::send_sendItemData(COpcProcessVar* pa_pItem){
  if(pa_pItem->getIsActive()) {
    m_eventHandler->sendCommand(new CCmd_SetProcessVarValue(pa_pItem));
  }
  return 0;
}

void COpcConnection::response_connect(bool pa_bConnectionState){
  m_bIsConnected = pa_bConnectionState;
  if(pa_bConnectionState) {
    m_eConnectionEvent = e_Connected;
  } else {
    m_eConnectionEvent = e_ConnectionFailed;
  }
    //m_eConnectionEvent = e_Disconnected;

  if(!m_bBlockingConnect){

    TOpcGroupMapList::Iterator itEnd = m_lOpcGroupMapList.end();
    for(TOpcGroupMapList::Iterator it = m_lOpcGroupMapList.begin(); it != itEnd; ++it){
      m_eventHandler->executeComCallback((*it)->m_nCallbackDesc);
    }

  }
}

void COpcConnection::response_dataReceived(const char *pa_acGroupName, TItemDataList & pa_lItemDataList){
  // Loop through OpcGroups
  TOpcGroupMapList::Iterator itEnd_group = m_lOpcGroupMapList.end();
  for(TOpcGroupMapList::Iterator it_group = m_lOpcGroupMapList.begin(); it_group != itEnd_group; ++it_group){

    if(strcmp(it_group->m_acGroupName, pa_acGroupName) == 0){

      // Loop through OpcItems in OpcGroup
      TItemDataList::Iterator itEnd_item = (*it_group)->m_lReadItemsList.end();
      for(TItemDataList::Iterator it_item = (*it_group)->m_lReadItemsList.begin(); it_item != itEnd_item; ++it_item){

        // Loop through OpcItems in ItemDataList
        TItemDataList::Iterator itEnd_newItem = pa_lItemDataList.end();
        TItemDataList::Iterator itErase = pa_lItemDataList.begin();
        for(TItemDataList::Iterator it_newItem = pa_lItemDataList.begin(); it_newItem != itEnd_newItem; ++it_newItem){

          if(strcmp(it_newItem->m_acItemName, it_item->m_acItemName) == 0){
            it_item->m_oItemData = it_newItem->m_oItemData;

            if(it_newItem == pa_lItemDataList.begin()) {
              pa_lItemDataList.popFront();
            } else {
              pa_lItemDataList.eraseAfter(itErase);
            }

            break;
          }

          if(it_newItem != pa_lItemDataList.begin()) {
            ++itErase;
          }
        }
          if(pa_lItemDataList.isEmpty()) {
            break;
          }
      }

      // Change state
      m_eConnectionEvent = e_DataReceived;

      // Notify Com Layer
      m_eventHandler->executeComCallback(it_group->m_nCallbackDesc);
      break;
    }
  }
}

void COpcConnection::response_itemAdded(COpcProcessVar* pa_pOpcItem){
  // Loop through OpcGroups
  m_oSync.lock();
  TOpcGroupMapList::Iterator itEnd_group = m_lOpcGroupMapList.end();
  for(TOpcGroupMapList::Iterator it_group = m_lOpcGroupMapList.begin(); it_group != itEnd_group; ++it_group){
    if(strcmp(it_group->m_acGroupName, pa_pOpcItem->getItemGroupName()) == 0){
      // Change state
      if (pa_pOpcItem->getIsActive()) {
        m_eConnectionEvent = e_ItemAddedOk;
      } else {
        m_eConnectionEvent = e_ItemAddedFailed;
      }
      // Notify Com Layer
      m_eventHandler->executeComCallback(it_group->m_nCallbackDesc);
      break;
    }
  }
  m_oSync.unlock();
}

int COpcConnection::receiveData(const char* pa_acGroupName, TOpcProcessVarList * pa_lOpcProcessVarList){
  int nrData = 0;
  // TODO Case when lists do not match in size

  m_oSync.lock();
  TOpcGroupMapList::Iterator itEnd_group = m_lOpcGroupMapList.end();
  for(TOpcGroupMapList::Iterator it_group = m_lOpcGroupMapList.begin(); it_group != itEnd_group; ++it_group){

    if(strcmp(it_group->m_acGroupName, pa_acGroupName) == 0){

      TItemDataList::Iterator itEnd_item = it_group->m_lReadItemsList.end();
      TItemDataList::Iterator it_item = it_group->m_lReadItemsList.begin();

      TOpcProcessVarList::Iterator itEnd_procVar = pa_lOpcProcessVarList->end();
      TOpcProcessVarList::Iterator it_procVar = pa_lOpcProcessVarList->begin();

      while(it_procVar != itEnd_procVar && it_item != itEnd_item){

        it_procVar->setNewValue(it_item->m_oItemData);

        nrData++;

        ++it_procVar;
        ++it_item;
      }

      break;
    }

  }
  m_oSync.unlock();
  return nrData;
}

