/*******************************************************************************
  * Copyright (c) 2005 - 2015 ACIN, Profactor GmbH, fortiss GmbH
  * All rights reserved. This program and the accompanying materials
  * are made available under the terms of the Eclipse Public License v1.0
  * which accompanies this distribution, and is available at
  * http://www.eclipse.org/legal/epl-v10.html
  *
  * Contributors:
  *    Alois Zoitl, Thomas Strasser, Gerhard Ebenhofer, Martin Melik Merkumians
  *      - initial implementation and rework communication infrastructure
  *******************************************************************************/

#include "device.h"
#include "if2indco.h"
#include "ecet.h"
#include <string.h>

EMGMResponse CDevice::executeMGMCommand(forte::core::SManagementCMD &pa_oCommand){
  EMGMResponse retval = e_INVALID_DST;

  if(CStringDictionary::scm_nInvalidStringId == pa_oCommand.mDestination){
    retval = CResource::executeMGMCommand(pa_oCommand);
  }
  else{
    CResource *res = static_cast<CResource *>(CFBContainer::getFB(pa_oCommand.mDestination));
    if(0 != res){
      pa_oCommand.mDestination = CStringDictionary::scm_nInvalidStringId;
      retval = res->executeMGMCommand(pa_oCommand);
    }
  }
  return retval;
}
