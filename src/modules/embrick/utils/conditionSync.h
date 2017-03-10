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

#ifndef SRC_MODULES_EMBRICK_UTILS_CONDITIONSYNC_H_
#define SRC_MODULES_EMBRICK_UTILS_CONDITIONSYNC_H_

#include <pthread.h>

namespace EmBrick {

class ConditionSync {
public:
  ConditionSync();
  virtual ~ConditionSync();

  void lock();
  void waitUntil(struct timespec& deadline);
  void unlock();
  void wakeUp();

private:
  pthread_cond_t cond;
  pthread_mutex_t mutex;

};

} /* namespace EmBrick */

#endif /* SRC_MODULES_EMBRICK_UTILS_CONDITIONSYNC_H_ */
