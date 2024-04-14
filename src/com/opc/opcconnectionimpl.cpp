/*******************************************************************************
 * Copyright (c) 2012, 2024 AIT, ACIN, HIT robot group, Samator Indo Gas
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Filip Andren, Alois Zoitl - initial API and implementation and/or initial documentation
 *   Tibalt Zhao - add the list of items instead of add item one by one
 *   Ketut Kumajaya - switch to OPCClientToolKit with 64bit support
 *                  - Code refactoring from char* to std::string
 *******************************************************************************/
#include "opcconnectionimpl.h"
#include "../../arch/devlog.h"
#include "../../arch/forte_stringFunctions.h"

#include "opceventhandler.h"
#include "opcconnection.h"

#include "OPCClient.h"
#include "OPCHost.h"
#include "OPCServer.h"
#include "OPCGroup.h"
#include "OPCItem.h"

#define S2WS(x) forte_stringToWstring(x)
#define WS2S(x) forte_wstringToString(x)

using namespace std::string_literals;

COpcConnectionImpl::COpcConnectionImpl(const std::string& paHost, const std::string& paServerName, COpcConnection* paOpcConn) :
    mOpcConn(paOpcConn), mOpcHost(0), mOpcServer(0),mConnected(0), mHost(paHost), mServerName(paServerName) {
}

COpcConnectionImpl::~COpcConnectionImpl(){
  DEVLOG_DEBUG("COpcConnectionImpl ~COpcConnectionImpl\n");
}

bool COpcConnectionImpl::isConnected(){
  return mConnected;
}

void COpcConnectionImpl::disconnect(){//const char* paGroupName){
  DEVLOG_INFO("COpcConnectionImpl disconnect\n");
  if (isConnected())
  {
    if (mOpcHost != nullptr) {
      delete mOpcHost;
      mOpcHost = nullptr;
    }

    if (mOpcServer != nullptr) {
      delete mOpcServer;
      mOpcServer = nullptr;
    }

    COPCClient::stop();
    mConnected = false;
    DEVLOG_INFO("COpcConnectionImpl disconnected\n");
  }
}

bool COpcConnectionImpl::connect(const std::string& paGroupName){
  if(isConnected()){
    DEVLOG_DEBUG("COpcConnectionImpl::connect: already connected[%s]\n",paGroupName.c_str());
    return true;
  }

  try{
    DEVLOG_INFO("try to connect OPC server in COpcConnectionImpl[%s]\n",paGroupName.c_str());
    if (true != COPCClient::init(OPCOLEInitMode::MULTITHREADED)) {
      DEVLOG_ERROR("CoInitializeEx init failed\n");
      return false;
    }

    mOpcHost = COPCClient::makeHost(S2WS(mHost));

    mOpcServer = mOpcHost->connectDAServer(S2WS(mServerName));
  } catch (OPCException &e){
    DEVLOG_ERROR("connect OPC server failed:%s[%s]\n",WS2S(e.reasonString()).c_str(),paGroupName.c_str());
    return false;
  }
  DEVLOG_INFO("successfully connect OPC server in COpcConnectionImpl[%s]\n",paGroupName.c_str());
  mConnected = true;
  return true;
}

