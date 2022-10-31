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
 *    Thomas Strasser, Alois Zoitl, Rene Smodic, Gunnar Grabmaier, Ingo Hegny,
 *    Matthias Plasch
 *      - initial implementation and rework communication infrastructure
 *******************************************************************************/
#include "dataconn.h"
#include "funcbloc.h"

CDataConnection::CDataConnection(CFunctionBlock *paSrcFB, TPortId paSrcPortId,
    const CIEC_ANY *paSrcDO) :
    CConnection(paSrcFB, paSrcPortId),
        m_poValue(nullptr),
        mSpecialCastConnection(false){

  if((nullptr != paSrcDO) && (CIEC_ANY::e_ANY != paSrcDO->getDataTypeID())){
    m_poValue = paSrcDO->clone(m_acDataBuf);
  }
}

CDataConnection::~CDataConnection(){
  if(nullptr != m_poValue){
    m_poValue->~CIEC_ANY();
  }
}

EMGMResponse CDataConnection::connect(CFunctionBlock *paDstFB,
    CStringDictionary::TStringId paDstPortNameId){
  EMGMResponse retVal = EMGMResponse::NoSuchObject;

  TPortId dstPortId = paDstFB->getDIID(paDstPortNameId);
  if(cg_unInvalidPortId != dstPortId){
    CIEC_ANY *dstDataPoint = paDstFB->getDIFromPortId(dstPortId);
    retVal = establishDataConnection(paDstFB, dstPortId, dstDataPoint);
  }
  return retVal;
}

EMGMResponse CDataConnection::connectToCFBInterface(CFunctionBlock *paDstFB,
    CStringDictionary::TStringId paDstPortNameId){
  EMGMResponse retVal = EMGMResponse::NoSuchObject;
  TPortId nDOID = paDstFB->getDOID(paDstPortNameId);

  if(cg_nInvalidEventID != nDOID){
    CIEC_ANY *dstDataPoint = paDstFB->getDataOutput(paDstPortNameId);
    nDOID |= cgInternal2InterfaceMarker;
    retVal = establishDataConnection(paDstFB, nDOID, dstDataPoint);
  }

  return retVal;
}

void CDataConnection::handleAnySrcPortConnection(const CIEC_ANY &paDstDataPoint){
  if(CIEC_ANY::e_ANY != paDstDataPoint.getDataTypeID()){
    m_poValue = paDstDataPoint.clone(m_acDataBuf);
    getSourceId().mFB->configureGenericDO(getSourceId().mPortId, paDstDataPoint);
    if(isConnected()){
      //We already have some connection also set their correct type
      for(TDestinationIdList::Iterator it = mDestinationIds.begin();
          it != mDestinationIds.end(); ++it){
        it->mFB->connectDI(it->mPortId, this);
      }
    }
  }
}

EMGMResponse
CDataConnection::disconnect(CFunctionBlock *paDstFB, CStringDictionary::TStringId paDstPortNameId){
  EMGMResponse retval = EMGMResponse::NoSuchObject;
  TPortId dstPortId = paDstFB->getDIID(paDstPortNameId);

  if(cg_unInvalidPortId != dstPortId){
    retval = CConnection::removeDestination(CConnectionPoint(paDstFB, dstPortId));
    if(EMGMResponse::Ready == retval){
      // the CConnection class didn't respond an error
      paDstFB->connectDI(dstPortId, nullptr);
    }
  }
  return retval;
}

void CDataConnection::readData(CIEC_ANY *pa_poValue) const{
  if(m_poValue){
    if(!mSpecialCastConnection){
      pa_poValue->setValue(*m_poValue);
    }
    else{
      CIEC_ANY::specialCast(*m_poValue, *pa_poValue);
    }
  }
}

bool CDataConnection::canBeConnected(const CIEC_ANY *pa_poSrcDataPoint,
    const CIEC_ANY *pa_poDstDataPoint, bool &pa_rbSpecialCast){
  CIEC_ANY::EDataTypeID eSrcId = pa_poSrcDataPoint->getDataTypeID();
  CIEC_ANY::EDataTypeID eDstId = pa_poDstDataPoint->getDataTypeID();
  bool bCanConnect = false;

  if(eSrcId == eDstId){
    bCanConnect = true;
  }
  else{
    if(((eSrcId == CIEC_ANY::e_ANY) && (eDstId != CIEC_ANY::e_ANY))
        || ((eSrcId != CIEC_ANY::e_ANY) && (eDstId == CIEC_ANY::e_ANY))){
      bCanConnect = true;
    }
    else{
      bCanConnect = CIEC_ANY::isCastable(eSrcId, eDstId);
      pa_rbSpecialCast = ((bCanConnect) && needsSpecialCast(eSrcId));
    }
  }
  return bCanConnect;
}

bool CDataConnection::needsSpecialCast(CIEC_ANY::EDataTypeID pa_eSrcDTId){
  //we want to cast from floating point data to an integer data type
  return ((CIEC_ANY::e_LREAL == pa_eSrcDTId) || (CIEC_ANY::e_REAL == pa_eSrcDTId));
}

EMGMResponse CDataConnection::establishDataConnection(CFunctionBlock *paDstFB, TPortId paDstPortId,
    CIEC_ANY *paDstDataPoint){
  EMGMResponse retVal = EMGMResponse::InvalidOperation;

  if(nullptr == m_poValue){
    handleAnySrcPortConnection(*paDstDataPoint);
    retVal = EMGMResponse::Ready;
  }
  else{
    if(canBeConnected(m_poValue, paDstDataPoint, mSpecialCastConnection)){
      retVal = EMGMResponse::Ready;
    }
  }

  if(EMGMResponse::Ready == retVal){
    retVal = CConnection::addDestination(CConnectionPoint(paDstFB, paDstPortId));
    if(EMGMResponse::Ready == retVal && !paDstFB->connectDI(paDstPortId, this)) {
      retVal = EMGMResponse::InvalidState;
      mDestinationIds.popFront(); //empty the list so that the have created connection is not here anymore
    }
  }
  return retVal;
}
