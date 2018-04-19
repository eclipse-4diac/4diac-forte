/*******************************************************************************
 * Copyright (c) 2017 fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Johannes Messmer - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "io_controller_part.h"

IOConfigFBPartController::IOConfigFBPartController(CResource *pa_poSrcRes,
    const SFBInterfaceSpec *pa_pstInterfaceSpec,
    const CStringDictionary::TStringId pa_nInstanceNameId,
    TForteByte *pa_acFBConnData, TForteByte *pa_acFBVarsData) :
    IOConfigFBBase(pa_poSrcRes, pa_pstInterfaceSpec, pa_nInstanceNameId, pa_acFBConnData,
        pa_acFBVarsData), master(0) {

}

void IOConfigFBPartController::executeEvent(int pa_nEIID) {
  if (IOConfigFBMultiAdapter().INIT() == pa_nEIID) {
    if (IOConfigFBMultiAdapter().QI() == true) {
      // Get master by id
      master = IOConfigFBSplitController::getControllerById(IOConfigFBMultiAdapter().MasterId());

      if (master == 0) {
        QO() = false;
      } else {
        // Initialize handles
        initHandles();

        QO() = true;
      }
      // Send confirmation of init
      IOConfigFBMultiAdapter().QO() = QO();
      sendAdapterEvent(scm_nSplitAdapterAdpNum,
          IOConfigFBSplitAdapter::scm_nEventINITOID);
    } else {
      QO() = false;

      // Send confirmation of deInit
      IOConfigFBMultiAdapter().QO() = QO();
      sendAdapterEvent(scm_nSplitAdapterAdpNum,
          IOConfigFBSplitAdapter::scm_nEventINITOID);
    }
  }
}

void IOConfigFBPartController::initHandle(
    IODeviceController::HandleDescriptor *handleDescriptor) {
  master->initHandle(handleDescriptor);
}
