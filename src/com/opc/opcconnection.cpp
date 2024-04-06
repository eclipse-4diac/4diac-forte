/*******************************************************************************
 * Copyright (c) 2012, 2024 AIT, fortiss GmbH, Hit robot group, Samator Indo Gas
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Filip Andren, Alois Zoitl - initial API and implementation and/or initial documentation
 *   ys guo - Fix opc module compilation errors and deadlock bug
 *   Tibalt Zhao - Guard the opc connection from requests from FBs,filter non-sense requests 
 *   Ketut Kumajaya - switch to OPCClientToolKit with 64bit support
 *                  - Code refactoring from char* to std::string
 *******************************************************************************/
#include "opcconnection.h"
#include "opcconnectionimpl.h"
#include "opceventhandler.h"
#include "opcprocessvar.h"
#include "Cmd_AddConnection.h"
#include "Cmd_RemoveConnection.h"
#include "Cmd_SetProcessVarValue.h"

using namespace forte::com_infra;

COpcConnection::COpcConnection(const std::string& paHost, const std::string& paServerName, COpcEventHandler* paEventHandler) :
    mGroupCount(0), mConnectionState(e_Disconnected), mHost(paHost), mServerName(paServerName),
    mConnected(false), mBlockingConnect(false), mEventHandler(paEventHandler){
  mOpcConnectionImpl = new COpcConnectionImpl(paHost, paServerName, this);
  mLastEvent.mLastHappenedEvent = e_Idle;
  mLastEvent.mSuccess = true;
}

COpcConnection::~COpcConnection(){
  delete mOpcConnectionImpl;
}

void COpcConnection::addGroup(const std::string& paGroupName, unsigned long paReqUpdateRate, float paDeadBand, CComLayer* paComCallback){
  DEVLOG_INFO("Add group&opc layer added [0x%lX][%s]\n",paComCallback,paGroupName.c_str());
  if(ifInGroupList(paGroupName)){
    DEVLOG_INFO("group [%s] already in list\n", paGroupName.c_str());
    DEVLOG_INFO("group map size is %d\n", mOpcGroupMapList.size());
    return;
  }
  CCriticalRegion criticalRegion(mSync);
  mOpcGroupMapList.push_back(new SOpcGroupMap(paGroupName, mEventHandler->addComCallback(paComCallback)));
  DEVLOG_INFO("group map size is %d\n", mOpcGroupMapList.size());
}

int COpcConnection::removeGroupCallbackDesc(const std::string& paGroupName){
  for(size_t i = 0; i < mOpcGroupMapList.size(); i++){
    if(mOpcGroupMapList[i]->mGroupName == paGroupName){
      mEventHandler->removeComCallback(mOpcGroupMapList[i]->mCallbackDesc);
      DEVLOG_INFO("erase from m_lOpcGroupMapList %d[%s]\n", i,paGroupName.c_str());
      delete mOpcGroupMapList[i];
      mOpcGroupMapList.erase(mOpcGroupMapList.begin()+i);
      return 0;
    }
  }
  return -1;
};

void COpcConnection::removeGroup(const std::string& paGroupName){
  DEVLOG_INFO("COpcConnection::removeGroup[%s]\n", paGroupName.c_str());
  CCriticalRegion criticalRegion(mSync);
  if(paGroupName.empty()){
    DEVLOG_INFO("the group list is cleared![%s]\n", paGroupName.c_str());
    for(unsigned int i = 0; i < mOpcGroupMapList.size(); i++){
      removeGroupCallbackDesc(mOpcGroupMapList[i]->mGroupName);
    }
  } else{
    removeGroupCallbackDesc(paGroupName);
  }
}

bool COpcConnection::ifLetEventPass(EOpcConnectionEvents paEvent, const std::string& paGroupName){

  switch(mConnectionState){
    case e_Disconnected:
      return (e_Connect == paEvent);

    case e_Connecting:
      return (e_Connect == paEvent && !ifInGroupList(paGroupName));

    case e_Connected:
      if(e_Connect == paEvent){
        return true;
      }
      if(e_DataReceive == paEvent && ifInGroupList(paGroupName)){
        return true;
      }
      if(e_Disconnect == paEvent && ifInGroupList(paGroupName) && mOpcGroupMapList.size() == 1 ||
          e_Disconnect == paEvent && mLastEvent.mLastHappenedEvent == e_ItemSend && !mLastEvent.mSuccess){
        return true;
      }
      if(e_ItemSend == paEvent && ifInGroupList(paGroupName)){//and paGroupName is in m_lOpcGroupMapList
        return true;
      }
      break;

    case e_Disconnecting:
      return (e_Disconnect == paEvent);
  }
  return false;
}

