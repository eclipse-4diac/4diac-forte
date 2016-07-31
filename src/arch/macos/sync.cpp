/*******************************************************************************
 * Copyright (c) 2005 - 2010, 2016 ACIN, fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *  Alois Zoitl, Rene Smodic - initial API and implementation and/or initial
 *                             documentation
 *  Alois Zoitl  - taken this file from the Posix version and adapted it for
 *                 Mac OS
 *******************************************************************************/
#include "sync.h"

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
