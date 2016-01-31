/*******************************************************************************
 * Copyright (c) 2006 - 2015 Profactor GmbH, ACIN, fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Gunnar Grabmaier, Alois Zoitl, Ingo Hegny
 *      - initial implementation and rework communication infrastructure
 *******************************************************************************/
#include "if2indco.h"

CInterface2InternalDataConnection::CInterface2InternalDataConnection() :
    CDataConnection(0, cg_unInvalidPortId, 0){
}

CInterface2InternalDataConnection::~CInterface2InternalDataConnection(){
  m_poValue = 0; // in interface data connections we are not allowed to delete the m_poValue
}

void CInterface2InternalDataConnection::cloneInputInterfaceValue(void){
  for(TDestinationIdList::Iterator it = mDestinationIds.begin();
      it != mDestinationIds.end(); ++it){
    it->mFB->connectDI(it->mPortId, this);
  }
}

void CInterface2InternalDataConnection::setSource(CFunctionBlock *paSrcFB, TPortId paSrcPortId){
  CConnection::setSource(paSrcFB, paSrcPortId);
  m_poValue = paSrcFB->getDIFromPortId(paSrcPortId);
}

