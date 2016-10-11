/*******************************************************************************
 * Copyright (c) 2013, 2014 ACIN, fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Martin Melik-Merkumians, Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "cwin32sercomhandler.h"
#include "cwin32sercomlayer.h"
#include <criticalregion.h>
#include <devlog.h>
#include <devexec.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

DEFINE_SINGLETON(CWin32SerComHandler);

CWin32SerComHandler::CWin32SerComHandler()  {
}

CWin32SerComHandler::~CWin32SerComHandler(){
  this->end();
}

void CWin32SerComHandler::registerSerComLayer(CWin32SerComLayer *pa_poComLayer){
  m_oSync.lock();
  m_lstComLayerList.push_back(pa_poComLayer);
  m_oSync.unlock();
  if(!isAlive()){
    this->start();
  }
  mSem.semInc();
}

void CWin32SerComHandler::unregisterSerComLayer(CWin32SerComLayer *pa_poComLayer){
  CCriticalRegion region(m_oSync);

  TCWin32SerComLayerContainer::Iterator itRunner(m_lstComLayerList.begin());
  TCWin32SerComLayerContainer::Iterator itRefNode(m_lstComLayerList.end());
  TCWin32SerComLayerContainer::Iterator itEnd(m_lstComLayerList.end());

  while(itRunner != itEnd){
    if(*itRunner == pa_poComLayer){
      if(itRefNode == itEnd){
        m_lstComLayerList.pop_front();
      }
      else{
        m_lstComLayerList.eraseAfter(itRefNode);
      }
      break;
    }

    itRefNode = itRunner;
    ++itRunner;
  }
}

void CWin32SerComHandler::run(){
  while(isAlive()){

    if(true == m_lstComLayerList.isEmpty()){
      mSem.semWaitIndefinitly();
    }

    m_oSync.lock();
    TCWin32SerComLayerContainer::Iterator itEnd(m_lstComLayerList.end());
    for(TCWin32SerComLayerContainer::Iterator itRunner = m_lstComLayerList.begin(), itCurrent = m_lstComLayerList.begin(); itRunner != itEnd;){
      itCurrent = itRunner;
      ++itRunner;

      m_oSync.unlock();
      if(forte::com_infra::e_Nothing != (*itCurrent)->recvData(0,0)){
        startNewEventChain((*itCurrent)->getCommFB());
      }
      m_oSync.lock();
    }
    m_oSync.unlock();
    Sleep(1000);
  }

}
