/*******************************************************************************
 * Copyright (c) 2005, 2024 Profactor GmbH, ACIN, fortiss GmbH,
 *                          Johannes Kepler University Linz
 *                          Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Thomas Strasser, Alois Zoitl, Gunnar Grabmaier, Gerhard Ebenhofer,
 *    Ingo Hegny
 *      - initial implementation and rework communication infrastructure
 *    Alois Zoitl - added support for adapter connections in CFBs
 *    Martin Jobst - add smart pointer for internal FBs
 *******************************************************************************/
#include <string.h>
#include "cfb.h"
#include "adapter.h"
#include "resource.h"
#include "core/util/criticalregion.h"
#include "if2indco.h"
#include "adapterconn.h"

CCompositeFB::CCompositeFB(forte::core::CFBContainer &paContainer, const SFBInterfaceSpec& paInterfaceSpec,
                           CStringDictionary::TStringId paInstanceNameId, const SCFB_FBNData & paFBNData) :
        CFunctionBlock(paContainer, paInterfaceSpec, paInstanceNameId),
        cmFBNData(paFBNData),
        mEventConnections(nullptr),
        mDataConnections(nullptr) {
}

bool CCompositeFB::initialize() {
  if(!CFunctionBlock::initialize()) {
    return false;
  }

  if(!createInternalFBs()){
    return false;
  }

  createEventConnections();
  createDataConnections();
  createAdapterConnections();
  setFBNetworkInitialValues();

  //remove adapter-references for CFB
  for(TPortId i = 0; i < getFBInterfaceSpec().mNumAdapters; i++){
    if(CAdapter* adapter = getAdapterUnchecked(i); adapter != nullptr) {
      adapter->setParentFB(nullptr, 0);
    }
  }
  return true;
}

CCompositeFB::~CCompositeFB() {
  if (cmFBNData.mNumEventConnections) {
    delete[] mEventConnections;
  }

  if (cmFBNData.mNumDataConnections) {
    if (nullptr != mDataConnections) {
      delete[] mDataConnections;
    }
  }
}

bool CCompositeFB::connectDI(TPortId paDIPortId, CDataConnection *paDataCon){
  // TODO remove?
  if(cgInternal2InterfaceMarker & paDIPortId){
    paDIPortId = static_cast<TPortId>(paDIPortId & cgInternal2InterfaceRemovalMask);
    return paDIPortId < getFBInterfaceSpec().mNumDOs;
  }
  return CFunctionBlock::connectDI(paDIPortId, paDataCon);
}

bool CCompositeFB::configureGenericDO(TPortId paDOPortId, const CIEC_ANY &paRefValue){
  // TODO remove?
  return CFunctionBlock::configureGenericDO(paDOPortId, paRefValue);
}

EMGMResponse CCompositeFB::changeExecutionState(EMGMCommandType paCommand){
  EMGMResponse nRetVal = CFunctionBlock::changeExecutionState(paCommand);

  //Update FB parameters that maybe got overwritten by default values of the FB
  if((EMGMCommandType::Reset == paCommand) && (E_FBStates::Idle == getState())){
    setFBNetworkInitialValues();
  }
  return nRetVal;
}

CIEC_ANY *CCompositeFB::getVar(CStringDictionary::TStringId *paNameList, unsigned int paNameListSize){
  CIEC_ANY *retVal = nullptr;

  if(1 > paNameListSize){
    CFunctionBlock *child = forte::core::CFBContainer::getFB(*paNameList);
    if(child != nullptr){
      paNameList++;
      paNameListSize--;
      retVal = child->getVar(paNameList, paNameListSize);
    }
  } else {
    retVal = CFunctionBlock::getVar(paNameList, paNameListSize);
  }

  return retVal;
}

void CCompositeFB::executeEvent(TEventID paEIID, CEventChainExecutionThread * const paECET) {
  if(cgInternal2InterfaceMarker & paEIID) {
    TEventID internalEvent = static_cast<TEventID>(paEIID & cgInternal2InterfaceRemovalMask);
    sendOutputEvent(internalEvent, paECET);
  } else {
    if(paEIID < getFBInterfaceSpec().mNumEIs && mInterface2InternalEventCons[paEIID]) {
      mInterface2InternalEventCons[paEIID]->triggerEvent(paECET);
    }
  }
}

void CCompositeFB::createEventConnections(){
  prepareIf2InEventCons();  //the interface to internal event connections are needed even if they are not connected therefore we have to create them correctly in any case
  if(0 != cmFBNData.mNumEventConnections){
    mEventConnections = new CEventConnection *[cmFBNData.mNumEventConnections]; //TODO for a major revison this list could be ommited but requires a change in the faned out connections

    for(size_t i = 0; i < cmFBNData.mNumEventConnections; ++i){
      const SCFB_FBConnectionData *currentConn = &(cmFBNData.mEventConnections[i]);
      //FIXME implement way to inform FB creator that creation failed
      CFunctionBlock *srcFB = getFunctionBlock(currentConn->mSrcFBNum);
      CFunctionBlock *dstFB = getFunctionBlock(currentConn->mDstFBNum);

      if((nullptr != srcFB) && (nullptr != dstFB)){
        if(this == srcFB){
          mEventConnections[i] =
              mInterface2InternalEventCons[getEIID(currentConn->mSrcId)].get();
        }
        else{
          mEventConnections[i] = srcFB->getEOConnection(currentConn->mSrcId);
        }
        establishConnection(mEventConnections[i], dstFB, currentConn->mDstId);
      }
      else{
        DEVLOG_ERROR("Could not create event connection in CFB");
      }
    }
    //now handle the fanned out connections
    for(size_t i = 0; i < cmFBNData.mNumFannedOutEventConnections; ++i){
      const SCFB_FBFannedOutConnectionData *currentFannedConn = &(cmFBNData.mFannedOutEventConnections[i]);
      CFunctionBlock *dstFB = getFunctionBlock(currentFannedConn->mDstFBNum);
      establishConnection(mEventConnections[currentFannedConn->mConnectionNum], dstFB, currentFannedConn->mDstId);
    }
  }
}

