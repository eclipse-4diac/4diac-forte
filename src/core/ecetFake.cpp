/*******************************************************************************
 * Copyright (c) 2018 - fortiss GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Jose Cabral - initial implementation and rework communication infrastructure
 *******************************************************************************/

#include "ecetFake.h"
#include "funcbloc.h"

CFakeEventExecutionThread::CFakeEventExecutionThread() :
    CEventChainExecutionThread(){
  mProcessEventCallback = [this](TEventEntry paEvent){
    paEvent.mFB->receiveInputEvent(paEvent.mPortId, this);
  };
  removeExternalControl();
}

void CFakeEventExecutionThread::removeExternalControl() {
  mIsRemoteEnabled = false;      
  resumeSelfSuspend();
}

void CFakeEventExecutionThread::takeExternalControl() {
  mIsRemoteEnabled = true;
}

void CFakeEventExecutionThread::setRemoteCallbackForEventTriggering(HandleEvent paCallback) {
  mProcessEventCallback = paCallback;
}

void CFakeEventExecutionThread::triggerNextEvent(){
  // if remote is not enabled, we don't manually trigger events
  if(!mIsRemoteEnabled){
    return;
  }

  // manually triggering events, is similar to the regular one but: 
  // - no external events are handled 
  // - if no events are available, the function returns instead of suspending itself
  auto event = mEventList.pop();
  if(nullptr == event){
    return;
  }
  mProcessEventCallback(*event);
}

void CFakeEventExecutionThread::startEventChain(TEventEntry paEvent) {
  // if remote is enabled, external events are inhibited
  if(mIsRemoteEnabled){
    return;
  }
  CEventChainExecutionThread::startEventChain(paEvent);
}

// remote methods
void CFakeEventExecutionThread::insertFront(TEventEntry paEvent){
  // the ring buffer does not have a way to insert in the front,
  // so we create a new one and push the event first there and then the rest
  // and then copy the events back to the original list
  decltype(mEventList) temp;
  temp.push(paEvent);
  while(!mEventList.isEmpty()){
    temp.push(*mEventList.pop());
  }

  while(!temp.isEmpty()){
    mEventList.push(*temp.pop());
  }
}

void CFakeEventExecutionThread::removeFromBack(size_t paNumberOfItemsToRemove){
 // if remote is not enabled, we don't manually remove events
  if(!mIsRemoteEnabled){
    return;
  }

  std::vector<TEventEntry> temp;
  while(!mEventList.isEmpty()){
    temp.push_back(*mEventList.pop());
  }

  while(paNumberOfItemsToRemove-- != 0){
    temp.pop_back();
  }

  for(auto& event : temp){
    mEventList.push(event);
  }
}

std::optional<TEventEntry> CFakeEventExecutionThread::getNextEvent(){
  auto nextEvent = mEventList.pop(); // get a copy, but need to pop for it
  if(nextEvent == nullptr){
    return std::nullopt;
  }

  insertFront(*nextEvent);
  return *nextEvent;
}

void CFakeEventExecutionThread::run(){
  while(isAlive()){
    if(mIsRemoteEnabled){
      selfSuspend();
      if(mIsRemoteEnabled){
        continue;
      }
    }
    if(auto nextEvent = getNextEvent(); nextEvent.has_value() && mBreakpoints.count(nextEvent.value()) != 0){
      takeExternalControl();
      mBreakpointWasHit(nextEvent.value());
      continue;
    }
    CEventChainExecutionThread::mainRun();
  }
}

// Breakpoint methods

void CFakeEventExecutionThread::addBreakpoint(TEventEntry paBreakpoint){
  mBreakpoints.insert(paBreakpoint);
}

void CFakeEventExecutionThread::removeBreakpoint(TEventEntry paBreakpoint){
  mBreakpoints.erase(paBreakpoint);
}

void CFakeEventExecutionThread::setBreakpointHitCallback(HandleEvent paCallback) {
  mBreakpointWasHit = paCallback;
}