bool COpcConnection::ifInGroupList(const std::string& paGroupName){
  CCriticalRegion criticalRegion(mSync);
  for(auto it = mOpcGroupMapList.begin(); it != mOpcGroupMapList.end(); it++) {
    if(paGroupName == (*it)->mGroupName) {
      return true;
    }
  }
  return false;
}

int COpcConnection::send_connect(const std::string& paGroupName, unsigned long paReqUpdateRate, float paDeadBand,
    forte::com_infra::CComLayer* paComCallback, std::vector<COpcProcessVar*> paNewItems){
  if(ifLetEventPass(e_Connect,paGroupName)){
    mConnectionState = e_Connecting;
    addGroup(paGroupName, paReqUpdateRate, paDeadBand, paComCallback);
    for(size_t i = 0; i < paNewItems.size(); i++) {
      maintainGroupMapInfo(paNewItems[i]);
    }
    mEventHandler->sendCommand(new CCmd_AddConnection(mOpcConnectionImpl, paGroupName, paReqUpdateRate, paDeadBand, paNewItems));
  } 
  else{
    DEVLOG_INFO("Connection cmd cannot be sent,state=%d[%s]\n",mConnectionState,paGroupName.c_str());
  }
  return 0;
}

int COpcConnection::send_disconnect(const std::string& paGroupName){
  if(ifLetEventPass(e_Disconnect, paGroupName)){
    DEVLOG_INFO("send disconnect cmd, remove group %s\n", paGroupName.c_str());
    mConnected = false;
    removeGroup(paGroupName);
    mConnectionState = e_Disconnected;
    mEventHandler->sendCommand(new CCmd_RemoveConnection(*mOpcConnectionImpl, paGroupName, true));
    return 0;
  }
  //remove  this group but not affect the connection as there are other fb still using it
  DEVLOG_INFO("disconnect cmd cannot pass, there are %d groups left, connnection state=%d[%s]\n",
                mOpcGroupMapList.size(),mConnectionState, paGroupName.c_str());
  DEVLOG_INFO("remove group %s\n", paGroupName.c_str());
  removeGroup(paGroupName);
  mEventHandler->sendCommand(new CCmd_RemoveConnection(*mOpcConnectionImpl, paGroupName, false));
  return -1 ;
}

int COpcConnection::maintainGroupMapInfo(COpcProcessVar* paNewItem){
  CCriticalRegion criticalRegion(mSync);
  for(auto group = mOpcGroupMapList.begin(); group != mOpcGroupMapList.end(); ++group){
    if((*group)->mGroupName == paNewItem->getItemGroupName()){
      if(paNewItem->getItemFunction() == COpcProcessVar::e_FBOutput){
        //check if item already added
        for(auto item = (*group)->mReadItemsList.begin(); item != (*group)->mReadItemsList.end(); ++item){
          if((*item)->mItemName.compare(paNewItem->getItemName()) == 0){
            return 1;
          }
        }
        (*group)->mReadItemsList.push_back(new SOpcItemData(paNewItem->getItemName()));
        break;
      }
      else if(paNewItem->getItemFunction() == COpcProcessVar::e_FBInput){
        //check if item already added
        for(auto item = (*group)->mWriteItemsList.begin(); item != (*group)->mWriteItemsList.end(); ++item){
          if((*item)->mItemName.compare(paNewItem->getItemName()) == 0){
            return 1;
          }
        }
        (*group)->mWriteItemsList.push_back(new SOpcItemData(paNewItem->getItemName()));
        break;
      }
    }
  }
  return 0;
}

int COpcConnection::send_sendItemData(COpcProcessVar* paItem){
  if(!ifLetEventPass(e_ItemSend,paItem->getItemGroupName())){
    DEVLOG_INFO("Item data cannot be sent,state=%d[%s]\n",mConnectionState, paItem->getItemGroupName().c_str());
    return -1;
  }

  mEventHandler->sendCommand(new CCmd_SetProcessVarValue(mOpcConnectionImpl, paItem->getItemGroupName(),
    paItem->getItemName(),paItem->updateValue()));
  return 0;
}

