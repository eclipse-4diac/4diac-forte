/*******************************************************************************
 * Copyright (c) 2012, 2023 AIT, Hit robot group
 *               2024 Samator Indo Gas
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Filip Andren - initial API and implementation and/or initial documentation
 *   Tibalt Zhao  - Merge additem into opc connect
 *   Ketut Kumajaya - Code refactoring from char* to std::string
 *******************************************************************************/
#include "Cmd_AddConnection.h"
#include "opcconnectionimpl.h"
#include "opcconnection.h"
#include "opcprocessvar.h"

CCmd_AddConnection::CCmd_AddConnection(COpcConnectionImpl *paConnectionimpl, const std::string& paGroupName,
      unsigned long paReqUpdateRate, float paDeadBand, std::vector<COpcProcessVar*> paNewItems) :
      mConnection(paConnectionimpl), mGroupName(paGroupName), mReqUpdateRate(paReqUpdateRate), mDeadBand(paDeadBand) {
  for(auto opcVar : paNewItems){
    if(COpcProcessVar::e_FBInput == opcVar->getItemFunction()){
      mWriteItems.push_back(opcVar->getItemName());
    }
    else if(COpcProcessVar::e_FBOutput == opcVar->getItemFunction()){
      mReadItems.push_back(opcVar->getItemName());
    }
    else{
      DEVLOG_ERROR("COpcProcessVar::getItemFunction() returned unknown value\n");
    }
  }
}

void CCmd_AddConnection::runCommand(){
  if(mConnection->connect(mGroupName)){
    if(mConnection->addGroup(mGroupName,mReqUpdateRate,mDeadBand)){
      mConnection->addItemList(mGroupName,mReadItems, mWriteItems);
    }
  }
}
const char* CCmd_AddConnection::getCommandName() const{
  return "Cmd_AddConnection";
}
