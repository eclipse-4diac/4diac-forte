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

#include "io_controller_part.h"

using namespace forte::core::io;

IOConfigFBPartController::IOConfigFBPartController(CResource *paSrcRes, const SFBInterfaceSpec *paInterfaceSpec,
    const CStringDictionary::TStringId paInstanceNameId) :
    IOConfigFBBase(paSrcRes, paInterfaceSpec, paInstanceNameId), mMaster(nullptr) {
}

void IOConfigFBPartController::executeEvent(TEventID paEIID, CEventChainExecutionThread * const paECET) {
  if(IOConfigFBMultiAdapter().INIT() == paEIID) {
    if(IOConfigFBMultiAdapter().QI() == true) {
      // Get master by id
      mMaster = IOConfigFBSplitController::getControllerById(static_cast<CIEC_UINT::TValueType>(IOConfigFBMultiAdapter().MasterId()));

      if(nullptr == mMaster) {
        QO() = CIEC_BOOL(false);
      } else {
        // Initialize handles
        initHandles();

        QO() = CIEC_BOOL(true);
      }
      // Send confirmation of init
      IOConfigFBMultiAdapter().QO() = QO();
      sendAdapterEvent(scmSplitAdapterAdpNum, IOConfigFBSplitAdapter::scmEventINITOID, paECET);
    } else {
      QO() = CIEC_BOOL(false);

      // Send confirmation of deInit
      IOConfigFBMultiAdapter().QO() = QO();
      sendAdapterEvent(scmSplitAdapterAdpNum, IOConfigFBSplitAdapter::scmEventINITOID, paECET);
    }
  }
}

void IOConfigFBPartController::initHandle(IODeviceController::HandleDescriptor *paHandleDescriptor) {
  mMaster->initHandle(paHandleDescriptor);
}
