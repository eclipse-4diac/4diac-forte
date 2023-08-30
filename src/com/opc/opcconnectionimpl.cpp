/*******************************************************************************
 * Copyright (c) 2012 AIT, ACIN
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Filip Andren, Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "opcconnectionimpl.h"

#include "opceventhandler.h"
#include "opcconnection.h"
#include "opcprocessvar.h"

#include "OPCClient.h"
#include "OPCHost.h"
#include "OPCServer.h"
#include "OPCGroup.h"
#include "OPCItem.h"

COpcConnectionImpl::COpcConnectionImpl(const char *paHost, const char *paServerName, COpcConnection* paOpcConn) :
  mOpcConn(paOpcConn), mOpcHost(0), mOpcServer(0), mHost(paHost), mServerName(paServerName), mGroupName(0), mReqUpdateRate(0), mRealUpdateRate(0), mDeadBand(0) {
}

COpcConnectionImpl::~COpcConnectionImpl() = default;

void COpcConnectionImpl::connect(){
  try{

    COPCClient::init();

    mOpcHost = COPCClient::makeHost(mHost);

    mOpcServer = mOpcHost->connectDAServer(mServerName);
  } catch (OPCException &e){
    mOpcConn->response_connect(false);
    return;
  }

  mOpcConn->response_connect(true);
}

void COpcConnectionImpl::addItem(COpcProcessVar* paNewItem){
  COPCGroup* itemGroup = getOpcGroup(paNewItem->getItemGroupName(), paNewItem->getItemFunction());

  ATL::CString itemName(paNewItem->getItemName());
  try{
    COPCItem* newItem = itemGroup->addItem(itemName, true);
    paNewItem->setOpcItem(newItem);
    paNewItem->setIsActive(true);
  } catch (OPCException &e){
    paNewItem->setIsActive(false);
  }
  mOpcConn->response_itemAdded(paNewItem);
}

void COpcConnectionImpl::addGroup(const char* paGroupName, unsigned long paReqUpdateRate, float paDeadBand){
  mOpcGroupSettingsList.pushBack(new SOpcGroupSettings(paGroupName, paReqUpdateRate, paDeadBand));
}

int COpcConnectionImpl::sendItemData(COpcProcessVar* paNewItem){
  return 0;
}

void COpcConnectionImpl::OnDataChange(COPCGroup & group, CAtlMap<COPCItem *, OPCItemData *> & changes){

  TItemDataList itemList;
  for(POSITION pos = changes.GetStartPosition(); pos != nullptr;){
    OPCItemData *itemData = changes.GetValueAt(pos);
    COPCItem *item = changes.GetNextKey(pos);

    itemList.pushBack(new SOpcItemData((LPCTSTR) (item->getName()), (Variant) itemData->vDataValue));
  }

  const char *c_groupName = (const char*) group.getName();

  int position = 0;
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

COPCGroup* COpcConnectionImpl::getOpcGroup(const char* paGroupName, COpcProcessVar::EOpcProcessVarFunctions paFunction){
  COPCGroup *retGroup = nullptr;

  TOpcGroupSettingsList::Iterator itEnd = mOpcGroupSettingsList.end();
  for(TOpcGroupSettingsList::Iterator it = mOpcGroupSettingsList.begin(); it != itEnd; ++it){
    if(strcmp(it->mGroupName, paGroupName) == 0){
      if(paFunction == COpcProcessVar::e_FBOutput){
        if(!(it->mReadGroupAdded)){
          char *groupName = (char*) malloc(strlen(paGroupName) + 5 + 1);
          strcpy(groupName, paGroupName);
          strcat(groupName, "_read");
          try{
            it->mOpcGroupRead = retGroup = mOpcServer->makeGroup(groupName, true, it->mReqUpdateRate, it->mRevisedUpdateRate, it->mDeadBand);
            it->mOpcGroupRead->enableAsynch(*this);
            it->mReadGroupAdded = true;
          } catch (OPCException &e){
            // TODO
          }
          free(groupName);
        } else {
          retGroup = it->mOpcGroupRead;
        }
      }
      else if(paFunction == COpcProcessVar::e_FBInput){
        if(!(it->mWriteGroupAdded)){
          char *groupName = (char*) malloc(strlen(paGroupName) + 6 + 1);
          strcpy(groupName, paGroupName);
          strcat(groupName, "_write");
          try{
            it->mOpcGroupWrite = retGroup = mOpcServer->makeGroup(groupName, true, it->mReqUpdateRate, it->mRevisedUpdateRate, it->mDeadBand);
            it->mWriteGroupAdded = true;
          } catch (OPCException &e){
            // TODO
          }
          free(groupName);
        } else {
          retGroup = it->mOpcGroupWrite;
        }
      }

      break;
    }
  }

  return retGroup;
}
