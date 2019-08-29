/*******************************************************************************
 * Copyright (c) 2016 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *  Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "forte_sync.h"

#include "rcXUtilities.h"
#include <fortenew.h>
#include "../devlog.h"

CrcXSyncObject::CrcXSyncObject(){
  mMutexHandle = forte_malloc(RX_MUTEX_SIZE);
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
  forte_free(mMutexHandle);
}
