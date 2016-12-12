/*******************************************************************************
 * Copyright (c) 2005 - 2010 ACIN
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *  Alois Zoitl, Rene Smodic - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "forte_sync.h"

CPCSyncObject::CPCSyncObject(){
//  m_oMutexHandle = PTHREAD_MUTEX_INITIALIZER; 
  
  pthread_mutexattr_t attr;
  pthread_mutexattr_init(&attr);

  #ifdef __CYGWIN__
  pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
  #else
  pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE_NP);
  #endif

  pthread_mutex_init(&m_oMutexHandle, &attr);
  //TODO handle return value
  pthread_mutexattr_destroy(&attr);  
}

CPCSyncObject::~CPCSyncObject(){
  pthread_mutex_destroy(&m_oMutexHandle);
  //TODO handle return value
}
