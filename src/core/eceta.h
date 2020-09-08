/*******************************************************************************
 * Copyright (c) 2019 TU Wien ACIN
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Peter Gsellmann, Martin Melik Merkumians - inital API for virtual interface for several types of
 *       event chain thread execution mechanisms
 *******************************************************************************/

#ifndef SRC_CORE_ECETA_H_
#define SRC_CORE_ECETA_H_

#include "event.h"
#include "mgmcmd.h"
#include "forte_thread.h"

class CEventChainExecutionThreadAbstract {
  public:
    CEventChainExecutionThreadAbstract() {
    }
    ;
    virtual ~CEventChainExecutionThreadAbstract() {
    }
    ;
    virtual void addEventEntry(SEventEntry *paEventToAdd) = 0;
    virtual void changeExecutionState(EMGMCommandType paCommand) = 0;
    virtual void startEventChain(SEventEntry *paEventToAdd) = 0;
    virtual void joinEventChainExecutionThread() = 0;
    virtual void resumeSelfSuspend() = 0;

  private:
    virtual void selfSuspend() = 0;

};

#endif /* SRC_CORE_ECETA_H_ */
