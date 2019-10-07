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

#include "io_controller_multi.h"

using namespace forte::core::io;

IODeviceMultiController::IODeviceMultiController(CDeviceExecution& paDeviceExecution) :
    IODeviceController(paDeviceExecution) {
}

void IODeviceMultiController::addHandle(IODeviceController::HandleDescriptor *paHandleDescriptor) {
  HandleDescriptor* desc = static_cast<HandleDescriptor*>(paHandleDescriptor);
  IOHandle* handle = initHandle(desc);

  if(0 == handle) {
    DEVLOG_WARNING("[IODeviceMultiController] Failed to initialize handle '%s'. Check initHandle method.\n", desc->mId.getValue());
    return;
  }

  if(IOMapper::getInstance().registerHandle(desc->mId, handle)) {
    addSlaveHandle(desc->mSlaveIndex, handle);
  } else {
    delete handle;
  }
}

