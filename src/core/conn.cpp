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
 *    Thomas Strasser, Alois Zoitl, Ingo Hegny,
 *      - initial implementation and rework communication infrastructure
 *******************************************************************************/
#include <string.h>
#include "conn.h"

CConnection::CConnection(CFunctionBlock *paSrcFB, TPortId paSrcPortId) :
    mSourceId(paSrcFB, paSrcPortId){
}

EMGMResponse CConnection::addDestination(const CConnectionPoint &paDestPoint){
  EMGMResponse retval = e_INVALID_STATE;

  if(!dstExists(paDestPoint)){ // check if there is up to now no such fan out with this destination
    mDestinationIds.pushBack(paDestPoint);
    retval = e_RDY;
  }
  return retval;
}

EMGMResponse CConnection::removeDestination(const CConnectionPoint &paDestPoint){
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

void CConnection::setSource(CFunctionBlock *paSrcFB, TPortId paSrcPortId){
  mSourceId.mFB = paSrcFB;
  mSourceId.mPortId = paSrcPortId;
}

bool CConnection::dstExists(const CConnectionPoint& paDestPoint) const {
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


