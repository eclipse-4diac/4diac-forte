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
#include "conn.h"
#include <algorithm>

CConnection::CConnection(CFunctionBlock *paSrcFB, TPortId paSrcPortId) :
    mSourceId(paSrcFB, paSrcPortId) {

}

EMGMResponse CConnection::addDestination(const CConnectionPoint &paDestPoint){
  EMGMResponse retval = EMGMResponse::InvalidState;

  if(!dstExists(paDestPoint)){ // check if there is up to now no such fan out with this destination
    mDestinationIds.push_back(paDestPoint);
    retval = EMGMResponse::Ready;
  }
  return retval;
}

EMGMResponse CConnection::removeDestination(const CConnectionPoint &paDestPoint){
  auto it = std::remove(mDestinationIds.begin(), mDestinationIds.end(), paDestPoint);
  if(it != mDestinationIds.end()){
    mDestinationIds.erase(it, mDestinationIds.end());
    return EMGMResponse::Ready;
  }
  return EMGMResponse::InvalidState;
}

void CConnection::setSource(CFunctionBlock *paSrcFB, TPortId paSrcPortId){
  mSourceId.mFB = paSrcFB;
  mSourceId.mPortId = paSrcPortId;
}

bool CConnection::dstExists(const CConnectionPoint& paDestPoint) const {
  for(const auto& runner : mDestinationIds){
    if(runner == paDestPoint){
      return true;
    }
  }
  return false;
}


