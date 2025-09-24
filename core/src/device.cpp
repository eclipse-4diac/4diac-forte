/*******************************************************************************
 * Copyright (c) 2005 - 2015 ACIN, Profactor GmbH, fortiss GmbH
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Alois Zoitl, Thomas Strasser, Gerhard Ebenhofer, Martin Melik Merkumians
 *      - initial implementation and rework communication infrastructure
 *******************************************************************************/

#include "forte/device.h"

#include "forte/mgmcmdstruct.h"

namespace forte {
  EMGMResponse CDevice::executeMGMCommand(SManagementCMD &paCommand) {
    EMGMResponse retval = EMGMResponse::InvalidDst;

    if (paCommand.mDestination.empty()) {
      retval = CResource::executeMGMCommand(paCommand);
    } else {
      CResource *res = static_cast<CResource *>(getChild(paCommand.mDestination));
      if (nullptr != res) {
        paCommand.mDestination = {};
        retval = res->executeMGMCommand(paCommand);
      }
    }
    return retval;
  }

  EMGMResponse CDevice::changeExecutionState(EMGMCommandType paCommand) {
    if (EMGMCommandType::Kill == paCommand) {
      mDeviceExecution.disableHandlers();
    }
    return CResource::changeExecutionState(paCommand);
  }
} // namespace forte
