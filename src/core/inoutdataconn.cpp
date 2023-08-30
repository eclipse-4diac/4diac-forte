/*******************************************************************************
 * Copyright (c) 2023 Primetals Technologies Austria GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Martin Melik Merkumians
 *      - initial implementation
 *******************************************************************************/

#include "inoutdataconn.h"

#include "funcbloc.h"

CInOutDataConnection::CInOutDataConnection(CFunctionBlock *paSrcFB, TPortId paSrcPortId, CIEC_ANY *paValue) : CDataConnection(paSrcFB, paSrcPortId, paValue) {
}

EMGMResponse CInOutDataConnection::connect(CFunctionBlock *paDstFB,
    CStringDictionary::TStringId paDstPortNameId){
  //Check if the superclass connect is working (connection with plain IN)
  EMGMResponse retVal = CDataConnection::connect(paDstFB, paDstPortNameId);

  if(retVal != EMGMResponse::NoSuchObject) {
    return retVal; // we already have a connection
  }

  TPortId dstPortId = paDstFB->getDIOID(paDstPortNameId);
  if(cgInvalidPortId != dstPortId){
    CIEC_ANY *dstDataPoint = paDstFB->getDIOFromPortId(dstPortId);
    retVal = establishDataConnection(paDstFB, dstPortId, dstDataPoint);
  }
  
  return retVal;
}

EMGMResponse CInOutDataConnection::disconnect(CFunctionBlock *paDstFB, CStringDictionary::TStringId paDstPortNameId) {
  EMGMResponse retVal = CDataConnection::disconnect(paDstFB, paDstPortNameId);

  if(retVal != EMGMResponse::NoSuchObject) {
    return retVal; // we already have a connection
  }

  TPortId dstPortId = paDstFB->getDIOID(paDstPortNameId);

  if(cgInvalidPortId != dstPortId){
    retVal = CConnection::removeDestination(CConnectionPoint(paDstFB, dstPortId, EConnectionDestinationType::e_INOUT));
    if(EMGMResponse::Ready == retVal){
      // the CConnection class didn't respond an error
      paDstFB->connectDIO(dstPortId, nullptr);
    }
  }
  return retVal;
}

void CInOutDataConnection::setValue(CIEC_ANY *paValue) {
  mValue = paValue;
  const auto destinationList = getDestinationList();
  for(auto connectionPoint : destinationList) {
    if(connectionPoint.getConnectionDestinationType() == EConnectionDestinationType::e_INOUT) {
      connectionPoint.mFB->connectDIO(connectionPoint.mPortId, this);
    } 
  }
}

EMGMResponse CInOutDataConnection::establishDataConnection(CFunctionBlock *paDstFB, TPortId paDstPortId,
    CIEC_ANY *paDstDataPoint) {
  EMGMResponse retVal = EMGMResponse::InvalidOperation;

  if(mValue) {
    if (mValue->getDataTypeID() == CIEC_ANY::e_ANY) {
      handleAnySrcPortConnection(*paDstDataPoint);
      retVal = EMGMResponse::Ready;
    } else {
      if (canBeConnected(mValue, paDstDataPoint)) {
        retVal = EMGMResponse::Ready;
      }
    }
  }

  if(EMGMResponse::Ready == retVal){
    retVal = CConnection::addDestination(CConnectionPoint(paDstFB, paDstPortId, EConnectionDestinationType::e_INOUT));
    if(EMGMResponse::Ready == retVal && !paDstFB->connectDIO(paDstPortId, this)) {
      retVal = EMGMResponse::InvalidState;
      mDestinationIds.pop_back(); //remove the newly created connection from the list
    }
  }
  return retVal;
}

bool CInOutDataConnection::isConnected() const {
  return true;
};
