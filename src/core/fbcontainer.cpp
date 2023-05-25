/*******************************************************************************
 * Copyright (c) 2015 - 2016 fortiss GmbH, 2018 TU Wien/ACIN
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
  for (TFunctionBlockList::Iterator itRunner(mFunctionBlocks.begin()); itRunner != mFunctionBlocks.end(); ++itRunner) {
    CTypeLib::deleteFB(*itRunner);
  }
  mFunctionBlocks.clearAll();

  for (TFBContainerList::iterator itRunner(mSubContainers.begin()); itRunner != mSubContainers.end(); ++itRunner) {
    delete (*itRunner);
  }
  mSubContainers.clear();
}

EMGMResponse CFBContainer::addFB(CFunctionBlock* pa_poFuncBlock){
  EMGMResponse eRetVal = EMGMResponse::InvalidObject;
  if(nullptr != pa_poFuncBlock){
    mFunctionBlocks.pushBack(pa_poFuncBlock);
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
        mFunctionBlocks.pushBack(newFB);
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

    if((CStringDictionary::scm_nInvalidStringId != fBNameId) && (!mFunctionBlocks.isEmpty())){

      TFunctionBlockList::Iterator itRunner = mFunctionBlocks.begin();
      TFunctionBlockList::Iterator itRefNode = mFunctionBlocks.end();

      while(itRunner != mFunctionBlocks.end()){
        if(fBNameId == (*itRunner)->getInstanceNameId()){
          if((*itRunner)->isCurrentlyDeleteable()){
            CTypeLib::deleteFB(*itRunner);
            if(itRefNode == mFunctionBlocks.end()){
              //we have the first entry in the list
              mFunctionBlocks.popFront();
            }
            else{
              mFunctionBlocks.eraseAfter(itRefNode);
            }
            retval = EMGMResponse::Ready;
          }
          else{
            retval = EMGMResponse::InvalidState;
          }
          break;
        }

        itRefNode = itRunner;
        ++itRunner;
      }
    }
  }
  return retval;
}

CFunctionBlock *CFBContainer::getFB(CStringDictionary::TStringId paFBName) {
  CFunctionBlock *retVal = nullptr;

  if(CStringDictionary::scm_nInvalidStringId != paFBName){
    for(TFunctionBlockList::Iterator it = mFunctionBlocks.begin(); it != mFunctionBlocks.end();
        ++it){
      if(paFBName == ((*(*it)).getInstanceNameId())){
        retVal = (*it);
        break;
      }
    }
  }
  return retVal;
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
  if(it != mSubContainers.end() && (*it)->getName() == paContainerName){
    return *it;
  }
  return nullptr;
}

CFBContainer::TFBContainerList::iterator CFBContainer::getFBContainerIterator(CStringDictionary::TStringId paContainerName)  {
  if(CStringDictionary::scm_nInvalidStringId != paContainerName && !mSubContainers.empty()){
    TFBContainerList::iterator it = std::lower_bound(mSubContainers.begin(), mSubContainers.end(), paContainerName,
                                                     [](CFBContainer* container, CStringDictionary::TStringId paContainerName)
                                                     { return container->getName() < paContainerName; });
      return it;
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
    if (nullptr == getFB(paContainerName) && (it == mSubContainers.end() || (*it)->getName() != paContainerName)) {
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
    for(TFunctionBlockList::Iterator itRunner(mFunctionBlocks.begin());
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
