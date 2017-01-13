/*******************************************************************************
 * Copyright (c) 2012 ACIN
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Alois Zoitl - initial API and implementation and/or initial documentation
 *******************************************************************************/
#ifndef _FORTE_SYNC_H_
#define _FORTE_SYNC_H_

#include "vxWorks.h"
#include "semLib.h"

class CVXWorksSyncObject{
  public:
    CVXWorksSyncObject();
    virtual ~CVXWorksSyncObject();

    void lock(void){
      semTake(m_oSemBinary, WAIT_FOREVER);
    }

    void unlock(void){
      semGive(m_oSemBinary);
    }

  private:
    SEM_ID m_oSemBinary;

};

typedef CVXWorksSyncObject CSyncObject; //allows that doxygen can generate better documentation

#endif /*FORTE_SYNC_H_*/
