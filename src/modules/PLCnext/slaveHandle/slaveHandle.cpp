/*******************************************************************************
 * Copyright (c) 2022 Peirlberger Juergen
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Peirlberger Juergen - initial API and implementation and/or initial documentation
 *******************************************************************************/

#include "slaveHandle.h"
#include <io/mapper/io_mapper.h>
#include "../slaveHandler.h"
#include <devlog.h>

PLCnextSlaveHandle::PLCnextSlaveHandle(forte::core::io::IODeviceController* paController, forte::core::io::IOMapper::Direction paDirection,
    CIEC_ANY::EDataTypeID paType, PLCnextSlaveHandler *paSlave) :
    forte::core::io::IOHandle(paController, paDirection, paType), mSlave(paSlave),
    mOffset(paSlave->imageOffset) {

}

PLCnextSlaveHandle::~PLCnextSlaveHandle() {}

void PLCnextSlaveHandle::onObserver(forte::core::io::IOObserver* paObserver) {
    reset();
    forte::core::io::IOHandle::onObserver(paObserver);
}

void PLCnextSlaveHandle::dropObserver() {
    forte::core::io::IOHandle::dropObserver();
    reset();
}
