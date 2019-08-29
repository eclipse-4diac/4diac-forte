/*******************************************************************************
 * Copyright (c) 2006 - 2015 Profactor GmbH, ACIN, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Gunnar Grabmaier, Alois Zoitl, Ingo Hegny
 *      - initial implementation and rework communication infrastructure
 *******************************************************************************/
#include "if2indco.h"
#include "funcbloc.h"

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

