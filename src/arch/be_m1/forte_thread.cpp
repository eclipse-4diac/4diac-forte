/*******************************************************************************
 * Copyright (c) 2012, 2016, 2017 ACIN, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl - initial API and implementation and/or initial documentation
 *   Alois Zoitl - extracted common functions to new base class CThreadBase
 *******************************************************************************/
#include "forte_thread.h"
#include "../devlog.h"
#include <errno.h>
#include <BEModule.hpp>
#include <unistd.h>

void CBEThread::Main(VOID){
  CThreadBase::runThread(this);  //this is a little bit an overkill but better complies to our overall rules
}

CBEThread::CBEThread() : CThreadBase(0){
}

CBEThread::~CBEThread(){
}

void CBEThread::setDeadline(TForteUInt32 paVal){
  mDeadline = paVal;
  //TODO implement priority assignment similar to threadx and ecos
}

forte::arch::CThreadBase<int>::TThreadHandleType CBEThread::createThread(long ){
  int retVal = 1;
  if(Spawn(0, 0, 130, VX_FP_TASK, APP_TSK_STACK) == ERROR){
    retVal = 0;
  }
  return retVal;
}

void CBEThread::sleepThread(unsigned int paMilliSeconds){
  usleep(1000 * paMilliSeconds);
}

void CBEThread::deleteThread(int){
  Delete();
}
