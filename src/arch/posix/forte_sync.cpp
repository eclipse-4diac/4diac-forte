/*******************************************************************************
 * Copyright (c) 2005 - 2010, 2017 ACIN, fortiss GmbH
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
  pthread_mutex_init(&m_oMutexHandle, 0);
}

CPCSyncObject::~CPCSyncObject(){
  pthread_mutex_destroy(&m_oMutexHandle);
  //TODO handle return value
}
