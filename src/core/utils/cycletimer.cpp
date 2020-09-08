/*******************************************************************************
 * Copyright (c) 2020 TU Wien ACIN
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Peter Gsellmann, Martin Melik Merkumians - initial implementation
 *******************************************************************************/
#include "cycletimer.h"
#include "../devlog.h"

CCycleTimer::CCycleTimer(CEventChainExecutionThread61131 *paThread, TForteInt32 paPreset) :
    cycleTime(paPreset), mThread(paThread) {
}

CCycleTimer::~CCycleTimer() {
  end();
}

void CCycleTimer::run() {
  while(isAlive()) {
    sleepThread(cycleTime);
    if(!mThread->getExeDone()) {
      mThread->end();
    } else {
      mThread->resumeSelfSuspend();
    }
  }
}
