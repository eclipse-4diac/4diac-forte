/*******************************************************************************
 * Copyright (c) 2010, 2011 ACIN, Profactor GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *  Alois Zoitl, Gerhard Ebenhofer - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "sync.h"

CPCSyncObject::CPCSyncObject(){
   InitializeCriticalSection(&m_oMutexHandle);
}

CPCSyncObject::~CPCSyncObject(){
	DeleteCriticalSection(&m_oMutexHandle);
  //TODO handle return value
}
