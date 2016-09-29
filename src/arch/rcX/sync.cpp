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
#include "sync.h"

CrcXSyncObject::CrcXSyncObject(){
  rX_MemAllocateMemory(&hMtxTsk1,RX_MUTEX_SIZE);
  //TODO handle result
  
  rX_MtxCreateMutex("Mforte", mMutexHandle, 0);
  //TODO handle result
}

CrcXSyncObject::~CrcXSyncObject(){
  rX_MtxDeleteMutex(mMutexHandle);
  //TODO handle return value
}
