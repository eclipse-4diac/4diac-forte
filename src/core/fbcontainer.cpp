/*******************************************************************************
 * Copyright (c) 2015 - 2016 fortiss GmbH, 2018 TU Wien/ACIN
 *                      2023 Primetals Technologies Austria GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Alois Zoitl
 *      - initial implementation and rework communication infrastructure
 *    Martin Jobst - adapt for LUA integration
 *    Martin Melik Merkumians
 *      - implementation for checkForActionEquivalentState
 *    Fabio Gandolfi
 *      - refactored fb and container list to sorted vectors
 *******************************************************************************/
#include "funcbloc.h"
#include "fbcontainer.h"

using namespace forte::core; 

EMGMResponse checkForActionEquivalentState(const CFunctionBlock &paFB, const EMGMCommandType paCommand){
  CFunctionBlock::E_FBStates currentState = paFB.getState();
  switch (paCommand){
    case EMGMCommandType::Stop:
      return (CFunctionBlock::E_FBStates::Killed == currentState) ? EMGMResponse::Ready : EMGMResponse::InvalidState;
      break;
    case EMGMCommandType::Kill:
      return (CFunctionBlock::E_FBStates::Stopped == currentState || CFunctionBlock::E_FBStates::Idle == currentState) ? EMGMResponse::Ready : EMGMResponse::InvalidState;
      break;
    default:
      break;
  }
  return EMGMResponse::InvalidState;
}

CFBContainer::CFBContainer(CStringDictionary::TStringId paContainerName, CFBContainer *paParent) :
    mContainerName(paContainerName), mParent(paParent) {
}

CFBContainer::~CFBContainer() {
  for (TFunctionBlockList::iterator itRunner(mFunctionBlocks.begin()); itRunner != mFunctionBlocks.end(); ++itRunner) {
    CTypeLib::deleteFB(*itRunner);
  }
  mFunctionBlocks.clear();

  for (TFBContainerList::iterator itRunner(mSubContainers.begin()); itRunner != mSubContainers.end(); ++itRunner) {
    delete (*itRunner);
  }
  mSubContainers.clear();
}

EMGMResponse CFBContainer::addFB(CFunctionBlock* paFuncBlock){
  EMGMResponse eRetVal = EMGMResponse::InvalidObject;
  if(nullptr != paFuncBlock){
    mFunctionBlocks.insert(getFBIterator(paFuncBlock->getInstanceNameId()), paFuncBlock);
    eRetVal = EMGMResponse::Ready;
  }
  return eRetVal;
}


EMGMResponse CFBContainer::createFB(forte::core::TNameIdentifier::CIterator &paNameListIt, CStringDictionary::TStringId paTypeName, CResource *paRes){
  EMGMResponse retval = EMGMResponse::InvalidState;

  if(paNameListIt.isLastEntry()){
    // test if the container does not contain any FB or a container with the same name
    if((nullptr == getFB(*paNameListIt)) && (nullptr == getFBContainer(*paNameListIt))){
      CFunctionBlock *newFB = CTypeLib::createFB(*paNameListIt, paTypeName, paRes);
      if(nullptr != newFB){
        //we could create a FB now add it to the list of contained FBs
        addFB(newFB);
        newFB->setContainer(this);
        retval = EMGMResponse::Ready;
      }
      else{
        retval = CTypeLib::getLastError();
      }
    }
  }
  else{
    //we have more than one name in the fb name list. Find or create the container and hand the create command to this container.
    CFBContainer *childCont = findOrCreateContainer(*paNameListIt);
    if(nullptr != childCont){
      //remove the container from the name list
      ++paNameListIt;
      retval = childCont->createFB(paNameListIt, paTypeName, paRes);
    }
  }
  return retval;
}

EMGMResponse CFBContainer::deleteFB(forte::core::TNameIdentifier::CIterator &paNameListIt){
  EMGMResponse retval = EMGMResponse::NoSuchObject;

  if(!paNameListIt.isLastEntry()){
    //we have more than one name in the fb name list. Find or create the container and hand the create command to this container.
    CFBContainer *childCont = findOrCreateContainer(*paNameListIt);
    if(nullptr != childCont){
      //remove the container from the name list
      ++paNameListIt;
      retval = childCont->deleteFB(paNameListIt);
    }
  }
  else{
    CStringDictionary::TStringId fBNameId = *paNameListIt;

    if((CStringDictionary::scmInvalidStringId != fBNameId) && (!mFunctionBlocks.empty())) {
      TFunctionBlockList::iterator it = getFBIterator(fBNameId);
      if(fBIteratorIsValid(it,fBNameId) && (*it)->isCurrentlyDeleteable()){
        mFunctionBlocks.erase(it);
        CTypeLib::deleteFB(*it);
        retval = EMGMResponse::Ready;
      } else {
        retval = EMGMResponse::InvalidState;
      }
    }
  }
  return retval;
}