void COpcConnectionImpl::addItemList(const std::string& paGroupName, std::vector<std::string>& paReadItems,
    std::vector<std::string>& paWriteItems){
  //we assume all the items in pa_lNewItems are of same group
  std::string groupName;
  TItemDataList itemList;
  DEVLOG_INFO("adding items in COpcConnectionImpl[%s]\n", paGroupName.c_str());
  size_t readSize = paReadItems.size();
  //now paReadItems contains read and write items
  paReadItems.insert(paReadItems.end(), paWriteItems.begin(), paWriteItems.end());
  for(size_t i = 0; i < paReadItems.size(); i++){
    groupName = paGroupName;
    COPCGroup *itemGroup = nullptr;
    if(i + 1 > readSize){
      itemGroup = getOpcGroup(groupName, false);
    }
    else{
      itemGroup = getOpcGroup(groupName, true);
    }
    if (itemGroup == nullptr){
      DEVLOG_INFO("there is no group for this item:%s [%s]\n", paReadItems[i].c_str(), groupName.c_str());
      //pa_pNewItem->setIsActive(false);
      this->disconnect();
      mConnected = false;
      mOpcConn->response_connect(false, groupName);
      return;
    }

    DEVLOG_DEBUG("adding item %s in COpcConnectionImpl[%s]\n", paReadItems[i].c_str(), WS2S(itemGroup->getName()).c_str());
    std::wstring itemName(S2WS(paReadItems[i]));
    try
    {
      COPCItem *newItem = itemGroup->addItem(itemName, true);
      //pa_pNewItem->setIsActive(true);
      mOpcItems[WS2S(itemGroup->getName())].push_back(newItem);
    }
    catch (OPCException &e)
    {
      DEVLOG_ERROR("addItem failed with exception:%s[%s:%s]\n", WS2S(e.reasonString()).c_str(),
                    groupName.c_str(),paReadItems[i].c_str());
      if(e.reasonString().compare(L"COPCGroup::addItem: FAILED to add item") != 0){
        //pa_pNewItem->setIsActive(false);
        this->disconnect();
        mConnected = false;
        mOpcConn->response_connect(false, groupName);
        return;
      }
      else{
        DEVLOG_ERROR("Check the opc item name: %s, probably it is wrong![%s]\n",
                   paReadItems[i].c_str(),groupName.c_str());
        itemList.push_back(new SOpcItemData(paReadItems[i], (Variant)NAN ));
      }
    }
  }
  if (!paReadItems.empty()){
    mOpcConn->response_connect(true, groupName);
  }

  if(!itemList.empty()){
    mOpcConn->response_dataReceived(groupName,itemList);
  }
}

bool COpcConnectionImpl::addGroup(const std::string& paGroupName, unsigned long paReqUpdateRate, float paDeadBand){
  if(paGroupName.empty()){
   DEVLOG_INFO("COpcConnectionImpl::addGroup: group name is empty\n");
   return false;
  }
  DEVLOG_INFO("addGroup in COpcConnectionImpl[%s]\n", paGroupName.c_str());

  for(auto var : mOpcGroupSettingsList){
    if(var->mGroupName == paGroupName){
      DEVLOG_INFO("COpcConnectionImpl::addGroup: %s is already added\n", paGroupName.c_str());
      return false;
    }
  }
  mOpcGroupSettingsList.push_back(new SOpcGroupSettings(paGroupName, paReqUpdateRate, paDeadBand));
  return true;
}

void COpcConnectionImpl::removeItems(const std::string& paGroupName){
  if(paGroupName.empty()){
    DEVLOG_INFO("COpcConnectionImpl::removeItems: group name is empty\n");
    return;
  }
  DEVLOG_INFO("removing items in COpcConnectionImpl[%s]\n", paGroupName.c_str());
  std::vector<COPCItem *> items;

  TOpcItemsIt it = mOpcItems.find(paGroupName);
  if(it != mOpcItems.end()){
    items = it->second;
    for(size_t i = 0; i < items.size(); i++){
      DEVLOG_DEBUG("removing item %s in COpcConnectionImpl[%s]\n", WS2S(items[i]->getName()).c_str(), paGroupName.c_str());
      if (nullptr != items[i]) {
        delete items[i];
        items[i] = nullptr;
      }
    }
    items.clear();
    mOpcItems.erase(it);
  }
  else{
    DEVLOG_ERROR("there is no item in group:%s\n", paGroupName.c_str());
  }
}

void COpcConnectionImpl::removeGroup(const std::string& paGroupName){
  if(paGroupName.empty()){
    DEVLOG_INFO("empty name is passed to removeGroup,clear the group\n");
  }
  else{
    DEVLOG_INFO("removeGroup in COpcConnectionImpl[%s]\n",paGroupName.c_str());
  }
  for(auto group = mOpcGroupSettingsList.begin(); group != mOpcGroupSettingsList.end();){
    if(paGroupName.empty() || (*group)->mGroupName == paGroupName){
      if (nullptr != (*group)->mOpcGroupRead && (*group)->mReadGroupAdded) {
        (*group)->mOpcGroupRead->disableAsync();
        removeItems(WS2S((*group)->mOpcGroupRead->getName()));
        delete (*group)->mOpcGroupRead;
        (*group)->mOpcGroupRead = nullptr;
      }
      if (nullptr != (*group)->mOpcGroupWrite && (*group)->mWriteGroupAdded) {
        removeItems(WS2S((*group)->mOpcGroupWrite->getName()));
        delete (*group)->mOpcGroupWrite;
        (*group)->mOpcGroupWrite = nullptr;
      }
      group =  mOpcGroupSettingsList.erase(group);
      if(paGroupName.empty()){
        continue;
      }
      else{
        break;
      }
    }
    else{
      group++;
    }
  }
  return;
}

