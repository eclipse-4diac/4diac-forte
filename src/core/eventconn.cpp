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

CEventConnection::CEventConnection(CFunctionBlock &paSrcFB, const TPortId paSrcPortId) :
    CConnection(paSrcFB, paSrcPortId){
}

CEventConnection::~CEventConnection() = default;

EMGMResponse CEventConnection::connect(CFunctionBlock &paDstFB, CStringDictionary::TStringId paDstPortNameId){
  EMGMResponse retval = EMGMResponse::NoSuchObject;
  const TPortId nEIID = paDstFB.getEIID(paDstPortNameId);

  if(cgInvalidEventID != nEIID){
    retval = addDestination(CConnectionPoint(paDstFB, nEIID));
    paDstFB.addInputEventConnection(nEIID);
  }
  return retval;
}

EMGMResponse CEventConnection::connectToCFBInterface(CFunctionBlock &paDstFB, CStringDictionary::TStringId paDstPortNameId){
  EMGMResponse retval = EMGMResponse::NoSuchObject;
  TPortId nEOID = paDstFB.getEOID(paDstPortNameId);

  if(cgInvalidEventID != nEOID){
    nEOID |= cgInternal2InterfaceMarker;
    retval = addDestination(CConnectionPoint(paDstFB, nEOID));
  }
  return retval;
}

EMGMResponse CEventConnection::disconnect(CFunctionBlock &paDstFB, CStringDictionary::TStringId paDstPortNameId){
  EMGMResponse retval = EMGMResponse::NoSuchObject;
  const TEventID nEIID = paDstFB.getEIID(paDstPortNameId);

  if(cgInvalidEventID != nEIID){
    retval = removeDestination(CConnectionPoint(paDstFB, nEIID));
    paDstFB.removeInputEventConnection(nEIID);
  }
  return retval;
}

void CEventConnection::triggerEvent(CEventChainExecutionThread *paExecEnv) const {
  if(paExecEnv != nullptr){
    for(const auto& runner : mDestinationIds){
      paExecEnv->addEventEntry(runner);
    }
  }
}

EMGMResponse CEventConnection::addDestination(const CConnectionPoint &paDestPoint) {
  if (std::find(mDestinationIds.begin(), mDestinationIds.end(), paDestPoint) != mDestinationIds.end()) {
    return EMGMResponse::InvalidState;
  }
  mDestinationIds.push_back(paDestPoint);
  return EMGMResponse::Ready;
}

EMGMResponse CEventConnection::removeDestination(const CConnectionPoint &paDestPoint) {
  auto it = std::remove(mDestinationIds.begin(), mDestinationIds.end(), paDestPoint);
  if (it == mDestinationIds.end()) {
    return EMGMResponse::InvalidState;
  }
  mDestinationIds.erase(it, mDestinationIds.end());
  return EMGMResponse::Ready;
}

