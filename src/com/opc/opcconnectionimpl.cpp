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
 *******************************************************************************/
#include "opcconnectionimpl.h"
#include "../../arch/devlog.h"

#include "opceventhandler.h"
#include "opcconnection.h"

#include "OPCClient.h"
#include "OPCHost.h"
#include "OPCServer.h"
#include "OPCGroup.h"
#include "OPCItem.h"

#define S2WS(x) COPCHost::S2WS(x)
#define WS2S(x) COPCHost::WS2S(x)

COpcConnectionImpl::COpcConnectionImpl(const char *paHost, const char *paServerName, COpcConnection* paOpcConn) :
    mOpcConn(paOpcConn), mOpcHost(0), mOpcServer(0),mConnected(0), mHost(paHost), mServerName(paServerName),
    mGroupName(0), mReqUpdateRate(0), mRealUpdateRate(0), mDeadBand(0) {
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
  }
}

bool COpcConnectionImpl::connect(const char* paGroupName){
  if(isConnected()){
    DEVLOG_DEBUG("COpcConnectionImpl::connect: already connected[%s]\n",paGroupName);
    return true;
  }

  try{
    DEVLOG_INFO("try to connect OPC server in COpcConnectionImpl[%s]\n",paGroupName);
    if (true != COPCClient::init(OPCOLEInitMode::MULTITHREADED)) {
      DEVLOG_ERROR("CoInitializeEx init failed\n");
      return false;
    }

    mOpcHost = COPCClient::makeHost(S2WS(std::string(mHost)));

    mOpcServer = mOpcHost->connectDAServer(S2WS(std::string(mServerName)));
  } catch (OPCException &e){
    DEVLOG_ERROR("connect OPC server failed:%s[%s]\n",WS2S(e.reasonString()).c_str(),paGroupName);
    return false;
  }
  DEVLOG_INFO("successfully connect OPC server in COpcConnectionImpl[%s]\n",paGroupName);
  mConnected = true;
  return true;
}

