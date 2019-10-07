/*******************************************************************************
 * Copyright (c) 2013, 2014 ACIN, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Martin Melik-Merkumians, Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "cwin32sercomhandler.h"
#include "cwin32sercomlayer.h"
#include "../../../core/utils/criticalregion.h"
#include "../../../core/cominfra/basecommfb.h"


DEFINE_HANDLER(CWin32SerComHandler)

CWin32SerComHandler::CWin32SerComHandler(CDeviceExecution& paDeviceExecution) : CExternalEventHandler(paDeviceExecution)  {
}

CWin32SerComHandler::~CWin32SerComHandler(){
  this->end();
}

void CWin32SerComHandler::registerSerComLayer(CWin32SerComLayer *paComLayer){
  {
    CCriticalRegion region(mSync);
    mComLayerList.pushBack(paComLayer);
  }
  if(!isAlive()){
    this->start();
  }
  mSem.inc();
}

void CWin32SerComHandler::unregisterSerComLayer(CWin32SerComLayer *paComLayer){
  CCriticalRegion region(mSync);

  TCWin32SerComLayerContainer::Iterator itRunner(mComLayerList.begin());
  TCWin32SerComLayerContainer::Iterator itRefNode(mComLayerList.end());
  TCWin32SerComLayerContainer::Iterator itEnd(mComLayerList.end());

  while(itRunner != itEnd){
    if(*itRunner == paComLayer){
      if(itRefNode == itEnd){
        mComLayerList.popFront();
      }
      else{
        mComLayerList.eraseAfter(itRefNode);
      }
      break;
    }

    itRefNode = itRunner;
    ++itRunner;
  }
}

void CWin32SerComHandler::run(){
  while(isAlive()){

    if(true == mComLayerList.isEmpty()){
      mSem.waitIndefinitely();
    }
    if(!isAlive()){
      break;
    }

    mSync.lock();
    TCWin32SerComLayerContainer::Iterator itEnd(mComLayerList.end());
    for(TCWin32SerComLayerContainer::Iterator itRunner = mComLayerList.begin(), itCurrent = mComLayerList.begin(); itRunner != itEnd;){
      itCurrent = itRunner;
      ++itRunner;

      if(forte::com_infra::e_Nothing != (*itCurrent)->recvData(0,0)){
        startNewEventChain((*itCurrent)->getCommFB());
      }
    }
    mSync.unlock();
    Sleep(1000);
  }

}