CFunctionBlock *CFBContainer::getFB(CStringDictionary::TStringId paFBName) {
  if(CStringDictionary::scmInvalidStringId != paFBName){
    TFunctionBlockList::iterator retIter = getFBIterator(paFBName);
    if(fBIteratorIsValid(retIter,paFBName))
      return *retIter;
  }
  return nullptr;
}

CFBContainer::TFunctionBlockList::iterator CFBContainer::getFBIterator(CStringDictionary::TStringId paFBName){
  if(CStringDictionary::scmInvalidStringId != paFBName) {
    return lower_bound(mFunctionBlocks.begin(), mFunctionBlocks.end(), paFBName,
                              [](CFunctionBlock *fb,
                                 CStringDictionary::TStringId fbName) {
                                return fb->getInstanceNameId() < fbName;
                              });
  }
  return mFunctionBlocks.end();;
}

CFunctionBlock* CFBContainer::getContainedFB(forte::core::TNameIdentifier::CIterator &paNameListIt)  {
  if(!paNameListIt.isLastEntry()){
    //we have more than one name in the fb name list. Find or create the container and hand the create command to this container.
    CFBContainer *childCont = getFBContainer(*paNameListIt);
    if(nullptr != childCont){
      //remove the container from the name list
      ++paNameListIt;
      return childCont->getContainedFB(paNameListIt);
    }
  }

  return getFB(*paNameListIt);
}

CFBContainer* CFBContainer::getFBContainer(CStringDictionary::TStringId paContainerName)  {
  TFBContainerList::iterator it = getFBContainerIterator(paContainerName);
  if(it != mSubContainers.end() && (*it)->getNameId() == paContainerName){
    return *it;
  }
  return nullptr;
}

CFBContainer::TFBContainerList::iterator CFBContainer::getFBContainerIterator(CStringDictionary::TStringId paContainerName)  {
  if(CStringDictionary::scmInvalidStringId != paContainerName && !mSubContainers.empty()){
    return std::lower_bound(mSubContainers.begin(), mSubContainers.end(), paContainerName,
                                                     [](CFBContainer* container, CStringDictionary::TStringId containerName)
                                                     { return container->getNameId() < containerName; });
  }
  return mSubContainers.end();
}

CFBContainer *CFBContainer::findOrCreateContainer(CStringDictionary::TStringId paContainerName) {
  CFBContainer *retVal;
  if (mSubContainers.empty()) {
    retVal = new CFBContainer(paContainerName, this);
    mSubContainers.insert(mSubContainers.begin(), retVal);
  } else {
    TFBContainerList::iterator it = getFBContainerIterator(paContainerName);
    if (nullptr == getFB(paContainerName) && (it == mSubContainers.end() || (*it)->getNameId() != paContainerName)) {
      //the container with the given name does not exist but only create it if there is no FB with the same name.
      retVal = new CFBContainer(paContainerName, this);
      mSubContainers.insert(it, retVal);
    } else {
      retVal = (*it);
    }
  }
  return retVal;
}

EMGMResponse CFBContainer::changeContainedFBsExecutionState(EMGMCommandType paCommand){
  EMGMResponse retVal = EMGMResponse::Ready;

  for(TFBContainerList::iterator it(mSubContainers.begin());
      ((it != mSubContainers.end()) && (EMGMResponse::Ready == retVal));
      ++it){
    retVal = (*it)->changeContainedFBsExecutionState(paCommand);
  }

  if(EMGMResponse::Ready == retVal){
    for(TFunctionBlockList::iterator itRunner = mFunctionBlocks.begin();
        ((itRunner != mFunctionBlocks.end()) && (EMGMResponse::Ready == retVal));
        ++itRunner){
      retVal = (*itRunner)->changeFBExecutionState(paCommand);
      if(EMGMResponse::Ready != retVal) {
        retVal = checkForActionEquivalentState(*(*itRunner), paCommand);
      }
    }
  }
  return retVal;
}

bool CFBContainer::fBIteratorIsValid(CFBContainer::TFunctionBlockList::iterator iterator,CStringDictionary::TStringId paFBName){
  return (iterator != mFunctionBlocks.end() && paFBName == ((*(*iterator)).getInstanceNameId()));
}
