/*******************************************************************************
 * Copyright (c) 2006 - 2016 ACIN, Profactor GmbH, fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Alois Zoitl, Thomas Strasser, Rene Smodic, Ingo Hegny
 *    - initial API and implementation and/or initial documentation
 *  Alois Zoitl - extracted common functions to new base class CThreadBase
 *******************************************************************************/
#include "forte_thread.h"
#include <unistd.h>

const char *CTXThread::csm_cName = { "Test Name" };

const int CTXThread::scm_nThreadListSize = 27;
TCTXThreadPtr CTXThread::sm_aoThreadList[27] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

bool CTXThread::create(void) {
  // Get new Thread

  if (m_stThread.tx_thread_id != 0)
    return true;
  if (TX_SUCCESS == tx_thread_create(&m_stThread, m_acThreadName, threadFunction, (ULONG)(this), m_cStack, m_nStackSize, scm_nThreadListSize + 3, scm_nThreadListSize + 3, //disable preemption threshhold
      TX_NO_TIME_SLICE, //disable timeslicing
      TX_AUTO_START))// we start manually
  {
    tx_thread_sleep(1);
    //TODO check return values
    return true;
  }
  else {
    DEVLOG_DEBUG("Error while creating thread with ID: %d\n",(int)m_stThread.tx_thread_id);
    return false;
  }
}

void CTXThread::threadFunction(ULONG data) {
  // Get pointer to CThread object out of void pointer
  CTXThread *pThread = static_cast<CTXThread *>(data);
  // if pointer is ok
  if (pThread) {
    tx_mutex_get(&(pThread->m_stMutex), TX_WAIT_FOREVER);
    pThread->setAlive(true);
    pThread->run();
    pThread->setAlive(false);
    tx_mutex_put(&(pThread->m_stMutex));
  }
}

CTXThread::CTXThread(long pa_nStackSize, char * pa_acThreadName) {
  m_nStackSize = pa_nStackSize;
  m_cStack = malloc(m_nStackSize);
  m_acThreadName = pa_acThreadName;
  memset((void *) &m_stThread, 0, sizeof(m_stThread));

  tx_mutex_create(&m_stMutex, "test", TX_NO_INHERIT);
}

CTXThread::~CTXThread() {
  end();
  tx_mutex_delete(&m_stMutex);
}

void CTXThread::setDeadline(const CIEC_TIME &pa_roVal) {
  int i;
  int ii;
  mDeadLine = pa_roVal;
  //first of all check if this thread is already in the list and remove it from the list
  for (i = 0; i < scm_nThreadListSize; i++) {
    if (0 == sm_aoThreadList[i])
      break;
    else
      if (this == sm_aoThreadList[i]) {
        for (ii = i; ii < scm_nThreadListSize - 1; ii++) {
          if (0 == sm_aoThreadList[ii + 1])
            break;
          sm_aoThreadList[ii + 1]->setPriority(ii + 2);
          sm_aoThreadList[ii] = sm_aoThreadList[ii + 1];
        }
        break;
      }
  }

  if (0 == mDeadLine)
    setPriority(scm_nThreadListSize + 2);
  else {
    for (i = 0; i < scm_nThreadListSize; i++) {
      if (0 == sm_aoThreadList[i]) {
        sm_aoThreadList[i] = this;
        setPriority(i + 2);
        break;
      }
      else
        if (mDeadLine < sm_aoThreadList[i]->getDeadline()) {
          CTXThread *poRBuf, *poSBuf = sm_aoThreadList[i];
          sm_aoThreadList[i] = this;
          setPriority(i + 2);
          for (ii = i + 1; ii < scm_nThreadListSize; ii++) {
            poSBuf->setPriority(ii + 2);
            poRBuf = sm_aoThreadList[ii];
            sm_aoThreadList[ii] = poSBuf;
            if (0 == poRBuf)
              break;
            poSBuf = poRBuf;
          }
          break;
        }
    }
  }
}

void CTXThread::sleepThread(unsigned int pa_miliSeconds){
  usleep(1000 * pa_miliSeconds);
}

void CTXThread::join(void) {
  if (0 != m_stThread.tx_thread_id) {
    //wait till the thread is up and running
    while (!isAlive()) {
      tx_thread_sleep(1);
    }

    tx_mutex_get(&m_stMutex, TX_WAIT_FOREVER);
    tx_mutex_put(&m_stMutex);
    tx_thread_delete(&m_stThread);
    m_stThread.tx_thread_id = 0;
  }
}

// function for output of the semaphore status, for debugging
void CTXThread::printseminfo(TX_SEMAPHORE *pa_stSemaphore) {
  CHAR *name;
  ULONG current_value;
  TX_THREAD *first_suspended;
  ULONG suspended_count;
  TX_SEMAPHORE *next_semaphore;
  UINT status;
  status = tx_semaphore_info_get(pa_stSemaphore, &name, &current_value, &first_suspended, &suspended_count, &next_semaphore);
  //DEVLOG_DEBUG_2(cg_nID_CThread, 0x01,"Seminfo: Name : %s, Value : %d, First Susp. Thread: %d, status : %d\n", name, (int)current_value, (int)first_suspended->tx_thread_id, status);
}