void COpcConnectionImpl::clearGroup(){
  DEVLOG_INFO("clearGroup in COpcConnectionImpl\n");
  mOpcItems.clear();
  removeGroup(std::string());
}

int COpcConnectionImpl::sendItemData(const std::string& paGroupName, const std::string& paItemName, Variant paVar){
  if(paGroupName.empty() || paItemName.empty()) {
    DEVLOG_ERROR("nullptr is passed to COpcConnectionImpl::sendItemData\n");
    return -1;
  }
  int rtn = -1;
  DEVLOG_INFO("sendItemData in COpcConnectionImpl[%s:%s]\n",paGroupName.c_str(),paItemName.c_str());
  std::vector<COPCItem *> items;
  const std::string writeGrpName(paGroupName + "_write"s);
  TOpcItemsIt it = mOpcItems.find(writeGrpName);
  if(it != mOpcItems.end()){
    items = it->second;
    for(size_t i = 0; i < items.size(); i++){
      if(WS2S(items[i]->getName()) == paItemName){
        try{
          items[i]->writeSync(paVar);
        }
        catch (OPCException &e){
          DEVLOG_ERROR("opcitem writesync failed with exception:%s[%s:%s]\n", WS2S(e.reasonString()).c_str(), writeGrpName.c_str(), paItemName.c_str());
          rtn = -1;
          break;
        }
        rtn = 0;
        break;
      }
    }
  }
  else{
    DEVLOG_ERROR("there is no item in group:%s\n", writeGrpName.c_str());
    rtn = -1;
  }
  mOpcConn->response_dataSent(paGroupName, paItemName, rtn == 0 ? true : false);
  return 0;
}

void COpcConnectionImpl::OnDataChange(COPCGroup & paGroup, COPCItemDataMap & paChanges){
  TItemDataList itemList;
  POSITION pos = paChanges.GetStartPosition();
  while(pos){
    OPCItemData *data = paChanges.GetNextValue(pos);
    if (data) {
      itemList.push_back(new SOpcItemData(WS2S(data->item()->getName()), (Variant) data->vDataValue));
    }
  }

  std::string groupName(WS2S(paGroup.getName()));
  size_t position = groupName.rfind("_read");
  if (position != std::string::npos) {
    groupName.resize(position);
    mOpcConn->response_dataReceived(groupName, itemList);
  }
}

COPCGroup* COpcConnectionImpl::getOpcGroup(const std::string& paGroupName, bool paIfRead){
  COPCGroup *retGroup = nullptr;

  TOpcGroupSettingsList::iterator itEnd = mOpcGroupSettingsList.end();
  for(TOpcGroupSettingsList::iterator it = mOpcGroupSettingsList.begin(); it != itEnd; ++it){
    if((*it)->mGroupName == paGroupName){
      if(paIfRead){
        if(!((*it)->mReadGroupAdded)){
          const std::string groupName(paGroupName + "_read"s);
          try{
            (*it)->mOpcGroupRead = retGroup = mOpcServer->makeGroup(S2WS(groupName), true, (*it)->mReqUpdateRate, (*it)->mRevisedUpdateRate, (*it)->mDeadBand);
            (*it)->mOpcGroupRead->enableAsync(this);
            (*it)->mReadGroupAdded = true;
          } catch (OPCException &e){
            // TODO
            DEVLOG_ERROR("exception in make opc group[%s]:%s\n",groupName.c_str(),WS2S(e.reasonString()).c_str());
            (*it)->mOpcGroupRead = nullptr;
            retGroup = nullptr;
          }
        } else {
          retGroup = (*it)->mOpcGroupRead;
        }
      } else{
        if(!((*it)->mWriteGroupAdded)){
          const std::string groupName(paGroupName + "_write"s);
          try{
            (*it)->mOpcGroupWrite = retGroup = mOpcServer->makeGroup(S2WS(groupName), true, (*it)->mReqUpdateRate, (*it)->mRevisedUpdateRate, (*it)->mDeadBand);
            (*it)->mWriteGroupAdded = true;
          } catch (OPCException &e){
            // TODO
            DEVLOG_ERROR("exception in make opc group[%s]:%s\n",groupName.c_str(),WS2S(e.reasonString()).c_str());
            (*it)->mOpcGroupWrite = nullptr;
            retGroup = nullptr;
          }
        } else {
          retGroup = (*it)->mOpcGroupWrite;
        }
      }

      break;
    }
  }

  return retGroup;
}
