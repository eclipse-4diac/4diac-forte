/************************************************************************************
 * Copyright (c) 2016 fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 * Milan Vathoopan, Guru Chandrasekhara - initial API and implementation and/or initial documentation
 ************************************************************************************/

#include "forte_sync.h"

CFreeRTOSSyncObject::CFreeRTOSSyncObject(){
  mMutexHandle = xSemaphoreCreateMutex();
}

CFreeRTOSSyncObject::~CFreeRTOSSyncObject(){
  vSemaphoreDelete(mMutexHandle);
}
