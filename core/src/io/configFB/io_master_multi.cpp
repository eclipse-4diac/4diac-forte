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

#include "forte/io/configFB/io_master_multi.h"

namespace forte::io {
  const char *const IOConfigFBMultiMaster::scmFailedToInitSlaves =
      "Failed to initialize slaves. Check if the configuration matches the hardware setup.";

  std::vector<IOConfigFBMultiMaster *> IOConfigFBMultiMaster::mInstances;
  TForteUInt16 IOConfigFBMultiMaster::mInstancesIncrement = 0;

  IOConfigFBMultiMaster::IOConfigFBMultiMaster(CFBContainer &paContainer,
                                               const SFBInterfaceSpec &paInterfaceSpec,
                                               const StringId paInstanceNameId) :
      IOConfigFBController(paContainer, paInterfaceSpec, paInstanceNameId) {
    mId = mInstancesIncrement++;
    mInstances.push_back(this);
  }

  IOConfigFBMultiMaster *IOConfigFBMultiMaster::getMasterById(TForteUInt16 paId) {
    auto it = std::find_if(mInstances.begin(), mInstances.end(),
                           [paId](const IOConfigFBMultiMaster *obj) { return obj->mId == paId; });
    return (it != mInstances.end()) ? *it : nullptr;
  }

  void IOConfigFBMultiMaster::onStartup(CEventChainExecutionThread *const paECET) {
    if (nullptr == BusAdapterOut().getPeer()) {
      return IOConfigFBController::onStartup(paECET);
    }

    BusAdapterOut().var_MasterId = CIEC_UINT(mId);
    BusAdapterOut().var_Index = 0_UINT;
    BusAdapterOut().var_QI = true_BOOL;
    sendAdapterEvent(BusAdapterOut(), IOConfigFBMultiAdapter::scmEventINITID, paECET);
  }

  void IOConfigFBMultiMaster::onStop(CEventChainExecutionThread *const paECET) {
    if (nullptr == BusAdapterOut().getPeer()) {
      return IOConfigFBController::onStop(paECET);
    }

    BusAdapterOut().var_QI = false_BOOL;
    sendAdapterEvent(BusAdapterOut(), IOConfigFBMultiAdapter::scmEventINITID, paECET);
  }

  void IOConfigFBMultiMaster::executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) {
    IOConfigFBController::executeEvent(paEIID, paECET);

    if (BusAdapterOut().INITO() == paEIID) {
      QO() = BusAdapterOut().var_QO;

      if (BusAdapterOut().var_QI == true) {
        if (BusAdapterOut().var_QO == true) {
          IOConfigFBController::onStartup(paECET);
        } else {
          started(paECET, scmFailedToInitSlaves);
        }
      } else {
        IOConfigFBController::onStop(paECET);
      }
    }
  }
} // namespace forte::io
