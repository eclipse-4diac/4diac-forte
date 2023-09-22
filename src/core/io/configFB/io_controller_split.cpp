/*******************************************************************************
 * Copyright (c) 2017 - 2018 fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Johannes Messmer - initial API and implementation and/or initial documentation
 *   Jose Cabral - Cleaning of namespaces
 *******************************************************************************/

#include "io_controller_split.h"

using namespace forte::core::io;

const char * const IOConfigFBSplitController::scmFailedToInitParts = "Failed to initialize parts.";

TControllerList IOConfigFBSplitController::mInstances;
TForteUInt16 IOConfigFBSplitController::mInstancesIncrement = 0;

IOConfigFBSplitController::IOConfigFBSplitController(const TForteUInt8* const paSplitAdapter, const TForteUInt8 paSplitAdapterNum, CResource *paSrcRes,
    const SFBInterfaceSpec *paInterfaceSpec, const CStringDictionary::TStringId paInstanceNameId) :
    IOConfigFBController(paSrcRes, paInterfaceSpec, paInstanceNameId), scmSplitAdapter(paSplitAdapter),
        scmSplitAdapterNum(paSplitAdapterNum), mSplitIterator(0) {
  mId = mInstancesIncrement++;
  mInstances.pushBack(this);
}

IOConfigFBSplitController* IOConfigFBSplitController::getControllerById(TForteUInt16 paId) {
  TControllerList::Iterator itEnd = mInstances.end();
  int i = 0;
  for(TControllerList::Iterator it = mInstances.begin(); it != itEnd; ++it, i++) {
    if(paId == i && *it != nullptr) {
      return *it;
    }
  }
  return nullptr;
}

void IOConfigFBSplitController::onStartup(CEventChainExecutionThread * const paECET) {
  mSplitIterator = 0;

  if(!scmSplitAdapterNum) {
    return IOConfigFBController::onStartup(paECET);
  }

  IOConfigFBSplitAdapter *cur = static_cast<IOConfigFBSplitAdapter*>(mAdapters[scmSplitAdapter[mSplitIterator]]);

  while(!cur || nullptr == cur->getPeer()) {
    mSplitIterator++;

    if(mSplitIterator == scmSplitAdapterNum) {
      return IOConfigFBController::onStartup(paECET);
    }

    cur = static_cast<IOConfigFBSplitAdapter*>(mAdapters[scmSplitAdapter[mSplitIterator]]);
  }

  cur->MasterId() = CIEC_UINT(mId);
  cur->QI() = CIEC_BOOL(true);
  sendAdapterEvent(scmSplitAdapter[mSplitIterator], IOConfigFBSplitAdapter::scmEventINITID, paECET);
}

void IOConfigFBSplitController::onStop(CEventChainExecutionThread * const paECET) {
  mSplitIterator = 0;

  if(!scmSplitAdapterNum) {
    return IOConfigFBController::onStop(paECET);
  }

  IOConfigFBSplitAdapter *cur = static_cast<IOConfigFBSplitAdapter*>(mAdapters[scmSplitAdapter[mSplitIterator]]);

  while(cur->getPeer() == nullptr) {
    mSplitIterator++;

    if(mSplitIterator == scmSplitAdapterNum) {
      return IOConfigFBController::onStop(paECET);
    }

    cur = static_cast<IOConfigFBSplitAdapter*>(mAdapters[scmSplitAdapter[mSplitIterator]]);
  }

  cur->QI() = CIEC_BOOL(false);
  sendAdapterEvent(scmSplitAdapter[mSplitIterator], IOConfigFBSplitAdapter::scmEventINITID, paECET);
}

void IOConfigFBSplitController::executeEvent(TEventID paEIID, CEventChainExecutionThread * const paECET) {
  IOConfigFBController::executeEvent(paEIID, paECET);

  if(mSplitIterator < scmSplitAdapterNum) {
    IOConfigFBSplitAdapter *cur = static_cast<IOConfigFBSplitAdapter*>(mAdapters[scmSplitAdapter[mSplitIterator]]);
    if(cur->INITO() == paEIID) {

      QO() = cur->QO();

      mSplitIterator++;

      if(mSplitIterator == scmSplitAdapterNum || (cur->QI() && !cur->QO())) {
        if(cur->QI() == true) {
          if(cur->QO() == true) {
            IOConfigFBController::onStartup(paECET);
          } else {
            started(paECET, scmFailedToInitParts);
          }
        } else {
          IOConfigFBController::onStop(paECET);
        }
      } else {
        IOConfigFBSplitAdapter *next = static_cast<IOConfigFBSplitAdapter*>(mAdapters[scmSplitAdapter[mSplitIterator]]);

        while(!next || nullptr == next->getPeer()) {
          mSplitIterator++;

          if(mSplitIterator == scmSplitAdapterNum) {
            if(cur->QO() == true) {
              return IOConfigFBController::onStartup(paECET);
            } else {
              return IOConfigFBController::onStop(paECET);
            }
          }

          next = static_cast<IOConfigFBSplitAdapter*>(mAdapters[scmSplitAdapter[mSplitIterator]]);
        }

        next->MasterId() = CIEC_UINT(mId);
        next->QI() = cur->QI();

        sendAdapterEvent(scmSplitAdapter[mSplitIterator], IOConfigFBSplitAdapter::scmEventINITID, paECET);
      }
    }
  }
}
