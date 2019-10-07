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

#include "device.h"
#include "if2indco.h"
#include "ecet.h"
#include <string.h>

EMGMResponse CDevice::executeMGMCommand(forte::core::SManagementCMD &paCommand){
  EMGMResponse retval = e_INVALID_DST;

  if(CStringDictionary::scm_nInvalidStringId == paCommand.mDestination){
    retval = CResource::executeMGMCommand(paCommand);
  }
  else{
    CResource *res = static_cast<CResource *>(CFBContainer::getFB(paCommand.mDestination));
    if(0 != res){
      paCommand.mDestination = CStringDictionary::scm_nInvalidStringId;
      retval = res->executeMGMCommand(paCommand);
    }
  }
  return retval;
}

EMGMResponse CDevice::changeFBExecutionState(EMGMCommandType paCommand){
  if(cg_nMGM_CMD_Kill == paCommand){
    mDeviceExecution.disableHandlers();
  }
  return CResource::changeFBExecutionState(paCommand);
}
