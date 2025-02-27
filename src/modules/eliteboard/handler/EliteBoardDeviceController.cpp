/*******************************************************************************
 * Copyright (c) 2021, 2022 Jonathan Lainer (kontakt@lainer.co.at)
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Jonathan Lainer - Initial implementation.
 *******************************************************************************/

#include "EliteBoardDeviceController.h"
#include "../handler/EliteBoardDeviceController.h"
#include "../handle/PinHandle.h"

DEFINE_HANDLER(EliteBoardDeviceController);

EliteBoardDeviceController::EliteBoardDeviceController(CDeviceExecution& paDeviceExecution)
    : forte::core::io::IODeviceController(paDeviceExecution){
}

EliteBoardDeviceController::~EliteBoardDeviceController() {}

EliteBoardDeviceController::IOHandle *EliteBoardDeviceController::createIOHandle(HandleDescriptor &paHandleDescriptor) {
  auto desc = static_cast<EliteBoardHandleDescriptor &>(paHandleDescriptor);
  IOHandle *handle = new IOHandleGPIO(this, desc.mGPIO_Port, desc.mPin);
  return static_cast<IOHandle *>(handle);
}
