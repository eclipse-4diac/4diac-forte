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

#include "io_master_multi.h"

using namespace forte::core::io;

const char * const IOConfigFBMultiMaster::scmFailedToInitSlaves = "Failed to initialize slaves. Check if the configuration matches the hardware setup.";

TMasterList IOConfigFBMultiMaster::mInstances;
TForteUInt16 IOConfigFBMultiMaster::mInstancesIncrement = 0;

IOConfigFBMultiMaster::IOConfigFBMultiMaster(CResource *paSrcRes, const SFBInterfaceSpec *paInterfaceSpec, const CStringDictionary::TStringId paInstanceNameId) :
    IOConfigFBController(paSrcRes, paInterfaceSpec, paInstanceNameId) {
  mId = mInstancesIncrement++;
  mInstances.pushBack(this);
}

IOConfigFBMultiMaster* IOConfigFBMultiMaster::getMasterById(TForteUInt16 paId) {
  TMasterList::Iterator itEnd = mInstances.end();
  int i = 0;
  for(TMasterList::Iterator it = mInstances.begin(); it != itEnd; ++it, i++) {
    if(paId == i && *it != nullptr) {
      return *it;
    }
  }
  return nullptr;
}

void IOConfigFBMultiMaster::onStartup(CEventChainExecutionThread * const paECET) {
  if(nullptr == BusAdapterOut().getPeer()) {
    return IOConfigFBController::onStartup(paECET);
  }

  BusAdapterOut().MasterId() = CIEC_UINT(mId);
  BusAdapterOut().Index() = 0_UINT;
  BusAdapterOut().QI() = true_BOOL;
  sendAdapterEvent(scmBusAdapterAdpNum, IOConfigFBMultiAdapter::scmEventINITID, paECET);
}

void IOConfigFBMultiMaster::onStop(CEventChainExecutionThread * const paECET) {
  if(nullptr == BusAdapterOut().getPeer()) {
    return IOConfigFBController::onStop(paECET);
  }

  BusAdapterOut().QI() = false_BOOL;
  sendAdapterEvent(scmBusAdapterAdpNum, IOConfigFBMultiAdapter::scmEventINITID, paECET);
}

void IOConfigFBMultiMaster::executeEvent(TEventID paEIID, CEventChainExecutionThread * const paECET) {
  IOConfigFBController::executeEvent(paEIID, paECET);

  if(BusAdapterOut().INITO() == paEIID) {
    QO() = BusAdapterOut().QO();

    if(BusAdapterOut().QI() == true) {
      if(BusAdapterOut().QO() == true) {
        IOConfigFBController::onStartup(paECET);
      } else {
        started(paECET, scmFailedToInitSlaves);
      }
    } else {
      IOConfigFBController::onStop(paECET);
    }
  }
}

