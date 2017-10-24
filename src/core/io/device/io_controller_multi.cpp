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

namespace IO {
namespace Device {

MultiController::MultiController() {
}

void MultiController::addHandle(
    Controller::HandleDescriptor *handleDescriptor) {
  HandleDescriptor* desc = static_cast<HandleDescriptor*>(handleDescriptor);
  Handle* handle = initHandle(desc);

  if (handle == 0) {
    DEVLOG_WARNING(
        "[IO:Device:MultiController] Failed to initialize handle '%s'. Check initHandle method.\n",
        desc->id.getValue());
    return;
  }

  if (Mapper::getInstance().registerHandle(desc->id, handle)) {
    addSlaveHandle(desc->slaveIndex, handle);
  } else {
    delete handle;
  }
}

} /* namespace Device */
} /* namespace IO */
