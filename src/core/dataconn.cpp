/*******************************************************************************
 * Copyright (c) 2005, 2023 Profactor GmbH, ACIN, fortiss GmbH
 *                          Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Thomas Strasser, Alois Zoitl, Rene Smodic, Gunnar Grabmaier, Ingo Hegny,
 *    Matthias Plasch
 *      - initial implementation and rework communication infrastructure
 *    Martin Jobst - move allocation of data value to FB
 *******************************************************************************/
#include "dataconn.h"
#include "funcbloc.h"
#include "mgmcmd.h"
#include "negdataconn.h"

USE_STRING_ID(NOT)

CDataConnection::CDataConnection(CFunctionBlock &paSrcFB, const TPortId paSrcPortId)
        : CConnection(paSrcFB, paSrcPortId) {
}

EMGMResponse CDataConnection::connect(CFunctionBlock &paDstFB,
    CStringDictionary::TStringId paDstPortNameId){
  EMGMResponse retVal = EMGMResponse::NoSuchObject;

  const TPortId dstPortId = paDstFB.getDIID(paDstPortNameId);
  if(cgInvalidPortId != dstPortId){
    CIEC_ANY *dstDataPoint = paDstFB.getDIFromPortId(dstPortId);
    retVal = CDataConnection::establishDataConnection(paDstFB, dstPortId, *dstDataPoint);
    if(retVal == EMGMResponse::Ready) {
      getSourceId().getFB().incConnRefCount();
    }
  }
  return retVal;
}

EMGMResponse CDataConnection::connectToCFBInterface(CFunctionBlock &paDstFB,
    CStringDictionary::TStringId paDstPortNameId){
  EMGMResponse retVal = EMGMResponse::NoSuchObject;
  TPortId nDOID = paDstFB.getDOID(paDstPortNameId);

  if(cgInvalidEventID != nDOID){
    CIEC_ANY *dstDataPoint = paDstFB.getDataOutput(paDstPortNameId);
    nDOID |= cgInternal2InterfaceMarker;
    retVal = establishDataConnection(paDstFB, nDOID, *dstDataPoint);
  }

  return retVal;
}

void CDataConnection::handleAnySrcPortConnection(const CIEC_ANY &paDstDataPoint){
  if(CIEC_ANY::e_ANY != paDstDataPoint.unwrap().getDataTypeID()){
    getValue().setValue(paDstDataPoint.unwrap());
    getSourceId().getFB().configureGenericDO(getSourceId().getPortId(), paDstDataPoint);
  }
}

EMGMResponse CDataConnection::disconnect(CFunctionBlock &paDstFB, CStringDictionary::TStringId paDstPortNameId) {
  if (paDstFB.getDIConnection(paDstPortNameId) != this) {
    return EMGMResponse::NoSuchObject;
  }

  const TPortId dstPortId = paDstFB.getDIID(paDstPortNameId);
  if (cgInvalidPortId == dstPortId) {
    return EMGMResponse::NoSuchObject;
  }
  paDstFB.connectDI(dstPortId, nullptr);
  getSourceId().getFB().decConnRefCount();
  return EMGMResponse::Ready;
}

bool CDataConnection::canBeConnected(const CIEC_ANY &paSrcDataPoint,
    const CIEC_ANY &paDstDataPoint){
  CIEC_ANY::EDataTypeID eSrcId = paSrcDataPoint.getDataTypeID();
  CIEC_ANY::EDataTypeID eDstId = paDstDataPoint.getDataTypeID();
  bool bCanConnect = false;

  if(eSrcId == eDstId){
    bCanConnect = true;
  }
  else{
    if(((eSrcId == CIEC_ANY::e_ANY) && (eDstId != CIEC_ANY::e_ANY))
        || ((eSrcId != CIEC_ANY::e_ANY) && (eDstId == CIEC_ANY::e_ANY))){
      bCanConnect = true;
    } else {
      bCanConnect = CIEC_ANY::isCastable(eSrcId, eDstId);
    }
  }
  return bCanConnect;
}

EMGMResponse CDataConnection::establishDataConnection(CFunctionBlock &paDstFB,
                                                      const TPortId paDstPortId,
                                                      const CIEC_ANY &paDstDataPoint) {
  if (getValue().getDataTypeID() == CIEC_ANY::e_ANY) {
    handleAnySrcPortConnection(paDstDataPoint);
  } else if (!canBeConnected(getValue(), paDstDataPoint)) {
    return EMGMResponse::InvalidOperation;
  }

  if (!paDstFB.connectDI(paDstPortId, this)) {
    return EMGMResponse::InvalidState;
  }
  return EMGMResponse::Ready;
}

CConnection::Wrapper CDataConnection::getDelegatingConnection(forte::core::TNameIdentifier &paSrcNameList) {
  switch (getValue().getDataTypeID()) {
    case CIEC_ANY::e_BOOL:
      if (paSrcNameList.size() == 1 && paSrcNameList.front() == STRID(NOT)) {
        return make_delegating<forte::core::internal::CNegatingDataConnection>(
          getSourceId().getFB(), getSourceId().getPortId(),
          static_cast<CIEC_BOOL &>(getValue()));
      }
    default: break;
  }
  return CConnection::getDelegatingConnection(paSrcNameList);
}
