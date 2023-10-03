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


#include "Arp/System/ModuleLib/Module.h"
#include "Arp/System/Commons/Logging.h"
#include "Arp/Plc/AnsiC/Gds/DataLayout.h"
#include "Arp/Plc/AnsiC/Io/FbIoSystem.h"
#include "Arp/Plc/AnsiC/Io/Axio.h"
#include "Arp/Plc/AnsiC/Domain/PlcOperationHandler.h"
#include "Arp/System/Rsc/ServiceManager.hpp"
#include "Arp/Device/Interface/Services/IDeviceStatusService.hpp"

//#include <syslog.h>
#include <unistd.h>
#include <libgen.h>
#include <thread>
#include <pthread.h>
#include <criticalregion.h>
#include <devlog.h>

#include "plcNextDeviceInterface.h"
#include "plcNextDeviceStatus.h"

using namespace Arp;
using namespace Arp::System::Rsc;
using namespace Arp::Device::Interface::Services;
using namespace Arp::System::Commons::Diagnostics::Logging;


const char* PLCnextDeviceInterface::init(int nodeId, const char* pinName) {
    // initialize buffers
    this->nodeId = nodeId;
    this->pinName = pinName;
    this->fqPinIdentifier = busIdentifier + "/" + std::to_string(this->nodeId) + "." + pinName;

    DEVLOG_INFO("[PLCnextDeviceInterface] initialize %s", fqPinIdentifier.c_str());

    if (!ArpPlcIo_GetBufferPtrByPortName(busIdentifier.c_str(), fqPinIdentifier.c_str(), &gdsBuffer)) {
        DEVLOG_ERROR("[PLCnextDeviceInterface] ArpPlcIo_GetBufferPtrByPortName failed\n");
        return "ArpPlcIo_GetBufferPtrByPortName failed";
    }

    if (!ArpPlcGds_GetVariableOffset(gdsBuffer, fqPinIdentifier.c_str(), &offset)) {
        DEVLOG_ERROR("[PLCnextDeviceInterface] ArpPlcGds_GetVariableOffset failed");
        return "ArpPlcGds_GetVariableOffset failed";
    }

    // set PLC to ready
    DeviceStatus::ready = true;

    return 0; // successfull
}

PLCnextDeviceInterface::~PLCnextDeviceInterface() {
    if (gdsBuffer != NULL && !ArpPlcIo_ReleaseGdsBuffer(gdsBuffer)) {
        DEVLOG_ERROR("[PLCnextDeviceInterface] ArpPlcIo_ReleaseGdsBuffer failed");
    }
}

void PLCnextDeviceInterface::read(char* value, size_t valueSize) {

    if(!DeviceStatus::isReady()) {
        return;
    }

    // Begin read operation, memory buffer will be locked
    char* readDataBufferPage;
    if (!ArpPlcGds_BeginRead(gdsBuffer, &readDataBufferPage)) {
        DEVLOG_ERROR("[PLCnextDeviceInterface] ArpPlcGds_BeginRead failed");
        ArpPlcGds_EndRead(gdsBuffer);
        return;
    }

    // Copy data from GDS buffer
    char* dataAddress = readDataBufferPage + offset;
    memcpy(value, dataAddress, valueSize);

    // Unlock buffer
    ArpPlcGds_EndRead(gdsBuffer);
}

void PLCnextDeviceInterface::write(char* value, size_t valueSize) {

    if(!DeviceStatus::isReady()) {
        return;
    }

    // Begin write operation, memory buffer will be locked
    char* dataBufferPage = NULL;

    if (!ArpPlcGds_BeginWrite(gdsBuffer, &dataBufferPage)) {
        DEVLOG_ERROR("[PLCnextDeviceInterface] ArpPlcGds_BeginWrite failed");
        ArpPlcGds_EndWrite(gdsBuffer);
        return;
    }

    // Copy data to GDS buffer
    char* dataAddress = dataBufferPage + offset;
    memcpy(dataAddress, value, valueSize);

    ArpPlcGds_EndWrite(gdsBuffer);
}
