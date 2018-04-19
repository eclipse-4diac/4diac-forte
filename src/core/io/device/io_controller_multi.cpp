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

#include "io_controller_multi.h"

IODeviceMultiController::IODeviceMultiController(CDeviceExecution& paDeviceExecution) : IODeviceController(paDeviceExecution){
}

void IODeviceMultiController::addHandle(
    IODeviceController::HandleDescriptor *handleDescriptor) {
  HandleDescriptor* desc = static_cast<HandleDescriptor*>(handleDescriptor);
  IOHandle* handle = initHandle(desc);

  if (handle == 0) {
    DEVLOG_WARNING(
        "[IODeviceMultiController] Failed to initialize handle '%s'. Check initHandle method.\n",
        desc->id.getValue());
    return;
  }

  if (IOMapper::getInstance().registerHandle(desc->id, handle)) {
    addSlaveHandle(desc->slaveIndex, handle);
  } else {
    delete handle;
  }
}

