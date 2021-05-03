/*******************************************************************************
 * Copyright (c) 2010, 2020 ACIN, Profactor GmbH, fortiss GmbH, OFFIS e.V.
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *  Alois Zoitl, Gerhard Ebenhofer - initial API and implementation and/or initial documentation
 *  Jörg Walter - Windows XP compatibility
 *******************************************************************************/

#include "forte_sync.h"


CPCSyncObject::CPCSyncObject(){
  InitializeCriticalSection(&mLock);
}

CPCSyncObject::~CPCSyncObject(){
  DeleteCriticalSection(&mLock);
}

void CPCSyncObject::lock(void){
  EnterCriticalSection(&mLock);
}

//!Free the resource coming after the lock command
void CPCSyncObject::unlock(void){
  LeaveCriticalSection(&mLock);
}
