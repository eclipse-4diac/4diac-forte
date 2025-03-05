/*******************************************************************************
 * Copyright (c) 2015, 2024 fortiss GmbH, 2018 TU Wien/ACIN,
 *                          Primetals Technologies Austria GmbH
 *                          Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl  - initial implementation and rework communication infrastructure
 *   Martin Jobst - adapt for LUA integration
 *   Martin Melik Merkumians - implementation for checkForActionEquivalentState
 *   Fabio Gandolfi - refactored fb and container list to sorted vectors
 *   Alois Zoitl  - merged fbs and containers in one list
 *   Martin Jobst - add smart pointer for internal FBs
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

CFBContainer::CFBContainer(CStringDictionary::TStringId paContInstanceName, CFBContainer &paParent) :
    mContInstanceName(paContInstanceName), mParent(paParent) {
}

bool CFBContainer::initialize() {
  // initialize all statically added internal FBs
  for (auto it: getChildren()) {
    if (it->isFB()) {
      if (!static_cast<CFunctionBlock *>(it)->initialize()) {
        return false;
      }
    }
  }
  return true;
}

CFBContainer::~CFBContainer() {
  for (TFBContainerList::iterator itRunner(mChildren.begin()); itRunner != mChildren.end(); ++itRunner) {
    delete (*itRunner);
  }
  mChildren.clear();
}

void CFBContainer::addFB(CFunctionBlock &paFuncBlock) {
  mChildren.insert(getChildrenIterator(paFuncBlock.getInstanceNameId()), &paFuncBlock);
}

void CFBContainer::removeFB(CFunctionBlock &paFuncBlock) {
  mChildren.erase(std::remove(mChildren.begin(), mChildren.end(), &paFuncBlock), mChildren.end());
}

std::string CFBContainer::getFullQualifiedApplicationInstanceName(const char sepChar) const {
  std::string result(mParent.getFullQualifiedApplicationInstanceName(sepChar));
  if(!result.empty()){
    result += sepChar;
  }
  result += getInstanceName();
  return result;
}

EMGMResponse CFBContainer::createFB(NameIterator &paNameListIt, NameIterator paNameListEnd, CStringDictionary::TStringId paTypeName){
  if (paNameListIt+1 == paNameListEnd) {
    return createFB(*paNameListIt, paTypeName);
  } else if(isDynamicContainer()) {
    //we have more than one name in the fb name list. Find or create the container and hand the create command to this container.
    CFBContainer *childCont = findOrCreateContainer(*paNameListIt);
    if(childCont != nullptr && childCont->isDynamicContainer()){
      //remove the container from the name list
      ++paNameListIt;
      return childCont->createFB(paNameListIt, paNameListEnd, paTypeName);
    }
  }
  return EMGMResponse::InvalidDst;
}

EMGMResponse CFBContainer::createFB(CStringDictionary::TStringId paInstanceNameId, CStringDictionary::TStringId paTypeName) {
  if(!isDynamicContainer()) {
    return EMGMResponse::InvalidDst;
  }
  TFBContainerList::iterator childIt = getChildrenIterator(paInstanceNameId);
  // test if the container does not contain any FB or a container with the same name
  if(!isChild(childIt, paInstanceNameId)) {
    CFunctionBlock *newFB = CTypeLib::createFB(paInstanceNameId, paTypeName, *this);
    if(newFB != nullptr) {
      //we could create a FB now add it to the list of contained FBs
      mChildren.insert(childIt, newFB);
      return EMGMResponse::Ready;
    }
    return CTypeLib::getLastError();
  }
  return EMGMResponse::InvalidState;
}

EMGMResponse CFBContainer::deleteFB(NameIterator &paNameListIt, NameIterator paNameListEnd){
  EMGMResponse retval = EMGMResponse::NoSuchObject;

  CStringDictionary::TStringId childName = *paNameListIt;
  TFBContainerList::iterator childIt = getChildrenIterator(childName);
  if(isChild(childIt, childName)){
    CFBContainer *child = *childIt;
    if (paNameListIt+1 != paNameListEnd) {
      //we have more than one name in the fb name list. Hand the process on to the child if it is a dynamic container
      if(child->isDynamicContainer()){
        //remove the container from the name list
        ++paNameListIt;
        retval = child->deleteFB(paNameListIt, paNameListEnd);
      }
    } else if(child->isFB()){
      CFunctionBlock *fb = static_cast<CFunctionBlock *>(child);
      if(fb->isCurrentlyDeleteable()){
        CTypeLib::deleteFB(fb);
        mChildren.erase(childIt);
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
    CFBContainer *child = getChild(paFBName);
    if(child != nullptr && child->isFB()){
      return static_cast<CFunctionBlock *>(child);
    }
  }
  return nullptr;
}


CFunctionBlock* CFBContainer::getFB(NameIterator &paNameListIt, NameIterator paNameListEnd)  {
  if (paNameListIt+1 != paNameListEnd) {
    //we have more than one name in the fb name list. Find or create the container and hand the create command to this container.
    CFBContainer *childCont = getChild(*paNameListIt);
    if(childCont != nullptr){
      //remove the container from the name list
      ++paNameListIt;
      return childCont->getFB(paNameListIt, paNameListEnd);
    }
  }
  return getFB(*paNameListIt);
}

CFBContainer* CFBContainer::getChild(CStringDictionary::TStringId paName)  {
  TFBContainerList::iterator it = getChildrenIterator(paName);
  return isChild(it, paName) ? *it : nullptr;
}

CFBContainer::TFBContainerList::iterator CFBContainer::getChildrenIterator(CStringDictionary::TStringId paName)  {
  if(paName != CStringDictionary::scmInvalidStringId && !mChildren.empty()){
    return std::lower_bound(mChildren.begin(), mChildren.end(), paName,
                                                     [](CFBContainer* container, CStringDictionary::TStringId containerName)
                                                     { return container->getInstanceNameId() < containerName; });
  }
  return mChildren.end();
}

CFBContainer *CFBContainer::findOrCreateContainer(CStringDictionary::TStringId paContainerName) {
  CFBContainer *retVal;
  if (mChildren.empty()) {
    retVal = new CFBContainer(paContainerName, *this);
    mChildren.insert(mChildren.begin(), retVal);
  } else {
    TFBContainerList::iterator childIt = getChildrenIterator(paContainerName);
    if (isChild(childIt, paContainerName)) {
      retVal = *childIt;
    } else {
      //the container with the given name does not exist but only create it if there is no FB with the same name.
      retVal = new CFBContainer(paContainerName, *this);
      mChildren.insert(childIt, retVal);
    }
  }
  return retVal;
}

EMGMResponse CFBContainer::changeExecutionState(EMGMCommandType paCommand){
  EMGMResponse retVal = EMGMResponse::Ready;

  for(TFBContainerList::iterator it(mChildren.begin());
      ((it != mChildren.end()) && (EMGMResponse::Ready == retVal));
      ++it){
    retVal = (*it)->changeExecutionState(paCommand);
    if((EMGMResponse::Ready != retVal) && (*it)->isFB()) {
      retVal = checkForActionEquivalentState(static_cast<CFunctionBlock &>(**it), paCommand);
    }
  }
  return retVal;
}
