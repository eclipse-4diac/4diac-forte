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

#include "slaveHandler.h"
#include "deviceController.h"
#include <io/mapper/io_mapper.h>

const TForteUInt8 PLCnextSlaveHandler::scmSlaveConfigurationIO[] = { };
const TForteUInt8 PLCnextSlaveHandler::scmSlaveConfigurationIONum = 0;

PLCnextSlaveHandler::PLCnextSlaveHandler(int paType, CResource* paSrcRes, const SFBInterfaceSpec* paInterfaceSpec, 
    const CStringDictionary::TStringId paInstanceNameId) :
    IOConfigFBMultiSlave(scmSlaveConfigurationIO, scmSlaveConfigurationIONum, paType, paSrcRes, paInterfaceSpec),
    slaveType(SlaveType(paType)) {
}


PLCnextSlaveHandler::~PLCnextSlaveHandler() {
    delete imageBuffer;
}

int PLCnextSlaveHandler::update() {
    if (slaveType == SlaveType::Input) {
        plcNextDevice.read(this->imageBuffer, this->imageSize);
    }
    else if (slaveType == SlaveType::Output) {
        plcNextDevice.write(this->imageBuffer, this->imageSize);
    }
    else if (slaveType == SlaveType::NoUsage) {
        // PLCnext contains a "placeholder" module
        // for this module, nothing should happen
    }
    else {
        DEVLOG_ERROR("[PLCnextSlaveHandler] SlaveHander not correct initialized.\n");
    }
}

void PLCnextSlaveHandler::dropHandles() {
    CCriticalRegion criticalRegion(handleMutex);

    forte::core::io::IOMapper& mapper = forte::core::io::IOMapper::getInstance();

    TSlaveHandleList::Iterator itEnd = slaveHandles->end();
    for(TSlaveHandleList::Iterator it = slaveHandles->begin(); it != itEnd; ++it) {
        mapper.deregisterHandle(*it);
        delete *it;
    }
}

void PLCnextSlaveHandler::initBufferImage(size_t size)
{
    this->imageSize = size;
    this->imageBuffer = new char[imageSize];

    memset(this->imageBuffer, 0, size);
}

void PLCnextSlaveHandler::addHandle(PLCnextSlaveHandle* paHandle) {
    CCriticalRegion criticalRegion(handleMutex);
    slaveHandles->pushBack(paHandle);
}

PLCnextSlaveHandle* PLCnextSlaveHandler::getHandle(int paIndex) {
    TSlaveHandleList::Iterator itEnd = slaveHandles->end();
    
    int i = 0;
    for(TSlaveHandleList::Iterator it = slaveHandles->begin(); it != itEnd; ++it, i++) {
        if(paIndex == i) {
            return *it;
        }
    }
    return NULL;
}

