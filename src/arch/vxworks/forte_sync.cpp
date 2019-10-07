/*******************************************************************************
 * Copyright (c) 2012 ACIN
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

CVXWorksSyncObject::CVXWorksSyncObject(){
  m_oSemBinary = semBCreate(SEM_Q_FIFO, SEM_FULL); //semBCreate allows to be lock/unlock by different task, needed by CThread
  if (SEM_ID_NULL == m_oSemBinary){
    //TODO: check return value for out_of_memory error
  }
}

CVXWorksSyncObject::~CVXWorksSyncObject(){
  semDelete(m_oSemBinary);
}
