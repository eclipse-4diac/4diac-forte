/*******************************************************************************
 * Copyright (c) 2012, 2022 AIT, fortiss GmbH, Hit robot group
 *               2024 Samator Indo Gas
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
 *   Ketut Kumajaya - Clear command in queue on exit
 *                  - Fix disconnection issue on exit
 *******************************************************************************/
#include "opceventhandler.h"
#include "../core/devexec.h"
#include <commfb.h>

#include <ObjBase.h>

DEFINE_HANDLER(COpcEventHandler);

CSyncObject COpcEventHandler::mSync;
forte::arch::CSemaphore COpcEventHandler::mStateSemaphore;
bool COpcEventHandler::mIsSemaphoreEmpty = true;

COpcEventHandler::TCallbackDescriptor COpcEventHandler::mCallbackDescCount = 0;

COpcEventHandler::COpcEventHandler(CDeviceExecution& paDeviceExecution) : CExternalEventHandler(paDeviceExecution)  {
}

COpcEventHandler::~COpcEventHandler(){
  clearCommandQueue(); //check and delete all commands in queue if not empty
}

void COpcEventHandler::clearCommandQueue(){
  while(!mCommandQueue.isEmpty()) {
    // should never have come in here
    ICmd* nextCommand = getNextCommand();
    if(nextCommand != nullptr) {
      DEVLOG_ERROR("erase from command queue[%s]\n", nextCommand->getCommandName());
      delete nextCommand;
    }
  }
  mCommandQueue.clearAll();

  for(TCallbackList::iterator itRunner = mComCallbacks.begin(); itRunner != mComCallbacks.end(); ++itRunner){
    DEVLOG_ERROR("erase from command callback\n");
    mComCallbacks.erase(itRunner);
  }
  //clear vector and remove it from memory
  TCallbackList().swap(mComCallbacks);
}

void COpcEventHandler::executeCommandQueue(){
  while(!mCommandQueue.isEmpty()) {
    ICmd* nextCommand = getNextCommand();
    if(nextCommand != nullptr) {
      nextCommand->runCommand();
      delete nextCommand;
      nextCommand = nullptr;
    }
  }
  mCommandQueue.clearAll();
}

void COpcEventHandler::sendCommand(ICmd *paCmd){
  mSync.lock();
  mCommandQueue.pushBack(paCmd);
  mSync.unlock();
  resumeSelfSuspend();
}

void COpcEventHandler::run(){
  while(isAlive()){
    executeCommandQueue();
    if (!isAlive()) {
      break;
    }
    selfSuspend();
  }
  executeCommandQueue(); // immediately try to clear the command queue after a stop
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
  if(mComCallbacks.empty()){
    //no content, remove vector from memory
    TCallbackList().swap(mComCallbacks);
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

void COpcEventHandler::resumeSelfSuspend() {
  if (mIsSemaphoreEmpty) { //avoid incrementing many times
    {
      CCriticalRegion critcalRegion(mSync);
      mStateSemaphore.inc();
      mIsSemaphoreEmpty = false;
    }
  }
}

void COpcEventHandler::selfSuspend() {
  mStateSemaphore.waitIndefinitely();
  {
    CCriticalRegion critcalRegion(mSync);
    mIsSemaphoreEmpty = true;
  }
}
