/*******************************************************************************
 * Copyright (c) 2012 ACIN
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "thread.h"
#include "../devlog.h"
#include <errno.h>
#include <BEModule.hpp>

void CBEThread::Main(VOID){
  m_bAlive = true;
  run();
}

bool CBEThread::destroy(void){
  end();
  return true;
}

CBEThread::CBEThread() :
    mTskName(0){
  m_bAlive = false;
  m_nDeadline = 0;
}

CBEThread::~CBEThread(){
  destroy();
}

bool CBEThread::isAlive(void){
  return m_bAlive;
}

void CBEThread::setDeadline(TForteUInt32 pa_nVal){
  m_nDeadline = pa_nVal;
  //TODO implement priority assignment similar to threadx and ecos
}

TForteUInt32 CBEThread::getDeadline(void){
  return m_nDeadline;
}

void CBEThread::start(void){
  if(Spawn(0, 0, 130, VX_FP_TASK, APP_TSK_STACK) == ERROR){
    log_Err("test8_mod::APPLATEINIT: Error in TaskSpawn; '%s'!", "tForteTask");
  }
}

void CBEThread::selfSuspend(void){
  Suspend();
}

void CBEThread::resumeSelfSuspend(void){
  Resume();
}

void CBEThread::end(void){
  m_bAlive = false;
  resumeSelfSuspend();
  join();
}

void CBEThread::join(void){
  Delete();
}
