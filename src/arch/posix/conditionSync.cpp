/*******************************************************************************
 * Copyright (c) 2016 Johannes Messmer (admin@jomess.com)
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Johannes Messmer - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "conditionSync.h"

namespace Utils {

ConditionSync::ConditionSync() {
  pthread_condattr_t condAttr;
  pthread_condattr_init(&condAttr);
  pthread_condattr_setclock(&condAttr, CLOCK_MONOTONIC);

  pthread_cond_init(&cond, &condAttr);

  pthread_condattr_destroy(&condAttr);

  pthread_mutexattr_t mutexAttr;
  pthread_mutexattr_init(&mutexAttr);
  #ifdef __CYGWIN__
  pthread_mutexattr_settype(&mutexAttr, PTHREAD_MUTEX_RECURSIVE);
  #else
  pthread_mutexattr_settype(&mutexAttr, PTHREAD_MUTEX_RECURSIVE_NP);
  #endif
  pthread_mutex_init(&mutex, &mutexAttr);

  pthread_mutexattr_destroy(&mutexAttr);
}

ConditionSync::~ConditionSync() {
  pthread_cond_destroy(&cond);
  pthread_mutex_destroy(&mutex);
}

void ConditionSync::lock() {
  pthread_mutex_lock(&mutex);
}

void ConditionSync::unlock() {
  pthread_mutex_unlock(&mutex);
}

void ConditionSync::wait(struct timespec& referenceTime,
    unsigned long addMicroseconds) {
  addTime(referenceTime, addMicroseconds);
  waitUntil(referenceTime);
}

void ConditionSync::waitUntil(struct timespec& deadline) {
  pthread_cond_timedwait(&cond, &mutex, &deadline);
}

void ConditionSync::wakeUp() {
  pthread_cond_signal (&cond);
}

void ConditionSync::addTime(struct timespec& ts, unsigned long microseconds) {
  ts.tv_sec += microseconds / (unsigned long) 1E6;
  unsigned long t = ts.tv_nsec + microseconds * (unsigned long) 1E3
      - (microseconds / (unsigned long) 1E6) * (unsigned long) 1E9;
  if (t >= (unsigned long) 1E9) {
    t -= (unsigned long) 1E9;
    ts.tv_sec++;
  }
  ts.tv_nsec = t;
}

} /* namespace Utils */
