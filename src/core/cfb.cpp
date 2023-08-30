/*******************************************************************************
 * Copyright (c) 2005, 2023 Profactor GmbH, ACIN, fortiss GmbH,
 *                          Johannes Kepler University Linz
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
 *******************************************************************************/
#include <string.h>
#include "cfb.h"
#include "adapter.h"
#include "resource.h"
#include "utils/criticalregion.h"
#include "if2indco.h"
#include "adapterconn.h"

CCompositeFB::CCompositeFB(CResource *paSrcRes, const SFBInterfaceSpec *paInterfaceSpec,
                           CStringDictionary::TStringId paInstanceNameId, const SCFB_FBNData * paFBNData) :
        CFunctionBlock(paSrcRes, paInterfaceSpec, paInstanceNameId),
        mIf2InDConns(nullptr),
        mIn2IfDConns(nullptr),
        cmFBNData(paFBNData),
        mEventConnections(nullptr),
        mDataConnections(nullptr),
        mInterface2InternalEventCons(nullptr){
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
  setParams();

  //remove adapter-references for CFB
  for(TPortId i = 0; i < mInterfaceSpec->mNumAdapters; i++){
    if(nullptr != mAdapters){
      static_cast<CAdapter*>(mAdapters[i])->setParentFB(nullptr, 0);
    }
  }
  return true;
}

CCompositeFB::~CCompositeFB(){
  if(cmFBNData->mNumFBs){
    for(unsigned int i = 0; i < cmFBNData->mNumFBs; ++i){
      delete mInternalFBs[i];
    }
    delete[] mInternalFBs;
  }

  //only delete the interface to internal event connections all other connections are managed by their source's FBs
  //this has to be done even if we don't have any event connection to ensure correct behavior
  for(unsigned int i = 0; i < mInterfaceSpec->mNumEIs; ++i){
    delete mInterface2InternalEventCons[i];
  }
  delete[] mInterface2InternalEventCons;

  if(cmFBNData->mNumEventConnections){
    delete[] mEventConnections;
  }

  if(cmFBNData->mNumDataConnections){
    if(nullptr != mDataConnections){
      delete[] mDataConnections;
    }
    if(nullptr != mIf2InDConns){
      delete[] mIf2InDConns;
    }
    if(nullptr != mIn2IfDConns){
      delete[] mIn2IfDConns;
    }
  }
}

bool CCompositeFB::connectDI(TPortId paDIPortId, CDataConnection *paDataCon){
  bool retVal = false;

  if(cgInternal2InterfaceMarker & paDIPortId){
    paDIPortId = static_cast<TPortId>(paDIPortId & cgInternal2InterfaceRemovalMask);
    if(paDIPortId < mInterfaceSpec->mNumDOs){
      mIn2IfDConns[paDIPortId] = paDataCon;
      retVal = true;
    }
  }
  else if(paDIPortId < mInterfaceSpec->mNumDIs){
    bool needsCloning = (getDI(paDIPortId)->getDataTypeID() == CIEC_ANY::e_ANY);
    retVal = CFunctionBlock::connectDI(paDIPortId, paDataCon);
    if((true == retVal) && (true == needsCloning) && (nullptr != paDataCon)
        && (nullptr != paDataCon->getValue())){
      //if internal connected update the connections data type and maybe internal further connection points
      (mIf2InDConns + paDIPortId)->setValue(getDI(paDIPortId));
      (mIf2InDConns + paDIPortId)->cloneInputInterfaceValue();
    }
  }
  return retVal;
}

bool CCompositeFB::configureGenericDO(TPortId paDOPortId, const CIEC_ANY &paRefValue){
  bool bRetVal = CFunctionBlock::configureGenericDO(paDOPortId, paRefValue);
  if(bRetVal && nullptr != mIn2IfDConns[paDOPortId]){
    //issue a reconfiguration attempt so that all connection end points in this connection are also correctly configured
    mIn2IfDConns[paDOPortId]->connectToCFBInterface(this, paDOPortId);
  }
  return bRetVal;
}

EMGMResponse CCompositeFB::changeFBExecutionState(EMGMCommandType paCommand){
  EMGMResponse nRetVal = CFunctionBlock::changeFBExecutionState(paCommand);
  if (EMGMResponse::Ready == nRetVal) {
    nRetVal = changeInternalFBExecutionState(paCommand, cmFBNData->mNumFBs, mInternalFBs);
  }
  //Update FB parameters that maybe got overwritten by default values of the FB
  if((EMGMCommandType::Reset == paCommand) && (E_FBStates::Idle == getState())){
    setParams();
  }
  return nRetVal;
}

#ifdef FORTE_SUPPORT_MONITORING