void COpcConnectionImpl::addItemList(const char* paGroupName, std::vector<std::string> paReadItems,
    std::vector<std::string> paWriteItems){
  //we assume all the items in pa_lNewItems are of same group
  const char * groupName = nullptr;
  TItemDataList itemList;
  DEVLOG_INFO("adding items in COpcConnectionImpl[%s]\n", paGroupName);
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
      DEVLOG_INFO("there is no group for this item:%s [%s]\n", paReadItems[i].c_str(), groupName);
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
      mOpcItems[WS2S(itemGroup->getName()).c_str()].push_back(newItem);
    }
    catch (OPCException &e)
    {
      DEVLOG_ERROR("addItem failed with exception:%s[%s:%s]\n", WS2S(e.reasonString()).c_str(),
                    groupName,paReadItems[i].c_str());
      if(e.reasonString().compare(L"COPCGroup::addItem: FAILED to add item") != 0){
        //pa_pNewItem->setIsActive(false);
        this->disconnect();
        mConnected = false;
        mOpcConn->response_connect(false, groupName);
        return;
      }
      else{
        DEVLOG_ERROR("Check the opc item name: %s, probably it is wrong![%s]\n",
                   paReadItems[i].c_str(),groupName);
        itemList.push_back(new SOpcItemData(paReadItems[i].c_str(), (Variant)NAN ));
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

bool COpcConnectionImpl::addGroup(const char* paGroupName, unsigned long paReqUpdateRate, float paDeadBand){
  if(nullptr == paGroupName){
   DEVLOG_INFO("COpcConnectionImpl::addGroup: group name is nullptr\n");
   return false;
  }
  DEVLOG_INFO("addGroup in COpcConnectionImpl[%s]\n", paGroupName);

  for(auto var : mOpcGroupSettingsList){
    if(strcmp(var->mGroupName,paGroupName) == 0){
      DEVLOG_INFO("COpcConnectionImpl::addGroup: %s is already added\n", paGroupName);
      return false;
    }
  }
  mOpcGroupSettingsList.push_back(new SOpcGroupSettings(paGroupName, paReqUpdateRate, paDeadBand));
  return true;
}

void COpcConnectionImpl::removeItems(const char* paGroupName){
  if(nullptr == paGroupName){
   DEVLOG_INFO("COpcConnectionImpl::removeItems: group name is nullptr\n");
   return;
  }
  DEVLOG_INFO("removing items in COpcConnectionImpl[%s]\n", paGroupName);
  std::vector<COPCItem *> items;

  TOpcItemsIt it = mOpcItems.find(paGroupName);
  if(it != mOpcItems.end()){
    items = it->second;
    for(size_t i = 0; i < items.size(); i++){
      DEVLOG_DEBUG("removing item %s in COpcConnectionImpl[%s]\n", WS2S(items[i]->getName()).c_str(), paGroupName);
      delete items[i];
    }
    items.clear();
    mOpcItems.erase(it);
  }
  else{
    DEVLOG_ERROR("there is no item in group:%s\n", paGroupName);
  }
}

void COpcConnectionImpl::removeGroup(const char* paGroupName){
  if(paGroupName == nullptr){
    DEVLOG_INFO("nullptr is passed to removeGroup,clear the group\n");
  }
  else{
    DEVLOG_INFO("removeGroup in COpcConnectionImpl[%s]\n",paGroupName);
  }
  for(auto group = mOpcGroupSettingsList.begin(); group != mOpcGroupSettingsList.end();){
    if(nullptr == paGroupName || 0 == strcmp((*group)->mGroupName, paGroupName)){
      removeItems(WS2S((*group)->mOpcGroupRead->getName()).c_str());
      removeItems(WS2S((*group)->mOpcGroupWrite->getName()).c_str());
      delete (*group)->mOpcGroupRead;
      delete (*group)->mOpcGroupWrite;
      group =  mOpcGroupSettingsList.erase(group);
      if(nullptr == paGroupName){
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
  removeGroup(0);
}

int COpcConnectionImpl::sendItemData(const char *paGroupName, const char *paItemName, Variant paVar){
  if(paGroupName == nullptr || paItemName == nullptr) {
    DEVLOG_ERROR("nullptr is passed to COpcConnectionImpl::sendItemData\n");
    return -1;
  }
  int rtn = -1;
  DEVLOG_INFO("sendItemData in COpcConnectionImpl[%s:%s]\n",paGroupName,paItemName);
  std::vector<COPCItem *> items;

  char *writeGrpName= (char*) malloc(strlen(paGroupName) + 6 + 1);
  strcpy(writeGrpName, paGroupName);
  strcat(writeGrpName, "_write");

  TOpcItemsIt it = mOpcItems.find(writeGrpName);
  if(it != mOpcItems.end()){
    items = it->second;
    for(size_t i = 0; i < items.size(); i++){
      if(WS2S(items[i]->getName()) == paItemName){
        try{
          items[i]->writeSync(paVar);
        }
        catch (OPCException &e){
          DEVLOG_ERROR("opcitem writesync failed with exception:%s[%s:%s]\n", WS2S(e.reasonString()).c_str(), writeGrpName, paItemName);
          rtn = -1;
          break;
        }
        rtn = 0;
        break;
      }
    }
  }
  else{
    DEVLOG_ERROR("there is no item in group:%s\n", writeGrpName);
    rtn = -1;
  }
  mOpcConn->response_dataSent(paGroupName, paItemName, rtn == 0 ? true : false);
  free(writeGrpName);
  return 0;
}

void COpcConnectionImpl::OnDataChange(COPCGroup & paGroup, COPCItemDataMap & paChanges){
  TItemDataList itemList;
  POSITION pos = paChanges.GetStartPosition();
  while(pos){
    OPCItemData *data = paChanges.GetNextValue(pos);
    if (data) {
      itemList.push_back(new SOpcItemData(WS2S(data->item()->getName()).c_str(), (Variant) data->vDataValue));
    }
  }

  // Possible unpredictable string behavior in a multi-threaded program
  const std::string s_groupName(WS2S(paGroup.getName()));
  const char *c_groupName = s_groupName.c_str();

  long long position = 0;
  const char * subStrRead = strstr(c_groupName, "_read");
  if(subStrRead != nullptr) {
    position = subStrRead - c_groupName;
  }
  else{
    //TODO Should not happen error
  }

  char * groupName = (char*) malloc(position + 1);
  strncpy(groupName, c_groupName, position);
  groupName[position] = '\0';

  mOpcConn->response_dataReceived(groupName, itemList);

  free(groupName);
}

COPCGroup* COpcConnectionImpl::getOpcGroup(const char* paGroupName, bool paIfRead){
  COPCGroup *retGroup = nullptr;

  TOpcGroupSettingsList::iterator itEnd = mOpcGroupSettingsList.end();
  for(TOpcGroupSettingsList::iterator it = mOpcGroupSettingsList.begin(); it != itEnd; ++it){
    if(strcmp((*it)->mGroupName, paGroupName) == 0){
      if(paIfRead){
        if(!((*it)->mReadGroupAdded)){
          char *groupName = (char*) malloc(strlen(paGroupName) + 5 + 1);
          strcpy(groupName, paGroupName);
          strcat(groupName, "_read");
          try{
            (*it)->mOpcGroupRead = retGroup = mOpcServer->makeGroup(S2WS(std::string(groupName)), true, (*it)->mReqUpdateRate, (*it)->mRevisedUpdateRate, (*it)->mDeadBand);
            (*it)->mOpcGroupRead->enableAsync(this);
            (*it)->mReadGroupAdded = true;
          } catch (OPCException &e){
            // TODO
            DEVLOG_ERROR("exception in make opc group[%s]:%s\n",groupName,WS2S(e.reasonString()).c_str());
            (*it)->mOpcGroupRead = nullptr;
            retGroup = nullptr;
          }
          free(groupName);
        } else {
          retGroup = (*it)->mOpcGroupRead;
        }
      } else{
        if(!((*it)->mWriteGroupAdded)){
          char *groupName = (char*) malloc(strlen(paGroupName) + 6 + 1);
          strcpy(groupName, paGroupName);
          strcat(groupName, "_write");
          try{
            (*it)->mOpcGroupWrite = retGroup = mOpcServer->makeGroup(S2WS(std::string(groupName)), true, (*it)->mReqUpdateRate, (*it)->mRevisedUpdateRate, (*it)->mDeadBand);
            (*it)->mWriteGroupAdded = true;
          } catch (OPCException &e){
            // TODO
            DEVLOG_ERROR("exception in make opc group[%s]:%s\n",groupName,WS2S(e.reasonString()).c_str());
            (*it)->mOpcGroupWrite = nullptr;
            (*it)->mOpcGroupWrite = nullptr;
            retGroup = nullptr;
          }
          free(groupName);
        } else {
          retGroup = (*it)->mOpcGroupWrite;
        }
      }

      break;
    }
  }

  return retGroup;
}
