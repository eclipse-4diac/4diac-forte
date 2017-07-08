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

#ifndef SRC_ARCH_POSIX_CONDITIONSYNC_H_
#define SRC_ARCH_POSIX_CONDITIONSYNC_H_

#include <pthread.h>

namespace Utils {

class ConditionSync {
public:
  ConditionSync();
  virtual ~ConditionSync();

  void lock();
  void wait(struct timespec& referenceTime, unsigned long addMicroseconds);
  void waitUntil(struct timespec& deadline);
  void unlock();
  void wakeUp();

private:
  pthread_cond_t cond;
  pthread_mutex_t mutex;

  void addTime(struct timespec& t, unsigned long microseconds);

};

} /* namespace Utils */

#endif /* SRC_ARCH_POSIX_CONDITIONSYNC_H_ */
