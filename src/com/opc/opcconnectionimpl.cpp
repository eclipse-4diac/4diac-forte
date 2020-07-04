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

COpcConnectionImpl::COpcConnectionImpl(const char *pa_acHost, const char *pa_acServerName, COpcConnection* pa_pOpcConn) :
  m_pOpcConn(pa_pOpcConn), m_pOpcHost(0), m_pOpcServer(0), m_acHost(pa_acHost), m_acServerName(pa_acServerName), m_acGroupName(0), m_nReqUpdateRate(0), m_nRealUpdateRate(0), m_nDeadBand(0) {
}

COpcConnectionImpl::~COpcConnectionImpl(){
}

void COpcConnectionImpl::connect(){
  try{

    COPCClient::init();

    m_pOpcHost = COPCClient::makeHost(m_acHost);

    m_pOpcServer = m_pOpcHost->connectDAServer(m_acServerName);
  } catch (OPCException &e){
    m_pOpcConn->response_connect(false);
    return;
  }

  m_pOpcConn->response_connect(true);
}

void COpcConnectionImpl::addItem(COpcProcessVar* pa_pNewItem){
  COPCGroup* itemGroup = getOpcGroup(pa_pNewItem->getItemGroupName(), pa_pNewItem->getItemFunction());

  ATL::CString itemName(pa_pNewItem->getItemName());
  try{
    COPCItem* newItem = itemGroup->addItem(itemName, true);
    pa_pNewItem->setOpcItem(newItem);
    pa_pNewItem->setIsActive(true);
  } catch (OPCException &e){
    pa_pNewItem->setIsActive(false);
  }
  m_pOpcConn->response_itemAdded(pa_pNewItem);
}

void COpcConnectionImpl::addGroup(const char* pa_acGroupName, unsigned long pa_nReqUpdateRate, float pa_nDeadBand){
  m_lOpcGroupSettingsList.pushBack(new SOpcGroupSettings(pa_acGroupName, pa_nReqUpdateRate, pa_nDeadBand));
}

int COpcConnectionImpl::sendItemData(COpcProcessVar* pa_pNewItem){
  return 0;
}

void COpcConnectionImpl::OnDataChange(COPCGroup & group, CAtlMap<COPCItem *, OPCItemData *> & changes){

  TItemDataList itemList;
  for(POSITION pos = changes.GetStartPosition(); pos != NULL;){
    OPCItemData *itemData = changes.GetValueAt(pos);
    COPCItem *item = changes.GetNextKey(pos);

    itemList.pushBack(new SOpcItemData((LPCTSTR) (item->getName()), (Variant) itemData->vDataValue));
  }

  const char *c_groupName = (const char*) group.getName();

  int position = 0;
  const char * subStrRead = strstr(c_groupName, "_read");
  if(subStrRead != NULL) {
    position = subStrRead - c_groupName;
  }
  else{
    //TODO Should not happen error
  }

  char * groupName = (char*) malloc(position + 1);
  strncpy(groupName, c_groupName, position);
  groupName[position] = '\0';

  m_pOpcConn->response_dataReceived(groupName, itemList);

  free(groupName);
}

COPCGroup* COpcConnectionImpl::getOpcGroup(const char* pa_acGroupName, COpcProcessVar::EOpcProcessVarFunctions pa_eFunction){
  COPCGroup *retGroup = NULL;

  TOpcGroupSettingsList::Iterator itEnd = m_lOpcGroupSettingsList.end();
  for(TOpcGroupSettingsList::Iterator it = m_lOpcGroupSettingsList.begin(); it != itEnd; ++it){
    if(strcmp(it->m_acGroupName, pa_acGroupName) == 0){
      if(pa_eFunction == COpcProcessVar::e_FBOutput){
        if(!(it->m_bReadGroupAdded)){
          char *groupName = (char*) malloc(strlen(pa_acGroupName) + 5 + 1);
          strcpy(groupName, pa_acGroupName);
          strcat(groupName, "_read");
          try{
            it->m_pOpcGroupRead = retGroup = m_pOpcServer->makeGroup(groupName, true, it->m_nReqUpdateRate, it->m_nRevisedUpdateRate, it->m_nDeadBand);
            it->m_pOpcGroupRead->enableAsynch(*this);
            it->m_bReadGroupAdded = true;
          } catch (OPCException &e){
            // TODO
          }
          free(groupName);
        } else {
          retGroup = it->m_pOpcGroupRead;
        }
      }
      else if(pa_eFunction == COpcProcessVar::e_FBInput){
        if(!(it->m_bWriteGroupAdded)){
          char *groupName = (char*) malloc(strlen(pa_acGroupName) + 6 + 1);
          strcpy(groupName, pa_acGroupName);
          strcat(groupName, "_write");
          try{
            it->m_pOpcGroupWrite = retGroup = m_pOpcServer->makeGroup(groupName, true, it->m_nReqUpdateRate, it->m_nRevisedUpdateRate, it->m_nDeadBand);
            it->m_bWriteGroupAdded = true;
          } catch (OPCException &e){
            // TODO
          }
          free(groupName);
        } else {
          retGroup = it->m_pOpcGroupWrite;
        }
      }

      break;
    }
  }

  return retGroup;
}
