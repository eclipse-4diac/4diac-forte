/*******************************************************************************
 * Copyright (c) 2015, 2025 fortiss GmbH, 2018 TU Wien/ACIN,
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
 *                - use span for create and delete FB
 *******************************************************************************/
#include "forte/fbcontainer.h"
#include "forte/funcbloc.h"
#include "typelib_internal.h"

namespace forte {
  EMGMResponse checkForActionEquivalentState(const CFunctionBlock &paFB, const EMGMCommandType paCommand) {
    CFunctionBlock::E_FBStates currentState = paFB.getState();
    switch (paCommand) {
      case EMGMCommandType::Stop:
        return (CFunctionBlock::E_FBStates::Killed == currentState) ? EMGMResponse::Ready : EMGMResponse::InvalidState;
        break;
      case EMGMCommandType::Kill:
        return (CFunctionBlock::E_FBStates::Stopped == currentState || CFunctionBlock::E_FBStates::Idle == currentState)
                   ? EMGMResponse::Ready
                   : EMGMResponse::InvalidState;
        break;
      default: break;
    }
    return EMGMResponse::InvalidState;
  }

  CFBContainer::CFBContainer(StringId paContInstanceName, CFBContainer &paParent) :
      mContInstanceName(paContInstanceName),
      mParent(paParent) {
  }

  bool CFBContainer::initialize() {
    // initialize all statically added internal FBs
    for (auto it : getChildren()) {
      if (it->isFB()) {
        if (!static_cast<CFunctionBlock *>(it)->initialize()) {
          return false;
        }
      }
    }
    return true;
  }

  void CFBContainer::deinitialize() {
    for (auto it : getChildren()) {
      it->deinitialize();
    }
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
    if (!result.empty()) {
      result += sepChar;
    }
    result += getInstanceName();
    return result;
  }

  void CFBContainer::getFullQualifiedApplicationInstanceName(TNameIdentifier &paResult) const {
    mParent.getFullQualifiedApplicationInstanceName(paResult);
    paResult.push_back(getInstanceNameId());
  }

  EMGMResponse
  CFBContainer::createFB(const std::span<const StringId> paNameList, StringId paTypeName, std::string_view paTypeHash) {
    if (paNameList.empty()) {
      return EMGMResponse::InvalidDst;
    }
    if (paNameList.size() == 1) {
      return createFB(paNameList.front(), paTypeName, paTypeHash);
    }
    if (isDynamicContainer()) {
      // we have more than one name in the fb name list. Find or create the container and hand the create command to
      // this container.
      CFBContainer *childCont = findOrCreateContainer(paNameList.front());
      if (childCont != nullptr && childCont->isDynamicContainer()) {
        return childCont->createFB(paNameList.subspan(1), paTypeName, paTypeHash);
      }
    }
    return EMGMResponse::InvalidDst;
  }

  EMGMResponse CFBContainer::createFB(StringId paInstanceNameId, StringId paTypeName, std::string_view paTypeHash) {
    if (!isDynamicContainer()) {
      return EMGMResponse::InvalidDst;
    }
    TFBContainerList::iterator childIt = getChildrenIterator(paInstanceNameId);
    // test if the container does not contain any FB or a container with the same name
    if (isChild(childIt, paInstanceNameId)) {
      return EMGMResponse::InvalidState;
    }

    EMGMResponse errorMSG;
    CFunctionBlock *newFB = forte::createFB(paInstanceNameId, paTypeName, paTypeHash, *this, errorMSG);
    if (newFB == nullptr) {
      return errorMSG;
    }

    // we could create a FB now add it to the list of contained FBs
    mChildren.insert(childIt, newFB);
    return EMGMResponse::Ready;
  }

  EMGMResponse CFBContainer::deleteFB(const std::span<const StringId> paNameList) {
    if (paNameList.empty()) {
      return EMGMResponse::NoSuchObject;
    }

    StringId childName = paNameList.front();
    auto childIt = getChildrenIterator(childName);
    if (isChild(childIt, childName)) {
      CFBContainer *child = *childIt;
      if (paNameList.size() > 1) {
        // we have more than one name in the fb name list. Hand the process on to the child if it is a dynamic container
        if (child->isDynamicContainer()) {
          // remove the container from the name list
          return child->deleteFB(paNameList.subspan(1));
        }
      } else if (child->isFB()) {
        CFunctionBlock *fb = static_cast<CFunctionBlock *>(child);
        if (fb->isCurrentlyDeleteable()) {
          forte::deleteFB(fb);
          mChildren.erase(childIt);
          return EMGMResponse::Ready;
        }
        return EMGMResponse::InvalidState;
      }
    }
    return EMGMResponse::NoSuchObject;
  }

