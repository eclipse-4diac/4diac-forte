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
#ifndef _FMUECET_H_
#define _FMUECET_H_

#include "../ecet.h"

class CFMUEventChainExecutionThread : public CEventChainExecutionThread{
  public:
    CFMUEventChainExecutionThread();
    virtual ~CFMUEventChainExecutionThread();

    void setAllowedToRun(bool* paAllowedToRun){
      mAllowedToRun = paAllowedToRun;
    }

    void setStepSemaphore(forte::arch::CSemaphore* paStepSemaphore){
      mStepSemaphore = paStepSemaphore;
    }

    bool hasMoreEvents(){
      return (mEventListEnd != mEventListStart);
    }

    bool isInWaitingStepState(){
      return mWaitingStep;
    }

private:
    virtual void run(void);
    bool* mAllowedToRun;
    forte::arch::CSemaphore* mStepSemaphore;
    bool mWaitingStep;

};

#endif /*_FMUECET_H_*/
