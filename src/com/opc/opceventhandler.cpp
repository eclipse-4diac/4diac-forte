/*******************************************************************************
 * Copyright (c) 2012, 2022 AIT, fortiss GmbH, Hit robot group
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Filip Andren, Alois Zoitl - initial API and implementation and/or initial documentation
 *   ys guo - Fix opc module compilation errors and deadlock bug
 *   Tibalt Zhao - use stl vector
 *******************************************************************************/
#include "opceventhandler.h"
#include "../core/devexec.h"
#include <commfb.h>

#include <ObjBase.h>

DEFINE_HANDLER(COpcEventHandler);

COpcEventHandler::TCallbackDescriptor COpcEventHandler::mCallbackDescCount = 0;

COpcEventHandler::COpcEventHandler(CDeviceExecution& paDeviceExecution) : CExternalEventHandler(paDeviceExecution)  {
  this->start();
  // Sleep to allow new thread to start
  CThread::sleepThread(100);
}

COpcEventHandler::~COpcEventHandler(){
  this->end();
}

void COpcEventHandler::sendCommand(ICmd *paCmd){
  CCriticalRegion critcalRegion(mSync);
  mCommandQueue.pushBack(paCmd);
}

void COpcEventHandler::run(){
  HRESULT result = CoInitializeEx(nullptr, COINIT_MULTITHREADED);

  if(result == S_OK){
    while(isAlive()){
      ICmd* nextCommand = getNextCommand();
      if(nextCommand != nullptr) {
        nextCommand->runCommand();
        delete nextCommand;
      }

      MSG msg;
      while(PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)){
        TranslateMessage(&msg);
        DispatchMessage(&msg);
      }
      CThread::sleepThread(100);
    }
  }

  CoUninitialize();
}

COpcEventHandler::TCallbackDescriptor COpcEventHandler::addComCallback(forte::com_infra::CComLayer* paComCallback){
  CCriticalRegion critcalRegion(mSync);
  mCallbackDescCount++;
  DEVLOG_INFO("COpcEventHandler::addComCallback[%d]\n",mCallbackDescCount);
  TComContainer stNewNode = { mCallbackDescCount, paComCallback };
  mComCallbacks.push_back(stNewNode);

  return mCallbackDescCount;
}

void COpcEventHandler::removeComCallback(COpcEventHandler::TCallbackDescriptor paCallbackDesc){
  CCriticalRegion critcalRegion(mSync);
  for(TCallbackList::iterator itRunner = mComCallbacks.begin(); itRunner != mComCallbacks.end(); ++itRunner){
    if(itRunner->mCallbackDesc == paCallbackDesc){
      mComCallbacks.erase(itRunner);
      break; 
    }
  }
}

void COpcEventHandler::executeComCallback(COpcEventHandler::TCallbackDescriptor paCallbackDesc){
  CCriticalRegion critcalRegion(mSync);
  if(mComCallbacks.empty()){
    return;
  }
  for(TCallbackList::iterator itCallback = mComCallbacks.begin(); itCallback != mComCallbacks.end(); ++itCallback){
    if(itCallback->mCallbackDesc == paCallbackDesc){
      //FIX
      TComContainer comCon = (*itCallback);
      mSync.unlock();
      if(forte::com_infra::e_Nothing != comCon.mCallback->recvData(0,0)){
        startNewEventChain(comCon.mCallback->getCommFB());
      }
      mSync.lock();
      break;
    }
  }
}

ICmd* COpcEventHandler::getNextCommand(){
  ICmd* command = nullptr;

  CCriticalRegion critcalRegion(mSync);
  TCommandQueue::Iterator itBegin = mCommandQueue.begin();
  if(itBegin != mCommandQueue.end()){
    command = (*itBegin);
    mCommandQueue.popFront();
  }
  return command;
}
