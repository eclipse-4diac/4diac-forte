/*******************************************************************************
 * Copyright (c) 2005 - 2015 Profactor GmbH, ACIN, fortiss GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Thomas Strasser, Alois Zoitl,
 *      - initial implementation and rework communication infrastructure
 *******************************************************************************/
#include "eventconn.h"
#include "ecet.h"
#include "funcbloc.h"

CEventConnection::CEventConnection(CFunctionBlock *paSrcFB, TPortId paSrcPortId) :
    CConnection(paSrcFB, paSrcPortId){
}

CEventConnection::~CEventConnection(){
}

EMGMResponse CEventConnection::connect(CFunctionBlock *paDstFB, CStringDictionary::TStringId paDstPortNameId){
  EMGMResponse retval = e_NO_SUCH_OBJECT;
  TPortId nEIID = paDstFB->getEIID(paDstPortNameId);

  if(cg_nInvalidEventID != nEIID){
    retval = CConnection::addDestination(CConnectionPoint(paDstFB, nEIID));
  }
  return retval;
}

EMGMResponse CEventConnection::connectToCFBInterface(CFunctionBlock *paDstFB, CStringDictionary::TStringId paDstPortNameId){
  EMGMResponse retval = e_NO_SUCH_OBJECT;
  TPortId nEOID = paDstFB->getEOID(paDstPortNameId);

  if(cg_nInvalidEventID != nEOID){
    nEOID |= cgInternal2InterfaceMarker;
    retval = CConnection::addDestination(CConnectionPoint(paDstFB, nEOID));
  }
  return retval;
}

EMGMResponse CEventConnection::disconnect(CFunctionBlock *paDstFB, CStringDictionary::TStringId paDstPortNameId){
  EMGMResponse retval = e_NO_SUCH_OBJECT;
  TEventID nEIID = paDstFB->getEIID(paDstPortNameId);

  if(cg_nInvalidEventID != nEIID){
    retval = CConnection::removeDestination(CConnectionPoint(paDstFB, nEIID));
  }
  return retval;
}

void CEventConnection::triggerEvent(CEventChainExecutionThread *pa_poExecEnv) const {
  if(nullptr != pa_poExecEnv) {
    for(TDestinationIdList::Iterator it = mDestinationIds.begin();
        0 != it.getPosition(); ++it){
      pa_poExecEnv->addEventEntry(&(*it));
    }
  }
}