void CCompositeFB::prepareIf2InEventCons(){
  mInterface2InternalEventCons.reserve(getFBInterfaceSpec().mNumEIs);
  for(TPortId i = 0; i < getFBInterfaceSpec().mNumEIs; i++){
    mInterface2InternalEventCons.emplace_back(std::make_unique<CEventConnection>(this, i));
  }
}

void CCompositeFB::establishConnection(CConnection *paCon, CFunctionBlock *paDstFb,
    CStringDictionary::TStringId paDstNameId) {
  if(this == paDstFb){
    paCon->connectToCFBInterface(paDstFb, paDstNameId);
  }
  else{
    paCon->connect(paDstFb, paDstNameId);
  }
}

void CCompositeFB::createDataConnections(){
  if(cmFBNData.mNumDataConnections){
    mDataConnections = new CDataConnection *[cmFBNData.mNumDataConnections];

    for(size_t i = 0; i < cmFBNData.mNumDataConnections; ++i){
      const SCFB_FBConnectionData &currentConn(cmFBNData.mDataConnections[i]);
      //FIXME implement way to inform FB creator that creation failed
      CFunctionBlock *srcFB = getFunctionBlock(currentConn.mSrcFBNum);
      CFunctionBlock *dstFB = getFunctionBlock(currentConn.mDstFBNum);

      if((nullptr != srcFB) && (nullptr != dstFB)){
        mDataConnections[i] = getDataConn(srcFB, currentConn.mSrcId);
        establishConnection(mDataConnections[i], dstFB, currentConn.mDstId);
      }
      else{
        DEVLOG_ERROR("Could not create data connection in CFB");
      }
    }
    //now handle the fanned out connections
    for(size_t i = 0; i < cmFBNData.mNumFannedOutDataConnections; ++i){
      const SCFB_FBFannedOutConnectionData &currentFannedConn(cmFBNData.mFannedOutDataConnections[i]);
      CFunctionBlock *dstFB = getFunctionBlock(currentFannedConn.mDstFBNum);
      establishConnection(mDataConnections[currentFannedConn.mConnectionNum], dstFB, currentFannedConn.mDstId);
    }
  }
}

CDataConnection * CCompositeFB::getDataConn(CFunctionBlock *paSrcFB, CStringDictionary::TStringId paSrcNameId){
  if(this == paSrcFB){
    TPortId diId = getDIID(paSrcNameId);
    if(diId != cgInvalidPortId) {
      return getIf2InConUnchecked(diId);
    } else {
      TPortId dioId = getDIOID(paSrcNameId);
      return getDIOOutConInternalUnchecked(dioId);
    }
  }

  CDataConnection *con = paSrcFB->getDOConnection(paSrcNameId);
  if(con == nullptr) {
    con = paSrcFB->getDIOOutConnection(paSrcNameId);
  }
  return con;
}

void CCompositeFB::createAdapterConnections(){
  for(size_t i = 0; i < cmFBNData.mNumAdapterConnections; ++i){
    const SCFB_FBConnectionData &currentConn(cmFBNData.mAdapterConnections[i]);
    //FIXME implement way to inform FB creator that creation failed
    const CFunctionBlock *const srcFB = getFunctionBlock(currentConn.mSrcFBNum);
    CFunctionBlock *const dstFB = getFunctionBlock(currentConn.mDstFBNum);

    if((nullptr != srcFB) && (nullptr != dstFB)){
      const CAdapter *const adp = srcFB->getAdapter(currentConn.mSrcId);
      if((nullptr != adp) && (adp->isPlug())){
        //only plugs hold the connection object
        adp->getAdapterConnection()->connect(dstFB, currentConn.mDstId);
      } else{
        DEVLOG_ERROR("[CFB Creation] Adapter source is not a plug!");
      }
    } else{
      DEVLOG_ERROR("[CFB Creation] Source or destination not found in adapter connection!");
    }
  }
}

void CCompositeFB::setFBNetworkInitialValues() {
  for(size_t i = 0; i < cmFBNData.mNumParams; ++i){
    const SCFB_FBParameter &currentParam(cmFBNData.mParams[i]);
    CFunctionBlock *child = getFunctionBlock(currentParam.mFBNum);
    if(child != nullptr){
      CIEC_ANY *di = child->getDataInput(currentParam.mDINameID);
      if(di != nullptr){
        di->fromString(currentParam.mParamValue);
      }
      else{
        DEVLOG_ERROR("Could not get data input for setting a parameter");
      }
    } else {
      DEVLOG_ERROR("Could not get child FB for setting a parameter");
    }
  }
}

CFunctionBlock *CCompositeFB::getFunctionBlock(int paFBNum){
  if(-1 == paFBNum){
    return this;
  }
  if(0 <= paFBNum)  {
    TForteUInt32 fbNum = static_cast<TForteUInt32>(paFBNum);
    if(scmAdapterMarker == (scmAdapterMarker & fbNum)){
      fbNum &= scmAdapterFBRange;
      if(fbNum < getFBInterfaceSpec().mNumAdapters){
        return getAdapterUnchecked(fbNum);
      }
    } else{
      if(fbNum < cmFBNData.mNumFBs){
        return forte::core::CFBContainer::getFB(cmFBNData.mFBInstances[fbNum].mFBInstanceNameId);
      }
    }
  }
  return nullptr;
}
