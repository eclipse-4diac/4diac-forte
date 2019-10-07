/*******************************************************************************
 * Copyright (c) 2006 - 2010 ACIN, Profactor GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl, Thomas Strasser
 *    - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include "forte_sync.h"

CTXSyncObject::CTXSyncObject(){ 
  tx_mutex_create(&m_stMutex, "Test Mutex", TX_INHERIT);
  //TODO handle return value
}

CTXSyncObject::~CTXSyncObject(){
  tx_mutex_delete(&m_stMutex);
  //TODO handle return value
}
