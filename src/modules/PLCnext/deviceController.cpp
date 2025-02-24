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

#include "core/util/criticalregion.h"
#include "deviceController.h"
#include "slaveHandle/bitSlaveHandle.h"
#include "slaveHandle/slaveHandle.h"

PLCnextDeviceController::~PLCnextDeviceController() {
}

void PLCnextDeviceController::setConfig(struct forte::core::io::IODeviceController::Config* paConfig) {
    if (isAlive()) {
        DEVLOG_ERROR("PLCnext[PLCnextDeviceController]: Cannot change configuration while running.\n");
        return;
    }

    this->mConfig = *static_cast<PLCnextConfig*>(paConfig);
}

const char* PLCnextDeviceController::init() {
    return 0;
}

void PLCnextDeviceController::deInit() {
    TSlaveList::Iterator itEnd(mSlaves->end());
    for (TSlaveList::Iterator it = mSlaves->begin(); it != itEnd; ++it) {
        delete* it;
    }
}

void PLCnextDeviceController::registerSlaveHandler(PLCnextSlaveHandler *slave) {
    mSlaves->pushBack(slave);
}

forte::core::io::IOHandle* PLCnextDeviceController::createIOHandle(forte::core::io::IODeviceController::HandleDescriptor &paHandleDescriptor) {
    HandleDescriptor& desc = static_cast<HandleDescriptor&>(paHandleDescriptor);
    PLCnextSlaveHandler* slave = getSlave(desc.mSlaveIndex);

    if (slave == 0) {
        DEVLOG_ERROR("slave with index %d not found.\n", desc.mSlaveIndex);
        return 0;
    }

    switch (desc.mType) {
    case Bit:
        return new PLCnextBitSlaveHandle(this, desc.mDirection, desc.mPosition, slave);
    case BitString16:
        return 0; // TODO: implement AnalogSlaveHandle!
    }

    return 0;
}

void PLCnextDeviceController::runLoop() {
    while (isAlive()) {
        TSlaveList::Iterator itEnd = mSlaves->end();
        for (TSlaveList::Iterator it = mSlaves->begin(); it != itEnd; ++it) {
            (*it)->update();
        }

        CThread::sleepThread(mConfig.updateInterval);
    }
}

PLCnextSlaveHandler* PLCnextDeviceController::getSlave(int paIndex) {
    if (mSlaves == 0) {
        DEVLOG_ERROR("no slaves are registered.");
        return 0;
    }

    TSlaveList::Iterator itEnd = mSlaves->end();
    int i = 0;
    for (TSlaveList::Iterator it = mSlaves->begin(); it != itEnd; ++it, i++) {
        if (paIndex == i) {
            return *it;
        }
    }

    return 0;
}

void PLCnextDeviceController::addSlaveHandle(int paIndex, forte::core::io::IOHandle* paHandle) {
    PLCnextSlaveHandler* slave = getSlave(paIndex);
    if (slave == 0) {
        DEVLOG_ERROR("no slaves are registered.");
        return;
    }

    slave->addHandle((PLCnextSlaveHandle*)paHandle);
}

void PLCnextDeviceController::dropSlaveHandles(int paIndex) {
    PLCnextSlaveHandler* slave = getSlave(paIndex);
    if (slave == 0) {
        DEVLOG_ERROR("no slaves are registered.");
        return;
    }

    slave->dropHandles();
}

bool PLCnextDeviceController::isSlaveAvailable(int paIndex) {
    // (currently) don't check if index is really available at initialization;
    return true;
}

bool PLCnextDeviceController::checkSlaveType(int paIndex, int paType) {
    // (currently) don't check slaveType at initialization;
    return true;
}
