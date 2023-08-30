/*******************************************************************************
 * Copyright (c) 2012 - 2014 AIT, fortiss GmbH, Hit robot group
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Filip Andren, Alois Zoitl - initial API and implementation and/or initial documentation
 *   ys guo - Fix opc module compilation errors and deadlock bug
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
  mSync.lock();
  mCommandQueue.pushBack(paCmd);
  mSync.unlock();
}

void COpcEventHandler::run(){
  HRESULT result = CoInitializeEx(nullptr, COINIT_MULTITHREADED);

  if(result == S_OK){
    while(isAlive()){
      ICmd* nextCommand = getNextCommand();
      if(nextCommand != nullptr) {
        nextCommand->runCommand();
      }

      MSG msg;
      while(PeekMessage(&msg, nullptr, nullptr, nullptr, PM_REMOVE)){
        TranslateMessage(&msg);
        DispatchMessage(&msg);
      }
    }
  }

  CoUninitialize();
}

COpcEventHandler::TCallbackDescriptor COpcEventHandler::addComCallback(forte::com_infra::CComLayer* paComCallback){
  mSync.lock();
  mCallbackDescCount++;
  TComContainer stNewNode = { mCallbackDescCount, paComCallback };
  mComCallbacks.pushBack(stNewNode);
  mSync.unlock();

  return mCallbackDescCount;
}

void COpcEventHandler::removeComCallback(COpcEventHandler::TCallbackDescriptor paCallbackDesc){
  mSync.lock();

  TCallbackList::Iterator itRunner(mComCallbacks.begin());

  if(itRunner->mCallbackDesc == paCallbackDesc){
    mComCallbacks.popFront();
  }
  else{
    TCallbackList::Iterator itLastPos(itRunner);
    TCallbackList::Iterator itEnd(mComCallbacks.end());
    ++itRunner;
    while(itRunner != itEnd){
      if(itRunner->mCallbackDesc == paCallbackDesc){
        mComCallbacks.eraseAfter(itLastPos);
        break;
      }
      itLastPos = itRunner;
      ++itRunner;
    }
  }
  mSync.unlock();
}

void COpcEventHandler::executeComCallback(COpcEventHandler::TCallbackDescriptor paCallbackDesc){
  mSync.lock();
  TCallbackList::Iterator itEnd(mComCallbacks.end());
  for(TCallbackList::Iterator itCallback = mComCallbacks.begin(); itCallback != itEnd; ++itCallback){
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
  mSync.unlock();
}

ICmd* COpcEventHandler::getNextCommand(){
  ICmd* command;

  mSync.lock();
  TCommandQueue::Iterator itBegin = mCommandQueue.begin();
  if(itBegin != mCommandQueue.end()){
    command = (*itBegin);
    mCommandQueue.popFront();
  } else {
    command = nullptr;
  }
  mSync.unlock();

  return command;
}
