/*******************************************************************************
  * Copyright (c) 2015 - 2016 fortiss GmbH
  * All rights reserved. This program and the accompanying materials
  * are made available under the terms of the Eclipse Public License v1.0
  * which accompanies this distribution, and is available at
  * http://www.eclipse.org/legal/epl-v10.html
  *
  * Contributors:
  *    Alois Zoitl
  *      - initial implementation and rework communication infrastructure
  *    Martin Jobst - adapt for LUA integration
  *******************************************************************************/
#include "fbcontainer.h"

using namespace forte::core;

CFBContainer::CFBContainer(CStringDictionary::TStringId paContainerName, CFBContainer *paParent) :
    mContainerName(paContainerName), mParent(paParent) {
}

CFBContainer::~CFBContainer() {
  for (TFunctionBlockList::Iterator itRunner(mFunctionBlocks.begin()); itRunner != mFunctionBlocks.end(); ++itRunner) {
    CTypeLib::deleteFB(*itRunner);
  }
  mFunctionBlocks.clearAll();

  for (TFBContainerList::Iterator itRunner(mSubContainers.begin()); itRunner != mSubContainers.end(); ++itRunner) {
    delete (*itRunner);
  }
  mSubContainers.clearAll();
}

EMGMResponse CFBContainer::addFB(CFunctionBlock* pa_poFuncBlock){
  EMGMResponse eRetVal = e_INVALID_OBJECT;
  if(0 != pa_poFuncBlock){
    mFunctionBlocks.push_back(pa_poFuncBlock);
    eRetVal = e_RDY;
  }
  return eRetVal;
}


EMGMResponse CFBContainer::createFB(forte::core::TNameIdentifier::CIterator &paNameListIt, CStringDictionary::TStringId paTypeName, CResource *paRes){
  EMGMResponse retval = e_INVALID_STATE;

  if(paNameListIt.isLastEntry()){
    // test if the container does not contain any FB or a container with the same name
    if((0 == getFB(*paNameListIt)) && (0 == getFBContainer(*paNameListIt))){
      CFunctionBlock *newFB = CTypeLib::createFB(*paNameListIt, paTypeName, paRes);
      if(0 != newFB){
        //we could create a FB now add it to the list of contained FBs
        mFunctionBlocks.push_back(newFB);
        retval = e_RDY;
      }
      else{
        retval = CTypeLib::getLastError();
      }
    }
  }
  else{
    //we have more than one name in the fb name list. Find or create the container and hand the create command to this container.
    CFBContainer *childCont = findOrCreateContainer(*paNameListIt);
    if(0 != childCont){
      //remove the container from the name list
      ++paNameListIt;
      retval = childCont->createFB(paNameListIt, paTypeName, paRes);
    }
  }
  return retval;
}

EMGMResponse CFBContainer::deleteFB(forte::core::TNameIdentifier::CIterator &paNameListIt){
  EMGMResponse retval = e_NO_SUCH_OBJECT;

  if(!paNameListIt.isLastEntry()){
    //we have more than one name in the fb name list. Find or create the container and hand the create command to this container.
    CFBContainer *childCont = findOrCreateContainer(*paNameListIt);
    if(0 != childCont){
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
              mFunctionBlocks.pop_front();
            }
            else{
              mFunctionBlocks.eraseAfter(itRefNode);
            }
            retval = e_RDY;
          }
          else{
            retval = e_INVALID_STATE;
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
  CFunctionBlock *retVal = 0;

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
    if(0 != childCont){
      //remove the container from the name list
      ++paNameListIt;
      return childCont->getContainedFB(paNameListIt);
    }
  }

  return getFB(*paNameListIt);
}

CFBContainer *CFBContainer::getFBContainer(CStringDictionary::TStringId paContainerName)  {
  CFBContainer *retVal = 0;

  if(CStringDictionary::scm_nInvalidStringId != paContainerName){
    for(TFBContainerList::Iterator it = mSubContainers.begin(); it != mSubContainers.end();
        ++it){
      if(paContainerName == ((*(*it)).getName())){
        retVal = (*it);
        break;
      }
    }
  }
  return retVal;
}

CFBContainer *CFBContainer::findOrCreateContainer(CStringDictionary::TStringId paContainerName){
  CFBContainer *retVal = getFBContainer(paContainerName);
  if(0 == retVal){
    //the container with the given name does not exist create a new one
    if(0 == getFB(paContainerName)){
      //only create it if there is no FB with the same name.
      retVal = new CFBContainer(paContainerName, this);
      mSubContainers.push_back(retVal);
    }
  }
  return retVal;
}

EMGMResponse CFBContainer::changeContainedFBsExecutionState(EMGMCommandType paCommand){
  EMGMResponse retVal = e_RDY;

  for(TFBContainerList::Iterator it(mSubContainers.begin());
      ((it != mSubContainers.end()) && (e_RDY == retVal));
      ++it){
    retVal = (*it)->changeContainedFBsExecutionState(paCommand);
  }

  if(e_RDY == retVal){
    for(TFunctionBlockList::Iterator itRunner(mFunctionBlocks.begin());
        ((itRunner != mFunctionBlocks.end()) && (e_RDY == retVal));
        ++itRunner){
      retVal = (*itRunner)->changeFBExecutionState(paCommand);
    }
  }
  return retVal;
}
