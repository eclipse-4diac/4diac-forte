/*******************************************************************************
 * Copyright (c) 2006 - 2017 ACIN, Profactor GmbH, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Thomas Strasser, Rene Smodic, Ingo Hegny
 *    - initial API and implementation and/or initial documentation
 *  Alois Zoitl - extracted common functions to new base class CThreadBase
 *******************************************************************************/
#include "forte_thread.h"
#include <unistd.h>
#include <../devlog.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

const int CTXThread::scmThreadListSize = 27;
TCTXThreadPtr CTXThread::sm_aoThreadList[27] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

forte::arch::CThreadBase<TX_THREAD>::TThreadHandleType CTXThread::createThread(long paStackSize){
  memset((void *) &mThreadData, 0, sizeof(mThreadData));

  if (TX_SUCCESS == tx_thread_create(&mThreadData, mThreadName, threadFunction, (ULONG)(this), mStack, paStackSize, scmThreadListSize + 3, scmThreadListSize + 3, //disable preemption threshhold
      TX_NO_TIME_SLICE, TX_AUTO_START)){
    return mThreadData.tx_thread_id;
  }
  return 0;
}

void CTXThread::threadFunction(ULONG paData) {
  CThreadBase::runThread(static_cast<CTXThread *>(paData));
}

CTXThread::CTXThread(long paStackSize, char *paThreadName) : CThreadBase(paStackSize), mThreadName(paThreadName) {
  mStack = new char[paStackSize];
  memset((void *) &mThreadData, 0, sizeof(mThreadData));
}

CTXThread::~CTXThread() {
}

void CTXThread::setDeadline(const CIEC_TIME &paVal) {
  int i;
  int ii;
  mDeadline = paVal;
  //first of all check if this thread is already in the list and remove it from the list
  for (i = 0; i < scmThreadListSize; i++) {
    if (0 == smThreadList[i])
      break;
    else
      if (this == smThreadList[i]) {
        for (ii = i; ii < scmThreadListSize - 1; ii++) {
          if (0 == smThreadList[ii + 1])
            break;
          smThreadList[ii + 1]->setPriority(ii + 2);
          smThreadList[ii] = smThreadList[ii + 1];
        }
        break;
      }
  }

  if (0 == mDeadLine)
    setPriority(scmThreadListSize + 2);
  else {
    for (i = 0; i < scmThreadListSize; i++) {
      if (0 == smThreadList[i]) {
        smThreadList[i] = this;
        setPriority(i + 2);
        break;
      }
      else
        if (mDeadLine < smThreadList[i]->getDeadline()) {
          CTXThread *poRBuf, *poSBuf = smThreadList[i];
          sm_aoThreadList[i] = this;
          setPriority(i + 2);
          for (ii = i + 1; ii < scmThreadListSize; ii++) {
            poSBuf->setPriority(ii + 2);
            poRBuf = smThreadList[ii];
            smThreadList[ii] = poSBuf;
            if (0 == poRBuf)
              break;
            poSBuf = poRBuf;
          }
          break;
        }
    }
  }
}

void CTXThread::sleepThread(unsigned int paMilliSeconds){
  usleep(1000 * paMilliSeconds);
}

