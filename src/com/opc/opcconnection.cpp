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

COpcConnection::COpcConnection(const char *paHost, const char *paServerName, COpcEventHandler* pa_eventHandler) :
    mGroupCount(0), mConnectionEvent(e_Disconnected), mHost(paHost), mServerName(paServerName), mGroupName(0), mReqUpdateRate(0), mRealUpdateRate(0), mDeadBand(0), mIsConnected(false), mBlockingConnect(false),
   m_eventHandler(pa_eventHandler){
  mOpcConnectionImpl = new COpcConnectionImpl(paHost, paServerName, this);
}

COpcConnection::~COpcConnection(){
  delete mOpcConnectionImpl;
}

void COpcConnection::addGroup(const char* paGroupName, unsigned long paReqUpdateRate, float paDeadBand, CComLayer* paComCallback){
  mSync.lock();
  mOpcGroupMapList.pushBack(new SOpcGroupMap(paGroupName, m_eventHandler->addComCallback(paComCallback)));
  mSync.unlock();

  mOpcConnectionImpl->addGroup(paGroupName, paReqUpdateRate, paDeadBand);

  mGroupCount++;
}

void COpcConnection::removeGroup(const char* paGroupName){
  mSync.lock();
  TOpcGroupMapList::Iterator itDelete = mOpcGroupMapList.begin();
  TOpcGroupMapList::Iterator it_group = mOpcGroupMapList.begin();
  TOpcGroupMapList::Iterator itEnd_group = mOpcGroupMapList.end();

  if(it_group != itEnd_group){
    if(strcmp(it_group->mGroupName, paGroupName) == 0){
      mOpcGroupMapList.popFront();
      mGroupCount--;
      mSync.unlock();
      return;
    }
    ++it_group;
    while(it_group != itEnd_group){
      if(strcmp(it_group->mGroupName, paGroupName) == 0){
        mOpcGroupMapList.eraseAfter(itDelete);
        mGroupCount--;
        mSync.unlock();
        return;
      }

      ++itDelete;
      ++it_group;
    }
  }

  mSync.unlock();
}

int COpcConnection::send_connect(){
  switch (mConnectionEvent){
    case e_Disconnected:
      mConnectionEvent = e_Connecting;
      m_eventHandler->sendCommand(new CCmd_AddConnection(mOpcConnectionImpl));
      return 0;
    case e_Connecting:
      return 0;
    default: // all other connection states
      return 1;
  }
}

int COpcConnection::send_connect(bool paBlocking){
  mBlockingConnect = paBlocking;
  if(paBlocking && !mIsConnected){
    mOpcConnectionImpl->connect();
    return 1;
  }

  return send_connect();
}

int COpcConnection::send_addItem(COpcProcessVar* paNewItem){
  mSync.lock();
  TOpcGroupMapList::Iterator itEnd_group = mOpcGroupMapList.end();
  for(TOpcGroupMapList::Iterator it_group = mOpcGroupMapList.begin(); it_group != itEnd_group; ++it_group){
    if(strcmp(it_group->mGroupName, paNewItem->getItemGroupName()) == 0){
      if(paNewItem->getItemFunction() == COpcProcessVar::e_FBOutput){

        //check if item already added
        TItemDataList::Iterator itEnd_item = (*it_group)->mReadItemsList.end();
        for(TItemDataList::Iterator it_item = (*it_group)->mReadItemsList.begin(); it_item != itEnd_item; ++it_item){
          if(strcmp(it_item->mItemName, paNewItem->getItemName()) == 0){
            mSync.unlock();
            return 1;
          }
        }

        it_group->mReadItemsList.pushBack(new SOpcItemData(paNewItem->getItemName()));
        break;

      }
      else if(paNewItem->getItemFunction() == COpcProcessVar::e_FBInput){
        //check if item already added
        TItemDataList::Iterator itEnd_item = (*it_group)->mWriteItemsList.end();
        for(TItemDataList::Iterator it_item = (*it_group)->mWriteItemsList.begin(); it_item != itEnd_item; ++it_item){
          if(strcmp(it_item->mItemName, paNewItem->getItemName()) == 0){
            mSync.unlock();
            return 1;
          }
        }

        it_group->mWriteItemsList.pushBack(new SOpcItemData(paNewItem->getItemName()));
        break;

      }
    }
  }
  mSync.unlock();

  if(mConnectionEvent == e_Connected){
    m_eventHandler->sendCommand(new CCmd_AddOPCProcessVar(mOpcConnectionImpl, paNewItem));

    return 0;
  }

  return -1;
}

