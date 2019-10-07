/*******************************************************************************
 * Copyright (c) 2005 - 2010, 2016 ACIN, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *  Alois Zoitl, Rene Smodic - initial API and implementation and/or initial
 *                             documentation
 *  Alois Zoitl  - taken this file from the Posix version and adapted it for
 *                 Mac OS
 *******************************************************************************/
#include "forte_sync.h"
#include <errno.h>
#include <string.h>

CMacOsSyncObject::CMacOsSyncObject(){
  pthread_mutexattr_t attr;
  pthread_mutexattr_init(&attr);

  pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
  pthread_mutex_init(&m_oMutexHandle, &attr);
  //TODO handle return value
  pthread_mutexattr_destroy(&attr);
}

CMacOsSyncObject::~CMacOsSyncObject(){
  pthread_mutex_destroy(&m_oMutexHandle);
  //TODO handle return value
}
