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

#include <libgen.h>
#include <devlog.h>
#include "plcNextDeviceStatus.h"

using namespace Arp;
using namespace Arp::System::Rsc;
using namespace Arp::Device::Interface::Services;
using namespace Arp::System::Commons::Diagnostics::Logging;

volatile bool DeviceStatus::started = false;
volatile bool DeviceStatus::ready = false;

bool DeviceStatus::isReady() {
    return DeviceStatus::started && DeviceStatus::ready;
}

bool DeviceStatus::startup() {

    // register callback function to get information when plc is ready
    ArpPlcDomain_SetHandler(DeviceStatus::plcCallbackOperationHandler);

    /*
     * "forte.acf.settings" - File
     * need to be in the same directory as forte executable!!!
     */

    // get path of current executable
    char szExePath[PATH_MAX];
    ssize_t count = readlink("/proc/self/exe", szExePath, PATH_MAX);
    string strDirPath;

    if (count != -1) {
        strDirPath = dirname(szExePath);
    } else {
        return false;
    }

    // load acf.settings file
    string strSettingsFile(strDirPath + "/forte.acf.settings");

     /*
      * Intialize PLCnext module application
      * Arguments:
      *     arpBinaryDir:    Path to Arp binaries
      *        applicationName: Arbitrary Name of Application
      *        acfSettingsPath: Path to *.acf.settings document to set application up
      */

     if (ArpSystemModule_Load("/usr/lib", "forte", strSettingsFile.c_str()) != 0)
     {
         // ** FATAL ERROR **: ARP module could not be loaded
         return false;
     }

    // wait until plc is started and able to handle a connection
     while(!DeviceStatus::started) { /* wait until callback */ }

     DEVLOG_INFO("[DeviceStatus] PLCnext is ready! \n");
     Log::Info("[DeviceStatus] PLCnext is ready");

     return true;
}


// Callback function to get PLC state
void DeviceStatus::plcCallbackOperationHandler(enum PlcOperation operation)
{
    switch (operation)
    {
        case PlcOperation_Load:
        case PlcOperation_Setup:
        case PlcOperation_None:
            break;

        case PlcOperation_StartCold:
        case PlcOperation_StartWarm:
        case PlcOperation_StartHot:
            started = true;
            break;

        case PlcOperation_Stop:
        case PlcOperation_Reset:
        case PlcOperation_Unload:
            started = false;
            break;

        default:
            break;
    }
}

#ifdef FORTE_EXTERNAL_LOG_HANDLER
/*
 * Adapter to use 4diac logger
 * map DEVLOG_XX calls to LOG::XX calls
 */
void logMessage(E_MsgLevel paLevel, const char *paMessage, ...) {
    switch(paLevel) {
    case E_MsgLevel::E_INFO:
        Log::Info(paMessage);
        break;
    case E_MsgLevel::E_WARNING:
        Log::Warning(paMessage);
        break;
    case E_MsgLevel::E_ERROR:
        Log::Error(paMessage);
        break;
    case E_MsgLevel::E_DEBUG:
        Log::Debug(paMessage);
        break;
    case E_MsgLevel::E_TRACE:
        Log::Trace(paMessage);
        break;
    }
};
#endif /* FORTE_EXTERNAL_LOG_HANDLER */