int COpcConnection::send_sendItemData(COpcProcessVar* paItem){
  if(paItem->getIsActive()) {
    m_eventHandler->sendCommand(new CCmd_SetProcessVarValue(paItem));
  }
  return 0;
}

void COpcConnection::response_connect(bool paConnectionState){
  mIsConnected = paConnectionState;
  if(paConnectionState) {
    mConnectionEvent = e_Connected;
  } else {
    mConnectionEvent = e_ConnectionFailed;
  }
    //mConnectionEvent = e_Disconnected;

  if(!mBlockingConnect){

    TOpcGroupMapList::Iterator itEnd = mOpcGroupMapList.end();
    for(TOpcGroupMapList::Iterator it = mOpcGroupMapList.begin(); it != itEnd; ++it){
      m_eventHandler->executeComCallback((*it)->mCallbackDesc);
    }

  }
}

void COpcConnection::response_dataReceived(const char *paGroupName, TItemDataList & paItemDataList){
  // Loop through OpcGroups
  TOpcGroupMapList::Iterator itEnd_group = mOpcGroupMapList.end();
  for(TOpcGroupMapList::Iterator it_group = mOpcGroupMapList.begin(); it_group != itEnd_group; ++it_group){

    if(strcmp(it_group->mGroupName, paGroupName) == 0){

      // Loop through OpcItems in OpcGroup
      TItemDataList::Iterator itEnd_item = (*it_group)->mReadItemsList.end();
      for(TItemDataList::Iterator it_item = (*it_group)->mReadItemsList.begin(); it_item != itEnd_item; ++it_item){

        // Loop through OpcItems in ItemDataList
        TItemDataList::Iterator itEnd_newItem = paItemDataList.end();
        TItemDataList::Iterator itErase = paItemDataList.begin();
        for(TItemDataList::Iterator it_newItem = paItemDataList.begin(); it_newItem != itEnd_newItem; ++it_newItem){

          if(strcmp(it_newItem->mItemName, it_item->mItemName) == 0){
            it_item->mItemData = it_newItem->mItemData;

            if(it_newItem == paItemDataList.begin()) {
              paItemDataList.popFront();
            } else {
              paItemDataList.eraseAfter(itErase);
            }

            break;
          }

          if(it_newItem != paItemDataList.begin()) {
            ++itErase;
          }
        }
          if(paItemDataList.isEmpty()) {
            break;
          }
      }

      // Change state
      mConnectionEvent = e_DataReceived;

      // Notify Com Layer
      m_eventHandler->executeComCallback(it_group->mCallbackDesc);
      break;
    }
  }
}

void COpcConnection::response_itemAdded(COpcProcessVar* paOpcItem){
  // Loop through OpcGroups
  mSync.lock();
  TOpcGroupMapList::Iterator itEnd_group = mOpcGroupMapList.end();
  for(TOpcGroupMapList::Iterator it_group = mOpcGroupMapList.begin(); it_group != itEnd_group; ++it_group){
    if(strcmp(it_group->mGroupName, paOpcItem->getItemGroupName()) == 0){
      // Change state
      if (paOpcItem->getIsActive()) {
        mConnectionEvent = e_ItemAddedOk;
      } else {
        mConnectionEvent = e_ItemAddedFailed;
      }
      // Notify Com Layer
      m_eventHandler->executeComCallback(it_group->mCallbackDesc);
      break;
    }
  }
  mSync.unlock();
}

int COpcConnection::receiveData(const char* paGroupName, TOpcProcessVarList * paOpcProcessVarList){
  int nrData = 0;
  // TODO Case when lists do not match in size

  mSync.lock();
  TOpcGroupMapList::Iterator itEnd_group = mOpcGroupMapList.end();
  for(TOpcGroupMapList::Iterator it_group = mOpcGroupMapList.begin(); it_group != itEnd_group; ++it_group){

    if(strcmp(it_group->mGroupName, paGroupName) == 0){

      TItemDataList::Iterator itEnd_item = it_group->mReadItemsList.end();
      TItemDataList::Iterator it_item = it_group->mReadItemsList.begin();

      TOpcProcessVarList::Iterator itEnd_procVar = paOpcProcessVarList->end();
      TOpcProcessVarList::Iterator it_procVar = paOpcProcessVarList->begin();

      while(it_procVar != itEnd_procVar && it_item != itEnd_item){

        it_procVar->setNewValue(it_item->mItemData);

        nrData++;

        ++it_procVar;
        ++it_item;
      }

      break;
    }

  }
  mSync.unlock();
  return nrData;
}

