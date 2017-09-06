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

namespace IO {
namespace ConfigurationFB {

PartController::PartController(CResource *pa_poSrcRes,
    const SFBInterfaceSpec *pa_pstInterfaceSpec,
    const CStringDictionary::TStringId pa_nInstanceNameId,
    TForteByte *pa_acFBConnData, TForteByte *pa_acFBVarsData) :
    Base(pa_poSrcRes, pa_pstInterfaceSpec, pa_nInstanceNameId, pa_acFBConnData,
        pa_acFBVarsData), master(0) {

}

void PartController::executeEvent(int pa_nEIID) {
  if (Adapter().INIT() == pa_nEIID) {
    if (Adapter().QI() == true) {
      // Get master by id
      master = SplitController::getControllerById(Adapter().MasterId());

      if (master == 0) {
        QO() = false;
      } else {
        // Initialize handles
        initHandles();

        QO() = true;
      }
      // Send confirmation of init
      Adapter().QO() = QO();
      sendAdapterEvent(scm_nSplitAdapterAdpNum,
          SplitAdapter::scm_nEventINITOID);
    } else {
      QO() = false;

      // Send confirmation of deInit
      Adapter().QO() = QO();
      sendAdapterEvent(scm_nSplitAdapterAdpNum,
          SplitAdapter::scm_nEventINITOID);
    }
  }
}

void PartController::initHandle(
    Device::Controller::HandleDescriptor *handleDescriptor) {
  master->initHandle(handleDescriptor);
}

} /* namespace ConfigurationFB */
} /* namespace IO */
