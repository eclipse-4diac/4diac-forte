/*******************************************************************************
 * Copyright (c) 2016 fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *  Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "rcXUtilities.h"
#include "sync.h"
#include <fortenew.h>

CrcXSyncObject::CrcXSyncObject(){
  mMutexHandle = new char[RX_MUTEX_SIZE];
  if(0 == mMutexHandle){
    DEVLOG_ERROR("Not enough memory to allocate %i bytes for creating a new mutex\n", RX_MUTEX_SIZE);
  }else{
    char mutexName[8];
    getRandomString(&mutexName[0], 7);
    if(RX_OK != rX_MtxCreateMutex(&mutexName[0], mMutexHandle, 0)){
      DEVLOG_ERROR("Could not create mutex\n");
    }
  }
}

CrcXSyncObject::~CrcXSyncObject(){
  rX_MtxDeleteMutex(mMutexHandle);
  delete[] mMutexHandle;
}
