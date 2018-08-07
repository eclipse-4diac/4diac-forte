/*******************************************************************************
  * Copyright (c) 2018 - fortiss GmbH
  * All rights reserved. This program and the accompanying materials
  * are made available under the terms of the Eclipse Public License v1.0
  * which accompanies this distribution, and is available at
  * http://www.eclipse.org/legal/epl-v10.html
  *
  * Contributors:
  *    Jose Cabral - initial implementation and rework communication infrastructure
  *
  *******************************************************************************/

#include "fmuEcet.h"
#include "fmi/fmiTimerHandler.h"
#include "fmuInstance.h"
#include <assert.h>


CFMUEventChainExecutionThread::CFMUEventChainExecutionThread() :
    CEventChainExecutionThread(), mAllowedToRun(0), mStepSemaphore(0), mWaitingStep(true){
  fmuInstance::sFmuInstance->registerEcet(this);

}

CFMUEventChainExecutionThread::~CFMUEventChainExecutionThread(){
}

void CFMUEventChainExecutionThread::run(void){
  assert(mAllowedToRun);
  assert(mStepSemaphore);
  bool runOnce = true; //don't allow the first time to run
  while(isAlive()){ //thread is allowed to execute
    if(*mAllowedToRun || !runOnce){
      runOnce = true;
      CEventChainExecutionThread::mainRun();
    }else{
      mWaitingStep = true;
      mStepSemaphore->waitIndefinitely();
      mWaitingStep = false;
      runOnce = false; //this allows the ecet to run at least once, for example if mAllowedToRun is set to false quickly enough after this leaves the StepSuspend state
    }
  }
}