CFunctionBlock *CCompositeFB::getFB(forte::core::TNameIdentifier::CIterator &paNameListIt){
  CFunctionBlock *retVal = nullptr;
  for(unsigned int i = 0; i < cmFBNData->mNumFBs; ++i){
    if(mInternalFBs[i]->getInstanceNameId() == *paNameListIt){
      if(paNameListIt.isLastEntry()){
        retVal = mInternalFBs[i];
      } else {
         //we are looking for a child of this fB
        ++paNameListIt;
        retVal = mInternalFBs[i]->getFB(paNameListIt);
      }
    }
  }
  if(nullptr == retVal){
    //check if it is an adapter of the function block
    retVal = CFunctionBlock::getFB(paNameListIt);
  }
  return retVal;
}

#endif

CIEC_ANY *CCompositeFB::getVar(CStringDictionary::TStringId *paNameList,
    unsigned int paNameListSize){
  CIEC_ANY *retVal = nullptr;

  if(1 > paNameListSize){
    for(unsigned int i = 0; i < cmFBNData->mNumFBs; ++i){
      if(*paNameList == mInternalFBs[i]->getInstanceNameId()){
        paNameList++;
        paNameListSize--;
        retVal = mInternalFBs[i]->getVar(paNameList, paNameListSize);
        break;
      }
    }
  }
  else{
    retVal = CFunctionBlock::getVar(paNameList, paNameListSize);
  }

  return retVal;
}

void CCompositeFB::executeEvent(TEventID paEIID, CEventChainExecutionThread * const paECET){
  if(cgInternal2InterfaceMarker & paEIID){
    TEventID internalEvent = static_cast<TEventID>(paEIID & cgInternal2InterfaceRemovalMask);
    readInternal2InterfaceOutputData(internalEvent);
    sendOutputEvent(internalEvent, paECET);
  }
  else{
    if(paEIID < mInterfaceSpec->mNumEIs && nullptr != mInterface2InternalEventCons[paEIID]){
      mInterface2InternalEventCons[paEIID]->triggerEvent(paECET);
    }
  }
}

bool CCompositeFB::createInternalFBs(){
  if(cmFBNData->mNumFBs){
    mInternalFBs = new TFunctionBlockPtr[cmFBNData->mNumFBs];
    for(unsigned int i = 0; i < cmFBNData->mNumFBs; ++i){
      const SCFB_FBInstanceData &cfbInstanceData(cmFBNData->mFBInstances[i]);
      mInternalFBs[i] = CTypeLib::createFB(cfbInstanceData.mFBInstanceNameId, cfbInstanceData.mFBTypeNameId, getResourcePtr());
      if(mInternalFBs[i] == nullptr){
        DEVLOG_ERROR("Cannot create internal FB (name: %s, type: %s) in CFB (type: %s)!\n",
            CStringDictionary::getInstance().get(cfbInstanceData.mFBInstanceNameId),
            CStringDictionary::getInstance().get(cfbInstanceData.mFBTypeNameId),
            getFBTypeName());
        return false;
      }
    }
  }
  return true;
}

void CCompositeFB::createEventConnections(){
  prepareIf2InEventCons();  //the interface to internal event connections are needed even if they are not connected therefore we have to create them correctly in any case
  if(0 != cmFBNData->mNumEventConnections){
    mEventConnections = new CEventConnection *[cmFBNData->mNumEventConnections]; //TODO for a major revison this list could be ommited but requires a change in the faned out connections

    CFunctionBlock *poSrcFB;
    CFunctionBlock *poDstFB;
    for(unsigned int i = 0; i < cmFBNData->mNumEventConnections; ++i){
      const SCFB_FBConnectionData *cpstCurrentConn = &(cmFBNData->mEventConnections[i]);
      //FIXME implement way to inform FB creator that creation failed
      poSrcFB = getFunctionBlock(cpstCurrentConn->mSrcFBNum);
      poDstFB = getFunctionBlock(cpstCurrentConn->mDstFBNum);

      if((nullptr != poSrcFB) && (nullptr != poDstFB)){
        if(this == poSrcFB){
          mEventConnections[i] =
              mInterface2InternalEventCons[getEIID(cpstCurrentConn->mSrcId)];
        }
        else{
          mEventConnections[i] = poSrcFB->getEOConnection(cpstCurrentConn->mSrcId);
        }
        establishConnection(mEventConnections[i], poDstFB, cpstCurrentConn->mDstId);
      }
      else{
        DEVLOG_ERROR("Could not create event connection in CFB");
      }
    }
    //now handle the fanned out connections
    for(unsigned int i = 0; i < cmFBNData->mNumFannedOutEventConnections; ++i){
      const SCFB_FBFannedOutConnectionData *cpstCurrentFannedConn = &(cmFBNData->mFannedOutEventConnections[i]);
      poDstFB = getFunctionBlock(cpstCurrentFannedConn->mDstFBNum);
      establishConnection(mEventConnections[cpstCurrentFannedConn->mConnectionNum],
          poDstFB, cpstCurrentFannedConn->mDstId);
    }
  }
}

