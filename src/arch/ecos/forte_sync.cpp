/*******************************************************************************
 * Copyright (c) 2006, 2007 ACIN
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "forte_sync.h"

CECOSSyncObject::CECOSSyncObject(){
  cyg_mutex_init(&m_oMutexHandle);
  cyg_mutex_set_protocol(&m_oMutexHandle, CYG_MUTEX_NONE);
}

CECOSSyncObject::~CECOSSyncObject(){
  cyg_mutex_destroy(&m_oMutexHandle);
}