void COpcConnection::response_connect(bool paConnectionState,const std::string& paGroupName){
  maintainStateinEventResponse(e_Connect,paConnectionState);
  mConnected = paConnectionState;

  if(mOpcGroupMapList.empty()){
    this->mOpcConnectionImpl->disconnect();
    return;
  }

  if(!mBlockingConnect && paConnectionState){
    for(TOpcGroupMapList::iterator it = mOpcGroupMapList.begin(); 
        it != mOpcGroupMapList.end(); ++it){
      if((*it)->mGroupName == paGroupName){
        mEventHandler->executeComCallback((*it)->mCallbackDesc);
        break;
      }
    }
  }
}

void COpcConnection::response_dataReceived(const std::string& paGroupName, TItemDataList & paItemDataList){
  maintainStateinEventResponse(e_DataReceive,true);
  // Loop through OpcGroups
  for(auto group = mOpcGroupMapList.begin(); group != mOpcGroupMapList.end(); ++group){
    if((*group)->mGroupName == paGroupName){
      // Loop through OpcItems in OpcGroup
      for(auto item = (*group)->mReadItemsList.begin(); item != (*group)->mReadItemsList.end(); ++item){
        // Loop through OpcItems in ItemDataList
        for(auto newItem = paItemDataList.begin(); newItem != paItemDataList.end(); ++newItem){
          if((*newItem)->mItemName.compare((*item)->mItemName) == 0){
            (*item)->mItemData = (*newItem)->mItemData;
            paItemDataList.erase(newItem);
            break;
          }
        }
      }
      // Notify Com Layer
      mEventHandler->executeComCallback((*group)->mCallbackDesc);
      break;
    }
  }
  // Should not happen unless something is manipulating the contents of the string
  if(!paItemDataList.empty()){
    int count = 0;
    for(size_t i = 0; i < paItemDataList.size(); i++){
      DEVLOG_INFO("deleting item from onchange itemlist[%s:%s]\n",paGroupName.c_str(),paItemDataList[i]->mItemName.c_str());
      delete paItemDataList[i];
      count++;
    }
    paItemDataList.clear();
    DEVLOG_ERROR("there are %d items cleared from onchange itemlist[%s]\n",count,paGroupName.c_str());
  }
}

void COpcConnection::response_dataSent(const std::string& paGroupName,const std::string& paItemName, bool paSuccess){
  // Loop through OpcGroups
  CCriticalRegion criticalRegion(mSync);
  TOpcGroupMapList::iterator itEnd_group = mOpcGroupMapList.end();
  for(TOpcGroupMapList::iterator it_group = mOpcGroupMapList.begin(); it_group != itEnd_group; ++it_group){
    //if(strcmp((*it_group)->mGroupName, pa_pOpcItem->getItemGroupName()) == 0){
    if((*it_group)->mGroupName == paGroupName){
      // Change state
      if (paSuccess) {
        DEVLOG_INFO("COpcConnection::response_dataSent():data sent OK: %s[%s]\n", paItemName.c_str(),paGroupName.c_str());
        maintainStateinEventResponse(e_ItemSend,true);
      } else {
        DEVLOG_INFO("COpcConnection::response_dataSent():data sent failed: %s[%s]\n", paItemName.c_str(),paGroupName.c_str());
        maintainStateinEventResponse(e_ItemSend,false);
      }
      // Notify Com Layer
      mEventHandler->executeComCallback((*it_group)->mCallbackDesc);
      break;
    }
  }
}

int COpcConnection::receiveData(const std::string& paGroupName, TOpcProcessVarList * paOpcProcessVarList){
  int rData = 0;
  // TODO Case when lists do not match in size

  CCriticalRegion criticalRegion(mSync);
  TOpcGroupMapList::iterator itEnd_group = mOpcGroupMapList.end();
  for(TOpcGroupMapList::iterator it_group = mOpcGroupMapList.begin(); it_group != itEnd_group; ++it_group){

    if((*it_group)->mGroupName == paGroupName){
      TItemDataList::iterator itEnd_item = (*it_group)->mReadItemsList.end();
      TItemDataList::iterator it_item = (*it_group)->mReadItemsList.begin();

      TOpcProcessVarList::iterator itEnd_procVar = paOpcProcessVarList->end();
      TOpcProcessVarList::iterator it_procVar = paOpcProcessVarList->begin();

      while(it_procVar != itEnd_procVar && it_item != itEnd_item){
        (*it_procVar)->setNewValue((*it_item)->mItemData);
        rData++;
        ++it_procVar;
        ++it_item;
      }

      break;
    }

  }
  return rData;
}