void CCompositeFB::prepareIf2InEventCons(){
  if(0 != mInterfaceSpec->mNumEIs){
    mInterface2InternalEventCons = new TEventConnectionPtr[mInterfaceSpec->mNumEIs];
    //FIXME find a way to avoid that each connection has to be allocated separately
    for(TPortId i = 0; i < mInterfaceSpec->mNumEIs; i++){
      mInterface2InternalEventCons[i] = new CEventConnection(this, i);
    }
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
  if(cmFBNData->mNumDataConnections){
    if(mInterfaceSpec->mNumDIs){
      prepareIf2InDataCons();
    }
    if(mInterfaceSpec->mNumDOs){
      mIn2IfDConns = new CDataConnection *[mInterfaceSpec->mNumDOs];
      for(TPortId i = 0; i < mInterfaceSpec->mNumDOs; i++){
        mIn2IfDConns[i] = nullptr;
      }
    }

    mDataConnections = new CDataConnection *[cmFBNData->mNumDataConnections];

    CFunctionBlock *poSrcFB;
    CFunctionBlock *poDstFB;
    for(size_t i = 0; i < cmFBNData->mNumDataConnections; ++i){
      const SCFB_FBConnectionData &currentConn(cmFBNData->mDataConnections[i]);
      //FIXME implement way to inform FB creator that creation failed
      poSrcFB = getFunctionBlock(currentConn.mSrcFBNum);
      poDstFB = getFunctionBlock(currentConn.mDstFBNum);

      if((nullptr != poSrcFB) && (nullptr != poDstFB)){
        if(this == poSrcFB){
          mDataConnections[i] = &(mIf2InDConns[getDIID(currentConn.mSrcId)]);
        }
        else{
          mDataConnections[i] = poSrcFB->getDOConnection(currentConn.mSrcId);
        }
        establishConnection(mDataConnections[i], poDstFB, currentConn.mDstId);
      }
      else{
        DEVLOG_ERROR("Could not create data connection in CFB");
      }
    }
    //now handle the fanned out connections
    for(size_t i = 0; i < cmFBNData->mNumFannedOutDataConnections; ++i){
      const SCFB_FBFannedOutConnectionData &currentFannedConn(cmFBNData->mFannedOutDataConnections[i]);
      poDstFB = getFunctionBlock(currentFannedConn.mDstFBNum);
      establishConnection(mDataConnections[currentFannedConn.mConnectionNum], poDstFB, currentFannedConn.mDstId);
    }
  }
}

void CCompositeFB::createAdapterConnections(){
  for(size_t i = 0; i < cmFBNData->mNumAdapterConnections; ++i){
    const SCFB_FBConnectionData &currentConn(cmFBNData->mAdapterConnections[i]);
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

void CCompositeFB::prepareIf2InDataCons(){
  mIf2InDConns = new CInterface2InternalDataConnection[mInterfaceSpec->mNumDIs];
  for(TPortId i = 0; i < mInterfaceSpec->mNumDIs; i++){
    (mIf2InDConns + i)->setSource(this, i);
  }
}

void CCompositeFB::setParams() {
  for(unsigned int i = 0; i < cmFBNData->mNumParams; ++i){
    const SCFB_FBParameter *pstCurrentParam = &(cmFBNData->mParams[i]);
    CIEC_ANY *poDI =
        mInternalFBs[pstCurrentParam->mFBNum]->getDataInput(pstCurrentParam->mDINameID);
    if(nullptr != poDI){
      poDI->fromString(pstCurrentParam->mParamValue);
    }
    else{
      DEVLOG_ERROR("Could not get date input for setting a parameter");
    }
  }
}

CFunctionBlock *CCompositeFB::getFunctionBlock(int paFBNum){
  CFunctionBlock *poRetVal = nullptr;
  if(-1 == paFBNum){
    poRetVal = this;
  }
  else if(0 <= paFBNum)  {
    TForteUInt32 fbNum = static_cast<TForteUInt32>(paFBNum);
    if(scmAdapterMarker == (scmAdapterMarker & fbNum)){
      fbNum &= scmAdapterFBRange;
      if(fbNum < mInterfaceSpec->mNumAdapters){
        poRetVal = mAdapters[fbNum];
      }
    }
    else{
      if(fbNum < cmFBNData->mNumFBs){
        poRetVal = mInternalFBs[fbNum];
      }
    }
  }
  return poRetVal;
}
