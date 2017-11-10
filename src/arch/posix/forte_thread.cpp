/*******************************************************************************
 * Copyright (c) 2005 - 2017 ACIN, fortiss GmbH, Red Hat Inc
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *  Alois Zoitl, Rene Smodic, Martin Melik Merkiumians - initial API and implementation and/or initial documentation
 *  Alois Zoitl - extracted common functions to new base class CThreadBase
 *  Jens Reimann - fix waiting for thread initialization
 *******************************************************************************/
#include <fortealloc.h>
#include "forte_thread.h"
#include "../devlog.h"
#include <errno.h>
#include <cstring>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include <criticalregion.h>

forte::arch::CThreadBase<pthread_t, static_cast<pthread_t>(0)>::TThreadHandleType CPosixThread::createThread(long paStackSize){
	TThreadHandleType retVal = 0;

	if(paStackSize){
	  pthread_attr_t stAttr;

	  if(pthread_attr_init(&stAttr)){
      DEVLOG_ERROR("Error could not get the default thread attributes! %s\n", strerror(errno));
      return 0;
	  }
	#ifdef __CYGWIN__
	  if (pthread_attr_setstacksize (&stAttr, paStackSize)){
      DEVLOG_ERROR("Error could not set the stacksize for the thread! %s\n", strerror(errno));
      return 0;
	  }
	#else
	  //if (pthread_attr_setstacksize(&stAttr, m_nStackSize)) {
	  if(pthread_attr_setstack(&stAttr, m_pacStack, m_nStackSize)){
      DEVLOG_ERROR("Error could not set the stacksize for the thread! %s\n", strerror(errno));
      return 0;
	  }
	#endif
	  if(pthread_create(&retVal, &stAttr, threadFunction, this)){
      DEVLOG_ERROR("Error could not create the thread! %s\n", strerror(errno));
      return 0;
	  }
	  if(pthread_attr_destroy(&stAttr)){
      DEVLOG_ERROR("Error could not free the thread attributes! %s\n", strerror(errno));
      return 0;
	  }
	}
	else{
	  if(pthread_create(&retVal, NULL, threadFunction, this)){
      DEVLOG_ERROR("Error could not create the thread! %s\n", strerror(errno));
      return 0;
	  }
	}
	// Detach because we don't care about the thread anymore/don't need to join. To cleanup either call pthread_detach or pthread_join
	pthread_detach(retVal);

	return retVal;
}

void * CPosixThread::threadFunction(void *paArguments){
  // Get pointer to CThread object out of void pointer
  CThreadBase::runThread(static_cast<CPosixThread *>(paArguments));
  return 0;
}

CPosixThread::CPosixThread(long paStackSize) : CThreadBase(paStackSize), mStack(0){
  if(0 != paStackSize){
    mStack = new char[paStackSize];
  }
}

CPosixThread::~CPosixThread(){
  if(0 != mStack ){
    delete[] mStack;
  }
}

void CPosixThread::setDeadline(const CIEC_TIME &paVal){
  mDeadline = paVal;
  //under the posix pthread implemention currently it makes no sense to set any priority.
  //It will not be considered.
}

void CPosixThread::sleepThread(unsigned int paMilliSeconds){
  usleep(1000 * paMilliSeconds);
}
