/*******************************************************************************
 * Copyright (c) 2012 - 2014 AIT, fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Filip Andren, Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "opceventhandler.h"
#include "../core/devexec.h"
#include "devlog.h"
#include <commfb.h>

#include <ObjBase.h>

DEFINE_SINGLETON(COpcEventHandler);

COpcEventHandler::TCallbackDescriptor COpcEventHandler::m_nCallbackDescCount = 0;

COpcEventHandler::COpcEventHandler(){
  this->start();
  // Sleep to allow new thread to start
  CThread::sleep(100);
}

COpcEventHandler::~COpcEventHandler(){
  this->end();
}

void COpcEventHandler::sendCommand(ICmd *pa_pCmd){
  m_oSync.lock();
  m_lCommandQueue.push_back(pa_pCmd);
  m_oSync.unlock();
}

void COpcEventHandler::run(){
  HRESULT result = CoInitializeEx(NULL, COINIT_MULTITHREADED);

  if(result == S_OK){
    while(isAlive()){
      ICmd* nextCommand = getNextCommand();
      if(nextCommand != NULL)
        nextCommand->runCommand();

      MSG msg;
      while(PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE)){
        TranslateMessage(&msg);
        DispatchMessage(&msg);
      }
    }
  }

  CoUninitialize();
}

COpcEventHandler::TCallbackDescriptor COpcEventHandler::addComCallback(forte::com_infra::CComLayer* pa_pComCallback){
  m_oSync.lock();
  m_nCallbackDescCount++;
  TComContainer stNewNode = { m_nCallbackDescCount, pa_pComCallback };
  m_lstComCallbacks.push_back(stNewNode);
  m_oSync.unlock();

  return m_nCallbackDescCount;
}

void COpcEventHandler::removeComCallback(COpcEventHandler::TCallbackDescriptor pa_nCallbackDesc){
  m_oSync.lock();

  TCallbackList::Iterator itRunner(m_lstComCallbacks.begin());

  if(itRunner->m_nCallbackDesc == pa_nCallbackDesc){
    m_lstComCallbacks.pop_front();
  }
  else{
    TCallbackList::Iterator itLastPos(itRunner);
    TCallbackList::Iterator itEnd(m_lstComCallbacks.end());
    ++itRunner;
    while(itRunner != itEnd){
      if(itRunner->m_nCallbackDesc == pa_nCallbackDesc){
        m_lstComCallbacks.eraseAfter(itLastPos);
        break;
      }
      itLastPos = itRunner;
      ++itRunner;
    }
  }
  m_oSync.unlock();
}

void COpcEventHandler::executeComCallback(COpcEventHandler::TCallbackDescriptor pa_nCallbackDesc){
  m_oSync.lock();
  TCallbackList::Iterator itEnd(m_lstComCallbacks.end());
  for(TCallbackList::Iterator itCallback = m_lstComCallbacks.begin(); itCallback != itEnd; ++itCallback){
    if(itCallback->m_nCallbackDesc == pa_nCallbackDesc){
      //FIX
      TComContainer comCon = (*itCallback);
      m_oSync.unlock();
      if(forte::com_infra::e_Nothing != comCon.m_pCallback->recvData(0,0)){
        startNewEventChain(comCon.m_pCallback->getCommFB());
      }
      m_oSync.lock();
      break;
    }
  }
  m_oSync.unlock();
}

ICmd* COpcEventHandler::getNextCommand(){
  ICmd* command;

  m_oSync.lock();
  TCommandQueue::Iterator itBegin = m_lCommandQueue.begin();
  if(itBegin != m_lCommandQueue.end()){
    command = (*itBegin);
    m_lCommandQueue.pop_front();
  }
  else
    command = NULL;
  m_oSync.unlock();

  return command;
}