  CFunctionBlock *CFBContainer::getFB(StringId paFBName) {
    if (paFBName) {
      CFBContainer *child = getChild(paFBName);
      if (child != nullptr && child->isFB()) {
        return static_cast<CFunctionBlock *>(child);
      }
    }
    return nullptr;
  }

  CFunctionBlock *CFBContainer::getFB(NameIterator &paNameListIt, NameIterator paNameListEnd) {
    if (paNameListIt != paNameListEnd) {
      CFBContainer *childCont = getChild(*paNameListIt);
      if (childCont != nullptr) {
        // remove the container from the name list
        ++paNameListIt;
        return childCont->getFB(paNameListIt, paNameListEnd);
      }
    }
    // we are the last FB in the name list
    return isFB() ? static_cast<CFunctionBlock *>(this) : nullptr;
  }

  CFBContainer *CFBContainer::getChild(StringId paName) {
    TFBContainerList::iterator it = getChildrenIterator(paName);
    return isChild(it, paName) ? *it : nullptr;
  }

  CFBContainer::TFBContainerList::iterator CFBContainer::getChildrenIterator(StringId paName) {
    if (paName && !mChildren.empty()) {
      return std::lower_bound(mChildren.begin(), mChildren.end(), paName,
                              [](CFBContainer *container, StringId containerName) {
                                return container->getInstanceNameId() < containerName;
                              });
    }
    return mChildren.end();
  }

  CFBContainer *CFBContainer::findOrCreateContainer(StringId paContainerName) {
    CFBContainer *retVal;
    if (mChildren.empty()) {
      retVal = new CFBContainer(paContainerName, *this);
      mChildren.insert(mChildren.begin(), retVal);
    } else {
      TFBContainerList::iterator childIt = getChildrenIterator(paContainerName);
      if (isChild(childIt, paContainerName)) {
        retVal = *childIt;
      } else {
        // the container with the given name does not exist but only create it if there is no FB with the same name.
        retVal = new CFBContainer(paContainerName, *this);
        mChildren.insert(childIt, retVal);
      }
    }
    return retVal;
  }

  EMGMResponse CFBContainer::changeExecutionState(EMGMCommandType paCommand) {
    EMGMResponse retVal = EMGMResponse::Ready;

    for (TFBContainerList::iterator it(mChildren.begin()); ((it != mChildren.end()) && (EMGMResponse::Ready == retVal));
         ++it) {
      retVal = (*it)->changeExecutionState(paCommand);
      if ((EMGMResponse::Ready != retVal) && (*it)->isFB()) {
        retVal = checkForActionEquivalentState(static_cast<CFunctionBlock &>(**it), paCommand);
      }
    }
    return retVal;
  }

  CIEC_ANY *CFBContainer::getVar(const std::span<const StringId> paNameList) {
    if (paNameList.empty()) {
      return nullptr;
    }
    const StringId name = paNameList.front();
    if (const auto child = getChild(name)) {
      return child->getVar(paNameList.subspan(1));
    }
    return nullptr;
  }

  bool CFBContainer::setForced(const std::span<const StringId> paNameList, const bool paForce) {
    if (paNameList.empty()) {
      return false;
    }
    const StringId name = paNameList.front();
    if (const auto child = getChild(name)) {
      return child->setForced(paNameList.subspan(1), paForce);
    }
    return false;
  }

  CConnection *CFBContainer::getInputConnection(const std::span<const StringId> paDstNameList) {
    if (paDstNameList.empty()) {
      return nullptr;
    }
    StringId name = paDstNameList.front();
    if (const auto child = getChild(name); child) {
      return child->getInputConnection(paDstNameList.subspan(1));
    }
    return nullptr;
  }

  CConnection::Wrapper CFBContainer::getOutputConnection(const std::span<const StringId> paSrcNameList) {
    if (paSrcNameList.empty()) {
      return CConnection::Wrapper();
    }
    StringId name = paSrcNameList.front();
    if (const auto child = getChild(name); child) {
      return child->getOutputConnection(paSrcNameList.subspan(1));
    }
    return CConnection::Wrapper();
  }
} // namespace forte
