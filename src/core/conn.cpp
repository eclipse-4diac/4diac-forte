/*******************************************************************************
  * Copyright (c) 2005 - 2015 Profactor GmbH, ACIN, fortiss GmbH
  * All rights reserved. This program and the accompanying materials
  * are made available under the terms of the Eclipse Public License v1.0
  * which accompanies this distribution, and is available at
  * http://www.eclipse.org/legal/epl-v10.html
  *
  * Contributors:
  *    Thomas Strasser, Alois Zoitl, Ingo Hegny,
  *      - initial implementation and rework communication infrastructure
  *******************************************************************************/
#include <string.h>
#include "conn.h"

CConnection::CConnection(CFunctionBlock *paSrcFB, TPortId paSrcPortId) :
    mSourceId(paSrcFB, paSrcPortId){
}

EMGMResponse CConnection::addDestination(const SConnectionPoint &paDestPoint){
  EMGMResponse retval = e_INVALID_STATE;

  if(!dstExists(paDestPoint)){ // check if there is up to now no such fan out with this destination
    mDestinationIds.pushBack(paDestPoint);
    retval = e_RDY;
  }
  return retval;
}

#ifndef FORTE_CLASS_0
EMGMResponse CConnection::removeDestination(const SConnectionPoint &paDestPoint){
  EMGMResponse retval = e_INVALID_STATE;

  TDestinationIdList::Iterator itRunner = mDestinationIds.begin();
  TDestinationIdList::Iterator itRefNode = mDestinationIds.end();

  while(itRunner != mDestinationIds.end()){
    if(paDestPoint == (*itRunner)){
      retval = e_RDY;
      if(itRefNode == mDestinationIds.end()){
        mDestinationIds.popFront();
      }
      else{
        mDestinationIds.eraseAfter(itRefNode);
      }
      break;
    }
    itRefNode = itRunner;
    ++itRunner;
  }

  return retval;
}
#endif

void CConnection::setSource(CFunctionBlock *paSrcFB, TPortId paSrcPortId){
  mSourceId.mFB = paSrcFB;
  mSourceId.mPortId = paSrcPortId;
}

bool CConnection::dstExists(const SConnectionPoint &paDestPoint){
  bool bRetVal = false;
  for(TDestinationIdList::Iterator it = mDestinationIds.begin();
      it != mDestinationIds.end(); ++it){
    if(paDestPoint == (*it)){
      bRetVal = true;
      break;
    }
  }
  return bRetVal;
}


